//
// Created by Jonas Zell on 28.05.18.
//

#include "ASTReader.h"

#include "ASTReaderInternals.h"
#include "Basic/FileManager.h"
#include "Basic/NestedNameSpecifier.h"
#include "BitCodes.h"
#include "ILReader.h"
#include "IL/Module.h"
#include "IncrementalCompilation.h"
#include "ModuleFile.h"
#include "ModuleReader.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Sema/SemaPass.h"

#include <llvm/ADT/StringExtras.h>
#include <llvm/Support/SaveAndRestore.h>
#include <Support/Format.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;
using namespace cdot::serial::reader;

std::pair<unsigned, unsigned>
ASTDeclContextNameLookupTrait::ReadKeyDataLength(const unsigned char *&d)
{
   using namespace llvm::support;

   unsigned KeyLen = endian::readNext<uint16_t, little, unaligned>(d);
   unsigned DataLen = endian::readNext<uint16_t, little, unaligned>(d);
   return std::make_pair(KeyLen, DataLen);
}

ASTDeclContextNameLookupTrait::internal_key_type
ASTDeclContextNameLookupTrait::ReadKey(const unsigned char *d, unsigned)
{
   using namespace llvm::support;

   auto Kind = (DeclarationName::DeclarationKind)*d++;

   DeclarationName Result;
   auto &Tbl = Reader.getContext().getDeclNameTable();

   switch (Kind) {
   case DeclarationName::NormalIdentifier:
   case DeclarationName::InfixOperatorName:
   case DeclarationName::PrefixOperatorName:
   case DeclarationName::PostfixOperatorName:
   case DeclarationName::MacroName: {
      auto *II = Reader.getLocalIdentifier(
         endian::readNext<uint32_t, little, unaligned>(d));

      Result = Tbl.getIdentifiedName(Kind, *II);
      break;
   }
   case DeclarationName::InstantiationName: {
      Result = ReadKey(d, 0);
      break;
   }
   case DeclarationName::LocalVarName: {
      auto *II = Reader.getLocalIdentifier(
         endian::readNext<uint32_t, little, unaligned>(d));
      auto ScopeID = endian::readNext<uint32_t, little, unaligned>(d);

      Result = Tbl.getLocalVarName(II, ScopeID);
      break;
   }
   case DeclarationName::OperatorDeclName: {
      auto Kind = (DeclarationName::DeclarationKind)
         endian::readNext<uint8_t, little, unaligned>(d);

      auto &II = *Reader.getLocalIdentifier(
         endian::readNext<uint32_t, little, unaligned>(d));

      switch (Kind) {
      case DeclarationName::InfixOperatorName:
         Result = Tbl.getOperatorDeclName(Tbl.getInfixOperatorName(II));
         break;
      case DeclarationName::PrefixOperatorName:
         Result = Tbl.getOperatorDeclName(Tbl.getPrefixOperatorName(II));
         break;
      case DeclarationName::PostfixOperatorName:
         Result = Tbl.getOperatorDeclName(Tbl.getPostfixOperatorName(II));
         break;
      default:
         llvm_unreachable("bad name kind");
      }

      break;
   }
   case DeclarationName::ConversionOperatorName:
   case DeclarationName::ExtensionName:
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
   case DeclarationName::DestructorName: {
      QualType Ty = Reader.getLocalType(
         endian::readNext<uint32_t, little, unaligned>(d));

      Result = Tbl.getTypedName(Kind, Ty);
      break;
   }
   case DeclarationName::PackExpansionName: {
      auto *II = Reader.getLocalIdentifier(
         endian::readNext<uint32_t, little, unaligned>(d));
      auto Idx = endian::readNext<uint32_t, little, unaligned>(d);

      Result = Tbl.getPackExpansionName(II, Idx);
      break;
   }
   case DeclarationName::AccessorName: {
      auto *II = Reader.getLocalIdentifier(
         endian::readNext<uint32_t, little, unaligned>(d));
      auto AccKind = static_cast<DeclarationName::AccessorKind>(
         endian::readNext<uint8_t, little, unaligned>(d));

      Result = Tbl.getAccessorName(*II, AccKind);
      break;
   }
   case DeclarationName::ClosureArgumentName: {
      auto Idx = endian::readNext<uint32_t, little, unaligned>(d);
      Result = Tbl.getClosureArgumentName(Idx);

      break;
   }
   case DeclarationName::ErrorName:
      Result = Tbl.getErrorName();
      break;
   case DeclarationName::SubscriptName: {
      auto SubKind = static_cast<DeclarationName::SubscriptKind >(
         endian::readNext<uint8_t, little, unaligned>(d));

      Result = Tbl.getSubscriptName(SubKind);
      break;
   }
   default:
      llvm_unreachable("bad name kind");
   }

   return Result;
}

void ASTDeclContextNameLookupTrait::ReadDataInto(internal_key_type,
                                                 const unsigned char *d,
                                                 unsigned DataLen,
                                                 data_type_builder &Val) {
   using namespace llvm::support;

   for (unsigned NumDecls = DataLen / 4; NumDecls; --NumDecls) {
      uint32_t LocalID = endian::readNext<uint32_t, little, unaligned>(d);
      Val.insert(LocalID);
   }
}

