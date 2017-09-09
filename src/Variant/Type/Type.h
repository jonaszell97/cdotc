//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_TYPE_H
#define CDOT_TYPE_H


#include <llvm/IR/IRBuilder.h>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::pair;

class Expression;
class TypeCheckPass;
class CodeGen;

namespace cdot {

   class Variant;
   class PointerType;
   class ObjectType;

   namespace cl {
      struct EnumCase;
   }

   enum class TypeID {
      AutoTypeID,
      VoidTypeID,
      PrimitiveTypeID,
      FunctionTypeID,
      PointerTypeID,
      ObjectTypeID,
      EnumTypeID,
      CollectionTypeID,
      IntegerTypeID,
      FPTypeID,
      TupleTypeID
   };

   using cdot::cl::EnumCase;

   class Type {
   public:

      static llvm::IRBuilder<>* Builder;
      static bool classof(Type const* T) { return true; }
      static bool GenericTypesCompatible(Type* given, Type* needed);

      static void CopyProperties(Type *src, Type *dst);
      static void resolve(
         Type **ty,
         string& className,
         std::vector<ObjectType*>* generics,
         std::vector<string>& namespaces
      );

      typedef std::unique_ptr<Type> UniquePtr;
      typedef std::shared_ptr<Type> SharedPtr;

      virtual ~Type() = default;

      virtual llvm::Type* getLlvmType() {
         auto ty = _getLlvmType();
         if (lvalue) {
            return ty->getPointerTo();
         }

         return ty;
      }

      bool isNull() {
         return isNull_;
      }

      void isNull(bool null_) {
         isNull_ = null_;
      }

      bool isNullable() {
         return isNullable_;
      }

      void isNullable(bool nullable) {
         isNullable_ = nullable;
      }

      bool isConst() {
         return isConst_;
      }

      void isConst(bool const_) {
         isConst_ = const_;
      }

      virtual bool isLvalue() {
         return lvalue;
      }

      virtual void isLvalue(bool lval) {
         lvalue = lval;
      }

      virtual bool needsLvalueToRvalueConv() {
         return lvalue && !isStruct() && !cstyleArray && !carrayElement ;
      }

      bool isInferred() {
         return isInferred_;
      }

      bool isVararg() {
         return vararg;
      }

      void isVararg(bool va) {
         vararg = va;
      }

      bool isCStyleVararg() {
         return cstyleVararg;
      }

      void isCStyleVararg(bool va) {
         cstyleVararg = va;
         vararg = va;
      }

      virtual bool hasSelfRequirement() {
         return false;
      }

      virtual void hasSelfRequirement(bool selfReq) {

      }

      virtual unordered_map<string, Type*>& getConcreteGenericTypes() {
         assert(false && "Call isObject first");
      }

      virtual string& getClassName() {
         return className;
      }

      virtual bool isGeneric() {
         return false;
      }

      virtual bool isBoxedPrimitive();

      virtual void isGeneric(bool gen) {}

      virtual Type* getCovariance() {
         return nullptr;
      }

      virtual Type* getContravariance() {
         return nullptr;
      }

      virtual void setCovariance(Type* cov) {}
      virtual void setContravariance(Type* con) {}

      virtual string& getGenericClassName() {
         return className;
      }

      virtual Type*& getPointeeType() {
         llvm_unreachable("Call isPointerTy() first");
      }

      virtual void setGenericClassName(string name) {}

      virtual bool isPointerToStruct() {
         return false;
      }

      virtual PointerType* getPointerTo();

      virtual Type& operator=(const Type& other) = default;
      virtual std::vector<Type*> getContainedTypes(bool includeSelf = false);
      virtual std::vector<Type**> getTypeReferences();

      static void resolveGeneric(Type**, unordered_map<string, Type*>);
      static void resolveUnqualified(Type*);
      static unordered_map<string, Type*> resolveUnqualified(std::vector<Type*>&, std::vector<ObjectType*>&);

      virtual bool operator==(Type*& other);
      virtual bool operator!=(Type*& other) {
         return !operator==(other);
      }

      string toString() {
         if (lvalue) {
            return "ref " + _toString();
         }

         return _toString();
      }

      virtual bool implicitlyCastableTo(Type*) {
         return false;
      }

      virtual bool explicitlyCastableTo(Type*) {
         return false;
      }

      virtual Type* unbox() {
         return this;
      }

      virtual Type* toRvalue() {
         lvalue = false;
         return this;
      }

      virtual bool hasDefaultArgVal() {
         return hasDefaultArg;
      }

      virtual void hasDefaultArgVal(bool defVal) {
         hasDefaultArg = defVal;
      }

      virtual bool hasDefaultValue() {
         return false;
      }

      virtual Type* deepCopy();

      virtual llvm::Value* getDefaultVal() {
         return nullptr;
      }

