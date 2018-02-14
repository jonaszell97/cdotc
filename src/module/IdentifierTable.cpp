//
// Created by Jonas Zell on 25.01.18.
//

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/DenseMap.h>

#include "IdentifierTable.h"
#include "Module.h"

#include "../AST/Passes/ILGen/ILGenPass.h"
#include "../AST/Passes/Declaration/DeclPass.h"
#include "../Support/ExtendedSerializerBase.h"

#include "../AST/Statement/Block/CompoundStmt.h"
#include "../AST/Statement/Declaration/CallableDecl.h"
#include "../AST/Statement/Declaration/Class/RecordDecl.h"
#include "../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../AST/Statement/Declaration/Class/PropDecl.h"
#include "../AST/Statement/Declaration/Class/FieldDecl.h"
#include "../AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "../AST/Statement/Declaration/TypedefDecl.h"
#include "../AST/Statement/Declaration/NamespaceDecl.h"
#include "../AST/Statement/Declaration/LocalVarDecl.h"

using namespace cdot::support;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::module;

namespace cdot {
namespace module {

class IdentifierTableImpl:
   public ExtendedDeserializerBase<IdentifierTableImpl> {
public:
   using QueryLevel = IdentifierTable::QueryLevel;

   struct TableEntry {
      explicit TableEntry(size_t offset)
         : level(QueryLevel::QL_None), offset(offset), decl(nullptr)
      {}

      QueryLevel getLevel() const
      {
         return level;
      }

      void setLevel(QueryLevel level)
      {
         TableEntry::level = level;
      }

      size_t getOffset() const
      {
         return offset;
      }

      void setOffset(size_t offset)
      {
         TableEntry::offset = offset;
      }

      NamedDecl *getDecl() const
      {
         assert(level != QueryLevel::QL_None && "entry not deserialized");
         return decl;
      }

      void setDecl(NamedDecl *decl)
      {
         TableEntry::decl = decl;
      }

   private:
      QueryLevel level;
      size_t offset;
      NamedDecl *decl;
   };

   explicit IdentifierTableImpl(SemaPass &SP,
                                std::unique_ptr<llvm::MemoryBuffer> &&Buf,
                                SourceLocation importLoc)
      : ExtendedDeserializerBase(SP, Buf.get()),
        Buf(move(Buf)),
        privateFileNamespace(SP.getDeclPass()
                               ->getPrivateFileNamespace(importLoc))
   {
      readOffsetMap();
   }

   NamedDecl *readDecl(llvm::StringRef name, QueryLevel level)
   {
      assert(level != QueryLevel::QL_None && "invalid query level");

      auto it = OffsetMap.find(name);
      if (it == OffsetMap.end())
         return nullptr;

      auto &entry = it->getValue();
      if (entry.getLevel() >= level) {
         return entry.getDecl();
      }

      Reader.reposition(entry.getOffset());
      auto decl = deserializeDecl(entry, level);

      entry.setOffset(size_t(Reader.getOffset()));
      entry.setLevel(level);
      entry.setDecl(decl);

      return decl;
   }

private:
   llvm::StringMap<TableEntry> OffsetMap;
   std::unique_ptr<llvm::MemoryBuffer> Buf;
   NamespaceDecl *privateFileNamespace;

   llvm::StringMap<NamespaceDecl*> NamespaceMap;

   void readOffsetMap()
   {
      auto numDecls = ReadSize();
      for (size_t i = 0; i < numDecls; ++i) {
         auto str = ReadString();

         auto offsetPtr = Reader.data();

         Reader.skip(sizeof(size_t));
         auto offset = *reinterpret_cast<size_t const*>(offsetPtr);

         OffsetMap.try_emplace(str, offset);
      }
   }

   FuncArgDecl* readArgDecl(bool readDefaultValue = false)
   {
      auto name = ReadString();
      auto ty = ReadQualType();
      bool isConst, isVararg, isCstyleVararg, isVariadic;
      ReadBools(isConst, isVararg, isCstyleVararg, isVariadic);

      Expression *defaultVal = nullptr;
      if (readDefaultValue) {
         if (ReadBool())
            defaultVal = deserializeStmt<Expression>();
      }

      auto argTy = new (SP.getContext()) TypeRef(ty);
      return new (SP.getContext()) FuncArgDecl(move(name), argTy, defaultVal,
                                               isVariadic, isConst,
                                               isCstyleVararg);
   }

   struct DeclPrelude {
      AccessModifier access;
      unsigned flags;
      string name;
      std::vector<StaticExpr*> constraints;
   };

   void declareFunction(FunctionDecl *func)
   {
//      llvm::SmallString<128> privateName;
//      privateName += privateFileNamespace->getName();
//      privateName += ".";
//      privateName += func->getName();
//
//      DstSymTab.declareFunction(privateName.str(), func);
   }

   void declareFunctionLevelTwo(FunctionDecl *func)
   {
      SP.getILGen().DeclareFunction(func);
   }

   void declareRecord(RecordDecl *R)
   {
//      llvm::SmallString<128> privateName;
//      privateName += privateFileNamespace->getName();
//      privateName += ".";
//      privateName += R->getName();
//
//      DstSymTab.declareRecord(privateName.str(), R);
      SP.getILGen().ForwardDeclareRecord(R);
   }

   void declareRecordLevelTwo(RecordDecl *R)
   {
      SP.getILGen().DeclareRecord(R);
   }

   void declareTypedef(TypedefDecl *TD)
   {
//      llvm::SmallString<128> privateName;
//      privateName += privateFileNamespace->getName();
//      privateName += ".";
//      privateName += TD->getName();
//
//      DstSymTab.declareTypedef(privateName.str(), TD);
   }

   void declareAlias(AliasDecl *alias)
   {
//      llvm::SmallString<128> privateName;
//      privateName += privateFileNamespace->getName();
//      privateName += ".";
//      privateName += alias->getName();
//
//      DstSymTab.declareAlias(privateName.str(), alias);
   }

   void declareNamespace(NamespaceDecl *NS)
   {
//      llvm::SmallString<128> privateName;
//      privateName += privateFileNamespace->getName();
//      privateName += ".";
//      privateName += NS->getName();
//
//      NS->setName(privateName.str());
//      DstSymTab.declareNamespace(NS);
   }

   NamedDecl *deserializeDecl(TableEntry &Entry, QueryLevel level);
   DeclPrelude readDeclPrelude();

   NamespaceDecl *readNamespaceDecl();

   RecordDecl *deserializeRecordDecl(TableEntry &Entry, QueryLevel level);

   void readMethodDecl(RecordDecl *R);
   void readPropertyDecl(RecordDecl *R);
   void readFieldDecl(RecordDecl *R);
   void readCaseDecl(RecordDecl *R);
   void readAssociatedTypeDecl(RecordDecl *R);
   void readTypedefDecl(RecordDecl *R);

   void readMethodDefinition(RecordDecl *R);
   void readPropertyDefinition(RecordDecl *R);
   void readFieldDefinition(RecordDecl *R);
   void readCaseDefinition(RecordDecl *R);
   void readAssociatedTypeDefinition(RecordDecl *R);
   void readTypedefDefinition(RecordDecl *R);

   FunctionDecl *deserializeFunctionDecl(TableEntry &Entry, QueryLevel level);
   AliasDecl *deserializeAliasDecl(TableEntry &Entry, QueryLevel level);
   TypedefDecl *deserializeTypedefDecl(TableEntry &Entry, QueryLevel level);
   NamespaceDecl *deserializeNamespaceDecl(TableEntry &Entry, QueryLevel level);
};

class IdentifierTableWriterImpl:
   public ExtendedSerializerBase<IdentifierTableWriterImpl> {
public:
   IdentifierTableWriterImpl(Module *M,
                             unsigned char beginLevel = 0,
                             unsigned char targetLevel = 3)
      : M(M), beginLevel(beginLevel), targetLevel(targetLevel)
   {
      serialize();
   }

private:
   ASTSerializer serializer;
   Module *M;

   unsigned char beginLevel;
   unsigned char targetLevel;

   void serialize()
   {
      WriteSize(M->getDeclarations());

      llvm::SmallDenseMap<uintptr_t, size_t> OffsetMap;
      for (auto &declPair : M->getDeclarations()) {
         auto decl = declPair.getValue().getDecl();

         WriteString(decl->getName());
         auto pos = Writer.reserve(sizeof(size_t));

         OffsetMap.try_emplace(reinterpret_cast<uintptr_t>(decl), pos);
      }

      for (auto &declPair : M->getDeclarations()) {
         auto decl = declPair.getValue().getDecl();

         auto it = OffsetMap.find(reinterpret_cast<uintptr_t >(decl));
         assert(it != OffsetMap.end());

         auto ptr = reinterpret_cast<size_t*>(Writer.data() + it->second);
         *ptr = Writer.size();

         writeDecl(decl);
      }
   }

   void writeArgDecl(FuncArgDecl *Arg, bool writeDefaultValue = false)
   {
      WriteString(Arg->getArgName());
      WriteQualType(Arg->getArgType()->getType());
      WriteBools(Arg->isConst(), Arg->isVararg(), Arg->isCstyleVararg(),
                 Arg->isVariadicArgPackExpansion());

      if (writeDefaultValue) {
         if (auto val = Arg->getDefaultVal()) {
            WriteBool(true);
            serializeStmt(val);
         }
         else {
            WriteBool(false);
         }
      }
   }

   void writeDecl(NamedDecl *decl);
   void writeDeclPrelude(NamedDecl *decl);

   void writeRecordDecl(RecordDecl *decl);

   void writeMethodDecl(MethodDecl *decl);
   void writePropertyDecl(PropDecl *decl);
   void writeFieldDecl(FieldDecl *decl);
   void writeCaseDecl(EnumCaseDecl *decl);
   void writeAssociatedTypeDecl(AssociatedTypeDecl *decl);
   void writeTypedefDecl(TypedefDecl *decl, int);

   void writeMethodDef(MethodDecl *decl);
   void writePropertyDef(PropDecl *decl);
   void writeFieldDef(FieldDecl *decl);
   void writeCaseDef(EnumCaseDecl *decl);
   void writeAssociatedTypeDef(AssociatedTypeDecl *decl);
   void writeTypedefDef(TypedefDecl *decl, int);

   void writeFunctionDecl(FunctionDecl *decl);
   void writeAliasDecl(AliasDecl *decl);
   void writeTypedefDecl(TypedefDecl *decl);
   void writeNamespaceDecl(NamespaceDecl *decl);
};

void IdentifierTableWriterImpl::writeDecl(NamedDecl *decl)
{
   Writer.WriteULEB128(decl->getTypeID());
   switch (decl->getTypeID()) {
      case AstNode::ClassDeclID:
      case AstNode::StructDeclID:
      case AstNode::EnumDeclID:
      case AstNode::UnionDeclID:
      case AstNode::ProtocolDeclID:
         writeRecordDecl(cast<RecordDecl>(decl));
         break;
      case AstNode::FunctionDeclID:
         writeFunctionDecl(cast<FunctionDecl>(decl));
         break;
      case AstNode::AliasDeclID:
         writeAliasDecl(cast<AliasDecl>(decl));
         break;
      case AstNode::TypedefDeclID:
         writeTypedefDecl(cast<TypedefDecl>(decl));
         break;
      default:
         llvm_unreachable("bad decl kind");
   }
}

NamedDecl* IdentifierTableImpl::deserializeDecl(TableEntry &Entry,
                                                QueryLevel level) {
   auto kind = ReadEnum<AstNode::NodeType>();

   NamedDecl *decl;
   switch (kind) {
      case AstNode::ClassDeclID:
      case AstNode::StructDeclID:
      case AstNode::EnumDeclID:
      case AstNode::UnionDeclID:
      case AstNode::ProtocolDeclID:
         decl = deserializeRecordDecl(Entry, level);
         break;
      case AstNode::FunctionDeclID:
         decl = deserializeFunctionDecl(Entry, level);
         break;
      case AstNode::AliasDeclID:
         decl = deserializeAliasDecl(Entry, level);
         break;
      case AstNode::TypedefDeclID:
         decl = deserializeTypedefDecl(Entry, level);
         break;
      default:
         llvm_unreachable("bad decl kind");
   }

   return decl;
}

void IdentifierTableWriterImpl::writeDeclPrelude(NamedDecl *decl)
{
   WriteByte(decl->getAccess());
   Writer.WriteULEB128(decl->getFlags());
   WriteString(decl->getName());

   WriteSize(decl->getConstraints());
   for (auto &C : decl->getConstraints())
      serializeStmt(C);
}

IdentifierTableImpl::DeclPrelude IdentifierTableImpl::readDeclPrelude()
{
   IdentifierTableImpl::DeclPrelude prelude;
   prelude.access = ReadEnum<AccessModifier>();
   prelude.flags = (unsigned)Reader.ReadULEB128();
   prelude.name = ReadString();

   auto numConstraints = ReadSize();
   for (size_t i = 0; i < numConstraints; ++i)
      prelude.constraints.push_back(deserializeStmt<StaticExpr>());

   return prelude;
}

void IdentifierTableWriterImpl::writeNamespaceDecl(NamespaceDecl *decl)
{
   while (decl) {
      assert(!decl->isAnonymousNamespace());
      WriteString(decl->getName());

      decl = decl->getOuterNamespace();
   }

   Writer.WriteULEB128(0);
}

NamespaceDecl* IdentifierTableImpl::readNamespaceDecl()
{
   NamespaceDecl *first = nullptr;
   NamespaceDecl *current = nullptr;

   while (true) {
      auto name = ReadString();
      if (name.empty())
         break;

      NamespaceDecl *next;

      auto it = NamespaceMap.find(name);
      if (it != NamespaceMap.end()) {
         next = it->getValue();
      }
      else {
         next = new (SP.getContext()) NamespaceDecl(move(name), nullptr, false);
      }

      if (!first) {
         first = next;
      }

      if (current) {
         current->setOuterNamespace(next);
      }

      current = next;
   }

   return first;
}

void IdentifierTableWriterImpl::writeFunctionDecl(FunctionDecl *decl)
{
   // level 1 - declaration
   if (beginLevel < 1) {
      writeDeclPrelude(decl);

      WriteOperatorInfo(decl->getOperator());
      WriteBools(decl->isExternC(), decl->isNoThrow(), decl->isNative(),
                 decl->isConversionOp(),
                 decl->getClosestNamespace() != nullptr);
   }

   if (targetLevel == 1)
      return;

   // level 2 - interface
   if (beginLevel < 2) {
      WriteString(decl->getLinkageName());
      WriteList(decl->getArgs(), &IdentifierTableWriterImpl::writeArgDecl,
                false);
      WriteQualType(decl->getReturnType()->getType());

      size_t i = 0;
      bool first = true;

      for (auto &arg : decl->getArgs()) {
         if (auto val = arg->getDefaultVal()) {
            if (first) {
               Writer.WriteULEB128(i);
               first = false;
            }

            serializeStmt(val);
         }

         ++i;
      }

      if (!first)
         Writer.WriteULEB128(i);
   }

   if (targetLevel == 2)
      return;

   // level 3 - definition
   if (auto body = decl->getBody()) {
      WriteBool(true);
      serializeStmt(body);
   }
   else {
      WriteBool(false);
   }
}

FunctionDecl* IdentifierTableImpl::deserializeFunctionDecl(TableEntry &Entry,
                                                           QueryLevel level) {
   FunctionDecl *func;

   // level 1 - declaration
   if (Entry.getLevel() <= QueryLevel::QL_Declaration) {
      auto prelude = readDeclPrelude();
      auto op = ReadOperatorInfo();

      bool isExternC, isNoThrow, isNative, isConvOp, hasNS;
      ReadBools(isExternC, isNoThrow, isNative, isConvOp, hasNS);

      func = new (SP.getContext()) FunctionDecl(prelude.access,
                                                move(prelude.name), {}, {},
                                                move(prelude.constraints),
                                                nullptr, op);

      func->setExternC(isExternC);
      func->isNoThrow(isNoThrow);
      func->setNative(isNative);
      func->setIsConversionOp(isConvOp);
      func->setFlags(prelude.flags);

      declareFunction(func);
   }
   else {
      func = cast<FunctionDecl>(Entry.getDecl());
   }

   if (level == QueryLevel::QL_Declaration)
      return func;

   // level 2 - interface
   if (Entry.getLevel() < 2) {
      auto linkageName = ReadString();
      auto args = ReadList<FuncArgDecl *>(&IdentifierTableImpl::readArgDecl,
                                          false);
      auto retTy = ReadQualType();
      auto typeref = new(SP.getContext()) TypeRef(retTy);

      auto fstDefaultArg = Reader.ReadULEB128();
      for (size_t i = fstDefaultArg; i < args.size(); ++i) {
         auto &arg = args[i];
         auto val = deserializeStmt<Expression>();

         arg->setDefaultVal(val);
      }

      func->setArgs(move(args));
      func->setReturnType(typeref);
      func->setLinkageName(move(linkageName));

      func->createFunctionType(SP);

      declareFunctionLevelTwo(func);
   }

   if (level == QueryLevel::QL_Interface)
      return func;

   // level 3 - definition
   if (ReadBool())
      func->setBody(deserializeStmt<CompoundStmt>());

   return func;
}

void IdentifierTableWriterImpl::writeAliasDecl(AliasDecl *decl)
{
   // level 1 - declaration
   if (beginLevel < 1) {
      writeDeclPrelude(decl);
      serializeStmt(decl->getAliasExpr());
   }

   // no further levels
}

AliasDecl* IdentifierTableImpl::deserializeAliasDecl(TableEntry &Entry,
                                                     QueryLevel level) {
   if (Entry.getLevel() > 0)
      return cast<AliasDecl>(Entry.getDecl());

   auto prelude = readDeclPrelude();
   auto expr = deserializeStmt<StaticExpr>();

   auto decl = new (SP.getContext()) AliasDecl(move(prelude.name),
                                               move(prelude.constraints),
                                               expr);

   decl->setFlags(prelude.flags);

   return decl;
}

void IdentifierTableWriterImpl::writeTypedefDecl(TypedefDecl *decl)
{
   // level 1 - declaration
   if (beginLevel < 1) {
      writeDeclPrelude(decl);
   }

   if (targetLevel == 1)
      return;

   // level 2 - interface
   if (beginLevel < 2) {
      WriteQualType(decl->getOriginTy()->getType());
   }

   // no level 3
}

TypedefDecl* IdentifierTableImpl::deserializeTypedefDecl(TableEntry &Entry,
                                                         QueryLevel level) {
   TypedefDecl *td;

   if (Entry.getLevel() < 1) {
      auto prelude = readDeclPrelude();
      td = new (SP.getContext()) TypedefDecl(prelude.access,
                                             move(prelude.name),
                                             new (SP.getContext()) TypeRef);

      td->setFlags(prelude.flags);

      declareTypedef(td);
   }
   else {
      td = cast<TypedefDecl>(Entry.getDecl());
   }

   if (level == 1)
      return td;

   if (Entry.getLevel() < 2) {
      auto qualTy = ReadQualType();
      td->getOriginTy()->setType(qualTy);
   }

   return td;
}

void IdentifierTableWriterImpl::writeRecordDecl(RecordDecl *decl)
{
   // level 1 - declaration

   if (beginLevel < 1) {
      Writer.WriteULEB128(decl->getTypeID());
      writeDeclPrelude(decl);
      
      if (auto C = dyn_cast<ClassDecl>(decl))
         WriteBool(C->isAbstract());
   }
   
   if (targetLevel == 1)
      return;
   
   // level 2 - interface

   if (auto C = dyn_cast<ClassDecl>(decl)) {
      if (auto P = C->getParentClass()) {
         WriteBool(true);
         WriteString(P->getName());
      }
      else {
         WriteBool(false);
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(decl)) {
      WriteType(*E->getRawType()->getType());
   }

   if (beginLevel < 2) {
      for (auto &nd : decl->getDecls()) {
         switch (nd->getTypeID()) {
            case AstNode::MethodDeclID:
            case AstNode::InitDeclID:
            case AstNode::DeinitDeclID:
               writeMethodDecl(cast<MethodDecl>(nd));
               break;
            case AstNode::FieldDeclID:
               writeFieldDecl(cast<FieldDecl>(nd));
               break;
            case AstNode::PropDeclID:
               writePropertyDecl(cast<PropDecl>(nd));
               break;
            case AstNode::EnumCaseDeclID:
               writeCaseDecl(cast<EnumCaseDecl>(nd));
               break;
            case AstNode::AssociatedTypeDeclID:
               writeAssociatedTypeDecl(cast<AssociatedTypeDecl>(nd));
               break;
            case AstNode::TypedefDeclID:
               writeTypedefDecl(cast<TypedefDecl>(nd), 0);
               break;
            case AstNode::StructDeclID:
            case AstNode::ClassDeclID:
            case AstNode::EnumDeclID:
            case AstNode::UnionDeclID:
            case AstNode::ProtocolDeclID:
               writeRecordDecl(cast<RecordDecl>(nd));
               break;
            default:
               llvm_unreachable("bad decl kind");
         }
      }

      Writer.WriteULEB128(AstNode::BreakStmtID);
   }
   
   if (targetLevel == 2)
      return;

   for (auto &nd : decl->getDecls()) {
      switch (nd->getTypeID()) {
         case AstNode::MethodDeclID:
         case AstNode::InitDeclID:
         case AstNode::DeinitDeclID:
            writeMethodDef(cast<MethodDecl>(nd));
            break;
         case AstNode::FieldDeclID:
            writeFieldDef(cast<FieldDecl>(nd));
            break;
         case AstNode::PropDeclID:
            writePropertyDef(cast<PropDecl>(nd));
            break;
         case AstNode::EnumCaseDeclID:
            writeCaseDef(cast<EnumCaseDecl>(nd));
            break;
         case AstNode::AssociatedTypeDeclID:
            writeAssociatedTypeDef(cast<AssociatedTypeDecl>(nd));
            break;
         case AstNode::TypedefDeclID:
            writeTypedefDef(cast<TypedefDecl>(nd), 0);
            break;
         case AstNode::StructDeclID:
         case AstNode::ClassDeclID:
         case AstNode::EnumDeclID:
         case AstNode::UnionDeclID:
         case AstNode::ProtocolDeclID:
            writeRecordDecl(cast<RecordDecl>(nd));
            break;
         default:
            llvm_unreachable("bad decl kind");
      }
   }

   // this is used as a sentinel value to indicate that no further decls are
   // following
   Writer.WriteULEB128(AstNode::BreakStmtID);
}

RecordDecl* IdentifierTableImpl::deserializeRecordDecl(TableEntry &Entry,
                                                       QueryLevel level) {
   RecordDecl *R;

   // level 1 - declaration
   if (Entry.getLevel() < 1) {
      auto kind = ReadEnum<AstNode::NodeType>();
      auto prelude = readDeclPrelude();

      switch (kind) {
         case AstNode::StructDeclID:
            R = new (SP.getContext()) StructDecl(prelude.access,
                                                 move(prelude.name), {},
                                                 move(prelude.constraints));

            break;
         case AstNode::ClassDeclID:
            R = new (SP.getContext()) ClassDecl(prelude.access,
                                                move(prelude.name), {},
                                                move(prelude.constraints),
                                                nullptr, ReadBool());

            break;
         case AstNode::EnumDeclID:
            R = new (SP.getContext()) EnumDecl(prelude.access,
                                               move(prelude.name), {},
                                               move(prelude.constraints),
                                               nullptr);

            break;
         case AstNode::UnionDeclID:
            R = new (SP.getContext()) UnionDecl(prelude.access,
                                                move(prelude.name), {},
                                                move(prelude.constraints));

            break;
         case AstNode::ProtocolDeclID:
            R = new(SP.getContext()) ProtocolDecl(prelude.access,
                                                  move(prelude.name), {},
                                                  move(prelude.constraints));

            break;
         default:
            llvm_unreachable("not a record decl");
      }

      R->setFlags(prelude.flags);

      declareRecord(R);
   }
   else {
      R = cast<RecordDecl>(Entry.getDecl());
   }

   if (level == 1)
      return R;

   if (Entry.getLevel() < 2) {
      bool done = false;
      while (!done) {
         auto kind = ReadEnum<AstNode::NodeType>();
         switch (kind) {
            case AstNode::MethodDeclID:
            case AstNode::InitDeclID:
            case AstNode::DeinitDeclID: {
               readMethodDecl(R);
               break;
            }
            case AstNode::FieldDeclID:
               readFieldDecl(R);
               break;
            case AstNode::PropDeclID:
               readPropertyDecl(R);
               break;
            case AstNode::EnumCaseDeclID:
               readCaseDecl(R);
               break;
            case AstNode::AssociatedTypeDeclID:
               readAssociatedTypeDecl(R);
               break;
            case AstNode::TypedefDeclID:
               readTypedefDecl(R);
               break;
            case AstNode::StructDeclID:
            case AstNode::ClassDeclID:
            case AstNode::EnumDeclID:
            case AstNode::UnionDeclID:
            case AstNode::ProtocolDeclID:
               deserializeRecordDecl(Entry, level);
               break;
            case AstNode::BreakStmtID:
               done = true;
               break;
            default:
               llvm_unreachable("bad decl kind");
         }
      }

      declareRecordLevelTwo(R);
   }

   if (level == 2)
      return R;

   bool done = false;
   while (!done) {
      auto kind = ReadEnum<AstNode::NodeType>();
      switch (kind) {
         case AstNode::MethodDeclID:
         case AstNode::InitDeclID:
         case AstNode::DeinitDeclID: {
            readMethodDefinition(R);
            break;
         }
         case AstNode::FieldDeclID:
            readFieldDefinition(R);
            break;
         case AstNode::PropDeclID:
            readPropertyDefinition(R);
            break;
         case AstNode::EnumCaseDeclID:
            readCaseDefinition(R);
            break;
         case AstNode::AssociatedTypeDeclID:
            readAssociatedTypeDefinition(R);
            break;
         case AstNode::TypedefDeclID:
            readTypedefDefinition(R);
            break;
         case AstNode::StructDeclID:
         case AstNode::ClassDeclID:
         case AstNode::EnumDeclID:
         case AstNode::UnionDeclID:
         case AstNode::ProtocolDeclID:
            deserializeRecordDecl(Entry, level);
            break;
         case AstNode::BreakStmtID:
            done = true;
            break;
         default:
            llvm_unreachable("bad decl kind");
      }
   }

   return R;
}

void IdentifierTableWriterImpl::writeMethodDecl(MethodDecl *decl)
{
   Writer.WriteULEB128(decl->getTypeID());
   writeDeclPrelude(decl);

   WriteString(decl->getLinkageName());
   WriteList(decl->getArgs(), &IdentifierTableWriterImpl::writeArgDecl,
             /*writeDefaultValue*/ true);

   if (isa<InitDecl>(decl)) {
      WriteBools(decl->isMemberwiseInitializer(),
                 decl->isProtocolDefaultImpl(),
                 decl->isProtocolMethod(),
                 decl->isNoThrow());
   }
   else if (isa<DeinitDecl>(decl)) {
      // nothing to do
   }
   else {
      WriteQualType(decl->getReturnType()->getType());
      WriteOperatorInfo(decl->getOperator());
      WriteBools(decl->isStatic(), decl->isProperty(),
                 decl->isProtocolDefaultImpl(), decl->isProtocolMethod(),
                 decl->isNoThrow(), decl->isConversionOp());
   }
}

void IdentifierTableWriterImpl::writeMethodDef(MethodDecl *decl)
{
   WriteString(decl->getLinkageName());
   if (auto body = decl->getBody()) {
      WriteBool(true);
      serializeStmt(body);
   }
   else {
      WriteBool(false);
   }
}

void IdentifierTableImpl::readMethodDecl(RecordDecl *R)
{
   auto kind = ReadEnum<AstNode::NodeType>();
   auto prelude = readDeclPrelude();
   auto linkageName = ReadString();

   auto args = ReadList<FuncArgDecl*>(&IdentifierTableImpl::readArgDecl, true);

   MethodDecl *M;
   if (kind == AstNode::InitDeclID) {
      bool memberwiseInit, ProtoDefaultImpl, ProtoMethod, NoThrow;
      ReadBools(memberwiseInit, ProtoDefaultImpl, ProtoMethod, NoThrow);

      M = new (SP.getContext()) InitDecl(move(args), prelude.access,
                                         nullptr, move(prelude.name));

      M->setMemberwiseInitializer(memberwiseInit);
      M->setProtocolDefaultImpl(ProtoDefaultImpl);
      M->setIsProtocolMethod(ProtoMethod);
      M->isNoThrow(NoThrow);
   }
   else if (kind == AstNode::DeinitDeclID) {
      M = new (SP.getContext()) DeinitDecl(nullptr);
   }
   else {
      assert(kind == AstNode::MethodDeclID);

      auto retTy = ReadQualType();
      auto typeref = new (SP.getContext()) TypeRef(retTy);

      auto op = ReadOperatorInfo();
      bool isStatic, isProp, isProtoDefaultImpl, isProtoMethod,
         isNoThrow, isConvOp;

      ReadBools(isStatic, isProp, isProtoDefaultImpl, isProtoMethod,
                isNoThrow, isConvOp);

      M = new (SP.getContext()) MethodDecl(move(prelude.name), typeref,
                                           move(args),
                                           move(prelude.constraints), nullptr,
                                           op, isConvOp, prelude.access,
                                           isStatic);

      M->setProperty(isProp);
      M->setProtocolDefaultImpl(isProtoDefaultImpl);
      M->setIsProtocolMethod(isProtoMethod);
      M->isNoThrow(isNoThrow);
   }

   M->setLinkageName(move(linkageName));
   M->setExternal(true);
   M->setFlags(prelude.flags);

   R->addDecl(M);
}

void IdentifierTableImpl::readMethodDefinition(RecordDecl *R)
{
   auto linkage = ReadString();
   auto M = R->getMethod(linkage, false);

   if (ReadBool())
      M->setBody(deserializeStmt<CompoundStmt>());
}

void IdentifierTableWriterImpl::writeFieldDecl(FieldDecl *decl)
{
   writeDeclPrelude(decl);
   WriteQualType(decl->getType()->getType());
   WriteBool(decl->isConst());

   if (auto G = decl->getGetterMethod()) {
      WriteBool(true);
      WriteString(G->getLinkageName());
   }
   else {
      WriteBool(false);
   }

   if (auto S = decl->getSetterMethod()) {
      WriteBool(true);
      WriteString(S->getLinkageName());
   }
   else {
      WriteBool(false);
   }
}

void IdentifierTableWriterImpl::writeFieldDef(FieldDecl *decl)
{
   WriteString(decl->getName());
   maybeSerializeStmt(decl->getDefaultVal());
}

void IdentifierTableImpl::readFieldDecl(RecordDecl *R)
{
   auto prelude = readDeclPrelude();
   auto fieldTy = ReadQualType();
   auto isConst = ReadBool();
   auto typeref = new (SP.getContext()) TypeRef(fieldTy);

   MethodDecl *Getter = nullptr;
   if (ReadBool()) {
      Getter = R->getMethod(ReadString(), false);
   }

   MethodDecl *Setter = nullptr;
   if (ReadBool()) {
      Setter = R->getMethod(ReadString(), false);
   }

   auto F = new (SP.getContext()) FieldDecl(move(prelude.name), typeref,
                                            prelude.access, false, isConst,
                                            nullptr);

   F->setGetterMethod(Getter);
   F->setSetterMethod(Setter);
   F->setFlags(prelude.flags);
   F->setExternal(true);

   R->addDecl(F);
}

void IdentifierTableImpl::readFieldDefinition(RecordDecl *R)
{
   auto name = ReadString();
   auto F = R->getField(name);

   auto defaultVal = maybeDeserializeStmt<Expression>();
   F->setDefault(defaultVal);
}

void IdentifierTableWriterImpl::writePropertyDecl(PropDecl *decl)
{
   writeDeclPrelude(decl);
   WriteQualType(decl->getType()->getType());
   WriteString(decl->getNewValName());

   if (auto G = decl->getGetterMethod()) {
      WriteBool(true);
      WriteString(G->getLinkageName());
   }
   else {
      WriteBool(false);
   }

   if (auto S = decl->getSetterMethod()) {
      WriteBool(true);
      WriteString(S->getLinkageName());
   }
   else {
      WriteBool(false);
   }
}

void IdentifierTableWriterImpl::writePropertyDef(PropDecl *decl)
{
   // nothing
}

void IdentifierTableImpl::readPropertyDecl(RecordDecl *R)
{
   auto prelude = readDeclPrelude();
   auto propTy = ReadQualType();
   auto typeref = new (SP.getContext()) TypeRef(propTy);

   auto newValName = ReadString();

   MethodDecl *Getter = nullptr;
   if (ReadBool()) {
      Getter = R->getMethod(ReadString(), false);
   }

   MethodDecl *Setter = nullptr;
   if (ReadBool()) {
      Setter = R->getMethod(ReadString(), false);
   }

   auto P = new (SP.getContext()) PropDecl(move(prelude.name), typeref,
                                           prelude.access, false, false,
                                           Getter != nullptr, Setter != nullptr,
                                           nullptr, nullptr,
                                           move(newValName));

   P->setGetterMethod(Getter);
   P->setSetterMethod(Setter);
   P->setFlags(prelude.flags);
   P->setExternal(true);

   R->addDecl(P);
}

void IdentifierTableImpl::readPropertyDefinition(RecordDecl *R)
{
   // nothing
}

void IdentifierTableWriterImpl::writeAssociatedTypeDecl(
                                                     AssociatedTypeDecl *decl) {
   writeDeclPrelude(decl);
   WriteString(decl->getProtocolSpecifier());

   if (auto ty = decl->getActualType()) {
      WriteBool(true);
      WriteQualType(ty->getType());
   }
   else {
      WriteBool(false);
   }
}

void IdentifierTableWriterImpl::writeAssociatedTypeDef(AssociatedTypeDecl *decl)
{
   // nothing
}

void IdentifierTableImpl::readAssociatedTypeDecl(RecordDecl *R)
{
   auto prelude = readDeclPrelude();
   auto proto = ReadString();

   TypeRef *actualType = nullptr;
   if (ReadBool())
      new (SP.getContext()) TypeRef(ReadQualType());

   auto AT = new (SP.getContext())
      AssociatedTypeDecl(move(proto), move(prelude.name),
                         move(prelude.constraints), actualType);

   AT->setFlags(prelude.flags);
   AT->setExternal(true);

   R->addDecl(AT);
}

void IdentifierTableImpl::readAssociatedTypeDefinition(RecordDecl *R)
{
   // nothing
}

void IdentifierTableWriterImpl::writeCaseDecl(EnumCaseDecl *decl)
{
   writeDeclPrelude(decl);
   WriteList(decl->getArgs(), &IdentifierTableWriterImpl::writeArgDecl, false);
   Writer.WriteULEB128(size_t(decl->getRawValue()));
}

void IdentifierTableWriterImpl::writeCaseDef(EnumCaseDecl *decl)
{
   WriteString(decl->getName());
   maybeSerializeStmt(decl->getRawValExpr());
}

void IdentifierTableImpl::readCaseDecl(RecordDecl *R)
{
   auto prelude = readDeclPrelude();
   auto args = ReadList<FuncArgDecl*>(&IdentifierTableImpl::readArgDecl, false);
   auto rawVal = (long long)(Reader.ReadULEB128());

   auto C = new (SP.getContext()) EnumCaseDecl(move(prelude.name), move(args));

   C->setRawValue(rawVal);
   C->setFlags(prelude.flags);
   C->setExternal(true);

   R->addDecl(C);
}

void IdentifierTableImpl::readCaseDefinition(RecordDecl *R)
{
   auto name = ReadString();
   auto C = cast<EnumDecl>(R)->hasCase(name);
   assert(C && "case not declared");

   auto expr = maybeDeserializeStmt<StaticExpr>();
   C->setRawValExpr(expr);
}

void IdentifierTableWriterImpl::writeTypedefDecl(TypedefDecl *decl, int)
{
   writeDeclPrelude(decl);
}

void IdentifierTableWriterImpl::writeTypedefDef(TypedefDecl *decl, int)
{
   WriteString(decl->getName());
   WriteQualType(decl->getOriginTy()->getType());
}

void IdentifierTableImpl::readTypedefDecl(RecordDecl *R)
{
   auto prelude = readDeclPrelude();
   auto td = new (SP.getContext()) TypedefDecl(prelude.access,
                                               move(prelude.name),
                                               new (SP.getContext()) TypeRef);

   td->setFlags(prelude.flags);
   td->setExternal(true);

   R->addDecl(td);
}

void IdentifierTableImpl::readTypedefDefinition(RecordDecl *R)
{
   auto name = ReadString();
   auto td = cast<TypedefDecl>(R->lookupSingle(name));

   auto qualTy = ReadQualType();
   td->getOriginTy()->setType(qualTy);
}

IdentifierTable::IdentifierTable(cdot::ast::SemaPass &SP,
                                 std::unique_ptr<llvm::MemoryBuffer> &&Buf,
                                 cdot::SourceLocation importLoc) {
   pImplStorage = new IdentifierTableImpl(SP, move(Buf), importLoc);
}

IdentifierTable::Decl* IdentifierTable::lookup(llvm::StringRef name,
                                               QueryLevel required) {
   return pImplStorage->readDecl(name, required);
}

IdentifierTable::~IdentifierTable()
{
   delete pImplStorage;
}

void serializeModuleInterface(module::Module *M,
                              llvm::raw_ostream &out,
                              unsigned char beginLevel,
                              unsigned char endLevel) {
   IdentifierTableWriterImpl(M, beginLevel, endLevel).finalize(out);
}

} // namespace module
} // namespace cdot