bool ASTReader::ReadLexicalDeclContextStorage(uint64_t Offset, DeclContext *DC)
{
   assert(Offset != 0);

   SavedStreamPosition SavedPosition(DeclsCursor);
   DeclsCursor.JumpToBit(Offset);

   RecordData Record;
   StringRef Blob;

   unsigned Code = DeclsCursor.ReadCode();
   unsigned RecCode = DeclsCursor.readRecord(Code, Record, &Blob);

   if (RecCode != DECL_CONTEXT_LEXICAL) {
      Error("Expected lexical block");
      return true;
   }

   auto IDs = reinterpret_cast<const uint32_t*>(Blob.data());
   auto NumDecls = Record[0];

   for (unsigned i = 0; i < NumDecls; ++i) {
      auto NextDeclID = *IDs++;
      if (IsDeclLoaded(NextDeclID)) {
         auto *D = GetDecl(NextDeclID);
         if (!D)
            continue;

         addDeclToContext(D, DC);
      }
      else {
         DeclContextMap[NextDeclID] = DC;
      }
   }

   return false;
}

bool ASTReader::ReadVisibleDeclContextStorage(uint64_t Offset,
                                              ast::DeclContext *DC,
                                              unsigned ID) {
   assert(Offset != 0);

   if (!DC->isPrimaryCtx()) {
      assert(isa<ModuleDecl>(DC) && "non-primary context with lookup!");
      DC = DC->getPrimaryCtx();
   }

   assert(!DC->getModFile() && "duplicate module file");

   SavedStreamPosition SavedPosition(DeclsCursor);
   DeclsCursor.JumpToBit(Offset);

   RecordData Record;
   StringRef Blob;

   unsigned Code = DeclsCursor.ReadCode();
   unsigned RecCode = DeclsCursor.readRecord(Code, Record, &Blob);

   if (RecCode != DECL_CONTEXT_VISIBLE) {
      Error("Expected visible lookup table block");
      return true;
   }

   auto TblOffset = Record[0];
   auto *Data = (const unsigned char*)Blob.data();

   auto Buckets = Data + TblOffset;
   auto *Tbl = HashTable::Create(Buckets, Data + 1, Data,
                                 ASTDeclContextNameLookupTrait(*this));

   Lookups[DC].Table = Tbl;
   DC->setModFile(new(Sema.getContext()) ModuleFile(Reader, Tbl));

   if (DC == Reader.Mod->getDecl()) {
      Reader.ModTbl = Tbl;
   }

   return false;
}

void ASTReader::Error(llvm::StringRef Msg) const
{
   llvm::report_fatal_error(Msg);
}

void ASTReader::Error(unsigned DiagID, llvm::StringRef Arg1,
                      llvm::StringRef Arg2) const {
   Sema.diagnose((diag::MessageKind)DiagID, Arg1, Arg2);
}

unsigned ASTReader::TypeCursorForIndex(unsigned ID)
{
   return TypeOffsets[ID - BaseTypeIndex];
}

