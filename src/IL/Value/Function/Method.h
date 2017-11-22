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
   Method(cl::Method *m, Module *parent);
   Method(const std::string &name,
          QualType returnType,
          llvm::ArrayRef<cdot::Argument> args,
          AggregateType *forType,
          bool isStatic,
          Module *parent,
          SourceLocation loc,
          bool mightThrow);

   Method(const std::string &name,
          FunctionType *ty,
          QualType returnType,
          llvm::ArrayRef<Argument*> args,
          AggregateType *forType,
          bool isStatic,
          Module *parent,
          SourceLocation loc,
          bool mightThrow);

   AggregateType *getRecordType() const;
   bool isStatic() const;
   bool isProperty() const;

   Argument *getSelf() const;

   const llvm::StringRef &getSelfBinding() const
   {
      return selfBinding;
   }

   void setSelfBinding(const llvm::StringRef &selfBinding)
   {
      Method::selfBinding = selfBinding;
   }

protected:
   AggregateType *recordType;
   Argument *Self;
   llvm::StringRef selfBinding;

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
   Initializer(cl::Method *m, Module *parent);
   Initializer(const std::string &name,
               llvm::ArrayRef<cdot::Argument> args,
               AggregateType *forType,
               Module *parent,
               SourceLocation loc,
               bool mightThrow);

   Initializer(const std::string &name,
               FunctionType *ty,
               QualType returnType,
               llvm::ArrayRef<Argument*> args,
               AggregateType *forType,
               Module *parent,
               SourceLocation loc,
               bool mightThrow);

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
