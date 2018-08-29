//
// Created by Jonas Zell on 24.08.18.
//

#include "TableGen/Record.h"
#include "TableGen/Value.h"
#include "TableGen/Type.h"
#include "Support/Casting.h"
#include "Support/LLVM.h"
#include "Support/StringSwitch.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>

#include <unordered_map>

using namespace cdot;
using namespace cdot::tblgen;
using namespace cdot::support;

using std::string;

namespace {

class QueryDefEmitter {
   /// Reference to the record keeper.
   RecordKeeper &RK;

   /// The stream to write to.
   llvm::raw_ostream &OS;

public:
   QueryDefEmitter(llvm::raw_ostream &OS, RecordKeeper &RK)
      : RK(RK), OS(OS)
   {}

   void Emit();
};

} // anonymous namespace

void QueryDefEmitter::Emit()
{
   static constexpr auto QueryMacro = "CDOT_QUERY";
   OS << "#ifndef " << QueryMacro << "\n"
      << "   #error \"define " << QueryMacro << " before including this!\"\n"
      << "#endif\n\n";

   SmallVector<Record*, 128> Queries;
   RK.getAllDefinitionsOf(RK.lookupClass("Query"), Queries);

   for (auto *Q : Queries) {
      std::string Name = Q->getName();
      Name += "Query";

      OS << QueryMacro << "(" << Name << ")\n";
   }

   OS << "\n#undef " << QueryMacro;
}

namespace {

class QueryClassEmitter {
   /// Reference to the record keeper.
   RecordKeeper &RK;

   /// The stream to write to.
   llvm::raw_ostream &OS;

   struct QueryParam {
      QueryParam(string Type, StringRef Name,
                 StringRef DefaultVal, bool InnerType, bool Excluded)
         : Type(Type), Name(std::move(Name)), DefaultVal(DefaultVal),
           InnerType(InnerType), Excluded(Excluded)
      {}

      std::string Type;
      StringRef Name;
      StringRef DefaultVal;
      bool InnerType;
      bool Excluded;
   };

   struct QueryInfo {
      std::string ClassName;
      bool CanBeCached;
      bool CanBeSerialized;
      bool ShouldMoveResult;
      bool Private;
      bool HashMap;
      std::vector<QueryParam> Params;

      /// The parameter string with types and parameter names.
      std::string ParamStr;

      /// The parameter string with default values.
      std::string ParamStrWithDefaultVals;

      /// The parameter string with no types and potentially moved values.
      std::string ParamStrNoTypes;

      /// The parameter string with types and no moved values.
      std::string ParamStrNoMove;

      /// The parameter string with no types and no moved values.
      std::string ParamStrNoMoveNoTypes;

      /// The parameter string without excluded parameters.
      std::string ParamStrNoExclude;

      /// The parameter string without excluded parameters and no types.
      std::string ParamStrNoExcludeNoTypes;

      StringRef Type;
      StringRef GetReturnType;
      StringRef CustomKeyType;
      StringRef CustomGet;
      StringRef CustomHeaderCode;
      StringRef ConstructorCode;
      StringRef CustomImplCode;
      StringRef PersistentState;
      StringRef RefreshCondition;
   };

   enum ParamKind {
      Integer, QualType, Pointer, String, Other,
   };

   ParamKind getParamKind(StringRef Str);
   void appendParam(ParamKind K, llvm::raw_ostream &OS,
                    StringRef TypeName, StringRef VarName);

   void appendString(ParamKind K, llvm::raw_ostream &OS,
                     StringRef TypeName, StringRef VarName);

   bool shouldBeMoved(StringRef TypeName);

   /// Information about a query record.
   llvm::DenseMap<Record*, QueryInfo> QueryMap;

   /// Query
   void EmitDecl(Record *Query);
   void EmitImpl(Record *Query);
   void EmitQueryClassImpls(ArrayRef<Record*> Queries);