QualType ASTReader::readTypeRecord(unsigned ID)
{
   ++Reader.NumTypesRead;
   unsigned Loc = TypeCursorForIndex(ID);

   // Keep track of where we are in the stream, then jump back there
   // after reading this type.
   SavedStreamPosition SavedPosition(DeclsCursor);

   ReadingKindTracker ReadingKind(Read_Type, *this);

   unsigned Idx = 0;
   DeclsCursor.JumpToBit(Loc);

   auto &Context = Reader.CI.getContext();
   RecordData Record;
   unsigned Code = DeclsCursor.ReadCode();

   auto TypeID = (Type::TypeID)DeclsCursor.readRecord(Code, Record);
   switch (TypeID) {
   case Type::ArrayTypeID: {
      if (Record.size() != 2) {
         Error("bad array type encoding");
         return QualType();
      }

      auto NumElements = Record[Idx++];
      QualType ElementTy = readType(Record, Idx);

      return Context.getArrayType(ElementTy, NumElements);
   }
   case Type::AssociatedTypeID: {
      if (Record.size() != 1) {
         Error("bad associated type encoding");
         return QualType();
      }

      auto *AD = ReadDeclAs<AssociatedTypeDecl>(Record, Idx);
      return Context.getAssociatedType(AD);
   }
   case Type::BoxTypeID: {
      if (Record.size() != 1) {
         Error("bad box type encoding");
         return QualType();
      }

      auto BoxedTy = readType(Record, Idx);
      return Context.getBoxType(BoxedTy);
   }
   case Type::TokenTypeID:
      return Context.getTokenType();
   case Type::BuiltinTypeID: {
      if (Record.size() != 1) {
         Error("bad builtin type encoding");
         return QualType();
      }

      auto Kind = static_cast<Type::BuiltinKind>(Record[Idx++]);
      return Context.getBuiltinType(Kind);
   }
   case Type::DependentRecordTypeID: {
      auto *R = ReadDeclAs<RecordDecl>(Record, Idx);
      auto *Args = ReadTemplateArgumentList(Record, Idx);

      return Context.getDependentRecordType(R, Args);
   }
   case Type::DependentSizeArrayTypeID: {
      auto *SizeExpr = ReadExpr();
      QualType ElementTy = readType(Record, Idx);

      return Context.getValueDependentSizedArrayType(ElementTy,
                                                     cast<StaticExpr>(SizeExpr));
   }
   case Type::DependentNameTypeID: {
      auto *NameSpec = ReadNestedNameSpecWithLoc(Record, Idx);
      return Context.getDependentNameType(NameSpec);
   }
   case Type::FunctionTypeID:
   case Type::LambdaTypeID: {
      QualType RetTy = readType(Record, Idx);

      SmallVector<QualType, 4> Params;
      SmallVector<FunctionType::ParamInfo, 4> ParamInfo;

      unsigned NumParams = Record[Idx++];
      for (unsigned i = 0; i < NumParams; ++i) {
         Params.push_back(readType(Record, Idx));
         ParamInfo.emplace_back(static_cast<ArgumentConvention>(Record[Idx++]));
      }

      unsigned RawFlags = Record[Idx++];
      return Context.getFunctionType(RetTy, Params, ParamInfo, RawFlags,
                                     TypeID == Type::LambdaTypeID);
   }
   case Type::GenericTypeID: {
      if (Record.size() != 1) {
         Error("bad generic type encoding");
         return QualType();
      }

      auto *PD = ReadDeclAs<TemplateParamDecl>(Record, Idx);
      return Context.getTemplateArgType(PD);
   }
   case Type::InferredSizeArrayTypeID: {
      if (Record.size() != 1) {
         Error("bad array type encoding");
         return QualType();
      }

      QualType ElementTy = readType(Record, Idx);
      return Context.getInferredSizeArrayType(ElementTy);
   }
   case Type::MetaTypeID: {
      if (Record.size() != 1) {
         Error("bad meta type encoding");
         return QualType();
      }

      auto Ty = readType(Record, Idx);
      return Context.getMetaType(Ty);
   }
   case Type::MutableBorrowTypeID: {
      if (Record.size() != 1) {
         Error("bad reference type encoding");
         return QualType();
      }

      auto Ty = readType(Record, Idx);
      return Context.getMutableBorrowType(Ty);
   }
   case Type::MutableReferenceTypeID: {
      if (Record.size() != 1) {
         Error("bad reference type encoding");
         return QualType();
      }

      auto Ty = readType(Record, Idx);
      return Context.getMutableReferenceType(Ty);
   }
   case Type::MutablePointerTypeID: {
      if (Record.size() != 1) {
         Error("bad pointer type encoding");
         return QualType();
      }

      auto Ty = readType(Record, Idx);
      return Context.getMutablePointerType(Ty);
   }
   case Type::PointerTypeID: {
      if (Record.size() != 1) {
         Error("bad pointer type encoding");
         return QualType();
      }

      auto Ty = readType(Record, Idx);
      return Context.getPointerType(Ty);
   }
   case Type::RecordTypeID: {
      if (Record.size() != 1) {
         Error("bad record type encoding");
         return QualType();
      }

      auto *RD = ReadDeclAs<RecordDecl>(Record, Idx);
      if (!RD)
         return QualType();

      return Context.getRecordType(RD);
   }
   case Type::ReferenceTypeID: {
      if (Record.size() != 1) {
         Error("bad reference type encoding");
         return QualType();
      }

      auto Ty = readType(Record, Idx);
      return Context.getReferenceType(Ty);
   }
   case Type::TupleTypeID: {
      SmallVector<QualType, 4> Tys;

      unsigned NumTys = Record[Idx++];
      for (unsigned i = 0; i < NumTys; ++i) {
         Tys.push_back(readType(Record, Idx));
      }

      return Context.getTupleType(Tys);
   }
   case Type::TypedefTypeID: {
      if (Record.size() != 1) {
         Error("bad typedef type encoding");
         return QualType();
      }

      auto *RD = ReadDeclAs<AliasDecl>(Record, Idx);
      auto Ty = Context.getTypedefType(RD);
      Ty->setCanonicalType(RD->getType()->asMetaType()->getUnderlyingType());

      return Ty;
   }
   }
}

QualType ASTReader::GetType(unsigned ID)
{
   if (!ID)
      return QualType();

   unsigned Index = ID - BaseTypeIndex;
   QualType &Ty = TypesLoaded[Index];
   if (Ty.isNull()) {
      Ty = readTypeRecord(ID);
   }

   return Ty;
}

QualType ASTReader::getLocalType(unsigned LocalID)
{
   return GetType(LocalID);
}

IdentifierInfo* ASTReader::GetIdentifierInfo(const RecordData &Record,
                                             unsigned &Idx) {
   return Reader.GetIdentifierInfo(Record, Idx);
}

