//
// Created by Jonas Zell on 01.02.18.
//

#ifndef TABLEGEN_RECORD_H
#define TABLEGEN_RECORD_H

#include "lex/SourceLocation.h"

#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/StringMap.h>
#include <vector>

namespace llvm {
   template<class T>
   class SmallVectorImpl;

   class raw_ostream;
} // namespace llvm

namespace cdot {
namespace tblgen {

class TableGen;
class Type;
class Value;
class RecordKeeper;
class Class;
class Record;

class RecordField {
public:
   RecordField(llvm::StringRef name,
               Type *type,
               Value *defaultValue,
               SourceLocation declLoc,
               size_t associatedTemplateParm = size_t(-1))
      : name(name), type(type), defaultValue(defaultValue),
        declLoc(declLoc), associatedTemplateParm(associatedTemplateParm)
   {}

   llvm::StringRef getName() const
   {
      return name;
   }

   Type *getType() const
   {
      return type;
   }

   Value *getDefaultValue() const
   {
      return defaultValue;
   }

   SourceLocation getDeclLoc() const
   {
      return declLoc;
   }

   bool hasAssociatedTemplateParm() const
   {
      return associatedTemplateParm != -1;
   }

   size_t getAssociatedTemplateParm() const
   {
      assert(hasAssociatedTemplateParm());
      return associatedTemplateParm;
   }

   friend class Class;
   friend class Record;

private:
   llvm::StringRef name;
   Type *type;
   Value *defaultValue;
   SourceLocation declLoc;

   size_t associatedTemplateParm;
};

class Class {
public:
   class BaseClass {
   public:
      BaseClass(Class *Base, std::vector<Value*> &&templateParams)
         : Base(Base), templateArgs(move(templateParams))
      { }

      Class *getBase() const
      {
         return Base;
      }

      const std::vector<Value*> &getTemplateArgs() const
      {
         return templateArgs;
      }

   private:
      Class *Base;
      std::vector<Value*> templateArgs;
   };

   bool addField(llvm::StringRef name,
                 Type *type,
                 Value *defaultValue,
                 SourceLocation declLoc,
                 size_t associatedTemplateParm = size_t(-1)) {
      if (getField(name))
         return false;

      fields.emplace_back(name, type, defaultValue, declLoc,
                          associatedTemplateParm);

      return true;
   }

   bool addTemplateParam(llvm::StringRef name,
                         Type *type,
                         Value *defaultValue,
                         SourceLocation declLoc) {
      if (getTemplateParameter(name))
         return false;

      parameters.emplace_back(name, type, defaultValue, declLoc);

      return true;
   }

   void addBase(Class *Base, std::vector<Value*> &&templateParams)
   {
      bases.emplace_back(Base, move(templateParams));
   }

   llvm::StringRef getName() const
   {
      return name;
   }

   SourceLocation getDeclLoc() const
   {
      return declLoc;
   }

   RecordField *getTemplateParameter(llvm::StringRef name) const
   {
      for (auto &field : parameters)
         if (field.getName() == name)
            return const_cast<RecordField*>(&field);

      return nullptr;
   }

   RecordField* getField(llvm::StringRef name) const
   {
      auto F = getOwnField(name);
      if (F)
         return F;

      for (auto &b : bases)
         if ((F = b.getBase()->getField(name)))
            return F;

      return nullptr;
   }

   RecordField* getOwnField(llvm::StringRef name) const
   {
      for (auto &field : fields)
         if (field.getName() == name)
            return const_cast<RecordField*>(&field);

      return nullptr;
   }

   const std::vector<RecordField> &getParameters() const
   {
      return parameters;
   }

   const std::vector<RecordField> &getFields() const
   {
      return fields;
   }

   const std::vector<BaseClass> &getBases() const
   {
      return bases;
   }

   bool inheritsFrom(Class *C)
   {
      for (auto &B : bases) {
         if (B.getBase() == C)
            return true;

         if (B.getBase()->inheritsFrom(C))
            return true;
      }

      return false;
   }

   void dump();
   void printTo(llvm::raw_ostream &out);

