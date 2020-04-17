
#include "tblgen/Record.h"
#include "tblgen/Support/StringSwitch.h"
#include "tblgen/Type.h"
#include "tblgen/Value.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>

using namespace tblgen;
using namespace tblgen::support;

using llvm::StringRef;
using std::string;

namespace {

llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, std::string_view str)
{
   return OS << string(str);
}

std::ostream& operator<<(std::ostream& OS, StringRef str)
{
   return OS << str.str();
}

class QueryDefEmitter {
   /// Reference to the record keeper.
   RecordKeeper& RK;

   /// The stream to write to.
   std::ostream& OS;

public:
   QueryDefEmitter(std::ostream& OS, RecordKeeper& RK) : RK(RK), OS(OS) {}

   void Emit();
};

} // anonymous namespace

void QueryDefEmitter::Emit()
{
   static constexpr auto QueryMacro = "CDOT_QUERY";
   OS << "#ifndef " << QueryMacro << "\n"
      << "   #error \"define " << QueryMacro << " before including this!\"\n"
      << "#endif\n\n";

   std::vector<Record*> Queries;
   RK.getAllDefinitionsOf(RK.lookupClass("Query"), Queries);

   for (auto* Q : Queries) {
      std::string Name = Q->getName();
      Name += "Query";

      OS << QueryMacro << "(" << Name << ")\n";
   }

   OS << "\n#undef " << QueryMacro;
}

namespace {

class QueryClassEmitter {
   /// The stream to write to.
   std::ostream& OS;

   /// All query def's.
   std::vector<Record*> Queries;

   struct QueryParam {
      QueryParam(string Type, StringRef Name, StringRef DefaultVal,
                 bool InnerType, bool Excluded, bool Nullable)
          : Type(Type), Name(std::move(Name)), DefaultVal(DefaultVal),
            InnerType(InnerType), Excluded(Excluded), Nullable(Nullable)
      {
         this->Nullable |= DefaultVal == "nullptr";
      }

      std::string Type;
      StringRef Name;
      StringRef DefaultVal;
      bool InnerType;
      bool Excluded;
      bool Nullable;
   };

   struct QueryInfo {
      std::string ClassName;
      bool SimpleQuery;
      bool CanBeCached;
      bool CanBeSerialized;
      bool CanBeDependent;
      bool ShouldMoveResult;
      bool Private;
      bool HashMap;
      bool IgnoreCircularDependency;
      bool Infallible;
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

      /// The fields of this query.
      std::string Fields;

      /// The trailing objects of this query.
      std::vector<StringRef> TrailingObjects;

      std::string_view Type;
      std::string_view GetReturnType;
      std::string_view CustomKeyType;
      std::string_view CustomGet;
      std::string_view CustomAssign;
      std::string_view CustomHeaderCode;
      std::string_view CustomPreReturnCode;
      std::string_view EarlyExitCode;
      std::string_view ConstructorCode;
      std::string_view CustomImplCode;
      std::string_view PersistentState;
      std::string_view RefreshCondition;
   };

   enum ParamKind {
      Integer,
      QualType,
      Pointer,
      String,
      Other,
   };

   ParamKind getParamKind(StringRef Str);
   void appendParam(ParamKind K, std::ostream& OS, StringRef TypeName,
                    StringRef VarName);

   void appendString(ParamKind K, std::ostream& OS, StringRef TypeName,
                     StringRef VarName);

   bool shouldBeMoved(StringRef TypeName);

   /// Information about a query record.
   llvm::DenseMap<Record*, QueryInfo> QueryMap;

public:
   QueryClassEmitter(RecordKeeper& RK, std::ostream& OS) : OS(OS)
   {
      RK.getAllDefinitionsOf(RK.lookupClass("Query"), Queries);
      Setup();
   }

   void EmitQueryDecls();
   void EmitQueryImpls();

   void EmitQueryContextFields();
   void EmitQueryContextDecls();
   void EmitQueryContextSpecializations();
   void EmitQueryContextImpls();

private:
   void Setup();

   /// Query
   void EmitDecl(Record* Query);
   void EmitSimpleDecl(Record* Query, QueryInfo& Info);

   void EmitImpl(Record* Query);
   void EmitQueryClassImpls(llvm::ArrayRef<Record*> Queries);

   /// Query Macros
   void EmitQueryMacros(llvm::ArrayRef<Record*> Queries);
};

} // anonymous namespace

void QueryClassEmitter::EmitQueryDecls()
{
   for (auto* Q : Queries) {
      EmitDecl(Q);
   }
}

void QueryClassEmitter::EmitQueryImpls()
{
   EmitQueryClassImpls(Queries);

   for (auto* Q : Queries) {
      EmitImpl(Q);
   }
}

