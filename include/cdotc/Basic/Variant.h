#ifndef CDOT_VARIANT_H
#define CDOT_VARIANT_H

#include <cassert>
#include <string>
#include <vector>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/StringRef.h>

namespace cdot {

class QualType;
class Type;

enum class VariantType : unsigned char {
   String = 0,
   Int = 1,
   Floating = 2,
   Void = 3,
   Array = 4,
   MetaType = 5,
   Struct = 6,
};

struct Variant {
   Variant() : kind(VariantType::Void), Data{} {}

   Variant(std::string&& s) : kind(VariantType::String), Data{}
   {
      new (Data.buffer) std::string(move(s));
   }

   Variant(llvm::StringRef s) : kind(VariantType::String), Data{}
   {
      new (Data.buffer) std::string(s.data(), s.size());
   }

   explicit Variant(Type* Ty, bool isConst = false);
   explicit Variant(QualType const& ty);

   explicit Variant(uint64_t l, unsigned numBits = 64, bool isUnsigned = false)
       : kind(VariantType::Int), Data{}
   {
      new (Data.buffer) llvm::APInt(numBits, (uint64_t)l, !isUnsigned);
   }

   Variant(bool b) : kind(VariantType::Int), Data{}
   {
      new (Data.buffer) llvm::APSInt(llvm::APInt(1, (uint64_t)b), true);
   }

   Variant(char c) : kind(VariantType::Int), Data{}
   {
      new (Data.buffer) llvm::APSInt(llvm::APInt(8, (uint64_t)c), true);
   }

   Variant(double d) : kind(VariantType::Floating), Data{}
   {
      new (Data.buffer) llvm::APFloat(d);
   }

   Variant(float f) : kind(VariantType::Floating), Data{}
   {
      new (Data.buffer) llvm::APFloat(f);
   }

   Variant(VariantType kind, std::vector<Variant>&& v) : kind(kind), Data{}
   {
      assert(kind == VariantType::Struct || kind == VariantType::Array);
      new (Data.buffer) std::vector<Variant>(move(v));
   }

   Variant(llvm::APSInt&& Int) : kind(VariantType::Int), Data{}
   {
      new (Data.buffer) llvm::APSInt(std::move(Int));
   }

   Variant(llvm::APInt&& Int) : kind(VariantType::Int), Data{}
   {
      new (Data.buffer) llvm::APInt(std::move(Int));
   }

   Variant(llvm::APFloat&& F) : kind(VariantType::Floating), Data{}
   {
      new (Data.buffer) llvm::APFloat(std::move(F));
   }

   Variant(Variant&& var) noexcept;
   Variant(const Variant& var);
   ~Variant();

   Variant& operator=(const Variant& var);
   Variant& operator=(Variant&& var);

   enum PrintOptions : unsigned char {
      Opt_ExactFP = 1,
      Opt_ShowQuotes = Opt_ExactFP << 1,
      Opt_IntAsPtr = Opt_ShowQuotes << 1,
   };

   std::string toString(unsigned char opts = 0) const;

   bool isVoid() const { return kind == VariantType::Void; }
   bool isArray() const { return kind == VariantType::Array; }
   bool isStr() const { return kind == VariantType::String; }
   bool isInt() const { return kind == VariantType::Int; }
   bool isFloat() const { return kind == VariantType::Floating; }
   bool isMetaType() const { return kind == VariantType::MetaType; }
   bool isStruct() const { return kind == VariantType::Struct; }

   void Profile(llvm::FoldingSetNodeID& ID) const;

   operator bool() const { return !isVoid(); }

   int64_t getSExtValue() const
   {
      assert(isInt());
      return getAPSInt().getSExtValue();
   }

   uint64_t getZExtValue() const
   {
      assert(isInt());
      return getAPSInt().getZExtValue();
   }

   llvm::APSInt const& getAPSInt() const
   {
      return *reinterpret_cast<llvm::APSInt const*>(Data.buffer);
   }