   /// QueryContext
   void EmitQueryContextFoldingSets(ArrayRef<Record*> Queries);
   void EmitQueryContextDecls(ArrayRef<Record*> Queries);
   void EmitQueryContextImpls(ArrayRef<Record*> Queries);

public:
   QueryClassEmitter(RecordKeeper &RK, raw_ostream &OS)
      : RK(RK), OS(OS)
   {}

   void Emit();
};

} // anonymous namespace

void QueryClassEmitter::Emit()
{
   SmallVector<Record*, 128> Queries;
   RK.getAllDefinitionsOf(RK.lookupClass("Query"), Queries);

   OS << "#ifdef CDOT_QUERY_DECL\n";
   for (auto *Q : Queries) {
      EmitDecl(Q);
   }
   OS << "#endif\n\n";

   OS << "#ifdef CDOT_QUERY_IMPL\n";
   for (auto *Q : Queries) {
      EmitImpl(Q);
   }
   OS << "#endif\n\n";

   OS << "#ifdef CDOT_QUERY_CLASS_IMPL\n";
   EmitQueryClassImpls(Queries);
   OS << "#endif\n\n";

   OS << "#ifdef CDOT_QUERY_CONTEXT_FIELDS\n";
   EmitQueryContextFoldingSets(Queries);
   OS << "#endif\n\n";

   OS << "#ifdef CDOT_QUERY_CONTEXT_DECLS\n";
   EmitQueryContextDecls(Queries);
   OS << "#endif\n\n";

   OS << "#ifdef CDOT_QUERY_CONTEXT_IMPL\n";
   EmitQueryContextImpls(Queries);
   OS << "#endif\n\n";

   OS << "#undef CDOT_QUERY_DECL\n";
   OS << "#undef CDOT_QUERY_IMPL\n";
   OS << "#undef CDOT_QUERY_CLASS_IMPL\n";
   OS << "#undef CDOT_QUERY_CONTEXT_FIELDS\n";
   OS << "#undef CDOT_QUERY_CONTEXT_DECLS\n";
   OS << "#undef CDOT_QUERY_CONTEXT_IMPL\n";
}

