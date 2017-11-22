//
// Created by Jonas Zell on 01.07.17.
//

#include "LambdaExpr.h"
#include "../../Statement/Declaration/FuncArgDecl.h"
#include "../../Statement/ControlFlow/ReturnStmt.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../TypeRef.h"

namespace cdot {
namespace ast {

LambdaExpr::LambdaExpr(std::shared_ptr<TypeRef> &&return_type,
                       std::vector<std::shared_ptr<FuncArgDecl>> &&args,
                       Statement::SharedPtr &&body)
   : Expression(LambdaExprID), returnType(move(return_type)), args(move(args)),
     body(move(body))
{

}

} // namespace ast
} // namespace cdot