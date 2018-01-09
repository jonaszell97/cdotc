//
// Created by Jonas Zell on 05.01.18.
//

#include "Mangle.h"

#include <llvm/Support/raw_ostream.h>

#include "../AST/Passes/SemanticAnalysis/Function.h"
#include "../AST/Passes/SemanticAnalysis/Record/Record.h"
#include "../AST/Passes/PrettyPrint/PrettyPrinter.h"

#include "../AST/Statement/Declaration/CallableDecl.h"
#include "../AST/Expression/StaticExpr.h"

#include "../Variant/Type/GenericType.h"

using namespace cdot::support;
using namespace cdot::ast;
using namespace cdot::cl;

using std::string;

namespace cdot {

string SymbolMangler::mangleVariable(llvm::StringRef id, size_t scope) const
{
   string s;
   llvm::raw_string_ostream out(s);
   out << "_V" << id.size() << id.str() << scope;

   return out.str();
}

string SymbolMangler::mangleFunction(Function *C) const
{
   string s;
   llvm::raw_string_ostream out(s);
   out << "_F" << C->getName().size() << C->getName();
   out << util::vectorToString<Argument, '\0', '\0', '\0'>(C->getArguments(),
                                                           true);

   if (auto decl = C->getDeclaration()) {
      if (!decl->getConstraints().empty()) {
         string constraintString;
         llvm::raw_string_ostream sout(constraintString);
         ast::PrettyPrinter printer(sout);

         for (auto &constraint : decl->getConstraints()) {
            printer.print(constraint.get());

            auto str = sout.str();
            out << "W" << str.size() << str;

            constraintString.clear();
         }
      }
   }

   return out.str();
}

string SymbolMangler::mangleMethod(Method *M) const
{
   string s;
   llvm::raw_string_ostream out(s);

   auto recordName = M->getOwningRecord()->getName();
   auto name = M->getName();
   auto &args = M->getArguments();

   out << "_M" << recordName.size() + name.size() + 1LLU
       << recordName.str() << '.' << name;

   for (const auto &arg : args) {
      auto str = arg.type.toString();
      out << str.length() << str;
   }

   if (auto decl = M->getDeclaration()) {
      if (!decl->getConstraints().empty()) {
         string constraintString;
         llvm::raw_string_ostream sout(constraintString);
         ast::PrettyPrinter printer(sout);

         for (auto &constraint : decl->getConstraints()) {
            printer.print(constraint.get());

            auto str = sout.str();
            out << "W" << str.size() << str;

            constraintString.clear();
         }
      }
   }

   return out.str();
}

std::string SymbolMangler::mangleProtocolMethod(
                     cl::Record *R, cl::Method *M,
                     llvm::ArrayRef<cl::AssociatedType> associatedTypes) const {
   string s;
   llvm::raw_string_ostream out(s);

   auto recordName = R->getName();
   auto name = M->getName();
   auto &args = M->getArguments();

   out << "_M" << recordName.size() + name.size() + 1LLU
       << recordName.str() << '.' << name;

   for (const auto &arg : args) {
      if (auto Gen = dyn_cast<GenericType>(*arg.type)) {
         bool found = false;
         for (const auto &AT : associatedTypes) {
            if (AT.getName().equals(Gen->getGenericTypeName())) {
               auto str = AT.getType().toString();
               out << str.length() << str;
               found = true;

               break;
            }
         }

         if (found)
            continue;
      }

      if (arg.type->isSelfTy()) {
         out << recordName.size() << recordName.str();
      }
      else {
         auto str = arg.type.toString();
         out << str.length() << str;
      }
   }

   if (auto decl = M->getDeclaration()) {
      if (!decl->getConstraints().empty()) {
         string constraintString;
         llvm::raw_string_ostream sout(constraintString);
         ast::PrettyPrinter printer(sout);

         for (auto &constraint : decl->getConstraints()) {
            printer.print(constraint.get());

            auto str = sout.str();
            out << "W" << str.size() << str;

            constraintString.clear();
         }
      }
   }

   return out.str();
}

} // namespace cdot