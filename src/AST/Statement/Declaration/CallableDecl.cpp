
#include "CallableDecl.h"
#include "NamespaceDecl.h"
#include "LocalVarDecl.h"
#include "Class/MethodDecl.h"

#include "../../Expression/TypeRef.h"
#include "../../Passes/SemanticAnalysis/SemaPass.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

void CallableDecl::createFunctionType(SemaPass &SP, unsigned flags)
{
   if (functionType)
      return;

   QualType retTy;
   if (isa<InitDecl>(this)) {
      retTy = SP.getContext().getVoidType();
   }
   else if (isa<DeinitDecl>(this)) {
      retTy = SP.getContext().getVoidType();
   }
   else {
      retTy = returnType->getType();
   }

   std::vector<QualType> args;
   for (auto &arg : this->args)
      args.push_back(arg->getTypeRef()->getType());

   if (isVararg())
      flags |= FunctionType::Vararg;

   if (isCstyleVararg())
      flags |= FunctionType::CStyleVararg;

   functionType = SP.getContext().getFunctionType(retTy, args, flags);
}

llvm::StringRef CallableDecl::getNameWithoutFix() const
{
   if (!isOperator())
      return getNameWithoutNamespace();

   size_t opLen;
   switch (op.getFix()) {
      case FixKind::Infix:
         opLen = 6; // "infix "
         break;
      case FixKind::Prefix:
         opLen = 7; // "prefix "
         break;
      case FixKind::Postfix:
         opLen = 8; // "postfix "
         break;
   }

   return getNameWithoutNamespace().substr(opLen);
}

} // namespace ast
} // namespace cdot