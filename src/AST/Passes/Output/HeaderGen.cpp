//
// Created by Jonas Zell on 26.09.17.
//

#include <fstream>
#include "HeaderGen.h"
#include "../ASTIncludes.txt"
#include "../../Attribute/Attribute.h"
#include "../../../Variant/Type/PrimitiveType.h"
#include "../StaticAnalysis/Class.h"

namespace {
   void removeTrailingWhitespace(string& src) {
      long i = src.length() - 1;
      size_t count = 0;
      while (src[i] == ' ' || src[i] == '\n' || src[i] == '\r') {
         ++count;
         --i;

         if (i < 0) {
            break;
         }
      }

      src = src.substr(0, src.length() - count);

      i = 0;
      count = 0;
      while (src[i] == ' ' || src[i] == '\n' || src[i] == '\r') {
         ++count;
         ++i;

         if (i == src.length()) {
            break;
         }
      }

      src = src.substr(count);
   }

   string accessModifierToString(AccessModifier access)
   {
      switch (access) {
         case AccessModifier::PUBLIC:
            return "public";
         case AccessModifier::PRIVATE:
            return "private";
         case AccessModifier::PROTECTED:
            return "protected";
         case AccessModifier::DEFAULT:
            return "";
      }
   }
}

HeaderGen::HeaderGen(string& fileName) :
   fileName(fileName)
{
   nextLine();
   PrimitiveType::PrintSpecificTypes = true;
}

HeaderGen::~HeaderGen()
{
   PrimitiveType::PrintSpecificTypes = false;
}

void HeaderGen::finalize()
{
   auto str = out.str();
   out.flush();

   removeTrailingWhitespace(str);

   std::ofstream outfile(fileName);
   outfile << str;

   outfile.close();
}

void HeaderGen::addSpace(size_t num)
{
   out << string(num, ' ');
}

void HeaderGen::applyIndent()
{
   addSpace(currentIndent);
}

void HeaderGen::increaseIndent()
{
   currentIndent += indentStep;
}

void HeaderGen::decreaseIndent()
{
   currentIndent = currentIndent == 0 ? 0 : currentIndent - indentStep;
}

void HeaderGen::nextLine()
{
   out << '\n';
}

void HeaderGen::openBrace()
{
   out << '{';
   increaseIndent();
   nextLine();
}

void HeaderGen::closeBrace()
{
   decreaseIndent();
   applyIndent();
   out << "}\n";
}

void HeaderGen::write(string str, bool space)
{
   out << str;
   if (space) {
      addSpace();
   }
}

void HeaderGen::writec(char c)
{
   out << c;
}

namespace {
   string getAttrNameFromKind(Attr kind)
   {
      for (const auto& attr : AttributeMap) {
         if (attr.second == kind) {
            return attr.first;
         }
      }

      llvm_unreachable("Unknown attribute");
   }
}

void HeaderGen::writeAttributes(std::vector<Attribute> &attrs)
{
   if (attrs.empty()) {
      return;
   }

   applyIndent();
   for (const auto& attr : attrs) {
      writec('@');
      out << getAttrNameFromKind(attr.kind);
      if (!attr.args.empty()) {
         writec('(');

         auto argc = attr.args.size();
         for (size_t i = 0; i < argc; ++i) {
            writeIdent(attr.args[i], false);
            if (i < argc - 1) {
               write(",");
            }
         }

         writec(')');
      }

      nextLine();
   }
}

void HeaderGen::writeGenerics(std::vector<ObjectType *> &generics)
{
   if (generics.empty()) {
      return;
   }

   writec('<');
   auto genc = generics.size();
   for (size_t i = 0; i < genc; ++i) {
      const auto& gen  = generics[i];
      writeIdent(gen->getGenericClassName(), false);
      write(":");

      write(gen->getClassName(), false);
      if (i < genc - 1) {
         write(",");
      }
   }

   write(">");
}

void HeaderGen::writeGenerics(unordered_map<string, Type *> &generics)
{
   if (generics.empty()) {
      return;
   }

   writec('<');
   size_t i = 0;
   auto genc = generics.size();
   for (const auto& gen : generics) {
      write(gen.second->toString(), false);
      if (i < genc - 1) {
         write(",");
      }
   }

   write(">");
}

void HeaderGen::writeProtocols(std::vector<ObjectType *> &conformsTo)
{
   if (conformsTo.empty()) {
      return;
   }

   write("with");
   auto protc = conformsTo.size();
   for (size_t i = 0; i < protc; ++i) {
      const auto& gen = conformsTo[i];
      writeIdent(gen->getClassName(), false);
      writeGenerics(gen->getConcreteGenericTypes());

      if (i < protc - 1) {
         write(",");
      }
   }

   addSpace();
}

