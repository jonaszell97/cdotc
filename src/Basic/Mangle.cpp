//
// Created by Jonas Zell on 05.01.18.
//

#include "Mangle.h"
#include "../AST/Passes/SemanticAnalysis/SemaPass.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>


#include "../AST/Passes/PrettyPrint/PrettyPrinter.h"

#include "../AST/Statement/Declaration/CallableDecl.h"
#include "../AST/Statement/Declaration/LocalVarDecl.h"
#include "../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../AST/Statement/Declaration/Class/RecordDecl.h"
#include "../AST/Statement/Declaration/Class/FieldDecl.h"

#include "../AST/Expression/TypeRef.h"
#include "../AST/Expression/StaticExpr.h"

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

std::string SymbolMangler::mangleFunction(ast::FunctionDecl *F) const
{
   string s;
   llvm::raw_string_ostream out(s);

   auto name = F->getFullName();
   out << "_F" << name.size() << name;

   size_t i = 0;
   for (auto &arg : F->getArgs()) {
      if (i++ != 0) out << ", ";

      auto str = arg->getArgType()->toString();
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
   auto &args = M->getArgs();

   out << "_M" << name.size() << name;

   for (const auto &arg : args) {
      auto str = arg->getArgType()->getType().toString();
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

void SymbolMangler::mangleMethod(ast::SemaPass &SP,
                                 llvm::SmallString<128> &Buf,
                                 ast::MethodDecl *M,
                                 ast::RecordDecl *recordInst) const {
   auto name = M->getFullName();
   auto &args = M->getArgs();

   Buf += "_M";
   Buf += std::to_string(name.size());
   Buf += name;

   for (const auto &arg : args) {
      auto argType = arg->getArgType()->getType();
      auto ty = SP.resolveDependencies(*argType,
                                       recordInst->getTemplateArgs());

      QualType realArgTy(ty, argType.isLvalue(), argType.isConst());
      auto str = realArgTy.toString();

      Buf += std::to_string(str.length());
      Buf += str;
   }

   if (!M->getConstraints().empty()) {
      string constraintString;
      llvm::raw_string_ostream sout(constraintString);
      ast::PrettyPrinter printer(sout);

      for (auto &constraint : M->getConstraints()) {
         printer.print(constraint);

         auto str = sout.str();
         Buf += "W";
         Buf += std::to_string(str.size());
         Buf += str;

         constraintString.clear();
      }
   }
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
      auto argType = arg->getArgType()->getType();

      if (auto Gen = argType->asGenericType()) {
         auto AT = R->getAssociatedType(Gen->getGenericTypeName());
         if (AT) {
            auto str = AT->getActualType()->getType().toString();
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