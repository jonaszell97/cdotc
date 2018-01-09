//
// Created by Jonas Zell on 05.12.17.
//

#ifndef CDOT_TEMPLATEINSTANTIATOR_H
#define CDOT_TEMPLATEINSTANTIATOR_H

#include <vector>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

struct SourceLocation;

namespace cl {
class Record;
struct Method;
} // namespace cl

namespace ast {

class Function;
class DeclPass;
class Statement;
class StaticExpr;
class SemaPass;

} // namespace ast

class TemplateArg;

namespace sema {
   class TemplateArgList;
} // namespace sema

struct Alias;

class TemplateInstantiator {
public:
   static cl::Record *InstantiateRecord(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 cl::Record *rec,
                                 sema::TemplateArgList&& templateArgs,
                                 bool *isNew = nullptr);

   static ast::Function *InstantiateFunction(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 ast::Function *F,
                                 sema::TemplateArgList&& templateArgs,
                                 bool *isNew = nullptr);

   static cl::Method *InstantiateMethod(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 cl::Method *M,
                                 sema::TemplateArgList &&templateArgs,
                                 bool *isNew = nullptr,
                                 cl::Record *R = nullptr);

   static cl::Method *InstantiateProtocolDefaultImpl(
                                       ast::SemaPass &SP,
                                       const SourceLocation &instantiatedFrom,
                                       cl::Record *Rec,
                                       cl::Method const* M);

   static std::shared_ptr<ast::Statement> InstantiateStatement(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 const std::shared_ptr<ast::Statement> &stmt,
                                 sema::TemplateArgList const& templateArgs);

   static std::shared_ptr<ast::Statement> InstantiateMethodBody(
                                    ast::SemaPass &SP,
                                    const SourceLocation &instantiatedFrom,
                                    cl::Method const* baseMethod,
                                    cl::Method* newMethod);

   static std::shared_ptr<ast::StaticExpr> InstantiateStaticExpr(
                                 ast::SemaPass &SP,
                                 const SourceLocation &instantiatedFrom,
                                 const std::shared_ptr<ast::StaticExpr> &stmt,
                                 sema::TemplateArgList const& templateArgs);

   static std::shared_ptr<ast::StaticExpr> InstantiateAlias(
                                 ast::SemaPass &SP,
                                 Alias *alias,
                                 const SourceLocation &instantiatedFrom,
                                 const std::shared_ptr<ast::StaticExpr> &stmt,
                                 sema::TemplateArgList const& templateArgs);
};

} // namespace cdot

#endif //CDOT_TEMPLATEINSTANTIATOR_H
