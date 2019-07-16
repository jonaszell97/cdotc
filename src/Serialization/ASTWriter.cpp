//
// Created by Jonas Zell on 27.05.18.
//

#include "ASTWriter.h"

#include "ASTCommon.h"
#include "AST/TypeVisitor.h"
#include "Basic/NestedNameSpecifier.h"
#include "Driver/Compiler.h"
#include "ILWriter.h"
#include "IL/Constant.h"
#include "IncrementalCompilation.h"
#include "Lex/Token.h"
#include "ModuleWriter.h"
#include "Module/Module.h"
#include "Sema/SemaPass.h"
#include "Support/Various.h"

#include <llvm/ADT/Hashing.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/Support/OnDiskHashTable.h>
#include <Support/Format.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::serial;
using namespace cdot::support;

namespace cdot {
namespace serial {

unsigned ComputeHash(DeclarationName Name)
{
   auto hash = static_cast<unsigned>(Name.getKind());
   switch (Name.getKind()) {
   case DeclarationName::NormalIdentifier:
   case DeclarationName::InfixOperatorName:
   case DeclarationName::PrefixOperatorName:
   case DeclarationName::PostfixOperatorName:
   case DeclarationName::MacroName:
      support::hash_combine(hash,
                            llvm::HashString(Name.getIdentifierInfo()
                                                 ->getIdentifier()));

      break;
   case DeclarationName::InstantiationName:
      support::hash_combine(hash, ComputeHash(Name.getInstantiationName()));
      break;
   case DeclarationName::LocalVarName:
      support::hash_combine(hash, ComputeHash(Name.getLocalVarName()));
      support::hash_combine(hash, Name.getLocalVarScope());
      break;
   case DeclarationName::OperatorDeclName:
      support::hash_combine(hash, ComputeHash(Name.getDeclaredOperatorName()));
   case DeclarationName::ConversionOperatorName:
   case DeclarationName::ExtensionName:
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
   case DeclarationName::DestructorName:
      // type hashes aren't stable, so we have to sacrifice some hashing
      // quality for correctness here.
      break;
   case DeclarationName::PackExpansionName:
      support::hash_combine(hash, ComputeHash(Name.getPackExpansionName()));
      support::hash_combine(hash, Name.getPackExpansionIndex());
      break;
   case DeclarationName::AccessorName:
      support::hash_combine(hash, Name.getAccessorKind());
      break;
   case DeclarationName::ClosureArgumentName:
      support::hash_combine(hash, Name.getClosureArgumentIdx());
      break;
   case DeclarationName::SubscriptName:
      support::hash_combine(hash, Name.getSubscriptKind());
      break;
   case DeclarationName::UniqueName:
      support::hash_combine(hash, Name.getUniqueID());
      break;
   case DeclarationName::ErrorName:
   default:
      break;
   }

   return hash;
}

} // namespace serial
} // namespace cdot

ASTWriter::ASTWriter(cdot::serial::ModuleWriter &Writer)
   : Stream(Writer.Stream), Writer(Writer)
{

}

ASTWriter::ASTWriter(cdot::serial::ModuleWriter &Writer, ASTWriter &DWriter)
   : Stream(Writer.Stream), Writer(Writer), DeclWriter(&DWriter)
{

}

namespace {

class InstantiationTableLookupTrait {
public:
   using key_type     = StringRef;
   using key_type_ref = key_type;

   using data_type     = unsigned;
   using data_type_ref = data_type;

   using hash_value_type = unsigned;
   using offset_type     = unsigned;

   static bool EqualKey(key_type_ref a, key_type_ref b)
   {
      return a == b;
   }

   hash_value_type ComputeHash(key_type_ref Name)
   {
      return llvm::HashString(Name);
   }

   std::pair<unsigned, unsigned> EmitKeyDataLength(llvm::raw_ostream &Out,
                                                   key_type_ref Name,
                                                   data_type_ref Lookup) {
      unsigned KeyLen = Name.size() + 1;
      unsigned DataLen = 4;

      using namespace llvm::support;

      endian::Writer<little> LE(Out);

      assert((uint16_t)DataLen == DataLen && (uint16_t)KeyLen == KeyLen);
      LE.write<uint16_t>(DataLen);
      LE.write<uint16_t>(KeyLen);

      return std::make_pair(KeyLen, DataLen);
   }

   void EmitKey(llvm::raw_ostream &Out, key_type_ref Name, unsigned KeyLen)
   {
      Out.write(Name.begin(), KeyLen);
   }

   void EmitData(llvm::raw_ostream& Out, key_type_ref,
                 unsigned ID, unsigned) {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      LE.write<uint32_t>(ID);
   }
};

} // anonymous namespace