void QueryClassEmitter::EmitDecl(Record *Query)
{
   auto &Info = QueryMap[Query];

   Info.ClassName = Query->getName();
   Info.ClassName += "Query";

   Info.Type = cast<StringLiteral>(Query->getFieldValue("type"))
      ->getVal();

   Info.GetReturnType = cast<StringLiteral>(
      Query->getFieldValue("getReturnType"))->getVal();
   Info.CustomKeyType = cast<StringLiteral>(
      Query->getFieldValue("customKeyType"))->getVal();
   Info.CustomGet = cast<CodeBlock>(Query->getFieldValue("customGet"))
      ->getCode();
   Info.CustomHeaderCode = cast<CodeBlock>(
      Query->getFieldValue("customHeaderCode"))->getCode();
   Info.ConstructorCode = cast<CodeBlock>(
      Query->getFieldValue("constructorCode"))->getCode();
   Info.PersistentState = cast<CodeBlock>(
      Query->getFieldValue("persistentState"))->getCode();
   Info.RefreshCondition = cast<CodeBlock>(
      Query->getFieldValue("refreshCondition"))->getCode();
   Info.CanBeCached = cast<IntegerLiteral>(
      Query->getFieldValue("canBeCached"))->getVal().getBoolValue();
   Info.CanBeSerialized = cast<IntegerLiteral>(
      Query->getFieldValue("canBeSerialized"))->getVal().getBoolValue();
   Info.ShouldMoveResult = cast<IntegerLiteral>(
      Query->getFieldValue("shouldMoveResult"))->getVal().getBoolValue();
   Info.Private = cast<IntegerLiteral>(
      Query->getFieldValue("private"))->getVal().getBoolValue();

   // Use a HashMap instead of a FoldingSet if there's a single key.
   auto *ParamsVal = cast<ListLiteral>(Query->getFieldValue("parameters"));
   Info.HashMap = ParamsVal->getValues().size() == 1;

   OS << "class " << Info.ClassName << ": public Query";

   if (Info.CanBeCached && !Info.HashMap) {
      OS << ", public llvm::FoldingSetNode";
   }

   OS << " {\n";

   // custom code
   OS << Info.CustomHeaderCode;

   // classof, classofKind
   OS << "public:\n"
      << "   static bool classof(const Query *Q) "
         "{ return classofKind(Q->kind()); }\n"
      << "   static bool classofKind(Kind K) "
         "{ return K == Kind::" << Info.ClassName << "ID; }\n\n";

   // description, summary
   OS << "   std::string description() const;\n";
   OS << "   std::string summary() const;\n\n";

   // get
   if (Info.GetReturnType != "void") {
      OS << "   " << Info.GetReturnType << " get() const;\n\n";
   }

   // Friend decls.
   OS << "   friend class Query; // for run().\n\n";
   OS << "   friend class QueryContext; // for C'tor.\n\n";

   // Private decls.
   OS << "private:\n";

   // run
   OS << "   QueryResult run();\n\n";

   // refresh
   if (!Info.RefreshCondition.empty()) {
      OS << "   void refresh();\n\n";
   }

   // C'tor.
   llvm::raw_string_ostream ParamWithDefaultOS(Info.ParamStrWithDefaultVals);
   llvm::raw_string_ostream ParamOS(Info.ParamStr);
   llvm::raw_string_ostream ParamNoTypesOS(Info.ParamStrNoTypes);
   llvm::raw_string_ostream ParamNoMoveOS(Info.ParamStrNoMove);
   llvm::raw_string_ostream ParamNoMoveNoTypeOS(Info.ParamStrNoMoveNoTypes);
   llvm::raw_string_ostream ParamNoExcludeOS(Info.ParamStrNoExclude);
   llvm::raw_string_ostream ParamNoExcludeNoTypeOS(Info.ParamStrNoExcludeNoTypes);

   std::string FieldsStr;
   llvm::raw_string_ostream FieldOS(FieldsStr);

   auto &ParamInfo = Info.Params;

   unsigned i = 0;
   unsigned j = 0;

   for (auto *ParamVal : ParamsVal->getValues()) {
      auto *Param = cast<RecordVal>(ParamVal)->getRecord();

      StringRef Name = cast<StringLiteral>(Param->getFieldValue("name"))
         ->getVal();
      string Type = cast<StringLiteral>(Param->getFieldValue("type"))
         ->getVal();
      StringRef DefaultVal = cast<StringLiteral>(Param->getFieldValue("defaultVal"))
         ->getVal();
      bool InnerType = cast<IntegerLiteral>(Param->getFieldValue("isInnerType"))
         ->getVal().getBoolValue();
      bool Exclude = cast<IntegerLiteral>(Param->getFieldValue("exclude"))
         ->getVal().getBoolValue();

      if (InnerType) {
         Type = Info.ClassName + "::" + Type;
      }
      else if (Param->getBases().front().getBase()->getName() == "ArrayParam") {
         Type = "llvm::ArrayRef<" + Type + ">";
      }

      ParamInfo.emplace_back(Type, Name, DefaultVal, InnerType, Exclude);

      if (i++ != 0) {
         ParamOS << ", ";
         ParamNoTypesOS << ", ";
         ParamWithDefaultOS << ", ";
         ParamNoMoveOS << ", ";
         ParamNoMoveNoTypeOS << ", ";
      }
      if (!Exclude && j++ != 0) {
         ParamNoExcludeOS << ", ";
         ParamNoExcludeNoTypeOS << ", ";
      }

      if (!Exclude) {
         ParamNoExcludeNoTypeOS << Name;
      }

      ParamNoMoveNoTypeOS << Name;
      FieldOS << "   " << Type << " " << Name << ";\n";

      if (shouldBeMoved(Type)) {
         ParamNoTypesOS << "std::move(" << Name << ")";
         ParamNoMoveOS << "const " << Type << " &" << Name;

         if (!Exclude) {
            ParamNoExcludeOS << "const " << Type << " &" << Name;;
         }
      }
      else {
         ParamNoTypesOS << Name;
         ParamNoMoveOS << Type << " " << Name;

         if (!Exclude) {
            ParamNoExcludeOS << Type << " " << Name;
         }
      }

      ParamOS << Type << " " << Name;
      ParamWithDefaultOS << Type << " " << Name;

      if (!DefaultVal.empty()) {
         ParamWithDefaultOS << " = " << DefaultVal;
      }
   }

   ParamOS.flush();
   ParamWithDefaultOS.flush();
   ParamNoTypesOS.flush();
   ParamNoMoveOS.flush();
   ParamNoMoveNoTypeOS.flush();
   ParamNoExcludeOS.flush();
   ParamNoExcludeNoTypeOS.flush();

   OS << "   " << Info.ClassName << "(QueryContext &QC"
      << (ParamWithDefaultOS.str().empty() ? "" : ", ")
      << ParamWithDefaultOS.str()
      << ", SourceRange Loc = {});\n\n";

   // finish()
   if (Info.GetReturnType != "void") {
      StringRef Result;
      if (Info.ShouldMoveResult) {
         Result = "&&Result";
      }
      else {
         Result = "Result";
      }

      OS << "   QueryResult finish(" << Info.Type << " "
         << Result << ", Status St = Done);\n\n";
   }

   OS << FieldOS.str();
   if (Info.Type != "void") {
      // Result field value.
      OS << "   llvm::Optional<" << Info.Type << "> Result;\n";
   }

   if (Info.CanBeCached && !Info.ParamStr.empty()) {
      // Profile.
      OS << "public:\n";
      OS << "   void Profile(llvm::FoldingSetNodeID &ID) const;\n";
      OS << "   static void Profile(llvm::FoldingSetNodeID &ID"
         << (ParamNoExcludeOS.str().empty() ? "" : ", ")
         << ParamNoExcludeOS.str()
         << ");\n\n";
   }

   OS << "};\n\n";
}

