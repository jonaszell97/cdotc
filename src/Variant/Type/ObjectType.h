//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_OBJECTTYPE_H
#define CDOT_OBJECTTYPE_H


#include <unordered_map>
#include <vector>

#include "Type.h"
#include "../../lex/SourceLocation.h"
#include "../../AST/Passes/SemanticAnalysis/Template.h"

using std::unordered_map;

namespace cdot {

class PointerType;
class GenericType;

namespace cl {
class Record;
}

class ObjectType : public Type {
protected:
   explicit ObjectType(cl::Record *record,
                       BoxedPrimitive primitive = BoxedPrimitive::BP_None);

   cl::Record *Rec;
   BoxedPrimitive primitiveType = BP_None;

public:
   static ObjectType *get(cl::Record *record);
   static ObjectType *get(llvm::StringRef className);
   static ObjectType *get(BoxedPrimitive primitive);

   static ObjectType* getAnyTy();

   bool isRawEnum() const;

   sema::TemplateArgList const& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   bool isBoxedEquivOf(Type const* other) const;

   std::string toString() const;

   cl::Record *getRecord() const
   {
      return Rec;
   }

   unsigned short getAlignment() const;
   size_t getSize() const;

   Type* unbox() const;

   BoxedPrimitive getPrimitiveKind() const
   {
      return primitiveType;
   }

   static bool classof(ObjectType const*) { return true; }
   static bool classof(Type const* T)
   {
      switch(T->getTypeID()) {
#     define CDOT_OBJ_TYPE(Name) \
         case TypeID::Name##ID: return true;
#     include "Types.def"

         default: return false;
      }
   }
};

// to be used when resolving template arguments and the actual underlying
// record of the type might not exist
class InconcreteObjectType: public ObjectType {
protected:
   InconcreteObjectType(llvm::StringRef className,
                        sema::TemplateArgList &&templateArgs);

   InconcreteObjectType(cl::Record *record,
                        sema::TemplateArgList &&templateArgs);

   sema::TemplateArgList templateArgs;

public:
   static InconcreteObjectType *get(llvm::StringRef className,
                                    sema::TemplateArgList &&templateArgs);

   static InconcreteObjectType *get(cl::Record *record,
                                    sema::TemplateArgList &&templateArgs);

   static bool classof (Type const* T)
   {
      return T->getTypeID() == TypeID::InconcreteObjectTypeID;
   }

   const sema::TemplateArgList& getTemplateArgs() const
   {
      return templateArgs;
   }

   bool hasTemplateArgs() const
   {
      return true;
   }
};

} // namespace cdot


#endif //CDOT_OBJECTTYPE_H
