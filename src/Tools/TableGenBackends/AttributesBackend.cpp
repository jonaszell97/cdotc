//
// Created by Jonas Zell on 16.03.18.
//

#include "TableGen/Record.h"
#include "TableGen/Value.h"
#include "TableGen/Type.h"
#include "Support/Casting.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>

#include <unordered_map>

using namespace cdot::tblgen;
using namespace cdot::support;

namespace {

struct Tab {
   explicit Tab(unsigned Size = 3) : TabSize(Size) {}
   unsigned TabSize;
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &out, Tab T)
{
   for (unsigned i = 0; i < T.TabSize; ++i)
      out << ' ';

   return out;
}

struct Newline { };

llvm::raw_ostream &operator<<(llvm::raw_ostream &out, Newline)
{
   out << '\n';
   return out;
}

class AttrDefEmitter {
public:
   AttrDefEmitter(llvm::raw_ostream &out, RecordKeeper &RK)
      : out(out), RK(RK)
   {}

   void emit();

private:
   llvm::raw_ostream &out;
   RecordKeeper &RK;
};

} // anonymous namespace

void AttrDefEmitter::emit()
{
   static constexpr auto AttrMacro     = "CDOT_ATTR";
   static constexpr auto TypeAttrMacro = "CDOT_TYPE_ATTR";
   static constexpr auto DeclAttrMacro = "CDOT_DECL_ATTR";
   static constexpr auto ExprAttrMacro = "CDOT_EXPR_ATTR";
   static constexpr auto StmtAttrMacro = "CDOT_STMT_ATTR";
   static constexpr auto AbstractMacro = "CDOT_ABSTRACT_ATTR";
   static constexpr auto VisitMacro    = "CDOT_VISITATION_POINT";


   out << "#ifdef " << AttrMacro << "\n";
   out << "#   define " << TypeAttrMacro << "(Name, Spelling) "
                        << AttrMacro     << "(Name, Spelling)\n";
   out << "#   define " << DeclAttrMacro << "(Name, Spelling) "
                        << AttrMacro     << "(Name, Spelling)\n";
   out << "#   define " << ExprAttrMacro << "(Name, Spelling) "
                        << AttrMacro     << "(Name, Spelling)\n";
   out << "#   define " << StmtAttrMacro << "(Name, Spelling) "
                        << AttrMacro     << "(Name, Spelling)\n";
   out << "#endif\n\n";

   out << "#ifndef " << AbstractMacro << "\n"
       << "#   define " << AbstractMacro << "(Name)\n"
       << "#endif\n\n";

   out << "#ifndef " << VisitMacro << "\n"
       << "#   define " << VisitMacro << "(Name)\n"
       << "#endif\n\n";

   llvm::SmallVector<Record*, 16> Attrs;

   // Decl Attributes
   Attrs.clear();
   RK.getAllDefinitionsOf(RK.lookupClass("DeclAttr"), Attrs);

   out << "#ifdef " << DeclAttrMacro << "\n";
   out << "   " << AbstractMacro << "(_firstDeclAttr)\n";

   for (auto &Attr : Attrs) {
      auto Spelling = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
      out << "   " << DeclAttrMacro << "(" << Attr->getName() << ", "
          << Spelling << ")\n";
   }

   out << "   " << AbstractMacro << "(_lastDeclAttr)\n";
   out << "#endif\n#undef " << DeclAttrMacro << "\n\n";

   // Stmt Attributes
   Attrs.clear();
   RK.getAllDefinitionsOf(RK.lookupClass("StmtAttr"), Attrs);

   out << "#ifdef " << StmtAttrMacro << "\n";
   out << "   " << AbstractMacro << "(_firstStmtAttr)\n";

   for (auto &Attr : Attrs) {
      auto Spelling = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
      out << "   " << StmtAttrMacro << "(" << Attr->getName() << ", "
          << Spelling << ")\n";
   }

   out << "   " << AbstractMacro << "(_lastStmtAttr)\n";
   out << "#endif\n#undef " << StmtAttrMacro << "\n\n";

   // Expr Attributes
   Attrs.clear();
   RK.getAllDefinitionsOf(RK.lookupClass("ExprAttr"), Attrs);

   out << "#ifdef " << ExprAttrMacro << "\n";
   out << "   " << AbstractMacro << "(_firstExprAttr)\n";

   for (auto &Attr : Attrs) {
      auto Spelling = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
      out << "   " << ExprAttrMacro << "(" << Attr->getName() << ", "
          << Spelling << ")\n";
   }

   out << "   " << AbstractMacro << "(_lastExprAttr)\n";
   out << "#endif\n#undef " << ExprAttrMacro << "\n\n";

   // Type Attributes
   Attrs.clear();
   RK.getAllDefinitionsOf(RK.lookupClass("TypeAttr"), Attrs);

   out << "#ifdef " << TypeAttrMacro << "\n";
   out << "   " << AbstractMacro << "(_firstTypeAttr)\n";

   for (auto &Attr : Attrs) {
      auto Spelling = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
      out << "   " << TypeAttrMacro << "(" << Attr->getName() << ", "
          << Spelling << ")\n";
   }

   out << "   " << AbstractMacro << "(_lastTypeAttr)\n";
   out << "#endif\n#undef " << TypeAttrMacro << "\n\n";

   llvm::SmallVector<Record*, 16> VisitPoints;
   RK.getAllDefinitionsOf("VisitationPoint", VisitPoints);

   for (auto &VP : VisitPoints) {
      out << VisitMacro << "(" << VP->getName() << ")\n";
   }

   out << "\n#undef " << AbstractMacro << "\n";
   out << "#undef " << VisitMacro << "\n";
   out << "#undef " << AttrMacro;
}

