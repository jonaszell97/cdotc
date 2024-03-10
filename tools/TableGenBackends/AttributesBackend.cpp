
#include "tblgen/Record.h"
#include "tblgen/Support/Casting.h"
#include "tblgen/Support/StringSwitch.h"
#include "tblgen/Type.h"
#include "tblgen/Value.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>

using namespace tblgen;
using namespace tblgen::support;

using std::string;

namespace {

struct Tab {
   explicit Tab(unsigned Size = 3) : TabSize(Size) {}
   unsigned TabSize;
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& out, Tab T)
{
   for (unsigned i = 0; i < T.TabSize; ++i)
      out << ' ';

   return out;
}

struct Newline {
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& out, Newline)
{
   out << '\n';
   return out;
}

class AttrDefEmitter {
public:
   AttrDefEmitter(llvm::raw_ostream& out, RecordKeeper& RK) : out(out), RK(RK)
   {
   }

   void emit();

private:
   llvm::raw_ostream& out;
   RecordKeeper& RK;
};

} // anonymous namespace

void AttrDefEmitter::emit()
{
   static constexpr auto AttrMacro = "CDOT_ATTR";
   static constexpr auto TypeAttrMacro = "CDOT_TYPE_ATTR";
   static constexpr auto DeclAttrMacro = "CDOT_DECL_ATTR";
   static constexpr auto ExprAttrMacro = "CDOT_EXPR_ATTR";
   static constexpr auto StmtAttrMacro = "CDOT_STMT_ATTR";
   static constexpr auto AbstractMacro = "CDOT_ABSTRACT_ATTR";
   static constexpr auto VisitMacro = "CDOT_VISITATION_POINT";

   out << "#ifdef " << AttrMacro << "\n";
   out << "#   define " << TypeAttrMacro << "(Name, Spelling) " << AttrMacro
       << "(Name, Spelling)\n";
   out << "#   define " << DeclAttrMacro << "(Name, Spelling) " << AttrMacro
       << "(Name, Spelling)\n";
   out << "#   define " << ExprAttrMacro << "(Name, Spelling) " << AttrMacro
       << "(Name, Spelling)\n";
   out << "#   define " << StmtAttrMacro << "(Name, Spelling) " << AttrMacro
       << "(Name, Spelling)\n";
   out << "#endif\n\n";

   out << "#ifndef " << AbstractMacro << "\n"
       << "#   define " << AbstractMacro << "(Name)\n"
       << "#endif\n\n";

   out << "#ifndef " << VisitMacro << "\n"
       << "#   define " << VisitMacro << "(Name)\n"
       << "#endif\n\n";

   std::vector<Record*> Attrs;

   // Decl Attributes
   Attrs.clear();
   RK.getAllDefinitionsOf(RK.lookupClass("DeclAttr"), Attrs);

   out << "#ifdef " << DeclAttrMacro << "\n";
   out << "   " << AbstractMacro << "(_firstDeclAttr)\n";

   for (auto& Attr : Attrs) {
      auto Spelling
          = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
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

   for (auto& Attr : Attrs) {
      auto Spelling
          = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
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

   for (auto& Attr : Attrs) {
      auto Spelling
          = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
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

   for (auto& Attr : Attrs) {
      auto Spelling
          = cast<StringLiteral>(Attr->getFieldValue("name"))->getVal();
      out << "   " << TypeAttrMacro << "(" << Attr->getName() << ", "
          << Spelling << ")\n";
   }

   out << "   " << AbstractMacro << "(_lastTypeAttr)\n";
   out << "#endif\n#undef " << TypeAttrMacro << "\n\n";

   std::vector<Record*> VisitPoints;
   RK.getAllDefinitionsOf("VisitationPoint", VisitPoints);

   for (auto& VP : VisitPoints) {
      out << VisitMacro << "(" << VP->getName() << ")\n";
   }

   out << "\n#undef " << AbstractMacro << "\n";
   out << "#undef " << VisitMacro << "\n";
   out << "#undef " << AttrMacro;
}

namespace {

class AttrClassEmitter {
public:
   AttrClassEmitter(llvm::raw_ostream& out, RecordKeeper& RK) : out(out), RK(RK)
   {
   }

   void emit();
   void emitAttrDecl(Record* Attr, Class* Base);
   void emitGetVisitationPoint();
   void emitIsInherited();

   void emitPrintPrettyImpl(Record* Attr, Class* Base);
   void emitCloneImpl(Record* Attr, Class* Base);

   void emitAttrSema(std::vector<Record*>& Attrs);

private:
   llvm::raw_ostream& out;
   RecordKeeper& RK;
};

llvm::StringRef getCXXType(RecordVal* Arg)
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

llvm::StringRef getCXXGetterType(RecordVal* Arg)
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

llvm::StringRef getCXXArgType(RecordVal* Arg)
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

std::string buildDefaultValue(RecordVal* Arg, Value* Default)
{
   std::string str;
   auto C = Arg->getRecord()->getBases().front().getBase();
   if (C->getName() == "IntArg") {
      str += "llvm::APSInt(llvm::APInt(64, ";
      str += std::to_string(cast<IntegerLiteral>(Default)->getVal());
      str += "), false)";

      return str;
   }

   if (C->getName() == "FloatArg") {
      str += "llvm::APFloat(";
      str += std::to_string(cast<FPLiteral>(Default)->getVal());
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
   std::vector<Record*> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   // class declarations
   out << "#ifdef CDOT_ATTR_DECL\n";

   size_t i = 0;
   for (auto& Attr : Attrs) {
      if (i++ != 0)
         out << "\n\n";

      auto Base = Attr->getBases().front().getBase();
      emitAttrDecl(Attr, Base);
   }

   out << "\n#endif\n#undef CDOT_ATTR_DECL\n\n";

   // class implementations
   out << "#ifdef CDOT_ATTR_IMPL\n";

   emitGetVisitationPoint();
   emitIsInherited();

   i = 0;
   for (auto& Attr : Attrs) {
      if (i++ != 0)
         out << "\n\n";

      auto Base = Attr->getBases().front().getBase();
      emitPrintPrettyImpl(Attr, Base);
      emitCloneImpl(Attr, Base);
   }

   out << "\n#endif\n#undef CDOT_ATTR_IMPL\n\n";

   // Sema

   out << "\n#ifdef CDOT_ATTR_SEMA\n\n";
   emitAttrSema(Attrs);
   out << "\n#endif\n#undef CDOT_ATTR_SEMA\n\n";
}

void AttrClassEmitter::emitAttrDecl(Record* Attr, Class* Base)
{
   std::string ClassName = Attr->getName();
   ClassName += "Attr";

   out << "class " << ClassName << ": public " << Base->getName() << " {\n";

   out << "public:" << Newline() << Tab();

   out << "static bool classof(const Attr* A) "
          "{ return classofKind(A->getKind()); }"
       << Newline() << Tab();

   out << "static bool classofKind(AttrKind kind)"
          "{ return kind == AttrKind::"
       << Attr->getName() << "; }" << Newline() << Newline();

   out << R"__(
   void printPretty(llvm::raw_ostream &out) const;
   )__" << ClassName
       << R"__( *clone(ast::ASTContext &Ctx) const;
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
   size_t ArgNo = 0;

   auto Args = cast<ListLiteral>(Attr->getFieldValue("Args"));

   for (auto& Arg : Args->getValues()) {
      if (ArgNo++ != 0) {
         out << "\n";
         GetterStr << "\n";
      };

      auto ArgVal = cast<RecordVal>(Arg);
      auto Name
          = cast<StringLiteral>(ArgVal->getRecord()->getFieldValue("name"))
                ->getVal();

      auto TypeName = getCXXType(ArgVal);
      auto GetterTypeName = getCXXGetterType(ArgVal);
      auto ArgTypeName = getCXXArgType(ArgVal);
      std::string EnumName;

      // enum argument, introduce new enum
      if (TypeName.empty()) {
         if (NumEnumArgs++ != 0)
            EnumStr << "\n\n";

         EnumName = std::string(1, ::toupper(Name.front()));
         EnumName += Name.substr(1);
         EnumName += "Kind";

         EnumStr << Tab() << "enum " << EnumName << " {" << Newline();

         size_t i = 0;
         auto& CaseVals
             = cast<ListLiteral>(ArgVal->getRecord()->getFieldValue("cases"))
                   ->getValues();

         for (auto& Case : CaseVals) {
            if (i++ != 0)
               EnumStr << "," << Newline();
            EnumStr << Tab() << Tab() << cast<StringLiteral>(Case)->getVal();
         }

         EnumStr << Newline() << Tab() << "};";

         TypeName = EnumName;
         GetterTypeName = EnumName;
         ArgTypeName = EnumName;
      }

      FieldStr << Tab() << TypeName << " " << Name << ";";
      GetterStr << Tab() << GetterTypeName << " get"
                << (char)::toupper(Name.front()) << Name.substr(1)
                << "() const { return " << Name << "; }";

      // build constructor arguments and initialization
      std::string constructorArg = ArgTypeName.str();
      constructorArg += " ";
      constructorArg += Name;

      bool IsOptional
          = cast<IntegerLiteral>(ArgVal->getRecord()->getFieldValue("optional"))
                ->getVal()
            != 0;

      if (IsOptional) {
         if (ArgVal->getRecord()->hasField("defaultVal")) {
            constructorArg += " = ";
            constructorArg += buildDefaultValue(
                ArgVal, ArgVal->getRecord()->getFieldValue("defaultVal"));
         }
      }

      std::string init = Name;
      init += "(";

      if (ArgTypeName.ends_with("&&")) {
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
   for (auto& Arg : ConstructorArgs) {
      if (NumCtorArgs++ != 0)
         out << ", ";
      out << Arg.first;
   }

   if (NumCtorArgs != 0)
      out << ", ";
   out << "SourceRange SR = SourceRange())";

   out << "\n" << Tab() << Tab() << ": ";
   out << Base->getName() << "(AttrKind::" << Attr->getName() << ", SR)";

   for (auto& Arg : ConstructorArgs) {
      out << ", ";
      out << Arg.second;
   }

   out << Newline() << Tab() << "{}";
   out << Newline() << "};";
}

void AttrClassEmitter::emitGetVisitationPoint()
{
   std::unordered_map<Record*, std::vector<Record*>> AttrMap;

   std::vector<Record*> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   for (auto& Attr : Attrs) {
      auto VP = cast<RecordVal>(Attr->getFieldValue("Visit"))->getRecord();
      AttrMap[VP].push_back(Attr);
   }

   std::string CaseStr;
   llvm::raw_string_ostream CS(CaseStr);

   for (auto& Vec : AttrMap) {
      for (auto& Attr : Vec.second) {
         CS << Tab() << "case AttrKind::" << Attr->getName() << ":\n";
      }

      CS << Tab() << Tab() << "return " << Vec.first->getName() << ";\n";
   }

   CS.flush();

   out << R"__(
   Attr::VisitationPoint Attr::getVisitationPoint() const {
      switch (kind) {
      default: llvm_unreachable("bad attr kind!");
      )__"
       << CaseStr << R"__(
      }
   }
)__";
}

void AttrClassEmitter::emitIsInherited()
{
   std::unordered_map<Record*, bool> AttrMap;

   std::vector<Record*> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   for (auto& Attr : Attrs) {
      bool IsInherited
          = cast<IntegerLiteral>(Attr->getFieldValue("Inherited"))->getVal()
            != 0;

      AttrMap[Attr] = IsInherited;
   }

   std::string CaseStr;
   llvm::raw_string_ostream CS(CaseStr);

   for (auto& Pair : AttrMap) {
      CS << Tab() << "case AttrKind::" << Pair.first->getName() << ":\n";
      CS << Tab() << Tab() << "return " << (Pair.second ? "true" : "false")
         << ";\n";
   }

   CS.flush();

   out << R"__(
   bool Attr::isInherited() const {
      switch (kind) {
      default: llvm_unreachable("bad attr kind!");
      )__"
       << CaseStr << R"__(
      }
   }
)__";
}

void AttrClassEmitter::emitPrintPrettyImpl(Record* Attr, Class*)
{
   std::string ClassName = Attr->getName();
   ClassName += "Attr";

   // emit 'printPretty' implementation
   out << "void " << ClassName << "::printPretty(llvm::raw_ostream &out)"
       << " const {" << Newline();

   std::string PrettyPrintArgs;
   llvm::raw_string_ostream ArgStr(PrettyPrintArgs);

   size_t ArgNo = 0;
   auto& Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();

   std::string caseStrings;
   llvm::raw_string_ostream caseStr(caseStrings);

   if (!Args.empty()) {
      for (auto& Arg : Args) {
         if (ArgNo++ != 0)
            ArgStr << " << \", \" << ";
         auto ArgVal = cast<RecordVal>(Arg);
         auto Name
             = cast<StringLiteral>(ArgVal->getRecord()->getFieldValue("name"))
                   ->getVal();

         auto TypeName = getCXXType(ArgVal);
         if (TypeName.empty()) {
            // enum argument, print cases
            std::string varName = Name;
            varName += "Str";

            caseStr << "llvm::StringRef " << varName << ";\n";

            // switch over the cases to get the case name
            auto& CaseVals
                = cast<ListLiteral>(ArgVal->getRecord()->getFieldValue("cases"))
                      ->getValues();

            caseStr << "switch (" << Name << ") {" << Newline();

            for (auto& Case : CaseVals) {
               auto CaseName = cast<StringLiteral>(Case)->getVal();
               ;
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
      out << "out << '(' << " << PrettyPrintArgs << " << ')';" << Newline();
   }

   out << Newline() << "}\n\n";
}

void AttrClassEmitter::emitCloneImpl(Record* Attr, Class*)
{
   std::string ClassName = Attr->getName();
   ClassName += "Attr";

   // emit 'printPretty' implementation
   out << ClassName << " *" << ClassName << "::clone(ast::ASTContext &Ctx)"
       << " const {" << Newline();

   out << Tab() << "return new(Ctx) " << ClassName << "(";

   size_t ArgNo = 0;
   auto& Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();
   for (auto& Arg : Args) {
      if (ArgNo++ != 0)
         out << ", ";
      auto ArgVal = cast<RecordVal>(Arg);
      auto Name
          = cast<StringLiteral>(ArgVal->getRecord()->getFieldValue("name"))
                ->getVal();

      auto ArgType = getCXXArgType(ArgVal);

      // check if we need to make a copy
      if (ArgType.ends_with("&&")) {
         ArgType = ArgType.drop_back(2);
         out << ArgType << "(" << Name << ")";
      }
      else {
         out << Name;
      }
   }

   if (ArgNo != 0)
      out << ", ";
   out << "SourceLoc);";

   out << Newline() << "}\n\n";
}

void AttrClassEmitter::emitAttrSema(std::vector<Record*>& Attrs)
{
   for (auto& Attr : Attrs) {
      auto VisitNever = cast<RecordVal>(Attr->getFieldValue("Visit"))
                            ->getRecord()
                            ->getName()
                        == "Never";

      if (!VisitNever)
         continue;

      auto BaseName = Attr->getBases().front().getBase()->getName();
      llvm::StringRef Kind = StringSwitch<llvm::StringRef>(BaseName)
                                 .Case("DeclAttr", "Decl")
                                 .Case("ExprAttr", "Expression")
                                 .Case("TypeAttr", "Expression")
                                 .Case("StmtAttr", "Statement")
                                 .Default("");

      assert(!Kind.empty() && "bad attribute kind!");

      out << "void SemaPass::check" << Attr->getName() << "Attr(" << Kind
          << "*, " << Attr->getName() << "Attr*) {}\n";
   }
}

namespace {

class AttrParseEmitter {
public:
   AttrParseEmitter(llvm::raw_ostream& out, RecordKeeper& RK) : out(out), RK(RK)
   {
   }

   void emit();

   struct CurrentArgInfo {
      llvm::StringRef AttrName;
      unsigned ArgNo;
      std::pair<std::string, std::string>& TypeAndName;
      Record* Arg;
   };

   void parseStringArg(llvm::raw_ostream& out, CurrentArgInfo& ArgInfo);
   void parseIntArg(llvm::raw_ostream& out, CurrentArgInfo& ArgInfo);
   void parseFloatArg(llvm::raw_ostream& out, CurrentArgInfo& ArgInfo);
   void parseTypeArg(llvm::raw_ostream& out, CurrentArgInfo& ArgInfo);
   void parseExprArg(llvm::raw_ostream& out, CurrentArgInfo& ArgInfo);
   void parseEnumArg(llvm::raw_ostream& out, CurrentArgInfo& ArgInfo,
                     llvm::StringRef EnumOwner, llvm::ArrayRef<Value*> Cases);

   void readStringDefault(llvm::raw_ostream& out,
                          std::pair<std::string, std::string>& TypeAndName,
                          Value* DefaultVal);
   void readIntDefault(llvm::raw_ostream& out,
                       std::pair<std::string, std::string>& TypeAndName,
                       Value* DefaultVal);
   void readFloatDefault(llvm::raw_ostream& out,
                         std::pair<std::string, std::string>& TypeAndName,
                         Value* DefaultVal);
   void readEnumDefault(llvm::raw_ostream& out,
                        std::pair<std::string, std::string>& TypeAndName,
                        llvm::StringRef EnumOwner, Value* DefaultVal);

private:
   llvm::raw_ostream& out;
   RecordKeeper& RK;
};

} // anonymous namespace

void AttrParseEmitter::emit()
{
   std::vector<Record*> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   out << "#ifdef CDOT_PARSE_ATTR_MAIN" << Newline();

   // this is included inside a switch statement after the type of attribute
   // is known

   for (auto& Attr : Attrs) {
      auto& Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();

      out << "case AttrKind::" << Attr->getName() << ": {" << Newline();
      std::vector<std::pair<std::string, std::string>> NeededArgVals;

      // reserve space for every needed argument
      for (auto& Arg : Args) {
         auto ArgVal = cast<RecordVal>(Arg);
         auto Name
             = cast<StringLiteral>(ArgVal->getRecord()->getFieldValue("name"))
                   ->getVal();

         std::string TypeName = getCXXType(ArgVal).str();
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
      for (auto& Arg : Args) {
         Switch << "case " << ArgNo++ << ": {";
         auto C
             = cast<RecordVal>(Arg)->getRecord()->getBases().front().getBase();

         CurrentArgInfo ArgInfo{
             cast<StringLiteral>(Attr->getFieldValue("name"))->getVal(), ArgNo,
             NeededArgVals[ArgNo - 1], cast<RecordVal>(Arg)->getRecord()};

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
            auto& Cases
                = cast<ListLiteral>(
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
            if (lookahead().is(tok::close_paren)) advance();
            if (!lookahead().is(tok::eof))
               advance();

            return;
)__";

      Switch.flush();

      std::vector<Value*> DefaultVals;
      unsigned FirstDefault = unsigned(-1);
      unsigned i = 0;

      for (auto& Arg : Args) {
         auto Optional
             = cast<IntegerLiteral>(
                   cast<RecordVal>(Arg)->getRecord()->getFieldValue("optional"))
                   ->getVal()
               != 0;

         if (Optional) {
            if (FirstDefault == -1)
               FirstDefault = i;

            DefaultVals.push_back(
                cast<RecordVal>(Arg)->getRecord()->getFieldValue("defaultVal"));
         }

         ++i;
      }

      std::string DefaultStr;
      if (FirstDefault != -1) {
         llvm::raw_string_ostream OS(DefaultStr);
         OS << R"__(
         while (ArgNo < )__"
            << (FirstDefault + 1) << R"__() {
            switch (ArgNo) {
            default: llvm_unreachable("no default argument");
)__";

         unsigned j = FirstDefault;
         while (j < Args.size()) {
            auto& Arg = Args[j];
            auto& ArgInfo = NeededArgVals[j];
            auto* DefaultVal = DefaultVals[j - FirstDefault];

            OS << "case " << j++ << ": {\n";

            auto C = cast<RecordVal>(Arg)
                         ->getRecord()
                         ->getBases()
                         .front()
                         .getBase();

            if (C->getName() == "IntArg")
               readIntDefault(OS, ArgInfo, DefaultVal);
            else if (C->getName() == "FloatArg")
               readFloatDefault(OS, ArgInfo, DefaultVal);
            else if (C->getName() == "StringArg")
               readStringDefault(OS, ArgInfo, DefaultVal);
            else {
               std::string Namespace = Attr->getName();
               Namespace += "Attr";

               readEnumDefault(OS, ArgInfo, Namespace, DefaultVal);
            }

            OS << "\nArgNo++;\nbreak;\n}\n";
         }

         OS << R"__(
            }
         }
)__";
      }

      bool SkipArgs
          = cast<IntegerLiteral>(Attr->getFieldValue("IgnoreFollowingParens"))
                ->getVal()
            != 0;

      if (!SkipArgs) {
         // build a simple state machine for argument parsing
         out << "unsigned NumNeededArgs = " << Args.size() << ";" << Newline();
         out << R"__(
         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               )__"
             << SwitchStr << R"__(
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

         )__" << DefaultStr << R"__(
         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      )__";
      }

      out << "auto EndLoc = currentTok().getSourceLoc();\n"
          << "Attrs.push_back(new (Context) " << Attr->getName() << "Attr(";

      i = 0;
      for (auto& Arg : NeededArgVals) {
         if (i++ != 0)
            out << ", ";
         out << "std::move(" << Arg.second << ")";
      }

      if (i != 0)
         out << ", ";
      out << "SourceRange(AttrLoc, EndLoc)));\n break;";

      out << "}" << Newline();
   }

   out << "#endif\n#undef CDOT_PARSE_ATTR_MAIN\n\n";
   out << "#ifdef CDOT_PARSE_ATTR_CHECK\n";

   std::string SwitchStr;
   llvm::raw_string_ostream Switch(SwitchStr);

   Switch << "switch(A->getKind()) {\n";

   for (auto& Attr : Attrs) {
      auto Base = Attr->getBases().front().getBase();
      if (Base->getName() != "DeclAttr")
         continue;

      auto Decls = cast<ListLiteral>(Attr->getFieldValue("Decls"))->getValues();

      Switch << "case AttrKind::" << Attr->getName() << ": {";

      for (auto& D : Decls) {
         Switch << "if (isa<" << cast<RecordVal>(D)->getRecord()->getName()
                << ">(D)) break;\n";
      }

      Switch << "SP.diagnose(err_attribute_not_valid_here,"
                "            A->getSourceRange(), \""
             << cast<StringLiteral>(Attr->getFieldValue("name"))->getVal()
             << "\"); break;\n";

      Switch << "}\n";
   }

   Switch << "default: break; \n}\n";
   Switch.flush();

   out << "auto D = Result.getDecl();\n";
   out << SwitchStr;

   out << "#endif\n#undef CDOT_PARSE_ATTR_CHECK\n\n";
}

void AttrParseEmitter::parseIntArg(llvm::raw_ostream& out,
                                   CurrentArgInfo& ArgInfo)
{
   out << R"__(
   if (!currentTok().is(tok::integerliteral)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__"
       << ArgInfo.AttrName << R"__(", 0 /*integer literal */,
                  )__" << ArgInfo.ArgNo
       << R"__();

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

void AttrParseEmitter::parseFloatArg(llvm::raw_ostream& out,
                                     AttrParseEmitter::CurrentArgInfo& ArgInfo)
{
   out << R"__(
   if (!currentTok().is(tok::fpliteral)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__"
       << ArgInfo.AttrName << R"__(", 1 /*float literal */,
                  )__" << ArgInfo.ArgNo
       << R"__();

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

void AttrParseEmitter::parseStringArg(llvm::raw_ostream& out,
                                      AttrParseEmitter::CurrentArgInfo& ArgInfo)
{
   out << R"__(
   if (!currentTok().is(tok::stringliteral)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__"
       << ArgInfo.AttrName << R"__(", 2 /*string literal */,
                  )__" << ArgInfo.ArgNo
       << R"__();

      break;
   }
)__";

   out << ArgInfo.TypeAndName.second
       << " = prepareStringLiteral(currentTok());\n";
}

void AttrParseEmitter::parseExprArg(llvm::raw_ostream& out,
                                    AttrParseEmitter::CurrentArgInfo& ArgInfo)
{
   out << R"__(
   auto exprResult = parseExprSequence();
   if (exprResult.holdsExpr())
)__";

   out << ArgInfo.TypeAndName.second
       << " = StaticExpr::Create(Context, exprResult.getExpr());\n";
}

void AttrParseEmitter::parseTypeArg(llvm::raw_ostream& out,
                                    AttrParseEmitter::CurrentArgInfo& ArgInfo)
{
   out << R"__(
   auto typeResult = parseType();
   if (typeResult)
)__";

   out << ArgInfo.TypeAndName.second << " = typeResult.get();\n";
}