void ASTWriter::WriteInstantiationTable()
{
   using namespace llvm;

   auto &Ctx = Writer.CI.getContext();
   auto &Sema = Writer.CI.getSema();
   auto &Mangle = Sema.getMangler();
   auto &Idents = Ctx.getIdentifiers();

   llvm::OnDiskChainedHashTableGenerator<InstantiationTableLookupTrait> Gen;
   for (auto &Inst : Ctx.RecordTemplateInstatiations) {
      auto Pref = Mangle.getPrefix(&Inst);
      auto *II = &Idents.get(Pref);
      Gen.insert(II->getIdentifier(), GetDeclRef(&Inst));
   }

   for (auto &Inst : Ctx.FunctionTemplateInstatiations) {
      auto Pref = Mangle.getPrefix(&Inst);
      auto *II = &Idents.get(Pref);
      Gen.insert(II->getIdentifier(), GetDeclRef(&Inst));
   }

   for (auto &Inst : Ctx.AliasTemplateInstatiations) {
      auto Pref = Mangle.getPrefix(&Inst);
      auto *II = &Idents.get(Pref);
      Gen.insert(II->getIdentifier(), GetDeclRef(&Inst));
   }

   SmallString<256> TblData;
   TblData.push_back('\0'); // needs a byte of padding

   uint64_t Offset;
   {
      llvm::raw_svector_ostream OS(TblData);
      Offset = Gen.Emit(OS);
   }
   
   uint64_t Data[] = {INSTANTIATION_TABLE, Offset};
   Stream.EmitRecordWithBlob(InstantiationTableAbbrev, Data, bytes(TblData));
}

void ASTWriter::WriteOffsetAbbrevs()
{
   using namespace llvm;

   // Write the declaration offsets array
   auto Abbrev = std::make_shared<BitCodeAbbrev>();
   Abbrev->Add(BitCodeAbbrevOp(DECL_OFFSET));
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // # of declarations
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // base decl ID
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob)); // declarations block
   DeclOffsetAbbrev = Stream.EmitAbbrev(std::move(Abbrev));

   // Write the type offsets array
   Abbrev = std::make_shared<BitCodeAbbrev>();
   Abbrev->Add(BitCodeAbbrevOp(TYPE_OFFSET));
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // # of types
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // base decl ID
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob)); // types block
   TypeOffsetAbbrev = Stream.EmitAbbrev(std::move(Abbrev));

   // Write the type offsets array
   Abbrev = std::make_shared<BitCodeAbbrev>();
   Abbrev->Add(BitCodeAbbrevOp(IL_VALUE_OFFSETS));
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // # of values
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // base value ID
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob)); // values block
   ValueOffsetAbbrev = Stream.EmitAbbrev(std::move(Abbrev));
}

void ASTWriter::WriteDeclOffsets(unsigned Offset)
{
   ArrayRef<uint32_t> Offsets = DeclOffsets;
   Offsets = Offsets.drop_front(Offset);

   RecordData::value_type Record[] = {
      DECL_OFFSET, Offsets.size(), 1
   };

   Stream.EmitRecordWithBlob(DeclOffsetAbbrev, Record, bytes(Offsets));
}

void ASTWriter::AddDeclRef(const Decl *D, RecordDataImpl &Record)
{
   Record.push_back(GetDeclRef(D));
}

unsigned ASTWriter::GetDeclRef(const Decl *D)
{
   if (!D || D->isIgnored())
      return 0;

   unsigned &ID = DeclIDMap[D];
   if (ID == 0) {
      assert(!Writer.DoneWriting && "adding decl after done writing");

      ID = NextDeclID++;
      auto &Back = Writer.ValuesToEmit.emplace(const_cast<Decl*>(D));

#ifndef NDEBUG
      Decl::verifyID(((Decl*)Back)->getKind());
#endif
      (void)Back;
   }

   return ID;
}

unsigned serial::ASTWriter::getDeclID(const Decl *D)
{
   auto It = DeclIDMap.find(D);
   assert(It != DeclIDMap.end() && "decl not emitted");

   return It->getSecond();
}

unsigned ASTWriter::GetModuleID(Module *M)
{
   auto It = Writer.ModuleIDs.find(M);
   assert(It != Writer.ModuleIDs.end() && "module not emitted");

   return It->getSecond();
}

void ASTWriter::WriteTypeOffsets(unsigned Offset)
{
   ArrayRef<uint32_t> Offsets = TypeOffsets;
   Offsets = Offsets.drop_front(Offset);

   RecordData::value_type Record[] = {TYPE_OFFSET, Offsets.size(), 1};
   Stream.EmitRecordWithBlob(TypeOffsetAbbrev, Record, bytes(Offsets));
}

void ASTWriter::AddTypeRef(QualType T, RecordDataImpl &Record)
{
   Record.push_back(GetOrCreateTypeID(T));
}

unsigned ASTWriter::GetOrCreateTypeID(QualType T)
{
   if (T.isNull())
      return 0;

   unsigned &ID = TypeIDMap[T];
   if (ID == 0) {
      assert(!Writer.DoneWriting && "adding type after done writing");

      ID = NextTypeID++;
      auto &Back = Writer.ValuesToEmit.emplace(T);

#ifndef NDEBUG
      Type::verifyID(((QualType)Back)->getTypeID());
#endif
      (void)Back;
   }

   return ID;
}

unsigned ASTWriter::getTypeID(QualType T) const
{
   auto It = TypeIDMap.find(T);
   assert(It != TypeIDMap.end() && "type not emitted");

   return It->getSecond();
}

namespace {

class ASTTypeWriter: public TypeVisitor<ASTTypeWriter> {
   ASTRecordWriter Record;
   Type::TypeID Code;
   unsigned AbbrevToUse = 0;

public:
   ASTTypeWriter(ASTWriter &Writer, ASTWriter::RecordData &Data)
      : Record(Writer, Data), Code(static_cast<Type::TypeID>(0))
   {}

