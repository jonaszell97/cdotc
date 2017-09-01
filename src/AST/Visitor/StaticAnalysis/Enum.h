//
// Created by Jonas Zell on 29.08.17.
//

#ifndef CDOT_ENUM_H
#define CDOT_ENUM_H

#include "Class.h"
#include "../../../Variant/Variant.h"

namespace cdot {
namespace cl {

   struct EnumCase {
      string name;
      long rawValue;
      std::vector<pair<string, Type*>> associatedValues;
   };

   class Enum : public Class {
   public:
      Enum(
         string& name,
         std::vector<ObjectType*>& conformsTo,
         std::vector<ObjectType*>& generics
      );

      void addCase(string& name, EnumCase&& case_) {
         cases.emplace(name, case_);
         if (case_.associatedValues.size() > maxAssociatedValues) {
            maxAssociatedValues = case_.associatedValues.size();
         }

         if (!case_.associatedValues.empty()) {
            std::vector<Type*> argTypes;
            std::vector<string> argNames;
            for (const auto& assoc : case_.associatedValues) {
               argNames.push_back(assoc.first);
               argTypes.push_back(assoc.second);
            }

            declareMethod(name, nullptr, AccessModifier::PUBLIC, argNames, argTypes, {}, generics, false, nullptr);
         }
      }

      size_t getNumCases() {
         return cases.size();
      }

      void generateMemoryLayout(llvm::IRBuilder<>& Builder) override;

      size_t getMaxAssociatedValues() {
         return maxAssociatedValues;
      }

      bool isEnum() override {
         return true;
      }

      bool hasCase(string& caseName) {
         return cases.find(caseName) != cases.end();
      }

      MethodResult hasCase(string& caseName, std::vector<Type*>& assocTypes, std::vector<Type*>& generics) {
         MethodResult res;
         res.compatibility = CompatibilityType::FUNC_NOT_FOUND;

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

      EnumCase& getCase(string& caseName) {
         assert(hasCase(caseName) && "Call hasCase first!");
         return cases[caseName];
      }

   protected:
      unordered_map<string, EnumCase> cases;
      size_t maxAssociatedValues = 0;
   };

}
}


#endif //CDOT_ENUM_H