namespace {

class AttrClassEmitter {
public:
   AttrClassEmitter(llvm::raw_ostream &out, RecordKeeper &RK)
      : out(out), RK(RK)
   {}

   void emit();
   void emitAttrDecl(Record *Attr, Class *Base);
   void emitGetVisitationPoint();

   void emitPrintPrettyImpl(Record *Attr, Class *Base);
   void emitCloneImpl(Record *Attr, Class *Base);

private:
   llvm::raw_ostream &out;
   RecordKeeper &RK;
};

llvm::StringRef getCXXType(RecordVal *Arg)
{
   auto C = Arg->getRecord()->getBases().front().getBase();
   if (C->getName() == "IntArg")
      return "llvm::APSInt";

   if (C->getName() == "FloatArg")
      return "llvm::APFloat";

   if (C->getName() == "StringArg")
      return "std::string";

   if (C->getName() == "TypeArg")
      return "SourceType";

   if (C->getName() == "ExprArg")
      return "ast::StaticExpr*";

   assert(C->getName() == "EnumArg");
   return "";
}

llvm::StringRef getCXXGetterType(RecordVal *Arg)
{
   auto C = Arg->getRecord()->getBases().front().getBase();
   if (C->getName() == "IntArg")
      return "const llvm::APSInt&";

   if (C->getName() == "FloatArg")
      return "const llvm::APFloat&";

   if (C->getName() == "StringArg")
      return "llvm::StringRef";

   if (C->getName() == "TypeArg")
      return "SourceType";

   if (C->getName() == "ExprArg")
      return "ast::StaticExpr*";

   assert(C->getName() == "EnumArg");
   return "";
}

llvm::StringRef getCXXArgType(RecordVal *Arg)
{
   auto C = Arg->getRecord()->getBases().front().getBase();
   if (C->getName() == "IntArg")
      return "llvm::APSInt&&";

   if (C->getName() == "FloatArg")
      return "llvm::APFloat&&";

   if (C->getName() == "StringArg")
      return "llvm::StringRef";

   if (C->getName() == "TypeArg")
      return "SourceType";

   if (C->getName() == "ExprArg")
      return "ast::StaticExpr*";

   assert(C->getName() == "EnumArg");
   return "";
}

std::string buildDefaultValue(RecordVal *Arg, Value *Default)
{
   std::string str;
   auto C = Arg->getRecord()->getBases().front().getBase();
   if (C->getName() == "IntArg") {
      str += "llvm::APSInt(llvm::APInt(64, ";
      str += cast<IntegerLiteral>(Default)->getVal().toString(10);
      str += "), false)";

      return str;
   }

   if (C->getName() == "FloatArg") {
      str += "llvm::APFloat(";

      llvm::SmallString<64> Str;
      cast<FPLiteral>(Default)->getVal().toString(Str);

      str += Str.str();
      str += ")";

      return str;
   }

   if (C->getName() == "StringArg") {
      str += '"';
      str += cast<StringLiteral>(Default)->getVal();
      str += '"';

      return str;
   }

   if (C->getName() == "TypeArg") {
      return "SourceType()";
   }

   if (C->getName() == "ExprArg")
      return "nullptr";

   assert(C->getName() == "EnumArg");
   str += cast<StringLiteral>(Default)->getVal();

   return str;
}

} // anonymous namespace

