//
// Created by Jonas Zell on 10.10.17.
//

#ifndef CDOT_RECORD_H
#define CDOT_RECORD_H

#include <string>
#include <llvm/IR/IRBuilder.h>
#include <unordered_map>

#include "../../../SymbolTable.h"
#include "../../../../lex/Token.h"
#include "../../../../Variant/Type/Generic.h"

using std::string;
using std::unordered_map;

namespace cdot {

enum class AccessModifier : unsigned int;

namespace ast {

class CodeGen;
class AstNode;
class RecordTemplateDecl;
class RecordDecl;
class Statement;
class MethodDecl;
class PropDecl;

} // namespace ast

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
            const QualType &ty,
            bool isStatic,
            Method *getter,
            Method *setter,
            Record *rec,
            string &&newValName,
            ast::PropDecl *decl);

   bool hasGetter() const;
   bool hasSetter() const;

   const string &getName() const;
   void setName(const string &name);

   const QualType &getType() const;
   void setType(const QualType &ty);

   Method *getGetter() const;
   void setGetter(Method *getter);

   Method *getSetter() const;
   void setSetter(Method *setter);

   Record *getRecord() const;
   void setRecord(Record *record);

   bool isStatic() const;
   void isStatic(bool is_static);

   ast::PropDecl *getDecl() const;
   void setDecl(ast::PropDecl *decl);

   const string &getNewValName() const;
   void setNewValName(const string &newValName);

   const string &getNewValBinding() const;
   void setNewValBinding(const string &newValBinding);

protected:
   string name;
   QualType type;
   bool is_static;

   Method *getter;
   Method *setter;

   string newValName;
   string newValBinding;

   Record *record;
   ast::PropDecl *decl;
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
      Type *ty, const string &alias,
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

   ast::RecordDecl *getDecl() const;
   void setDecl(ast::RecordDecl *decl);

   const unordered_multimap<string, std::shared_ptr<Method>> &
   getMethods() const;
   void setMethods(
      const unordered_multimap<string, std::shared_ptr<Method>> &methods);

   virtual Method* getMethod(llvm::StringRef name);
   virtual Method* getOwnMethod(llvm::StringRef name)
   {
      return getMethod(name);
   }

   virtual Method* getMethod(unsigned id);

   void declareMethodAlias(const string& name, const string& mangledOriginal);

   virtual Method* declareMethod(
      const string &methodName,
      const QualType& ret_type,
      AccessModifier access,
      std::vector<Argument>&& args,
      bool isStatic,
      ast::MethodDecl* declaration,
      SourceLocation loc
   );

   Property* declareProperty(
      const string &propName,
      const QualType &ty,
      bool isStatic,
      Method *getter,
      Method *setter,
      string &&newValName,
      ast::PropDecl *decl
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
      Type*& caller
   ) const;

   virtual const ExtensionConstraint* checkConstraints(
      Method* method,
      Type* caller
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
      ast::RecordDecl *decl
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
   ast::RecordDecl *decl;

   Method *operatorEquals = nullptr;
};

}
}

#endif //CDOT_RECORD_H
