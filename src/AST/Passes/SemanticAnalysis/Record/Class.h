//
// Created by Jonas Zell on 09.07.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <string>
#include <unordered_map>
#include "../../../../Variant/Variant.h"
#include "../../../SymbolTable.h"

#include "../../../../Variant/Type/ObjectType.h"

#include "Record.h"

using namespace cdot;
using std::string;
using std::map;
using std::unordered_map;
using std::unordered_multimap;

namespace cdot {

struct TemplateParameter;

namespace ast {

class ClassDecl;
class Expression;
class FieldDecl;

}

namespace cl {

using namespace cdot::ast;

class Class;

struct Method;


class Struct: public Record {
public:
   Struct(
      AccessModifier am,
      const string& className,
      Namespace *NS,
      std::vector<TemplateParameter> &&templateParams,
      RecordDecl *decl,
      const SourceLocation &loc
   );

   ~Struct();

   void pushConstraintSet(std::vector<ExtensionConstraint> &&constraints);
   void popConstraintSet();

   const std::vector<ExtensionConstraint>& getConstraintSet(unsigned i);

   bool declareMemberwiseInitializer();

   void calculateSizeImpl();

   bool hasNonEmptyDeinitializer() const { return has_nonempty_deinit; }
   void hasNonEmptyDeinitializer(bool nonempty)
      { has_nonempty_deinit = nonempty; }

   size_t getMethodOffset(const string& methodName) const;

   Method* getMemberwiseInitializer() const
   {
      return memberwiseInitializer;
   }

   Method* getParameterlessConstructor() const
      { return parameterlessConstructor; }
   void setParameterlessConstructor(Method *m) { parameterlessConstructor = m; }

   void addInitializer(Method *init);

   typedef std::unique_ptr<Class> UniquePtr;

   static bool classof(Record const* T)
   {
      switch (T->getTypeID()) {
         case StructID:
         case ClassID:
            return true;
         default:
            return false;
      }
   }

protected:
   bool activeConstraints = false;
   size_t fieldCount = 0;

   llvm::SmallPtrSet<Method*, 4> initializers;

   Method* parameterlessConstructor = nullptr;
   Method* memberwiseInitializer = nullptr;

   unordered_map<string, size_t> methodOffsets;

   bool has_nonempty_deinit = false;
   bool finalized = false;
};

class Class: public Struct {
public:
   Class(
      AccessModifier am,
      const string& className,
      Namespace *NS,
      std::vector<TemplateParameter> &&templateParams,
      RecordDecl *decl,
      const SourceLocation &loc,
      bool isAbstract
   );

   bool isBaseClassOf(const string &child) const;
   bool isBaseClassOf(Class *child) const;

   const std::vector<Class*> &getSubclasses() const { return extendedBy; }

   void setParentClass(Class* parent) { parentClass = parent; }
   Class* getParent() { return parentClass; }

   bool isAbstract() const { return is_abstract; }

   static bool classof(Record const* T)
   {
      return T->getTypeID() == ClassID;
   }

private:
   Class* parentClass = nullptr;
   std::vector<Class*> extendedBy;

   bool is_abstract = false;
};

} // namespace cl
} // namespace cdot

#endif //CDOT_CLASS_H
