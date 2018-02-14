//
// Created by Jonas Zell on 14.02.18.
//

#include "TableGen/Record.h"
#include "TableGen/Value.h"
#include "TableGen/Type.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Casting.h>

using namespace cdot::tblgen;
using llvm::cast;

namespace {

class Emitter {
public:
   enum Fix {
      Infix, Prefix, Postfix
   };

   Emitter(llvm::raw_ostream &out, RecordKeeper &RK)
      : out(out), RK(RK)
   {
      OperandTy = RK.lookupRecord("OperandTy");
   }

   void emit()
   {
      llvm::SmallVector<Record*, 16> vec;

      // Types
      RK.getAllDefinitionsOf(RK.lookupClass("Type"), vec);

      out << "#ifdef " << TyMacro << "\n";
      for (auto &Ty : vec) {
         out << "   ";
         emitType(*Ty);
         out << "\n";
      }

      out << "#endif\n";
      out << "#undef " << TyMacro << "\n\n";

      vec.clear();

      // Precedence Groups
      RK.getAllDefinitionsOf(RK.lookupClass("PrecedenceGroup"),
                             vec);

      out << "#ifdef " << PGMacro << "\n";
      for (auto &PG : vec) {
         out << "   ";
         emitPrecedenceGroup(*PG);
         out << "\n";
      }

      out << "#endif\n";
      out << "#undef " << PGMacro << "\n\n";

      vec.clear();

      // Binary Operator declarations
      RK.getAllDefinitionsOf("BinaryOperator", vec);

      out << "#ifdef " << OpMacro << "\n";
      for (auto &Op : vec) {
         out << "   ";
         emitOpGeneral(*Op);
         out << "\n";
      }

      out << "#endif\n";
      out << "#undef " << OpMacro << "\n\n";

      // Operator definitions

      auto &Binary = *RK.lookupNamespace("Binary");
      auto &Unary  = *RK.lookupNamespace("Unary");

      size_t i = 0;
      for (auto &BinOp : Binary.getAllRecords()) {
         if (i++ != 0) out << "\n\n";
         emitOp(*BinOp.getValue(), Infix);
      }

      auto &PrefixUnary  = *Unary.lookupNamespace("Prefix");
      auto &PostfixUnary = *Unary.lookupNamespace("Postfix");

      for (auto &UnOp : PrefixUnary.getAllRecords()) {
         if (i++ != 0) out << "\n\n";
         emitOp(*UnOp.getValue(), Prefix);
      }
      for (auto &UnOp : PostfixUnary.getAllRecords()) {
         if (i++ != 0) out << "\n\n";
         emitOp(*UnOp.getValue(), Postfix);
      }
   }

private:
   llvm::raw_ostream &out;
   RecordKeeper &RK;

   Record *OperandTy;

   const char *TyMacro = "CDOT_TYPE";
   const char *OpMacro = "CDOT_OPERATOR";
   const char *PGMacro = "CDOT_PRECEDENCE_GROUP";
   const char *BinOpMacro = "CDOT_BINARY_OP";
   const char *PrefixOpMacro = "CDOT_PREFIX_OP";
   const char *PostfixOpMacro = "CDOT_POSTFIX_OP";

   void emitType(Record &Ty);
   void emitPrecedenceGroup(Record &PG);

   void emitOpGeneral(Record &Op);
   void emitOp(Record &Op, Fix fix);
};

} // end anonymous namespace

void Emitter::emitType(Record &Ty)
{
   out << TyMacro << "(" << Ty.getName() << ")";
}

void Emitter::emitPrecedenceGroup(Record &PG)
{
   // (name, precedence, associativity)

   auto prec  = cast<IntegerLiteral>(PG.getFieldValue("prec"));
   auto assoc = cast<RecordVal>(PG.getFieldValue("assoc"));

   out << PGMacro << "(" << PG.getName()
       << ", " << prec->getVal()
       << ", " << assoc->getRecord()->getName()
       << ")";
}

void Emitter::emitOpGeneral(Record &Op)
{
   // (name, symbol, precedenceGroup, fix)

   auto name = Op.getName();
   auto symb = cast<StringLiteral>(Op.getFieldValue("opString"))->getVal();
   auto PG   = cast<RecordVal>(Op.getFieldValue("precedenceGroup"));
   auto fix  = cast<RecordVal>(Op.getFieldValue("fix"));

   out << OpMacro << "(" << name << ", " << symb
       << ", " << PG->getRecord()->getName()
       << ", " << fix->getRecord()->getName() << ")";
}

void Emitter::emitOp(Record &Op, Fix fix)
{
   const char *macroPrefix;
   switch (fix) {
      case Infix:
         out << "// Binary Operator '" + Op.getName() + "'\n";
         macroPrefix = BinOpMacro;
         break;
      case Prefix:
         out << "// Prefix Operator '" + Op.getName() + "'\n";
         macroPrefix = PrefixOpMacro;
         break;
      case Postfix:
         out << "// Postfix Operator '" + Op.getName() + "'\n";
         macroPrefix = PostfixOpMacro;
         break;
   }

   llvm::SmallString<64> macro(macroPrefix);
   macro += "_";
   macro += cast<RecordVal>(Op.getFieldValue("builtinOp"))->getRecord()
                                                          ->getName();

   auto ApplicableTys =
      cast<ListLiteral>(Op.getFieldValue("applicableTypes"));

   auto ResultTy = cast<RecordVal>(Op.getFieldValue("resultType"))
      ->getRecord();

   out << "#ifdef " << macro << "\n";

   for (auto &TyVal : ApplicableTys->getValues()) {
      // (symbol, operandTy, resultTy)
      auto Ty = cast<RecordVal>(TyVal);
      out << "   " << macro
          << "(/*operandType=*/ " << Ty->getRecord()->getName()
          << ", /*resultType=*/ ";

      if (ResultTy == OperandTy) {
         out << Ty->getRecord()->getName();
      }
      else {
         out << ResultTy->getName();
      }

      out << ")\n";
   }

   out << "#endif\n";
   out << "#undef " << macro;
}

extern "C" {

void EmitBuiltinOperators(llvm::raw_ostream &out, RecordKeeper &RK)
{
   Emitter(out, RK).emit();
}

};