void QueryClassEmitter::Setup()
{
   for (auto* Query : Queries) {
      auto& Info = QueryMap[Query];

      Info.ClassName = Query->getName();
      Info.ClassName += "Query";

      Info.Type = cast<StringLiteral>(Query->getFieldValue("type"))->getVal();

      Info.GetReturnType
          = cast<StringLiteral>(Query->getFieldValue("getReturnType"))
                ->getVal();
      Info.CustomKeyType
          = cast<StringLiteral>(Query->getFieldValue("customKeyType"))
                ->getVal();
      Info.CustomGet
          = cast<CodeBlock>(Query->getFieldValue("customGet"))->getCode();
      Info.CustomAssign
          = cast<CodeBlock>(Query->getFieldValue("customAssign"))->getCode();
      Info.CustomHeaderCode
          = cast<CodeBlock>(Query->getFieldValue("customHeaderCode"))
                ->getCode();
      Info.CustomPreReturnCode
          = cast<CodeBlock>(Query->getFieldValue("customPreReturnCode"))
                ->getCode();
      Info.EarlyExitCode
          = cast<CodeBlock>(Query->getFieldValue("earlyExitCode"))->getCode();
      Info.ConstructorCode
          = cast<CodeBlock>(Query->getFieldValue("constructorCode"))->getCode();
      Info.PersistentState
          = cast<CodeBlock>(Query->getFieldValue("persistentState"))->getCode();
      Info.RefreshCondition
          = cast<CodeBlock>(Query->getFieldValue("refreshCondition"))
                ->getCode();
      Info.SimpleQuery
          = cast<IntegerLiteral>(Query->getFieldValue("simpleQuery"))->getVal()
            != 0;
      Info.CanBeCached
          = cast<IntegerLiteral>(Query->getFieldValue("canBeCached"))->getVal()
            != 0;
      Info.CanBeSerialized
          = cast<IntegerLiteral>(Query->getFieldValue("canBeSerialized"))
                ->getVal()
            != 0;
      Info.CanBeDependent
          = cast<IntegerLiteral>(Query->getFieldValue("canBeDependent"))
                ->getVal()
            != 0;
      Info.ShouldMoveResult
          = cast<IntegerLiteral>(Query->getFieldValue("shouldMoveResult"))
                ->getVal()
            != 0;
      Info.IgnoreCircularDependency
          = cast<IntegerLiteral>(
                Query->getFieldValue("ignoreCircularDependency"))
                ->getVal()
            != 0;
      Info.Private
          = cast<IntegerLiteral>(Query->getFieldValue("private"))->getVal()
            != 0;
      Info.Infallible
          = cast<IntegerLiteral>(Query->getFieldValue("infallible"))->getVal()
            != 0;

      auto* TrailingObjects
          = cast<ListLiteral>(Query->getFieldValue("trailingObjects"));

      for (auto* Val : TrailingObjects->getValues()) {
         Info.TrailingObjects.push_back(cast<StringLiteral>(Val)->getVal());
      }

      llvm::raw_string_ostream ParamWithDefaultOS(Info.ParamStrWithDefaultVals);
      llvm::raw_string_ostream ParamOS(Info.ParamStr);
      llvm::raw_string_ostream ParamNoTypesOS(Info.ParamStrNoTypes);
      llvm::raw_string_ostream ParamNoMoveOS(Info.ParamStrNoMove);
      llvm::raw_string_ostream ParamNoMoveNoTypeOS(Info.ParamStrNoMoveNoTypes);
      llvm::raw_string_ostream ParamNoExcludeOS(Info.ParamStrNoExclude);
      llvm::raw_string_ostream ParamNoExcludeNoTypeOS(
          Info.ParamStrNoExcludeNoTypes);
      llvm::raw_string_ostream FieldOS(Info.Fields);

      auto& ParamInfo = Info.Params;

      unsigned i = 0;
      unsigned j = 0;

      // Use a HashMap instead of a FoldingSet if there's a single key.
      auto* ParamsVal = cast<ListLiteral>(Query->getFieldValue("parameters"));
      Info.HashMap = ParamsVal->getValues().size() == 1 || Info.SimpleQuery;

      for (auto* ParamVal : ParamsVal->getValues()) {
         auto* Param = cast<RecordVal>(ParamVal)->getRecord();

         StringRef Name
             = cast<StringLiteral>(Param->getFieldValue("name"))->getVal();
         string Type
             = cast<StringLiteral>(Param->getFieldValue("type"))->getVal();
         StringRef DefaultVal
             = cast<StringLiteral>(Param->getFieldValue("defaultVal"))
                   ->getVal();
         bool InnerType
             = cast<IntegerLiteral>(Param->getFieldValue("isInnerType"))
                   ->getVal()
               != 0;
         bool Exclude
             = cast<IntegerLiteral>(Param->getFieldValue("exclude"))->getVal()
               != 0;
         bool Nullable
             = cast<IntegerLiteral>(Param->getFieldValue("nullable"))->getVal()
               != 0;

         if (Info.SimpleQuery && Type == "bool") {
            Type = "uint8_t";
         }

         if (InnerType) {
            Type = Info.ClassName + "::" + Type;
         }
         else if (Param->getBases().front().getBase()->getName()
                  == "ArrayParam") {
            Type = "llvm::ArrayRef<" + Type + ">";
         }
         else if (Param->getBases().front().getBase()->getName()
                  == "MutableArrayParam") {
            Type = "llvm::MutableArrayRef<" + Type + ">";
         }
         else if (Param->getBases().front().getBase()->getName()
                  == "FunctionRefParam") {
            Type = "llvm::function_ref<" + Type + ">";
         }

         ParamInfo.emplace_back(Type, Name, DefaultVal, InnerType, Exclude,
                                Nullable);

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
               ParamNoExcludeOS << "const " << Type << " &" << Name;
               ;
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
   }
}

void QueryClassEmitter::EmitDecl(Record* Query)
{
   auto& Info = QueryMap[Query];

   OS << "class " << Info.ClassName << ": public Query";

   if (Info.CanBeCached && !Info.HashMap) {
      OS << ", public llvm::FoldingSetNode";
   }

   for (auto& TO : Info.TrailingObjects) {
      OS << ", llvm::TrailingObjects<" << TO << ">";
   }

   OS << " {\n";

   // custom code
   OS << Info.CustomHeaderCode;

   // classof, classofKind
   OS << "public:\n"
      << "   static bool classof(const Query *Q) "
         "{ return classofKind(Q->kind()); }\n"
      << "   static bool classofKind(Kind K) "
         "{ return K == Kind::"
      << Info.ClassName << "ID; }\n\n";

   // result_type
   OS << "   using result_type = " << Info.GetReturnType << ";\n\n";

   // description, summary
   OS << "   std::string description() const;\n";
   OS << "   std::string summary() const;\n\n";

   // get
   if (Info.GetReturnType != "void") {
      OS << "   " << Info.GetReturnType << " get() const;\n\n";
   }

   // Friend decls.
   OS << "   friend class Query; // for run().\n";
   OS << "   friend class QueryContext; // for C'tor.\n\n";

   if (!Info.TrailingObjects.empty()) {
      OS << "   friend TrailingObjects;\n\n";
   }

   // Private decls.
   OS << "private:\n";

   // run
   OS << "   QueryResult run();\n\n";

   // refresh
   if (!Info.RefreshCondition.empty()) {
      OS << "   void refresh();\n\n";
   }

   // C'tor.
   OS << "   " << Info.ClassName << "(QueryContext &QC"
      << (Info.ParamStrWithDefaultVals.empty() ? "" : ", ")
      << Info.ParamStrWithDefaultVals << ");\n\n";

   // finish()
   if (Info.GetReturnType != "void") {
      StringRef Result;
      if (Info.ShouldMoveResult) {
         Result = "&&Result";
      }
      else {
         Result = "Result";
      }

      OS << "   QueryResult finish(" << Info.Type << " " << Result
         << ", Status St = Done);\n\n";
   }

   OS << Info.Fields;
   if (Info.Type != "void") {
      // Result field value.
      OS << "   llvm::Optional<" << Info.Type << "> Result;\n";
   }

   if (Info.CanBeCached && !Info.ParamStr.empty()) {
      // Profile.
      OS << "public:\n";
      OS << "   void Profile(llvm::FoldingSetNodeID &ID) const;\n";
      OS << "   static void Profile(llvm::FoldingSetNodeID &ID"
         << (Info.ParamStrNoExclude.empty() ? "" : ", ")
         << Info.ParamStrNoExclude << ");\n\n";
   }

   OS << "};\n\n";
}

QueryClassEmitter::ParamKind QueryClassEmitter::getParamKind(StringRef Str)
{
   if (Str.back() == '*')
      return Pointer;

   return StringSwitch<ParamKind>(Str)
       .Case("int", Integer)
       .Case("unsigned", Integer)
       .Case("bool", Integer)
       .Case("char", Integer)
       .Case("uint8_t", Integer)
       .Case("int8_t", Integer)
       .Case("uint16_t", Integer)
       .Case("int16_t", Integer)
       .Case("uint32_t", Integer)
       .Case("int32_t", Integer)
       .Case("uint64_t", Integer)
       .Case("int64_t", Integer)
       .Case("intptr_t", Integer)
       .Case("uintptr_t", Integer)
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

void QueryClassEmitter::appendParam(ParamKind K, std::ostream& OS,
                                    StringRef TypeName, StringRef VarName)
{
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
      if (TypeName.consume_front("llvm::ArrayRef<") || TypeName.consume_front("llvm::MutableArrayRef<")) {
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
      else if (TypeName == "SourceRange") {
         OS << "ID.AddInteger(" << VarName << ".getStart().getOffset());";
         OS << "ID.AddInteger(" << VarName << ".getEnd().getOffset());";
      }
      else if (TypeName == "LookupOpts") {
         OS << "ID.AddInteger((uint8_t)(" << VarName << "));";
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

void QueryClassEmitter::appendString(ParamKind K, std::ostream& OS,
                                     StringRef TypeName, StringRef VarName)
{
   switch (K) {
   case Integer:
      OS << "OS << " << VarName << ";";
      break;
   case Pointer: {
      if (TypeName.find("vector") != string::npos) {
         OS << "OS << " << VarName << ";";
      }
      else if (TypeName.find("DeclContext") != string::npos) {
         OS << "OS << \"'\" << (" << VarName << " ? " << VarName
            << "->getNameAsString() : \"<null>\") << \"'\";";
      }
      else if (TypeName.find("DeclConstraint") != string::npos) {
         OS << "OS << " << VarName << ";";
      }
      else if (TypeName.find("Decl") != string::npos) {
         OS << "OS << \"'\" << (" << VarName << " ? " << VarName
            << "->Decl::getNameAsString() : \"<null>\") << \"'\";";
      }
      else if (TypeName == "sema::FinalTemplateArgumentList*") {
         OS << "if (" << VarName << ")"
            << "   " << VarName << "->print(OS);"
            << "else OS << \"<null>\";";
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
      if (TypeName.consume_front("llvm::ArrayRef<") || TypeName.consume_front("llvm::MutableArrayRef<")) {
         auto ElementName = TypeName.drop_back(1);
         OS << "OS << '[';\n"
            << "   unsigned i = 0;\n"
            << "   for (auto &El : " << VarName << ") {\n"
            << "      if (i == 5) { OS << \", ...\"; break; }\n"
            << "      if (i++ != 0) OS << \", \";\n"
            << "      ";

         appendString(getParamKind(ElementName), OS, ElementName, "El");
         OS << "\n   }\n";
         OS << "   OS << ']';\n";
      }
      else if (TypeName.back() == '&') {
         OS << "OS << &" << VarName << ";";
      }
      else if (TypeName == "sema::MultiLevelFinalTemplateArgList") {
         OS << VarName << ".print(OS);";
      }
      else if (TypeName == "DeclarationName") {
         OS << "OS << \"'\" << " << VarName << " << \"'\";";
      }
      else if (TypeName == "StmtOrDecl") {
         OS << "OS << " << VarName << ".getAsOpaquePtr();";
      }
      else if (TypeName == "SourceLocation") {
         OS << "OS << QC.CI.getFileMgr().getSourceLocationAsString(" << VarName
            << ");";
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

void QueryClassEmitter::EmitImpl(Record* Query)
{
   auto& Info = QueryMap[Query];

   // C'tor.
   OS << Info.ClassName << "::" << Info.ClassName << "(QueryContext &QC"
      << (Info.ParamStr.empty() ? "" : ", ") << Info.ParamStr
      << ") "
         ": Query("
      << Info.ClassName << "ID, QC)";

   std::string NullChecks;
   llvm::raw_string_ostream NullOS(NullChecks);

   for (auto& P : Info.Params) {
      OS << ", " << P.Name << "(";

      if (shouldBeMoved(P.Type)) {
         OS << "std::move(" << P.Name << ")";
      }
      else {
         OS << P.Name;
      }

      OS << ")";

      // Assert on 'null' pointer arguments if they're not nullable.
      if (!P.Nullable) {
         NullOS << "assert(" << P.Name << " && \"parameter '" << P.Name
                << "' should not be null!\");\n";
      }
   }

   OS << " {\n"
      << NullOS.str() << Info.ConstructorCode << "\n"
      << "}\n\n";

   // custom code.
   OS << Info.CustomImplCode;

   // refresh
   if (!Info.RefreshCondition.empty()) {
      OS << "void " << Info.ClassName << "::refresh()\n{\n";
      OS << R"__(
   if (Stat == Idle) return;
   if ()__"
         << Info.RefreshCondition << R"__() Stat = Idle;
)__";
      OS << "\n}\n\n";
   }

   // get
   if (Info.GetReturnType != "void") {
      OS << Info.GetReturnType << " " << Info.ClassName << "::get() const\n{";

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
   )__" << Info.CustomGet
            << R"__(
}

)__";
      }
   }

   // description
   {
      unsigned i = 0;
      std::string DescStr
          = cast<StringLiteral>(Query->getFieldValue("description"))->getVal();

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
                  case '{':
                     ++OpenBraces;
                     break;
                  case '}':
                     ++CloseBraces;
                     break;
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
      for (auto& Param : Info.Params) {
         if (Param.Excluded)
            continue;

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

      auto customFinish
          = cast<CodeBlock>(Query->getFieldValue("customFinish"))->getCode();

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
         << Info.ParamStrNoExcludeNoTypes << ");\n}\n\n";

      OS << "void " << Info.ClassName << "::"
         << "Profile(llvm::FoldingSetNodeID &ID"
         << (Info.ParamStrNoExclude.empty() ? "" : ", ")
         << Info.ParamStrNoExclude << ")\n{\n";

      for (auto& P : Info.Params) {
         if (P.Excluded)
            continue;

         OS << "   ";
         appendParam(getParamKind(P.Type), OS, P.Type, P.Name);
         OS << "\n";
      }

      OS << "}\n\n";
   }
}

void QueryClassEmitter::EmitQueryClassImpls(llvm::ArrayRef<Record*> Queries)
{
   /// isPure()
   OS << "bool Query::isPure() const\n{\n"
      << "   switch (K) {\n";

   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];
      bool Val = cast<IntegerLiteral>(Q->getFieldValue("pure"))->getVal() != 0;

      OS << "   case " << Info.ClassName << "ID: return "
         << (Val ? "true" : "false") << ";\n";
   }

   OS << "   }\n}\n\n";

   /// canBeCached()
   OS << "bool Query::canBeCached() const\n{\n"
      << "   switch (K) {\n";

   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];
      OS << "   case " << Info.ClassName << "ID: return "
         << (Info.CanBeCached ? "true" : "false") << ";\n";
   }

   OS << "   }\n}\n\n";

   /// canBeSerialized()
   OS << "bool Query::canBeSerialized() const\n{\n"
      << "   switch (K) {\n";

   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];
      OS << "   case " << Info.ClassName << "ID: return "
         << (Info.CanBeSerialized ? "true" : "false") << ";\n";
   }

   OS << "   }\n}\n\n";

   /// canBeDependent()
   OS << "bool Query::canBeDependent() const\n{\n"
      << "   switch (K) {\n";

   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];
      OS << "   case " << Info.ClassName << "ID: return "
         << (Info.CanBeDependent ? "true" : "false") << ";\n";
   }

   OS << "   }\n}\n\n";
}