void AttrParseEmitter::parseEnumArg(llvm::raw_ostream& out,
                                    AttrParseEmitter::CurrentArgInfo& ArgInfo,
                                    llvm::StringRef EnumOwner,
                                    llvm::ArrayRef<Value*> Cases)
{
   std::string DiagStr;
   size_t i = 0;
   for (auto& Case : Cases) {
      if (i++ != 0)
         DiagStr += ", ";
      DiagStr += cast<StringLiteral>(Case)->getVal();
   }

   out << R"__(
   StringRef Ident;
   if (currentTok().is(tok::times)) {
      Ident = "*";
   } else if (currentTok().is(tok::ident)) {
      Ident = currentTok().getIdentifier();
   } else {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__"
       << ArgInfo.AttrName << R"__(", 5 /*one of */,
                  )__" << ArgInfo.ArgNo << ", \""
       << DiagStr << R"__(");

      break;
   }
)__";

   out << ArgInfo.TypeAndName.first << " _enumKind;\n";

   i = 0;
   for (auto& Case : Cases) {
      if (i++ != 0)
         out << "else ";
      out << "if (Ident==\"" << cast<StringLiteral>(Case)->getVal() << "\") {";
      out << " _enumKind = " << EnumOwner
          << "::" << cast<StringLiteral>(Case)->getVal() << "; }\n";
   }

   // aliases
   auto ArgAliases = cast<ListLiteral>(ArgInfo.Arg->getFieldValue("aliases"));
   for (auto& aliasVal : ArgAliases->getValues()) {
      auto Val = cast<RecordVal>(aliasVal)->getRecord();
      auto AliasName = cast<StringLiteral>(Val->getFieldValue("alias"));
      auto AliaseeName = cast<StringLiteral>(Val->getFieldValue("aliasee"));

      if (i++ != 0)
         out << "else ";
      out << "if (Ident==\"" << AliasName->getVal() << "\") {";
      out << " _enumKind = " << EnumOwner << "::" << AliaseeName->getVal()
          << "; }\n";
   }

   out << R"__(
   else {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  ")__"
       << ArgInfo.AttrName << R"__(", 5 /*one of */,
                  )__" << ArgInfo.ArgNo << ", \""
       << DiagStr << R"__(");

      break;
   }
)__";

   out << ArgInfo.TypeAndName.second << " = _enumKind;\n";
}