QueryClassEmitter::ParamKind QueryClassEmitter::getParamKind(StringRef Str)
{
   if (Str.back() == '*')
      return Pointer;

   return StringSwitch<ParamKind >(Str)
      .Case("int", Integer)
      .Case("unsigned", Integer)
      .Case("bool", Integer)
      .Case("char", Integer)
      .Case("size_t", Integer)
      .Case("QualType", QualType)
      .Case("StringRef", String)
      .Case("std::string", String)
      .Default(Other);
}

bool QueryClassEmitter::shouldBeMoved(StringRef TypeName)
{
   if (TypeName.startswith("std::unique_ptr")) {
      return true;
   }

   return StringSwitch<bool>(TypeName)
      .Case("std::string", true)
      .Case("SingleLevelLookupResult", true)
      .Case("MultiLevelLookupResult", true)
      .Case("sema::MultiLevelFinalTemplateArgList", true)
      .Default(false);
}

void QueryClassEmitter::appendParam(ParamKind K,
                                    llvm::raw_ostream &OS,
                                    StringRef TypeName,
                                    StringRef VarName) {
   switch (K) {
   case Integer:
      OS << "ID.AddInteger(" << VarName << ");";
      break;
   case Pointer:
      OS << "ID.AddPointer(" << VarName << ");";
      break;
   case QualType:
      OS << "ID.AddPointer(" << VarName << ".getAsOpaquePtr());";
      break;
   case String:
      OS << "ID.AddString(" << VarName << ");";
      break;
   case Other:
      if (TypeName.consume_front("llvm::ArrayRef<")) {
         auto ElementName = TypeName.drop_back(1);
         OS << "for (auto &El : " << VarName << ")\n      ";
         appendParam(getParamKind(ElementName), OS, ElementName, "El");
      }
      else if (TypeName.back() == '*') {
         OS << "ID.AddPointer(" << VarName << ");";
      }
      else if (TypeName.back() == '&') {
         OS << "ID.AddPointer(&" << VarName << ");";
      }
      else if (TypeName.endswith("Kind")) {
         OS << "ID.AddInteger((uint64_t)" << VarName << ");";
      }
      else if (TypeName == "SourceLocation") {
         OS << "ID.AddInteger(" << VarName << ".getOffset());";
      }
      else if (TypeName == "DeclarationName" || TypeName == "StmtOrDecl") {
         OS << "ID.AddPointer(" << VarName << ".getAsOpaquePtr());";
      }
      else {
         OS << VarName << ".Profile(ID);";
      }

      break;
   }
}

