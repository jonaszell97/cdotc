//
// Created by Jonas Zell on 10.10.17.
//

#ifndef CDOT_RECORD_H
#define CDOT_RECORD_H

#include <string>
#include <unordered_map>

#include "../Template.h"

#include "../../../SymbolTable.h"
#include "../../../../lex/Token.h"
#include "../../../../Variant/Type/Generic.h"
#include "../../../../Basic/Precedence.h"

namespace cdot {

enum class AccessModifier : unsigned int;

namespace ast {

class CodeGen;
class AstNode;
class RecordDecl;
class Statement;
class MethodDecl;
class PropDecl;
class ExtensionDecl;
class AssociatedTypeDecl;
class StaticExpr;
class FieldDecl;
class CallableDecl;

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

struct ExtensionConstraint {
   enum ConstraintKind {
      CONFORMANCE,
      TYPE_EQUALITY,
      TYPE_INEQUALITY,
      DEFAULT_CONSTRUCTIBLE,
      IS_STRUCT,
      IS_CLASS,
      IS_PROTOCOL,
      IS_ENUM
   };

   string constrainedGenericTypeName;
   std::shared_ptr<TypeRef> typeConstraint = nullptr;
   ConstraintKind kind;

   string reportFailure() const;
};

struct Field {
   Field(string name, QualType type, AccessModifier access_modifier,
         bool isConst, FieldDecl* decl);

   typedef std::unique_ptr<Field> UniquePtr;
   typedef std::shared_ptr<Field> SharedPtr;

   string fieldName;
   string linkageName;
   QualType fieldType;
   AccessModifier accessModifier;

   bool isConst = false;
   bool isStatic = false;

   bool hasGetter() const
   {
      return getter != nullptr;
   }

   bool hasSetter() const
   {
      return setter != nullptr;
   }

   bool hasDefaultValue() const;

   llvm::StringRef getFieldName() const
   {
      return fieldName;
   }

   llvm::StringRef getLinkageName() const
   {
      return linkageName;
   }

   QualType getFieldType() const
   {
      return fieldType;
   }

   AccessModifier getAccessModifier() const
   {
      return accessModifier;
   }

   bool isIsConst() const
   {
      return isConst;
   }

   bool isIsStatic() const
   {
      return isStatic;
   }

   Method *getGetter() const
   {
      return getter;
   }

   Method *getSetter() const
   {
      return setter;
   }

   FieldDecl *getDeclaration() const
   {
      return declaration;
   }

   Record *getOwningRecord() const
   {
      return owningRecord;
   }

   Method *getter = nullptr;
   Method *setter = nullptr;

   FieldDecl* declaration;
   Record* owningRecord = nullptr;
};

class Property {
public:
   Property(const std::string &name,
            const QualType &ty,
            bool isStatic,
            Method *getter,
            Method *setter,
            Record *rec,
            std::string &&newValName,
            ast::PropDecl *decl);

   bool hasGetter() const;
   bool hasSetter() const;

   const std::string &getName() const;
   void setName(const std::string &name);

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

   const std::string &getNewValName() const;
   void setNewValName(const std::string &newValName);

   const std::string &getNewValBinding() const;
   void setNewValBinding(const std::string &newValBinding);

   SourceLocation getSourceLoc() const;

protected:
   std::string name;
   QualType type;
   bool is_static;

   Method *getter;
   Method *setter;

   std::string newValName;
   std::string newValBinding;

   Record *record;
   ast::PropDecl *decl;
};

class AssociatedType {
public:
   explicit AssociatedType(AssociatedTypeDecl *decl);
   explicit AssociatedType(const AssociatedType &other);

   const llvm::StringRef &getName() const { return name; }
   const QualType &getType() const { return type; }

   SourceLocation getSourceLoc() const { return loc; }
   AssociatedTypeDecl *getDecl() const { return decl; }

   Protocol *getProto() const
   {
      return Proto;
   }

   void setProto(Protocol *Proto)
   {
      AssociatedType::Proto = Proto;
   }

private:
   llvm::StringRef name;
   QualType type;
   SourceLocation loc;

