//
// Created by Jonas Zell on 26.09.17.
//

#ifndef CDOT_HEADERGEN_H
#define CDOT_HEADERGEN_H

#include <string>
#include <sstream>
#include <unordered_map>
#include "../AbstractPass.h"

namespace cdot {
   struct Attribute;
   class ObjectType;
   class BuiltinType;
   class GenericType;

   struct GenericConstraint;
}

enum class ExternKind : unsigned char;
enum class AccessModifier : unsigned int;

using std::string;
using std::stringstream;
using std::unordered_map;

using namespace cdot;

class HeaderGen : public AbstractPass {
public:
   explicit HeaderGen(string& fileName);
   ~HeaderGen();

   void finalize() override;

   void visit(NamespaceDecl *node) override;
   void visit(UsingStmt *node) override;
   void visit(CompoundStmt *node) override;

   void visit(DeclStmt *node) override;
   void visit(FunctionDecl *node) override;
   void visit(DeclareStmt *node) override;

   void visit(ClassDecl *node) override;
   void visit(MethodDecl *node) override;
   void visit(FieldDecl *node) override;
   void visit(ConstrDecl *node) override;

   void visit(EnumDecl *node) override;
   void visit(EnumCaseDecl *node) override;

   void visit(FuncArgDecl *node) override;
   void visit(TypedefDecl *node) override;
   void visit(TypeRef *node) override;

   void visit(DebugStmt *node) override;

protected:
   string& fileName;
   stringstream out;

   size_t currentIndent = 0;
   size_t indentStep = 3;

   void write(string str, bool space = true);
   void writec(char c);

   void nextLine();
   void openBrace();
   void closeBrace();

   void addSpace(size_t num = 1);
   void applyIndent();
   void increaseIndent();
   void decreaseIndent();

   void writeAttributes(std::vector<Attribute>& attrs, bool indent = true);
   void writeExternKind(ExternKind& kind);

   void writeGenerics(std::vector<GenericConstraint>& generics);
   void writeGenerics(std::vector<GenericType*>& generics);

   void writeProtocols(std::vector<ObjectType*>& conformsTo);
   void writeProtocols(std::vector<std::shared_ptr<TypeRef>>& conformsTo);

   void writeArgs(std::vector<std::shared_ptr<FuncArgDecl>>& args);
   void writeAccess(AccessModifier& access);

   void writeIdent(const string& ident, bool newLine = true);
};


#endif //CDOT_HEADERGEN_H
