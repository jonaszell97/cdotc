#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Basic/DependencyGraph.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Log.h"
#include "cdotc/Support/SaveAndRestore.h"
#include "cdotc/Support/Timer.h"

#include <llvm/ADT/SetVector.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using std::string;
using llvm::SmallSetVector;

namespace {

struct DependencyNode : public llvm::FoldingSetNode {
   enum DependencyNodeKind : unsigned {
      /// A base node needed as an origin to add dependencies to.
      Complete,

      /// We need to know all conformances of a record.
      ConformancesOfRecord,

      /// We need to know whether or not a record conforms to some other record.
      SpecificConformance,

      /// We need to know the concrete type of an associated type.
      ConcreteTypeOfAssociatedType,

      /// We need to know the concrete type of a nested associated type.
      NestedAssociatedType,

      /// We need to know whether or not an extension applies to a record.
      ExtensionApplicability,

      /// We need to know whether or not a constraint set is satisfied.
      ConstraintsSatisfied,
   };

   /// The node kind.
   DependencyNodeKind Kind;

   /// The base declaration.
   NamedDecl *Decl;

   /// The potential other declaration.
   void *Data;

   /// Whether or not this node is completed.
   bool Done;

   DependencyNode(DependencyNodeKind K, NamedDecl *Decl,
                  void *Data = nullptr)
       : Kind(K), Decl(Decl), Data(Data), Done(false)
   {

   }

   NamedDecl *getOtherDecl() const
   {
      assert(Kind == SpecificConformance
             || Kind == ConcreteTypeOfAssociatedType
             || Kind == ExtensionApplicability);

      return static_cast<NamedDecl*>(Data);
   }

   ExtensionDecl *getExtension() const
   {
      assert(Kind == ExtensionApplicability);
      return static_cast<ExtensionDecl*>(Data);
   }

   ProtocolDecl *getProtocol() const
   {
      assert(Kind == SpecificConformance);
      return static_cast<ProtocolDecl*>(Data);
   }

   ConstraintSet *getConstraints() const
   {
      assert(Kind == ConstraintsSatisfied);
      return static_cast<ConstraintSet*>(Data);
   }

   QualType getType() const
   {
      assert(Kind == NestedAssociatedType);
      return QualType::getFromOpaquePtr(Data);
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       DependencyNodeKind K,
                       NamedDecl *Decl, void *Data)
   {
      ID.AddInteger(K);
      ID.AddPointer(Decl);
      ID.AddPointer(Data);
   }

   void Profile(llvm::FoldingSetNodeID &ID) const
   {
      Profile(ID, Kind, Decl, Data);
   }

#ifndef NDEBUG
   void print(llvm::raw_ostream &OS) const
   {
      switch (Kind) {
      case Complete:
         OS << Decl->getFullName();
         break;
      case ConformancesOfRecord:
         OS << "All conformances of " << Decl->getFullName();
         break;
      case SpecificConformance:
         OS << "Conformance of " << Decl->getFullName()
            << " to " << getProtocol()->getFullName();
         break;
      case ConcreteTypeOfAssociatedType:
         OS << "Concrete type of associated type " << getOtherDecl()->getFullName()
            << " For " << Decl->getFullName();
         break;
      case NestedAssociatedType:
         OS << "Concrete type of nested associated type " << getType()->toDiagString()
            << " For " << Decl->getFullName();
         break;
      case ExtensionApplicability:
         OS << "Applicability of extension [" << getExtension()->getFullSourceLoc()
            << "] for " << Decl->getFullName();
         break;
      case ConstraintsSatisfied: {
         OS << "Constraint set [";
         getConstraints()->print(OS);
         OS << "] satisfied for " << Decl->getFullName();
         break;
      }
      }

      if (Done) {
         OS << " (✔️)";
      }
   }

   std::string to_string() const
   {
      std::string res;
      {
         llvm::raw_string_ostream OS(res);
         print(OS);
      }

      return res;
   }

   void dump() const { print(llvm::errs()); }
#endif
};

struct UncheckedConformance {
   using ReadyKind = ReferencedAssociatedTypesReadyQuery::ResultKind;

   /// \brief The protocol that this conformance introduces.
   ProtocolDecl* proto;

   /// \brief The constraints that were placed on the extension
   /// that introduced this conformance.
   ConstraintSet* constraints;

   /// \brief The extension that introduced the conformance.
   DeclContext* introducedBy;

   /// \brief The (conditional) conformances that are introduced if this
   /// conformance applies.
   std::unique_ptr<std::vector<UncheckedConformance>> innerConformances;

   /// \brief The combined constraint set of all outer conformances.
   ConstraintSet* combinedConstraints;

   /// \brief The unique hash value of this conditional conformance.
   uintptr_t hashVal;

   /// \brief Set to true once the associated types introduced by this
   /// constraint are ready.
   ReadyKind associatedTypeStatus;

   /// \brief The 'depth' of the conformance, i.e. how many layers of
   /// protocol conformances we had to go through to find it.
   int depth;

   /// \brief Set to true once this conformance and all of its children are
   /// fully checked.
   bool done;

   /// Reference to the outer conformance.
   UncheckedConformance *outer;

   /// \brief This conditional conformance only exists for displaying it in the
   /// hierarchy, but should otherwise be ignored.
   bool exclude = false;

   /// \brief Memberwise C'tor.
   explicit UncheckedConformance(ASTContext& C, ProtocolDecl* proto = nullptr,
                                 ConstraintSet* constraints = nullptr,
                                 DeclContext* introducedBy = nullptr,
                                 UncheckedConformance* outer = nullptr)
       : proto(proto), constraints(constraints),
         introducedBy(introducedBy ? introducedBy : (outer ? outer->introducedBy : nullptr)),
         combinedConstraints(nullptr),
         associatedTypeStatus(ReadyKind::NotReady),
         depth(outer ? outer->depth + 1 : -1), done(false), outer(outer)
   {
      assert((!constraints || constraints->empty() || introducedBy)
             && "constrained conformance must be introduced somewhere!");

      uintptr_t outerHashVal = 0;
      if (outer) {
         outerHashVal = outer->hashVal;
      }

      // For uniquing purposes it doesn't matter which extension this
      // conformance was introduced by.
      hashVal = llvm::hash_combine((uintptr_t)proto, (uintptr_t)constraints,
                                   outerHashVal);

      if (outer) {
         exclude |= outer->exclude;
         if (constraints) {
            combinedConstraints = ConstraintSet::Combine(
                C, constraints, outer->combinedConstraints);
         }
         else {
            combinedConstraints = outer->combinedConstraints;
         }
      }
      else {
         combinedConstraints = constraints;
      }
   }

   /// \return The child conformance at index \param i.
   UncheckedConformance& operator[](size_t i)
   {
      assert(innerConformances && innerConformances->size() > i
             && "index out of bounds!");

      return innerConformances->at(i);
   }

   /// \return The number of child conformances.
   size_t size() const
   {
      return innerConformances ? innerConformances->size() : 0;
   }

   /// \brief Initialize the innerConformances pointer.
   void initializerInnerConformances()
   {
      innerConformances
          = std::make_unique<std::vector<UncheckedConformance>>();
   }

   /// \brief Find an associated type with a default value.
   void FindWithDefault(DeclarationName DN, llvm::SmallDenseSet<AssociatedTypeDecl*, 2> &Defaults)
   {
      if (proto) {
         for (auto *AT : proto->getDecls<AssociatedTypeDecl>()) {
            if (AT->getDeclName() == DN && AT->getDefaultType()) {
               Defaults.insert(AT);
            }
         }
      }

      if (innerConformances == nullptr) {
         return;
      }

      for (auto &inner : *innerConformances) {
         inner.FindWithDefault(DN, Defaults);
      }
   }

   /// Recursively iterate through all conformances.
   template<class Fn>
   bool ForEach(const Fn &fn)
   {
      if (proto != nullptr) {
         if (!fn(*this)) {
            return false;
         }
      }

      if (innerConformances == nullptr) {
         return true;
      }

      for (auto &inner : *innerConformances) {
         if (!inner.ForEach(fn)) {
            return false;
         }
      }

      return true;
   }

#ifndef NDEBUG
   struct PrintHelper {
      const UncheckedConformance& conf;
      int indent;

      void print(llvm::raw_ostream& OS) const { conf.print(OS, indent); }
   };

   PrintHelper indented(int indent = 3) const
   {
      return PrintHelper{*this, indent};
   }

   void dump() const { print(llvm::errs()); }

   void print(llvm::raw_ostream& OS, int indent = 0) const
   {
      llvm::SmallPtrSet<uintptr_t, 4> visited;
      print(visited, OS, indent);
   }

   void print(llvm::SmallPtrSetImpl<uintptr_t>& visited,
              llvm::raw_ostream& OS, int indent = 0) const
   {
      int indentIncrease = 0;
      if (proto && visited.insert(hashVal).second) {
         indentIncrease = 3;
         applyIndent(OS, indent);

         if (exclude)
            OS << "(";

         OS << proto->getDeclName();
         if (constraints && !constraints->empty()) {
            OS << " (where ";
            constraints->print(OS);
            OS << ")";
         }
         if (introducedBy) {
            OS << " [" << cast<NamedDecl>(introducedBy)->getFullSourceLoc() << "]";
         }

         if (exclude)
            OS << ")";

         OS << " [" << depth << "]";
         OS << "\n";
      }

      if (innerConformances) {
         for (auto& inner : *innerConformances) {
            inner.print(visited, OS, indent + indentIncrease);
         }
      }
   }

private:
   static void applyIndent(llvm::raw_ostream& OS, int indent)
   {
      for (int i = 0; i < indent; ++i)
         OS << ' ';
   }
#endif
};

} // anonymous namespace

class cdot::ConformanceResolver {
   using AssociatedTypeImplMap = llvm::DenseMap<
       RecordDecl*, llvm::DenseMap<
           AssociatedTypeDecl*, llvm::SetVector<std::pair<NamedDecl*, ConstraintSet*>>>>;

   /// Reference to the query context.
   QueryContext &QC;

   /// Reference to the conformance table.
   ConformanceTable &ConfTbl;

   /// Set of all encountered decl contexts.
   std::vector<DeclContext*> DeclContexts;

   /// The cache of conditional conformances.
   std::vector<std::unique_ptr<UncheckedConformance>> Cache;

   /// Map of current base conformances.
   llvm::DenseMap<RecordDecl*, int> BaseConformances;

   /// Set of records and protocols to which they might conform.
   llvm::DenseSet<std::pair<RecordDecl*, ProtocolDecl*>> PotentialConformances;

   /// The DeclContexts we have already added to the worklist.
   llvm::DenseSet<DeclContext*> DoneSet;

   /// The record decls that are fully resolved.
   llvm::DenseSet<RecordDecl*> CompletedRecordDecls;

   /// The record decls that need to have their conformances checked.
   llvm::SetVector<RecordDecl*> PendingRecordDecls;

   /// Allocator used for creating dependency nodes.
   llvm::BumpPtrAllocator Allocator;

   /// The protocol conformance dependency graph.
   DependencyGraph<ProtocolDecl*> ConformanceDAG;

   /// The resolution dependency graph.
   DependencyGraph<DependencyNode*> Dependencies;

   /// The nodes of the dependency graph.
   llvm::FoldingSet<DependencyNode> DependencyNodes;

   /// Possible associated type implementations for each record.
   AssociatedTypeImplMap PossibleAssociatedTypeImpls;

   /// Set of implemented associated types.
   llvm::DenseMap<std::pair<RecordDecl*, IdentifierInfo*>, AliasDecl*> ImplementedATs;

   /// Number of incoming constraints per dependency node at the time of
   /// constructing the resolution order.
   llvm::DenseMap<DependencyNode*, int> NumDependencies;

   bool PrepareMacros(DeclContext *DC);
   bool PrepareStaticDecls(DeclContext *DC);
   bool PrepareImports(DeclContext *DC);
   bool PrepareImplicitDecls(DeclContext *DC);

   bool FindDependencies(RecordDecl *R, UncheckedConformance &baseConf);
   bool FindDependencies(RecordDecl *R, UncheckedConformance &baseConf,
                         DependencyGraph<DependencyNode*>::Vertex &CompleteNode);
   bool FindLayoutDependencies(RecordDecl *R,
                               DependencyGraph<DependencyNode*>::Vertex &CompleteNode);

   bool ResolveDependencyNode(DependencyNode *Node,
                              bool &error, bool &shouldReset);

   bool HasNewDependencies(DependencyNode *Node)
   {
      auto &Vert = Dependencies.getOrAddVertex(Node);
      return Vert.getIncoming().size() > NumDependencies[Node];
   }

   bool CreateUncheckedConformanceForImportedDecl(RecordDecl *R);
   bool ResolveConformanceDependencies();

   friend class SemaPass;

public:
   using DependencyVertex = DependencyGraph<DependencyNode*>::Vertex;

   /// C'tor.
   explicit ConformanceResolver(QueryContext &QC)
       : QC(QC), ConfTbl(QC.Context.getConformanceTable()),
         ConformanceDAG(Allocator), Dependencies(Allocator)
   {}

   UncheckedConformance &CreateCondConformance(ProtocolDecl* proto = nullptr,
                                               ConstraintSet* constraints = nullptr,
                                               DeclContext* introducedBy = nullptr,
                                               UncheckedConformance* outer = nullptr)
   {
      Cache.emplace_back(std::make_unique<UncheckedConformance>(
          QC.Context, proto, constraints, introducedBy, outer));

      return *Cache.back();
   }

   DependencyVertex&
   GetDependencyNode(DependencyNode::DependencyNodeKind K,
                     NamedDecl *Decl, void *Data = nullptr,
                     bool *isNew = nullptr)
   {
      llvm::FoldingSetNodeID ID;
      DependencyNode::Profile(ID, K, Decl, Data);

      void *InsertPos;
      if (auto *Ptr = DependencyNodes.FindNodeOrInsertPos(ID, InsertPos)) {
         return Dependencies.getOrAddVertex(Ptr);
      }

      if (isNew)
         *isNew = true;

      auto *Node = new(QC.Context) DependencyNode(K, Decl, Data);
      if (Decl->isExternal()) {
         Node->Done = true;
      }

      DependencyNodes.InsertNode(Node, InsertPos);
      return Dependencies.getOrAddVertex(Node);
   }

   bool AddConformanceDependency(DependencyVertex &Node,
                                 RecordDecl *R, ProtocolDecl *P);

   bool FindDeclContexts(DeclContext *DC, bool includeFunctions);
   bool ResolveDependencyGraph();

   bool BuildWorklist();
   bool BuildWorklist(DeclContext *DC);
   bool BuildWorklistForShallowInstantiation(RecordDecl *Inst);

   bool IsBeingResolved(RecordDecl *R)
   {
      if (R->isExternal()) {
         return false;
      }

      return CompletedRecordDecls.count(R) == 0;
   }

   bool registerImplicitConformances(RecordDecl *R);

   bool registerConformance(CanType Self, ProtocolDecl* proto,
                            ConstraintSet* constraints,
                            SmallDenseSet<uintptr_t, 4>& testSet,
                            SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                            DeclContext* introducedBy,
                            UncheckedConformance& outer,
                            bool isDirect = false);

   bool registerDeclaredConformances(CanType Self, CanType protoTy,
                                     MutableArrayRef<SourceType> conformanceTypes,
                                     ConstraintSet* constraints,
                                     SmallDenseSet<uintptr_t, 4>& testSet,
                                     SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                                     DeclContext* introducedBy, UncheckedConformance& newConfRef);