void QueryClassEmitter::EmitQueryContextFields()
{
   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];
      OS << Info.PersistentState << "\n";

      if (!Info.CanBeCached)
         continue;

      if (Info.Params.empty()) {
         OS << "/// \\brief Single query instance.\n"
            << "private:   ";

         if (Info.SimpleQuery) {
            if (Info.Type == "void") {
               OS << "bool Ran" << Q->getName() << "Query = false;\n";
            }
            else {
               OS << "llvm::Optional<" << Info.Type << "> " << Q->getName()
                  << "Result = nullptr;\n";
            }
         }
         else {
            OS << Info.ClassName << " *" << Q->getName()
               << "Instance = nullptr;\n";
         }
      }
      else if (Info.HashMap) {
         OS << "/// \\brief Maps from query kinds to a folding set containing "
               "instances of\n"
               "/// that query.\n"
            << "private:   ";

         if (Info.SimpleQuery && Info.Type == "void") {
            OS << "llvm::DenseSet<"
               << (Info.CustomKeyType.empty()
                       ? std::string_view(Info.Params.front().Type)
                       : Info.CustomKeyType)
               << "> " << Q->getName() << "Queries;\n";
         }
         else if (Info.SimpleQuery) {
            OS << "llvm::DenseMap<";

            if (!Info.CustomKeyType.empty()) {
               OS << Info.CustomKeyType;
            }
            else if (Info.Params.size() == 1) {
               OS << Info.Params.front().Type;
            }
            else {
               // Use recursive pairs because they can be used as
               // DenseMap keys.
               OS << "std::pair<";

               unsigned i = 0;
               unsigned NumPairs = 1;
               unsigned NumParams = Info.Params.size();

               for (auto& P : Info.Params) {
                  if (i != 0)
                     OS << ", ";

                  // If we're at an uneven number and it's not the last one,
                  // we need to start a new pair.
                  if ((i & 1) != 0 && i != NumParams - 1) {
                     OS << "std::pair<";
                     ++NumPairs;
                  }

                  OS << P.Type;
                  ++i;
               }

               for (i = 0; i < NumPairs; ++i) {
                  OS << ">";
               }
            }

            OS << ", " << Info.Type << "> " << Q->getName() << "Queries;\n";
         }
         else {
            OS << "llvm::DenseMap<"
               << (Info.CustomKeyType.empty()
                       ? std::string_view(Info.Params.front().Type)
                       : Info.CustomKeyType)
               << ", " << Info.ClassName << "*> " << Q->getName()
               << "Queries;\n";
         }
      }
      else {
         OS << "/// \\brief Maps from query kinds to a folding set containing "
               "instances of\n"
               "/// that query.\n"
            << "private:   "
            << "llvm::FoldingSet<" << Info.ClassName << "> " << Q->getName()
            << "Queries;\n";
      }
   }
}

