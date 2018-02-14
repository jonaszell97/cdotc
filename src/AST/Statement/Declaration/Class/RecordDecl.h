//
// Created by Jonas Zell on 03.11.17.
//

#ifndef CDOT_RECORDDECL_H
#define CDOT_RECORDDECL_H

#include <llvm/ADT/SmallPtrSet.h>

#include "../NamedDecl.h"
#include "../../../../Basic/Precedence.h"

namespace cdot {

enum class AccessModifier : unsigned char;

namespace ast {

class FieldDecl;
class PropDecl;
class MethodDecl;
class InitDecl;
class TypedefDecl;
class DeinitDecl;
class TypeRef;
class StaticStmt;
class StaticExpr;
class AssociatedTypeDecl;

class EnumDecl;
class UnionDecl;
class ProtocolDecl;
class ExtensionDecl;

enum class ImplicitConformanceKind : unsigned char {
   StringRepresentable,
   Hashable,
   Equatable
};

struct ImplicitConformance {
   ImplicitConformance(ImplicitConformanceKind kind,
                       MethodDecl *method)
      : kind(kind), method(method) {}

   ImplicitConformanceKind kind;
   MethodDecl *method;
};

class RecordDecl: public NamedDecl, public DeclContext,
                  public llvm::FoldingSetNode {
public:
   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType type)
   {
      switch (type) {
#      define CDOT_RECORD_DECL(Name) \
          case Name##ID:
#      include "../../../AstNode.def"
            return true;
         default:
            return false;
      }
   }

   static bool classof(DeclContext const* T) { return true; }

   static DeclContext *castToDeclContext(RecordDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<RecordDecl*>(D));
   }

   static RecordDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<RecordDecl*>(const_cast<DeclContext*>(Ctx));
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, instantiationInfo->specializedTemplate,
              instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, RecordDecl *Template,
                       sema::TemplateArgList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

   friend class TransformImpl;

   AssociatedTypeDecl* getAssociatedType(llvm::StringRef name,
                                         ProtocolDecl *P = nullptr) const;

   MethodDecl *getConversionOperator(Type const* toType) const;
   MethodDecl *getComparisonOperator(Type const* withType) const;

   bool hasMethodWithName(llvm::StringRef name) const;
   bool hasMethodTemplate(llvm::StringRef name) const;

   PropDecl *getProperty(llvm::StringRef name) const;
   FieldDecl* getField(llvm::StringRef name) const;

   MethodDecl* getMethod(llvm::StringRef name, bool checkParent = true) const;
   MethodDecl* getMethod(size_t id) const;

   MethodDecl* getOwnMethod(llvm::StringRef name);

   RecordDecl *getInnerRecord(llvm::StringRef name) const
   {
      return lookupSingle<RecordDecl>(name);
   }

   bool hasInnerRecord(RecordDecl *R) const
   {
      return innerRecords.find(R) != innerRecords.end();
   }

   bool conformsTo(ProtocolDecl *P) const
   {
      return conformances.find(P) != conformances.end();
   }

   bool conformsToBaseTemplate(ProtocolDecl *P) const;
   void addExtension(ExtensionDecl *E);

   void calculateSize();

   [[nodiscard]]
   DeclContext::AddDeclResultKind addDecl(NamedDecl *decl);

   void addStaticStatement(StaticStmt *stmt)
   {
      staticStatements.push_back(stmt);
   }

protected:
   RecordDecl(NodeType typeID,
              AccessModifier access,
              std::string &&recordName,
              std::vector<TypeRef*> &&conformanceTypes,
              std::vector<StaticExpr*> &&constraints)
      : NamedDecl(typeID, access, move(recordName), move(constraints)),
        DeclContext(typeID),
        conformanceTypes(move(conformanceTypes)),
        manualAlignment(false), opaque(false),
        implicitlyEquatable(false), implicitlyHashable(false),
        implicitlyStringRepresentable(false)
   {}

   static size_t lastRecordID;

   size_t lastMethodID = 1;
   size_t uses = 0;
   size_t recordID;

   std::vector<TypeRef*> conformanceTypes;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<RecordDecl> *instantiationInfo = nullptr;

   std::vector<StaticStmt*> staticStatements;

   llvm::SmallPtrSet<ProtocolDecl*, 4> conformances;
   llvm::SmallPtrSet<RecordDecl*, 4> innerRecords;
   RecordDecl *outerRecord = nullptr;

   DeinitDecl *deinitializer = nullptr;

   size_t occupiedBytes = 0;
   unsigned short alignment = 1;

   bool manualAlignment : 1;
   bool opaque          : 1;
   bool implicitlyEquatable : 1;
   bool implicitlyHashable : 1;
   bool implicitlyStringRepresentable : 1;

   MethodDecl *operatorEquals = nullptr;
   MethodDecl *hashCodeFn = nullptr;
   MethodDecl *toStringFn = nullptr;

   std::unordered_multimap<size_t, MethodDecl*> destructuringOperators;

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

   void declareOperator(MethodDecl* M);

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

   void removeCastOperators()
   {
      namedDecls.erase("infix as");
   }