void QueryClassEmitter::appendString(ParamKind K,
                                     llvm::raw_ostream &OS,
                                     StringRef TypeName,
                                     StringRef VarName) {
   switch (K) {
   case Integer:
      OS << "OS << " << VarName << ";";
      break;
   case Pointer: {
      if (TypeName.find("DeclContext") != string::npos) {
         OS << "OS << \"'\" << " << VarName << "->getNameAsString() << \"'\";";
      }
      else if (TypeName.find("DeclConstraint") != string::npos) {
         OS << "OS << " << VarName << ";";
      }
      else if (TypeName.find("Decl") != string::npos) {
         OS << "OS << \"'\" << " << VarName
            << "->Decl::getNameAsString() << \"'\";";
      }
      else {
         OS << "OS << " << VarName << ";";
      }

      break;
   }
   case String:
   case QualType:
      OS << "OS << \"'\" << " << VarName << " << \"'\";";
      break;
   case Other: {
      if (TypeName.consume_front("llvm::ArrayRef<")) {
         auto ElementName = TypeName.drop_back(1);
         OS << "OS << '[';\n"
            << "   unsigned i = 0;\n"
            << "   for (auto &El : " << VarName << ") {\n"
            << "      if (i++ != 0) OS << \", \";\n"
            << "      ";

         appendString(getParamKind(ElementName), OS, ElementName, "El");
         OS << "\n   }\n";
         OS << "   OS << ']';\n";
      }
      else if (TypeName.back() == '&') {
         OS << "OS << &" << VarName  << ";";
      }
      else if (TypeName == "DeclarationName") {
         OS << "OS << \"'\" << " << VarName << " << \"'\";";
      }
      else if (TypeName == "StmtOrDecl") {
         OS << "OS << " << VarName << ".getAsOpaquePtr();";
      }
      else if (TypeName == "SourceLocation") {
         OS << "OS << " << VarName << ".getOffset();";
      }
      else if (TypeName == "sema::MultiLevelFinalTemplateArgList") {
         OS << "OS << " << VarName << ".toString();";
      }
      else if (TypeName.endswith("Kind")) {
         OS << "OS << (uint64_t)" << VarName << ";";
      }
      else {
         OS << "OS << " << VarName << ";";
      }

      break;
   }
   }
}