void AttrParseEmitter::readStringDefault(llvm::raw_ostream& out,
                                         std::pair<string, string>& TypeAndName,
                                         Value* DefaultVal)
{
   auto* Val = cast<StringLiteral>(DefaultVal);
   out << TypeAndName.second << " = R\"__(" << Val->getVal() << ")__\";";
}

void AttrParseEmitter::readIntDefault(llvm::raw_ostream& out,
                                      std::pair<string, string>& TypeAndName,
                                      Value* DefaultVal)
{
   auto* Val = cast<IntegerLiteral>(DefaultVal);
   out << TypeAndName.second << " = ";

   auto* type = cast<IntType>(Val->getType());
   auto bitwidth = type->getBitWidth();
   auto isUnsigned = type->isUnsigned();

   if (bitwidth <= 64) {
      out << "llvm::APSInt(llvm::APInt(" << bitwidth << ", " << Val->getVal()
          << ", " << !isUnsigned << ")";
   }
   else {
      out << "llvm::APSInt(llvm::APInt(" << bitwidth << ", "
          << "R\"__(" << Val->getVal() << ")__\", " << !isUnsigned << ")";
   }

   out << ";\n";
}

void AttrParseEmitter::readFloatDefault(llvm::raw_ostream& out,
                                        std::pair<string, string>& TypeAndName,
                                        Value* DefaultVal)
{
   out << TypeAndName.second << " = llvm::APFloat(";
   out << cast<FPLiteral>(DefaultVal)->getVal();
   out << ");\n";
}