   void visit(QualType Ty)
   {
      TypeVisitor::visit(Ty);
      Code = Ty->getTypeID();
   }

#  define CDOT_TYPE(TYPE, PARENT) void visit##TYPE(const TYPE *Ty);
#  include "AST/Types.def"

   uint64_t Emit()
   {
      return Record.Emit(Code, AbbrevToUse);
   }
};

} // anonymous namespace

void ASTTypeWriter::visitArrayType(const ArrayType *Ty)
{
   Record.push_back(Ty->getNumElements());
   Record.AddTypeRef(Ty->getElementType());
}

void ASTTypeWriter::visitAssociatedType(const AssociatedType *Ty)
{
   Record.AddDeclRef(Ty->getDecl());
   Record.AddTypeRef(Ty->getOuterAT());
}

void ASTTypeWriter::visitBoxType(const BoxType *Ty)
{
   Record.AddTypeRef(Ty->getBoxedType());
}

void ASTTypeWriter::visitExistentialType(const ExistentialType *Ty)
{
   auto Existentials = Ty->getExistentials();
   Record.push_back(Existentials.size());

   for (auto E : Existentials)
      Record.AddTypeRef(E);
}

void ASTTypeWriter::visitTokenType(const TokenType *Ty)
{

}

void ASTTypeWriter::visitBuiltinType(const BuiltinType *Ty)
{
   Record.push_back(Ty->getKind());
}

void ASTTypeWriter::visitDependentRecordType(const DependentRecordType *Ty)
{
   Record.AddDeclRef(Ty->getRecord());
   Record.AddTemplateArgumentList(Ty->getTemplateArgs());
   Record.AddTypeRef(Ty->getParent());
}

void ASTTypeWriter::visitDependentSizeArrayType(const DependentSizeArrayType *Ty)
{
   Record.AddStmt(Ty->getSizeExpr());
   Record.AddTypeRef(Ty->getElementType());
}

void ASTTypeWriter::visitDependentTypedefType(const DependentTypedefType *Ty)
{
   Record.AddDeclRef(Ty->getTypedef());
   Record.AddTemplateArgumentList(Ty->getTemplateArgs());
   Record.AddTypeRef(Ty->getParent());
}

void ASTTypeWriter::visitDependentNameType(const DependentNameType *Ty)
{
   Record.AddNestedNameSpecWithLoc(Ty->getNameSpecWithLoc());
}

void ASTTypeWriter::visitTypeVariableType(const TypeVariableType *Ty)
{
   Record.push_back(Ty->getVariableID());
}

void ASTTypeWriter::visitFunctionType(const FunctionType *Ty)
{
   Record.AddTypeRef(Ty->getReturnType());

   auto Params = Ty->getParamTypes();
   auto ParamInfo = Ty->getParamInfo();

   auto NumParams = Params.size();
   Record.push_back(NumParams);

   for (unsigned i = 0; i < NumParams; ++i) {
      Record.AddTypeRef(Params[i]);
      Record.push_back(static_cast<uint64_t>(ParamInfo[i].getConvention()));
   }

   Record.push_back(Ty->getRawFlags());
}

void ASTTypeWriter::visitTemplateParamType(const TemplateParamType *Ty)
{
   Record.AddDeclRef(Ty->getParam());
}

void ASTTypeWriter::visitInferredSizeArrayType(const InferredSizeArrayType *Ty)
{
   Record.AddTypeRef(Ty->getElementType());
}

void ASTTypeWriter::visitLambdaType(const LambdaType *Ty)
{
   visitFunctionType(Ty);
}

void ASTTypeWriter::visitMetaType(const MetaType *Ty)
{
   Record.AddTypeRef(Ty->getUnderlyingType());
}

void ASTTypeWriter::visitMutableBorrowType(const MutableBorrowType *Ty)
{
   Record.AddTypeRef(Ty->getReferencedType());
}

void ASTTypeWriter::visitMutablePointerType(const MutablePointerType *Ty)
{
   Record.AddTypeRef(Ty->getPointeeType());
}

void ASTTypeWriter::visitMutableReferenceType(const MutableReferenceType *Ty)
{
   Record.AddTypeRef(Ty->getReferencedType());
}

void ASTTypeWriter::visitPointerType(const PointerType *Ty)
{
   Record.AddTypeRef(Ty->getPointeeType());
}

void ASTTypeWriter::visitRecordType(const RecordType *Ty)
{
   Record.AddDeclRef(Ty->getRecord());
}

void ASTTypeWriter::visitReferenceType(const ReferenceType *Ty)
{
   Record.AddTypeRef(Ty->getReferencedType());
}

void ASTTypeWriter::visitTupleType(const TupleType *Ty)
{
   auto ContainedTypes = Ty->getContainedTypes();
   Record.push_back(ContainedTypes.size());

   for (auto Cont : ContainedTypes)
      Record.AddTypeRef(Cont);
}

void ASTTypeWriter::visitTypedefType(const TypedefType *Ty)
{
   Record.AddDeclRef(Ty->getTypedef());
}

void ASTWriter::WriteTypeAbbrevs()
{

}

