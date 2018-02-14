////
//// Created by Jonas Zell on 26.09.17.
////
//
//#ifndef CDOT_HEADERGEN_H
//#define CDOT_HEADERGEN_H
//
//#include <string>
//#include <sstream>
//#include <unordered_map>
//#include "../AbstractPass.h"
//
//namespace cdot {
//
//struct Attribute;
//class ObjectType;
//class Type;
//class GenericType;
//class Callable;
//struct TemplateArg;
//struct TemplateConstraint;
//
//}
//
//enum class ExternKind : unsigned char;
//enum class AccessModifier : unsigned char;
//
//using std::string;
//using std::stringstream;
//using std::unordered_map;
//
//using namespace cdot;
//
//class HeaderGen : public AbstractPass {
//public:
//   explicit HeaderGen(
//      std::vector<string>& fileNames
//   );
//
//   ~HeaderGen() override;
//
//   void run(std::vector<CompilationUnit> &CUs) override;
//   void finalize(const string &fileName);
//
//   void visit(NamespaceDecl *node) override;
//   void visit(UsingStmt *node) override;
//   void visit(CompoundStmt *node) override;
//
//   void visit(DeclStmt *node) override;
//   void visit(FunctionDecl *node) override;
//   void visit(DeclareStmt *node) override;
//
//   void visit(ClassDecl *node) override;
//   void visit(MethodDecl *node) override;
//   void visit(FieldDecl *node) override;
//   void visit(ConstrDecl *node) override;
//
//   void visit(EnumDecl *node) override;
//   void visit(EnumCaseDecl *node) override;
//
//   void visit(FuncArgDecl *node) override;
//   void visit(TypedefDecl *node) override;
//   void visit(TypeRef *node) override;
//
//   void visit(DebugStmt *node) override;
//
//protected:
//   std::vector<string>& fileNames;
//   stringstream out;
//
//   size_t currentIndent = 0;
//   size_t indentStep = 3;
//
//   void write(string str, bool space = true);
//   void writec(char c);
//
//   void nextLine();
//   void openBrace();
//   void closeBrace();
//
//   void addSpace(size_t num = 1);
//   void applyIndent();
//   void increaseIndent();
//   void decreaseIndent();
//
//   void writeAttributes(const std::vector<Attribute>& attrs,
//                        bool indent = true);
//   void writeThrows(Callable *callable);
//   void writeExternKind(ExternKind& kind);
//
//   void writeGenerics(const std::vector<TemplateConstraint>& generics);
//   void writeGenerics(const std::vector<TemplateArgExpr*>& generics);
//
//   void writeProtocols(const std::vector<ObjectType*>& conformsTo);
//   void writeProtocols(const std::vector<TypeRef* >& conformsTo);
//
//   void writeArgs(const std::vector<FuncArgDecl* >& args);
//   void writeAccess(const AccessModifier& access);
//
//   void writeIdent(const string& ident, bool newLine = true);
//};
//
//
//#endif //CDOT_HEADERGEN_H