void AttrParseEmitter::readEnumDefault(llvm::raw_ostream& out,
                                       std::pair<string, string>& TypeAndName,
                                       llvm::StringRef EnumOwner,
                                       Value* DefaultVal)
{
   out << TypeAndName.second << " = " << EnumOwner << "::" << TypeAndName.first
       << "::" << cast<StringLiteral>(DefaultVal)->getVal() << ";\n";
}

namespace {

class AttrSerializeEmitter {
public:
   AttrSerializeEmitter(llvm::raw_ostream& out, RecordKeeper& RK)
       : out(out), RK(RK)
   {
   }

   void emit();

private:
   llvm::raw_ostream& out;
   RecordKeeper& RK;

   void emitSerialize(llvm::ArrayRef<Record*> Attrs);
   void emitDeserialize(llvm::ArrayRef<Record*> Attrs);

   void writeStringArg(llvm::raw_ostream& out, llvm::StringRef GetterName);
   void writeIntArg(llvm::raw_ostream& out, llvm::StringRef GetterName);
   void writeFloatArg(llvm::raw_ostream& out, llvm::StringRef GetterName);
   void writeTypeArg(llvm::raw_ostream& out, llvm::StringRef GetterName);
   void writeExprArg(llvm::raw_ostream& out, llvm::StringRef GetterName);
   void writeEnumArg(llvm::raw_ostream& out, llvm::StringRef GetterName);

