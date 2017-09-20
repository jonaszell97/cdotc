//
// Created by Jonas Zell on 19.06.17.
//

#include "NumericLiteral.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/VoidType.h"

NumericLiteral::NumericLiteral(cdot::Variant v) : value(v) {
   switch (v.type) {
      case VariantType::INT:
         switch (v.bitwidth) {
            case 1:
               isBool = true; break;
            case 8:
               isChar = true; break;
            default:
               break;
         }

         type = IntegerType::get(v.bitwidth, v.isUnsigned);
         break;
      case VariantType::FLOAT:
         type = new FPType(v.bitwidth);
         break;
      case VariantType::VOID:
         type = new VoidType;
         break;
      default:
         assert(false && "Unknown literal type!");

   }
}

NumericLiteral::~NumericLiteral() {
   delete type;
}

bool NumericLiteral::canReturn(Type *ty)
{
   if (ty->isBoxedPrimitive()) {
      ty = ty->unbox();
   }

   switch (value.type) {
      case VariantType::INT:
         return isa<PrimitiveType>(ty);
      case VariantType::FLOAT: {
         return ty->isFPType();
      }
      default:
         return false;
   }
}

std::vector<AstNode::SharedPtr> NumericLiteral::get_children() {
   return memberExpr == nullptr ? std::vector<AstNode::SharedPtr>() : std::vector<AstNode::SharedPtr>{memberExpr };
}

void NumericLiteral::__dump(int depth) {
   for (int i = 0; i < depth; i++) {
      std::cout << "\t";
   }
   std::cout << className << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}