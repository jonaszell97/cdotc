////
//// Created by Jonas Zell on 17.10.17.
////
//
//#ifndef CDOT_MODULEWRITER_H
//#define CDOT_MODULEWRITER_H
//
//#include <string>
//#include <llvm/ADT/SmallVector.h>
//#include <stack>
//#include "../AbstractPass.h"
//#include "BitstreamWriter.h"
//
//class CompoundStmt;
//
//using std::string;
//using llvm::SmallVector;
//
//namespace cdot {
//
//class Type;
//struct QualType;
//struct Argument;
//struct Attribute;
//struct TemplateConstraint;
//struct Variant;
//
//namespace serial {
//
//class ModuleWriter: public AbstractPass {
//public:
//   ModuleWriter(
//      const string &moduleName,
//      std::shared_ptr<CompoundStmt> &moduleRoot
//   );
//
//   void write(
//      const string& outFilePath
//   );
//
//protected:
//   BitstreamWriter Writer;
//   string moduleName;
//   std::shared_ptr<CompoundStmt> moduleRoot;
//
//   void writeString(const string &str);
//   void writeSize(const size_t &size);
//   void writeBool(bool b);
//
//   void writeType(Type *ty);
//   void writeQualType(const QualType &ty);
//
//   void writeArg(const Argument &arg);
//   void writeVariant(const Variant &var);
//   void writeAttribute(const Attribute &attr);
//   void writeGeneric(const TemplateConstraint &gen);
//
//   std::stack<unsigned char> Packs;
//   std::stack<unsigned short> WrittenBitStack;
//
//   void beginPack();
//   void endPack();
//
//   void writeBoolPack(bool b[], unsigned n);
//
//   void writeModuleHeader();
//
//   void visit(AstNode *node) override;
//   void visit(Expression *node) override;
//   void visit(Statement *node) override;
//
//   void visit(NamespaceDecl *node) override;
//   void visit(UsingStmt *node) override;
//   void visit(CompoundStmt *node) override;
//
//   void visit(DeclStmt *node) override;
//   void visit(FunctionDecl *node) override;
//   void visit(DeclareStmt *node) override;
//
//   void visit(UnionDecl *node) override;
//   void visit(ClassDecl *node) override;
//   void visit(MethodDecl *node) override;
//   void visit(FieldDecl *node) override;
//   void visit(ConstrDecl *node) override;
//   void visit(DestrDecl *node) override;
//
//   void visit(EnumDecl *node) override;
//   void visit(EnumCaseDecl *node) override;
//
//   void visit(IdentifierRefExpr *node) override;
//   void visit(SubscriptExpr *node) override;
//   void visit(CallExpr *node) override;
//   void visit(MemberRefExpr *node) override;
//
//   void visit(ForStmt *node) override;
//   void visit(ForInStmt *node) override;
//   void visit(WhileStmt *node) override;
//   void visit(IfStmt *node) override;
//   void visit(MatchStmt *node) override;
//   void visit(CaseStmt *node) override;
//   void visit(LabelStmt *node) override;
//   void visit(GotoStmt *node) override;
//
//   void visit(ReturnStmt *node) override;
//   void visit(BreakStmt *node) override;
//   void visit(ContinueStmt *node) override;
//
//   void visit(CollectionLiteral *node) override;
//   void visit(IntegerLiteral *node) override;
//   void visit(NoneLiteral *node) override;
//   void visit(StringLiteral *node) override;
//   void visit(StringInterpolation *node) override;
//   void visit(TupleLiteral *node) override;
//
//   void visit(UnaryOperator *node) override;
//   void visit(BinaryOperator *node) override;
//   void visit(TertiaryOperator *node) override;
//
//   void visit(FuncArgDecl *node) override;
//   void visit(LambdaExpr *node) override;
//   void visit(ImplicitCastExpr *node) override;
//   void visit(TypedefDecl *node) override;
//   void visit(TypeRef *node) override;
//   void visit(LvalueToRvalue *node) override;
//
//   void visit(EndOfFileStmt *node) override;
//   void visit(DebugStmt *node) override;
//
//   void visit(TryStmt *node) override;
//   void visit(ThrowStmt *node) override;
//};
//
//}
//}
//
//#endif //CDOT_MODULEWRITER_H