IdentifierInfo* ASTReader::getLocalIdentifier(unsigned ID)
{
   return Reader.getLocalIdentifier(ID);
}

Scope* ASTReader::getScope(unsigned ID)
{
   if (!ID)
      return nullptr;

   if (ID + 1 > LoadedScopes.size())
      LoadedScopes.resize(ID + 1);

   auto &S = LoadedScopes[ID];
   if (!S) {
      S = ReadScopeRecord(ID);
   }

   return S;
}

lex::Token ASTReader::ReadToken(const RecordDataImpl &Record, unsigned &Idx)
{
   using namespace cdot::lex;

   auto Loc = ReadSourceLocation(Record, Idx);
   auto Kind = static_cast<tok::TokenType>(Record[Idx++]);

   switch (Kind) {
   case tok::ident:
   case tok::op_ident:
   case tok::dollar_ident:
   case tok::macro_name: {
      auto *II = getLocalIdentifier(Record[Idx++]);
      return Token(II, Loc, Kind);
   }
   case tok::space: {
      auto NumSpaces = Record[Idx++];
      return Token(Token::Space, NumSpaces, Loc);
   }
   case tok::charliteral:
   case tok::stringliteral:
   case tok::fpliteral:
   case tok::integerliteral: {
      auto Size = Record[Idx++];
      char *Ptr = static_cast<char*>(Context.Allocate(Size, 1));

      for (unsigned i = 0; i < Size; ++i) {
         *Ptr++ = static_cast<char>(Record[Idx++]);
      }

      return Token(Ptr, Size, Kind, Loc);
   }
   default:
      return Token(Kind, Loc);
   }
}

ConformanceLookupTrait::internal_key_type
ConformanceLookupTrait::ReadKey(const unsigned char *d, unsigned n)
{
   using namespace llvm::support;
   return endian::readNext<uint32_t, little, unaligned>(d);
}

ConformanceLookupTrait::data_type
ConformanceLookupTrait::ReadData(const internal_key_type &key,
                                 const unsigned char *d,
                                 unsigned) {
   using namespace llvm::support;

   auto *R = cast<RecordDecl>(Reader.GetDecl(key));
   auto Offset = endian::readNext<uint32_t, little, unaligned>(d);

   assert(Offset < Reader.ConformanceData.size() && "offset out of bounds!");
   auto *data = Reader.ConformanceData.data() + Offset;

   auto &Ctx = Reader.Reader.getCompilerInstance().getContext();
   auto &ConfTable = Ctx.getConformanceTable();

   auto NumConformances = endian::readNext<uint32_t, little, unaligned>(data);
   for (unsigned i = 0; i < NumConformances; ++i) {
      auto Kind = (ConformanceKind)endian::readNext<uint8_t, little, unaligned>(
         data);
      auto *Proto = cast<ProtocolDecl>(Reader.GetDecl(
         endian::readNext<uint32_t, little, unaligned>(data)));

      switch (Kind) {
      case ConformanceKind::Explicit:
         ConfTable.addExplicitConformance(Ctx, R, Proto);
         break;
      case ConformanceKind::Implicit:
         ConfTable.addImplicitConformance(Ctx, R, Proto);
         break;
      case ConformanceKind::Inherited:
         ConfTable.addInheritedConformance(Ctx, R, Proto);
         break;
      default:
         llvm_unreachable("bad conformance kind");
      }
   }
}

ReadResult ASTReader::ReadConformanceTable(RecordDataImpl &Record,
                                           StringRef Blob) {
   auto TblOffset = Record[0];
   auto *Data = (const unsigned char*)Blob.data();

   auto Buckets = Data + TblOffset;

   using Table = llvm::OnDiskChainedHashTable<ConformanceLookupTrait>;
   ConformanceTable = Table::Create(Buckets, Data,
                                    ConformanceLookupTrait(*this));

   return Success;
}

void ASTReader::ReadConformances(unsigned ID)
{
   if (!ConformanceTable)
      return;

   using Table = llvm::OnDiskChainedHashTable<ConformanceLookupTrait>;
   auto *Tbl = reinterpret_cast<Table*>(ConformanceTable);

   auto It = Tbl->find(ID);
   if (It == Tbl->end())
      return;

   ConformanceLookupTrait Trait(*this);
   Trait.ReadData(ID, It.getDataPtr(), It.getDataLen());
}

ReadResult ASTReader::ReadConformanceBlock(llvm::BitstreamCursor &Stream)
{
   if (Reader.ReadBlockAbbrevs(Stream, CONFORMANCE_BLOCK_ID)) {
      Error("malformed block record in module file");
      return Failure;
   }

   // Read all of the records and blocks for the AST file.
   RecordData Record;

   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("error at end of module block in module file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         return Success;
      case llvm::BitstreamEntry::SubBlock: {
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return Failure;
         }

         continue;
      }
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      // Read and process a record.
      Record.clear();
      StringRef Blob;

      auto RecordType =
         (ConformanceRecordTypes)Stream.readRecord(Entry.ID, Record, &Blob);

      switch (RecordType) {
      default:  // Default behavior: ignore.
         break;
      case CONFORMANCE_DATA: {
         ConformanceData = Blob;
         break;
      }
      case CONFORMANCE_TABLE: {
         auto Res = ReadConformanceTable(Record, Blob);
         if (Res != Success)
            return Res;

         break;
      }
      }
   }
}


