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
         string& name,
         std::vector<GenericConstraint>& generics,
         const SourceLocation &loc
      );

      void addCase(string& name, EnumCase&& case_, SourceLocation loc) {
         cases.emplace(name, case_);
         if (case_.associatedValues.size() > maxAssociatedValues) {
            maxAssociatedValues = case_.associatedValues.size();
         }

         if (!case_.associatedValues.empty()) {
            hasAssociatedValues_ = true;

            std::vector<Argument> args;
            for (const auto& assoc : case_.associatedValues) {
               args.emplace_back(assoc.first, assoc.second);
            }

            Type retTy;
            declareMethod(name, retTy, AccessModifier::PUBLIC, std::move(args), generics, false,
               nullptr, loc);
         }
      }

      size_t getNumCases() {
         return cases.size();
      }

      void generateMemoryLayout(CodeGen &CGM) override;

      size_t getMaxAssociatedValues() {
         return maxAssociatedValues;
      }

      bool isEnum() override
      {
         return true;
      }

      bool hasCase(string& caseName) {
         return cases.find(caseName) != cases.end();
      }

      CallCompatability hasCase(string& caseName, std::vector<Argument>& assocTypes,
         std::vector<GenericType*>& generics)
      {
         CallCompatability res;
         if (!hasCase(caseName)) {
            return res;
         }

         res.compatibility = CompatibilityType::NO_MATCHING_CALL;
         auto types = cases[caseName].associatedValues;
         if (types.size() != assocTypes.size()) {
            return res;
         }

         return hasMethod(caseName, assocTypes, generics);
      }

      bool hasAssociatedValues() {
         return hasAssociatedValues_;
      }

      void hasAssociatedValues(bool assoc)
      {
         hasAssociatedValues_ = assoc;
      }

      BuiltinType* getRawType() {
         return rawType;
      }

      void setRawType(BuiltinType* raw) {
         rawType = raw;
      }

      bool isRawEnum() override
      {
         return !hasAssociatedValues_;
      }

      unordered_map<string, EnumCase>& getCases()
      {
         return cases;
      }

      EnumCase& getCase(string& caseName) {
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