void AttrClassEmitter::emit()
{
   llvm::SmallVector<Record*, 16> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   // class declarations
   out << "#ifdef CDOT_ATTR_DECL\n";

   size_t i = 0;
   for (auto &Attr : Attrs) {
      if (i++ != 0) out << "\n\n";

      auto Base = Attr->getBases().front().getBase();
      emitAttrDecl(Attr, Base);
   }

   out << "\n#endif\n#undef CDOT_ATTR_DECL\n\n";

   // class implementations
   out << "#ifdef CDOT_ATTR_IMPL\n";

   emitGetVisitationPoint();

   i = 0;
   for (auto &Attr : Attrs) {
      if (i++ != 0) out << "\n\n";

      auto Base = Attr->getBases().front().getBase();
      emitPrintPrettyImpl(Attr, Base);
      emitCloneImpl(Attr, Base);
   }

   out << "\n#endif\n#undef CDOT_ATTR_IMPL\n\n";
}

void AttrClassEmitter::emitAttrDecl(Record *Attr, Class *Base)
{
   std::string ClassName = Attr->getName();
   ClassName += "Attr";

   out << "class " << ClassName
       << ": public " << Base->getName()
       << " {\n";

   out << "public:" << Newline()
       << Tab();

   out << "static bool classof(const Attr* A) "
          "{ return classofKind(A->getKind()); }"
          << Newline() << Tab();

   out << "static bool classofKind(AttrKind kind)"
          "{ return kind == AttrKind::" << Attr->getName() << "; }"
       << Newline() << Newline();

   out << R"__(
   void printPretty(llvm::raw_ostream &out) const;
   )__" << ClassName << R"__( *clone(ast::ASTContext &Ctx) const;
)__";

   std::string Enums;
   llvm::raw_string_ostream EnumStr(Enums);

   std::string Getters;
   llvm::raw_string_ostream GetterStr(Getters);

   std::string Fields;
   llvm::raw_string_ostream FieldStr(Fields);

   FieldStr << "private:" << Newline();

   std::vector<std::pair<std::string, std::string>> ConstructorArgs;

   size_t NumEnumArgs = 0;
   size_t ArgNo       = 0;

   auto Args = cast<ListLiteral>(Attr->getFieldValue("Args"));

   for (auto &Arg : Args->getValues()) {
      if (ArgNo++ != 0) { out << "\n"; GetterStr << "\n"; };

      auto ArgVal = cast<RecordVal>(Arg);
      auto Name = cast<StringLiteral>(ArgVal->getRecord()
                                            ->getFieldValue("name"))->getVal();

      auto TypeName       = getCXXType(ArgVal);
      auto GetterTypeName = getCXXGetterType(ArgVal);
      auto ArgTypeName    = getCXXArgType(ArgVal);

      // enum argument, introduce new enum
      if (TypeName.empty()) {
         if (NumEnumArgs++ != 0) EnumStr << "\n\n";

         std::string EnumName(1, ::toupper(Name.front()));
         EnumName += Name.substr(1);
         EnumName += "Kind";

         EnumStr << Tab() << "enum " << EnumName << " {" << Newline();

         size_t i = 0;
         auto &CaseVals = cast<ListLiteral>(ArgVal->getRecord()
                                                  ->getFieldValue("cases"))
            ->getValues();

         for (auto &Case : CaseVals) {
            if (i++ != 0) EnumStr << "," << Newline();
            EnumStr << Tab() << Tab() << cast<StringLiteral>(Case)->getVal();
         }

         EnumStr << Newline() << Tab() << "};";

         TypeName = EnumName;
         GetterTypeName = EnumName;
         ArgTypeName = EnumName;
      }

      FieldStr << Tab() << TypeName << " " << Name << ";";
      GetterStr << Tab() << GetterTypeName << " get"
                << (char)::toupper(Name.front())
                << Name.substr(1) << "() const { return " << Name << "; }";

      // build constructor arguments and initialization
      std::string constructorArg = ArgTypeName;
      constructorArg += " ";
      constructorArg += Name;

      bool IsOptional = cast<IntegerLiteral>(ArgVal->getRecord()
                                                   ->getFieldValue("optional"))
         ->getVal().getBoolValue();

      if (IsOptional) {
         if (ArgVal->getRecord()->hasField("defaultVal")) {
            constructorArg += " = ";
            constructorArg += buildDefaultValue(
               ArgVal, ArgVal->getRecord()->getFieldValue("defaultVal"));
         }
      }

      std::string init = Name;
      init += "(";

      if (ArgTypeName.endswith("&&")) {
         init += "std::move(";
         init += Name;
         init += ")";
      }
      else {
         init += Name;
      }

      init += ")";

      ConstructorArgs.emplace_back(std::move(constructorArg), std::move(init));
   }

   EnumStr.flush();
   if (!Enums.empty()) {
      out << "\n\n" << Enums;
   }

   FieldStr.flush();
   if (!Fields.empty()) {
      out << "\n\n" << Fields;
   }

   GetterStr.flush();
   if (!Getters.empty()) {
      out << "\n\npublic:\n";
      out << Getters;
   }

   out << "\n\npublic:\n" << Tab();
   out << "explicit " << ClassName << "(";

   size_t NumCtorArgs = 0;
   for (auto &Arg : ConstructorArgs) {
      if (NumCtorArgs++ != 0) out << ", ";
      out << Arg.first;
   }

   if (NumCtorArgs != 0) out << ", ";
   out << "SourceRange SR = SourceRange())";

   out << "\n" << Tab() << Tab() << ": ";
   out << Base->getName() << "(AttrKind::" << Attr->getName() << ", SR)";

   for (auto &Arg : ConstructorArgs) {
      out << ", ";
      out << Arg.second;
   }

   out << Newline() << Tab() << "{}";
   out << Newline() << "};";
}