void QueryClassEmitter::EmitImpl(Record *Query)
{
   auto &Info = QueryMap[Query];

   // C'tor.
   OS << Info.ClassName << "::" << Info.ClassName << "(QueryContext &QC"
      << (Info.ParamStr.empty() ? "" : ", ")
      << Info.ParamStr << ", SourceRange Loc) "
                          ": Query(" << Info.ClassName << "ID, QC, Loc)";

   for (auto &P : Info.Params) {
      OS << ", " << P.Name << "(";

      if (shouldBeMoved(P.Type)) {
         OS << "std::move(" << P.Name << ")";
      }
      else {
         OS << P.Name;
      }

      OS << ")";
   }

   OS << " {\n"
      << Info.ConstructorCode << "\n"
      << "}\n\n";

   // custom code.
   OS << Info.CustomImplCode;

   // refresh
   if (!Info.RefreshCondition.empty()) {
      OS << "void " << Info.ClassName << "::refresh()\n{\n";
      OS << R"__(
   if (Stat == Idle) return;
   if ()__" << Info.RefreshCondition << R"__() Stat = Idle;
)__";
      OS << "\n}\n\n";
   }

   // get
   if (Info.GetReturnType != "void") {
      OS << Info.GetReturnType << " " << Info.ClassName
         << "::get() const\n{";

      if (Info.CustomGet.empty()) {
         OS << R"__(
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

)__";
      }
      else {
         OS << R"__(
   assert(ok() && "query has not successfully completed!");
   )__" << Info.CustomGet << R"__(
}

)__";
      }
   }

   // description
   {
      unsigned i = 0;
      std::string DescStr = cast<StringLiteral>(
         Query->getFieldValue("description"))->getVal();

      auto Idx = DescStr.find('\\');
      while (Idx != string::npos) {
         DescStr.erase(Idx, 1);
         Idx = DescStr.find('\\');
      }

      StringRef Desc = DescStr;

      OS << "std::string " << Info.ClassName << "::description() const\n{\n"
         << "   std::string s;\n";

      unsigned Begin = 0;
      unsigned Len = (unsigned)Desc.size();
      while (i < Len) {
         auto c = Desc[i++];
         if (c == '$') {
            OS << "   s += \"" << Desc.substr(Begin, i - Begin - 1) << "\";\n";

            if (Desc[i] == '{') {
               ++i;

               Begin = i;
               unsigned OpenBraces = 1;
               unsigned CloseBraces = 0;

               while (OpenBraces != CloseBraces) {
                  switch (Desc[i++]) {
                  case '{': ++OpenBraces; break;
                  case '}': ++CloseBraces; break;
                  default:
                     break;
                  }
               }

               OS << "   s += " << Desc.substr(Begin, i - Begin - 1) << ";\n";
            }
            else {
               Begin = i;
               while (i < Len && ::isalnum(Desc[i])) {
                  ++i;
               }

               auto Name = Desc.substr(Begin, i - Begin);
               if (Name == "NAME") {
                  OS << "   s += \"" << Query->getName() << "\";\n";
               }
               else {
                  OS << "   s += this->" << Name << ";\n";
               }
            }

            Begin = i;
         }
      }

      if (Begin != i) {
         OS << "   s += \"" << Desc.substr(Begin, Len - i - 1) << "\";\n";
      }

      OS << "   return s;\n}\n\n";
   }

   // summary()
   {
      OS << "std::string " << Info.ClassName << "::summary() const\n{\n"
         << "   std::string s;\n"
         << "   llvm::raw_string_ostream OS(s);\n\n";

      // [QueryName]
      OS << "   OS << \"[" << Query->getName() << "]\"\n;";

      unsigned i = 0;
      for (auto &Param : Info.Params) {
         if (i++ != 0)
            OS << "   OS << ',';\n";

         OS << "   OS << \" " << Param.Name << " = \";\n";
         appendString(getParamKind(Param.Type), OS, Param.Type, Param.Name);
         OS << "\n";
      }

      OS << ";\n\n";
      OS << "   return OS.str();\n}\n\n";
   }

   // finish()
   if (Info.GetReturnType != "void") {
      StringRef Result;
      StringRef AssignResult;
      if (Info.ShouldMoveResult) {
         Result = "&&Result";
         AssignResult = "this->Result = std::move(Result);";
      }
      else {
         Result = "Result";
         AssignResult = "this->Result = Result;";
      }

      StringRef customFinish = cast<CodeBlock>(
         Query->getFieldValue("customFinish"))->getCode();

      OS << "QueryResult " << Info.ClassName << "::"
         << "finish(" << Info.Type << " " << Result << ", Status St)\n{\n"
         << "   " << AssignResult << "\n"
         << customFinish << "\n"
         << "   return Query::finish(St);\n"
         << "}\n\n";
   }

   // Profile.
   if (Info.CanBeCached && !Info.ParamStr.empty()) {
      OS << "void " << Info.ClassName << "::"
         << "Profile(llvm::FoldingSetNodeID &ID) const\n{\n"
         << "   Profile(ID"
         << (Info.ParamStrNoExcludeNoTypes.empty() ? "" : ", ")
         << Info.ParamStrNoExcludeNoTypes
         << ");\n}\n\n";

      OS << "void " << Info.ClassName << "::"
         << "Profile(llvm::FoldingSetNodeID &ID"
         << (Info.ParamStrNoExclude.empty() ? "" : ", ")
         << Info.ParamStrNoExclude << ")\n{\n";

      for (auto &P : Info.Params) {
         if (P.Excluded)
            continue;

         OS << "   ";
         appendParam(getParamKind(P.Type), OS, P.Type, P.Name);
         OS << "\n";
      }

      OS << "}\n\n";
   }
}