void HeaderGen::writeArgs(std::vector<std::shared_ptr<FuncArgDecl>> &args)
{
   writec('(');

   auto argc = args.size();
   for (size_t i = 0; i < argc; ++i) {
      auto& arg = args[i];
      arg->accept(this);
      if (i < argc - 1) {
         write(",");
      }
   }

   writec(')');
}

void HeaderGen::writeAccess(AccessModifier &access)
{
   auto str = accessModifierToString(access);
   write(str, !str.empty());
}

namespace {
   string invalidIdentifierChars = R"_(\\ \[\]\+\-\*/%&\|!=<>\.~\^,\(\)\{\}?:;@"'#)_";
   std::regex identifierRegex(
      "[^" + invalidIdentifierChars + "0-9][^" + invalidIdentifierChars + "]*"
   );
}

void HeaderGen::writeIdent(const string &ident, bool newLine)
{
   if (std::regex_match(ident, identifierRegex)) {
      write(ident, newLine);
   }
   else {
      writec('`');
      write(ident, false);
      write("`", newLine);
   }
}

void HeaderGen::writeExternKind(ExternKind &kind)
{
   switch(kind) {
      case ExternKind::NONE:
         return;
      case ExternKind::C:
         write("C");
         break;
      case ExternKind::CPP:
         write("C++");
         break;
   }
}

void HeaderGen::visit(NamespaceDecl *node)
{
   if (node->isAnonymousNamespace) {
      return;
   }

   applyIndent();
   write("namespace");
   writeIdent(node->nsName);

   openBrace();
   node->contents->accept(this);
   closeBrace();
}

void HeaderGen::visit(UsingStmt *node)
{
   write("using");
   writeIdent(node->importNamespace, false);
   if (node->isWildcardImport) {
      write(".*\n", false);
      return;
   }

   write(".{");

   size_t i = 0;
   size_t cnt = node->importedItems.size();
   for (const auto& item : node->importedItems) {
      writeIdent(item, false);
      if (i < cnt - 1) {
         write(",");
      }

      ++i;
   }

   write(" }\n", false);
}

void HeaderGen::visit(CompoundStmt *node)
{
   size_t nodeCnt = node->statements.size() - 1;
   size_t i = 0;
   NodeType lastType = NodeType::LAMBDA_EXPR; // can never happen

   for (const auto& stmt : node->statements) {
      stmt->accept(this);

      auto newType = stmt->get_type();
      bool needsIndent = lastType != NodeType::LAMBDA_EXPR && (lastType != newType
         || newType == NodeType::CLASS_DECL || newType == NodeType::ENUM_DECL);

      if (needsIndent && i != nodeCnt) {
         nextLine();
      }

      ++i;
      lastType = newType;
   }
}

void HeaderGen::visit(DeclStmt *node)
{
   writeAttributes(node->attributes);

   applyIndent();
   write("declare");
   writeAccess(node->access);
   writeExternKind(node->externKind);

   write(node->is_const ? "let" : "var");
   writeIdent(node->identifier, false);

   write(":");

   node->type->type->isLvalue(false);
   node->type->accept(this);
   node->type->type->isLvalue(true);

   nextLine();
}

void HeaderGen::visit(FunctionDecl *node)
{
   writeAttributes(node->attributes);

   applyIndent();
   write("declare");
   writeExternKind(node->externKind);

   write("def");
   writeIdent(node->funcName, false);
   writeGenerics(node->generics);

   writeArgs(node->args);

   write(" ->");
   node->returnType->accept(this);
   nextLine();
}

void HeaderGen::visit(DeclareStmt *node)
{
   for (const auto& decl : node->declarations) {
      decl->accept(this);
   }
}

namespace {
   size_t innerDecl = 0;
}