   Protocol *Proto = nullptr;
   AssociatedTypeDecl *decl;
};

class Protocol;

class Record {
public:
   enum TypeID {
      StructID = 0,
      ClassID,
      EnumID,
      UnionID,
      ProtocolID
   };

   ~Record();

   static bool classof(Record const* T) { return true; }

   bool isClass() const;
   bool isStruct() const;
   bool isProtocol() const;
   bool isUnion() const;
   bool isEnum() const;
   bool isRawEnum() const;

   void updateLatestMethodID(size_t id)
   {
      if (id > lastMethodID)
         lastMethodID = id;
   }

   size_t getSize() const
   {
      return occupiedBytes;
   }

   unsigned short getAlignment() const
   {
      return alignment;
   }

   void setAlignment(unsigned short align)
   {
      alignment = align;
      manualAlignment = true;
   }

   TypeID getTypeID() const
   {
      return typeID;
   }

   void addImplicitConformance(ImplicitConformanceKind kind);

   std::vector<ImplicitConformance>& getImplicitConformances()
   {
      return implicitConformances;
   }

   Method *getImplicitConformance(ImplicitConformanceKind kind);

   llvm::StringRef getName() const
   {
      return fullName;
   }

   Namespace *getDeclarationNamespace() const
   {
      return declarationNamespace;
   }

   bool isDeclared() const;

   llvm::StringRef getNameWitoutNamespace() const
   {
      return recordName;
   }

   AccessModifier getAccess() const;
   bool isPrivate() const;
   bool isProtected() const;

   std::string getTypeName() const;
   int getNameSelector() const;

   bool isGeneric() const { return false; }

   bool isRefcounted() const { return isClass(); }

   size_t getRecordID() const { return recordID; }

   void addUse()
   {
      ++uses;
   }

   size_t& getNumUses()
   {
      return uses;
   }

   const SourceLocation& getSourceLoc() const
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

   sema::ResolvedTemplateArg const* getTemplateArg(llvm::StringRef name) const;

   const sema::TemplateArgList &getTemplateArgs() const
   {
      return templateArgs;
   }

   void setTemplateArgs(sema::TemplateArgList &&templateArgs)
   {
      Record::templateArgs = std::move(templateArgs);
   }

   bool isTemplated() const;
   std::vector<TemplateParameter> &getTemplateParams()
   {
      return templateParams;
   }

   std::vector<TemplateParameter> const& getTemplateParams() const
   {
      return templateParams;
   }

   bool conformsTo(const std::string &name) const;
   bool conformsToTemplateBase(const std::string &name) const;

   void addConformance(Protocol *cl);
   std::set<Protocol*> &getConformances() { return conformances; }

   void inheritProtocols(Protocol* current);
   void checkProtocolConformance(Protocol *protoObj);

   bool protectedPropAccessibleFrom(Record *Rec);
   bool privatePropAccessibleFrom(Record *Rec);

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

   bool hasInnerRecord(Record *Rec) const;
   bool hasInnerRecord(const std::string &inner) const;

   Method *getOperatorEquals() const;
   void setOperatorEquals(Method *operatorEquals);

   ast::RecordDecl *getDecl() const;
   void setDecl(ast::RecordDecl *decl);

   std::unordered_multimap<string, Method> &getMethods()
   {
      return methods;
   }

   Method* getMethod(llvm::StringRef name, bool checkParent = true);
   Method* getOwnMethod(llvm::StringRef name) { return getMethod(name, false); }
   Method* getMethod(size_t id);

   struct method_iterator:
                     public std::iterator<std::forward_iterator_tag, Method> {
      explicit method_iterator(Record *R, llvm::StringRef methodName);

      method_iterator()
         : R(nullptr), current(nullptr), phase(3)
      {}

      bool operator==(method_iterator const& rhs) const;

      bool operator!=(method_iterator const& rhs) const
      {
         return !operator==(rhs);
      }

      Method &operator*()  { return *current; }
      Method &operator->() { return *current; }

      Method &operator++()
      {
         moveNext();
         return *current;
      }

      // post increment

      Method &operator++(int)
      {
         auto curr = current;
         moveNext();

         return *curr;
      }

   private:
      llvm::StringRef methodName;

      Record *R;
      Method *current;

      short phase;
      std::unordered_multimap<string, Method>::iterator currentBegin;
      std::unordered_multimap<string, Method>::iterator currentEnd;

      void moveNext();
   };