   bool isStruct() const;
   bool isClass() const;
   bool isEnum() const;
   bool isUnion() const;
   bool isProtocol() const;
   bool isRawEnum() const;

   const std::vector<TypeRef*> &getConformanceTypes() const
   {
      return conformanceTypes;
   }

   const llvm::SmallPtrSet<ProtocolDecl *, 4> &getConformances() const
   {
      return conformances;
   }

   void addConformance(ProtocolDecl *P)
   {
      conformances.insert(P);
   }

   RecordDecl *getOuterRecord() const
   {
      return outerRecord;
   }

   void setOuterRecord(RecordDecl *outerRecord)
   {
      RecordDecl::outerRecord = outerRecord;
   }

   const llvm::SmallPtrSet<RecordDecl *, 4> &getInnerRecords() const
   {
      return innerRecords;
   }

   void addInnerRecord(RecordDecl *R);

   const std::vector<StaticStmt*> &getStaticStatements() const
   {
      return staticStatements;
   }

   DeinitDecl *getDeinitializer() const
   {
      return deinitializer;
   }

   const std::unordered_multimap<size_t, MethodDecl*> &
   getDestructuringOperators() const
   {
      return destructuringOperators;
   }

   bool isTemplate() const
   {
      if (NamedDecl::isTemplate())
         return true;

      return outerRecord && outerRecord->isTemplate();
   }

   void setInstantiationInfo(InstantiationInfo<RecordDecl> *instantiationInfo)
   {
      RecordDecl::instantiationInfo = instantiationInfo;
   }

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      RecordDecl::templateParams = move(templateParams);
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   bool isInstantiation() const
   {
      return instantiationInfo != nullptr;
   }

   RecordDecl *getSpecializedTemplate() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return instantiationInfo->specializedTemplate;
   }

   const SourceLocation &getInstantiatedFrom() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return instantiationInfo->instantiatedFrom;
   }

   sema::TemplateArgList const& getTemplateArgs() const
   {
      assert(isInstantiation() && "not a record instantiation!");
      return instantiationInfo->templateArgs;
   }

   sema::ResolvedTemplateArg const* getTemplateArg(llvm::StringRef name) const;

   size_t getUses() const
   {
      return uses;
   }

   size_t getRecordID() const
   {
      return recordID;
   }

   size_t getSize() const
   {
      return occupiedBytes;
   }

   unsigned short getAlignment() const
   {
      return alignment;
   }

   bool hasManualAlignment() const
   {
      return manualAlignment;
   }

   void setAlignment(unsigned short align)
   {
      alignment = align;
      manualAlignment = true;
   }

   bool isOpaque() const
   {
      return opaque;
   }

   void setOpaque(bool opaque)
   {
      RecordDecl::opaque = opaque;
   }

   MethodDecl *getOperatorEquals() const
   {
      return operatorEquals;
   }

   void setOperatorEquals(MethodDecl *operatorEquals)
   {
      RecordDecl::operatorEquals = operatorEquals;
   }

   MethodDecl *getHashCodeFn() const
   {
      return hashCodeFn;
   }

   void setHashCodeFn(MethodDecl *hashCodeFn)
   {
      RecordDecl::hashCodeFn = hashCodeFn;
   }

   MethodDecl *getToStringFn() const
   {
      return toStringFn;
   }

   void setToStringFn(MethodDecl *toStringFn)
   {
      RecordDecl::toStringFn = toStringFn;
   }

   llvm::StringRef getOwnName() const;

   int getNameSelector() const;

   bool isImplicitlyEquatable() const
   {
      return implicitlyEquatable;
   }

   void setImplicitlyEquatable(bool implicitlyEquatable)
   {
      RecordDecl::implicitlyEquatable = implicitlyEquatable;
   }

   bool isImplicitlyHashable() const
   {
      return implicitlyHashable;
   }

   void setImplicitlyHashable(bool implicitlyHashable)
   {
      RecordDecl::implicitlyHashable = implicitlyHashable;
   }

   bool isImplicitlyStringRepresentable() const
   {
      return implicitlyStringRepresentable;
   }

   void setImplicitlyStringRepresentable(bool implicitlyStringRepresentable)
   {
      RecordDecl::implicitlyStringRepresentable = implicitlyStringRepresentable;
   }
};

struct FunctionTemplateInstantiation;

class StructDecl: public RecordDecl {
public:
   StructDecl(AccessModifier access,
              std::string &&recordName,
              std::vector<TypeRef*> &&conformanceTypes,
              std::vector<StaticExpr*> &&constraints)
      : RecordDecl(StructDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

   InitDecl *getParameterlessConstructor() const
   {
      return parameterlessConstructor;
   }

   void setParameterlessConstructor(InitDecl *parameterlessConstructor)
   {
      StructDecl::parameterlessConstructor = parameterlessConstructor;
   }

   InitDecl *getMemberwiseInitializer() const
   {
      return memberwiseInitializer;
   }

   void setMemberwiseInitializer(InitDecl *memberwiseInitializer)
   {
      StructDecl::memberwiseInitializer = memberwiseInitializer;
   }

   InitDecl *getDefaultInitializer() const
   {
      return defaultInitializer;
   }

   void setDefaultInitializer(InitDecl *defaultInitializer)
   {
      StructDecl::defaultInitializer = defaultInitializer;
   }

   llvm::ArrayRef<FieldDecl*> getFields() const
   {
      return fields;
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      switch (kind) {
         case StructDeclID:
         case ClassDeclID:
            return true;
         default:
            return false;
      }
   }

   static DeclContext *castToDeclContext(StructDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<StructDecl*>(D));
   }