void ASTWriter::WriteType(QualType Ty)
{
   unsigned &IdxRef = TypeIDMap[Ty];
   if (IdxRef == 0) // we haven't seen this type before.
      IdxRef = NextTypeID++;

   RecordData Record;

   // Emit the type's representation.
   ASTTypeWriter W(*this, Record);
   W.visit(Ty);

   uint64_t Offset = W.Emit();

   // Record the offset for this type.
   TypeOffsets.push_back(Offset);
}

void ASTWriter::AddSourceLocation(SourceLocation Loc, RecordDataImpl &Record)
{
   auto ID = Writer.CI.getFileMgr().getSourceId(Loc);
   uint64_t Val = static_cast<uint64_t>(Loc.getOffset()) << 32u;
   Val |= ID;

   Record.push_back(Val);
}

void ASTWriter::AddSourceRange(SourceRange Range, RecordDataImpl &Record)
{
   AddSourceLocation(Range.getStart(), Record);
   AddSourceLocation(Range.getEnd(), Record);
}

void ASTRecordWriter::AddAPInt(const llvm::APInt &Value)
{
   Record->push_back(Value.getBitWidth());
   const uint64_t *Words = Value.getRawData();
   Record->append(Words, Words + Value.getNumWords());
}

void ASTRecordWriter::AddAPSInt(const llvm::APSInt &Value)
{
   Record->push_back(Value.isUnsigned());
   AddAPInt(Value);
}

void ASTRecordWriter::AddAPFloat(const llvm::APFloat &Value)
{
   AddAPInt(Value.bitcastToAPInt());
}

void ASTWriter::AddIdentifierRef(const IdentifierInfo *II,
                                 RecordDataImpl &Record) {
   Record.push_back(getIdentifierRef(II));
}

unsigned ASTWriter::getIdentifierRef(const IdentifierInfo *II)
{
   return Writer.getIdentifierRef(II);
}

void ASTWriter::AddString(llvm::StringRef Str, RecordDataImpl &Record)
{
   Record.push_back(Str.size());
   Record.insert(Record.end(), Str.begin(), Str.end());
}

void ASTRecordWriter::AddTypeRef(SourceType T)
{
   AddStmt(T.getTypeExpr());
   AddTypeRef(T.getResolvedType());
}

void ASTWriter::AddDeclarationName(DeclarationName Name, ASTRecordWriter &Record)
{
   Record.push_back(Name.getKind());

   switch (Name.getKind()) {
   case DeclarationName::NormalIdentifier:
      Record.AddIdentifierRef(Name.getIdentifierInfo());
      break;
   case DeclarationName::InfixOperatorName:
      Record.AddIdentifierRef(Name.getInfixOperatorName());
      break;
   case DeclarationName::PrefixOperatorName:
      Record.AddIdentifierRef(Name.getPrefixOperatorName());
      break;
   case DeclarationName::PostfixOperatorName:
      Record.AddIdentifierRef(Name.getPostfixOperatorName());
      break;
   case DeclarationName::ConversionOperatorName:
      Record.AddTypeRef(Name.getConversionOperatorType());
      break;
   case DeclarationName::ExtensionName:
      Record.AddTypeRef(Name.getExtendedType());
      break;
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      Record.AddTypeRef(Name.getConstructorType());
      break;
   case DeclarationName::DestructorName:
      Record.AddTypeRef(Name.getDestructorType());
      break;
   case DeclarationName::PackExpansionName:
      Record.AddDeclarationName(Name.getPackExpansionName());
      Record.push_back(Name.getPackExpansionIndex());
      break;
   case DeclarationName::LocalVarName:
      Record.AddDeclarationName(Name.getLocalVarName());
      Record.push_back(Name.getLocalVarScope());
      break;
   case DeclarationName::OperatorDeclName:
      Record.AddDeclarationName(Name.getDeclaredOperatorName());
      break;
   case DeclarationName::AccessorName: {
      Record.push_back(Name.getAccessorKind());
      Record.AddIdentifierRef(Name.getAccessorName());
      break;
   }
   case DeclarationName::ClosureArgumentName:
      Record.push_back(Name.getClosureArgumentIdx());
      break;
   case DeclarationName::MacroName:
      Record.AddIdentifierRef(Name.getMacroName());
      break;
   case DeclarationName::InstantiationName:
      Record.AddDeclarationName(Name.getInstantiationName());
      Record.AddTemplateArgumentList(*Name.getInstantiationArgs());
      break;
   case DeclarationName::SubscriptName:
      Record.push_back(static_cast<uint64_t>(Name.getSubscriptKind()));
      break;
   case DeclarationName::UniqueName:
      Record.push_back(Name.getUniqueID());
      break;
   case DeclarationName::ErrorName:
      break;
   }
}