InstantiationTableLookupTrait::hash_value_type
InstantiationTableLookupTrait::ComputeHash(const internal_key_type &a)
{
   return llvm::HashString(a);
}

std::pair<unsigned, unsigned>
InstantiationTableLookupTrait::ReadKeyDataLength(const unsigned char *&d)
{
   using namespace llvm::support;

   unsigned DataLen = endian::readNext<uint16_t, little, unaligned>(d);
   unsigned KeyLen = endian::readNext<uint16_t, little, unaligned>(d);

   return std::make_pair(KeyLen, DataLen);
}

InstantiationTableLookupTrait::internal_key_type
InstantiationTableLookupTrait::ReadKey(const unsigned char *d,
                                       unsigned n) {
   assert(n >= 2 && d[n-1] == '\0');
   return StringRef((const char*) d, n-1);
}

InstantiationTableLookupTrait::data_type
InstantiationTableLookupTrait::ReadData(const internal_key_type &k,
                                        const unsigned char *d,
                                        unsigned) {
   using namespace llvm::support;

   uint32_t ID = endian::readNext<uint32_t, little, unaligned>(d);
   return cast<NamedDecl>(Reader.GetDecl(ID));
}

ReadResult ASTReader::ReadInstantiationTable(RecordDataImpl &Record,
                                             StringRef Blob) {
   auto TblOffset = Record[0];
   auto *Data = (const unsigned char*)Blob.data();

   auto Buckets = Data + TblOffset;

   using Table =
      llvm::OnDiskIterableChainedHashTable<InstantiationTableLookupTrait>;
   InstantiationTable = Table::Create(Buckets, Data + 1, Data,
                                      InstantiationTableLookupTrait(*this));

   return Success;
}

ReadResult ASTReader::ReadASTBlock(llvm::BitstreamCursor &Stream)
{
   if (ModuleReader::ReadBlockAbbrevs(Stream, AST_BLOCK_ID)) {
      Error("malformed block record in AST file");
      return Failure;
   }

   // Read all of the records and blocks for the AST file.
   RecordData Record;

   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("error at end of module block in AST file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         return Success;
      case llvm::BitstreamEntry::SubBlock:
         switch (Entry.ID) {
         case DECL_TYPES_BLOCK_ID:
            // We lazily load the decls block, but we want to set up the
            // F->DeclsCursor cursor to point into it.  Clone our current bitcode
            // cursor to it, enter the block and read the abbrevs in that block.
            // With the main cursor, we just skip over it.
            DeclsCursor = Stream;
            if (Stream.SkipBlock() ||  // Skip with the main cursor.
                // Read the abbrevs.
                ModuleReader::ReadBlockAbbrevs(DeclsCursor,DECL_TYPES_BLOCK_ID)){
               Error("malformed block record in AST file");
               return Failure;
            }

            break;
         default:
            if (Stream.SkipBlock()) {
               Error("malformed block record in AST file");
               return Failure;
            }
            break;
         }

         continue;
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      // Read and process a record.
      Record.clear();
      StringRef Blob;

      auto RecordType =
         (ASTRecordTypes)Stream.readRecord(Entry.ID, Record, &Blob);

      switch (RecordType) {
      default:  // Default behavior: ignore.
         break;
      case GLOBAL_DECL_CONTEXT: {
         MainModuleID = (unsigned)Record[0];
         break;
      }
      case CACHE_LOOKUP_TABLE: {
         auto TblOffset = Record[0];
         auto *Data = (const unsigned char*)Blob.data();

         auto Buckets = Data + TblOffset;
         auto *Tbl = HashTable::Create(Buckets, Data + 1, Data,
                                       ASTDeclContextNameLookupTrait(*this));

//         Reader.Mod->setModFile(new(Sema.getContext()) ModuleFile(Reader, Tbl));
         Reader.ModTbl = Tbl;

         break;
      }
      case INSTANTIATION_TABLE: {
         auto Result = ReadInstantiationTable(Record, Blob);
         if (Result != Success)
            return Result;

         break;
      }
      case OPERATOR_PRECEDENCE_DECLS: {
         NumOperatorPrecedenceDecls = (unsigned)Record[0];
         OpPrecedenceData = reinterpret_cast<const uint32_t*>(Blob.data());

         break;
      }
      }
   }
}