   void readStringArg(llvm::raw_ostream& out);
   void readIntArg(llvm::raw_ostream& out);
   void readFloatArg(llvm::raw_ostream& out);
   void readTypeArg(llvm::raw_ostream& out);
   void readExprArg(llvm::raw_ostream& out);
   void readEnumArg(llvm::raw_ostream& out, llvm::StringRef EnumName);
};

} // anonymous namespace

void AttrSerializeEmitter::emit()
{
   std::vector<Record*> Attrs;
   RK.getAllDefinitionsOf("Attr", Attrs);

   out << "#ifdef CDOT_ATTR_SERIALIZE" << Newline();
   emitSerialize(Attrs);
   out << "#endif" << Newline();

   out << "#ifdef CDOT_ATTR_DESERIALIZE" << Newline();
   emitDeserialize(Attrs);
   out << "#endif" << Newline();

   out << "#undef CDOT_ATTR_SERIALIZE\n";
   out << "#undef CDOT_ATTR_DESERIALIZE\n";
}

void AttrSerializeEmitter::writeStringArg(llvm::raw_ostream& out,
                                          llvm::StringRef GetterName)
{
   out << "Record.AddIdentifierRef(&Idents.get(A->" << GetterName << "()));\n";
}

void AttrSerializeEmitter::writeIntArg(llvm::raw_ostream& out,
                                       llvm::StringRef GetterName)
{
   out << "Record.AddAPSInt(A->" << GetterName << "());\n";
}

