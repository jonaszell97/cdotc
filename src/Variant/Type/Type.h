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

class Variant;

namespace cdot {

    class PointerType;

    enum class TypeID {
        AutoTypeID,
        VoidTypeID,
        PrimitiveTypeID,
        FunctionTypeID,
        PointerTypeID,
        ObjectTypeID,
        GenericTypeID,
        CollectionTypeID,
        IntegerTypeID,
        FPTypeID
    };

    class Type {
    public:

        virtual ~Type() = default;

        virtual llvm::Type* getLlvmType() = 0;
        virtual llvm::Type* getAllocaType() {
            return getLlvmType();
        }

        inline bool isReference() {
            return isReference_;
        }

        inline void isReference(bool ref) {
            isReference_ = ref;
            lvalue = ref;
        }

        inline bool isNull() {
            return isNull_;
        }

        inline void isNull(bool null_) {
            isNull_ = null_;
        }

        inline bool isNullable() {
            return isNullable_;
        }

        inline void isNullable(bool nullable) {
            isNullable_ = nullable;
        }

        inline bool isConst() {
            return isConst_;
        }

        inline void isConst(bool const_) {
            isConst_ = const_;
        }

        virtual inline bool isLvalue() {
            return lvalue;
        }

        virtual inline void isLvalue(bool lval) {
            lvalue = lval;
        }

        virtual inline bool isCStyleArray() {
            return false;
        }

        virtual inline void isCStyleArray(bool) {
            assert(false && "Should only be called on pointer types");
        }

        inline bool isInferred() {
            return isInferred_;
        }

        inline bool isVararg() {
            return vararg;
        }

        inline void isVararg(bool va) {
            vararg = va;
        }

        inline bool isCStyleVararg() {
            return cstyleVararg;
        }

        inline void isCStyleVararg(bool va) {
            cstyleVararg = va;
            vararg = va;
        }

        inline virtual bool isObject() {
            return false;
        }

        inline virtual bool isStruct() {
            return false;
        }

        inline virtual unordered_map<string, Type*>& getConcreteGenericTypes() {
            assert(false && "Call isObject first");
        }

        inline virtual string& getClassName() {
            return className;
        }

        virtual inline bool isGeneric() {
            return false;
        }

        virtual PointerType* getPointerTo();

        virtual Type& operator=(const Type& other) = default;
        virtual std::vector<Type*> getContainedTypes(bool includeSelf = false);
        virtual std::vector<Type**> getTypeReferences();

        static void resolveGeneric(Type**, unordered_map<string, Type*>);
        static void resolveUnqualified(Type*);

        virtual bool operator==(Type*& other);
        virtual inline bool operator!=(Type*& other) {
            return !operator==(other);
        }

        virtual string toString();

        virtual bool implicitlyCastableTo(Type*) {
            return false;
        }

        virtual bool explicitlyCastableTo(Type*) {
            return false;
        }

        virtual inline Type* toRvalue() {
            return this;
        }

        virtual inline bool hasDefaultArgVal() {
            return hasDefaultArg;
        }

        virtual inline void hasDefaultArgVal(bool defVal) {
            hasDefaultArg = defVal;
        }

        virtual inline bool hasDefaultValue() {
            return false;
        }

        virtual Type* deepCopy();

        virtual llvm::Value* getDefaultVal() {
            return nullptr;
        }

        virtual llvm::Constant* getConstantVal(Variant&) {
            assert(false && "Can't emit constant val for type");
        }

        virtual short getAlignment();

        virtual llvm::Value* castTo(llvm::Value*, Type*) {
            assert(false && "can't be casted");
        }

        virtual inline bool isUnsafePointer() {
            return false;
        }

        virtual inline TypeID getTypeID() const {
            return id;
        }

        virtual bool isBoxedEquivOf(Type*&) {
            return false;
        }

        static llvm::IRBuilder<>* Builder;

        static inline bool classof(Type const* T) { return true; }

    protected:

        TypeID id;
        bool isReference_ = false;
        bool isNull_ = false;
        bool isNullable_ = false;
        bool isInferred_ = false;
        bool isConst_ = false;
        bool lvalue = false;

        string className = "";

        bool hasDefaultArg = false;

        bool vararg = false;
        bool cstyleVararg = false;
    };

    template <class T>
    bool isa(const Type* t) {
        return T::classof(t);
    }

    template<class T>
    T* cast(Type* t) {
        assert(T::classof(t) && "Check isa<> before casting");
        return static_cast<T*>(t);
    }

} // namespace cdot


#endif //CDOT_TYPE_H
