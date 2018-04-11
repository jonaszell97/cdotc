//
// Created by Jonas Zell on 30.03.18.
//

#ifndef CDOT_TYPEVISITOR_H
#define CDOT_TYPEVISITOR_H

#include "Type.h"

#define DISPATCH(Name)                                  \
  static_cast<SubClass*>(this)->                        \
           visit##Name(static_cast<const Name*>(T))

namespace cdot {

template<class SubClass, class RetTy = void>
class TypeVisitor {
public:
   RetTy visit(const Type *T)
   {
      switch (T->getTypeID()) {
#     define CDOT_TYPE(Name, Parent)                              \
      case Type::Name##ID: return DISPATCH(Name);
#     include "Types.def"
      }
   }

#  define CDOT_TYPE(Name, Parent)                                 \
   RetTy visit##Name(const Name *T) { return DISPATCH(Parent); }
#  include "Types.def"

   RetTy visitType(const Type*) { return RetTy(); }
};

template<class SubClass>
class RecursiveTypeVisitor {
public:
   void visit(const Type *T)
   {
      bool Continue;
      switch (T->getTypeID()) {
#     define CDOT_TYPE(Name, Parent)                              \
      case Type::Name##ID: Continue = DISPATCH(Name); break;
#     include "Types.def"
      }

      if (Continue)
         for (auto SubTy : T->children())
            visit(SubTy);
   }

#  define CDOT_TYPE(Name, Parent)                                 \
   bool visit##Name(const Name *T) { return DISPATCH(Parent); }
#  include "Types.def"

   bool visitType(const Type*) { return true; }
};

} // namespace cdot

#undef DISPATCH

#endif //CDOT_TYPEVISITOR_H