void AttrClassEmitter::emitGetVisitationPoint()
{
   std::unordered_map<Record*, std::vector<Record*>> AttrMap;

   llvm::SmallVector<Record*, 16> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   for (auto &Attr : Attrs) {
      auto VP = cast<RecordVal>(Attr->getFieldValue("Visit"))->getRecord();
      AttrMap[VP].push_back(Attr);
   }

   std::string CaseStr;
   llvm::raw_string_ostream CS(CaseStr);

   for (auto &Vec : AttrMap) {
      for (auto &Attr : Vec.second) {
         CS << Tab() << "case AttrKind::" << Attr->getName() << ":\n";
      }

      CS << Tab() << Tab() << "return " << Vec.first->getName() << ";\n";
   }

   CS.flush();

   out << R"__(
   Attr::VisitationPoint Attr::getVisitationPoint() const {
      switch (kind) {
      default: llvm_unreachable("bad attr kind!");
      )__" << CaseStr << R"__(
      }
   }
)__";
}

void AttrClassEmitter::emitPrintPrettyImpl(Record *Attr, Class *)
{
   std::string ClassName = Attr->getName();
   ClassName += "Attr";

   // emit 'printPretty' implementation
   out << "void " << ClassName << "::printPretty(llvm::raw_ostream &out)"
       << " const {" << Newline();

   std::string PrettyPrintArgs;
   llvm::raw_string_ostream ArgStr(PrettyPrintArgs);

   size_t ArgNo = 0;
   auto &Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();

   std::string caseStrings;
   llvm::raw_string_ostream caseStr(caseStrings);

   if (!Args.empty()) {
      for (auto &Arg : Args) {
         if (ArgNo++ != 0) ArgStr << " << \", \" << ";
         auto ArgVal = cast<RecordVal>(Arg);
         auto Name = cast<StringLiteral>(ArgVal->getRecord()
                                               ->getFieldValue("name"))->getVal();

         auto TypeName = getCXXType(ArgVal);
         if (TypeName.empty()) {
            // enum argument, print cases
            std::string varName = Name;
            varName += "Str";

            caseStr << "llvm::StringRef " << varName << ";\n";

            // switch over the cases to get the case name
            auto &CaseVals = cast<ListLiteral>(ArgVal->getRecord()
                                                     ->getFieldValue("cases"))
               ->getValues();

            caseStr << "switch (" << Name << ") {" << Newline();

            for (auto &Case : CaseVals) {
               auto CaseName =  cast<StringLiteral>(Case)->getVal();;
               caseStr << Tab() << "case " << CaseName << ": " << varName
                       << " = " << '"' << CaseName << '"' << "; break;"
                       << Newline();
            }

            caseStr << Newline() << "}";

            ArgStr << varName;
         }
         else if (TypeName == "ast::StaticExpr*") {
            ArgStr << Name << "->getEvaluatedExpr()";
         }
         else {
            ArgStr << Name;
         }
      }
   }

   caseStr.flush();
   out << caseStrings;

   out << R"__(
   out << "@" << getSpelling();
)__";

   ArgStr.flush();
   if (!PrettyPrintArgs.empty()) {
      out << "out << '(' << " << PrettyPrintArgs
          << " << ')';" << Newline();
   }

   out << Newline() << "}\n\n";
}