   static StructDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<StructDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class RecordDecl; // for access to fields

protected:
   StructDecl(NodeType typeID,
              AccessModifier access,
              std::string &&recordName,
              std::vector<TypeRef*> &&conformanceTypes,
              std::vector<StaticExpr*> &&constraints)
      : RecordDecl(typeID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

   std::vector<FieldDecl*> fields;

   InitDecl* parameterlessConstructor = nullptr;
   InitDecl* memberwiseInitializer    = nullptr;
   InitDecl* defaultInitializer       = nullptr;
};

class ClassDecl: public StructDecl {
public:
   ClassDecl(AccessModifier access,
             std::string &&recordName,
             std::vector<TypeRef*> &&conformanceTypes,
             std::vector<StaticExpr*> &&constraints,
             TypeRef *parentClass,
             bool isAbstract)
      : StructDecl(ClassDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints)),
        parentType(parentClass), is_abstract(isAbstract)
   {}

   TypeRef* getParentType() const
   {
      return parentType;
   }

   ClassDecl *getParentClass() const
   {
      return parentClass;
   }

   void setParentClass(ClassDecl *parentClass)
   {
      ClassDecl::parentClass = parentClass;
   }

   bool isBaseClassOf(ClassDecl const* C) const
   {
      auto Outer = C->getParentClass();
      while (Outer) {
         if (Outer == this)
            return true;

         Outer = Outer->getParentClass();
      }

      return false;
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == ClassDeclID;
   }

   static DeclContext *castToDeclContext(ClassDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<ClassDecl*>(D));
   }

   static ClassDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<ClassDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;

protected:
   TypeRef* parentType = nullptr;
   ClassDecl *parentClass = nullptr;

   bool is_abstract = false;

public:
   bool isAbstract() const { return is_abstract; }
};

class EnumCaseDecl;

class EnumDecl: public RecordDecl {
public:
   EnumDecl(AccessModifier access,
            std::string &&recordName,
            std::vector<TypeRef*> &&conformanceTypes,
            std::vector<StaticExpr*> &&constraints,
            TypeRef *rawType)
      : RecordDecl(EnumDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints)),
        rawType(rawType)
   {}

   EnumCaseDecl *hasCase(llvm::StringRef name);

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == EnumDeclID;
   }

   static DeclContext *castToDeclContext(EnumDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<EnumDecl*>(D));
   }

   static EnumDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<EnumDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;
   friend class RecordDecl; // for maxAssociatedTypes, cases

protected:
   TypeRef* rawType;
   size_t maxAssociatedTypes = 0;
   std::vector<EnumCaseDecl*> cases;

public:
   TypeRef* getRawType() const
   {
      return rawType;
   }

   void setRawType(TypeRef* rawType)
   {
      EnumDecl::rawType = rawType;
   }

   size_t getMaxAssociatedTypes() const
   {
      return maxAssociatedTypes;
   }

   const std::vector<EnumCaseDecl *> &getCases() const
   {
      return cases;
   }
};

class UnionDecl: public RecordDecl {
public:
   UnionDecl(AccessModifier access,
             std::string &&recordName,
             std::vector<TypeRef*> &&conformanceTypes,
             std::vector<StaticExpr*> &&constraints)
      : RecordDecl(UnionDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

   bool isConst() const { return is_const; }
   void isConst(bool is_const) { this->is_const = is_const; }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == UnionDeclID;
   }

   static DeclContext *castToDeclContext(UnionDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<UnionDecl*>(D));
   }

   static UnionDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<UnionDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;

protected:
   bool is_const = false;
};

class ProtocolDecl: public RecordDecl {
public:
   ProtocolDecl(AccessModifier access,
                std::string &&recordName,
                std::vector<TypeRef*> &&conformanceTypes,
                std::vector<StaticExpr*> &&constraints)
      : RecordDecl(ProtocolDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == ProtocolDeclID;
   }

   static DeclContext *castToDeclContext(ProtocolDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<ProtocolDecl*>(D));
   }

   static ProtocolDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<ProtocolDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;
};

class ExtensionDecl: public RecordDecl {
public:
   ExtensionDecl(AccessModifier access,
                 std::string &&recordName,
                 std::vector<TypeRef*> &&conformanceTypes,
                 std::vector<StaticExpr*> &&constraints)
      : RecordDecl(ExtensionDeclID, access, move(recordName),
                   move(conformanceTypes), move(constraints))
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == ExtensionDeclID;
   }

   static DeclContext *castToDeclContext(ExtensionDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<ExtensionDecl*>(D));
   }

   static ExtensionDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<ExtensionDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_RECORDDECL_H