void ASTRecordWriter::AddNestedNameSpec(NestedNameSpecifier *Name)
{
   if (auto *Prev = Name->getPrevious()) {
      push_back(true);
      AddNestedNameSpec(Prev);
   }
   else {
      push_back(false);
   }

   push_back(Name->getKind());

   switch (Name->getKind()) {
   case NestedNameSpecifier::Type:
      AddTypeRef(Name->getType());
      break;
   case NestedNameSpecifier::Identifier:
      AddIdentifierRef(Name->getIdentifier());
      break;
   case NestedNameSpecifier::Namespace:
      AddDeclRef(Name->getNamespace());
      break;
   case NestedNameSpecifier::TemplateParam:
      AddDeclRef(Name->getParam());
      break;
   case NestedNameSpecifier::AssociatedType:
      AddDeclRef(Name->getAssociatedType());
      break;
   case NestedNameSpecifier::Alias:
      AddDeclRef(Name->getAlias());
      break;
   case NestedNameSpecifier::TemplateArgList:
      AddTemplateArgumentList(*Name->getTemplateArgs());
      break;
   case NestedNameSpecifier::Module:
      AddModuleRef(Name->getModule());
      break;
   }
}

void ASTRecordWriter::AddNestedNameSpecWithLoc(NestedNameSpecifierWithLoc *Name)
{
   AddNestedNameSpec(Name->getNameSpec());

   auto Locs = Name->getSourceRanges();
   push_back(Locs.size());

   for (auto Loc : Locs) {
      AddSourceRange(Loc);
   }
}

void ASTRecordWriter::AddTemplateArgument(const TemplateArgument &Arg)
{
   push_back(Arg.isNull());
   push_back(Arg.isType());
   push_back(Arg.isVariadic());

   AddSourceLocation(Arg.getLoc());
   AddDeclRef(Arg.getParam());

   if (Arg.isNull())
      return;

   if (Arg.isVariadic()) {
      auto &VAs = Arg.getVariadicArgs();
      push_back(VAs.size());

      for (auto &VA : VAs)
         AddTemplateArgument(VA);
   }
   else if (Arg.isType()) {
      AddTypeRef(Arg.getNonCanonicalType());
   }
   else {
      AddStmt(Arg.getValueExpr());
   }
}

void ASTRecordWriter::AddTemplateArgumentList(
                              const FinalTemplateArgumentList &TemplateArgs) {
   push_back(TemplateArgs.size());
   for (auto &TA : TemplateArgs)
      AddTemplateArgument(TA);
}

void ASTRecordWriter::AddTemplateParameterList(
                           llvm::ArrayRef<TemplateParamDecl *> TemplateParams) {
   push_back(TemplateParams.size());
   for (auto P : TemplateParams)
      AddDeclRef(P);
}

namespace {

class ASTAttrWriter: public AttrVisitor<ASTAttrWriter> {
   ASTRecordWriter Record;
   IdentifierTable &Idents;

public:
   ASTAttrWriter(ASTWriter &Writer, ASTWriter::RecordDataImpl &Data)
      : Record(Writer, Data),
        Idents(Writer.getWriter().getCompilerInstance()
                     .getContext().getIdentifiers())
   {}

   void visit(Attr *A)
   {
      Record.push_back(static_cast<uint64_t>(A->getKind()));
      Record.AddSourceRange(A->getSourceRange());

      AttrVisitor::visit(A);
   }

#  define CDOT_ATTR(NAME, SPELLING) void visit##NAME##Attr(NAME##Attr *A);
#  include "AST/Attributes.def"
};

} // anonymous namespace


#define CDOT_ATTR_SERIALIZE
#include "SerializeAttr.inc"

void ASTRecordWriter::AddAttributes(llvm::ArrayRef<const Attr *> Attrs)
{
   push_back(Attrs.size());

   for (auto A : Attrs) {
      // Emit the type's representation.
      ASTAttrWriter W(*Writer, *Record);
      W.visit(const_cast<Attr*>(A));
   }
}

void ASTRecordWriter::AddILConstant(il::Constant *C)
{
   push_back(Writer->Writer.ILWriter.GetOrCreateValueID(C));
}

void ASTRecordWriter::AddModuleRef(Module *M)
{
   if (M->getBaseModule() == Writer->getWriter().getCompilerInstance()
                                                .getCompilationModule()) {
      return push_back(Writer->GetModuleID(M));
   }

   push_back(0);

   // Write the module identifier.
   SmallVector<IdentifierInfo*, 4> FullModuleName;
   while (M) {
      FullModuleName.push_back(M->getName());
      M = M->getParentModule();
   }

   push_back(FullModuleName.size());
   for (auto it = FullModuleName.rbegin(), end = FullModuleName.rend();
        it != end; ++it) {
      AddIdentifierRef(*it);
   }
}

void ASTRecordWriter::AddToken(const lex::Token &Tok)
{
   using namespace cdot::lex;

   AddSourceLocation(Tok.getSourceLoc());
   push_back(Tok.getKind());

   switch (Tok.getKind()) {
   case tok::ident:
   case tok::op_ident:
   case tok::dollar_ident:
   case tok::macro_name:
      AddIdentifierRef(Tok.getIdentifierInfo());
      break;
   case tok::space:
      push_back(Tok.getNumSpaces());
      break;
   case tok::charliteral:
   case tok::stringliteral:
   case tok::fpliteral:
   case tok::integerliteral:
      AddString(Tok.getText());
      return;
   case tok::macro_expression:
      AddStmt(Tok.getExpr());
      break;
   case tok::macro_statement:
      AddStmt(Tok.getStmt());
      break;
   case tok::macro_declaration:
      AddDeclRef(Tok.getDecl());
      break;
   default:
      break;
   }
}