   method_iterator method_begin(llvm::StringRef name)
   {
      return method_iterator(this, name);
   }

   method_iterator method_end()
   {
      return method_iterator();
   }

   void declareMethodAlias(const std::string& name,
                           const std::string& mangledOriginal);

   Method* declareMethod(
      const std::string &methodName,
      const QualType& ret_type,
      AccessModifier access,
      std::vector<Argument>&& args,
      std::vector<TemplateParameter> &&templateParams,
      bool isStatic,
      ast::CallableDecl* declaration,
      SourceLocation loc,
      size_t methodID = 0
   );

   Method* declareInitializer(
      const std::string &methodName,
      AccessModifier access,
      std::vector<Argument>&& args,
      std::vector<TemplateParameter> &&templateParams,
      ast::CallableDecl* declaration,
      size_t methodID = 0
   );

   Method* declareMethod(
      const std::string &methodName,
      const QualType& ret_type,
      OperatorInfo op,
      AccessModifier access,
      std::vector<Argument>&& args,
      std::vector<TemplateParameter> &&templateParams,
      bool isStatic,
      ast::CallableDecl* declaration,
      SourceLocation loc,
      size_t methodID = 0
   );

   Property* declareProperty(
      const std::string &propName,
      const QualType &ty,
      bool isStatic,
      Method *getter,
      Method *setter,
      std::string &&newValName,
      ast::PropDecl *decl
   );

   Field* declareField(
      const string &name,
      QualType type,
      AccessModifier access,
      bool isConst,
      bool isStatic,
      FieldDecl* declaration
   );

   bool hasField(const string &field_name);
   Field* getField(const string &field_name);

   size_t getFieldOffset(const string& fieldName);

   void checkTemplateArgCompatability(
      Method &newMethod
   );

   void checkDuplicateMethodDeclaration(const SourceLocation &loc,
                                        const std::string &name,
                                        const std::string &mangledName);

   Method *getConversionOperator(Type const* toType);
   Method *getComparisonOperator(Type const* withType);

   bool hasMethodWithName(const std::string &name) const;
   bool hasMethodTemplate(const std::string &name) const;

   bool hasProperty(const std::string &name) const;
   Property *getProperty(const std::string &name);

   std::unordered_map<std::string, Property> &getProperties();

   const std::vector<TemplateParameter> &getMethodConstraints(
      const std::string &forMethod);

   bool checkConstraint(
      const ExtensionConstraint& constraint,
      Type*& caller
   ) const;

   const ExtensionConstraint* checkConstraints(
      Method* method,
      Type* caller
   ) const;

   void calculateSize();

   const std::vector<std::shared_ptr<RecordDecl>> &getInstantiations() const
   {
      return Instantiations;
   }

   void addInstantiation(std::shared_ptr<RecordDecl> &&decl)
   {
      Instantiations.push_back(move(decl));
   }

   Record *hasInstantiation(const string &templateArgStr);

   const std::vector<std::shared_ptr<ast::ExtensionDecl>>&
   getExtensions() const
   {
      return extensions;
   }

   void addExtension(std::shared_ptr<ast::ExtensionDecl> ext)
   {
      extensions.push_back(move(ext));
   }

   const std::vector<std::shared_ptr<MethodDecl>> &
   getProtocolMethodInstantiations() const
   {
      return ProtocolMethodInstantiations;
   }

   void addProtocolMethodInstantiation(const std::shared_ptr<MethodDecl> &node)
   {
      ProtocolMethodInstantiations.push_back(node);
   }

   Record *getSpecializedTemplate() const
   {
      return specializedTemplate;
   }

   void setSpecializedTemplate(Record *specializedTemplate)
   {
      Record::specializedTemplate = specializedTemplate;
   }

   const SourceLocation &getInstantiatedFrom() const
   {
      return instantiatedFrom;
   }

   void setInstantiatedFrom(const SourceLocation &instantiatedFrom)
   {
      Record::instantiatedFrom = instantiatedFrom;
   }

   AssociatedType const* getAssociatedType(llvm::StringRef name,
                                           Protocol *P = nullptr) const;

