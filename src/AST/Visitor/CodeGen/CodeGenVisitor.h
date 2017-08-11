//
// Created by Jonas Zell on 11.07.17.
//

#ifndef CDOT_CODEGENVISITOR_H
#define CDOT_CODEGENVISITOR_H


#include <llvm/IR/IRBuilder.h>
#include <unordered_map>
#include "../Visitor.h"
#include "../../../Variant/Variant.h"

class FuncArgDecl;
class CompoundStmt;
class FieldDecl;
class Statement;

using std::string;
using std::pair;
using std::unordered_map;
using std::unique_ptr;

namespace cdot {
namespace cl {
    class Class;
}
}

class CodeGenVisitor {
public:
    CodeGenVisitor();

    virtual CGValue visit(NamespaceDecl*);
    virtual CGValue visit(ImportStmt*);
    virtual CGValue visit(ExportStmt*);

    virtual CGValue visit(FunctionDecl*);
    virtual CGValue visit(CompoundStmt*);
    virtual CGValue visit(IdentifierRefExpr*);
    virtual CGValue visit(DeclStmt*);
    virtual CGValue visit(ForStmt*);
    virtual CGValue visit(WhileStmt*);

    virtual CGValue visit(ArrayLiteral*);
    virtual CGValue visit(LiteralExpr*);
    virtual CGValue visit(StringLiteral*);
    virtual CGValue visit(ArrayAccessExpr*);
    virtual CGValue visit(CallExpr*);
    virtual CGValue visit(MemberRefExpr*);
    virtual CGValue visit(BinaryOperator*);
    virtual CGValue visit(ExplicitCastExpr*);
    virtual CGValue visit(TertiaryOperator*);
    virtual CGValue visit(UnaryOperator*);
    virtual CGValue visit(BreakStmt*);
    virtual CGValue visit(ContinueStmt*);
    virtual CGValue visit(IfStmt*);
    virtual CGValue visit(SwitchStmt*);
    virtual CGValue visit(CaseStmt*);
    virtual CGValue visit(LabelStmt*);
    virtual CGValue visit(GotoStmt*);
    virtual CGValue visit(FuncArgDecl*);
    virtual CGValue visit(ReturnStmt*);
    virtual CGValue visit(Expression*);
    virtual CGValue visit(ClassDecl*);
    virtual CGValue visit(MethodDecl*);
    virtual CGValue visit(FieldDecl*);
    virtual CGValue visit(ConstrDecl*);
    virtual CGValue visit(LambdaExpr*);
    virtual CGValue visit(OperatorDecl*);
    virtual CGValue visit(InterfaceDecl*);
    virtual CGValue visit(ImplicitCastExpr*);
    virtual CGValue visit(ExtendStmt*);
    virtual CGValue visit(TypedefDecl*);
    virtual CGValue visit(TypeRef*);

    void finalize();

    friend class CGType;
    friend class CGMemory;
    friend class CGCast;
    friend class TypeCheckVisitor;
    friend class InternalClass;
    friend class CGInternal;
    friend class cdot::cl::Class;

protected:
    static llvm::LLVMContext Context;
    static llvm::IRBuilder<> Builder;
    static unique_ptr<llvm::Module> Module;
    static unordered_map<string, CGValue> ConstValues;
    static unordered_map<string, CGValue> MutableValues;
    static unordered_map<string, llvm::Constant*> Functions;
    static unordered_map<string, llvm::FunctionType*> FunctionTypes;
    static std::vector<pair<llvm::GlobalVariable*, Expression*>> global_initializers;
    static llvm::Function* global_init;

    // function stack
    std::vector<llvm::Function*> functions = {};

    // value stack
    std::vector<CGValue*> latest_val = {};
    std::vector<llvm::BasicBlock*> break_targets = {};
    std::vector<llvm::BasicBlock*> continue_targets = {};
    unordered_map<string, llvm::BasicBlock*> labels = {};

    // function captures
    unordered_map<string, std::vector<pair<string,string>>> function_captures = {};

    bool broken = false;

    // identifier expressions
    CGValue ReturnMemberRef(std::shared_ptr<Expression>, CGValue, int = 8);

    // functions
    unordered_map<unsigned int, llvm::Type*> LambdaTypes = {};
    unordered_map<string, int> LambdaEnvOrder = {};
    llvm::Function* DeclareFunction(string &bound_name, std::vector<std::shared_ptr<FuncArgDecl>> args,
        TypeSpecifier &return_type, bool set_this_arg = false, llvm::StructType *
    this_val = nullptr, string this_binding = "");
    llvm::Function* DeclareFunction(string &bound_name, std::vector<std::shared_ptr<FuncArgDecl>> args,
        llvm::Type *return_type, bool set_this_arg = false, llvm::StructType *
    this_val = nullptr, string this_binding = "");

    llvm::Function* DeclareMethod(string &bound_name, std::vector<std::shared_ptr<FuncArgDecl>> args,
        TypeSpecifier &return_type, llvm::StructType *this_arg, string &this_binding);

    llvm::Function* DefineFunction(string& bound_name, std::shared_ptr<Statement> body);

    llvm::Function* CreateDefaultConstructor(string& bound_name, std::vector<std::shared_ptr<FuncArgDecl>> args,
        TypeSpecifier& return_type, llvm::StructType* this_arg, string& this_binding,
        cdot::cl::Class*, string struct_name, std::vector<pair<int, llvm::Value*>>& vtables);

    // classes
    unordered_map<string, unordered_map<string, int>> struct_field_info = {};
    llvm::Value* AccessField(string ,string, llvm::Value*);
    llvm::Value* AccessField(int, llvm::Value*);
    void SetField(int, llvm::Value*, llvm::Value*);
    void SetField(string, string, llvm::Value*, llvm::Value*);

    unordered_map<string, unordered_map<string, int>> vtable_order = {};
    llvm::GlobalVariable* GenerateVTable(ClassDecl*);

    // utility
    llvm::ConstantInt* ONE;
    llvm::ConstantInt* ZERO;
    llvm::ConstantInt* ONE_64;
    llvm::ConstantInt* ZERO_64;

    // reusable values
    CGValue GetString(string&, bool = false, bool = false);
    unordered_map<string, CGValue> Strings = {};

    // imported internal classes
    std::vector<string> imported = {};
};


#endif //CDOT_CODEGENVISITOR_H
