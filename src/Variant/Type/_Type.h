//
// Created by Jonas Zell on 30.09.17.
//

#ifndef CDOT_TYPE_H
#define CDOT_TYPE_H

class CanonicalType;

struct Type {
   bool is_lvalue = false;
   bool is_const = false;

   CanonicalType *canonicalTy;

   CanonicalType *operator->() {
      return canonicalTy;
   }

   CanonicalType &operator*() {
      return *canonicalTy;
   }
};


#endif //CDOT_TYPE_H