uint64_t ASTWriter::WriteDeclContextLexicalBlock(ASTContext&,
                                                 DeclContext *DC) {
   if (DC->decl_empty())
      return 0;

   uint64_t Offset = Stream.GetCurrentBitNo();
   llvm::SmallVector<uint32_t, 128> DeclIDs;
   for (const auto *D : DC->getDecls()) {
      auto ID = GetDeclRef(D);
      if (ID)
         DeclIDs.push_back(ID);
   }

   RecordData::value_type Record[] = {DECL_CONTEXT_LEXICAL, DeclIDs.size()};
   Stream.EmitRecordWithBlob(DeclContextLexicalAbbrev, Record,
                             bytes(DeclIDs));

   return Offset;
}

uint64_t ASTWriter::WriteDeclContextVisibleBlock(ASTContext &Context,
                                                 DeclContext *DC) {
   uint64_t Offset = Stream.GetCurrentBitNo();
   auto &Map = DC->getOwnNamedDecls();
   if (Map.empty())
      return 0;

   assert(DC->isPrimaryCtx() && "non-primary context with lookup!");

   // Create the on-disk hash table in a buffer.
   llvm::SmallString<4096> LookupTable;
   uint32_t TblOffset = GenerateNameLookupTable(DC, LookupTable);

   // Write the lookup table
   RecordData::value_type Record[] = { DECL_CONTEXT_VISIBLE, TblOffset };
   Stream.EmitRecordWithBlob(DeclContextVisibleLookupAbbrev, Record,
                             LookupTable);

   return Offset;
}

void ASTWriter::WriteCacheLookupTable()
{
   // Create the on-disk hash table in a buffer.
   SmallString<4096> LookupTable;
   uint32_t TblOffset = GenerateNameLookupTable(Writer.Mod->getDecl(),
                                                LookupTable);

   // Write the lookup table
   RecordData::value_type Record[] = { CACHE_LOOKUP_TABLE, TblOffset };
   Stream.EmitRecordWithBlob(CacheLookupAbbrev, Record, LookupTable);
}

namespace {

// Trait used for the on-disk hash table
class ASTDeclContextNameLookupTrait {
   ASTWriter &Writer;
   llvm::SmallVector<unsigned, 64> DeclIDs;

public:
   using key_type     = DeclarationName;
   using key_type_ref = key_type;

   /// A start and end index into DeclIDs, representing a sequence of decls.
   using data_type = std::pair<unsigned, unsigned>;
   using data_type_ref = const data_type &;

   using hash_value_type = unsigned;
   using offset_type = unsigned;

   explicit ASTDeclContextNameLookupTrait(ASTWriter &Writer) : Writer(Writer) {}

   data_type getData(const DeclContextLookupResult &Decls)
   {
      unsigned Start = DeclIDs.size();
      for (NamedDecl *D : Decls) {
         auto ID = Writer.GetDeclRef(D);
         if (ID)
            DeclIDs.push_back(ID);
      }
      
      return std::make_pair(Start, DeclIDs.size());
   }

   static bool EqualKey(key_type_ref a, key_type_ref b)
   {
      return a == b;
   }

   hash_value_type ComputeHash(DeclarationName Name)
   {
      return serial::ComputeHash(Name);
   }

   unsigned GetKeyLength(DeclarationName Name)
   {
      unsigned KeyLen = 1;
      switch (Name.getKind()) {
      case DeclarationName::NormalIdentifier:
      case DeclarationName::InfixOperatorName:
      case DeclarationName::PrefixOperatorName:
      case DeclarationName::PostfixOperatorName:
      case DeclarationName::MacroName:
         KeyLen += 4;
         break;
      case DeclarationName::InstantiationName:
         KeyLen += GetKeyLength(Name.getInstantiationName());
         break;
      case DeclarationName::LocalVarName:
         KeyLen += 8;
         break;
      case DeclarationName::OperatorDeclName:
         KeyLen += 5;
         break;
      case DeclarationName::ConversionOperatorName:
      case DeclarationName::ExtensionName:
      case DeclarationName::ConstructorName:
      case DeclarationName::BaseConstructorName:
      case DeclarationName::DestructorName:
         KeyLen += 4;
         break;
      case DeclarationName::PackExpansionName:
         KeyLen += 8;
         break;
      case DeclarationName::AccessorName:
         KeyLen += 5;
         break;
      case DeclarationName::ClosureArgumentName:
         KeyLen += 4;
         break;
      case DeclarationName::SubscriptName:
         KeyLen += 1;
         break;
      case DeclarationName::ErrorName:
      default:
         break;
      }

      return KeyLen;
   }

   std::pair<unsigned, unsigned> EmitKeyDataLength(llvm::raw_ostream &Out,
                                                   DeclarationName Name,
                                                   data_type_ref Lookup) {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      unsigned KeyLen = GetKeyLength(Name);
      
      LE.write<uint16_t>(KeyLen);

      // 4 bytes for each DeclID.
      unsigned DataLen = 4 * (Lookup.second - Lookup.first);
      assert(uint16_t(DataLen) == DataLen &&
                "too many decls for serialized lookup result");
      
      LE.write<uint16_t>(DataLen);

      return std::make_pair(KeyLen, DataLen);
   }

