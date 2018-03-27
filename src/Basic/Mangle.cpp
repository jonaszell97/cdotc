//
// Created by Jonas Zell on 05.01.18.
//

#include "Mangle.h"

#include "Sema/SemaPass.h"
#include "AST/PrettyPrinter.h"
#include "AST/Decl.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::ast;

using std::string;

namespace cdot {

string SymbolMangler::mangleVariable(llvm::StringRef id, size_t scope) const
{
   string s;
   llvm::raw_string_ostream out(s);
   out << "_V" << id.size() << id.str() << scope;

   return out.str();
}

std::string SymbolMangler::mangleAnyFunction(ast::CallableDecl *C) const
{
   if (auto M = dyn_cast<ast::MethodDecl>(C)) {
      return mangleMethod(M);
   }

   return mangleFunction(cast<FunctionDecl>(C));
}

std::string SymbolMangler::mangleFunction(ast::FunctionDecl *F) const
{
   if (F->isExternC())
      return F->getName();

   string s;
   llvm::raw_string_ostream out(s);

   auto name = F->getFullName();
   out << "_F" << name.size() << name;

   for (auto &arg : F->getArgs()) {
      auto str = arg->getType().getResolvedType().toString();
      out << str.length() << str;
   }

   if (!F->getConstraints().empty()) {
      string constraintString;
      llvm::raw_string_ostream sout(constraintString);
      ast::PrettyPrinter printer(sout);

      for (auto &constraint : F->getConstraints()) {
         printer.print(constraint);

         auto str = sout.str();
         out << "W" << str.size() << str;

         constraintString.clear();
      }
   }

   return out.str();
}

std::string SymbolMangler::mangleMethod(ast::MethodDecl *M) const
{
   string s;
   llvm::raw_string_ostream out(s);

   auto name = M->getFullName();
   out << "_M" << name.size() << name;

   auto &args = M->getArgs();
   for (const auto &arg : args) {
      auto str = arg->getType().getResolvedType().toString();
      out << str.length() << str;
   }

   if (!M->getConstraints().empty()) {
      string constraintString;
      llvm::raw_string_ostream sout(constraintString);
      ast::PrettyPrinter printer(sout);

      for (auto &constraint : M->getConstraints()) {
         printer.print(constraint);

         auto str = sout.str();
         out << "W" << str.size() << str;

         constraintString.clear();
      }
   }

   return out.str();
}

std::string SymbolMangler::mangleProtocolMethod(ast::RecordDecl *R,
                                                ast::MethodDecl *M) const {
   string s;
   llvm::raw_string_ostream out(s);

   auto recordName = R->getFullName();
   auto name = M->getFullName();
   auto &args = M->getArgs();

   out << "_M" <<  name.size() + 1LLU << name;

   for (const auto &arg : args) {
      auto argType = arg->getType().getResolvedType();

      if (auto Gen = argType->asGenericType()) {
         auto *II = &SP.getContext().getIdentifiers().get
            (Gen->getGenericTypeName());
         auto AT = R->getAssociatedType(II);
         if (AT) {
            auto str = AT->getActualType().getResolvedType().toString();
            out << str.length() << str;

            continue;
         }
      }

      if (argType->isSelfTy()) {
         out << recordName.size() << recordName;
      }
      else {
         auto str = argType.toString();
         out << str.length() << str;
      }
   }

   if (!M->getConstraints().empty()) {
      string constraintString;
      llvm::raw_string_ostream sout(constraintString);
      ast::PrettyPrinter printer(sout);

      for (auto &constraint : M->getConstraints()) {
         printer.print(constraint);

         auto str = sout.str();
         out << "W" << str.size() << str;

         constraintString.clear();
      }
   }

   return out.str();
}

} // namespace cdot