   bool
   registerConformances(CanType Self, DeclContext* DC,
                        SmallDenseSet<uintptr_t, 4>& testSet,
                        SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                        UncheckedConformance& baseConf);


   ConstraintSet* getDependentConstraints(ConstraintSet* CS, QualType Self);
   bool isTemplateMember(NamedDecl* impl);
};

namespace {

struct PendingConformanceCheck {
   ProtocolDecl* conformance;
   ConstraintSet* constraints;
   DeclContext* introducedIn;
   NamedDecl* requirement;

   PendingConformanceCheck(ProtocolDecl* conformance,
                           ConstraintSet* constraints = nullptr,
                           DeclContext* introducedIn = nullptr,
                           NamedDecl* requirement = nullptr)
       : conformance(conformance), constraints(constraints),
         introducedIn(introducedIn), requirement(requirement)
   {
   }

   bool operator==(const PendingConformanceCheck& RHS) const
   {
      return conformance == RHS.conformance
         && constraints == RHS.constraints
         && requirement == RHS.requirement;
   }

   bool operator!=(const PendingConformanceCheck& RHS) const
   {
      return !(*this == RHS);
   }
};

} // anonymous namespace

namespace llvm {

template<> struct DenseMapInfo<::PendingConformanceCheck> {
   static PendingConformanceCheck getEmptyKey()
   {
      return PendingConformanceCheck(nullptr, nullptr);
   }

   static PendingConformanceCheck getTombstoneKey()
   {
      return PendingConformanceCheck(reinterpret_cast<ProtocolDecl*>(-1),
                                     nullptr);
   }

   static int getHashValue(const PendingConformanceCheck& P)
   {
      return hash_combine(P.conformance, P.constraints, P.requirement);
   }

   static bool isEqual(const PendingConformanceCheck& LHS,
                       const PendingConformanceCheck& RHS)
   {
      return LHS == RHS;
   }
};

} // namespace llvm

namespace cdot {
namespace sema {

class AssociatedTypeSubstVisitor
    : public TypeBuilder<AssociatedTypeSubstVisitor> {
   RecordDecl* R;
   NamedDecl* ND = nullptr;

public:
   explicit AssociatedTypeSubstVisitor(SemaPass& SP, RecordDecl* R)
       : TypeBuilder(SP, R->getSourceLoc()), R(R)
   {
   }

   void setLookupDecl(NamedDecl* ND) { this->ND = ND; }
   RecordDecl* getRecord() const { return R; }

   QualType visitBuiltinType(BuiltinType* Ty)
   {
      if (Ty->isSelfTy())
         return SP.getContext().getRecordType(R);

      return Ty;
   }

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& Types)
   {
      Types.push_back(visitTemplateParamType(T));
   }

   QualType visitTemplateParamType(TemplateParamType* Ty)
   {
      if (!ND)
         return Ty;

      // Template argument types do not need to be equal, just equivalent.
      auto* Param = Ty->getParam();
      auto Idx = Param->getIndex();

      if (ND->getTemplateParams().size() <= Idx)
         return Ty;

      auto* OtherParam = ND->getTemplateParams()[Idx];
      if (SP.equivalent(Param, OtherParam)) {
         return SP.getContext().getTemplateArgType(OtherParam);
      }

      return Ty;
   }
};

class ConformanceCheckerImpl {
public:
   ConformanceCheckerImpl(SemaPass& SP, RecordDecl *R)
       : Sema(SP), SelfTy(SP.Context.getRecordType(R)), Rec(R),
         TypeSubstVisitor(SP, R)
   {
   }

   void checkConformance();
   void checkSingleConformance(ProtocolDecl* P);

   struct MethodCandidate {
      MessageKind Msg;
      SourceRange SR;

      uintptr_t Data1;
      uintptr_t Data2;
      uintptr_t Data3;
      uintptr_t Data4;
   };

   bool IssuedError = false;
   SmallSetVector<NamedDecl*, 4> ExtensionDecls;

private:
   SemaPass& Sema;

   QualType SelfTy;
   RecordDecl *Rec;
   AssociatedTypeSubstVisitor TypeSubstVisitor;

   SmallSetVector<NamedDecl*, 4> DefaultImpls;
   SmallSetVector<PendingConformanceCheck, 4> DelayedChecks;
   SmallSetVector<PendingConformanceCheck, 4> CheckedConformanceSet;

   SmallSetVector<std::pair<NamedDecl*, int>, 4> ApplicableDefaultImpls;

   /// Cache for lookup results, stored here for efficiency.
   SmallVector<NamedDecl*, 8> FoundImpls;
   SmallPtrSet<NamedDecl*, 8> TestSet;

   bool FoundChanges = true;

   bool FindImplementations(RecordDecl *Rec, DeclarationName Name,
                            PendingConformanceCheck &Conf);

   void checkRecordCommon(RecordDecl* Rec, PendingConformanceCheck& Conf);
   void scanApplicableExtension(RecordDecl* Rec, PendingConformanceCheck& Conf,
                                ExtensionDecl* Ext, Conformance *C);

   bool checkAssociatedType(RecordDecl* Rec, PendingConformanceCheck& Conf,
                            AssociatedTypeDecl* AT);

   bool checkSingleDecl(RecordDecl* Rec, PendingConformanceCheck& Conf,
                        NamedDecl* Req);

   NamedDecl* checkSingleDeclImpl(RecordDecl* Rec,
                                  PendingConformanceCheck& Conf,
                                  NamedDecl* Req);

   NamedDecl* checkPropImpl(RecordDecl* Rec,
                            PendingConformanceCheck& Conf,
                            PropDecl* Req);

   NamedDecl* checkSubscriptImpl(RecordDecl* Rec,
                                 PendingConformanceCheck& Conf,
                                 SubscriptDecl* Req);

   NamedDecl* checkMethodImpl(RecordDecl* Rec,
                              PendingConformanceCheck& Conf,
                              MethodDecl* Req);

   NamedDecl* checkInitImpl(RecordDecl* Rec,
                            PendingConformanceCheck& Conf,
                            InitDecl* Req);

   bool CompareArgumentLists(
      MethodDecl *Req, MethodDecl *Impl,
      ProtocolDecl *Proto, std::vector<MethodCandidate> &Candidates);

   void inheritAttributes(NamedDecl* Req, NamedDecl* Impl);

   bool checkTypeCompatibility(QualType given, SourceType& needed,
                               NamedDecl* LookupDecl, ProtocolDecl *Proto);

   MethodDecl* checkIfImplicitConformance(RecordDecl* Rec, ProtocolDecl* Proto,
                                          MethodDecl& M);

   NamedDecl* checkIfProtocolDefaultImpl(RecordDecl* Rec, ProtocolDecl* Proto,
                                         NamedDecl* D,
                                         PendingConformanceCheck &Conf);

