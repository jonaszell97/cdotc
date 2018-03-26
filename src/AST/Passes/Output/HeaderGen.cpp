////
//// Created by Jonas Zell on 26.09.17.
////
//
//#include "HeaderGen.h"
//
//#include <fstream>
//
//#include "../../../Compiler.h"
//#include "../ASTIncludes.h"
//#include "../../Attribute/Attribute.h"
//
//#include "../../../Variant/Type/PrimitiveType.h"
//#include "../../../Variant/Type/GenericType.h"
//
//#include "../SemanticAnalysis/Record/Class.h"
//#include "../SemanticAnalysis/Record/Enum.h"
//#include "../SemanticAnalysis/Function.h"
//
//#include "../../../Files/FileUtils.h"
//#include "../../../Files/FileManager.h"
//
//
//namespace {
//   void removeTrailingWhitespace(string& src) {
//      long i = src.length() - 1;
//      size_t count = 0;
//      while (src[i] == ' ' || src[i] == '\n' || src[i] == '\r') {
//         ++count;
//         --i;
//
//         if (i < 0) {
//            break;
//         }
//      }
//
//      src = src.substr(0, src.length() - count);
//
//      i = 0;
//      count = 0;
//      while (src[i] == ' ' || src[i] == '\n' || src[i] == '\r') {
//         ++count;
//         ++i;
//
//         if (i == src.length()) {
//            break;
//         }
//      }
//
//      src = src.substr(count);
//   }
//
//   string accessModifierToString(AccessModifier access)
//   {
//      switch (access) {
//         case AccessModifier::PUBLIC:
//            return "public";
//         case AccessModifier::PRIVATE:
//            return "private";
//         case AccessModifier::PROTECTED:
//            return "protected";
//         case AccessModifier::DEFAULT:
//            return "";
//      }
//   }
//}
//
//HeaderGen::HeaderGen(
//   std::vector<string> &fileNames) : fileNames(fileNames)
//{
//
//}
//
//HeaderGen::~HeaderGen()
//{
//
//}
//
//void HeaderGen::run(std::vector<CompilationUnit> &CUs)
//{
//   auto &options = Compiler::getOptions();
//   for (size_t i = options.headerFiles.size(); i < CUs.size(); ++i) {
//      auto &root = CUs[i].root;
//      visit(root);
//      finalize(fileNames[i]);
//   }
//}
//
//void HeaderGen::finalize(const string &fileName)
//{
//   auto &options = Compiler::getOptions();
//   fs::mkdirIfNotExists(options.headerOutPath);
//
//   auto path = fs::getPath(fileName).substr(options.basePath.length());
//   auto outFileName = fs::getFileName(fileName);
//
//   string outPath = options.basePath + options.headerOutPath + path;
//   fs::mkdirIfNotExists(outPath);
//
//   string outFile = outPath + outFileName + ".doth";
//
//   auto str = out.str();
//   out.flush();
//
//   removeTrailingWhitespace(str);
//
//   std::ofstream outfile(outFile);
//   outfile << str;
//
//   outfile.close();
//}
//
//void HeaderGen::addSpace(size_t num)
//{
//   out << string(num, ' ');
//}
//
//void HeaderGen::applyIndent()
//{
//   addSpace(currentIndent);
//}
//
//void HeaderGen::increaseIndent()
//{
//   currentIndent += indentStep;
//}
//
//void HeaderGen::decreaseIndent()
//{
//   currentIndent = currentIndent == 0 ? 0 : currentIndent - indentStep;
//}
//
//void HeaderGen::nextLine()
//{
//   out << '\n';
//}
//
//void HeaderGen::openBrace()
//{
//   out << '{';
//   increaseIndent();
//   nextLine();
//}
//
//void HeaderGen::closeBrace()
//{
//   decreaseIndent();
//   applyIndent();
//   out << "}\n";
//}
//
//void HeaderGen::write(string str, bool space)
//{
//   out << str;
//   if (space) {
//      addSpace();
//   }
//}
//
//void HeaderGen::writec(char c)
//{
//   out << c;
//}
//
//namespace {
//   string getAttrNameFromKind(Attr kind)
//   {
//      for (const auto& attr : AttributeMap) {
//         if (attr.col == kind) {
//            return attr.line;
//         }
//      }
//
//      llvm_unreachable("Unknown attribute");
//   }
//}
//
//void HeaderGen::writeAttributes(const std::vector<Attribute> &attrs,
//                                bool indent)
//{
//   if (attrs.empty()) {
//      return;
//   }
//
//   if(indent) {
//      applyIndent();
//   }
//
//   for (const auto& attr : attrs) {
//      writec('@');
//      out << getAttrNameFromKind(attr.kind);
//      writec('(');
//
//      auto argc = attr.args.size();
//      for (size_t i = 0; i < argc; ++i) {
//         writeIdent(attr.args[i].toString(), false);
//         if (i < argc - 1) {
//            write(",");
//         }
//      }
//
//      writec(')');
//      if (indent) {
//         nextLine();
//      }
//      else {
//         writec(' ');
//      }
//   }
//}
//
//void HeaderGen::writeThrows(Callable *callable)
//{
//   const auto &thrown = callable->getThrownTypes();
//   if (thrown.empty()) {
//      return;
//   }
//
//   applyIndent();
//   write("@throws(", false);
//
//   auto numTys = thrown.size();
//   size_t i = 0;
//   for (const auto &ty : thrown) {
//      write(ty->toString(), false);
//      if (i < numTys - 1) {
//         write(",");
//      }
//
//      ++i;
//   }
//
//   writec(')');
//   nextLine();
//}
//
//void HeaderGen::writeGenerics(const std::vector<TemplateConstraint> &generics)
//{
//   if (generics.empty()) {
//      return;
//   }
//
//   writec('<');
//   auto genc = generics.size();
//   for (size_t i = 0; i < genc; ++i) {
//      const auto& gen  = generics[i];
//      writeIdent(gen.genericTypeName, false);
//
//      if (gen.covariance) {
//         write(":");
//         write(gen.covariance->toString(), false);
//      }
//
//      if (i < genc - 1) {
//         write(",");
//      }
//   }
//
//   write(">");
//}
//
//void HeaderGen::writeGenerics(const std::vector<TemplateArgExpr*> &generics)
//{
//   if (generics.empty()) {
//      return;
//   }
//
//   writec('<');
//   size_t i = 0;
//   auto genc = generics.size();
//   for (const auto& gen : generics) {
//      write(gen.toString(), false);
//      if (i < genc - 1) {
//         write(",");
//      }
//   }
//
//   writec('>');
//}
//
//void HeaderGen::writeProtocols(const std::vector<RecordType *> &conformsTo)
//{
//   if (conformsTo.empty()) {
//      return;
//   }
//
//   write("with");
//   auto protc = conformsTo.size();
//   for (size_t i = 0; i < protc; ++i) {
//      const auto& gen = conformsTo[i];
//      writeIdent(gen->getClassName(), false);
//      writeGenerics(gen->getTemplateArgs());
//
//      if (i < protc - 1) {
//         write(",");
//      }
//   }
//
//   addSpace();
//}
//
//void HeaderGen::writeProtocols(
//   const std::vector<TypeRef* > &conformsTo)
//{
//   if (conformsTo.empty()) {
//      return;
//   }
//
//   write("with");
//   auto protc = conformsTo.size();
//   for (size_t i = 0; i < protc; ++i) {
//      const auto& gen = conformsTo[i];
//      writeIdent(gen->getType()->toString(), false);
//      writeGenerics(gen->type->getTemplateArgs());
//
//      if (i < protc - 1) {
//         write(",");
//      }
//   }
//
//   addSpace();
//}
//
//void HeaderGen::writeArgs(const std::vector<FuncArgDecl* > &args)
//{
//   writec('(');
//
//   auto argc = args.size();
//   for (size_t i = 0; i < argc; ++i) {
//      auto& arg = args[i];
//      arg->accept(this);
//      if (i < argc - 1) {
//         write(",");
//      }
//   }
//
//   writec(')');
//}
//
//void HeaderGen::writeAccess(const AccessModifier &access)
//{
//   auto str = accessModifierToString(access);
//   write(str, !str.empty());
//}
//
//namespace {
//   string invalidIdentifierChars = R"_(\\ \[\]\+\-\*/%&\|!=<>\.~\^,\(\)\{\}?:;@"'#)_";
//   std::regex identifierRegex(
//      "[^" + invalidIdentifierChars + "0-9][^" + invalidIdentifierChars + "]*"
//   );
//}
//
//void HeaderGen::writeIdent(const string &ident, bool newLine)
//{
//   if (std::regex_match(ident, identifierRegex)) {
//      write(ident, newLine);
//   }
//   else {
//      writec('`');
//      write(ident, false);
//      write("`", newLine);
//   }
//}
//
//void HeaderGen::writeExternKind(ExternKind &kind)
//{
//   switch(kind) {
//      case ExternKind::NONE:
//         return;
//      case ExternKind::C:
//         write("C");
//         break;
//      case ExternKind::CPP:
//         write("C++");
//         break;
//   }
//}
//
//void HeaderGen::visit(NamespaceDecl *node)
//{
//   if (node->isAnonymousNamespace_) {
//      return;
//   }
//
//   applyIndent();
//   write("namespace");
//   writeIdent(node->nsName);
//
//   openBrace();
//   node->contents->accept(this);
//   closeBrace();
//}
//
//void HeaderGen::visit(UsingStmt *node)
//{
//   write("using");
//   writeIdent(node->importNamespace, false);
//   if (node->isWildcardImport) {
//      write(".*\n", false);
//      return;
//   }
//
//   write(".{");
//
//   size_t i = 0;
//   size_t cnt = node->importedItems.size();
//   for (const auto& item : node->importedItems) {
//      writeIdent(item, false);
//      if (i < cnt - 1) {
//         write(",");
//      }
//
//      ++i;
//   }
//
//   write(" }\n", false);
//}
//
//void HeaderGen::visit(CompoundStmt *node)
//{
//   size_t nodeCnt = node->statements.size() - 1;
//   size_t i = 0;
//   NodeType lastType = NodeType::LambdaExprID; // can never happen
//
//   for (const auto& stmt : node->statements) {
//      stmt->accept(this);
//
//      auto newType = stmt->get_type();
//      bool needsIndent = lastType != NodeType::LambdaExprID && (lastType != newType
//         || newType == NodeType::CLASS_DECL || newType == NodeType::ENUM_DECL);
//
//      if (needsIndent && i != nodeCnt) {
//         nextLine();
//      }
//
//      ++i;
//      lastType = newType;
//   }
//}
//
//void HeaderGen::visit(DeclStmt *node)
//{
//   writeAttributes(node->attributes);
//
//   applyIndent();
//   write("declare");
//   writeExternKind(node->externKind);
//
//   writeAccess(node->access);
//
//   write(node->is_const ? "let" : "var");
//   writeIdent(node->identifier, false);
//
//   write(":");
//
//   node->type->type.isLvalue(false);
//   node->type->accept(this);
//   node->type->type.isLvalue(true);
//
//   nextLine();
//}
//
//void HeaderGen::visit(FunctionDecl *node)
//{
//   writeAttributes(node->attributes);
//   writeThrows(node->getCallable());
//
//   applyIndent();
//   write("declare");
//   writeExternKind(node->externKind);
//
//   write("def");
//   writeIdent(node->getName(), false);
//
//   writeArgs(node->getArgs());
//
//   write(" ->");
//   node->getReturnType()->accept(this);
//   nextLine();
//}
//
//void HeaderGen::visit(DeclareStmt *node)
//{
//   for (const auto& decl : node->declarations) {
//      decl->accept(this);
//   }
//}
//
//namespace {
//   size_t innerDecl = 0;
//}
//
//void HeaderGen::visit(ClassDecl *node)
//{
////   if (node->declaredClass->isGeneric()) {
////      auto start = node->getStartIndex();
////      auto end = node->getEndIndex();
////      write(node->getSourceFile().second.substr(start, end - start));
////
////      return;
////   }
//
//   writeAttributes(node->attributes);
//
//   applyIndent();
//
//   if (innerDecl == 0) {
//      write("declare");
//      writeExternKind(node->externKind);
//   }
//
//   writeAccess(node->am);
//   if (node->IsAbstract && !node->is_protocol) {
//      write("abstract");
//   }
//
//   write(node->is_struct ? "struct" : node->is_protocol ? "protocol" : "class");
//
//   writeIdent(node->recordName);
//
//   if (node->parentClass != nullptr) {
//      writec(':');
//      writeIdent(node->parentClass->type->getClassName());
//   }
//
//   writeProtocols(node->conformsTo);
//   openBrace();
//
//   if (!node->typedefs.empty()) {
//      for (const auto &td : node->typedefs) {
//         td->accept(this);
//      }
//
//      if (!node->innerDeclarations.empty()) {
//         nextLine();
//      }
//   }
//
//   if (!node->innerDeclarations.empty()) {
//      ++innerDecl;
//      for (const auto &inner : node->innerDeclarations) {
//         inner->accept(this);
//      }
//
//      --innerDecl;
//      if (!node->fields.empty()) {
//         nextLine();
//      }
//   }
//
//   if (!node->fields.empty()) {
//      for (const auto &field : node->fields) {
//         field->accept(this);
//      }
//
//      if (!node->constructors.empty()) {
//         nextLine();
//      }
//   }
//
//   if (!node->constructors.empty()) {
//      for (const auto &constr : node->constructors) {
//         constr->accept(this);
//      }
//
//      if (!node->methods.empty()) {
//         nextLine();
//      }
//   }
//
//   for (const auto& method : node->methods) {
//      method->accept(this);
//   }
//
//   closeBrace();
//}
//
//void HeaderGen::visit(MethodDecl *node)
//{
//   writeAttributes(node->attributes);
//   writeThrows(node->getMethod());
//
//   applyIndent();
//   writeAccess(node->getAm());
//   if (node->isStatic) {
//      write("static");
//   }
//
//   write("def");
//
//   string methodName = node->getName();
//   if (node->getName().substr(0, 6) == "infix ") {
//      write("infix");
//      methodName = methodName.substr(6);
//
//      if (methodName.substr(0, 3) == "as ") {
//         write("as");
//         methodName = methodName.substr(3);
//      }
//   }
//   else if (node->getName().substr(0, 7) == "prefix ") {
//      write("prefix");
//      methodName = methodName.substr(7);
//   }
//   else if (node->getName().substr(0, 8) == "postfix ") {
//      write("postfix");
//      methodName = methodName.substr(8);
//   }
//
//   writeIdent(methodName, false);
//
//   writeArgs(node->getArgs());
//
//   write(" ->");
//   node->getReturnType()->accept(this);
//   nextLine();
//}
//
//void HeaderGen::visit(FieldDecl *node)
//{
//   applyIndent();
//   writeAccess(node->access);
//   if (node->is_static) {
//      write("static");
//   }
//
//   write(node->is_const ? "let" : "var");
//
//   writeIdent(node->name, false);
//   write(":");
//
//   node->type->accept(this);
//
//   if (node->has_getter || node->has_setter) {
//      write(" {");
//
//      if (node->has_getter) {
//         write("get");
//      }
//      if (node->has_setter) {
//         write("set");
//      }
//
//      writec('}');
//   }
//
//   nextLine();
//}
//
//void HeaderGen::visit(ConstrDecl *node)
//{
//   writeAttributes(node->attributes);
//   writeThrows(node->getMethod());
//
//   applyIndent();
//   writeAccess(node->am);
//   write("init");
//
//   writeArgs(node->args);
//   nextLine();
//}
//
//void HeaderGen::visit(EnumDecl *node)
//{
//   writeAttributes(node->attributes);
//
//   applyIndent();
//   write("declare");
//   writeExternKind(node->externKind);
//
//   writeAccess(node->am);
//
//   write("enum");
//   writeIdent(node->recordName, false);
//
//   if (node->rawType != nullptr) {
//      writec('(');
//      write(node->rawType->type->toString(), false);
//      write(")");
//   }
//   else {
//      addSpace();
//   }
//
//   writeProtocols(node->conformsTo);
//   openBrace();
//
//   if (!node->innerDeclarations.empty()) {
//      for (const auto &inner : node->innerDeclarations) {
//         inner->accept(this);
//      }
//
//      if (!node->cases.empty()) {
//         nextLine();
//      }
//   }
//
//   if (!node->cases.empty()) {
//      for (const auto& case_ : node->cases) {
//         case_->accept(this);
//      }
//
//      if (!node->methods.empty()) {
//         nextLine();
//      }
//   }
//
//   for (const auto& method : node->methods) {
//      method->accept(this);
//   }
//
//   closeBrace();
//}
//
//void HeaderGen::visit(EnumCaseDecl *node)
//{
//   applyIndent();
//
//   write("case");
//   writeIdent(node->caseName);
//   if (!node->associatedTypes.empty()) {
//      writec('(');
//
//      auto argc = node->associatedTypes.size();
//      for (size_t i = 0; i < argc; ++i) {
//         auto& arg = node->associatedTypes[i];
//         if (!arg.line.empty()) {
//            writeIdent(arg.line, false);
//            write(":");
//         }
//
//         arg.col->accept(this);
//         if (i < argc - 1) {
//            write(",");
//         }
//      }
//
//      write(")");
//   }
//
//   write("= " + std::to_string(node->rawValue), false);
//   nextLine();
//}
//
//void HeaderGen::visit(FuncArgDecl *node)
//{
//   if (node->argType->isCStyleVararg()) {
//      write("...", false);
//      return;
//   }
//
//   if (!node->argName.empty()) {
//      writeIdent(node->argName, false);
//      write(":");
//   }
//
//   if (node->isMut()) {
//      write("ref");
//   }
//
//   node->argType->accept(this);
//
//   if (node->defaultVal != nullptr) {
//      write(" =");
//
//      auto loc = node->defaultVal->getSourceLoc();
//      auto srcFile = fs::FileManager::getBuffer(loc);
//
//      auto *buf = srcFile->getBufferStart();
//      auto size = srcFile->getBufferSize();
//      unsigned line = 1;
//      unsigned locLine = loc.getLine();
//
//      unsigned i;
//      for (i = 0; line < locLine; ++i) {
//         assert(i < size);
//         if (buf[i] == '\n') {
//            ++line;
//         }
//      }
//
//      i += loc.getCol() - 1;
//      write(string(&buf[i], loc.getLength()), false);
//   }
//}
//
//void HeaderGen::visit(TypedefDecl *node)
//{
//   applyIndent();
//
//   write("typedef");
//
//   node->origin->accept(this);
//   write(" =");
//
//   writeIdent(node->alias, false);
//   writeGenerics(node->templateArgs);
//
//   nextLine();
//}
//
//void HeaderGen::visit(TypeRef *node)
//{
//   writeAttributes(node->attributes, false);
//   write(node->type->toString(), false);
//}
//
//void HeaderGen::visit(DebugStmt *node)
//{
//
//}