void AttrClassEmitter::emitCloneImpl(Record *Attr, Class *)
{
   std::string ClassName = Attr->getName();
   ClassName += "Attr";

   // emit 'printPretty' implementation
   out << ClassName << " *" << ClassName << "::clone(ast::ASTContext &Ctx)"
       << " const {" << Newline();

   out << Tab() << "return new(Ctx) " << ClassName << "(";

   size_t ArgNo = 0;
   auto &Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();
   for (auto &Arg : Args) {
      if (ArgNo++ != 0) out << ", ";
      auto ArgVal = cast<RecordVal>(Arg);
      auto Name = cast<StringLiteral>(ArgVal->getRecord()
                                            ->getFieldValue("name"))->getVal();

      auto ArgType = getCXXArgType(ArgVal);

      // check if we need to make a copy
      if (ArgType.endswith("&&")) {
         ArgType = ArgType.drop_back(2);
         out << ArgType << "(" << Name << ")";
      }
      else {
         out << Name;
      }
   }

   if (ArgNo != 0) out << ", ";
   out << "SourceLoc);";

   out << Newline() << "}\n\n";
}

namespace {

class AttrParseEmitter {
public:
   AttrParseEmitter(llvm::raw_ostream &out, RecordKeeper &RK)
      : out(out), RK(RK)
   {}

   void emit();

   struct CurrentArgInfo {
      llvm::StringRef AttrName;
      unsigned ArgNo;
      std::pair<std::string, std::string> &TypeAndName;
   };

   void parseStringArg(llvm::raw_ostream &out,
                       CurrentArgInfo &ArgInfo);
   void parseIntArg(llvm::raw_ostream &out,
                    CurrentArgInfo &ArgInfo);
   void parseFloatArg(llvm::raw_ostream &out,
                      CurrentArgInfo &ArgInfo);
   void parseTypeArg(llvm::raw_ostream &out,
                     CurrentArgInfo &ArgInfo);
   void parseExprArg(llvm::raw_ostream &out,
                     CurrentArgInfo &ArgInfo);
   void parseEnumArg(llvm::raw_ostream &out,
                     CurrentArgInfo &ArgInfo,
                     llvm::StringRef EnumOwner,
                     llvm::ArrayRef<Value*> Cases);

private:
   llvm::raw_ostream &out;
   RecordKeeper &RK;
};

} // anonymous namespace