void AttrSerializeEmitter::writeFloatArg(llvm::raw_ostream& out,
                                         llvm::StringRef GetterName)
{
   out << "Record.AddAPFloat(A->" << GetterName << "());\n";
}

void AttrSerializeEmitter::writeTypeArg(llvm::raw_ostream& out,
                                        llvm::StringRef GetterName)
{
   out << "Record.AddType(A->" << GetterName << "());\n";
}

void AttrSerializeEmitter::writeExprArg(llvm::raw_ostream& out,
                                        llvm::StringRef GetterName)
{
   out << "Record.AddStmt(A->" << GetterName << "());\n";
}

void AttrSerializeEmitter::writeEnumArg(llvm::raw_ostream& out,
                                        llvm::StringRef GetterName)
{
   out << "Record.push_back((uint64_t)A->" << GetterName << "());\n";
}

void AttrSerializeEmitter::readStringArg(llvm::raw_ostream& out)
{
   out << "Record.getIdentifierInfo()->getIdentifier()";
}

void AttrSerializeEmitter::readIntArg(llvm::raw_ostream& out)
{
   out << "Record.readAPSInt()";
}

void AttrSerializeEmitter::readFloatArg(llvm::raw_ostream& out)
{
   out << "Record.readAPFloat()";
}

void AttrSerializeEmitter::readTypeArg(llvm::raw_ostream& out)
{
   out << "Record.readSourceType()";
}