void QueryClassEmitter::EmitQueryContextDecls()
{
   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];
      if (Info.Private) {
         OS << "private: ";
      }
      else {
         OS << "public: ";
      }

      std::string_view Type;
      if (!Info.Infallible) {
         Type = "QueryResult";
      }
      else {
         Type = Info.GetReturnType;
      }

      if (Info.GetReturnType == "void" || Info.Infallible) {
         OS << Type << " " << Q->getName() << "("
            << Info.ParamStrWithDefaultVals << ");\n";
      }
      else {
         OS << Type << " " << Q->getName() << "(" << Info.GetReturnType
            << " &Result" << (Info.ParamStrWithDefaultVals.empty() ? "" : ", ")
            << Info.ParamStrWithDefaultVals << ");\n";
      }

      // Queries can also be retrieved without running them.
      // TODO
   }

   OS << "\n\n";

   for (auto* Q : Queries) {
      OS << "friend class " << QueryMap[Q].ClassName << ";\n";
   }
}

void QueryClassEmitter::EmitQueryContextSpecializations()
{
   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];
      if (Info.SimpleQuery || !Info.CanBeCached) {
         continue;
      }

      OS << "template<> " << Info.ClassName << " *"
         << "QueryContext::getQuery<" << Info.ClassName << ">("
         << Info.ParamStrNoExclude << ");\n";
   }
}

