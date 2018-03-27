////
//// Created by Jonas Zell on 28.11.17.
////
//
//#include <ctime>
//#include "ModuleWriter.h"
//#include "Module.h"
//
//#include "../Sema/Record/Record.h"
//#include "../Sema/Record/Class.h"
//#include "../Sema/Record/Protocol.h"
//#include "../Sema/Record/Enum.h"
//#include "../Sema/Record/Union.h"
//
//#include "../AST/Statement/Declaration/CallableDecl.h"
//#include "../AST/Statement/Declaration/Class/RecordDecl.h"
//#include "../AST/Statement/Declaration/TypedefDecl.h"
//
//#include "../Sema/Function.h"
//#include "../Support/WriterBase.h"
//#include "../AST/Passes/Serialization/Serialize.h"
//
//#include "../Variant/Type/ArrayType.h"
//#include "../Variant/Type/TupleType.h"
//#include "../Variant/Type/FunctionType.h"
//#include "../Variant/Type/RecordType.h"
//
//using namespace cdot::support;
//
//namespace cdot {
//namespace module {
//
//class WriterImpl: public WriterBase<WriterImpl> {
//public:
//   WriterImpl(Module *M, llvm::raw_ostream &out)
//      : WriterBase(outs), outs(s), realOut(out), M(M)
//   {
//      CurrentTab = 0;
//   }
//
//   void DoWrite();
//   bool WriteEntry(SymbolTableEntry *Entry);
//
//   void WriteRecordCommon(const Record &R);
//
//   void WriteFunction(const Callable &F);
//   void WriteOperator(const Callable &F);
//
//   void WriteGlobalVariable(const Variable &V);
//
//   void WriteTypedef(const Typedef &TD);
//   void WriteAlias(const Alias &alias);
//
//   void addTypeReferences(Type *ty)
//   {
//      if (ty->isObjectTy()) {
//         referencedTypes.insert(ty->getRecord());
//         return;
//      }
//
//      if (ty->isPointerTy())
//         return addTypeReferences(*ty->getPointeeType());
//
//      if (auto arr = dyn_cast<ArrayType>(ty))
//         return addTypeReferences(arr->getElementType());
//
//      if (auto tup = dyn_cast<TupleType>(ty)) {
//         for (auto &cont : tup->getContainedTypes())
//            addTypeReferences(*cont.col);
//
//         return;
//      }
//
//      if (auto fn = dyn_cast<FunctionType>(ty)) {
//         addTypeReferences(*fn->getReturnType());
//         for (auto &arg : fn->getParamTypes())
//            addTypeReferences(*arg.getType());
//
//         return;
//      }
//
//      if (auto obj = dyn_cast<RecordType>(ty)) {
//         for (auto &TA : obj->getTemplateArgs()) {
//            if (TA.isType())
//               addTypeReferences(TA.getType());
//         }
//
//         return;
//      }
//   }
//
//   void WriteType(const QualType &ty)
//   {
//      addTypeReferences(*ty);
//      out << ty.toString();
//   }
//
//   void WriteType(Type *ty)
//   {
//      addTypeReferences(ty);
//      out << ty->toString();
//   }
//
//   void WriteAccess(AccessModifier AM)
//   {
//      switch (AM) {
//         case AccessModifier::PUBLIC:
//            out << "public "; break;
//         case AccessModifier::PRIVATE:
//            out << "private "; break;
//         case AccessModifier::PROTECTED:
//            out << "protected "; break;
//         case AccessModifier::DEFAULT:
//            break;
//      }
//   }
//
//   void WriteTemplateParameter(TemplateParameter const& Param)
//   {
//      if (Param.isVariadic) out << "...";
//      out << Param.getGenericTypeName();
//      out << ": " << Param.covariance->toString();
//   }
//
//   void WriteArgument(Argument const& A)
//   {
//      if (A.isCstyleVararg()) {
//         out << "...";
//         return;
//      }
//
//      if (!A.getLabel().empty()) {
//         out << (A.getType().isConst() ? "let " : "var ");
//         out << A.getLabel() << ": ";
//      }
//
//      out << (A.getType().isLvalue() ? "ref " : "")
//          << A.getType().toString()
//          << (A.isVariadic() ? "..." : "");
//   }
//
//private:
//   static const char *Names[];
//
//   string s;
//   llvm::raw_string_ostream outs;
//   llvm::raw_ostream &realOut;
//
//   Module *M;
//
//   serial::ASTSerializer serializer;
//   std::vector<Statement*> Templates;
//
//   llvm::SmallPtrSet<Record*, 16> referencedTypes;
//
//   void WriteModuleName(Module const* M)
//   {
//      WriteList(M->getName(), &WriterBase::WriteIdentifier, "", ".", "");
//   }
//
//   void WriteRecordName(Record const* R)
//   {
//      WriteIdentifier(R->getName());
//   }
//
//   void WriteField(Field const& F);
//   void WriteMethod(Method const& M);
//   void WriteProp(Property const& P);
//   void WriteAssociatedType(AssociatedType const &A);
//   void WriteCase(EnumCase const& C);
//
//   void WriteTemplate(Statement *Stmt)
//   {
//      out << "template ";
//
//      string s;
//      llvm::raw_string_ostream str(s);
//      serializer.serialize(Stmt, str);
//
//      out << "\"";
//      WriteEscapedString(str.str());
//      out << "\"";
//   }
//};
//
//const char *WriterImpl::Names[] = {
//   "struct", "class", "enum", "union", "protocol"
//};
//
//void ModuleWriter::WriteTo(llvm::raw_ostream &out)
//{
//   WriterImpl W(M, out);
//   W.DoWrite();
//}
//
//void WriterImpl::DoWrite()
//{
//   out << '\n';
//
//   out << "module_name    ";
//   WriteModuleName(M);
//   NewLine();
//
//   out << "file_name      \"" << M->getFileName() << "\"\n";
//   out << "md5            \"" << M->getMd5() << "\"\n";
//   out << "imports        ";
//
//   WriteList(M->getImports(), &WriterImpl::WriteModuleName, "[", ", ", "]\n\n");
//
//   out.flush();
//
//   realOut << s;
//   s.clear();
//
//   for (const auto &E : M->getEntries()) {
//      auto written = WriteEntry(E);
//      if (written)
//         out << "\n\n";
//   }
//
//   for (auto &T : Templates) {
//      WriteTemplate(T);
//      out << "\n";
//   }
//
//   std::time_t rawtime;
//   std::tm* timeinfo;
//   char buffer [80];
//
//   std::time(&rawtime);
//   timeinfo = std::localtime(&rawtime);
//
//   std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
//
//   out << "\n; created at " << buffer;
//
//   for (auto R : referencedTypes) {
//      realOut << Names[R->getTypeID()] << " ";
//      realOut << R->getName();
//
//      if (!R->getTemplateParams().empty()) {
//         realOut << " <";
//         size_t i = 0;
//
//         for (auto &Param : R->getTemplateParams()) {
//            if (i++ != 0) realOut << ", ";
//            out << Param.getGenericTypeName();
//            out << ": " << Param.covariance->toString();
//         }
//
//         realOut << ">";
//      }
//
//      realOut << "\n";
//   }
//
//   out.flush();
//
//   realOut << "\n";
//   realOut << s;
//}
//
//bool WriterImpl::WriteEntry(SymbolTableEntry *Entry)
//{
//   switch (Entry->getKind()) {
//      case SymbolTableEntry::RecordID: {
//         auto &R = *Entry->getRecord();
//         if (R.isTemplated()) {
//            Templates.push_back(R.getDecl());
//            return false;
//         }
//
//         WriteRecordCommon(R);
//         break;
//      }
//      case SymbolTableEntry::FunctionID: {
//         size_t i = 0;
//         for (auto &F : Entry->getFunctions()) {
//            if (!F->getTemplateParams().empty())
//               continue;
//
//            if (i++ != 0) NewLine();
//            WriteFunction(*F);
//         }
//
//         return i != 0;
//      }
//      case SymbolTableEntry::VariableID:
//         WriteGlobalVariable(*Entry->getVariable()); break;
//      case SymbolTableEntry::TypedefID: {
//         if (!Entry->getTypedef()->getTemplateParams().empty()) {
////            Templates.push_back(Entry->ge)
//            return false;
//         }
//
//         WriteTypedef(*Entry->getTypedef());
//         break;
//      }
//      case SymbolTableEntry::AliasID: {
//         size_t i = 0;
//         for (auto &alias : Entry->getAliases()) {
//            if (!alias->getTemplateParams().empty()) {
//               Templates.push_back(alias->getDecl());
//               continue;
//            }
//
//            if (i++ != 0) NewLine();
//            WriteAlias(*alias);
//         }
//
//         return i != 0;
//      }
//      default:
//         break;
//   }
//
//   return true;
//}
//
//void WriterImpl::WriteRecordCommon(const Record &R)
//{
//   out << Names[R.getTypeID()] << " ";
//   WriteRecordName(&R);
//
//   out << " ";
//
//   if (auto C = dyn_cast<Class>(&R)) {
//      if (auto P = C->getParent())
//         out << P->getName();
//   }
//
//   WriteList(R.getConformances(), &WriterImpl::WriteRecordName, "with ", ", ",
//             "", true);
//
//   out << " {";
//
//   CurrentTab += 3;
//   bool prevWritten = false;
//
//   NewLine();
//
//   for (auto &I : R.getInnerRecords()) {
//      prevWritten = true;
//
//      ApplyTab();
//      WriteRecordCommon(*I);
//      NewLine();
//   }
//
//   if (prevWritten) {
//      NewLine();
//      prevWritten = false;
//   }
//
//   for (auto &F : R.getFields()) {
//      prevWritten = true;
//
//      ApplyTab();
//      WriteField(F);
//      NewLine();
//   }
//
//   if (prevWritten) {
//      NewLine();
//      prevWritten = false;
//   }
//
//   for (auto &P : R.getProperties()) {
//      prevWritten = true;
//
//      ApplyTab();
//      WriteProp(P.col);
//      NewLine();
//   }
//
//   if (prevWritten) {
//      NewLine();
//      prevWritten = false;
//   }
//
//   for (auto &AT : R.getAssociatedTypes()) {
//      prevWritten = true;
//
//      ApplyTab();
//      WriteAssociatedType(AT);
//      NewLine();
//   }
//
//   if (prevWritten) {
//      NewLine();
//      prevWritten = false;
//   }
//
//   for (auto &M : R.getMethods()) {
//      if (M.col.isTemplate()) {
//         Templates.push_back(M.col.getDeclaration());
//         continue;
//      }
//
//      if (M.col.getName() == "deinit" || M.col.isMemberwiseInitializer())
//         continue;
//
//      if (M.col.getName().size() >= 2
//          && M.col.getName().substr(0, 2) == "__")
//         continue;
//
//      prevWritten = true;
//
//      ApplyTab();
//      WriteMethod(M.col);
//      NewLine();
//   }
//
//   if (auto E = dyn_cast<Enum>(&R)) {
//      if (prevWritten) {
//         NewLine();
//         prevWritten = false;
//      }
//
//      for (auto &C : E->getCases()) {
//         prevWritten = true;
//
//         ApplyTab();
//         WriteCase(C.col);
//         NewLine();
//      }
//   }
//
//   if (auto U = dyn_cast<Union>(&R)) {
//      if (prevWritten) {
//         NewLine();
//         prevWritten = false;
//      }
//
//      for (auto &C : U->getFields()) {
//         prevWritten = true;
//
//         ApplyTab();
//         out << (U->isConst() ? "let " : "var ")
//             << C.line << ": ";
//
//         WriteType(C.col);
//         NewLine();
//      }
//   }
//
//   CurrentTab -= 3;
//
//   ApplyTab();
//   out << "}";
//}
//
//void WriterImpl::WriteField(Field const &F)
//{
//   out << (F.isIsStatic() ? "static " : "")
//       << (F.getFieldType().isConst() ? "let " : "var ");
//
//   WriteIdentifier(F.getFieldName());
//   out << ": ";
//
//   WriteType(F.getFieldType());
//
//   if (F.hasGetter() || F.hasSetter()) {
//      out << " { ";
//      if (F.hasGetter())
//         out << "get";
//
//      if (F.hasSetter()) {
//         if (F.hasGetter())
//            out << " ";
//
//         out << "set";
//      }
//
//      out << " }";
//   }
//}
//
//void WriterImpl::WriteProp(Property const &P)
//{
//   out << (P.isStatic() ? "static " : "")
//       << "prop ";
//
//   WriteIdentifier(P.getName());
//   out << ": ";
//   WriteType(P.getType());
//
//   out << " { ";
//   if (P.hasGetter())
//      out << "get";
//
//   if (P.hasSetter()) {
//      if (P.hasGetter())
//         out << " ";
//
//      out << "set";
//   }
//
//   out << " }";
//}
//
//void WriterImpl::WriteAssociatedType(AssociatedType const &A)
//{
//   out << "associatedType ";
//   WriteIdentifier(A.getName());
//
//   if (A.getType()) {
//      out << ": ";
//      WriteType(A.getType());
//   }
//}
//
//void WriterImpl::WriteMethod(Method const &M)
//{
//   WriteFunction(M);
//}
//
//void WriterImpl::WriteCase(EnumCase const &C)
//{
//   out << "case ";
//   WriteIdentifier(C.name);
//   WriteList(C.associatedValues, &WriterImpl::WriteArgument, "(", ", ", ")",
//             true);
//
//   out << " = " << C.rawValue;
//}
//
//void WriterImpl::WriteFunction(const Callable &F)
//{
//   if (F.isOperator())
//      return WriteOperator(F);
//
//   if (F.isExternC())
//      out << "extern(C) ";
//
//   bool isInit = false;
//   if (auto M = dyn_cast<Method>(&F)) {
//      if (M->isInitializer()) {
//         out << "init ";
//         isInit = true;
//      }
//   }
//
//   if (!isInit) {
//      out << "def ";
//      WriteIdentifier(F.getName());
//      out << " ";
//   }
//
//   WriteList(F.getArguments(), &WriterImpl::WriteArgument);
//   out << " -> ";
//
//   WriteType(F.getReturnType());
//}
//
//void WriterImpl::WriteOperator(const Callable &F)
//{
//   if (F.isExternC())
//      out << "extern(C) ";
//
//   out << "def ";
//
//   auto &op = F.getOperator();
//   switch (op.getFix()) {
//      case FixKind::Infix:   out << "infix "; break;
//      case FixKind::Prefix:  out << "prefix "; break;
//      case FixKind::Postfix: out << "postfix "; break;
//   }
//
//   if (F.isConversionOp()) {
//      out << "as ";
//      WriteType(F.getReturnType());
//   }
//   else {
//      WriteIdentifier(F.getNameWithoutFix());
//   }
//
//   out << " ";
//
//   WriteList(F.getArguments(), &WriterImpl::WriteArgument);
//   out << " -> ";
//
//   WriteType(F.getReturnType());
//}
//
//void WriterImpl::WriteTypedef(const Typedef &TD)
//{
//   out << "typedef ";
//   WriteType(TD.getAliasedType());
//
//   out << " as ";
//   WriteIdentifier(TD.getAlias());
//}
//
//void WriterImpl::WriteAlias(const Alias &alias)
//{
//   out << "alias ";
//   WriteIdentifier(alias.getName());
//}
//
//void WriterImpl::WriteGlobalVariable(const Variable &V)
//{
//   out << (V.getType().isConst() ? "let " : "var ");
//   WriteIdentifier(V.getName());
//
//   out << ": ";
//   WriteType(V.getType());
//}
//
//} // namespace module
//} // namespace cdot