void AttrSerializeEmitter::readExprArg(llvm::raw_ostream& out)
{
   out << "cast<StaticExpr>(Record.readExpr())";
}

void AttrSerializeEmitter::readEnumArg(llvm::raw_ostream& out,
                                       llvm::StringRef EnumName)
{
   out << "Record.readEnum<" << EnumName << ">()";
}

void AttrSerializeEmitter::emitSerialize(llvm::ArrayRef<Record*> Attrs)
{
   llvm::SmallString<64> GetterName;
   for (auto& Attr : Attrs) {
      out << "void ASTAttrWriter::visit" << Attr->getName() << "Attr("
          << Attr->getName() << "Attr *A) {\n";

      auto& Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();
      for (auto& Arg : Args) {
         auto* ArgVal = cast<RecordVal>(Arg);
         auto C = ArgVal->getRecord()->getBases().front().getBase();

         auto Name
             = cast<StringLiteral>(ArgVal->getRecord()->getFieldValue("name"))
                   ->getVal();

         GetterName += "get";
         GetterName += (char)::toupper(Name.front());
         GetterName += Name.substr(1);

         out << "   ";
         if (C->getName() == "IntArg")
            writeIntArg(out, GetterName);
         else if (C->getName() == "FloatArg")
            writeFloatArg(out, GetterName);
         else if (C->getName() == "StringArg")
            writeStringArg(out, GetterName);
         else if (C->getName() == "TypeArg")
            writeTypeArg(out, GetterName);
         else if (C->getName() == "ExprArg")
            writeExprArg(out, GetterName);
         else
            writeEnumArg(out, GetterName);

         GetterName.clear();
      }

      out << "}\n";
   }
}