   unsigned getBitwidth() const
   {
      assert(isInt());
      return getAPSInt().getBitWidth();
   }

   bool isUnsigned() const
   {
      assert(isInt());
      return getAPSInt().isUnsigned();
   }

   double getDouble() const
   {
      assert(isFloat());
      return getAPFloat().convertToDouble();
   }

   float getFloat() const
   {
      assert(isFloat());
      return getAPFloat().convertToFloat();
   }

   llvm::APFloat const& getAPFloat() const
   {
      return *reinterpret_cast<llvm::APFloat const*>(Data.buffer);
   }

   char getChar() const
   {
      assert(isInt());
      return (char)getSExtValue();
   }

   QualType getMetaType() const;

   const std::string& getString() const
   {
      assert(isStr());
      return *reinterpret_cast<std::string const*>(Data.buffer);
      ;
   }

   bool isString(llvm::StringRef str) const
   {
      if (!isStr())
         return false;

      return str.equals(getString());
   }

   const std::vector<Variant>& getVec() const
   {
      assert(isArray() || isStruct());
      return *reinterpret_cast<std::vector<Variant> const*>(Data.buffer);
   }

   VariantType getKind() const { return kind; }

   size_t size() const
   {
      assert(isStruct() || isArray());
      return getVec().size();
   }

   void push(Variant&& V) { getVecRef().push_back(std::move(V)); }

   Variant pop()
   {
      auto V = std::move(getVecRef().back());
      getVecRef().pop_back();

      return V;
   }

   Variant& operator[](unsigned index)
   {
      assert((isArray() || isStruct()) && "cannot index variant");
      return getVecRef()[index];
   }

   Variant const& operator[](unsigned index) const
   {
      assert((isArray() || isStruct()) && "cannot index variant");
      return getVec()[index];
   }

   using VecTy = std::vector<Variant>;

   // no QualType here because we want to avoid a dependency on Type.h
   // a static assertion will trigger if the size is not big enough (which
   // should never be the case)
   using DataType = llvm::AlignedCharArrayUnion<llvm::APSInt, llvm::APFloat,
                                                std::string, VecTy>;

   Variant applyBinaryOp(const Variant& rhs, const std::string& op) const;
   Variant applyUnaryOp(const std::string& op) const;

   std::vector<Variant>::iterator begin()
   {
      assert((isArray() || isStruct()) && "not an array");
      return getVecRef().begin();
   }

   std::vector<Variant>::iterator end()
   {
      assert((isArray() || isStruct()) && "not an array");
      return getVecRef().end();
   }

   std::vector<Variant>::const_iterator begin() const
   {
      assert((isArray() || isStruct()) && "not an array");
      return getVec().begin();
   }

   std::vector<Variant>::const_iterator end() const
   {
      assert((isArray() || isStruct()) && "not an array");
      return getVec().end();
   }

   const std::vector<Variant>& getFields() const { return getVec(); }

private:
   VariantType kind : 8;

   union {
      DataType Data;
      std::string _str;
      llvm::APSInt _int;
      std::vector<Variant> _vec;
   };

   void copyFrom(Variant&& V);
   void copyFrom(Variant const& V);

   void destroyValue();

   std::vector<Variant>& getVecRef()
   {
      assert(isArray() || isStruct());
      return *reinterpret_cast<std::vector<Variant>*>(Data.buffer);
   }

   std::string& getStringRef()
   {
      assert(isStr());
      return *reinterpret_cast<std::string*>(Data.buffer);
      ;
   }

   llvm::APSInt& getAPSIntRef()
   {
      return *reinterpret_cast<llvm::APSInt*>(Data.buffer);
   }

   llvm::APFloat& getAPFloatRef()
   {
      return *reinterpret_cast<llvm::APFloat*>(Data.buffer);
   }
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& out, const Variant& V);

} // namespace cdot

#endif // CDOT_VARIANT_H
