//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUM_H
#define CDOT_ENUM_H

#include "Class.h"
#include "../../../../Variant/Variant.h"

namespace cdot {
   struct Argument;
namespace cl {

   struct EnumCase {
      string name;
      long rawValue;
      std::vector<pair<string, Type>> associatedValues;
   };

   class Enum : public Class {
   public:
      Enum(
         AccessModifier am,
         const string& name,
         const SourceLocation &loc,
         EnumDecl *decl
      );

      void addCase(string& name, EnumCase&& case_, SourceLocation loc);

      llvm::Type *getSelfType() const override;

      size_t getNumCases() const
      {
         return cases.size();
      }

      void generateMemoryLayout(CodeGen &CGM) override;

      size_t getMaxAssociatedValues() const
      {
         return maxAssociatedValues;
      }

      bool isEnum() const override
      {
         return true;
      }

      bool hasCase(const string& caseName) const
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

      BuiltinType* getRawType()
      {
         return rawType;
      }

      void setRawType(BuiltinType* raw)
      {
         rawType = raw;
      }

      bool isRawEnum() const override
      {
         return !hasAssociatedValues_;
      }

      unordered_map<string, EnumCase>& getCases()
      {
         return cases;
      }

      EnumCase& getCase(string& caseName)
      {
         assert(hasCase(caseName) && "Call hasCase first!");
         return cases[caseName];
      }

   protected:
      unordered_map<string, EnumCase> cases;
      size_t maxAssociatedValues = 0;

      BuiltinType* rawType;
      bool hasAssociatedValues_ = false;
   };

}
}


#endif //CDOT_ENUM_H