      virtual llvm::Constant* getConstantVal(Variant&) {
         llvm_unreachable("Can't emit constant val for type");
      }

      virtual short getAlignment();
      virtual size_t getSize() {
         return getAlignment();
      }

      virtual llvm::Value* castTo(llvm::Value*, Type*) {
         llvm_unreachable("can't be casted");
      }

      virtual bool isUnsafePointer() {
         return false;
      }

      virtual TypeID getTypeID() const {
         return id;
      }

      virtual bool isBoxedEquivOf(Type*&) {
         return false;
      }

      virtual bool isCStyleArray() {
         return cstyleArray;
      }

      virtual void isCStyleArray(bool cstyle) {
         cstyleArray = cstyle;
      }

      virtual void setLengthExpr(std::shared_ptr<Expression> expr) {
         lengthExpr = expr;
      }

      std::shared_ptr<Expression>& getLengthExpr() {
         return lengthExpr;
      }

      virtual void setLength(long len) {
         length = len;
      }

      virtual long getLength() {
         return length;
      }

      virtual bool isPointerTy() {
         return false;
      }

      virtual bool isIntegerTy() {
         return false;
      }

      virtual bool isUnsigned() {
         return false;
      }

      virtual bool isInt64Ty(bool isUnsigned = false) {
         return false;
      }

      virtual bool isInt8Ty(bool isUnsigned = false) {
         return false;
      }

      virtual bool isInt1Ty(bool isUnsigned = false) {
         return false;
      }

      virtual bool isIntNTy(unsigned n, bool isUnsigned = false) {
         return false;
      }

      virtual bool isFloatTy() {
         return false;
      }

      virtual bool isDoubleTy() {
         return false;
      }

      virtual bool isFPType() {
         return false;
      }

      virtual bool isObject() {
         return false;
      }

      virtual bool isProtocol() {
         return false;
      }

      virtual bool isStruct() {
         return false;
      }

      virtual bool isEnum() {
         return false;
      }

      virtual void isEnum(bool) {

      }

      virtual void setKnownEnumCase(EnumCase *eCase, std::vector<pair<string, std::shared_ptr<Expression>>>
         associatedTypes = {}, std::vector<Type*> argTypes = {}) {

      }

      virtual EnumCase* getKnownEnumCase() {
         llvm_unreachable("Call hasKnownEnumCase first!");
      }

      virtual std::vector<pair<string, std::shared_ptr<Expression>>>& getAssociatedTypes() {
         llvm_unreachable("Call hasKnownEnumCase first!");
      }

      virtual std::vector<Type*>& getKnownEnumCaseTypes() {
         llvm_unreachable("Call hasKnownEnumCase first!");
      }

      virtual bool hasKnownEnumCase() {
         return false;
      }

      virtual bool isOptionTy() {
         return false;
      }

      virtual bool isOptionOf(string&) {
         return false;
      }

      virtual bool isFunctionTy() {
         return false;
      }

      virtual bool isTupleTy() {
         return false;
      }

      virtual void isCarrayElement(bool el) {
         carrayElement = el;
      }

      virtual bool isCarrayElement() {
         return carrayElement;
      }

      virtual void hasConstantSize(bool constSize) {
         constantSize = constSize;
      }

      virtual bool hasConstantSize() {
         return constantSize;
      }

      virtual bool isRefcounted() {
         return false;
      }

      virtual void visitContained(TypeCheckPass& t) {}
      virtual Type* visitLengthExpr(TypeCheckPass* v);
      virtual llvm::Value* visitLengthExpr(CodeGen* v);

      Type* getLengthExprType() {
         return lengthExprType;
      }

      llvm::Value* getLengthExprVal() {
         return lengthExprValue;
      }

   protected:
      TypeID id;

      bool isNull_ = false;
      bool isNullable_ = false;
      bool isInferred_ = false;
      bool isConst_ = false;
      bool lvalue = false;

      string className = "";

      bool hasDefaultArg = false;

      bool vararg = false;
      bool cstyleVararg = false;
      bool cstyleArray = false;
      bool carrayElement = false;

      std::shared_ptr<Expression> lengthExpr = nullptr;
      Type* lengthExprType = nullptr;
      llvm::Value* lengthExprValue = nullptr;
      bool constantSize = false;
      long length = -1;

      virtual llvm::Type* _getLlvmType() = 0;
      virtual llvm::Type* getLlvmFunctionType() {
         llvm_unreachable("Not a function type!");
      }
      virtual string _toString() = 0;
   };

   template <class T>
   bool isa(const Type* t) {
      return t == nullptr ? false : T::classof(t);
   }

   template<class T>
   T* cast(Type* t) {
      assert(T::classof(t) && "Check isa<> before casting");
      return static_cast<T*>(t);
   }

} // namespace cdot


#endif //CDOT_TYPE_H
