//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FUNCTIONTYPE_H
#define CDOT_FUNCTIONTYPE_H


#include "Type.h"

class Function;
class TypeRef;
using std::pair;

namespace cdot {

   class ObjectType;

   class FunctionType : public Type {
   public:
      FunctionType(std::shared_ptr<TypeRef>, std::vector<pair<string, std::shared_ptr<TypeRef>>>&&,
         std::vector<ObjectType*>&&);
      FunctionType(std::shared_ptr<TypeRef>, std::vector<pair<string, std::shared_ptr<TypeRef>>>&);

      FunctionType(Type*, std::vector<Type*>&&, std::vector<ObjectType*>&&);
      FunctionType(Type*, std::vector<Type*>&, std::vector<ObjectType*>&);
      FunctionType(Type*, std::vector<Type*>&);

      ~FunctionType() override;

      inline bool isLambda() {
         return isLambda_;
      }

      inline Type* getReturnType() {
         return returnType;
      }

      inline std::vector<Type*>& getArgTypes() {
         return argTypes;
      }

      inline std::vector<ObjectType*> getGenericTypes() {
         return genericTypes;
      }

      inline bool isGeneric() override {
         if (returnType->isGeneric()) {
            return true;
         }

         for (const auto& arg : argTypes) {
            if (arg->isGeneric()) {
               return true;
            }
         }

         return false;
      }

      Function* getFunction() {
         return declaredFunc;
      }

      void setFunction(Function* func) {
         declaredFunc = func;
      }

      bool isFunctionTy() override {
         return true;
      }

      bool isStruct() override {
         return true;
      }

      void visitContained(TypeCheckVisitor& v) override;

      bool operator==(Type*& other) override;
      inline bool operator!=(Type*& other) override {
         return !operator==(other);
      }

      std::vector<Type*> getContainedTypes(bool includeSelf = false) override;
      std::vector<Type**> getTypeReferences() override;

      string& getClassName() override {
         return className;
      }

      string toString() override;
      llvm::Type* _getLlvmType() override;

      bool implicitlyCastableTo(Type*) override;

      Type* deepCopy() override;

      static inline bool classof(FunctionType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::FunctionTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<FunctionType> UniquePtr;
      typedef std::shared_ptr<FunctionType> SharedPtr;

   protected:
      Type* returnType;
      std::vector<Type*> argTypes;
      std::vector<ObjectType*> genericTypes;
      Function* declaredFunc;

      std::shared_ptr<TypeRef> rawReturnType;
      std::vector<pair<string, std::shared_ptr<TypeRef>>> rawArgTypes;

      bool isLambda_;
   };

} // namespace cdot

#endif //CDOT_FUNCTIONTYPE_H
