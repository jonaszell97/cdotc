//
// Created by Jonas Zell on 26.01.18.
//

#include "NamedDecl.h"

#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Statement/Declaration/CallableDecl.h"

#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"

#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Statement/Declaration/TypedefDecl.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::support;

namespace cdot {
namespace ast {

bool NamedDecl::classof(const DeclContext *T)
{
   switch (T->getDeclKind()) {
#  define CDOT_DECL_CONTEXT(Name)                               \
      case Name##ID:                                            \
         return true;
#  include "../../AstNode.def"

      default:
         return false;
   }
}

DeclContext* NamedDecl::castToDeclContext(const NamedDecl *D)
{
   switch (D->getTypeID()) {
#  define CDOT_DECL_CONTEXT(Name)                               \
      case Name##ID:                                            \
         return static_cast<Name*>(const_cast<NamedDecl*>(D));
#  include "../../AstNode.def"

      default:
         llvm_unreachable("not a named decl");
   }
}

NamedDecl* NamedDecl::castFromDeclContext(const DeclContext *Ctx)
{
   switch (Ctx->getDeclKind()) {
#  define CDOT_DECL_CONTEXT(Name)                                   \
      case Name##ID:                                                \
         return static_cast<Name*>(const_cast<DeclContext*>(Ctx));
#  include "../../AstNode.def"

      default:
         llvm_unreachable("not a named decl");
   }
}

RecordDecl* NamedDecl::getRecord() const
{
   return support::dyn_cast<RecordDecl>(declContext);
}

bool NamedDecl::isExported() const
{
   if (access == AccessModifier::PRIVATE)
      return false;

   if (auto NS = support::dyn_cast<NamespaceDecl>(declContext))
      return !NS->isAnonymousNamespace();

   return true;
}

bool NamedDecl::inAnonymousNamespace() const
{
   for (auto ctx = getDeclContext(); ctx; ctx = ctx->getParentCtx())
      if (auto NS = dyn_cast<NamespaceDecl>(ctx))
         if (NS->isAnonymousNamespace())
            return true;

   return false;
}

bool NamedDecl::inStdNamespace() const
{
   for (auto ctx = getDeclContext(); ctx; ctx = ctx->getParentCtx())
      if (auto NS = dyn_cast<NamespaceDecl>(ctx))
         if (NS->getName() == "std" && isa<TranslationUnit>(NS->getParentCtx()))
            return true;

   return false;
}

bool NamedDecl::isGlobalDecl() const
{
   return isa<TranslationUnit>(declContext);
}

bool NamedDecl::isOverloadable() const
{
   switch (typeID) {
      case AliasDeclID:
      case FunctionDeclID:
      case MethodDeclID:
      case InitDeclID:
      // note that DeinitDeclID is missing - deinitializers cannot be overloaded
         return true;
      default:
         return false;
   }
}

llvm::StringRef NamedDecl::getNameWithoutNamespace() const
{
   return name;
}

bool NamedDecl::isTemplate() const
{
   return !getTemplateParams().empty();
}

llvm::ArrayRef<TemplateParamDecl*> NamedDecl::getTemplateParams() const
{
   switch (typeID) {
      case AliasDeclID:
         return cast<AliasDecl>(this)->getTemplateParams();
      case TypedefDeclID:
         return cast<TypedefDecl>(this)->getTemplateParams();
      case StructDeclID:
      case ClassDeclID:
      case EnumDeclID:
      case UnionDeclID:
         return cast<RecordDecl>(this)->getTemplateParams();
      case FunctionDeclID:
      case MethodDeclID:
      case InitDeclID:
         return cast<CallableDecl>(this)->getTemplateParams();
      default:
         return {};
   }
}

TranslationUnit* DeclContext::getTranslationUnit() const
{
   auto current = const_cast<DeclContext*>(this);
   while (!isa<TranslationUnit>(current)) {
      current = current->getParentCtx();
      assert(current && "no translation unit!");
   }

   return cast<TranslationUnit>(current);
}

NamespaceDecl* DeclContext::getClosestNamespace() const
{
   auto current = const_cast<DeclContext*>(this);
   while (current && !isa<NamespaceDecl>(current)) {
      current = current->getParentCtx();
   }

   if (!current)
      return nullptr;

   return cast<NamespaceDecl>(current);
}

bool DeclContext::isGlobalDeclContext() const
{
   for (auto ctx = this; ctx; ctx = ctx->getParentCtx()) {
      switch (ctx->getDeclKind()) {
         case AstNode::NamespaceDeclID:
         case AstNode::TranslationUnitID:
         case AstNode::BreakStmtID:
            break;
         default:
            return false;
      }
   }

   return true;
}

std::string NamedDecl::getJoinedName(char join, bool includeFile) const
{
   std::string joinedName;
   if (isa<TranslationUnit>(this) && !includeFile)
      return joinedName;

   for (auto ctx = declContext; ctx; ctx = ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(ctx)) {
         joinedName += ND->getJoinedName(join, includeFile);

         if (!joinedName.empty())
            joinedName += join;

         break;
      }
   }

   joinedName += name;
   return joinedName;
}

std::string DeclContext::getSpecifierForDiagnostic() const
{
   llvm::SmallString<128> declName;
   bool appendName = false;

   switch (kind) {
      case AstNode::TranslationUnitID:
         declName += "translation unit"; break;
      case AstNode::NamespaceDeclID:
         appendName = !cast<NamespaceDecl>(this)->isAnonymousNamespace();
         declName += "namespace"; break;
      case AstNode::ClassDeclID:
         appendName = true;
         declName += "class"; break;
      case AstNode::StructDeclID:
         appendName = true;
         declName += "struct"; break;
      case AstNode::EnumDeclID:
         appendName = true;
         declName += "enum"; break;
      case AstNode::UnionDeclID:
         appendName = true;
         declName += "union"; break;
      case AstNode::ProtocolDeclID:
         appendName = true;
         declName += "protocol"; break;
      case AstNode::FunctionDeclID:
         appendName = true;
         declName += "function"; break;
      case AstNode::MethodDeclID:
         appendName = true;
         declName += "method"; break;
      case AstNode::InitDeclID:
         declName += "initializer"; break;
      case AstNode::DeinitDeclID:
         declName += "deinitializer"; break;
      default:
         llvm_unreachable("not a decl context!");
   }

   if (auto ND = dyn_cast<NamedDecl>(this); appendName) {
      declName += " ";
      declName += ND->getName();
   }

   return declName.str();
}

} // namespace ast
} // namespace cdot