void AttrParseEmitter::emit()
{
   llvm::SmallVector<Record*, 16> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   out << "#ifdef CDOT_PARSE_ATTR_MAIN" << Newline();

   // this is included inside a switch statement after the type of attribute
   // is known

   for (auto &Attr : Attrs) {
      auto &Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();

      out << "case AttrKind::" << Attr->getName() << ": {" << Newline();
      out << "unsigned NumNeededArgs = " << Args.size() << ";" << Newline();

      std::vector<std::pair<std::string, std::string>> NeededArgVals;

      // reserve space for every needed argument
      for (auto &Arg : Args) {
         auto ArgVal = cast<RecordVal>(Arg);
         auto Name = cast<StringLiteral>(
            ArgVal->getRecord()->getFieldValue("name"))->getVal();

         std::string TypeName = getCXXType(ArgVal);
         if (TypeName.empty()) {
            TypeName += Attr->getName();
            TypeName += "Attr::";
            TypeName += (char)::toupper(Name.front());
            TypeName += Name.substr(1);
            TypeName += "Kind";
         }

         out << TypeName << " " << Name << ";\n";
         NeededArgVals.emplace_back(TypeName, Name);
      }

      // switch over the current argument and emit appropriate parsing code
      // for the argument type
      std::string SwitchStr;
      llvm::raw_string_ostream Switch(SwitchStr);

      unsigned ArgNo = 0;
      for (auto &Arg : Args) {
         Switch << "case " << ArgNo++ << ": {";
         auto C = cast<RecordVal>(Arg)->getRecord()
                     ->getBases().front().getBase();

         CurrentArgInfo ArgInfo{
            cast<StringLiteral>(Attr->getFieldValue("name"))->getVal(),
            ArgNo,
            NeededArgVals[ArgNo - 1]
         };

         if (C->getName() == "IntArg")
            parseIntArg(Switch, ArgInfo);
         else if (C->getName() == "FloatArg")
            parseFloatArg(Switch, ArgInfo);
         else if (C->getName() == "StringArg")
            parseStringArg(Switch, ArgInfo);
         else if (C->getName() == "TypeArg")
            parseTypeArg(Switch, ArgInfo);
         else if (C->getName() == "ExprArg")
            parseExprArg(Switch, ArgInfo);
         else {
            auto &Cases = cast<ListLiteral>(
               cast<RecordVal>(Arg)->getRecord()->getFieldValue("cases"))
               ->getValues();

            std::string Namespace = Attr->getName();
            Namespace += "Attr";

            parseEnumArg(Switch, ArgInfo, Namespace, Cases);
         }

         Switch << "break;\n}\n";
      }

      // if we land in the default case, we have too many arguments
      Switch << R"__(
         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;
)__";

      Switch.flush();

      // build a simple state machine for argument parsing
      out << R"__(
      unsigned ArgNo = 0;

      if (lookahead().is(tok::open_paren)) {
         advance();
         advance();

         while (!currentTok().is(tok::close_paren)) {
            switch (ArgNo++) {
            )__" << SwitchStr << R"__(
            }

            advance();
            if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), true, "')'");

               if (!findTokOnLine(tok::comma, tok::close_paren)) {
                  return skipAttribute();
               }
            }

            if (currentTok().is(tok::comma)) advance();
         }
      }

      if (ArgNo != NumNeededArgs) {
         SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                     NumNeededArgs, ArgNo);
         break;
      }

      auto EndLoc = currentTok().getSourceLoc();
)__";

      out << "Attrs.push_back(new (Context) " << Attr->getName()
                                              << "Attr(";

      unsigned i = 0;
      for (auto &Arg : NeededArgVals) {
         if (i++ != 0) out << ", ";
         out << "std::move(" << Arg.second << ")";
      }

      if (i != 0) out << ", ";
      out << "SourceRange(AttrLoc, EndLoc)));\n break;";

      out << "}" << Newline();
   }

   out << "#endif\n#undef CDOT_PARSE_ATTR_MAIN\n\n";
   out << "#ifdef CDOT_PARSE_ATTR_CHECK\n";

   std::string SwitchStr;
   llvm::raw_string_ostream Switch(SwitchStr);

   Switch << "switch(A->getKind()) {\n";

   for (auto &Attr : Attrs) {
      auto Base = Attr->getBases().front().getBase();
      if (Base->getName() != "DeclAttr")
         continue;

      auto Decls = cast<ListLiteral>(Attr->getFieldValue("Decls"))->getValues();

      Switch << "case AttrKind::" << Attr->getName() << ": {";

      for (auto &D : Decls) {
         Switch << "if (isa<" << cast<RecordVal>(D)->getRecord()->getName()
                              << ">(Decl)) break;\n";
      }

      Switch << "SP.diagnose(err_attribute_not_valid_here,"
                "            A->getSourceRange(), \""
          << cast<StringLiteral>(Attr->getFieldValue("name"))->getVal()
          << "\");\n";

      Switch << "}\n";
   }

   Switch << "default: break; \n}\n";
   Switch.flush();

   out << "auto Decl = Result.getDecl();\n";
   out << SwitchStr;

   out << "#endif\n#undef CDOT_PARSE_ATTR_CHECK\n\n";
}

void AttrParseEmitter::parseIntArg(llvm::raw_ostream &out,
                                   CurrentArgInfo &ArgInfo) {
   out << R"__(
   if (!currentTok().is(tok::integerliteral)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__" << ArgInfo.AttrName << R"__(", 0 /*integer literal */,
                  )__" << ArgInfo.ArgNo << R"__();

      break;
   }

   auto text = currentTok().getText();
   uint8_t radix = 10;
   unsigned offset = 0;

   if (text[0] == '0') {
      if (text.size() > 1) {
         if (text[1] == 'x' || text[1] == 'X') {
            offset = 2;
            radix = 16;
         }
         else if (text[1] == 'b' || text[1] == 'B') {
            offset = 2;
            radix = 2;
         }
         else {
            offset = 1;
            radix = 8;
         }
      }
   }

   auto API = llvm::APSInt(llvm::APInt(64, text.substr(offset), radix),
                           false);
)__";

   out << ArgInfo.TypeAndName.second << " = std::move(API);\n";
}