   void addProtocolImpl(RecordDecl* R, NamedDecl* Req, NamedDecl* Impl);
   void genericError(RecordDecl* Rec, ProtocolDecl* P);
};

void ConformanceCheckerImpl::genericError(RecordDecl* Rec, ProtocolDecl* P)
{
   if (IssuedError)
      return;

   IssuedError = true;
   Sema.diagnose(Rec, err_incorrect_protocol_impl, Rec, Rec->getDeclName(),
                 P->getDeclName(), Rec->getSourceLoc());
}

bool ConformanceCheckerImpl::checkTypeCompatibility(QualType given,
                                                    SourceType& needed,
                                                    NamedDecl* LookupDecl,
                                                    ProtocolDecl* P)
{
   QualType Self(Sema.Context.getRecordType(LookupDecl->getRecord()));
   bool Equivalent;
   if (Sema.QC.CheckTypeEquivalence(Equivalent, needed, given, Self, P, P)) {
      return true;
   }

   if (!Equivalent) {
      given = Sema.ApplyCapabilities(given, LookupDecl->getDeclContext());
      if (Sema.QC.CheckTypeEquivalence(Equivalent, needed, given, Self, P, P)) {
         return true;
      }
   }

   return Equivalent;
}

static DeclarationName getNameForProtocolExtensionDecl(ASTContext& C,
                                                       NamedDecl* D,
                                                       RecordDecl* Inst)
{
   auto DN = D->getDeclName();
   switch (DN.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      return C.getDeclNameTable().getConstructorName(
         C.getRecordType(Inst),
         DN.getKind() == DeclarationName::ConstructorName);
   case DeclarationName::DestructorName:
      return C.getDeclNameTable().getDestructorName(C.getRecordType(Inst));
   default:
      return DN;
   }
}

void ConformanceCheckerImpl::checkConformance()
{
   if (isa<ProtocolDecl>(Rec)) {
      return;
   }

   if (Rec->isInvalid()) {
      return;
   }

   SemaPass::DeclScopeRAII declScopeRAII(Sema, Rec);

   // Check which builtin conformances apply.
   if (Sema.QC.CheckBuiltinConformances(Rec)) {
      return;
   }

   // Note the added builtin conformances for future instantiations.
   if (Rec->isTemplateOrInTemplate()) {
      Sema.getConformanceResolver().registerImplicitConformances(Rec);
   }

   auto Conformances
       = Sema.getContext().getConformanceTable().getAllConformances(Rec);

   // Since extensions can only add declarations, check them first.
   SmallVector<PendingConformanceCheck, 4> AllConformances;
   for (auto Conf : Conformances) {
      auto* Proto = Conf->getProto();

      ConstraintSet *CS = Conf->getConstraints();
      if (CS == nullptr) {
         CS = Sema.Context.EmptyConstraintSet;
      }

      AllConformances.emplace_back(Proto, CS, Conf->getDeclarationCtx());

      // Check applicable extensions.
      for (auto* Ext : Proto->getExtensions()) {
         using ResultKind
             = CheckProtocolExtensionApplicabilityQuery::ResultKind;

         ResultKind Applicability;
         if (Sema.QC.CheckProtocolExtensionApplicability(Applicability,
                                                         Rec->getType(), Ext)) {
            return;
         }

         switch (Applicability) {
         case ResultKind::DoesNotApply:
            continue;
         case ResultKind::DoesApply:
         case ResultKind::CantTell:
            break;
         }

         if (Sema.QC.PrepareDeclInterface(Ext)) {
            continue;
         }

         // Remember which default implementations and other declarations are
         // provided by this extension.
         scanApplicableExtension(Rec, AllConformances.back(), Ext, Conf);
      }
   }

   // Now resolve all protocol requirements, since at this point no additional
   // conformances can be added.
   for (auto& Conf : AllConformances) {
      checkRecordCommon(Rec, Conf);
   }

   // If we already issued an error, stop here.
   if (IssuedError) {
      return;
   }

   // Now try the requirements we couldn't resolve before until either all
   // are resolved or there are no more changes.
   while (true) {
      FoundChanges = false;

      auto LocalDelayedChecks = move(DelayedChecks);
      DelayedChecks.clear();

      for (PendingConformanceCheck& Check : LocalDelayedChecks.takeVector()) {
         checkSingleDecl(Rec, Check, Check.requirement);
      }

      if (!FoundChanges) {
         break;
      }
   }

   for (auto *ND : ExtensionDecls) {
      NamedDecl *Equiv;
      if (Sema.QC.FindEquivalentDecl(Equiv, ND, Rec, SelfTy)) {
         continue;
      }

      // Declaration already implemented.
      if (Equiv) {
         continue;
      }

      // Make the declaration visible, but delay instantiation until it's
      // actually referenced.
      Sema.makeDeclAvailable(
         *Rec, getNameForProtocolExtensionDecl(Sema.Context, ND, Rec), ND);
   }
}

void ConformanceCheckerImpl::checkSingleConformance(ProtocolDecl* P)
{
   SemaPass::DeclScopeRAII declScopeRAII(Sema, Rec);

   if (Sema.QC.FindExtensions(Sema.Context.getRecordType(P))) {
      return;
   }

   Conformance *C = Sema.Context.getConformanceTable().getConformance(Rec, P);
   PendingConformanceCheck Conf(P, Sema.Context.EmptyConstraintSet);

   // Check applicable extensions.
   for (auto* Ext : P->getExtensions()) {
      using ResultKind
         = CheckProtocolExtensionApplicabilityQuery::ResultKind;

      ResultKind Applicability;
      if (Sema.QC.CheckProtocolExtensionApplicability(Applicability,
                                                      Rec->getType(), Ext)) {
         return;
      }

      ConstraintSet *neededConstraints = nullptr;
      switch (Applicability) {
      case ResultKind::DoesNotApply:
         continue;
      case ResultKind::DoesApply:
         break;
      case ResultKind::CantTell:
         neededConstraints = Sema.getDeclConstraints(Ext);
         break;
      }

      if (Sema.QC.PrepareDeclInterface(Ext)) {
         continue;
      }

      // Remember which default implementations and other declarations are
      // provided by this extension.
      scanApplicableExtension(Rec, Conf, Ext, C);
   }

   // Check requirements.
   checkRecordCommon(Rec, Conf);
   while (true) {
      FoundChanges = false;

      auto LocalDelayedChecks = move(DelayedChecks);
      DelayedChecks.clear();

      for (PendingConformanceCheck& Check : LocalDelayedChecks.takeVector()) {
         checkSingleDecl(Rec, Check, Check.requirement);
      }

      if (!FoundChanges) {
         break;
      }
   }
}

static void FilterImpls(QueryContext &QC, RecordDecl *Rec,
                        PendingConformanceCheck &Conf,
                        SmallVectorImpl<NamedDecl*> &FoundDecls,
                        bool &dependent) {
   QualType Self = QC.Context.getRecordType(Rec);
   dependent = false;

   // Exclude default impls whose constraints aren't met.
   int maxConstraints = 0;
   int applicableImpls = 0;

   for (auto &Impl : FoundDecls) {
      auto *DC = Impl->getLexicalContext();
      auto *Ext = dyn_cast<ExtensionDecl>(DC);

      if (!Ext) {
         ++applicableImpls;
         continue;
      }

      auto *CS = QC.Sema->getDeclConstraints(Ext);
      if (CS->empty()) {
         ++applicableImpls;
         continue;
      }

      maxConstraints = std::max(maxConstraints, (int)CS->size());
      QualType NewSelf = QC.Sema->ApplyCapabilities(Self, Conf.introducedIn);

      bool include = true;
      for (auto *C : *CS) {
         bool Satisfied;
         if (auto err = QC.IsConstraintSatisfied(Satisfied, C, Rec, Rec)) {
            dependent |= err.isDependent();
            continue;
         }

         if (!Satisfied) {
            if (auto err = QC.IsConstraintSatisfied(Satisfied, C, NewSelf->getRecord(), Rec)) {
               dependent |= err.isDependent();
               continue;
            }

            if (!Satisfied) {
               include = false;
            }

            break;
         }
      }

      if (!include) {
         Impl = nullptr;
      }
      else {
         ++applicableImpls;
      }
   }

   // Prefer implementations with the maximal constraint set.
   if (applicableImpls > 0 && maxConstraints > 0) {
      for (auto &Impl : FoundDecls) {
         auto* DC = Impl->getLexicalContext();
         auto* Ext = dyn_cast<ExtensionDecl>(DC);

         if (!Ext) {
            Impl = nullptr;
            continue;
         }

         auto* CS = QC.Sema->getDeclConstraints(Ext);
         if (CS->size() < maxConstraints) {
            Impl = nullptr;
            continue;
         }
      }
   }

   FoundDecls.erase(std::remove(FoundDecls.begin(), FoundDecls.end(), nullptr),
                    FoundDecls.end());
}

bool ConformanceCheckerImpl::FindImplementations(RecordDecl *Rec,
                                                 DeclarationName Name,
                                                 PendingConformanceCheck &Conf)
{
   // Find all implementations, disregarding constraints.
   const MultiLevelLookupResult *AllImpls;
   if (Sema.QC.DirectLookup(AllImpls, Rec, Name, true)) {
      return true;
   }

   TestSet.clear();
   FoundImpls.clear();

   for (auto *Impl : AllImpls->allDecls()) {
      if (!TestSet.insert(Impl).second) {
         continue;
      }

      FoundImpls.push_back(Impl);
   }

   bool Dependent;
   FilterImpls(Sema.QC, Rec, Conf, FoundImpls, Dependent);

   return false;
}

MethodDecl* ConformanceCheckerImpl::checkIfImplicitConformance(
    RecordDecl* Rec, ProtocolDecl* Proto, MethodDecl& M)
{
   if (Proto == Sema.getEquatableDecl()) {
      IdentifierInfo& II = Sema.getContext().getIdentifiers().get("==");
      DeclarationName DeclName
          = Sema.getContext().getDeclNameTable().getInfixOperatorName(II);

      if (M.getDeclName() == DeclName) {
         for (auto& decl : Rec->getDecls()) {
            auto F = dyn_cast<FieldDecl>(decl);
            if (!F || F->isStatic())
               continue;

            if (Sema.QC.PrepareDeclInterface(F)) {
               continue;
            }

            bool IsEquatable;
            if (Sema.QC.IsEquatable(IsEquatable, F->getType())) {
               continue;
            }

            if (!IsEquatable) {
               Sema.diagnose(
                   M.getRecord(), err_implicit_conformance_cannot_be_declared,
                   /*Equatable*/ 0, F->getDeclName(), F->getSourceLoc());

               return nullptr;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(Result, Rec,
                                            ImplicitConformanceKind::Equatable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == Sema.getHashableDecl()) {
      if (M.getDeclName().isStr("hashValue") && M.getArgs().size() == 1) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(Result, Rec,
                                            ImplicitConformanceKind::Hashable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == Sema.getCopyableDecl()) {
      if (M.getDeclName().isStr("copy") && M.getArgs().size() == 1) {
         for (auto F : Rec->getDecls<FieldDecl>()) {
            if (F->isStatic())
               continue;

            if (Sema.QC.PrepareDeclInterface(F)) {
               continue;
            }

            bool IsCopyable;
            if (Sema.QC.IsCopyable(IsCopyable, F->getType())) {
               continue;
            }

            if (!IsCopyable) {
               Sema.diagnose(M.getRecord(),
                             err_implicit_conformance_cannot_be_declared,
                             3 /*Copyable*/, F->getDeclName(), F->getSourceLoc());

               return nullptr;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(Result, Rec,
                                            ImplicitConformanceKind::Copyable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == Sema.getStringRepresentableDecl()) {
      auto *Str = Sema.getStringDecl(); (void)Str;
      assert(Str && "StringRepresentable without String decl?");

      DeclarationName DN = Sema.getContext().getIdentifiers().get("toString");
      if (M.getDeclName() == DN) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(
                 Result, Rec, ImplicitConformanceKind::StringRepresentable)) {
            return &M;
         }

         return Result;
      }
   }

   return nullptr;
}

NamedDecl* ConformanceCheckerImpl::checkIfProtocolDefaultImpl(
    RecordDecl* Rec, ProtocolDecl* Proto, NamedDecl* D,
    PendingConformanceCheck &Conf)
{
   auto Impls = Sema.Context.getProtocolDefaultImpls(Proto, D);
   if (Impls.empty()) {
      return nullptr;
   }

   TestSet.clear();
   FoundImpls.clear();

   // Find the most specific implementations.
   int maxSpecificity = 0;
   for (auto* ND : Impls) {
      for (auto [Decl, Specificity] : ApplicableDefaultImpls) {
         if (Decl != ND) {
            continue;
         }

         maxSpecificity = std::max(Specificity, maxSpecificity);
      }
   }

   for (auto* ND : Impls) {
      // Check that constraints on this default impl are satisfied.
      for (auto [Decl, Specificity] : ApplicableDefaultImpls) {
         if (Decl != ND || Specificity < maxSpecificity) {
            continue;
         }
         if (!TestSet.insert(Decl).second) {
            continue;
         }

         FoundImpls.push_back(Decl);
      }
   }

   if (FoundImpls.empty()) {
      return nullptr;
   }

   // For templates we don't know which implementation applies until
   // instantiation, so just check that there are possible ones.
   if (!Rec->isTemplateOrInTemplate()) {
      bool Dependent;
      FilterImpls(Sema.QC, Rec, Conf, FoundImpls, Dependent);

      if (Dependent && FoundImpls.size() > 1) {
         FoundChanges = true;
         return FoundImpls.front();
      }
      else if (FoundImpls.size() != 1) {
         Sema.diagnose(warn_generic_warn,
             "default implementation of " + D->getDeclName().toString()
             + " is ambiguous for " + Rec->getFullName(),
             D->getSourceLoc());

         for (auto *Impl : FoundImpls) {
            Sema.diagnose(note_candidate_here, Impl->getSourceLoc());
         }

         return nullptr;
      }
   }

   NamedDecl *Impl = FoundImpls.front();
   NamedDecl* Inst = Sema.getInstantiator().InstantiateProtocolDefaultImpl(
      Impl,Sema.Context.getRecordType(Rec));

   if (!Inst) {
      return D;
   }

   FoundChanges = true;
   DefaultImpls.insert(Inst);

   addProtocolImpl(Rec, D, Inst);
   addProtocolImpl(Rec, Impl, Inst);

   return Inst;
}

static StringRef ConventionToString(ArgumentConvention C)
{
   switch (C) {
   case ArgumentConvention::Owned:
      return "owned";
   case ArgumentConvention::Borrowed:
      return "borrow";
   case ArgumentConvention::MutableRef:
      return "mut ref";
   case ArgumentConvention::ImmutableRef:
      return "ref";
   case ArgumentConvention::Default:
      llvm_unreachable("should not happen!");
   }
}

static void
issueDiagnostics(SemaPass& SP,
                 ArrayRef<ConformanceCheckerImpl::MethodCandidate> Cands)
{
   for (auto& Cand : Cands) {
      switch (Cand.Msg) {
      case diag::note_incorrect_protocol_impl_attr:
         SP.diagnose(Cand.Msg, Cand.Data1, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_fallible:
      case diag::note_incorrect_protocol_impl_must_be_static:
      case diag::note_incorrect_protocol_impl_must_not_be_static:
         SP.diagnose(Cand.Msg, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_num_args:
         SP.diagnose(Cand.Msg, Cand.Data1, Cand.Data2, Cand.Data3, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_return_type:
         SP.diagnose(Cand.Msg, QualType::getFromOpaquePtr((void*)Cand.Data1),
                     QualType::getFromOpaquePtr((void*)Cand.Data2), Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_signature:
         SP.diagnose(Cand.Msg, QualType::getFromOpaquePtr((void*)Cand.Data1),
                     Cand.Data2 + 1,
                     QualType::getFromOpaquePtr((void*)Cand.Data3), Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_label:
         SP.diagnose(Cand.Msg, (IdentifierInfo*)Cand.Data1, Cand.Data2 + 1,
                     Cand.Data3, (IdentifierInfo*)Cand.Data4, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_no_label:
         SP.diagnose(Cand.Msg, Cand.Data1 + 1, (IdentifierInfo*)Cand.Data2,
                     Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_convention:
         SP.diagnose(Cand.Msg, ConventionToString((ArgumentConvention)Cand.Data1),
                     Cand.Data2 + 1,
                     ConventionToString((ArgumentConvention)Cand.Data3),
                     Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_prop:
         SP.diagnose(note_incorrect_protocol_impl_prop, Cand.Data1,
                     DeclarationName::getFromOpaquePtr((void*)Cand.Data2),
                     Cand.Data3, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_prop_type:
         SP.diagnose(note_incorrect_protocol_impl_prop_type, Cand.Data1,
                     DeclarationName::getFromOpaquePtr((void*)Cand.Data2),
                     QualType::getFromOpaquePtr((void*)Cand.Data3),
                     QualType::getFromOpaquePtr((void*)Cand.Data4), Cand.SR);
         break;
      default:
         llvm_unreachable("bad diag kind");
      }
   }
}

bool ConformanceCheckerImpl::checkAssociatedType(RecordDecl* Rec,
                                                 PendingConformanceCheck& Conf,
                                                 AssociatedTypeDecl* AT)
{
   auto* Impl = Sema.QC.LookupSingleAs<NamedDecl>(Rec, AT->getDeclName(), LookupOpts::Restricted);
   if (!Impl) {
      return false;
   }

   if (!isa<AliasDecl>(Impl) && !isa<RecordDecl>(Impl)) {
      Sema.diagnose(err_associated_type_impl_must_be_alias,
                    Impl->getSourceLoc());

      return false;
   }

   Sema.QC.PrepareDeclInterface(Impl);
   CheckedConformanceSet.insert(Conf);

   addProtocolImpl(Rec, AT, Impl);
   return true;
}

void ConformanceCheckerImpl::checkRecordCommon(RecordDecl* Rec,
                                               PendingConformanceCheck& Conf)
{
   if (!CheckedConformanceSet.insert(Conf)) {
      return;
   }

   if (Conf.conformance->isExternal()) {
      if (auto *ModFile = Conf.conformance->getModFile()) {
         ModFile->LoadAllDecls(*Conf.conformance);
      }
   }

   for (auto& decl : Conf.conformance->getDecls()) {
      if (Rec->isInvalid())
         return;

      // not a protocol requirement.
      if (decl->getDeclContext() != Conf.conformance)
         continue;

      if (decl->isSynthesized())
         continue;

      auto ND = dyn_cast<NamedDecl>(decl);
      if (!ND)
         continue;

      Conf.requirement = ND;
      if (!CheckedConformanceSet.insert(Conf)) {
         continue;
      }

      checkSingleDecl(Rec, Conf, ND);
   }

   IssuedError = false;
}

void ConformanceCheckerImpl::scanApplicableExtension(
    RecordDecl* Rec, PendingConformanceCheck& Conf, ExtensionDecl* Ext,
    Conformance *C)
{
   for (auto& decl : Ext->getDecls()) {
      if (Rec->isInvalid())
         return;

      auto* ND = dyn_cast<NamedDecl>(decl);
      if (!ND || ND->isSynthesized())
         continue;

      if (ND->isDefault()) {
         ApplicableDefaultImpls.insert(
             std::make_pair(ND, C->getProto()->getSpecificity()));
      }
      else {
         ExtensionDecls.insert(ND);
      }
   }
}

void ConformanceCheckerImpl::inheritAttributes(NamedDecl* Req, NamedDecl* Impl)
{
   auto& Context = Sema.getContext();
   for (auto* Attr : Req->getAttributes()) {
      if (Attr->isInherited()) {
         Context.addAttribute(Impl, Attr);
      }
   }
}

void ConformanceCheckerImpl::addProtocolImpl(RecordDecl* R, NamedDecl* Req,
                                             NamedDecl* Impl)
{
   Impl->setImplOfProtocolRequirement(true);
   Sema.Context.addProtocolImpl(Rec, Req, Impl);

   if (auto* Prop = dyn_cast<PropDecl>(Req)) {
      auto* FoundProp = cast<PropDecl>(Impl);
      if (auto* M = Prop->getGetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, M, FoundProp->getGetterMethod());
      }
      if (auto* M = Prop->getSetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, M, FoundProp->getSetterMethod());
      }
   }
   else if (auto* S = dyn_cast<SubscriptDecl>(Req)) {
      auto* FoundSub = cast<SubscriptDecl>(Impl);
      if (auto* M = S->getGetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, M, FoundSub->getGetterMethod());
      }
      if (auto* M = S->getSetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, M, FoundSub->getSetterMethod());
      }
   }
}

bool ConformanceCheckerImpl::checkSingleDecl(RecordDecl* Rec,
                                             PendingConformanceCheck& Conf,
                                             NamedDecl* Req)
{
   auto* Impl = checkSingleDeclImpl(Rec, Conf, Req);
   if (!Impl)
      return false;

   if (Impl == Req)
      return true;

   inheritAttributes(Req, Impl);
   addProtocolImpl(Rec, Req, Impl);

   return true;
}

NamedDecl* ConformanceCheckerImpl::checkPropImpl(RecordDecl *Rec,
                                                 PendingConformanceCheck &Conf,
                                                 PropDecl *Prop)
{
   auto *Proto = Conf.conformance;
   if (FindImplementations(Rec, Prop->getDeclName(), Conf)) {
      return nullptr;
   }

   PropDecl* FoundProp = nullptr;
   if (FoundImpls.size() == 1) {
      FoundProp = dyn_cast<PropDecl>(FoundImpls.front());
   }

   if (!FoundProp) {
      if (auto* Impl = checkIfProtocolDefaultImpl(Rec, Proto, Prop, Conf)) {
         return Impl;
      }
      if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }

      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                    Prop->getDeclName(), 0 /*is missing*/,
                    Prop->getSourceLoc());

      return nullptr;
   }

   FoundProp = Sema.maybeInstantiateTemplateMember(Rec, FoundProp);

   if (Sema.QC.PrepareDeclInterface(FoundProp)) {
      return nullptr;
   }

   if (Prop->isReadWrite() && !FoundProp->isReadWrite()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_must_be_read_write,
                    Prop->getSourceLoc());

      return nullptr;
   }

   if (!Prop->isReadWrite() && FoundProp->isReadWrite()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_must_not_be_read_write,
                    Prop->getSourceLoc());

      return nullptr;
   }

   if (Prop->isStatic() && !FoundProp->isStatic()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Prop, "prop",
                    Prop->getSourceLoc());
      Sema.diagnose(note_incorrect_protocol_impl_must_be_static,
                    FoundProp->getSourceLoc());

      return nullptr;
   }

   if (!Prop->isStatic() && FoundProp->isStatic()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Prop, "prop",
                    Prop->getSourceLoc());
      Sema.diagnose(note_incorrect_protocol_impl_must_not_be_static,
                    FoundProp->getSourceLoc());

      return nullptr;
   }

   auto GivenTy = FoundProp->getType().getResolvedType();

   SourceType NeededTy = Prop->getType();
   if (!checkTypeCompatibility(GivenTy, NeededTy, FoundProp, Proto)) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Prop, "prop",
                    Prop->getSourceLoc());
      Sema.diagnose(note_incorrect_protocol_impl_prop_type, 1 /*property*/,
                    Prop->getDeclName(), NeededTy, GivenTy,
                    Prop->getSourceLoc());

      return nullptr;
   }

   if (Prop->hasGetter() && !FoundProp->hasGetter()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Prop, "prop",
                    Prop->getSourceLoc());
      Sema.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                    Prop->getDeclName(), 1 /*requires getter*/,
                    Prop->getSourceLoc());

      return nullptr;
   }

   if (Prop->hasSetter() && !FoundProp->hasSetter()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Prop, "prop",
                    Prop->getSourceLoc());
      Sema.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                    Prop->getDeclName(), 2 /*requires setter*/,
                    Prop->getSourceLoc());

      return nullptr;
   }

   return FoundProp;
}

NamedDecl* ConformanceCheckerImpl::checkSubscriptImpl(RecordDecl *Rec,
                                                      PendingConformanceCheck &Conf,
                                                      SubscriptDecl *S)
{
   auto *Proto = Conf.conformance;
   if (FindImplementations(Rec, S->getDeclName(), Conf)) {
      return nullptr;
   }

   std::vector<MethodCandidate> Candidates;

   NamedDecl* Impl = nullptr;
   for (auto* D : FoundImpls) {
      auto* FoundSub = cast<SubscriptDecl>(D);
      FoundSub = Sema.maybeInstantiateTemplateMember(Rec, FoundSub);

      if (Sema.QC.PrepareDeclInterface(FoundSub)) {
         return nullptr;
      }

      if (FoundSub->isStatic() && !S->isStatic()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_must_not_be_static;
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }
      if (!FoundSub->isStatic() && S->isStatic()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_must_be_static;
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }

      auto GivenTy = FoundSub->getType().getResolvedType();

      SourceType NeededTy = S->getType();
      if (!checkTypeCompatibility(GivenTy, NeededTy, FoundSub, Proto)) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_prop_type;
         Cand.Data1 = 0 /*subscript*/;
         Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
         Cand.Data3 = (uintptr_t)NeededTy.getResolvedType().getAsOpaquePtr();
         Cand.Data4 = (uintptr_t)GivenTy.getAsOpaquePtr();
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }

      if (S->isReadWrite() && !FoundSub->isReadWrite()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_must_be_read_write;
         Cand.SR = FoundSub->getSourceLoc();

         return nullptr;
      }

      if (!S->isReadWrite() && FoundSub->isReadWrite()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_must_not_be_read_write;
         Cand.SR = FoundSub->getSourceLoc();

         return nullptr;
      }

      if (S->hasGetter() && !FoundSub->hasGetter()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_prop;
         Cand.Data1 = 0 /*subscript*/;
         Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
         Cand.Data3 = 1 /*requires getter*/;
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }

      if (S->hasSetter() && !FoundSub->hasSetter()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_prop;
         Cand.Data1 = 0 /*subscript*/;
         Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
         Cand.Data3 = 2 /*requires setter*/;
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }

      if (S->hasGetter()) {
         if (!CompareArgumentLists(S->getGetterMethod(),
                                   FoundSub->getGetterMethod(),
                                   Proto, Candidates)) {
            continue;
         }
      }

      if (S->hasSetter()) {
         if (!CompareArgumentLists(S->getSetterMethod(),
                                   FoundSub->getSetterMethod(),
                                   Proto, Candidates)) {
            continue;
         }
      }

      Impl = FoundSub;
      break;
   }

