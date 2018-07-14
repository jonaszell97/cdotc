//
// Created by Jonas Zell on 03.01.18.
//

#ifndef CDOT_VARIOUS_H
#define CDOT_VARIOUS_H

namespace cdot {
namespace support {

template<class BaseTy, class ExpTy>
BaseTy pow(BaseTy b, ExpTy exp)
{
   BaseTy result = 1;
   while (exp)
   {
      if (exp & 1)
         result *= b;
      exp >>= 1;
      b *= b;
   }

   return result;
}

template <class HashTy, class T>
inline void hash_combine(HashTy& seed, const T& v)
{
   std::hash<T> hasher;
   seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

} // namespace support
} // namespace cdot

#endif //CDOT_VARIOUS_H
