//
// Created by Jonas Zell on 30.11.17.
//

#ifndef CDOT_BUILTINGEN_H
#define CDOT_BUILTINGEN_H

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

namespace {

const char *Types[] = {
   "Int1", "Int8", "Int16", "Int32", "Int64",
   "UInt1", "UInt8", "UInt16", "UInt32", "UInt64",
   "Float", "Double", "Bool", "Char"
};

const char *BinaryOps[][2] = {
   { "Add", "+" }, { "Sub", "-" }, { "Mul", "*" }, { "Div", "/" },
   { "Mod", "%" }, { "And", "&" }, { "Or", "|" }, { "Xor", "^" },
   { "Shl", "<<" }, { "AShr", ">>" }, { "LShr", ">>>" }, { "Exp", "**" },
   { "CompEQ", "==" }, { "CompNE", "!=" }, { "CompLT", "<" }, { "CompGT", ">" },
   { "CompLE", "<=" }, { "CompGE", ">=" }, { "CompRefEQ", "===" },
   { "CompRefNE", "!==" }
};

const char *PrefixUnaryOps[][2] = {
   { "Min", "-" }, { "Plus", "+" }, { "Neg", "~" }, { "Not", "!" },
   { "PreInc", "++" }, { "PreDec", "--" }, { "AddrOf", "&" }, { "Deref", "*" }
};

const char *PostfixUnaryOps[][2] = {
   { "PostInc", "++" }, { "PostDec", "--" }
};

void genBuiltins()
{
   std::error_code ec;
   llvm::raw_fd_ostream out("./Builtin.def", ec, llvm::sys::fs::F_RW);

   out << "#ifdef CDOT_BUILTIN_BINARY" << "\n";

   for (const auto &Ty : Types) {
      out << "#   define CDOT_BUILTIN_BINARY_" << Ty << "(Lhs, Rhs, Op, OpStr) "
         "CDOT_BUILTIN_BINARY(Lhs, Rhs, Op, OpStr)\n";
   }

   out << "#endif\n\n";

   out << "#ifdef CDOT_BUILTIN_UNARY" << "\n";

   for (const auto &Ty : Types) {
      out << "#   define CDOT_BUILTIN_UNARY_" << Ty << "(Lhs, Op, OpStr) "
         "CDOT_BUILTIN_UNARY(Lhs, Op, OpStr)\n";
   }

   out << "#endif\n\n";

   for (const auto &lhs : Types) {
      out << "#ifdef CDOT_BUILTIN_BINARY_" << lhs << "\n";

      for (const auto &op : BinaryOps) {
         for (const auto &rhs : Types) {
            out << "   CDOT_BUILTIN_BINARY_" << lhs << "(" << lhs << ", " << rhs
                << ", " << op[0] << ", " << op[1] << ")\n";
         }
      }

      out << "#endif" << "\n";
      out << "#undef CDOT_BUILTIN_BINARY_" << lhs << "\n";

      out << "#ifdef CDOT_BUILTIN_UNARY_" << lhs << "\n";

      for (const auto &op : PrefixUnaryOps) {
         out << "   CDOT_BUILTIN_UNARY_" << lhs << "(" << lhs
             << ", " << op[0] << ", " << op[1] << ")\n";
      }

      out << "\n";

      for (const auto &op : PostfixUnaryOps) {
         out << "   CDOT_BUILTIN_UNARY_" << lhs << "(" << lhs
             << ", " << op[0] << ", " << op[1] << ")\n";
      }

      out << "#endif" << "\n" << "\n";
      out << "#undef CDOT_BUILTIN_UNARY_" << lhs << "\n";
   }

   out << "#undef CDOT_BUILTIN" << "\n";
   out << "#undef CDOT_BUILTIN_BINARY" << "\n";
   out << "#undef CDOT_BUILTIN_UNARY";
}

} // anonymous namespace

#endif //CDOT_BUILTINGEN_H