void QueryClassEmitter::EmitQueryClassImpls(ArrayRef<Record*> Queries)
{
   /// isPure()
   OS << "bool Query::isPure() const\n{\n"
      << "   switch (K) {\n";

   for (auto *Q : Queries) {
      auto &Info = QueryMap[Q];
      bool Val = cast<IntegerLiteral>(Q->getFieldValue("pure"))
         ->getVal().getBoolValue();

      OS << "   case " << Info.ClassName << "ID: return "
         << (Val ? "true" : "false") << ";\n";
   }

   OS << "   }\n}\n\n";

   /// canBeCached()
   OS << "bool Query::canBeCached() const\n{\n"
      << "   switch (K) {\n";

   for (auto *Q : Queries) {
      auto &Info = QueryMap[Q];
      OS << "   case " << Info.ClassName << "ID: return "
         << (Info.CanBeCached ? "true" : "false") << ";\n";
   }

   OS << "   }\n}\n\n";

   /// canBeSerialized()
   OS << "bool Query::canBeSerialized() const\n{\n"
      << "   switch (K) {\n";

   for (auto *Q : Queries) {
      auto &Info = QueryMap[Q];
      OS << "   case " << Info.ClassName << "ID: return "
         << (Info.CanBeSerialized ? "true" : "false") << ";\n";
   }

   OS << "   }\n}\n\n";
}

void QueryClassEmitter::EmitQueryContextFoldingSets(ArrayRef<Record*> Queries)
{
   for (auto *Q : Queries) {
      auto &Info = QueryMap[Q];
      OS << Info.PersistentState << "\n";

      if (!Info.CanBeCached)
         continue;

      if (Info.Params.empty()) {
         OS << "/// \\brief Single query instance.\n"
            << "private:   "
            <<  Info.ClassName << " *" << Q->getName() << "Instance = nullptr;\n";
      }
      else if (Info.HashMap) {
         OS << "/// \\brief Maps from query kinds to a folding set containing instances of\n"
               "/// that query.\n"
            << "private:   "
            << "llvm::DenseMap<"
            << (Info.CustomKeyType.empty()
               ? llvm::StringRef(Info.Params.front().Type)
               : Info.CustomKeyType)
            << ", " << Info.ClassName << "*> "
            << Q->getName() << "Queries;\n";
      }
      else {
         OS << "/// \\brief Maps from query kinds to a folding set containing instances of\n"
               "/// that query.\n"
            << "private:   "
            << "llvm::FoldingSet<" << Info.ClassName << "> "
            << Q->getName() << "Queries;\n";
      }
   }
}

void QueryClassEmitter::EmitQueryContextDecls(ArrayRef<Record*> Queries)
{
   for (auto *Q : Queries) {
      auto &Info = QueryMap[Q];
      if (Info.Private) {
         OS << "private: ";
      }
      else {
         OS << "public: ";
      }

      if (Info.GetReturnType == "void") {
         OS << "QueryResult " << Q->getName()
            << "(" << Info.ParamStrWithDefaultVals
            << (Info.ParamStrWithDefaultVals.empty() ? "" : ", ")
            << "SourceRange Loc = {});\n";
      }
      else {
         OS << "QueryResult " << Q->getName()
            << "(" << Info.GetReturnType << " &Result"
            << (Info.ParamStrWithDefaultVals.empty() ? "" : ", ")
            << Info.ParamStrWithDefaultVals
            << ", SourceRange Loc = {});\n";
      }
   }

   OS << "\n\n";

   for (auto *Q : Queries) {
      OS << "friend class " << QueryMap[Q].ClassName << ";\n";
   }
}