void AttrParseEmitter::parseFloatArg(llvm::raw_ostream &out,
                                     AttrParseEmitter::CurrentArgInfo &ArgInfo){
   out << R"__(
   if (!currentTok().is(tok::fpliteral)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__" << ArgInfo.AttrName << R"__(", 1 /*float literal */,
                  )__" << ArgInfo.ArgNo << R"__();

      break;
   }

   auto text = currentTok().getText();
   llvm::APFloat APF(0.0);
   auto res = APF.convertFromString(text,
                                    llvm::APFloat::rmNearestTiesToEven);

   switch (res) {
      default:
         break;
      case llvm::APFloat::opInexact:
         SP.diagnose(warn_inexact_fp, currentTok().getSourceLoc());
         break;
   }
)__";

   out << ArgInfo.TypeAndName.second << " = std::move(APF);\n";
}

void AttrParseEmitter::parseStringArg(llvm::raw_ostream &out,
                                      AttrParseEmitter::CurrentArgInfo &ArgInfo){
   out << R"__(
   if (!currentTok().is(tok::stringliteral)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__" << ArgInfo.AttrName << R"__(", 2 /*string literal */,
                  )__" << ArgInfo.ArgNo << R"__();

      break;
   }
)__";

   out << ArgInfo.TypeAndName.second
       << " = prepareStringLiteral(currentTok());\n";
}

void AttrParseEmitter::parseExprArg(llvm::raw_ostream &out,
                                    AttrParseEmitter::CurrentArgInfo &ArgInfo) {
   out << R"__(
   auto exprResult = parseExprSequence();
   if (exprResult.holdsExpr())
)__";

   out << ArgInfo.TypeAndName.second
       << " = StaticExpr::Create(Context, exprResult.getExpr());\n";
}

void AttrParseEmitter::parseTypeArg(llvm::raw_ostream &out,
                                    AttrParseEmitter::CurrentArgInfo &ArgInfo) {
   out << R"__(
   auto typeResult = parseType();
   if (typeResult)
)__";

   out << ArgInfo.TypeAndName.second << " = typeResult.get();\n";
}

void AttrParseEmitter::parseEnumArg(llvm::raw_ostream &out,
                                    AttrParseEmitter::CurrentArgInfo &ArgInfo,
                                    llvm::StringRef EnumOwner,
                                    llvm::ArrayRef<Value *> Cases) {
   std::string DiagStr;
   size_t i = 0;
   for (auto &Case : Cases) {
      if (i++ != 0) DiagStr += ", ";
      DiagStr += cast<StringLiteral>(Case)->getVal();
   }

   out << R"__(
   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__" << ArgInfo.AttrName << R"__( ", 5 /*one of */,
                  )__" << ArgInfo.ArgNo << ", \"" << DiagStr << R"__(");

      break;
   }

   auto Ident = currentTok().getIdentifier();
)__";

   out << ArgInfo.TypeAndName.first << " _enumKind;\n";

   i = 0;
   for (auto &Case : Cases) {
      if (i++ != 0) out << "else ";
      out << "if (Ident==\"" << cast<StringLiteral>(Case)->getVal() << "\") {";
      out << " _enumKind = " << EnumOwner << "::"
          << cast<StringLiteral>(Case)->getVal() << "; }\n";
   }

   out << R"__(
   else {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__" << ArgInfo.AttrName << R"__(", 5 /*one of */,
                  )__" << ArgInfo.ArgNo << ", \"" << DiagStr << R"__(");

      break;
   }
)__";

   out << ArgInfo.TypeAndName.second << " = _enumKind;\n";
}

extern "C" {

void EmitAttributeDefs(llvm::raw_ostream &out, RecordKeeper &RK)
{
   AttrDefEmitter(out, RK).emit();
}

void EmitAttributeClasses(llvm::raw_ostream &out, RecordKeeper &RK)
{
   AttrClassEmitter(out, RK).emit();
}

void EmitAttributeParse(llvm::raw_ostream &out, RecordKeeper &RK)
{
   AttrParseEmitter(out, RK).emit();
}

};