   if (!Impl) {
      if ((Impl = checkIfProtocolDefaultImpl(Rec, Proto, S, Conf))) {
         return Impl;
      }

      if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }

      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_prop, 0 /*subscript*/,
                    S->getDeclName(), 0 /*is missing*/, S->getSourceLoc());

      issueDiagnostics(Sema, Candidates);
      return nullptr;
   }

   return Impl;
}

NamedDecl* ConformanceCheckerImpl::checkInitImpl(RecordDecl *Rec,
                                                 PendingConformanceCheck &Conf,
                                                 InitDecl *Init)
{
   auto *Proto = Conf.conformance;

   // Make sure all initializers are deserialized.
   auto InitName = Sema.getContext().getDeclNameTable().getConstructorName(
      Sema.getContext().getRecordType(Rec));

   if (FindImplementations(Rec, InitName, Conf)) {
      return nullptr;
   }

   InitDecl* MethodImpl = nullptr;
   std::vector<MethodCandidate> Candidates;

   for (auto* D : FoundImpls) {
      auto* Impl = cast<InitDecl>(D);
      Impl = Sema.maybeInstantiateTemplateMember(Rec, Impl);

      if (Sema.QC.PrepareDeclInterface(Impl)) {
         return nullptr;
      }

      if (Impl->isFallible() && !Init->isFallible()) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = diag::note_incorrect_protocol_impl_fallible;
         Cand.SR = Impl->getSourceLoc();

         continue;
      }

      if (!CompareArgumentLists(Init, Impl, Proto, Candidates))
         continue;

      MethodImpl = Impl;
      break;
   }

   if (!MethodImpl) {
      if (auto* Impl = checkIfProtocolDefaultImpl(Rec, Proto, Init, Conf)) {
         MethodImpl = cast<InitDecl>(Impl);
      }
      else if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }
      else {
         genericError(Rec, Proto);
         Sema.diagnose(note_incorrect_protocol_impl_missing, Init, "init",
                       Init->getSourceLoc());

         issueDiagnostics(Sema, Candidates);
         return nullptr;
      }
   }

   // This needs to be updated here in case Rec is a template and
   // the parameterless constructor comes from a protocol default implementation.
   if (Rec->isInstantiation() && MethodImpl->getArgs().empty()) {
      if (auto *S = dyn_cast<StructDecl>(Rec)) {
         S->setParameterlessConstructor(MethodImpl);
      }
   }

   MethodImpl->setIsProtocolMethod(true);
   return MethodImpl;
}

NamedDecl* ConformanceCheckerImpl::checkMethodImpl(RecordDecl *Rec,
                                                   PendingConformanceCheck &Conf,
                                                   MethodDecl *Method)
{
   auto *Proto = Conf.conformance;

   // Make sure all methods with this name are deserialized.
   if (FindImplementations(Rec, Method->getDeclName(), Conf)) {
      return nullptr;
   }

   MethodDecl* MethodImpl = nullptr;
   std::vector<MethodCandidate> Candidates;

   for (auto Decl : FoundImpls) {
      auto Impl = dyn_cast<MethodDecl>(Decl);
      if (!Impl)
         continue;

      Impl = Sema.maybeInstantiateTemplateMember(Rec, Impl);

      if (Sema.QC.PrepareDeclInterface(Impl)) {
         return nullptr;
      }

      if (Impl->throws() && !Method->throws()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 0;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->isUnsafe() && !Method->isUnsafe()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 1;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->isAsync() && !Method->isAsync()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 2;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->hasMutableSelf() && !Method->hasMutableSelf()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 3;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->isStatic() && !Method->isStatic()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_must_not_be_static;
         Cand.SR = Impl->getSourceLoc();

         continue;
      }
      if (!Impl->isStatic() && Method->isStatic()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_must_not_be_static;
         Cand.SR = Impl->getSourceLoc();

         continue;
      }

      SourceType NeededRet = Method->getReturnType();
      if (!checkTypeCompatibility(Impl->getReturnType(), NeededRet, Impl, Proto)) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_method_return_type;
         Cand.Data1
            = (uintptr_t)NeededRet.getResolvedType().getAsOpaquePtr();
         Cand.Data2 = (uintptr_t)Impl->getReturnType()
                                     .getResolvedType()
                                     .getAsOpaquePtr();

         Cand.SR = Impl->getSourceLoc();

         continue;
      }

      if (!CompareArgumentLists(Method, Impl, Proto, Candidates))
         continue;

      MethodImpl = Impl;
      break;
   }

   if (!MethodImpl) {
      if (auto* Impl = checkIfProtocolDefaultImpl(Rec, Proto, Method, Conf)) {
         return Impl;
      }

      if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }

      if (auto* Impl = checkIfImplicitConformance(Rec, Proto, *Method)) {
         return Impl;
      }

      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Method,
                    Method->getDeclName(), Method->getSourceLoc());

      issueDiagnostics(Sema, Candidates);
      return nullptr;
   }

   MethodImpl->setIsProtocolMethod(true);
   return MethodImpl;
}

bool ConformanceCheckerImpl::CompareArgumentLists(
   MethodDecl *Method,
   MethodDecl *Impl,
   ProtocolDecl *Proto,
   std::vector<MethodCandidate> &Candidates)
{
   auto GivenArgs = Impl->getArgs();
   auto NeededArgs = Method->getArgs();

   auto NumGiven = GivenArgs.size();
   auto NumNeeded = NeededArgs.size();

   if (NumGiven != NumNeeded) {
      auto& Cand = Candidates.emplace_back();

      Cand.Msg = diag::note_incorrect_protocol_impl_method_num_args;
      Cand.Data1 = (uintptr_t)(NumGiven > NumNeeded);
      Cand.Data2 = NumNeeded;
      Cand.Data3 = NumGiven;
      Cand.SR = Impl->getSourceLoc();

      return false;
   }

   bool ArgsValid = true;
   unsigned i = isa<InitDecl>(Method) ? 0 : 1;

   for (; i < NumGiven; ++i) {
      if (GivenArgs[i]->getLabel() && !NeededArgs[i]->getLabel()) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_method_no_label;
         Cand.Data1 = i;
         Cand.Data2 = (uintptr_t)GivenArgs[i]->getLabel();
         Cand.SR = Impl->getSourceLoc();

         ArgsValid = false;
         break;
      }
      if (GivenArgs[i]->getLabel() != NeededArgs[i]->getLabel()) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_method_label;
         Cand.Data1 = (uintptr_t)NeededArgs[i]->getLabel();
         Cand.Data2 = i;
         Cand.Data3 = GivenArgs[i]->getLabel() != nullptr;
         Cand.Data4 = (uintptr_t)GivenArgs[i]->getLabel();
         Cand.SR = Impl->getSourceLoc();

         ArgsValid = false;
         break;
      }
      if (!Sema.AreConventionsCompatible(NeededArgs[i]->getConvention(),
                                         GivenArgs[i]->getConvention())) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_convention;
         Cand.Data1 = (uintptr_t)NeededArgs[i]->getConvention();
         Cand.Data2 = i;
         Cand.Data3 = (uintptr_t)GivenArgs[i]->getConvention();
         Cand.SR = Impl->getSourceLoc();

         ArgsValid = false;
         break;
      }

      QualType Given = GivenArgs[i]->getType();
      SourceType Needed = NeededArgs[i]->getType();
      if (!checkTypeCompatibility(Given, Needed, Impl, Proto)) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_method_signature;
         Cand.Data1
            = (uintptr_t)Needed.getResolvedType().getAsOpaquePtr();
         Cand.Data2 = i;
         Cand.Data3 = (uintptr_t)Given.getAsOpaquePtr();
         Cand.SR = Impl->getSourceLoc();

         ArgsValid = false;
         break;
      }
   }

   return ArgsValid;
}

NamedDecl* ConformanceCheckerImpl::checkSingleDeclImpl(
    RecordDecl* Rec, PendingConformanceCheck& Conf, NamedDecl* Req)
{
   if (Sema.QC.PrepareDeclInterface(Req)) {
      Rec->setIsInvalid(true);
      return nullptr;
   }

   if (auto* AT = dyn_cast<AssociatedTypeDecl>(Req)) {
      checkAssociatedType(Rec, Conf, AT);
      return nullptr;
   }

   if (auto Prop = dyn_cast<PropDecl>(Req)) {
      return checkPropImpl(Rec, Conf, Prop);
   }

   if (auto S = dyn_cast<SubscriptDecl>(Req)) {
      return checkSubscriptImpl(Rec, Conf, S);
   }

   if (auto Init = dyn_cast<InitDecl>(Req)) {
      return checkInitImpl(Rec, Conf, Init);
   }

   if (auto Method = dyn_cast<MethodDecl>(Req)) {
      auto *MethodImpl = cast_or_null<MethodDecl>(checkMethodImpl(Rec, Conf, Method));
      if (!MethodImpl)
         return nullptr;

      if (Conf.conformance == Sema.getCopyableDecl()
          && Method->getDeclName().isStr("copy")) {
         Sema.QC.AddImplicitConformance(
             MethodImpl, Rec, ImplicitConformanceKind::Copyable, MethodImpl);
      }
      else if (Conf.conformance == Sema.getStringRepresentableDecl()
               && Method->getDeclName().isStr("toString")) {
         Sema.QC.AddImplicitConformance(
             MethodImpl, Rec, ImplicitConformanceKind::StringRepresentable,
             MethodImpl);
      }
      else if (Conf.conformance == Sema.getHashableDecl()
               && Method->getDeclName().isStr("hashValue")) {
         Sema.QC.AddImplicitConformance(
             MethodImpl, Rec, ImplicitConformanceKind::Hashable, MethodImpl);
      }
      else if (Conf.conformance == Sema.getEquatableDecl()
               && Method->getDeclName().getInfixOperatorName()->isStr("==")) {
         Sema.QC.AddImplicitConformance(
             MethodImpl, Rec, ImplicitConformanceKind::Equatable, MethodImpl);
      }

      return MethodImpl;
   }

   llvm_unreachable("bad requirement kind");
}

} // namespace sema
} // namespace cdot

static AliasDecl* makeAssociatedType(SemaPass& Sema, NamedDecl* AT,
                                     SourceType DefaultType, RecordDecl* R)
{
   QualType Inst;
   if (Sema.QC.SubstAssociatedTypes(Inst, DefaultType, R->getType(),
                                    R->getSourceRange())) {
      return nullptr;
   }

   SourceType Ty(Sema.Context.getMetaType(Inst));
   auto* typeExpr = new (Sema.Context)
       IdentifierRefExpr(AT->getSourceLoc(), IdentifierKind::MetaType, Ty);

   auto* rawTypeExpr = StaticExpr::Create(Sema.Context, typeExpr);
   if (Ty->containsTemplateParamType()) {
      typeExpr->setNeedsInstantiation(true);
      rawTypeExpr->setNeedsInstantiation(true);
   }

   auto* InstDecl = AliasDecl::Create(Sema.Context, AT->getSourceLoc(),
                                      AccessSpecifier::Public,
                                      AT->getDeclName(), Ty, rawTypeExpr, {});

   Sema.addDeclToContext(*R, InstDecl);
   return InstDecl;
}

static AliasDecl* makeAssociatedType(SemaPass& Sema, AssociatedTypeDecl* AT,
                                     RecordDecl* R)
{
   if (Sema.QC.TypecheckDecl(AT)) {
      return nullptr;
   }

   return makeAssociatedType(Sema, AT, AT->getDefaultType(), R);
}

QueryResult DeclareSelfAliasQuery::run()
{
   auto* SelfII = QC.Sema->getIdentifier("Self");
   if (R->lookupSingle<NamedDecl>(SelfII)) {
      return finish();
   }

   QualType CanonicalTy = QC.Context.getRecordType(R);

   NamedDecl* Self;
   if (isa<ProtocolDecl>(R)) {
      Self = AssociatedTypeDecl::Create(QC.Context, R->getSourceLoc(), SelfII,
                                        SourceType(), CanonicalTy);
   }
   else {
      SourceType Ty(QC.Sema->Context.getMetaType(CanonicalTy));
      auto* typeExpr = new (QC.Sema->Context)
          IdentifierRefExpr(R->getSourceLoc(), IdentifierKind::MetaType, Ty);

      auto* rawTypeExpr = StaticExpr::Create(QC.Sema->Context, typeExpr);
      Self = AliasDecl::Create(QC.Sema->Context, R->getSourceLoc(),
                               AccessSpecifier::Public, SelfII, Ty, rawTypeExpr,
                               {});

      QC.Sema->registerAssociatedTypeImpl(R, cast<AliasDecl>(Self));
   }

   Self->setSynthesized(true);
   R->setType(CanonicalTy);

   QC.Sema->ActOnDecl(R, Self);
   return finish();
}