void QueryClassEmitter::EmitQueryContextImpls(ArrayRef<Record*> Queries)
{
   for (auto *Q : Queries) {
      auto &Info = QueryMap[Q];
      if (Info.GetReturnType == "void") {
         OS << "QueryResult QueryContext::" << Q->getName()
            << "(" << Info.ParamStr
            << (Info.ParamStr.empty() ? "" : ", ")
            << "SourceRange Loc)\n";
      }
      else {
         OS << "QueryResult QueryContext::" << Q->getName()
            << "(" << Info.GetReturnType << " &Result"
            << (Info.ParamStr.empty() ? "" : ", ") << Info.ParamStr
            << ", SourceRange Loc)\n";
      }

      OS << "\n{\n";

      if (Info.CanBeCached && !Info.Params.empty()) {
         if (Info.HashMap) {
            std::string HashMap = Q->getName();
            HashMap += "Queries";

            OS << "   auto It = " << HashMap << ".find("
               << Info.Params.front().Name << ");\n";

            OS << "   " << Info.ClassName << " *_Q;\n";
            OS << "   if (It == " << HashMap << ".end()) {\n"
               << "      _Q = new(*this) " << Info.ClassName << "(*this"
                                   << (Info.ParamStrNoTypes.empty() ? "" : ", ")
                                   << Info.ParamStrNoTypes << ");\n"
               << "      " << HashMap << "[_Q->" << Info.Params.front().Name
                           << "] = _Q;\n"
               << "   } else {\n"
               << "      _Q = It->getSecond();\n"
               << "   }";
         }
         else {
            std::string FoldingSet = Q->getName();
            FoldingSet += "Queries";

            OS << "   llvm::FoldingSetNodeID ID;\n";
            OS << "   " << Info.ClassName << "::Profile(ID"
               << (Info.ParamStrNoExcludeNoTypes.empty() ? "" : ", ")
               << Info.ParamStrNoExcludeNoTypes << ");\n";

            OS << "   void *InsertPos;\n";
            OS << "   auto *_Q = " << FoldingSet << ".FindNodeOrInsertPos(ID, InsertPos);\n";

            OS << "   if (!_Q) {\n"
               << "      _Q = new(*this) " << Info.ClassName << "(*this"
               << (Info.ParamStrNoTypes.empty() ? "" : ", ")
               << Info.ParamStrNoTypes << ");\n"
               << "      " << FoldingSet << ".InsertNode(_Q, InsertPos);\n   }";
         }
      }
      else if (Info.CanBeCached && Info.ParamStr.empty()) {
         std::string Instance = Q->getName();
         Instance += "Instance";

         OS << "   if (!" << Instance << ")\n"
            << "      " << Instance
            << " = new(*this) " << Info.ClassName << "(*this"
            << (Info.ParamStrNoTypes.empty() ? "" : ", ")
            << Info.ParamStrNoTypes << ");\n";

         OS << "auto *_Q = " << Instance << ";\n";
      }
      else {
         OS << "   auto *_Q = new(*this) " << Info.ClassName << "(*this"
            << (Info.ParamStrNoTypes.empty() ? "" : ", ")
            << Info.ParamStrNoTypes << ");";
      }

      StringRef AssignResult;
      if (Info.GetReturnType != "void") {
         AssignResult = "Result = _Q->get();";
      }

      if (!Info.RefreshCondition.empty()) {
         OS << "   _Q->refresh();\n";
      }

      OS << R"__(
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         if (auto _R = _Q->Query::run()) {
            return _R;
         }
      }

      )__" << AssignResult << R"__(
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      )__" << AssignResult << R"__(
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }
)__";

      OS << "\n}\n\n";
   }
}

extern "C" {

void EmitQueryDefs(llvm::raw_ostream &out, RecordKeeper &RK)
{
   QueryDefEmitter(out, RK).Emit();
}

void EmitQueryClasses(llvm::raw_ostream &out, RecordKeeper &RK)
{
   QueryClassEmitter(RK, out).Emit();
}

} // extern C