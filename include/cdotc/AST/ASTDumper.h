#ifndef CDOT_ASTDUMPER_H
#define CDOT_ASTDUMPER_H

namespace llvm {
class raw_ostream;
} // namespace llvm

namespace cdot {

namespace ast {
class Expression;
class Statement;
} // namespace ast

class ASTDumper {
public:
   enum Options : unsigned {
      None = 0,
   };

   ASTDumper(llvm::raw_ostream& out, Options opts = None);
   ~ASTDumper();

   void print(ast::Expression* expr);
   void print(ast::Statement* stmt);

private:
   void* pImpl;
};

} // namespace cdot

#endif // CDOT_ASTDUMPER_H
