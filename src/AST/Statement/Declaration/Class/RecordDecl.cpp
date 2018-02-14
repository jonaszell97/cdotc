
#include "RecordDecl.h"
#include "../../../SymbolTable.h"

#include "FieldDecl.h"
#include "MethodDecl.h"
#include "PropDecl.h"
#include "EnumCaseDecl.h"
#include "../TypedefDecl.h"
#include "../NamespaceDecl.h"

#include "../LocalVarDecl.h"

#include "../../../Expression/TypeRef.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

void RecordDecl::addInnerRecord(RecordDecl *R)
{
   R->setOuterRecord(this);
   innerRecords.insert(R);
}

DeclContext::AddDeclResultKind RecordDecl::addDecl(NamedDecl *decl)
{
   decl->setDeclContext(this);

   if (auto I = dyn_cast<InitDecl>(decl)) {
      if (I->getArgs().empty())
         if (auto S = dyn_cast<StructDecl>(this))
            S->setParameterlessConstructor(I);
   }

   if (auto D = dyn_cast<DeinitDecl>(decl))
      deinitializer = D;

   if (auto E = dyn_cast<EnumCaseDecl>(decl)) {
      auto EDecl = cast<EnumDecl>(this);
      if (E->getArgs().size() > EDecl->maxAssociatedTypes)
         EDecl->maxAssociatedTypes = E->getArgs().size();
   }

   if (auto F = dyn_cast<FieldDecl>(decl)) {
      if (!F->isStatic())
         if (auto S = dyn_cast<StructDecl>(this))
            S->fields.push_back(F);
   }

   if (auto C = dyn_cast<EnumCaseDecl>(decl)) {
      if (auto E = dyn_cast<EnumDecl>(this))
         E->cases.push_back(C);
   }


   if (auto R = dyn_cast<RecordDecl>(decl))
      addInnerRecord(R);

   return DeclContext::addDecl(decl);
}

void RecordDecl::declareOperator(MethodDecl *M)
{
   Operator opInfo(M->getOperator().getPrecedenceGroup().getAssociativity(),
                   M->getOperator().getPrecedenceGroup().getPrecedence(),
                   M->getOperator().getFix(),
                   M->getReturnType()->getType());

   OperatorPrecedences.try_emplace(M->getNameWithoutFix(), opInfo);
}

#define CDOT_RECORD_IS_X(Name) \
bool RecordDecl::is##Name() const { return isa<Name##Decl>(this); }

CDOT_RECORD_IS_X(Struct)
CDOT_RECORD_IS_X(Class)
CDOT_RECORD_IS_X(Enum)
CDOT_RECORD_IS_X(Union)
CDOT_RECORD_IS_X(Protocol)

#undef CDOT_RECORD_IS_X

bool RecordDecl::isRawEnum() const
{
   if (auto E = dyn_cast<EnumDecl>(this))
      return E->getMaxAssociatedTypes() == 0;

   return false;
}

llvm::StringRef RecordDecl::getOwnName() const
{
   if (!outerRecord)
      return name;

   return llvm::StringRef(
      name.data() + outerRecord->getName().size() + 1,
      name.size() - outerRecord->getName().size() - 1);
}

int RecordDecl::getNameSelector() const
{
   switch (typeID) {
      case ClassDeclID: return 0;
      case StructDeclID: return 1;
      case EnumDeclID: return 2;
      case UnionDeclID: return 3;
      case ProtocolDeclID: return 4;
      default:
         llvm_unreachable("bad record decl");
   }
}

AssociatedTypeDecl* RecordDecl::getAssociatedType(llvm::StringRef name,
                                                  ProtocolDecl *P) const {
   auto AT = dyn_cast_or_null<AssociatedTypeDecl>(lookupSingle(name));
   if (AT) {
      if (!AT->getProto() || AT->getProto() == P)
         return AT;
   }

   return nullptr;
}

MethodDecl* RecordDecl::getConversionOperator(Type const *toType) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (!Method)
         continue;

      if (!Method->isCastOp())
         continue;

      if (*Method->getReturnType()->getType() == toType)
         return Method;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->getConversionOperator(toType);
      }
   }

   return nullptr;
}