void QueryClassEmitter::EmitQueryContextImpls()
{
   for (auto* Q : Queries) {
      auto& Info = QueryMap[Q];

      // getQuery specialization
      if (Info.CanBeCached && !Info.SimpleQuery) {
         OS << "template<> " << Info.ClassName << " *"
            << "QueryContext::getQuery<" << Info.ClassName << ">("
            << Info.ParamStrNoExclude << ")\n{\n";

         if (Info.HashMap) {
            std::string HashMap = Q->getName();
            HashMap += "Queries";

            OS << "   auto It = " << HashMap << ".find("
               << Info.Params.front().Name << ");\n"
               << "   if (It != " << HashMap
               << ".end()) return It->getSecond();\n"
               << "   return nullptr;\n";
         }
         else if (!Info.Params.empty()) {
            std::string FoldingSet = Q->getName();
            FoldingSet += "Queries";

            OS << "   llvm::FoldingSetNodeID ID;\n";
            OS << "   " << Info.ClassName << "::Profile(ID"
               << (Info.ParamStrNoExcludeNoTypes.empty() ? "" : ", ")
               << Info.ParamStrNoExcludeNoTypes << ");\n";

            OS << "   void *InsertPos;\n";
            OS << "   return " << FoldingSet
               << ".FindNodeOrInsertPos(ID, InsertPos);\n";
         }
         else {
            string Instance = Q->getName();
            Instance += "Instance";

            OS << "   return " << Instance << ";\n";
         }

         OS << "}\n\n";
      }

      std::string_view RetType;
      if (!Info.Infallible) {
         RetType = "QueryResult";
      }
      else {
         RetType = Info.GetReturnType;
      }

      // query run implementation
      if (Info.GetReturnType == "void" || Info.Infallible) {
         OS << RetType << " QueryContext::" << Q->getName() << "("
            << Info.ParamStr << ")\n";
      }
      else {
         OS << RetType << " QueryContext::" << Q->getName() << "("
            << Info.GetReturnType << " &Result"
            << (Info.ParamStr.empty() ? "" : ", ") << Info.ParamStr << ")\n";
      }

      OS << "\n{\n" << Info.EarlyExitCode;

      std::string DenseMapKey;
      if (Info.CanBeCached && !Info.Params.empty()) {
         if (Info.HashMap) {
            std::string HashMap = Q->getName();
            HashMap += "Queries";

            llvm::raw_string_ostream KeyOS(DenseMapKey);

            if (Info.SimpleQuery && Info.Params.size() == 1) {
               KeyOS << Info.Params.front().Name;
            }
            else if (Info.SimpleQuery) {
               // Use recursive pairs because they can be used as
               // DenseMap keys.
               KeyOS << "std::make_pair(";

               unsigned i = 0;
               unsigned NumPairs = 1;
               unsigned NumParams = Info.Params.size();

               for (auto& P : Info.Params) {
                  if (i != 0)
                     KeyOS << ", ";

                  // If we're at an uneven number and it's not the last one,
                  // we need to start a new pair.
                  if ((i & 1) != 0 && i != NumParams - 1) {
                     KeyOS << "std::make_pair(";
                     ++NumPairs;
                  }

                  KeyOS << P.Name;
                  ++i;
               }

               for (i = 0; i < NumPairs; ++i) {
                  KeyOS << ")";
               }
            }
            else {
               KeyOS << Info.Params.front().Name;
            }

            OS << "   auto Key = " << KeyOS.str() << ";\n";
            OS << "   auto It = " << HashMap << ".find(Key);\n";

            if (Info.SimpleQuery) {
               OS << "   if (It != " << HashMap
                  << ".end() && !shouldReset()) {\n";

               if (Info.Infallible) {
                  if (!Info.CustomPreReturnCode.empty()) {
                     OS << "      " << Info.CustomPreReturnCode;
                  }

                  if (!Info.CustomAssign.empty()) {
                     OS << "      return " << Info.CustomAssign << ";";
                  }
                  else {
                     OS << "      return It->getSecond();";
                  }
               }
               else {
                  if (!Info.CustomPreReturnCode.empty()) {
                     OS << "      " << Info.CustomPreReturnCode;
                  }

                  if (Info.Type != "void") {
                     if (!Info.CustomAssign.empty()) {
                        OS << "      Result = " << Info.CustomAssign << ";";
                     }
                     else {
                        OS << "      Result = It->getSecond();";
                     }
                  }

                  OS << "      return QueryResult(QueryResult::Success);";
               }

               OS << "   }\n";
            }
            else {
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
         }
         else {
            assert(!Info.SimpleQuery);

            std::string FoldingSet = Q->getName();
            FoldingSet += "Queries";

            OS << "   llvm::FoldingSetNodeID ID;\n";
            OS << "   " << Info.ClassName << "::Profile(ID"
               << (Info.ParamStrNoExcludeNoTypes.empty() ? "" : ", ")
               << Info.ParamStrNoExcludeNoTypes << ");\n";

            OS << "   void *InsertPos;\n";
            OS << "   auto *_Q = " << FoldingSet
               << ".FindNodeOrInsertPos(ID, InsertPos);\n";

            OS << "   if (!_Q) {\n"
               << "      _Q = new(*this) " << Info.ClassName << "(*this"
               << (Info.ParamStrNoTypes.empty() ? "" : ", ")
               << Info.ParamStrNoTypes << ");\n"
               << "      " << FoldingSet << ".InsertNode(_Q, InsertPos);\n   }";
         }
      }
      else if (Info.CanBeCached && Info.ParamStr.empty()) {
         string Instance = Q->getName();
         if (Info.SimpleQuery && Info.Type == "void") {
            Instance = string("Ran") + Q->getName();

            if (Info.Infallible) {
               OS << "   if (" << Instance << " && !shouldReset()) {\n"
                  << (Info.CustomPreReturnCode.empty()
                          ? ""
                          : Info.CustomPreReturnCode)
                  << "      return;"
                  << "   }\n";
            }
            else {
               OS << "   if (" << Instance << " && !shouldReset()) {\n"
                  << (Info.CustomPreReturnCode.empty()
                          ? ""
                          : Info.CustomPreReturnCode)
                  << "      return QueryResult(QueryResult::Success);"
                  << "   }\n";
            }
         }
         else if (Info.SimpleQuery) {
            Instance += "Result";

            if (Info.Infallible) {
               OS << "   if (" << Instance << " && !shouldReset()) {\n"
                  << (Info.CustomPreReturnCode.empty()
                          ? ""
                          : Info.CustomPreReturnCode)
                  << "      return Instance.getValue();"
                  << "   }\n";
            }
            else {
               OS << "   if (" << Instance << " && !shouldReset()) {\n"
                  << (Info.CustomPreReturnCode.empty()
                          ? ""
                          : Info.CustomPreReturnCode)
                  << "      Result = Instance.getValue();"
                  << "      return QueryResult(QueryResult::Success);"
                  << "   }\n";
            }
         }
         else {
            Instance += "Instance";

            OS << "   if (!" << Instance << ")\n"
               << "      " << Instance << " = new(*this) " << Info.ClassName
               << "(*this" << (Info.ParamStrNoTypes.empty() ? "" : ", ")
               << Info.ParamStrNoTypes << ");\n";

            OS << "auto *_Q = " << Instance << ";\n";
         }
      }
      else if (!Info.SimpleQuery) {
         OS << "   auto *_Q = new(*this) " << Info.ClassName << "(*this"
            << (Info.ParamStrNoTypes.empty() ? "" : ", ")
            << Info.ParamStrNoTypes << ");";
      }

      if (!Info.RefreshCondition.empty()) {
         OS << "   _Q->refresh();\n";
      }

      if (Info.SimpleQuery && Info.Type == "void") {
         OS << Info.ClassName << " _Q(*this"
            << (Info.ParamStrNoTypes.empty() ? "" : ", ")
            << Info.ParamStrNoTypes << ");\n\n"
            << "ExecutingQuery EQ(*this, &_Q);\n"
            << "auto MaybeErr = _Q.run();\n"
            << "if (MaybeErr.isErr()) ";

         if (Info.Infallible) {
            OS << "llvm_unreachable(\"infallible query failed!\");\n\n";
         }
         else {
            OS << "return MaybeErr;\n\n";
         }

         if (Info.CanBeCached) {
            if (Info.Params.empty()) {
               OS << "Ran" << Info.ClassName << " = true;\n";
            }
            else {
               OS << Q->getName() << "Queries.insert("
                  << Info.Params.front().Name << ");\n";
            }
         }

         if (!Info.CustomPreReturnCode.empty()) {
            OS << "      " << Info.CustomPreReturnCode;
         }

         if (Info.Infallible) {
            OS << "return;";
         }
         else {

            OS << "return QueryResult(QueryResult::Success);";
         }
      }
      else if (Info.SimpleQuery) {
         OS << Info.ClassName << " _Q(*this"
            << (Info.ParamStrNoTypes.empty() ? "" : ", ")
            << Info.ParamStrNoTypes << ");\n\n"
            << "ExecutingQuery EQ(*this, &_Q);\n"
            << "auto MaybeErr = _Q.run();\n"
            << "if (MaybeErr.isErr()) ";

         if (Info.Infallible) {
            OS << "llvm_unreachable(\"infallible query failed!\");\n\n";

            OS << "auto Result = _Q.get();\n";
            if (Info.CanBeCached) {
               if (Info.Params.empty()) {
                  OS << Info.ClassName << "Result = Result;\n";
               }
               else {
                  OS << Q->getName() << "Queries[Key] = Result;\n";
               }
            }

            if (!Info.CustomPreReturnCode.empty()) {
               OS << "      " << Info.CustomPreReturnCode;
            }

            OS << "return Result;";
         }
         else {
            OS << "return MaybeErr;\n\n";
            OS << "Result = _Q.get();\n";

            if (Info.CanBeCached) {
               if (Info.Params.empty()) {
                  OS << Info.ClassName << "Result = Result;\n";
               }
               else {
                  OS << Q->getName() << "Queries[Key] = Result;\n";
               }
            }

            if (!Info.CustomPreReturnCode.empty()) {
               OS << "      " << Info.CustomPreReturnCode;
            }

            OS << "return QueryResult(QueryResult::Success);";
         }
      }
      else if (Info.Infallible) {
         std::string ReturnResult;
         if (!Info.CustomPreReturnCode.empty()) {
            ReturnResult += Info.CustomPreReturnCode;
            ReturnResult += "\n";
         }

         if (Info.GetReturnType != "void") {
            ReturnResult += "return _Q->get()";
         }
         else {
            ReturnResult += "return";
         }

         std::string beginExecution;
         if (Info.IgnoreCircularDependency) {
            beginExecution += "_Q->Stat = Query::Idle;";
         }
         else {
            beginExecution += "_Q->Stat = Query::Running;";
         }

         OS << R"__(
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      llvm_unreachable("circular dependency in infallible query!");
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         )__"
            << beginExecution << R"__(
         if (auto _R = _Q->run()) {
            llvm_unreachable("infallible query failed!");
         }
      }

      )__" << ReturnResult << R"__(;
   case Query::Done:
      )__" << ReturnResult
            << R"__(;
   case Query::Aborted:
   case Query::Dependent:
   case Query::DoneWithError:
      llvm_unreachable(infallible query failed!");
   }
)__";
      }
      else {
         std::string AssignResult;
         if (!Info.CustomPreReturnCode.empty()) {
            AssignResult += Info.CustomPreReturnCode;
            AssignResult += "\n";
         }

         if (Info.GetReturnType != "void") {
            AssignResult += "Result = _Q->get();";
         }

         std::string beginExecution;
         if (Info.IgnoreCircularDependency) {
            beginExecution += "_Q->Stat = Query::Idle;";
         }
         else {
            beginExecution += "_Q->Stat = Query::Running;";
         }

         OS << R"__(
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         )__"
            << beginExecution << R"__(
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      )__" << AssignResult << R"__(
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      )__" << AssignResult
            << R"__(
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }
)__";
      }

      OS << "\n}\n\n";
   }
}

void QueryClassEmitter::EmitQueryMacros(llvm::ArrayRef<Record*> Queries) {}

extern "C" {

void EmitQueryDefs(std::ostream& out, RecordKeeper& RK)
{
   QueryDefEmitter(out, RK).Emit();
}

void EmitQueryDecls(std::ostream& out, RecordKeeper& RK)
{
   QueryClassEmitter(RK, out).EmitQueryDecls();
}

void EmitQueryImpls(std::ostream& out, RecordKeeper& RK)
{
   QueryClassEmitter(RK, out).EmitQueryImpls();
}

void EmitQueryContextFields(std::ostream& out, RecordKeeper& RK)
{
   QueryClassEmitter(RK, out).EmitQueryContextFields();
}

void EmitQueryContextDecls(std::ostream& out, RecordKeeper& RK)
{
   QueryClassEmitter(RK, out).EmitQueryContextDecls();
}

void EmitQueryContextSpecializations(std::ostream& out, RecordKeeper& RK)
{
   QueryClassEmitter(RK, out).EmitQueryContextSpecializations();
}

void EmitQueryContextImpls(std::ostream& out, RecordKeeper& RK)
{
   QueryClassEmitter(RK, out).EmitQueryContextImpls();
}

} // extern C