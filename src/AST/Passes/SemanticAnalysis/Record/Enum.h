//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUM_H
#define CDOT_ENUM_H

#include "Record.h"
#include "../../../../Variant/Variant.h"

namespace cdot {

struct Argument;

namespace ast {

class EnumDecl;

}

namespace cl {

struct EnumCase {
   string name;
   long rawValue;
   std::vector<Argument> associatedValues;
   SourceLocation loc;
};

class Enum : public Record {
public:
   Enum(AccessModifier am,
        const string& name,
        Namespace *NS,
        std::vector<TemplateParameter> &&templateParams,
        const SourceLocation &loc,
        ast::EnumDecl *decl);

   void addCase(const string& name, EnumCase&& case_, SourceLocation loc);

   size_t getNumCases() const
   {
      return cases.size();
   }

   bool isRawEnum() const
   {
      return !hasAssociatedValues_;
   }

   size_t getMaxAssociatedValues() const
   {
      return maxAssociatedValues;
   }

   bool hasCase(llvm::StringRef caseName) const
   {
      return cases.find(caseName) != cases.end();
   }

   bool hasAssociatedValues() const
   {
      return hasAssociatedValues_;
   }

   void hasAssociatedValues(bool assoc)
   {
      hasAssociatedValues_ = assoc;
   }

   Type* getRawType()
   {
      return rawType;
   }

   void setRawType(Type* raw)
   {
      rawType = raw;
   }

   unordered_map<string, EnumCase>& getCases()
   {
      return cases;
   }

   EnumCase& getCase(llvm::StringRef caseName)
   {
      assert(hasCase(caseName) && "Call hasCase first!");
      return cases[caseName];
   }

   static bool classof(Record const* T)
   {
      return T->getTypeID() == EnumID;
   }

   void calculateSizeImpl();

protected:
   unordered_map<string, EnumCase> cases;
   size_t maxAssociatedValues = 0;

   Type* rawType;
   bool hasAssociatedValues_ = false;
};

}
}


#endif //CDOT_ENUM_H