static AssociatedTypeDecl* getReferencedAssociatedType(const DeclConstraint* C)
{
   QualType constrainedType = C->getConstrainedType();
   while (constrainedType->isAssociatedType()) {
      if (auto Outer = constrainedType->asAssociatedType()->getOuterAT()) {
         if (Outer->isAssociatedType()
             && Outer->asAssociatedType()->getDecl()->isSelf()) {
            break;
         }

         constrainedType = Outer;
      }
      else {
         break;
      }
   }

   if (auto* AT = constrainedType->asAssociatedType()) {
      return AT->getDecl();
   }

   return nullptr;
}

QueryResult GetReferencedAssociatedTypesQuery::run()
{
   std::vector<AssociatedTypeDecl*> result;
   for (const DeclConstraint* C : *CS) {
      AssociatedTypeDecl* AT = getReferencedAssociatedType(C);
      if (!AT) {
         continue;
      }

      result.push_back(AT);
   }

   return finish(move(result));
}

QueryResult GetNeededAssociatedTypesQuery::run()
{
   auto& ConfTable = QC.Context.getConformanceTable();
   SmallSetVector<AssociatedTypeDecl*, 4> NeededAssociatedTypes;

   for (auto& Conf : ConfTable.getAllConformances(R)) {
      ProtocolDecl* Proto = Conf->getProto();

      for (auto* AT : Proto->getDecls<AssociatedTypeDecl>()) {
         if (!AT->isSelf()) {
            NeededAssociatedTypes.insert(AT);
         }
      }
   }

   return finish(NeededAssociatedTypes.takeVector());
}

QueryResult ReferencedAssociatedTypesReadyQuery::run()
{
   assert(CS && "should have early-exited!");

   RecordDecl* Rec = T->getRecord();
   DeclContext* Ctx = Rec->lookThroughExtension();

   ArrayRef<ExtensionDecl*> Extensions;
   if (isa<RecordDecl>(Ctx)) {
      Extensions = QC.Sema->Context.getExtensions(T);
   }

   ArrayRef<AssociatedTypeDecl*> referencedATs;
   QC.GetReferencedAssociatedTypes(referencedATs, CS);

   ResultKind RK = Ready;
   for (AssociatedTypeDecl* AT : referencedATs) {
      // Check if the associated type is already visible.
      AliasDecl* ATImpl = QC.Sema->getAssociatedTypeImpl(Rec, AT->getDeclName());
      if (!ATImpl) {
         return finish(NotReady);
      }

      if (ATImpl->isSelf()) {
         RK = ReferencesSelf;
      }
   }

   return finish(RK);
}

namespace {

class TypeDependencyFinder: public RecursiveTypeVisitor<TypeDependencyFinder> {
   /// The query context.
   QueryContext &QC;

   /// The conformance resolver.
   ConformanceResolver &Resolver;

   /// The dependency graph node to add dependencies to.
   ConformanceResolver::DependencyVertex &Node;

   /// The current decl context.
   RecordDecl *DC;

   /// Covariance of the template parameter we're visiting, if any.
   ProtocolDecl *NeededConformance = nullptr;

   /// Whether or not we encountered an error.
   bool encounteredError = false;

public:
   TypeDependencyFinder(QueryContext &QC,
                        ConformanceResolver &Resolver,
                        ConformanceResolver::DependencyVertex &Node,
                        RecordDecl *DC,
                        ProtocolDecl *NeededConformance = nullptr)
       : QC(QC), Resolver(Resolver), Node(Node),
         DC(DC), NeededConformance(NeededConformance)
   {}

   bool hasError() const { return encounteredError; }

   bool visitAssociatedType(AssociatedType *T)
   {
      SmallVector<QualType, 2> Types;

      QualType Current = T;
      while (Current) {
         QualType Outer;
         if (auto *AT = Current->asAssociatedType()) {
            if (AT->getDecl()->isSelf()) {
               break;
            }

            Outer = AT->getOuterAT();
         }

         Types.push_back(Current);
         Current = Outer;
      }

      auto *CurNode = &Node;
      for (int i = Types.size() - 1; i >= 0; --i) {
         QualType Ty = Types[i];
         if (auto *AT = Ty->asAssociatedType()) {
            assert (!AT->getDecl()->isSelf());

            ConformanceResolver::DependencyVertex *DepNode;
            if (i == Types.size() - 1) {
               DepNode = &Resolver.GetDependencyNode(
                   DependencyNode::ConcreteTypeOfAssociatedType, DC,
                   AT->getDecl());
            }
            else {
               DepNode = &Resolver.GetDependencyNode(
                   DependencyNode::NestedAssociatedType, DC,
                   Ty.getAsOpaquePtr());
            }

            if (CurNode != DepNode) {
               CurNode->addIncoming(DepNode);
               CurNode = DepNode;
            }
         }
      }

      return true;
   }

   bool visitRecordType(RecordType *R)
   {
      if (NeededConformance != nullptr) {
         encounteredError |= Resolver.AddConformanceDependency(
             Node, R->getRecord(), NeededConformance);
      }

      if (!QC.Sema->getInstantiator().isShallowInstantiation(R->getRecord())) {
         return true;
      }

      encounteredError |= Resolver.BuildWorklist(R->getRecord());
      return true;
   }
};

} // anonymous namespace

ConformanceResolver &SemaPass::getConformanceResolver()
{
   if (ConfResolver == nullptr) {
      ConfResolver = new(Context) ConformanceResolver(QC);
   }

   return *ConfResolver;
}

bool SemaPass::IsBeingResolved(RecordDecl *R)
{
   return getConformanceResolver().IsBeingResolved(R);
}

bool ConformanceResolver::registerImplicitConformances(RecordDecl *R)
{
   assert(R->isTemplateOrInTemplate());

   auto &baseConf = *Cache[BaseConformances[R]];
   auto &Meta = QC.RecordMeta[R];

   if (Meta.IsImplicitlyCopyable) {
      ProtocolDecl *Proto = QC.Sema->getCopyableDecl();
      UncheckedConformance &newConf = CreateCondConformance(
          Proto, QC.Context.EmptyConstraintSet, R, &baseConf);

      baseConf.innerConformances->emplace_back(move(newConf));
   }
   if (Meta.IsImplicitlyEquatable) {
      ProtocolDecl *Proto = QC.Sema->getEquatableDecl();
      UncheckedConformance &newConf = CreateCondConformance(
          Proto, QC.Context.EmptyConstraintSet, R, &baseConf);

      baseConf.innerConformances->emplace_back(move(newConf));
   }
   if (Meta.IsImplicitlyHashable) {
      ProtocolDecl *Proto = QC.Sema->getHashableDecl();
      UncheckedConformance &newConf = CreateCondConformance(
          Proto, QC.Context.EmptyConstraintSet, R, &baseConf);

      baseConf.innerConformances->emplace_back(move(newConf));
   }
   if (Meta.IsImplicitlyRawRepresentable) {
      ProtocolDecl *Proto = QC.Sema->getRawRepresentableDecl();
      UncheckedConformance &newConf = CreateCondConformance(
          Proto, QC.Context.EmptyConstraintSet, R, &baseConf);

      baseConf.innerConformances->emplace_back(move(newConf));
   }
   if (Meta.IsImplicitlyStringRepresentable) {
      ProtocolDecl *Proto = QC.Sema->getStringRepresentableDecl();
      UncheckedConformance &newConf = CreateCondConformance(
          Proto, QC.Context.EmptyConstraintSet, R, &baseConf);

      baseConf.innerConformances->emplace_back(move(newConf));
   }

   return false;
}

#ifndef NDEBUG
static string PrintAssociatedTypes(QueryContext &QC, RecordDecl *Rec)
{
   string result = "[";

   ArrayRef<AssociatedTypeDecl*> NeededAssociatedTypes;
   QC.GetNeededAssociatedTypes(NeededAssociatedTypes, Rec);

   int i = 0;
   for (auto *AT : NeededAssociatedTypes) {
      if (i++ != 0)
         result += ", ";

      result += AT->getDeclName().getIdentifierInfo()->getIdentifier();
      result += " = ";

      AliasDecl *Impl = QC.Sema->getAssociatedTypeImpl(Rec, AT->getDeclName());
      if (!Impl) {
         assert(Rec->isInvalid());
         result += "???";
         continue;
      }

      QC.PrepareDeclInterface(Impl);
      result += Impl->getType()->removeMetaType()->toDiagString();
   }

   result += "]";
   return result;
}
#endif

bool ConformanceResolver::registerDeclaredConformances(
    CanType Self, CanType protoTy,
    MutableArrayRef<SourceType> conformanceTypes, ConstraintSet* constraints,
    SmallDenseSet<uintptr_t, 4>& testSet,
    SmallPtrSetImpl<ProtocolDecl*>& directConformances,
    DeclContext* introducedBy, UncheckedConformance& newConfRef)
{
   ArrayRef<ProtocolDecl*> newConformances;
   if (QC.ResolveConformancesToProtocols(newConformances, protoTy,
                                         conformanceTypes)) {
      return true;
   }

   for (auto* newConfProto : newConformances) {
      if (registerConformance(Self, newConfProto, constraints,
                              testSet, directConformances, introducedBy,
                              newConfRef, false)) {
         return true;
      }
   }

   return false;
}

static void diagnoseCircularConformance(QueryContext &QC, CanType Self,
                                        ProtocolDecl *proto,
                                        UncheckedConformance& outer,
                                        DeclContext *introducedBy) {
   QC.Sema->diagnose(
       err_conform_to_itself, Self,
       cast<NamedDecl>(introducedBy)->getSourceLoc());

   SmallVector<ProtocolDecl*, 4> Chain { proto };

   auto *cur = &outer;
   while (cur && cur->proto) {
      Chain.push_back(cur->proto);
      cur = cur->outer;
   }

   Chain.push_back(proto);

   std::string str = "conformance chain: ";
   {
      llvm::raw_string_ostream OS(str);
      for (int i = Chain.size() - 1; i >= 0; --i) {
         if (i < Chain.size() - 1)
            OS << " -> ";

         OS << Chain[i]->getDeclName();
      }
   }

   QC.Sema->diagnose(
       note_generic_note,
       str,
       cast<NamedDecl>(introducedBy)->getSourceLoc());
}