void HeaderGen::visit(ClassDecl *node)
{
//   if (node->declaredClass->isGeneric()) {
//      auto start = node->getStartIndex();
//      auto end = node->getEndIndex();
//      write(node->getSourceFile().second.substr(start, end - start));
//
//      return;
//   }

   writeAttributes(node->attributes);

   applyIndent();

   if (!node->is_extension) {
      if (!node->is_protocol && innerDecl == 0) {
         write("declare");
         writeExternKind(node->externKind);
      }

      writeAccess(node->am);
      if (node->is_abstract && !node->is_protocol) {
         write("abstract");
      }

      write(node->is_struct ? "struct" : node->is_protocol ? "protocol" : "class");
   }
   else {
      write("declare extend");
   }

   writeIdent(node->className);
   writeGenerics(node->generics);

   if (node->parentClass != nullptr) {
      writec(':');
      writeIdent(node->parentClass->getClassName());
   }

   writeProtocols(node->conformsTo);
   openBrace();

   if (!node->typedefs.empty()) {
      for (const auto &td : node->typedefs) {
         td->accept(this);
      }

      if (!node->innerDeclarations.empty()) {
         nextLine();
      }
   }

   if (!node->innerDeclarations.empty()) {
      ++innerDecl;
      for (const auto &inner : node->innerDeclarations) {
         inner->accept(this);
      }

      --innerDecl;
      if (!node->fields.empty()) {
         nextLine();
      }
   }

   if (!node->fields.empty()) {
      for (const auto &field : node->fields) {
         field->accept(this);
      }

      if (!node->constructors.empty()) {
         nextLine();
      }
   }

   if (!node->constructors.empty()) {
      for (const auto &constr : node->constructors) {
         constr->accept(this);
      }

      if (!node->methods.empty()) {
         nextLine();
      }
   }

   for (const auto& method : node->methods) {
      method->accept(this);
   }

   closeBrace();
}

void HeaderGen::visit(MethodDecl *node)
{
   writeAttributes(node->attributes);

   applyIndent();
   writeAccess(node->am);
   if (node->isStatic) {
      write("static");
   }

   write("def");
   writeIdent(node->methodName, false);
   writeGenerics(node->generics);

   writeArgs(node->args);

   write(" ->");
   node->returnType->accept(this);
   nextLine();
}

void HeaderGen::visit(FieldDecl *node)
{
   applyIndent();
   writeAccess(node->am);
   if (node->isStatic) {
      write("static");
   }

   write(node->isConst ? "let" : "var");
   writeIdent(node->fieldName, false);
   write(":");

   node->type->accept(this);

   if (node->hasGetter || node->hasSetter) {
      write(" {");

      if (node->hasGetter) {
         write("get");
      }
      if (node->hasSetter) {
         write("set");
      }

      writec('}');
   }

   nextLine();
}

void HeaderGen::visit(ConstrDecl *node)
{
   writeAttributes(node->attributes);

   applyIndent();
   writeAccess(node->am);
   write("init");

   writeArgs(node->args);
   nextLine();
}

void HeaderGen::visit(EnumDecl *node)
{
   writeAttributes(node->attributes);

   applyIndent();
   write("declare");
   writeExternKind(node->externKind);

   writeAccess(node->am);

   write("enum");
   writeIdent(node->className, false);

   if (node->rawType != nullptr) {
      writec('(');
      write(node->rawType->type->toString(), false);
      write(")");
   }
   else {
      addSpace();
   }

   writeGenerics(node->generics);

   writeProtocols(node->conformsTo);
   openBrace();

   if (!node->innerDeclarations.empty()) {
      for (const auto &inner : node->innerDeclarations) {
         inner->accept(this);
      }

      if (!node->cases.empty()) {
         nextLine();
      }
   }

   if (!node->cases.empty()) {
      for (const auto& case_ : node->cases) {
         case_->accept(this);
      }

      if (!node->methods.empty()) {
         nextLine();
      }
   }

   for (const auto& method : node->methods) {
      method->accept(this);
   }

   closeBrace();
}

void HeaderGen::visit(EnumCaseDecl *node)
{
   applyIndent();

   write("case");
   writeIdent(node->caseName);
   if (!node->associatedTypes.empty()) {
      writec('(');

      auto argc = node->associatedTypes.size();
      for (size_t i = 0; i < argc; ++i) {
         auto& arg = node->associatedTypes[i];
         if (!arg.first.empty()) {
            writeIdent(arg.first, false);
            write(":");
         }

         arg.second->accept(this);
         if (i < argc - 1) {
            write(",");
         }
      }

      write(")");
   }

   write("= " + std::to_string(node->rawValue), false);
   nextLine();
}

void HeaderGen::visit(FuncArgDecl *node)
{
   if (node->argType->type->isCStyleVararg()) {
      write("...", false);
      return;
   }

   if (!node->argName.empty()) {
      writeIdent(node->argName, false);
      write(":");
   }

   node->argType->accept(this);

   if (node->defaultVal != nullptr) {
      write(" =");

      auto start = node->defaultVal->getStartIndex();
      auto end = node->defaultVal->getEndIndex() - 1;
      write(node->defaultVal->getSourceFile().second.substr(start, end - start), false);
   }
}

void HeaderGen::visit(TypedefDecl *node)
{
   applyIndent();

   write("typedef");

   node->origin->accept(this);
   addSpace();

   writeIdent(node->alias, false);
   nextLine();
}

void HeaderGen::visit(TypeRef *node)
{
   write(node->type->toString(), false);
}

void HeaderGen::visit(DebugStmt *node)
{

}