   friend class RecordKeeper;

private:
   Class(RecordKeeper &RK, llvm::StringRef name, SourceLocation declLoc)
      : RK(RK), name(name), declLoc(declLoc)
   {}

   RecordKeeper &RK;

   llvm::StringRef name;
   SourceLocation declLoc;

   std::vector<BaseClass> bases;

   std::vector<RecordField> parameters;
   std::vector<RecordField> fields;
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &str, Class &C)
{
   C.printTo(str);
   return str;
}

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &str, Class *C)
{
   str << *C;
   return str;
}

class Record {
public:
   void addOwnField(SourceLocation loc, llvm::StringRef key,
                    Type *Ty, Value *V) {
      auto &F = ownFields.emplace_back(key, Ty, V, loc);
      auto &Entry = *fieldValues.try_emplace(key, V).first;
      F.name = Entry.getKey();
   }

   void setFieldValue(llvm::StringRef key, Value *V)
   {
      fieldValues[key] = V;
   }

   const llvm::StringRef &getName() const
   {
      return name;
   }

   SourceLocation getDeclLoc() const
   {
      return declLoc;
   }

   const std::vector<Class::BaseClass> &getBases() const
   {
      return bases;
   }

   const llvm::StringMap<Value *> &getFieldValues() const
   {
      return fieldValues;
   }

   void addBase(Class *Base, std::vector<Value*> &&templateParams)
   {
      bases.emplace_back(Base, move(templateParams));
   }

   bool hasField(llvm::StringRef name) const
   {
      auto it = fieldValues.find(name);
      return it != fieldValues.end();
   }

   Type *getFieldType(llvm::StringRef fieldName) const
   {
      for (auto &B : bases) {
         if (auto F = B.getBase()->getField(fieldName))
            return F->getType();
      }

      return nullptr;
   }

   Value *getFieldValue(llvm::StringRef fieldName) const
   {
      auto it = fieldValues.find(fieldName);
      if (it != fieldValues.end())
         return it->getValue();

      return nullptr;
   }

   const std::vector<RecordField> &getOwnFields() const
   {
      return ownFields;
   }

   RecordField *getOwnField(llvm::StringRef name)
   {
      for (auto &f : ownFields)
         if (f.getName() == name)
            return &f;

      return nullptr;
   }

   bool inheritsFrom(Class *C)
   {
      for (auto &B : bases) {
         if (B.getBase() == C)
            return true;

         if (B.getBase()->inheritsFrom(C))
            return true;
      }

      return false;
   }

   void dump();
   void dumpAllValues();

   void printTo(llvm::raw_ostream &out);

   friend class RecordKeeper;

private:
   Record(RecordKeeper &RK, llvm::StringRef name, SourceLocation declLoc)
      : RK(RK), name(name), declLoc(declLoc)
   {}

   RecordKeeper &RK;

   llvm::StringRef name;
   SourceLocation declLoc;

   std::vector<Class::BaseClass> bases;
   std::vector<RecordField> ownFields;

   llvm::StringMap<Value*> fieldValues;
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &str, Record &R)
{
   R.printTo(str);
   return str;
}

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &str, Record *R)
{
   str << *R;
   return str;
}

class RecordKeeper {
public:
   RecordKeeper(TableGen &TG,
                llvm::StringRef namespaceName = "",
                SourceLocation loc = {},
                RecordKeeper *Parent = nullptr)
      : TG(TG), namespaceName(namespaceName), declLoc(loc), Parent(Parent)
   {}

   template<class KeyTy, class ValueTy>
   struct MapVectorPair : std::pair<KeyTy, ValueTy> {
      using parent = std::pair<KeyTy, ValueTy>;

      MapVectorPair(parent &&p)
         : parent(std::move(p))
      {}

      KeyTy &getKey() { return parent::first; }
      KeyTy const& getKey() const { return parent::first; }

      ValueTy &getValue() { return parent::second; }
      ValueTy const& getValue() const { return parent::second; }
   };

   using MapVectorTy =
      llvm::MapVector<llvm::StringRef, Record*,
                      llvm::DenseMap<llvm::StringRef, unsigned>,
                      std::vector<MapVectorPair<llvm::StringRef, Record*>>>;

