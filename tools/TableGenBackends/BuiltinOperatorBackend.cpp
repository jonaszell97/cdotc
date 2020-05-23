
#include "tblgen/Record.h"
#include "tblgen/Type.h"
#include "tblgen/Value.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>

using namespace tblgen;
using namespace tblgen::support;

namespace {

class AttrDefEmitter {
public:
   enum Fix { Infix, Prefix, Postfix };

   AttrDefEmitter(llvm::raw_ostream& out, RecordKeeper& RK) : out(out), RK(RK)
   {
      OperandTy = RK.lookupRecord("OperandTy");
   }

   void emit()
   {
      std::vector<Record*> vec;

      // Precedence Groups
      RK.getAllDefinitionsOf(RK.lookupClass("PrecedenceGroup"), vec);

      out << "#ifdef " << PGMacro << "\n";
      for (auto& PG : vec) {
         out << "   ";
         emitPrecedenceGroup(*PG);
         out << "\n";
      }

      out << "#endif\n";
      out << "#undef " << PGMacro << "\n\n";

      vec.clear();

      // Binary Operator declarations
      RK.getAllDefinitionsOf("BuiltinOperator", vec);

      out << "#ifdef " << OpMacro << "\n";
      for (auto& Op : vec) {
         out << "   ";
         emitOpGeneral(*Op);
         out << "\n";
      }

      out << "#endif\n";
      out << "#undef " << OpMacro << "\n\n";

      // Operator definitions
      auto& Binary = *RK.lookupNamespace("Binary");
      auto& Unary = *RK.lookupNamespace("Unary");

      out << "#ifdef " << BinOpMacro << "\n";

      for (auto& BinOp : Binary.getAllRecords()) {
         emitOp(*BinOp, Infix);
      }

      out << "#endif\n#undef " << BinOpMacro << "\n\n";

      auto& PrefixUnary = *Unary.lookupNamespace("Prefix");
      auto& PostfixUnary = *Unary.lookupNamespace("Postfix");

      out << "#ifdef " << PrefixOpMacro << "\n";

      for (auto& UnOp : PrefixUnary.getAllRecords()) {
         emitOp(*UnOp, Prefix);
      }

      out << "#endif\n#undef " << PrefixOpMacro << "\n\n";
      out << "#ifdef " << PostfixOpMacro << "\n";

      for (auto& UnOp : PostfixUnary.getAllRecords()) {
         emitOp(*UnOp, Postfix);
      }

      out << "#endif\n#undef " << PostfixOpMacro;
   }

private:
   llvm::raw_ostream& out;
   RecordKeeper& RK;

   Record* OperandTy;

   const char* OpMacro = "CDOT_OPERATOR";
   const char* PGMacro = "CDOT_PRECEDENCE_GROUP";
   const char* BinOpMacro = "CDOT_BINARY_OP";
   const char* PrefixOpMacro = "CDOT_PREFIX_OP";
   const char* PostfixOpMacro = "CDOT_POSTFIX_OP";

   void emitPrecedenceGroup(Record& PG);
   void emitOpGeneral(Record& Op);
   void emitOp(Record& Op, Fix fix);
};

} // end anonymous namespace

void AttrDefEmitter::emitPrecedenceGroup(Record& PG)
{
   // (name, precedence, associativity)

   auto prec = cast<IntegerLiteral>(PG.getFieldValue("prec"));
   auto assoc = cast<RecordVal>(PG.getFieldValue("assoc"));

   out << PGMacro << "(" << PG.getName() << ", " << prec->getVal() << ", "
       << assoc->getRecord()->getName() << ")";
}

void AttrDefEmitter::emitOpGeneral(Record& Op)
{
   // (name, symbol, precedenceGroup, fix)

   auto name = Op.getName();
   auto symb = cast<StringLiteral>(Op.getFieldValue("opString"))->getVal();
   auto PG = cast<RecordVal>(Op.getFieldValue("precedenceGroup"));
   auto fix = cast<RecordVal>(Op.getFieldValue("fix"));

   out << OpMacro << "(" << name << ", \"" << symb << '"' << ", "
       << PG->getRecord()->getName() << ", " << fix->getRecord()->getName()
       << ")";
}

void AttrDefEmitter::emitOp(Record& Op, Fix fix)
{
   const char* macroPrefix;
   switch (fix) {
   case Infix:
      macroPrefix = BinOpMacro;
      break;
   case Prefix:
      macroPrefix = PrefixOpMacro;
      break;
   case Postfix:
      macroPrefix = PostfixOpMacro;
      break;
   }

   llvm::SmallString<64> macro(macroPrefix);

   auto name
       = cast<RecordVal>(Op.getFieldValue("builtinOp"))->getRecord()->getName();

   auto ApplicableTys = cast<ListLiteral>(Op.getFieldValue("applicableTypes"));

   auto ResultTy = cast<RecordVal>(Op.getFieldValue("resultType"))->getRecord();

   bool lvalueOperand
       = cast<IntegerLiteral>(Op.getFieldValue("lvalueOperand"))->getVal() != 0;

   for (auto& TyVal : ApplicableTys->getValues()) {
      // (name, symbol, operandTy, resultTy, lvalueOperand)
      auto Ty = cast<RecordVal>(TyVal);
      out << "   " << macro << "(" << name << ", " << Op.getName()
          << ", /*operandType=*/ " << Ty->getRecord()->getName()
          << ", /*resultType=*/ ";

      if (ResultTy == OperandTy) {
         out << Ty->getRecord()->getName();
      }
      else {
         out << ResultTy->getName();
      }

      out << ", /*lvalueOperand=*/ " << (lvalueOperand ? "true" : "false")
          << ")\n";
   }
}

extern "C" {

void EmitBuiltinOperators(std::ostream& out, RecordKeeper& RK)
{
   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      AttrDefEmitter(OS, RK).emit();
   }

   out << s;
}

}