   void EmitKey(llvm::raw_ostream &Out, DeclarationName Name, unsigned KeyLen)
   {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      LE.write<uint8_t>(Name.getKind());
      switch (Name.getKind()) {
      case DeclarationName::NormalIdentifier:
      case DeclarationName::InfixOperatorName:
      case DeclarationName::PrefixOperatorName:
      case DeclarationName::PostfixOperatorName:
      case DeclarationName::MacroName:
         LE.write<uint32_t>(Writer.getIdentifierRef(Name.getIdentifierInfo()));
         break;
      case DeclarationName::InstantiationName:
         EmitKey(Out, Name.getInstantiationName(), KeyLen);
         break;
      case DeclarationName::OperatorDeclName:
         LE.write<uint8_t>(Name.getDeclaredOperatorName().getKind());
         LE.write<uint32_t>(Writer.getIdentifierRef(
            Name.getDeclaredOperatorName().getIdentifierInfo()));
         break;
      case DeclarationName::LocalVarName:
         LE.write<uint32_t>(Writer.getIdentifierRef(
            Name.getLocalVarName().getIdentifierInfo()));
         LE.write<uint32_t>(Name.getLocalVarScope());
         break;
      case DeclarationName::ConversionOperatorName:
         LE.write<uint32_t>(
            Writer.GetOrCreateTypeID(Name.getConversionOperatorType()));
         break;
      case DeclarationName::ExtensionName:
         LE.write<uint32_t>(
            Writer.GetOrCreateTypeID(Name.getExtendedType()));
         break;
      case DeclarationName::ConstructorName:
      case DeclarationName::BaseConstructorName:
         LE.write<uint32_t>(
            Writer.GetOrCreateTypeID(Name.getConstructorType()));
         break;
      case DeclarationName::DestructorName:
         LE.write<uint32_t>(Writer.GetOrCreateTypeID(Name.getDestructorType()));
         break;
      case DeclarationName::PackExpansionName:
         LE.write<uint32_t>(Writer.getIdentifierRef(
            Name.getPackExpansionName().getIdentifierInfo()));
         LE.write<uint32_t>(Name.getPackExpansionIndex());

         break;
      case DeclarationName::AccessorName:
         LE.write<uint32_t>(Writer.getIdentifierRef(Name.getAccessorName()));
         LE.write<uint8_t>(Name.getAccessorKind());
         break;
      case DeclarationName::ClosureArgumentName:
         LE.write<uint32_t>(Name.getClosureArgumentIdx());
         break;
      case DeclarationName::SubscriptName:
         LE.write<uint8_t>(static_cast<uint8_t>(Name.getSubscriptKind()));
         break;
      case DeclarationName::ErrorName:
      default:
         break;
      }
   }

   void EmitData(llvm::raw_ostream &Out, key_type_ref, data_type Lookup,
                 unsigned DataLen) {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      uint64_t Start = Out.tell(); (void)Start;
      for (unsigned I = Lookup.first, N = Lookup.second; I != N; ++I)
         LE.write<uint32_t>(DeclIDs[I]);
      assert(Out.tell() - Start == DataLen && "Data length is wrong");
   }
};

} // anonymous namespace

uint32_t ASTWriter::GenerateNameLookupTable(DeclContext *DC,
                                      llvm::SmallVectorImpl<char> &LookupTable){
   ASTDeclContextNameLookupTrait Trait(*this);
   llvm::OnDiskChainedHashTableGenerator<ASTDeclContextNameLookupTrait>
      Generator;

   llvm::SmallVector<DeclarationName, 16> Names;
   for (auto &DeclPair : DC->getAllNamedDecls()) {
      Names.push_back(DeclPair.getFirst());
   }

   // Sort the names into a stable order.
   std::stable_sort(Names.begin(), Names.end());

   for (auto Name : Names) {
      auto Lookup = DC->lookup(Name);
      Generator.insert(Name, Trait.getData(Lookup), Trait);
   }

   llvm::raw_svector_ostream OS(LookupTable);
   LookupTable.push_back('\0'); // lookup table needs a byte of padding

   // Emit the table.
   return Generator.Emit(OS, Trait);
}

void ASTWriter::addOperatorsPrecedenceGroups(ModuleDecl *M,
                                             SmallVectorImpl<uint32_t> &Vec) {
   for (auto &DeclNamePair : M->getAllNamedDecls()) {
      for (auto *D : DeclNamePair.getSecond().getAsLookupResult()) {
         switch (D->getKind()) {
         case Decl::ModuleDeclID:
            addOperatorsPrecedenceGroups(cast<ModuleDecl>(D), Vec);
            break;
         case Decl::OperatorDeclID:
         case Decl::PrecedenceGroupDeclID:
         case Decl::ImportDeclID:
            if (!D->isExternal()) {
               Vec.push_back(GetDeclRef(D));
            }

            break;
         default:
            break;
         }
      }
   }
}

void ASTWriter::WriteOperatorPrecedenceGroupRecords()
{
   SmallVector<uint32_t, 8> OpPGDeclIDs;
   addOperatorsPrecedenceGroups(Writer.Mod->getDecl(), OpPGDeclIDs);
   
   RecordData::value_type Data[] = {
      OPERATOR_PRECEDENCE_DECLS, OpPGDeclIDs.size()
   };

   Writer.NumOpPGDecls = static_cast<unsigned>(OpPGDeclIDs.size());
   Stream.EmitRecordWithBlob(OperatorPredecedenceAbbrev, Data,
                             bytes(OpPGDeclIDs));
}