   Record *addRecord(llvm::StringRef name, SourceLocation loc);
   Class *addClass(llvm::StringRef name, SourceLocation loc);

   struct ValueDecl {
      ValueDecl(Value *Val, SourceLocation loc) : Val(Val), loc(loc)
      { }

      ValueDecl() : Val(nullptr) {}

      Value *getVal() const
      {
         return Val;
      }

      SourceLocation getLoc() const
      {
         return loc;
      }

   private:
      Value *Val;
      SourceLocation loc;
   };

   void addValue(llvm::StringRef name,
                 Value *V,
                 SourceLocation loc);

   RecordKeeper *addNamespace(llvm::StringRef name,
                              SourceLocation loc) {
      return &Namespaces.try_emplace(name, TG, name, loc, this)
                        .first->getValue();
   }

   Record *lookupRecord(llvm::StringRef name) const
   {
      auto it = Records.find(name);
      if (it == Records.end()) {
         if (Parent)
            return Parent->lookupRecord(name);

         return nullptr;
      }

      return it->getValue();
   }

   Class *lookupClass(llvm::StringRef name) const
   {
      auto it = Classes.find(name);
      if (it == Classes.end()) {
         if (Parent)
            return Parent->lookupClass(name);

         return nullptr;
      }

      return it->getValue();
   }

   ValueDecl *lookupValueDecl(llvm::StringRef name) const
   {
      auto it = Values.find(name);
      if (it == Values.end()) {
         if (Parent)
            return Parent->lookupValueDecl(name);

         return nullptr;
      }

      return const_cast<ValueDecl*>(&it->getValue());
   }

   RecordKeeper *lookupNamespace(llvm::StringRef name) const
   {
      auto it = Namespaces.find(name);
      if (it == Namespaces.end()) {
         if (Parent)
            return Parent->lookupNamespace(name);

         return nullptr;
      }

      return const_cast<RecordKeeper*>(&it->getValue());
   }

   SourceLocation lookupAnyDecl(llvm::StringRef name) const
   {
      if (auto R = lookupRecord(name))
         return R->getDeclLoc();

      if (auto C = lookupClass(name))
         return C->getDeclLoc();

      if (auto V = lookupValueDecl(name))
         return V->getLoc();

      if (auto NS = lookupNamespace(name))
         return NS->getDeclLoc();

      return SourceLocation();
   }

   const llvm::StringMap<Class *> &getAllClasses() const
   {
      return Classes;
   }

   const MapVectorTy &getAllRecords() const
   {
      return Records;
   }

   const llvm::StringMap<ValueDecl> &getValueDecls() const
   {
      return Values;
   }

   const llvm::StringMap<RecordKeeper> &getAllNamespaces() const
   {
      return Namespaces;
   }

   const llvm::StringRef &getNamespaceName() const
   {
      return namespaceName;
   }

   bool isGlobalRecordKeeper() const
   {
      return namespaceName.empty();
   }

   const SourceLocation &getDeclLoc() const
   {
      return declLoc;
   }

   void getAllDefinitionsOf(Class *C,
                            llvm::SmallVectorImpl<Record*> &vec) const {
      for (auto &R : Records)
         if (R.getValue()->inheritsFrom(C))
            vec.push_back(R.getValue());
   }

   void getAllDefinitionsOf(llvm::StringRef className,
                            llvm::SmallVectorImpl<Record*> &vec) const {
      return getAllDefinitionsOf(lookupClass(className), vec);
   }

   void dump();
   void printTo(llvm::raw_ostream &out);

private:
   TableGen &TG;

   llvm::StringRef namespaceName;
   SourceLocation declLoc;
   RecordKeeper *Parent;

   llvm::StringMap<Class*> Classes;
   MapVectorTy Records;
   llvm::StringMap<ValueDecl> Values;
   llvm::StringMap<RecordKeeper> Namespaces;
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &str, RecordKeeper &RK)
{
   RK.printTo(str);
   return str;
}

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &str, RecordKeeper *RK)
{
   str << *RK;
   return str;
}

} // namespace tblgen
} // namespace cdot

#endif //TABLEGEN_RECORD_H