void ASTReader::ReadDeclsEager(ArrayRef<unsigned> Decls)
{
   for (auto ID : Decls) {
      auto *NextDecl = GetDecl(ID);
      if (auto Op = dyn_cast<OperatorDecl>(NextDecl)) {
         Sema.ActOnOperatorDecl(Op);
      }
      else if (auto PG = dyn_cast<PrecedenceGroupDecl>(NextDecl)) {
         Sema.ActOnPrecedenceGroupDecl(PG);
      }
      else if (auto *DC = dyn_cast<DeclContext>(NextDecl)) {
         auto *MF = DC->getModFile();
         if (MF) {
            MF->LoadAllDecls(*DC, true);
            DC->setModFile(nullptr);
         }
      }
   }

   while (!LazyFnInfos.empty()) {
      auto &&Next = LazyFnInfos.front();
      LazyFnInfos.pop();

      Next.second->loadBody(Next.first);
   }
}

void ASTReader::ReadOperatorPrecedenceGroups()
{
   SemaPass::DeclScopeRAII DSR(Sema, Reader.Mod->getDecl());
   while (NumOperatorPrecedenceDecls--) {
      auto *NextDecl = GetDecl(*OpPrecedenceData++);
      NextDecl->setDeclared(false);

      if (auto Op = dyn_cast<OperatorDecl>(NextDecl)) {
         Sema.ActOnOperatorDecl(Op);
      }
      else if (auto PG = dyn_cast<PrecedenceGroupDecl>(NextDecl)) {
         Sema.ActOnPrecedenceGroupDecl(PG);
      }
      else {
         Sema.declareImportDecl(cast<ImportDecl>(NextDecl));
      }
   }
}

/// Given a cursor at the start of an AST file, scan ahead and drop the
/// cursor into the start of the given block ID, returning false on success and
/// true on failure.
LLVM_ATTRIBUTE_UNUSED
static bool SkipCursorToBlock(llvm::BitstreamCursor &Cursor, unsigned BlockID)
{
   while (true) {
      llvm::BitstreamEntry Entry = Cursor.advance();
      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
      case llvm::BitstreamEntry::EndBlock:
         return true;

      case llvm::BitstreamEntry::Record:
         // Ignore top-level records.
         Cursor.skipRecord(Entry.ID);
         break;

      case llvm::BitstreamEntry::SubBlock:
         if (Entry.ID == BlockID) {
            if (Cursor.EnterSubBlock(BlockID))
               return true;
            // Found it!
            return false;
         }

         if (Cursor.SkipBlock())
            return true;
      }
   }
}

Decl *ASTReader::GetDecl(unsigned ID)
{
   if (!ID)
      return nullptr;

   unsigned Index = ID - BaseDeclID;
   if (Index >= DeclsLoaded.size()) {
      Error("declaration ID out-of-range for AST file");
      return nullptr;
   }

   if (!DeclsLoaded[Index]) {
      ReadDeclRecord(ID);
   }

   return DeclsLoaded[Index];
}

unsigned ASTReader::ReadDeclID(const RecordData &Record, unsigned &Idx)
{
   if (Idx >= Record.size()) {
      Error("Corrupted AST file");
      return 0;
   }

   return Record[Idx++];
}

bool ASTReader::IsDeclLoaded(unsigned ID)
{
   if (!ID)
      return true;

   if (ID == MainModuleID)
      return true;

   unsigned Index = ID - BaseDeclID;
   if (Index >= DeclsLoaded.size()) {
      Error("declaration ID out-of-range for AST file");
      return false;
   }

   return DeclsLoaded[Index] != nullptr;
}

il::Constant* ASTRecordReader::readILConstant()
{
   return cast_or_null<il::Constant>(
      Reader->Reader.ILReader.GetValue(readInt()));
}

Scope* ASTRecordReader::ReadScope()
{
   return Reader->ReadScope(Record, Idx);
}

Module* ASTRecordReader::readModule()
{
   auto ID = (unsigned)readInt();
   if (ID) {
      return Reader->GetModule(ID);
   }

   class SmallVector<IdentifierInfo*, 4> ModuleName;
   auto NameSize = readInt();

   while (NameSize--) {
      ModuleName.push_back(getIdentifierInfo());
   }

   return Reader->getReader().getCompilerInstance().getModuleMgr()
                .GetModule(ModuleName);
}

Scope* ASTReader::ReadScope(const RecordDataImpl &Record, unsigned &Idx)
{
   return getScope(Record[Idx++]);
}

Module* ASTReader::GetModule(unsigned ID)
{
   auto It = Reader.Modules.find(ID);
   assert(It != Reader.Modules.end());

   return It->getSecond();
}

bool ASTReader::FindExternalVisibleDeclsByName(const DeclContext *DC,
                                               DeclarationName Name) {
   if (!Name)
      return false;

   auto It = Lookups.find(DC);
   if (It == Lookups.end())
      return false;

   // Load the list of declarations.
   SmallVector<NamedDecl *, 64> Decls;
   for (unsigned ID : *It->second.Table->find(Name)) {
      NamedDecl *ND = cast<NamedDecl>(GetDecl(ID));
      if (ND->getDeclName() == Name)
         Decls.push_back(ND);
   }

   return !Decls.empty();
}