void AttrSerializeEmitter::emitDeserialize(llvm::ArrayRef<Record*> Attrs)
{
   std::vector<llvm::StringRef> FieldNames;
   for (auto& Attr : Attrs) {
      out << Attr->getName() << "Attr *ASTAttrReader::read" << Attr->getName()
          << "Attr(SourceRange SR) {\n";

      auto& Args = cast<ListLiteral>(Attr->getFieldValue("Args"))->getValues();
      for (auto& Arg : Args) {
         auto* ArgVal = cast<RecordVal>(Arg);
         auto C = ArgVal->getRecord()->getBases().front().getBase();

         auto Name
             = cast<StringLiteral>(ArgVal->getRecord()->getFieldValue("name"))
                   ->getVal();

         FieldNames.push_back(Name);

         out << "   auto " << Name << " = ";
         if (C->getName() == "IntArg")
            readIntArg(out);
         else if (C->getName() == "FloatArg")
            readFloatArg(out);
         else if (C->getName() == "StringArg")
            readStringArg(out);
         else if (C->getName() == "TypeArg")
            readTypeArg(out);
         else if (C->getName() == "ExprArg")
            readExprArg(out);
         else {
            std::string EnumName;
            EnumName += Attr->getName();
            EnumName += "Attr::";
            EnumName += (char)::toupper(Name.front());
            EnumName += Name.substr(1);
            EnumName += "Kind";

            readEnumArg(out, EnumName);
         }

         out << ";\n";
      }

      out << "   return new(C) " << Attr->getName() << "Attr(";

      unsigned i = 0;
      for (auto Name : FieldNames) {
         if (i++ != 0)
            out << ", ";

         out << "std::move(" << Name << ")";
      }

      if (i++ != 0)
         out << ", ";

      out << "SR);\n"
          << "}\n";

      FieldNames.clear();
   }
}

extern "C" {

void EmitAttributeDefs(std::ostream& out, RecordKeeper& RK)
{
   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      AttrDefEmitter(OS, RK).emit();
   }

   out << s;
}

void EmitAttributeClasses(std::ostream& out, RecordKeeper& RK)
{
   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      AttrClassEmitter(OS, RK).emit();
   }

   out << s;
}

void EmitAttributeParse(std::ostream& out, RecordKeeper& RK)
{
   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      AttrParseEmitter(OS, RK).emit();
   }

   out << s;
}

void EmitAttributeSerialize(std::ostream& out, RecordKeeper& RK)
{
   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      AttrSerializeEmitter(OS, RK).emit();
   }

   out << s;
}

};