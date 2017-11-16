//
// Created by Jonas Zell on 10.10.17.
//

#ifndef CDOT_RECORD_H
#define CDOT_RECORD_H

#include <string>
#include <llvm/IR/IRBuilder.h>
#include <unordered_map>

#include "../../../SymbolTable.h"
#include "../../../../Token.h"
#include "../../../../Variant/Type/Generic.h"

class CodeGen;
class AstNode;
class RecordTemplateDecl;
class RecordDecl;
class Statement;
class MethodDecl;
class PropDecl;

enum class AccessModifier : unsigned int;

using std::string;
using std::unordered_map;

namespace cdot {

namespace cl {

enum class ImplicitConformanceKind {
   StringRepresentable,
   Hashable,
   Equatable
};

struct ImplicitConformance {
   ImplicitConformance(
      ImplicitConformanceKind kind,
      Method *method
   ) : kind(kind), method(method) {}

   ImplicitConformanceKind kind;
   Method *method;
};

class Property {
public:
   Property(const string &name,
            const Type &ty,
            bool isStatic,
            Method *getter,
            Method *setter,
            Record *rec,
            string &&newValName,
            PropDecl *decl);

   bool hasGetter() const;
   bool hasSetter() const;

   const string &getName() const;
   void setName(const string &name);

   const Type &getType() const;
   void setType(const Type &ty);

   Method *getGetter() const;
   void setGetter(Method *getter);

   Method *getSetter() const;
   void setSetter(Method *setter);

   Record *getRecord() const;
   void setRecord(Record *record);

   bool isStatic() const;
   void isStatic(bool is_static);

   PropDecl *getDecl() const;
   void setDecl(PropDecl *decl);

   const string &getNewValName() const;
   void setNewValName(const string &newValName);

   const string &getNewValBinding() const;
   void setNewValBinding(const string &newValBinding);

protected:
   string name;
   Type type;
   bool is_static;

   Method *getter;
   Method *setter;

   string newValName;
   string newValBinding;

   Record *record;
   PropDecl *decl;
};

class Record {
public:

   void isTemplate(
      RecordTemplate *Template,
      TemplateArgList *templateArg
   );

   virtual bool isClass() const
   {
      return false;
   }

   virtual bool isNonUnion() const
   {
      return false;
   }

   virtual bool isStruct() const
   {
      return false;
   }

   virtual bool isProtocol() const
   {
      return false;
   }

   virtual bool isUnion() const
   {
      return false;
   }

   virtual bool isEnum() const
   {
      return false;
   }

   virtual bool isRawEnum() const
   {
      return false;
   }

   virtual size_t getSize() const
   {
      return occupiedBytes;
   }

   virtual short getAlignment() const
   {
      return alignment;
   }

   void setAlignment(short align)
   {
      alignment = align;
      manualAlignment = true;
   }

   void addImplicitConformance(ImplicitConformanceKind kind,
                               std::vector<string> &protocolMethods);

   std::vector<ImplicitConformance>& getImplicitConformances()
   {
      return implicitConformances;
   }

   Method *getImplicitConformance(ImplicitConformanceKind kind);

   string& getName()
   {
      return recordName;
   }

   virtual void finalize() {}

   bool isDeclared() const;

   virtual void generateTypeInfo(CodeGen &CGM) {}
   virtual void generateMemoryLayout(CodeGen &CGM) {}
   virtual void generateProtocolMemoryLayout(CodeGen &CGM) {}
   virtual void generateVTables(CodeGen &CGM) {}

   virtual llvm::Type *getSelfType() const;

   const string& getDeclarationNamespace()
   {
      return declarationNamespace;
   }

   AccessModifier getAccess() const;
   bool isPrivate() const;
   bool isProtected() const;

   string getTypeName() const;
   int getNameSelector() const;

   const std::vector<Typedef> &getTypedefs() const;
   void setTypedefs(const std::vector<Typedef> &typedefs);

   void declareTypedef(
      BuiltinType *ty, const string &alias,
      const std::vector<TemplateConstraint> &templateArgs,
      AccessModifier access, AstNode *decl
   );

   virtual bool isGeneric() const
   {
      return false;
   }