DeclarationName ASTReader::ReadDeclarationName(const RecordData &Record,
                                               unsigned &Idx) {
   auto Kind = static_cast<DeclarationName::DeclarationKind>(Record[Idx++]);
   switch (Kind) {
   case DeclarationName::NormalIdentifier:
   case DeclarationName::InfixOperatorName:
   case DeclarationName::PrefixOperatorName:
   case DeclarationName::PostfixOperatorName:
   case DeclarationName::MacroName: {
      auto *II = GetIdentifierInfo(Record, Idx);
      return Context.getDeclNameTable().getIdentifiedName(Kind, *II);
   }
   case DeclarationName::OperatorDeclName:
      return Context.getDeclNameTable().getOperatorDeclName(
         ReadDeclarationName(Record, Idx));
   case DeclarationName::ConversionOperatorName:
   case DeclarationName::ExtensionName:
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
   case DeclarationName::DestructorName: {
      auto Ty = getLocalType(Record[Idx++]);
      return Context.getDeclNameTable().getTypedName(Kind, Ty);
   }
   case DeclarationName::PackExpansionName: {
      auto DN = ReadDeclarationName(Record, Idx);
      auto PackIdx = Record[Idx++];

      return Context.getDeclNameTable().getPackExpansionName(DN, PackIdx);
   }
   case DeclarationName::LocalVarName: {
      auto DN = ReadDeclarationName(Record, Idx);
      auto ScopeID = Record[Idx++];

      return Context.getDeclNameTable().getLocalVarName(DN, ScopeID);
   }
   case DeclarationName::AccessorName: {
      auto AccKind = static_cast<DeclarationName::AccessorKind>(Record[Idx++]);
      auto *II = GetIdentifierInfo(Record, Idx);

      return Context.getDeclNameTable().getAccessorName(*II, AccKind);
   }
   case DeclarationName::ClosureArgumentName: {
      auto ClosureIdx = Record[Idx++];
      return Context.getDeclNameTable().getClosureArgumentName(ClosureIdx);
   }
   case DeclarationName::InstantiationName: {
      auto DN = ReadDeclarationName(Record, Idx);
      auto Args = ReadTemplateArgumentList(Record, Idx);

      return Context.getDeclNameTable().getInstantiationName(DN, *Args);
   }
   case DeclarationName::SubscriptName: {
      auto SubKind = static_cast<DeclarationName::SubscriptKind>(Record[Idx++]);
      return Context.getDeclNameTable().getSubscriptName(SubKind);
   }
   case DeclarationName::ErrorName:
      return Context.getDeclNameTable().getErrorName();
   }
}

NestedNameSpecifier *ASTReader::ReadNestedNameSpec(const RecordData &Record,
                                                   unsigned &Idx) {
   bool HasPrev = Record[Idx++] != 0;
   NestedNameSpecifier *Previous = nullptr;
   if (HasPrev) {
      Previous = ReadNestedNameSpec(Record, Idx);
   }

   auto &Tbl = Context.getDeclNameTable();
   auto Kind = (NestedNameSpecifier::Kind)Record[Idx++];

   NestedNameSpecifier *Name;
   switch (Kind) {
   case NestedNameSpecifier::Type:
      Name = NestedNameSpecifier::Create(Tbl, readType(Record, Idx), Previous);
      break;
   case NestedNameSpecifier::Identifier:
      Name = NestedNameSpecifier::Create(Tbl, GetIdentifierInfo(Record, Idx),
                                         Previous);

      break;
   case NestedNameSpecifier::Namespace:
      Name = NestedNameSpecifier::Create(
         Tbl, ReadDeclAs<NamespaceDecl>(Record, Idx), Previous);
      break;
   case NestedNameSpecifier::TemplateParam:
      Name = NestedNameSpecifier::Create(
         Tbl, ReadDeclAs<TemplateParamDecl>(Record, Idx), Previous);
      break;
   case NestedNameSpecifier::AssociatedType:
      Name = NestedNameSpecifier::Create(
         Tbl, ReadDeclAs<AssociatedTypeDecl>(Record, Idx), Previous);
      break;
   case NestedNameSpecifier::Module:
      Name = NestedNameSpecifier::Create(Tbl, GetModule(Record[Idx++]),
                                         Previous);

      break;
   }

   return Name;
}

NestedNameSpecifierWithLoc*
ASTReader::ReadNestedNameSpecWithLoc(const RecordData &Record, unsigned &Idx)
{
   auto *Name = ReadNestedNameSpec(Record, Idx);
   auto NumLocs = Record[Idx++];

   SmallVector<SourceRange, 4> Locs;
   Locs.reserve(NumLocs);

   while (NumLocs--)
      Locs.push_back(ReadSourceRange(Record, Idx));

   return NestedNameSpecifierWithLoc::Create(Context.getDeclNameTable(),
                                             Name, Locs);
}