namespace {

class ConformanceTableLookupTrait {
public:
   /// An ID representing a declaration.
   using key_type     = uint32_t;
   using key_type_ref = key_type;

   /// An offset into the conformance data.
   using data_type     = uint32_t;
   using data_type_ref = const data_type &;

   using hash_value_type = uint32_t;
   using offset_type     = uint32_t;

   hash_value_type ComputeHash(key_type_ref Key)
   {
      return Key;
   }

   std::pair<unsigned, unsigned>
   EmitKeyDataLength(llvm::raw_ostream& Out, uint32_t, uint32_t)
   {
      using namespace llvm::support;
      return std::make_pair(4, 4);
   }

   void EmitKey(llvm::raw_ostream& Out, uint32_t Key, unsigned)
   {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      LE.write<uint32_t>(Key);
   }

   void EmitData(llvm::raw_ostream& Out, uint32_t, uint32_t Data, unsigned)
   {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      LE.write<uint32_t>(Data);
   }
};

} // anonymous namespace

void ASTWriter::WriteConformanceAbbrevs()
{
   using namespace llvm;

   auto Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(CONFORMANCE_TABLE));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // table offset
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob)); // raw data
   ConformanceTableAbbrev = Stream.EmitAbbrev(move(Abv));

   Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(CONFORMANCE_DATA));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));      // raw data
   ConformanceDataAbbrev = Stream.EmitAbbrev(move(Abv));
}

void ASTWriter::WriteConformanceData()
{
   auto &ConfTable = Writer.CI.getContext().getConformanceTable();
   SmallString<256> Blob;
   {
      using namespace llvm::support;
      llvm::raw_svector_ostream OS(Blob);

      endian::Writer<little> Writer(OS);
      for (auto R : EmittedRecordDecls) {
         ConformanceOffsetMap[R] = static_cast<unsigned>(OS.tell());

         auto Conformances = ConfTable.getAllConformances(R);
         Writer.write<uint32_t>(static_cast<uint32_t>(Conformances.size()));

         for (auto Conf : Conformances) {
            Writer.write<uint8_t>(static_cast<uint8_t>(Conf->getKind()));
            Writer.write<uint32_t>(GetDeclRef(Conf->getProto()));
         }
      }
   }

   uint64_t Data[] = {CONFORMANCE_DATA};
   Stream.EmitRecordWithBlob(ConformanceDataAbbrev, Data, bytes(Blob));
}

void ASTWriter::WriteConformanceTable()
{
   llvm::OnDiskChainedHashTableGenerator<ConformanceTableLookupTrait> Gen;
   ConformanceTableLookupTrait Trait;

   for (auto &RecordIDPair : ConformanceOffsetMap) {
      Gen.insert(GetDeclRef(RecordIDPair.getFirst()), RecordIDPair.getSecond(),
                 Trait);
   }

   SmallString<256> LookupTable;
   unsigned Offset;
   {
      llvm::raw_svector_ostream OS(LookupTable);
      LookupTable.push_back('\0'); // lookup table needs a byte of padding

      // Emit the table.
      Offset = Gen.Emit(OS, Trait);
   }

   uint64_t Data[] = {CONFORMANCE_TABLE, Offset};
   Stream.EmitRecordWithBlob(ConformanceTableAbbrev, Data, bytes(LookupTable));
}

void ASTWriter::WriteAST(ModuleDecl *M)
{
   using namespace llvm;

   // Write the remaining AST contents.
   Stream.EnterSubblock(AST_BLOCK_ID, 5);
   
   // Write abbreviations.
   auto Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(INSTANTIATION_TABLE));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // table offset
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   InstantiationTableAbbrev = Stream.EmitAbbrev(move(Abv));

   Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(OPERATOR_PRECEDENCE_DECLS));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // # decls
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   OperatorPredecedenceAbbrev = Stream.EmitAbbrev(move(Abv));

   Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(CACHE_LOOKUP_TABLE));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // table offset
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   CacheLookupAbbrev = Stream.EmitAbbrev(std::move(Abv));

   // The ID assigned to the main module
   unsigned MainModuleID = GetDeclRef(M);

   // Write the declaration table of the module.
   uint64_t Data[] = { MainModuleID };
   Stream.EmitRecord(GLOBAL_DECL_CONTEXT, Data);

   // Add all declarations to be written.
   for (auto &File : Writer.CI.getFileMgr().getSourceFiles()) {
      GetDeclRef(Writer.CI.getModuleForSource(File.getValue().SourceId));
   }

   // Write the instantiation table
   WriteInstantiationTable();

   // Write OperatorDecls and PrecedenceGroupDecls, as these need to be
   // eagerly deserialized
   WriteOperatorPrecedenceGroupRecords();

   if (Writer.IncMgr) {
      // Generate the name lookup table of only the decls from this file.
      WriteCacheLookupTable();
   }

   Stream.ExitBlock(); // AST_BLOCK
}

void ASTWriter::WriteASTCache(ModuleDecl *M)
{
   WriteAST(M);
}