   virtual bool isRefcounted() const
   {
      return false;
   }

   size_t getTypeID() const
   {
      return typeID;
   }

   void addUse()
   {
      ++uses;
   }

   size_t& getNumUses()
   {
      return uses;
   }

   const SourceLocation& getSourceLoc()
   {
      return loc;
   }

   bool isOpaque() const
   {
      return is_opaque;
   }

   void isOpaque(bool opaque)
   {
      is_opaque = opaque;
   }

   bool isTemplated() const;
   const TemplateArg &getTemplateArg(const string &name) const;
   const std::vector<TemplateArg>& getTemplateArgs() const;
   RecordTemplate* getTemplate();

   virtual bool conformsTo(const string &name) const;
   void addConformance(Class *cl);
   std::set<Class*> &getConformances();

   template<typename T>
   T* getAs()
   {
      return static_cast<T*>(this);
   }

   void addInnerRecord(Record *rec)
   {
      innerRecords.push_back(rec);
      rec->outerRecord = this;
   }

   std::vector<Record*>& getInnerRecords()
   {
      return innerRecords;
   }

   Record *getOuterRecord()
   {
      return outerRecord;
   }

   bool hasInnerRecord(const string &inner) const;
   Record *getInnerRecord(const string &name);

   Method *getOperatorEquals() const;
   void setOperatorEquals(Method *operatorEquals);

   RecordDecl *getDecl() const;
   void setDecl(RecordDecl *decl);

   const unordered_multimap<string, std::shared_ptr<Method>> &
   getMethods() const;
   void setMethods(
      const unordered_multimap<string, std::shared_ptr<Method>> &methods);

   virtual Method* getMethod(const string &method_name);
   virtual Method* getMethod(unsigned id);

   void declareMethodAlias(const string& name, const string& mangledOriginal);

   virtual Method* declareMethod(
      const string &methodName,
      const Type& ret_type,
      AccessModifier access,
      std::vector<Argument>&& args,
      bool isStatic,
      MethodDecl* declaration,
      SourceLocation loc
   );

   Property* declareProperty(
      const string &propName,
      const Type &ty,
      bool isStatic,
      Method *getter,
      Method *setter,
      string &&newValName,
      PropDecl *decl
   );

   Method *declareMethodTemplate(
      MethodTemplate *Template
   );

   void checkTemplateArgCompatability(
      Method &newMethod
   );

   void checkDuplicateMethodDeclaration(
      AstNode *decl,
      const string &name,
      const string &mangledName
   );

   bool hasMethodWithName(const string &name) const;
   bool hasMethodTemplate(const string &name) const;

   bool hasProperty(const string &name) const;
   Property *getProperty(const string &name);

   unordered_map<string, Property> &getProperties();

   std::vector<TemplateConstraint> &getMethodConstraints(
      const string &forMethod);

   virtual bool checkConstraint(
      const ExtensionConstraint& constraint,
      BuiltinType*& caller
   ) const;

   virtual const ExtensionConstraint* checkConstraints(
      Method* method,
      BuiltinType* caller
   ) const;

protected:
   static size_t lastTypeID;

   size_t uses = 0;

   AccessModifier access;
   size_t typeID;

   Record(
      const string &name,
      AccessModifier access,
      const SourceLocation &loc,
      RecordDecl *decl
   );

   string recordName;
   string declarationNamespace;

   unordered_map<string, Property> properties;
   unordered_multimap<string, std::shared_ptr<Method>> methods;

public:
   auto getMethodTemplates(const string &name)
      -> decltype(this->methods.equal_range(""));

protected:

   std::set<Class*> conforms_to;
   std::vector<Typedef> typedefs;

   std::vector<Record*> innerRecords;
   Record *outerRecord = nullptr;

   RecordTemplate *Template = nullptr;
   TemplateArgList *templateArgs;

   std::vector<ImplicitConformance> implicitConformances;

   size_t occupiedBytes = 0;
   short alignment = 1;
   bool manualAlignment = false;

   bool is_opaque = false;

   SourceLocation loc;
   RecordDecl *decl;

   Method *operatorEquals = nullptr;
};

}
}

#endif //CDOT_RECORD_H