   const std::vector<AssociatedType> &getAssociatedTypes() const
   {
      return associatedTypes;
   }

   AssociatedType &declareAssociatedType(AssociatedTypeDecl *decl)
   {
      associatedTypes.emplace_back(decl);
      return associatedTypes.back();
   }

   AssociatedType &declareAssociatedType(const AssociatedType &AT)
   {
      associatedTypes.emplace_back(AT);
      return associatedTypes.back();
   }

   const std::vector<Field> &getFields() const
   {
      return fields;
   }

   Method *getDeinitializer() const
   {
      return deinitializer;
   }

   void setDeinitializer(Method *deinitializer)
   {
      Record::deinitializer = deinitializer;
   }

   const std::unordered_multimap<size_t, Method *> &
   getDestructuringOperators() const
   {
      return destructuringOperators;
   }

protected:
   static size_t lastRecordID;
   size_t lastMethodID = 1;

   TypeID typeID;
   size_t uses = 0;

   AccessModifier access;
   size_t recordID;

   Record(TypeID id,
          const std::string &name,
          Namespace *NS,
          AccessModifier access,
          std::vector<TemplateParameter> &&templateParams,
          const SourceLocation &loc,
          ast::RecordDecl *decl);

   std::string recordName;
   std::string fullName;

   std::unordered_map<std::string, Property> properties;
   std::vector<Field> fields;
   std::unordered_multimap<string, Method> methods;
   std::vector<AssociatedType> associatedTypes;

   std::vector<std::shared_ptr<ast::ExtensionDecl>> extensions;

   Method *deinitializer = nullptr;

public:
   auto getMethodTemplates(const std::string &name)
      -> decltype(this->methods.equal_range(""));

protected:
   std::set<Protocol*> conformances;

   std::vector<Record*> innerRecords;
   Record *outerRecord = nullptr;

   SourceLocation instantiatedFrom;
   Record *specializedTemplate = nullptr;
   std::vector<TemplateParameter> templateParams;
   sema::TemplateArgList templateArgs;
   std::vector<std::shared_ptr<RecordDecl>> Instantiations;

   std::vector<std::shared_ptr<MethodDecl>> ProtocolMethodInstantiations;

   std::vector<ImplicitConformance> implicitConformances;
   std::vector<std::vector<ExtensionConstraint>> ConstraintSets;

   size_t occupiedBytes = 0;
   unsigned short alignment = 1;
   bool manualAlignment = false;

   bool is_opaque = false;

   SourceLocation loc;
   ast::RecordDecl *decl;
   Namespace *declarationNamespace;

   Method *operatorEquals = nullptr;
   std::unordered_multimap<size_t, Method*> destructuringOperators;

public:
   struct Operator {
      Operator(Associativity assoc,
               int precedence,
               FixKind fix,
               QualType returnType)
         : fixKind((unsigned char)fix), precedenceGroup(precedence, assoc),
           returnType(returnType)
      {}

      void addFix(FixKind fix)
      {
         fixKind |= (unsigned char)fix;
      }

      bool isInfix()  const { return fixKind & (unsigned char)FixKind::Infix; }
      bool isPrefix() const { return fixKind & (unsigned char)FixKind::Prefix; }
      bool isPostfix() const
      {
         return fixKind & (unsigned char)FixKind::Postfix;
      }

      bool hasFix(FixKind fix) const { return fixKind & (unsigned char)fix; }

      QualType getReturnType() const { return returnType; }

      const PrecedenceGroup &getPrecedenceGroup() const
      {
         return precedenceGroup;
      }

   private:
      unsigned char fixKind : 8;
      PrecedenceGroup precedenceGroup;
      QualType returnType;
   };

protected:
   llvm::StringMap<Operator> OperatorPrecedences;

public:
   Operator *getOperatorPrecedence(FixKind fix,
                                   const std::string &op) {
      auto it = OperatorPrecedences.find(op);
      if (it == OperatorPrecedences.end()) {
         return nullptr;
      }

      if (!it->second.hasFix(fix)) {
         return nullptr;
      }

      return &it->second;
   }
};

}
}

#endif //CDOT_RECORD_H
