//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_METHOD_H
#define CDOT_METHOD_H

#include "Function.h"

namespace cdot {
namespace il {

class AggregateType;

class Method: public Function {
public:
   friend class Function; // for copy init

   Method(const std::string &name,
          QualType returnType,
          llvm::ArrayRef<Argument *> args,
          AggregateType *forType,
          bool isStatic,
          bool isVirtual,
          bool isProperty,
          bool isOperator,
          bool isConversionOp,
          Module *parent,
          bool mightThrow);

   AggregateType *getRecordType() const;

   bool isStatic() const
   {
      return (SubclassData & Flag::Static) != 0;
   }

   bool isProperty() const
   {
      return (SubclassData & Flag::Property) != 0;
   }

   bool isVirtual() const
   {
      return (SubclassData & Flag::Virtual) != 0;
   }

   bool isProtocolMethod() const;

   Argument *getSelf();

   const llvm::StringRef &getSelfBinding() const
   {
      return selfBinding;
   }

   void setSelfBinding(const llvm::StringRef &selfBinding)
   {
      Method::selfBinding = selfBinding;
   }

   bool isPrimitiveOperator() const
   {
      return (SubclassData & Flag::BoxedOperator) != 0;
   }

   bool isOperator() const
   {
      return (SubclassData & Flag::Operator) != 0;
   }

   size_t getVtableOffset() const
   {
      return vtableOffset;
   }

   void setVtableOffset(size_t vtableOffset)
   {
      Method::vtableOffset = vtableOffset;
   }

   size_t getPtableOffset() const
   {
      return ptableOffset;
   }

   void setPtableOffset(size_t ptableOffset)
   {
      Method::ptableOffset = ptableOffset;
   }

protected:
   AggregateType *recordType;
   Argument *Self;
   llvm::StringRef selfBinding;

   size_t vtableOffset = 0;
   size_t ptableOffset = 0;

   void checkIfPrimitiveOp();

   Method(const Method &other);

public:
   static inline bool classof(Value const* T) {
     switch(T->getTypeID()) {
        case MethodID:
        case InitializerID:
           return true;
        default:
           return false;
     }
   }
};

class Initializer: public Method {
public:
   friend class Function; // for copy init

   Initializer(const std::string &name,
               llvm::ArrayRef<Argument *> args,
               AggregateType *forType,
               Module *parent,
               bool mightThrow);

protected:
   Initializer(const Initializer &other);

public:
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case InitializerID:
            return true;
         default:
            return false;
      }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_METHOD_H