bool ConformanceResolver::registerConformance(
   CanType Self, ProtocolDecl* proto, ConstraintSet* constraints,
   SmallDenseSet<uintptr_t, 4>& testSet,
   SmallPtrSetImpl<ProtocolDecl*>& directConformances,
   DeclContext* introducedBy, UncheckedConformance& outer,
   bool isDirect)
{
   bool exclude;
   if (isDirect) {
      exclude = !directConformances.insert(proto).second;
   }
   else {
      exclude = directConformances.count(proto) != 0;
   }

   if (proto == Self->getRecord()) {
      diagnoseCircularConformance(QC, Self, proto, outer, introducedBy);
      return true;
   }

   // Check if we already covered this conformance.
   UncheckedConformance &newConf = CreateCondConformance(
      proto, constraints, introducedBy, &outer);

   newConf.exclude = exclude;
   PotentialConformances.insert(std::make_pair(Self->getRecord(), proto));

   auto insertResult = testSet.insert(newConf.hashVal);
   if (!insertResult.second) {
      return false;
   }

   if (auto *SelfProto = dyn_cast<ProtocolDecl>(Self->getRecord())) {
      if (newConf.combinedConstraints && !newConf.combinedConstraints->empty()) {
         ConfTbl.addConformance(QC.Context, ConformanceKind::Conditional,
                                Self->getRecord(), proto, newConf.introducedBy,
                                newConf.combinedConstraints, newConf.depth);
      }
      else {
         ConfTbl.addConformance(QC.Context, ConformanceKind::Explicit,
                                Self->getRecord(), proto, newConf.introducedBy,
                                nullptr, newConf.depth);
      }

      auto &SelfNode = ConformanceDAG.getOrAddVertex(SelfProto);
      auto &OtherNode = ConformanceDAG.getOrAddVertex(proto);
      SelfNode.addIncoming(&OtherNode);

      SelfProto->setHasAssociatedTypeConstraint(
          SelfProto->hasAssociatedTypeConstraint()
          || proto->hasAssociatedTypeConstraint());
   }

   auto& newConfRef = outer.innerConformances->emplace_back(std::move(newConf));
   newConfRef.initializerInnerConformances();

   // Register the conformances declared directly on the protocol.
   MutableArrayRef<SourceType> conformanceTypes = proto->getConformanceTypes();
   QualType protoTy = QC.Context.getRecordType(proto);

   if (registerDeclaredConformances(Self, protoTy,
                                    conformanceTypes, nullptr, testSet,
                                    directConformances, introducedBy,
                                    newConfRef)) {
      return true;
   }

   // Make sure extensions of this protocol are ready.
   if (QC.FindExtensions(protoTy)) {
      return true;
   }

   // Insert the dependent conformances.
   auto protocolExtensions = QC.Context.getExtensions(protoTy);
   if (protocolExtensions.empty()) {
      return false;
   }

   for (auto* ext : protocolExtensions) {
      auto* extConstraints = QC.Sema->getDeclConstraints(ext);
      conformanceTypes = ext->getConformanceTypes();

      if (registerDeclaredConformances(
              Self, protoTy, conformanceTypes, extConstraints,
              testSet, directConformances, introducedBy, newConfRef)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::registerConformances(
   CanType Self, DeclContext* DC, SmallDenseSet<uintptr_t, 4>& testSet,
   SmallPtrSetImpl<ProtocolDecl*>& directConformances,
    UncheckedConformance& baseConf)
{
   ConstraintSet* constraints = nullptr;
   MutableArrayRef<SourceType> conformanceTypes;
   bool direct;

   auto* ext = dyn_cast<ExtensionDecl>(DC);
   if (ext) {
      conformanceTypes = ext->getConformanceTypes();
      constraints = QC.Sema->getDeclConstraints(ext);
      direct = constraints->size() == 0;
   }
   else {
      auto* R = cast<RecordDecl>(DC);
      conformanceTypes = R->getConformanceTypes();
      direct = true;

      auto extensions = QC.Context.getExtensions(QC.Context.getRecordType(R));
      for (auto* directExt : extensions) {
         if (registerConformances(Self, directExt, testSet,
                                  directConformances, baseConf)) {
            return true;
         }
      }
   }

   ArrayRef<ProtocolDecl*> protocols;
   if (QC.ResolveConformancesToProtocols(protocols, Self, conformanceTypes)) {
      return true;
   }

   for (auto* proto : protocols) {
      if (registerConformance(Self, proto, constraints, testSet,
                              directConformances, DC, baseConf, direct)) {
         return true;
      }
   }

   return false;
}

ConstraintSet* ConformanceResolver::getDependentConstraints(ConstraintSet* CS,
                                                            QualType Self)
{
   if (!CS) {
      return nullptr;
   }

   SmallVector<DeclConstraint*, 2> dependentConstraints;
   for (auto* C : *CS) {
      QualType constrainedType = C->getConstrainedType();
      if (constrainedType->containsTemplateParamType()) {
         dependentConstraints.push_back(C);
         continue;
      }
      if (constrainedType->containsAssociatedType()) {
         if (QC.SubstAssociatedTypes(constrainedType, constrainedType, Self, {})) {
            continue;
         }

         if (constrainedType->containsTemplateParamType()) {
            dependentConstraints.push_back(C);
            continue;
         }
      }
   }

   return ConstraintSet::Create(QC.Context, dependentConstraints);
}

bool ConformanceResolver::isTemplateMember(NamedDecl* impl)
{
   auto* ND = cast<NamedDecl>(impl->getNonTransparentDeclContext());
   if (auto *Ext = dyn_cast<ExtensionDecl>(ND))
      ND = Ext->getExtendedRecord();

   return ND->isTemplate();
}

bool ConformanceResolver::PrepareMacros(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<MacroExpansionDecl>()) {
      SemaPass::DeclScopeRAII DSR(*QC.Sema, Decl->getDeclContext());
      if (!QC.Sema->checkNamespaceRef(Decl))
         return true;

      DeclContext* Ctx = Decl->getDeclContext();
      if (auto* Ident = cast_or_null<IdentifierRefExpr>(Decl->getParentExpr())) {
         if (Ident->getKind() == IdentifierKind::Namespace) {
            Ctx = Ident->getNamespaceDecl();
         }
         else if (Ident->getKind() == IdentifierKind::Import) {
            Ctx = Ident->getImport();
         }
         else if (Ident->getKind() == IdentifierKind::Module) {
            Ctx = Ident->getModule();
         }
      }

      StmtOrDecl Result;
      if (QC.ExpandMacro(Result, Decl, Decl->getMacroName(), Ctx,
                         Decl->getDelim(),Decl->getTokens(),
                         (unsigned)parse::Parser::ExpansionKind::Decl)) {
         fail = true;
      }
   }

   return fail;
}

bool ConformanceResolver::PrepareStaticDecls(DeclContext *DC)
{
   bool fail = false;
   for (auto* Decl : DC->getDecls()) {
      switch (Decl->getKind()) {
      case Decl::StaticIfDeclID: {
         auto* If = cast<StaticIfDecl>(Decl);
         ast::Decl* Result;

         fail |= QC.ResolveStaticIf(Result, If);
         break;
      }
      case Decl::StaticForDeclID: {
         auto* For = cast<StaticForDecl>(Decl);
         ast::Decl* Result;

         fail |= QC.ResolveStaticFor(Result, For);
         break;
      }
      default:
         break;
      }
   }

   return fail;
}

bool ConformanceResolver::PrepareImports(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<ImportDecl>()) {
      fail |= QC.ResolveImport(Decl);
   }

   return fail;
}

bool ConformanceResolver::PrepareImplicitDecls(DeclContext *DC)
{
   auto *R = dyn_cast<RecordDecl>(DC);
   if (!R) {
      return false;
   }

   // Make sure implicit initializers / deinitializers are declared.
   if (QC.DeclareImplicitInitializers(R)) {
      return true;
   }

   // Resolve the initializer names and declare base ones.
   for (auto* Init : R->getDecls<InitDecl>()) {
      if (QC.AssignInitName(Init)) {
         return true;
      }
      if (auto Err = QC.CreateBaseInit(Init)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::FindDeclContexts(DeclContext *DC, bool includeFunctions)
{
   if (!DoneSet.insert(DC).second) {
      return false;
   }
   if (isa<Decl>(DC) && cast<Decl>(DC)->isExternal()) {
      return false;
   }

   DeclContexts.push_back(DC);

   if (PrepareMacros(DC)) {
      return true;
   }

   if (PrepareStaticDecls(DC)) {
      return true;
   }

   if (PrepareImports(DC)) {
      return true;
   }

   auto *Rec = dyn_cast<RecordDecl>(DC);
   if (!Rec) {
      QC.Sema->updateLookupLevel(DC, LookupLevel::Complete);
   }

   for (auto *InnerDC : DC->getDecls<DeclContext>()) {
      if (isa<CallableDecl>(InnerDC) && !includeFunctions) {
         continue;
      }

      if (FindDeclContexts(InnerDC, includeFunctions)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::BuildWorklist()
{
   bool error = false;

   // Resolve protocols first.
   for (auto *DC : DeclContexts) {
      auto *Proto = dyn_cast<ProtocolDecl>(DC);
      if (!Proto) {
         continue;
      }

      error |= BuildWorklist(DC);
   }

   error |= ResolveConformanceDependencies();

   for (auto *DC : DeclContexts) {
      if (isa<ProtocolDecl>(DC)) {
         continue;
      }

      error |= BuildWorklist(DC);
   }

   for (auto *DC : DeclContexts) {
      auto *R = dyn_cast<RecordDecl>(DC);
      if (!R || isa<ProtocolDecl>(R))
         continue;

      error |= FindDependencies(R, *Cache[BaseConformances[R]]);
   }

   DeclContexts.clear();
   return error;
}

bool ConformanceResolver::BuildWorklist(DeclContext *DC)
{
   auto *Rec = dyn_cast<RecordDecl>(DC);
   if (!Rec) {
      return false;
   }

   SemaPass::DeclScopeRAII Scope(*QC.Sema, DC);

   // Instantiations are handled separately.
   if (Rec->isInstantiation()) {
      if (BuildWorklistForShallowInstantiation(Rec)) {
         return true;
      }

      auto *Template = Rec->getSpecializedTemplate();
      if (Template->isExternal()) {
         if (CreateUncheckedConformanceForImportedDecl(Template)) {
            return true;
         }
      }

      return FindDependencies(Rec, *Cache[BaseConformances[Template]]);
   }

   // Add implicit conformance to Any.
   if (auto* Any = QC.Sema->getAnyDecl()) {
      if (Rec != Any) {
         ConfTbl.addConformance(
            QC.Context, ConformanceKind::Implicit, Rec, Any, Rec,
            QC.Context.EmptyConstraintSet);

         if (auto *Proto = dyn_cast<ProtocolDecl>(Rec)) {
            Proto->setSpecificity(1);
         }
      }
   }

   // Make sure all extensions that apply to this type are resolved and their
   // conformances added.
   CanType Self = QC.Context.getRecordType(Rec);
   if (QC.FindExtensions(Self)) {
      return true;
   }

   // Find all declared conformances of this type.
   SmallDenseSet<uintptr_t, 4> testSet;

   // Conformances provided directly by the type, ignore potential protocol
   // extensions that also provide these.
   SmallPtrSet<ProtocolDecl*, 4> directConformances;

   // Use an empty conditional conformance object to simplify the recursive
   // algorithm.
   UncheckedConformance &baseConf = CreateCondConformance();
   baseConf.initializerInnerConformances();
   BaseConformances[Rec] = Cache.size() - 1;

   // Gather all (conditional) conformances.
   if (registerConformances(Self, Rec, testSet, directConformances, baseConf)) {
      return true;
   }

   // Make sure 'Self' and template parameters are ready.
   if (QC.DeclareSelfAlias(Rec) || QC.PrepareTemplateParameters(Rec)) {
      return true;
   }

   LOG(ConformanceHierarchy, Rec->getDeclName(), ": \n", baseConf.indented());
   QC.Sema->updateLookupLevel(Rec, LookupLevel::Conformances);

   return false;
}

bool
ConformanceResolver::CreateUncheckedConformanceForImportedDecl(RecordDecl* Rec)
{
   if (BaseConformances.find(Rec) != BaseConformances.end()) {
      return false;
   }

   // Use an empty conditional conformance object to simplify the recursive
   // algorithm.
   UncheckedConformance &baseConf = CreateCondConformance();
   baseConf.initializerInnerConformances();
   BaseConformances[Rec] = Cache.size() - 1;

   auto AllConformances = QC.Context.getConformanceTable().getAllConformances(Rec);
   for (auto *C : AllConformances) {
      UncheckedConformance &innerConf = CreateCondConformance(
          C->getProto(), C->getConstraints(), C->getDeclarationCtx(),
          &baseConf);

      innerConf.depth = C->getDepth();
      baseConf.innerConformances->emplace_back(move(innerConf));
   }

   return false;
}

static bool AddConstraintDependencies(
    QueryContext &QC,
    ConformanceResolver &Resolver,
    ConformanceResolver::DependencyVertex &ConfNode,
    ConstraintSet *CS,
    RecordDecl *R,
    AssociatedTypeDecl *Exclude = nullptr)
{
   bool isNew = false;
   auto &CSNode = Resolver.GetDependencyNode(
       DependencyNode::ConstraintsSatisfied, R, CS, &isNew);

   ConfNode.addIncoming(&CSNode);

   if (!isNew) {
      return false;
   }

   SmallVector<ConformanceResolver::DependencyVertex*, 2> Nodes;
   for (auto *C : *CS) {
      auto *AT = C->getConstrainedType()->asAssociatedType();
      if (!AT || AT->getDecl() == Exclude) {
         continue;
      }

      QualType Outer = AT->getOuterAT();
      if (!Outer) {
         continue;
      }

      bool include = true;
      auto *OuterAT = Outer->asAssociatedType();

      while (OuterAT) {
         if (OuterAT->getDecl() == Exclude) {
            include = false;
            break;
         }

         if (OuterAT->getDecl()->isSelf()) {
            break;
         }

         OuterAT = OuterAT->getOuterAT()->asAssociatedType();
      }

      if (!include) {
         continue;
      }

      Nodes.push_back(&CSNode);

      while ((OuterAT = Outer->asAssociatedType())) {
         if (OuterAT->getDecl()->isSelf()) {
            auto &ATNode = Resolver.GetDependencyNode(
                DependencyNode::ConcreteTypeOfAssociatedType, R, AT->getDecl());

            Nodes.push_back(&ATNode);
            break;
         }
         else {
            auto &NestedATNode = Resolver.GetDependencyNode(
                DependencyNode::NestedAssociatedType, R, AT);

            Nodes.push_back(&NestedATNode);
         }

         AT = OuterAT;
         Outer = OuterAT->getOuterAT();
      }

      for (int i = 0; i < Nodes.size() - 1; ++i) {
         Nodes[i]->addIncoming(Nodes[i + 1]);
      }

      Nodes.clear();
   }

   return false;
}

bool
ConformanceResolver::AddConformanceDependency(DependencyVertex &Node,
                                              RecordDecl *R, ProtocolDecl *P)
{
   bool isNew = false;
   auto &ConfNode = GetDependencyNode(
       DependencyNode::SpecificConformance, R, P, &isNew);

   Node.addIncoming(&ConfNode);

   if (!isNew) {
      return false;
   }

   for (auto *AT : P->getDecls<AssociatedTypeDecl>()) {
      if (AT->isSelf()) {
         continue;
      }

      auto& ATNode = GetDependencyNode(
          DependencyNode::ConcreteTypeOfAssociatedType, R, AT);

      ConfNode.addIncoming(&ATNode);
   }

   auto &baseConf = *Cache[BaseConformances[
       R->isInstantiation() ? R->getSpecializedTemplate() : R]];

   bool found = false;
   bool error = false;

   baseConf.ForEach([&](UncheckedConformance &conf) {
      if (conf.proto != P) {
         return true;
      }

      found = true;

      if (!conf.combinedConstraints || conf.combinedConstraints->empty()) {
         return true;
      }

      if (AddConstraintDependencies(QC, *this, ConfNode, conf.combinedConstraints, R)) {
         error = true;
         return false;
      }

      return true;
   });

   if (error) {
      return true;
   }
   if (found) {
      return false;
   }

   // Check for a builtin conformance.
   auto *Equatable = QC.Sema->getEquatableDecl();
   auto *Hashable = QC.Sema->getHashableDecl();
   auto *StrRep = QC.Sema->getStringRepresentableDecl();

   if (P != Equatable || P != Hashable || P != StrRep) {
      return false;
   }

   TypeDependencyFinder Finder(QC, *this, ConfNode, R, P);

   if (auto *S = dyn_cast<StructDecl>(R)) {
      for (auto *F : S->getFields()) {
         if (QC.PrepareDeclInterface(F)) {
            return true;
         }

         Finder.visit(F->getType().getResolvedType());
      }
   }
   else if (auto *E = dyn_cast<EnumDecl>(R)) {
      for (auto *C : E->getCases()) {
         if (QC.PrepareDeclInterface(C)) {
            return true;
         }

         for (auto *Val : C->getArgs()) {
            Finder.visit(Val->getType().getResolvedType());
         }
      }
   }

   return Finder.hasError();
}

static bool FindPossibleAssociatedTypeImpls(
    QueryContext &QC,
    RecordDecl *R,
    AssociatedTypeDecl *AT,
    UncheckedConformance &conf,
    UncheckedConformance &baseConf,
    llvm::SetVector<std::pair<NamedDecl*, ConstraintSet*>> &Result)
{
   auto *LookupCtx = R->isInstantiation() ? R->getSpecializedTemplate() : R;

   // First, look in the record itself and in its unconstrained extensions - if
   // we find an implementation there, stop looking.
   auto *Impl = LookupCtx->lookupSingle<AliasDecl>(AT->getDeclName());
   if (Impl) {
      Result.insert(std::make_pair(Impl, (ConstraintSet*)nullptr));
   }

   for (auto *Ext : LookupCtx->getExtensions()) {
      auto *CS = QC.Context.getExtConstraints(Ext);
      Impl = Ext->lookupSingle<AliasDecl>(AT->getDeclName());

      if (Impl) {
         Result.insert(std::make_pair(Impl, CS));
      }
   }

   if (!Result.empty()) {
      return false;
   }

   // Now look in protocol extensions.
   int minDepth = INT_MAX;
   baseConf.ForEach([&](UncheckedConformance &innerConf) {
      if (minDepth < innerConf.depth) {
         return true;
      }

     auto *ReDecl = innerConf.proto->lookupSingle<AssociatedTypeDecl>(
          AT->getDeclName());

     if (ReDecl && ReDecl->getDefaultType()) {
        Result.insert(std::make_pair(ReDecl, (ConstraintSet*)nullptr));
        minDepth = innerConf.depth;
     }

      for (auto *Ext : innerConf.proto->getExtensions()) {
         auto *Impl = Ext->lookupSingle<AliasDecl>(AT->getDeclName());
         if (!Impl) {
            continue;
         }

         auto *CS = QC.Context.getExtConstraints(Ext);
         Result.insert(std::make_pair(Impl, CS));

         minDepth = innerConf.depth;
      }

     return true;
   });

   return false;
}

static void diagnoseMissingAssociatedTypes(
    QueryContext &QC,
    RecordDecl *R,
    ArrayRef<AssociatedTypeDecl*> MissingATs)
{
   llvm::DenseMap<ProtocolDecl*, std::vector<AssociatedTypeDecl*>> Map;
   for (auto *AT : MissingATs) {
      Map[cast<ProtocolDecl>(AT->getDeclContext())].push_back(AT);
   }

   for (auto &[Proto, ATs] : Map) {
      QC.Sema->diagnose(R, err_incorrect_protocol_impl, R,
                        R->getDeclName(), Proto->getDeclName(),
                        R->getSourceLoc());

      for (auto* AT : ATs) {
         QC.Sema->diagnose(note_incorrect_protocol_impl_missing, AT,
                           AT->getDeclName(), AT->getSourceLoc());
      }
   }
}

static bool AddTypeDependencies(QueryContext &QC,
                                ConformanceResolver &Resolver,
                                QualType TypeExpr,
                                ConformanceResolver::DependencyVertex &Vert,
                                RecordDecl *R,
                                SmallPtrSetImpl<IdentifierInfo*> &NeededATs)
{
   TypeDependencyFinder Finder(QC, Resolver, Vert, R);
   Finder.visit(TypeExpr);

   return Finder.hasError();
}

bool ConformanceResolver::FindDependencies(RecordDecl *R,
                                           UncheckedConformance &baseConf)
{
   if (isa<ProtocolDecl>(R)) {
      return false;
   }

   auto& CompleteNode = GetDependencyNode(DependencyNode::Complete, R);
   return FindDependencies(R, baseConf, CompleteNode);
}

bool ConformanceResolver::FindDependencies(RecordDecl *R,
                                           UncheckedConformance &baseConf,
                                           DependencyVertex &CompleteNode)
{
   if (R->isInstantiation()) {
      LOG(ConformanceHierarchy, R->getDeclName(), ": \n", baseConf.indented());
   }

   SmallSetVector<AssociatedTypeDecl*, 2> MissingATs;
   SmallPtrSet<IdentifierInfo*, 4> NeededATNames;
   SmallSetVector<std::pair<QualType, DependencyVertex*>, 4> TypesToCheck;

   bool error = false;
   baseConf.ForEach([&](UncheckedConformance &conf) {
     // Add the conformance dependencies.
     error |= AddConformanceDependency(CompleteNode, R, conf.proto);

     // Add the extension applicability dependencies.
      for (auto *Ext : conf.proto->getExtensions()) {
         auto *CS = QC.Context.getExtConstraints(Ext);
         if (!CS || CS->empty()) {
            continue;
         }

         auto &ExtNode = GetDependencyNode(
             DependencyNode::ExtensionApplicability, R, Ext);

         CompleteNode.addIncoming(&ExtNode);
         error |= AddConstraintDependencies(QC, *this, ExtNode, CS, R);
      }

      // Add the associated type dependencies.
      for (auto *AT : conf.proto->getDecls<AssociatedTypeDecl>()) {
         if (AT->isSelf()) {
            continue;
         }

         NeededATNames.insert(AT->getDeclName().getIdentifierInfo());

         auto &Impls = PossibleAssociatedTypeImpls[R][AT];
         auto &ATNode = GetDependencyNode(
             DependencyNode::ConcreteTypeOfAssociatedType, R, AT);

         if (FindPossibleAssociatedTypeImpls(QC, R, AT, conf, baseConf, Impls)) {
            error = true;
            continue;
         }

         if (Impls.empty()) {
            MissingATs.insert(AT);
            error = true;
            continue;
         }

         for (auto [Impl, CS] : Impls) {
            if (QC.PrepareDeclInterface(Impl)) {
               error = true;
               continue;
            }

            if (auto *ATImpl = dyn_cast<AssociatedTypeDecl>(Impl)) {
               TypesToCheck.insert(std::make_pair(ATImpl->getDefaultType(), &ATNode));
            }
            else {
               TypesToCheck.insert(std::make_pair(
                   cast<AliasDecl>(Impl)->getType(), &ATNode));
            }

            if (CS && !CS->empty()) {
               error |= AddConstraintDependencies(QC, *this, ATNode, CS, R);
            }
         }

         // Add dependencies to the associated types referenced in the constraints.
         auto *CS = QC.Context.getExtConstraints(AT);
         if (CS && !CS->empty()) {
            error |= AddConstraintDependencies(QC, *this, ATNode, CS, R, AT);
         }
      }

      return true;
   });

   if (!MissingATs.empty()) {
      diagnoseMissingAssociatedTypes(QC, R, MissingATs.takeVector());
      return true;
   }

   if (error) {
      return true;
   }

   for (auto [TyExpr, Node] : TypesToCheck) {
      error |= AddTypeDependencies(QC, *this, TyExpr, *Node, R, NeededATNames);
   }

   if (!R->isShallowInstantiation()) {
      return FindLayoutDependencies(R, CompleteNode);
   }

   return false;
}

bool ConformanceResolver::FindLayoutDependencies(
    RecordDecl* R, DependencyGraph<DependencyNode*>::Vertex& CompleteNode)
{
   if (isa<ClassDecl>(R) || isa<ProtocolDecl>(R)) {
      return false;
   }

   SmallSetVector<QualType, 4> TypesToCheck;
   if (auto *S = dyn_cast<StructDecl>(R)) {
      for (auto *F : S->getStoredFields()) {
         if (QC.PrepareDeclInterface(F)) {
            continue;
         }

         TypesToCheck.insert(F->getType().getResolvedType());
      }
   }
   else if (auto *E = dyn_cast<EnumDecl>(R)) {
      for (auto *EC : E->getCases()) {
         if (EC->isIndirect() || QC.PrepareDeclInterface(EC)) {
            continue;
         }

         for (auto *Val : EC->getArgs()) {
            TypesToCheck.insert(Val->getType().getResolvedType());
         }
      }
   }

   for (auto &Ty : TypesToCheck) {
      visitSpecificType<RecordType, FunctionType, PointerType, ReferenceType>(
          [&](QualType T) {
        // Functions, pointers and references do not actually store
        // their contained types.
        if (!T->isRecordType()) {
           return false;
        }
        if (T->isProtocol()) {
           return false;
        }

        auto &OtherNode = GetDependencyNode(
            DependencyNode::Complete, T->getRecord());

        if (OtherNode.getVal()->Done) {
           return false;
        }

        CompleteNode.addIncoming(&OtherNode);
        return false;
      }, Ty);
   }

   return false;
}

bool ConformanceResolver::BuildWorklistForShallowInstantiation(RecordDecl *Inst)
{
   auto *Template = Inst->getSpecializedTemplate();
   auto &CompleteNode = GetDependencyNode(DependencyNode::Complete, Inst);

   if (IsBeingResolved(Template)) {
      auto& TemplateNode
          = GetDependencyNode(DependencyNode::Complete, Template);
      CompleteNode.addIncoming(&TemplateNode);
   }

   for (auto *Ext : Template->getExtensions()) {
      auto *CS = QC.Context.getExtConstraints(Ext);
      if (!CS || CS->empty()) {
         continue;
      }

      if (AddConstraintDependencies(QC, *this, CompleteNode, CS, Inst)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::ResolveDependencyNode(DependencyNode *Node,
                                                bool &error,
                                                bool &shouldReset)
{
   if (Node->Done) {
      return false;
   }
   if (HasNewDependencies(Node)) {
      shouldReset = true;
      return false;
   }

   switch (Node->Kind) {
   case DependencyNode::Complete: {
      auto *Rec = cast<RecordDecl>(Node->Decl);
      assert(!isa<ProtocolDecl>(Rec));

      auto &Instantiator = QC.Sema->getInstantiator();
      if (Instantiator.isShallowInstantiation(Rec)) {
         if (Instantiator.completeShallowInstantiation(Rec)) {
            return true;
         }
         if (FindLayoutDependencies(Rec, Dependencies.getOrAddVertex(Node))) {
            return true;
         }
         if (HasNewDependencies(Node)) {
            shouldReset = true;
            return false;
         }
      }

      PendingRecordDecls.insert(Rec);
      LOG(AssociatedTypeImpls, Rec->getFullName(), " ", PrintAssociatedTypes(QC, Rec));

      if (PrepareImplicitDecls(Rec)) {
         error = true;
         Rec->setIsInvalid(true);
         return true;
      }

      QualType T = QC.Context.getRecordType(Rec);
      if (QC.CheckConformances(T)) {
         error = true;
         Rec->setIsInvalid(true);
         Node->Done = true;

         return false;
      }

      CompletedRecordDecls.insert(Rec);
      break;
   }
   case DependencyNode::ConcreteTypeOfAssociatedType: {
      auto *R = cast<RecordDecl>(Node->Decl);
      auto *AT = cast<AssociatedTypeDecl>(Node->getOtherDecl());

      auto key = std::make_pair(R, AT->getDeclName().getIdentifierInfo());
      auto it = ImplementedATs.find(key);

      if (it != ImplementedATs.end()) {
         QC.Context.addProtocolImpl(R, AT, it->getSecond());
         QC.Sema->registerAssociatedTypeImpl(R, AT, it->getSecond());
         Node->Done = true;

         return false;
      }

      SemaPass::DeclScopeRAII DSR(*QC.Sema, R);

      QualType Self = QC.Context.getRecordType(R);
      auto &possibleImpls = PossibleAssociatedTypeImpls[R][AT];

      AliasDecl *foundImpl = nullptr;
      for (auto [Impl, CS] : possibleImpls) {
         if (CS && !CS->empty()) {
            bool Satisfied;
            for (DeclConstraint *C : *CS) {
               if (auto Err = QC.IsConstraintSatisfied(Satisfied, C, R, Impl)) {
                  if (!Err.isDependent()) {
                     error = true;
                  }

                  Satisfied = true;
               }

               if (!Satisfied) {
                  break;
               }
            }

            if (!Satisfied) {
               continue;
            }
         }

         if (foundImpl) {
            QC.Sema->diagnose(err_conflicting_associated_type_impls,
                R->getFullName(), AT->getFullName(), R->getSourceLoc());

            QC.Sema->diagnose(note_candidate_here, foundImpl->getSourceLoc());
            QC.Sema->diagnose(note_candidate_here, Impl->getSourceLoc());

            error = true;
            break;
         }

         if (auto *DefaultImpl = dyn_cast<AssociatedTypeDecl>(Impl)) {
            foundImpl = makeAssociatedType(*QC.Sema, DefaultImpl, R);
         }
         else if (Impl->isDefault()) {
            auto* inst = QC.Sema->getInstantiator().InstantiateProtocolDefaultImpl(
                Impl, Self);

            if (!inst) {
               return true;
            }

            foundImpl = cast<AliasDecl>(inst);
         }
         else if (isTemplateMember(Impl) && R->isInstantiation()) {
            auto* inst = QC.Sema->getInstantiator().InstantiateTemplateMember(
                Impl, Self->getRecord());

            if (!inst) {
               return true;
            }

            foundImpl = cast<AliasDecl>(inst);
         }
         else {
            foundImpl = cast<AliasDecl>(Impl);
         }

         foundImpl->setImplOfProtocolRequirement(true);
         QC.Context.addProtocolImpl(R, AT, foundImpl);
         QC.Sema->registerAssociatedTypeImpl(R, AT, foundImpl);

         ImplementedATs[key] = foundImpl;
      }

      break;
   }
   case DependencyNode::SpecificConformance: {
      auto *R = cast<RecordDecl>(Node->Decl);
      auto *P = Node->getProtocol();

      QualType Self = QC.Context.getRecordType(R);
      auto &baseConf = *Cache[BaseConformances[
         R->isInstantiation() ? R->getSpecializedTemplate() : R]];

      bool found = false;
      baseConf.ForEach([&](UncheckedConformance &conf) {
         if (found) {
            return false;
         }

         if (conf.proto != P) {
            return true;
         }

         if (!R->isInstantiation()) {
            ConstraintSet* dependentConstraints = nullptr;
            if (conf.combinedConstraints
                && !conf.combinedConstraints->empty()) {
               dependentConstraints
                   = getDependentConstraints(conf.combinedConstraints, Self);
            }

            if (dependentConstraints && !dependentConstraints->empty()) {
               ConfTbl.addConformance(QC.Context, ConformanceKind::Conditional,
                                      Self->getRecord(), conf.proto,
                                      conf.introducedBy, dependentConstraints,
                                      conf.depth);

               return true;
            }
         }

        bool Satisfied = true;
        if (conf.combinedConstraints && !conf.combinedConstraints->empty()) {
           for (auto* C : *conf.combinedConstraints) {
              if (auto Err = QC.IsConstraintSatisfied(
                      Satisfied, C, R, cast<NamedDecl>(conf.introducedBy))) {
                 assert(!Err.isDependent() && "can this happen?");
                 return true;
              }
              if (!Satisfied) {
                 break;
              }
           }
        }

        if (!Satisfied) {
           return true;
        }

        found = true;
        ConfTbl.addConformance(QC.Context, ConformanceKind::Explicit,
                               Self->getRecord(), conf.proto,
                               conf.introducedBy, nullptr,
                               conf.depth);

         return true;
      });

      break;
   }
   case DependencyNode::NestedAssociatedType: {
      QualType Resolved = QC.Sema->ResolveNestedAssociatedType(
          Node->getType()->asAssociatedType()->getOuterAT(),
          QC.Context.getRecordType(cast<RecordDecl>(Node->Decl)));

      if (Resolved->isTemplateParamType()) {
         break;
      }

      auto &DepNode = GetDependencyNode(
          DependencyNode::ConcreteTypeOfAssociatedType, Resolved->getRecord(),
          Node->getType()->asAssociatedType()->getDecl());

      if (!DepNode.getVal()->Done) {
         Dependencies.getOrAddVertex(Node).addIncoming(&DepNode);
         if (!DepNode.getIncoming().empty()) {
            shouldReset = true;
         }

         return false;
      }

      break;
   }
   case DependencyNode::ExtensionApplicability:
   case DependencyNode::ConformancesOfRecord:
   case DependencyNode::ConstraintsSatisfied:
      // These are only used as markers.
      break;
   }

   Node->Done = true;
   return false;
}

#ifndef NDEBUG

static void LogDependencyGraph(DependencyGraph<DependencyNode*> &Graph,
                               bool valid)
{
   std::string str = "\n### CONSTRAINTS\n";
   llvm::raw_string_ostream OS(str);

   Graph.print([](DependencyNode *Node) {
     return Node->to_string();
   }, OS);

   if (!valid) {
      auto offending = Graph.getOffendingPair();
      OS << "\nCONFLICT ";
      offending.first->print(OS);
      OS << " <-> ";
      offending.second->print(OS);
      OS << "\n";
   }

   LOG(ConformanceDependencies, OS.str());
}

LLVM_ATTRIBUTE_UNUSED
void printStats(const DependencyGraph<DependencyNode*> &DG)
{
   int completeNodes = 0;
   int isolatedNodes = 0;
   auto size = DG.getVertices().size();

   for (auto &Vert : DG.getVertices()) {
      if (Vert->getVal()->Done)
         ++completeNodes;
      if (Vert->getIncoming().empty()&&Vert->getOutgoing().empty())
         ++isolatedNodes;
   }

   llvm::errs() << "-------\n";
   llvm::errs() << "Done: " << completeNodes << " / " << size << " (" << (int)(((float)completeNodes/(float)size)*100.f) << "%)\n";
   llvm::errs() << "Isolated: " << isolatedNodes << " / " << size << " (" << (int)(((float)isolatedNodes/(float)size)*100.f) << "%)\n";
   llvm::errs() << "-------\n";
}

LLVM_ATTRIBUTE_UNUSED
void verifyNode(DependencyGraph<DependencyNode*>::Vertex *V)
{
   assert(V->getVal()->Done && "unfinished node");
   for (auto *Inc : V->getIncoming())
      verifyNode(Inc);
}

LLVM_ATTRIBUTE_UNUSED
void verifyDAG(const DependencyGraph<DependencyNode*> &DG)
{
   for (auto *V : DG.getVertices()) {
      if (!V->getVal()->Done)
         continue;

      verifyNode(V);
   }
}

#endif

bool ConformanceResolver::ResolveDependencyGraph()
{
   llvm::SmallVector<DependencyNode*, 8> DependencyOrder;
   auto valid = Dependencies.constructOrderedList(DependencyOrder, true);

#ifndef NDEBUG
BEGIN_LOG(ConformanceDependencies)
   LogDependencyGraph(Dependencies, valid);
END_LOG
#endif

   assert(valid && "loop in conformance dependency graph!");

   for (auto *Vert : Dependencies.getVertices()) {
      NumDependencies[Vert->getVal()] = Vert->getIncoming().size();
   }

   bool error = false;
   bool shouldReset = false;

   size_t CurrentNodeIdx = 0;
   for (; CurrentNodeIdx < DependencyOrder.size(); ++CurrentNodeIdx) {
      auto *Node = DependencyOrder[CurrentNodeIdx];
      if (Node->Done) {
         continue;
      }

      LOG(ConformanceDependencies, "Resolving item ", CurrentNodeIdx + 1, " / ",
          DependencyOrder.size(), " (", Node->to_string(), ")");

      if (ResolveDependencyNode(Node, error, shouldReset)) {
         return true;
      }

      if (shouldReset) {
         shouldReset = false;
         CurrentNodeIdx = -1;

         Dependencies.remove_if([](DependencyNode *V) {
           return V->Done;
         });

         DependencyOrder.clear();
         valid = Dependencies.constructOrderedList(DependencyOrder, true);

#ifndef NDEBUG
         BEGIN_LOG(ConformanceDependencies)
            LogDependencyGraph(Dependencies, valid);
         END_LOG
#endif
         assert(valid && "loop in conformance dependency graph!");

         for (auto *Vert : Dependencies.getVertices()) {
            NumDependencies[Vert->getVal()] = Vert->getIncoming().size();
         }
      }
   }

   if (error) {
      return true;
   }

   if (CurrentNodeIdx < Dependencies.size()) {
      return ResolveDependencyGraph();
   }

   auto PendingDecls = move(PendingRecordDecls);
   PendingRecordDecls.clear();

   for (auto *Rec : PendingDecls) {
      if (QC.CheckAssociatedTypeConstraints(Rec)) {
         error = true;
         Rec->setIsInvalid(true);

         return false;
      }

      QC.Sema->updateLookupLevel(Rec, LookupLevel::Complete);
      LOG(ProtocolConformances, Rec->getFullName(), " ✅");
   }

   Dependencies.clear();
   NumDependencies.clear();
   Allocator.Reset();

   return error;
}

bool ConformanceResolver::ResolveConformanceDependencies()
{
   if (ConformanceDAG.empty())
      return false;

   SmallVector<ProtocolDecl*, 16> Protocols;
   bool valid = ConformanceDAG.constructOrderedList(Protocols);
   if (!valid) {
      assert(QC.Sema->encounteredError() && "should have been diagnosed!");
      ConformanceDAG.clear();
      return true;
   }

   for (auto *P : Protocols) {
      unsigned Specificity = P->getSpecificity();
      for (auto *Conf : ConfTbl.getAllConformances(P)) {
         Specificity = std::max(Specificity,
                                Conf->getProto()->getSpecificity() + 1);
      }

      P->setSpecificity(Specificity);
      LOG(ProtocolSpecificity, P->getDeclName(), ": ", Specificity);
   }

   ConformanceDAG.clear();
   return false;
}

bool SemaPass::PrepareNameLookup(DeclContext *DC)
{
   START_TIMER("Preparing Name Lookup");

   auto &Resolver = getConformanceResolver();
   auto SAR = support::saveAndRestore(Instantiator->InstantiateShallowly, true);

   if (Resolver.FindDeclContexts(DC, isa<CallableDecl>(DC))) {
      return true;
   }
   if (Resolver.BuildWorklist()) {
      return true;
   }

   return Resolver.ResolveDependencyGraph();
}

bool SemaPass::FindDependencies(RecordDecl *Inst)
{
   auto &Resolver = getConformanceResolver();
   return Resolver.BuildWorklist(Inst);
}

bool SemaPass::UncheckedConformanceExists(RecordDecl *R, ProtocolDecl *P)
{
   auto &Resolver = getConformanceResolver();
   auto key = std::make_pair(R, P);

   return Resolver.PotentialConformances.count(key) != 0;
}

void SemaPass::registerAssociatedTypeImpl(RecordDecl *R,
                                          AssociatedTypeDecl *AT,
                                          AliasDecl *Impl) {
   auto &ExistingImpl = AssociatedTypeDeclMap[R][AT->getDeclName()];
   assert((!ExistingImpl || ExistingImpl == Impl) && "duplicate associated type impl");

   ExistingImpl = Impl;
}

void SemaPass::registerAssociatedTypeImpl(RecordDecl *R, AliasDecl *Impl)
{
   auto &ExistingImpl = AssociatedTypeDeclMap[R][Impl->getDeclName()];
   assert((!ExistingImpl || ExistingImpl == Impl) && "duplicate associated type impl");

   ExistingImpl = Impl;
}

AliasDecl* SemaPass::getAssociatedTypeImpl(RecordDecl *R, DeclarationName Name)
{
   return AssociatedTypeDeclMap[R][Name];
}

QueryResult CheckConformancesQuery::run()
{
   if (T->getRecord()->isExternal()) {
      return finish();
   }

   ConformanceCheckerImpl Checker(*QC.Sema, T->getRecord());
   Checker.checkConformance();

   return finish(Checker.IssuedError ? Aborted : Done);
}

QueryResult CheckSingleConformanceQuery::run()
{
   ConformanceCheckerImpl Checker(*QC.Sema, T->getRecord());
   Checker.checkSingleConformance(P);

   return finish(Checker.IssuedError ? Aborted : Done);
}

QueryResult CheckAssociatedTypeConstraintsQuery::run()
{
   if (R->isInstantiation()) {
      return finish();
   }

   if (R->isInvalid()) {
      return Query::finish(DoneWithError);
   }

   QualType Self = QC.Context.getRecordType(R);
   if (auto Err = QC.CheckConformances(Self)) {
      return Query::finish(Err);
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, R);

   auto allConformances = QC.Context.getConformanceTable().getAllConformances(R);
   for (auto *Conf : allConformances) {
      for (auto *AT : Conf->getProto()->getDecls<AssociatedTypeDecl>()) {
         if (AT->isSelf()) {
            continue;
         }

         auto *Impl = cast<AliasDecl>(QC.Context.getProtocolImpl(R, AT));
         if (Impl->isInvalid()) {
            continue;
         }

         if (!Impl->isTypedef()) {
            QC.Sema->diagnose(
               err_associated_type_impl_must_be_type,
               Impl->getSourceRange());

            QC.Sema->diagnose(note_generic_note,
                              "implementation of associated type '"
                              + AT->getIdentifierInfo()->getIdentifier()
                              + "'",
                              AT->getSourceRange());

            continue;
         }

         auto *Proto = cast<ProtocolDecl>(AT->getRecord());
         QualType implType = Impl->getType()->removeMetaType();

         // If the type of the implentation is the associated type itself,
         // it must be fulfilled.
         if (auto *OtherAT = implType->asAssociatedType()) {
            if (OtherAT->getDecl() == AT) {
               continue;
            }
         }

         if (Conf->isConditional()) {
            implType = QC.Sema->ApplyCapabilities(implType, Conf->getDeclarationCtx());
         }

         // Check that the covariance is fulfilled.
         bool IsCovariant;
         if (!QC.IsCovariant(IsCovariant, implType, AT->getCovariance())
             && !IsCovariant) {
            QC.Sema->diagnose(
               err_generic_error,
               implType.toDiagString() + " is not covariant with "
               + AT->getCovariance().getResolvedType().toDiagString(),
               Impl->getSourceLoc());

            // This might make other constraints invalid, so stop here.
            return finish();
         }

         // Make sure the actual type is ready.
         if (auto *RT = implType->asRecordType()) {
            if (QC.PrepareDeclInterface(RT->getRecord())) {
               continue;
            }
         }

         // Check that constraints are satisfied.
         auto Constraints = QC.Sema->getDeclConstraints(AT);
         for (auto *C : *Constraints) {
            bool Satisfied;
            if (QC.IsConstraintSatisfied(Satisfied, C, R, Proto) || Satisfied) {
               continue;
            }

            std::string str;
            llvm::raw_string_ostream OS(str);
            QC.Sema->printConstraint(OS, C->getConstrainedType(), C, Self);

            QC.Sema->diagnose(err_associated_type_constraint,
                              Impl->getFullName(), implType, OS.str(),
                              Impl->getSourceLoc());

            QC.Sema->diagnose(note_constraint_here,
                              QC.Context.getConstraintLoc(AT, C));
         }
      }
   }

   return finish();
}

QueryResult ResolveConformanceToProtocolQuery::run()
{
   SourceLocation Loc;
   if (auto* E = Conf.getTypeExpr()) {
      Loc = E->getSourceLoc();
   }

   auto res = QC.Sema->visitSourceType(Conf);
   if (!res) {
      return fail();
   }

   auto protoTy = res.get();
   if (!protoTy->isRecordType()) {
      QC.Sema->diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                        protoTy, Loc);
      return fail();
   }

   auto Proto = protoTy->getRecord();
   if (!isa<ProtocolDecl>(Proto)) {
      QC.Sema->diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                        Proto->getDeclName(), Loc);

      return fail();
   }

   QC.CheckAccessibility(T->getRecord(), Proto, Loc);

   auto PD = cast<ProtocolDecl>(Proto);
   if (PD->isTemplate())
      return finish(PD, Dependent);

   return finish(PD);
}

QueryResult ResolveConformancesToProtocolsQuery::run()
{
   llvm::SmallVector<ProtocolDecl*, 2> newConformances;
   llvm::SmallPtrSet<ProtocolDecl*, 2> checkedConformances;

   Status S = Done;
   for (auto& Conf : Conformances) {
      ProtocolDecl* PD;
      if (QC.ResolveConformanceToProtocol(PD, T, Conf)) {
         S = DoneWithError;
         continue;
      }

      Conf.setResolvedType(QC.Context.getRecordType(PD));

      if (checkedConformances.insert(PD).second) {
         newConformances.push_back(PD);
      }
   }

   return finish(std::move(newConformances), S);
}

QueryResult ResolveExplicitConformancesQuery::run()
{
   MutableArrayRef<SourceType> Conformances;
   if (auto* R = T->asRecordType()) {
      Conformances = R->getRecord()->getConformanceTypes();
   }
   else {
      // FIXME generalized extensions
   }

   llvm::ArrayRef<ProtocolDecl*> protocols;
   if (auto Err
       = QC.ResolveConformancesToProtocols(protocols, T, Conformances)) {
      return Err;
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, T->getRecord()->getDeclContext());

   Status S = Done;
   for (auto* PD : protocols) {
      if (auto Err = QC.AddSingleConformance(T, PD, ConformanceKind::Explicit,
                                             nullptr, false)) {
         S = Err.isDependent() ? Dependent : DoneWithError;
      }
   }

   return finish(S);
}

QueryResult ResolveDeclaredConformancesQuery::run()
{
   llvm::ArrayRef<ProtocolDecl*> protocols;
   if (auto Err
       = QC.ResolveConformancesToProtocols(protocols, T, Conformances)) {
      return Err;
   }

   Status S = Done;
   for (auto* PD : protocols) {
      if (auto Err = QC.AddSingleConformance(T, PD, Kind, CS)) {
         S = Err.isDependent() ? Dependent : DoneWithError;
      }
   }

   return finish(S);
}

QueryResult AddSingleConformanceQuery::run()
{
   if (Proto->isTemplate()) {
      return finish(Dependent);
   }

   // Prepare the protocol for lookup.
   if (QC.ResolveExplicitConformances(QC.Context.getRecordType(Proto))) {
      return fail();
   }

   auto& Context = QC.CI.getContext();
   auto& ConfTable = Context.getConformanceTable();

   Conformance* NewConf = nullptr;
   bool IsNew = ConfTable.addConformance(Context, Kind, T->getRecord(), Proto,
                                         T->getRecord(), CS, 0, &NewConf);

   if (!IsNew) {
      return finish();
   }

   // Make sure to invalidate queries that depend on the number of conformances.
   if (auto* Q = QC.getQuery<GetNeededAssociatedTypesQuery>(T->getRecord())) {
      Q->invalidate();
   }

   Status S = Done;
   if (AddRecursiveConformances) {
      ConformanceKind K = Kind;
      if (K != ConformanceKind::Conditional) {
         K = ConformanceKind::Implicit;
      }

      // Add inherited conformances.
      for (auto* Conf : ConfTable.getAllConformances(Proto)) {
         ConstraintSet* newConstraints = nullptr;
         if (Conf->isConditional()) {
            newConstraints
                = ConstraintSet::Combine(QC.Context, CS, newConstraints);
         }
         else {
            newConstraints = CS;
         }

         if (auto Err = QC.AddSingleConformance(T, Conf->getProto(), K,
                                                newConstraints, true)) {
            S = Err.isDependent() ? Dependent : DoneWithError;
         }
      }
   }

   return finish(S);
}

QueryResult ConformsToQuery::run()
{
   QualType T = this->T->getDesugaredType();
   if (P->isAny()) {
      return finish(true);
   }

   // Protocols without static requirements conform to themselves.
   if (!P->hasStaticRequirements()) {
      if (T->isRecordType() && T->getRecord() == P) {
         return finish(true);
      }
   }

   // Values of 'Self' type used in protocol extensions conform to themselves.
   if (auto* AT = T->asAssociatedType()) {
      if (AT->getDecl()->isSelf() && AT->getDecl()->getRecord()->isProtocol()
          && AT->getDecl()->getRecord() == P) {
         return finish(true);
      }

      CanType covariance = AT->getDecl()->getCovariance().getResolvedType();
      if (covariance == T) {
         return finish(true);
      }
      if (covariance->isExistentialType()
          && covariance->asExistentialType()->contains(T)) {
         return finish(true);
      }
   }

   // Check builtin type conformances.
   if (auto *Builtin = T->asBuiltinType()) {
      if (P->getDeclName().isStr("_BuiltinPrimitiveIntegerType")) {
         switch (Builtin->getKind()) {
#define CDOT_BUILTIN_INT(NAME, BW, SIGNED) case Type::NAME: return finish(true);
#include "cdotc/Basic/BuiltinTypes.def"
         default:
            return finish(false);
         }
      }

      if (P->getDeclName().isStr("_BuiltinPrimitiveFloatingPointType")) {
         switch (Builtin->getKind()) {
#define CDOT_BUILTIN_FP(NAME, BW) case Type::NAME: return finish(true);
#include "cdotc/Basic/BuiltinTypes.def"
         default:
            return finish(false);
         }
      }

      if (P->getDeclName().isStr("_BuiltinPrimitiveType")) {
         switch (Builtin->getKind()) {
#define CDOT_BUILTIN_INT(NAME, BW, SIGNED) case Type::NAME: return finish(true);
#define CDOT_BUILTIN_FP(NAME, BW) case Type::NAME: return finish(true);
#include "cdotc/Basic/BuiltinTypes.def"
         default:
            return finish(false);
         }
      }
   }

   auto& ConfTable = QC.CI.getContext().getConformanceTable();
   if (auto* R = T->asRecordType()) {
      if (R->getRecord() == P) {
         return finish(true);
      }

      // Make sure all conformances are registered.
      if (QC.ResolveExplicitConformances(T)) {
         return finish(true, DoneWithError);
      }

      // If the conformance is already known, we can return here.
      if (ConfTable.conformsTo(R->getRecord(), P)) {
         return finish(true);
      }

      if (R->getRecord()->isTemplate()) {
         auto potentialConfs = ConfTable.getAllConformances(R->getRecord());
         for (auto *conf : potentialConfs) {
            if (conf->getProto() == P) {
               return finish(true, Dependent);
            }
         }
      }

      return finish(false);
   }
   else if (auto* Ext = T->asExistentialType()) {
      for (QualType EQ : Ext->getExistentials()) {
         bool Conforms;
         if (auto Err = QC.ConformsTo(Conforms, EQ, P)) {
            return Query::finish(Err);
         }

         if (!Conforms) {
            return finish(true);
         }
      }

      return finish(true);
   }
   else {
      // FIXME generalized extensions
      return finish(false);
   }
}