MethodDecl* RecordDecl::getComparisonOperator(Type const *withType) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (!Method)
         continue;

      if (Method->getNameWithoutFix() != "==" || Method->getArgs().size() != 1)
         continue;

      if (*Method->getArgs().front()->getArgType()->getType() == withType)
         return Method;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->getComparisonOperator(withType);
      }
   }

   return nullptr;
}

bool RecordDecl::hasMethodWithName(llvm::StringRef name) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getName() == name)
         return true;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->hasMethodWithName(name);
      }
   }

   return false;
}

bool RecordDecl::hasMethodTemplate(llvm::StringRef name) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getName() == name && Method->isTemplate())
         return true;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->hasMethodTemplate(name);
      }
   }

   return false;
}

PropDecl* RecordDecl::getProperty(llvm::StringRef name) const
{
   return dyn_cast_or_null<PropDecl>(lookupSingle(name));
}

FieldDecl* RecordDecl::getField(llvm::StringRef name) const
{
   return dyn_cast_or_null<FieldDecl>(lookupSingle(name));
}

MethodDecl* RecordDecl::getMethod(llvm::StringRef name, bool checkParent) const
{
   auto M = dyn_cast_or_null<MethodDecl>(lookupSingle(name));
   if (M)
      return M;

   for (auto &decl : getDecls()) {
      auto MDecl = dyn_cast<MethodDecl>(decl);
      if (MDecl && MDecl->getLinkageName() == name)
         return MDecl;
   }

   if (checkParent) {
      if (auto C = dyn_cast<ClassDecl>(this)) {
         if (auto P = C->getParentClass()) {
            return P->getMethod(name, checkParent);
         }
      }
   }

   return nullptr;
}

MethodDecl* RecordDecl::getMethod(size_t id) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getMethodID() == id)
         return Method;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->getMethod(id);
      }
   }

   return nullptr;
}

MethodDecl* RecordDecl::getOwnMethod(llvm::StringRef name)
{
   return lookupOwn<MethodDecl>(name);
}

bool RecordDecl::conformsToBaseTemplate(ProtocolDecl *P) const
{
   for (auto C : conformances)
      if (C->getSpecializedTemplate() == P)
         return true;

   return false;
}

sema::ResolvedTemplateArg const* RecordDecl::getTemplateArg(
   llvm::StringRef name) const
{
   assert(isInstantiation());
   return instantiationInfo->templateArgs.getNamedArg(name);
}

void RecordDecl::addExtension(ExtensionDecl *E)
{
   E->setDeclContext(this);

   for (auto &decl : E->getDecls())
      addDecl(decl);

   conformanceTypes.insert(conformanceTypes.end(),
                           E->getConformanceTypes().begin(),
                           E->getConformanceTypes().end());

   staticStatements.insert(staticStatements.end(),
                           E->getStaticStatements().begin(),
                           E->getStaticStatements().end());
}

void RecordDecl::calculateSize()
{
   if (auto S = dyn_cast<StructDecl>(this)) {
      if (occupiedBytes)
         return;

      for (const auto &f : S->getFields()) {
         auto ty = f->getType()->getType();
         occupiedBytes += ty->getMemberSize();

         if (ty->getAlignment() > alignment)
            alignment = ty->getAlignment();
      }

      if (!occupiedBytes) {
         occupiedBytes = sizeof(void*);
         alignment = alignof(void*);
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(this)) {
      occupiedBytes = sizeof(void*) * E->getMaxAssociatedTypes()
                      + E->getRawType()->getType()->getSize();

      alignment = alignof(void*);
   }
   else if (isa<UnionDecl>(this)) {
      for (auto &decl : getDecls()) {
         auto f = dyn_cast<FieldDecl>(decl);
         if (!f)
            continue;

         auto ty = f->getType()->getType();
         if (ty->getSize() > occupiedBytes)
            occupiedBytes = ty->getSize();

         if (ty->getAlignment() > alignment)
            alignment = ty->getAlignment();
      }
   }
}

EnumCaseDecl* EnumDecl::hasCase(llvm::StringRef name)
{
   return lookupSingle<EnumCaseDecl>(name);
}

} // namespace ast
} // namespace cdot