sema::TemplateArgument
ASTReader::ReadTemplateArgument(const RecordData &Record, unsigned &Idx)
{
   bool IsNull = Record[Idx++] != 0;
   bool IsType = Record[Idx++] != 0;
   bool IsVariadic = Record[Idx++] != 0;

   SourceLocation Loc = ReadSourceLocation(Record, Idx);
   auto *Param = ReadDeclAs<TemplateParamDecl>(Record, Idx);

   if (IsNull) {
      if (IsVariadic) {
         return sema::TemplateArgument(Param, IsType, {}, Loc);
      }
      if (IsType) {
         return sema::TemplateArgument(Param, QualType(), Loc);
      }

      return sema::TemplateArgument(Param, (StaticExpr*)nullptr, Loc);
   }

   if (IsVariadic) {
      auto NumArgs = Record[Idx++];
      std::vector<sema::TemplateArgument> Vec;
      Vec.reserve(NumArgs);

      for (unsigned i = 0; i < NumArgs; ++i) {
         Vec.push_back(ReadTemplateArgument(Record, Idx));
      }

      return sema::TemplateArgument(Param, IsType, move(Vec), Loc);
   }

   if (IsType) {
      auto Ty = getLocalType(Record[Idx++]);
      return sema::TemplateArgument(Param, Ty, Loc);
   }

   auto *SE = cast_or_null<StaticExpr>(ReadExpr());
   return sema::TemplateArgument(Param, SE, Loc);
}

sema::FinalTemplateArgumentList*
ASTReader::ReadTemplateArgumentList(const RecordData &Record, unsigned &Idx)
{
   llvm::SmallVector<sema::TemplateArgument, 0> Args;
   auto NumArgs = Record[Idx++];

   for (unsigned i = 0; i < NumArgs; ++i) {
      Args.push_back(ReadTemplateArgument(Record, Idx));
   }

   return sema::FinalTemplateArgumentList::Create(Context, Args, false);
}

SourceLocation ASTReader::ReadSourceLocation(const RecordDataImpl &Record,
                                             unsigned &Idx) {
   uint64_t Val = Record[Idx++];

   auto SourceID = static_cast<uint32_t>(Val);
   auto Offset = static_cast<uint32_t>(Val >> 32u);

   if (!Offset)
      return SourceLocation();

   auto It = Reader.SourceIDSubstitutionOffsets.find(SourceID);
   if (It != Reader.SourceIDSubstitutionOffsets.end()) {
      return SourceLocation(Offset + It->getSecond());
   }

   if (Reader.DiagLoc)
      return Reader.DiagLoc;

   return SourceLocation(Offset);
}

SourceRange ASTReader::ReadSourceRange(const RecordData &Record,
                                       unsigned &Idx) {
   SourceLocation beg = ReadSourceLocation(Record, Idx);
   SourceLocation end = ReadSourceLocation(Record, Idx);

   return SourceRange(beg, end);
}

/// Read an integral value
llvm::APInt ASTReader::ReadAPInt(const RecordData &Record, unsigned &Idx)
{
   unsigned BitWidth = Record[Idx++];
   unsigned NumWords = llvm::APInt::getNumWords(BitWidth);
   llvm::APInt Result(BitWidth, NumWords, &Record[Idx]);
   Idx += NumWords;
   return Result;
}

/// Read a signed integral value
llvm::APSInt ASTReader::ReadAPSInt(const RecordData &Record, unsigned &Idx)
{
   bool isUnsigned = Record[Idx++];
   return llvm::APSInt(ReadAPInt(Record, Idx), isUnsigned);
}

/// Read a floating-point value
llvm::APFloat ASTReader::ReadAPFloat(const RecordData &Record,
                                     const llvm::fltSemantics &Sem,
                                     unsigned &Idx) {
   return llvm::APFloat(Sem, ReadAPInt(Record, Idx));
}

// Read a string
std::string ASTReader::ReadString(const RecordData &Record, unsigned &Idx)
{
   unsigned Len = Record[Idx++];
   std::string Result(Record.data() + Idx, Record.data() + Idx + Len);
   Idx += Len;
   return Result;
}

ASTReader::ASTReader(ModuleReader &Reader)
   : Reader(Reader), Sema(Reader.CI.getSema()), Context(Sema.getContext()),
     FileMgr(*Sema.getDiags().getFileMgr())
{

}

ASTReader::ASTReader(ModuleReader &Reader, ASTReader &DeclReader)
   : Reader(Reader), Sema(Reader.CI.getSema()), Context(Sema.getContext()),
     FileMgr(*Sema.getDiags().getFileMgr()), DeclReader(&DeclReader)
{

}

ASTReader::~ASTReader()
{
   for (auto &Tbl : Lookups)
      delete Tbl.getSecond().Table;

   using InstTable =
      llvm::OnDiskIterableChainedHashTable<InstantiationTableLookupTrait>;
   using ConfTable = llvm::OnDiskChainedHashTable<ConformanceLookupTrait>;

   delete reinterpret_cast<InstTable*>(InstantiationTable);
   delete reinterpret_cast<ConfTable*>(ConformanceTable);
}

unsigned ASTRecordReader::readRecord(llvm::BitstreamCursor &Cursor,
                                     unsigned AbbrevID) {
   Idx = 0;
   Record.clear();
   return Cursor.readRecord(AbbrevID, Record);
}