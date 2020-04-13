#include "ConstraintSystem.h"

#include "ConstraintStep.h"
#include "AST/TypeBuilder.h"
#include "AST/TypeVisitor.h"
#include "Query/QueryContext.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::support;

using PathElement = ConstraintLocator::PathElement;

void PathElement::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Kind, Data);
}

void PathElement::Profile(llvm::FoldingSetNodeID &ID,
                          LocatorKind Kind,
                          uint64_t Data) {
   ID.AddInteger(Kind);
   ID.AddInteger(Data);
}

ConstraintLocator::ConstraintLocator(Expression *Anchor,
                                     ArrayRef<PathElement> Elements)
   : Anchor(Anchor), NumElements((unsigned)Elements.size())
{
   std::copy(Elements.begin(), Elements.end(),
             reinterpret_cast<PathElement*>(this + 1));
}

void ConstraintLocator::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Anchor, getPathElements());
}

void ConstraintLocator::Profile(llvm::FoldingSetNodeID &ID,
                                Expression *Anchor,
                                ArrayRef<PathElement> Elements) {
   ID.AddPointer(Anchor);
   ID.AddInteger(Elements.size());

   for (auto &E : Elements) {
      E.Profile(ID);
   }
}

ConstraintLocator* ConstraintLocator::Create(llvm::BumpPtrAllocator &Allocator,
                                             Expression *Anchor,
                                             ArrayRef<PathElement> Elements) {
   size_t Size = sizeof(ConstraintLocator)
      + Elements.size() * sizeof(PathElement);
   void *Mem = Allocator.Allocate(Size, alignof(ConstraintLocator));

   return new(Mem) ConstraintLocator(Anchor, Elements);
}

Constraint::Constraint(ConstraintKind K,
                       TypeVariable Var,
                       Locator Loc)
   : Kind(K), ConstrainedType(Var), Loc(Loc)
{

}

QualType Constraint::getRHSType() const
{
   if (auto *RT = dyn_cast<RelationalConstraint>(this)) {
      return RT->getType();
   }
   if (auto *Member = dyn_cast<MemberConstraint>(this)) {
      return Member->getMemberType();
   }
   if (auto *Def = dyn_cast<DefaultableConstraint>(this)) {
      return Def->getType();
   }

   return QualType();
}

void Constraint::print(llvm::raw_ostream &OS) const
{
   switch (Kind) {
#  define CDOT_CONSTRAINT(NAME)                                            \
   case NAME##ID: {                                                        \
      static_assert(&Constraint::print != &NAME##Constraint::print,        \
                    "constraint subclass must override print method!");    \
      return static_cast<const NAME##Constraint*>(this)->print(OS);        \
   }
#  include "Constraints.def"
   }
}

void Constraint::dump() const
{
   print(llvm::errs());
}

llvm::raw_ostream &sema::operator<<(llvm::raw_ostream &OS, const Constraint &C)
{
   C.print(OS);
   return OS;
}

RelationalConstraint::RelationalConstraint(ConstraintKind K,
                                           TypeVariable Var,
                                           QualType RHS,
                                           Locator Loc)
   : Constraint(K, Var, Loc), Type(RHS)
{

}

TypePropertyConstraint::TypePropertyConstraint(ConstraintKind K,
                                               TypeVariable Var,
                                               Locator Loc)
   : Constraint(K, Var, Loc)
{

}

TypeBindingConstraint::TypeBindingConstraint(TypeVariable Var,
                                             QualType Type,
                                             Locator Loc)
   : RelationalConstraint(TypeBindingID, Var, Type, Loc)
{
   assert(!Type->containsTypeVariable()
      && "binding should not contain type variable!");
}

TypeBindingConstraint*
TypeBindingConstraint::Create(ConstraintSystem &Sys,
                              TypeVariable Var,
                              QualType Type,
                              Locator Loc) {
   return new(Sys) TypeBindingConstraint(Var, Type, Loc);
}

void TypeBindingConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " := " << Type;
}

TypeEqualityConstraint::TypeEqualityConstraint(TypeVariable Var,
                                               QualType Type,
                                               Locator Loc)
   : RelationalConstraint(TypeEqualityID, Var, Type, Loc)
{

}

TypeEqualityConstraint*
TypeEqualityConstraint::Create(ConstraintSystem &Sys,
                               TypeVariable Var,
                               QualType Type,
                               Locator Loc) {
   return new(Sys) TypeEqualityConstraint(Var, Type, Loc);
}

void TypeEqualityConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " == " << Type;
}

TypeInequalityConstraint::TypeInequalityConstraint(TypeVariable Var,
                                                   QualType Type,
                                                   Locator Loc)
   : RelationalConstraint(TypeInequalityID, Var, Type, Loc)
{

}

TypeInequalityConstraint*
TypeInequalityConstraint::Create(ConstraintSystem &Sys,
                                 TypeVariable Var,
                                 QualType Type,
                                 Locator Loc) {
   return new(Sys) TypeInequalityConstraint(Var, Type, Loc);
}

void TypeInequalityConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " != " << Type;
}

FunctionReturnTypeConstraint::FunctionReturnTypeConstraint(TypeVariable Var,
                                                           QualType Type,
                                                           Locator Loc)
   : RelationalConstraint(FunctionReturnTypeID, Var, Type, Loc)
{

}

FunctionReturnTypeConstraint*
FunctionReturnTypeConstraint::Create(ConstraintSystem &Sys,
                                     TypeVariable Var,
                                     QualType Type,
                                     Locator Loc) {
   return new(Sys) FunctionReturnTypeConstraint(Var, Type, Loc);
}

void FunctionReturnTypeConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " := return type of " << Type;
}

FunctionParamTypeConstraint::FunctionParamTypeConstraint(TypeVariable Var,
                                                         QualType Type,
                                                         unsigned ParamIdx,
                                                         Locator Loc)
   : RelationalConstraint(FunctionParamTypeID, Var, Type, Loc),
     ParamIdx(ParamIdx)
{

}

FunctionParamTypeConstraint*
FunctionParamTypeConstraint::Create(ConstraintSystem &Sys,
                                    TypeVariable Var,
                                    QualType Type,
                                    unsigned ParamIdx,
                                    Locator Loc) {
   return new(Sys) FunctionParamTypeConstraint(Var, Type, ParamIdx, Loc);
}

void FunctionParamTypeConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " <c type of parameter #" << ParamIdx
      << " of " << Type;
}

CovarianceConstraint::CovarianceConstraint(TypeVariable Var,
                                           QualType Covariance,
                                           bool Variadic,
                                           Locator Loc)
   : RelationalConstraint(CovarianceID, Var, Covariance, Loc),
     Variadic(Variadic)
{ }

CovarianceConstraint* CovarianceConstraint::Create(ConstraintSystem &Sys,
                                                   TypeVariable Var,
                                                   QualType Covariance,
                                                   bool Variadic,
                                                   Locator Loc) {
   return new(Sys) CovarianceConstraint(Var, Covariance, Variadic, Loc);
}

void CovarianceConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " <cov" << (Variadic ? "... " : " ") << Type;
}

InferenceConstraint::InferenceConstraint(TypeVariable Var,
                                         QualType Inference,
                                         Locator Loc)
   : RelationalConstraint(InferenceID, Var, Inference, Loc)
{ }

InferenceConstraint* InferenceConstraint::Create(ConstraintSystem &Sys,
                                                 TypeVariable Var,
                                                 QualType Inference,
                                                 Locator Loc) {
   return new(Sys) InferenceConstraint(Var, Inference, Loc);
}

void InferenceConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " inferrable via " << Type;
}

DefaultableConstraint::DefaultableConstraint(TypeVariable Var,
                                             QualType Type,
                                             Locator Loc)
   : Constraint(DefaultableID, Var, Loc),
     Type(Type)
{

}

DefaultableConstraint*
DefaultableConstraint::Create(ConstraintSystem &Sys,
                              TypeVariable Var,
                              QualType Type,
                              Locator Loc) {
   return new(Sys) DefaultableConstraint(Var, Type, Loc);
}

void DefaultableConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " is defaultable to " << Type;
}

ConformanceConstraint::ConformanceConstraint(TypeVariable Var,
                                             ProtocolDecl *ProtoDecl,
                                             Locator Loc)
   : TypePropertyConstraint(ConformanceID, Var, Loc),
     ProtoDecl(ProtoDecl)
{

}

ConformanceConstraint*
ConformanceConstraint::Create(ConstraintSystem &Sys,
                              TypeVariable Var,
                              ProtocolDecl *ProtoDecl,
                              Locator Loc) {
   return new(Sys) ConformanceConstraint(Var, ProtoDecl, Loc);
}

void ConformanceConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " conforms to " << ProtoDecl->getFullName();
}

ClassConstraint::ClassConstraint(TypeVariable Var,
                                 Locator Loc)
   : TypePropertyConstraint(ClassID, Var, Loc)
{

}

ClassConstraint* ClassConstraint::Create(ConstraintSystem &Sys,
                                         TypeVariable Var,
                                         Locator Loc) {
   return new(Sys) ClassConstraint(Var, Loc);
}

void ClassConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " is class";
}

HasDefaultValueConstraint::HasDefaultValueConstraint(TypeVariable Var,
                                                     Locator Loc)
   : TypePropertyConstraint(HasDefaultValueID, Var, Loc)
{

}

HasDefaultValueConstraint* HasDefaultValueConstraint::Create(ConstraintSystem &Sys,
                                                             TypeVariable Var,
                                                             Locator Loc) {
   return new(Sys) HasDefaultValueConstraint(Var, Loc);
}

void HasDefaultValueConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " has default value";
}

MemberConstraint::MemberConstraint(TypeVariable Var,
                                   DeclarationName MemberName,
                                   QualType MemberType,
                                   Locator Loc)
   : Constraint(MemberID, Var, Loc),
     MemberName(MemberName), MemberType(MemberType)
{

}

MemberConstraint* MemberConstraint::Create(ConstraintSystem &Sys,
                                           TypeVariable Var,
                                           DeclarationName MemberName,
                                           QualType MemberType,
                                           Locator Loc) {
   return new(Sys) MemberConstraint(Var, MemberName, MemberType, Loc);
}

void MemberConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << "." << MemberName << " == " << MemberType;
}

ImplicitConversionConstraint::ImplicitConversionConstraint(TypeVariable Var,
                                                           QualType Type,
                                                           Locator Loc)
   : RelationalConstraint(ImplicitConversionID, Var, Type, Loc)
{

}

ImplicitConversionConstraint*
ImplicitConversionConstraint::Create(ConstraintSystem &Sys,
                                     TypeVariable Var,
                                     QualType Type,
                                     Locator Loc) {
   return new(Sys) ImplicitConversionConstraint(Var, Type, Loc);
}

void ImplicitConversionConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " <c " << Type;
}

LiteralConstraint::LiteralConstraint(TypeVariable Var,
                                     LiteralKind LK,
                                     Locator Loc)
   : Constraint(LiteralID, Var, Loc),
     LK(LK)
{

}

LiteralConstraint* LiteralConstraint::Create(ConstraintSystem &Sys,
                                             TypeVariable Var,
                                             LiteralKind LK,
                                             Locator Loc) {
   return new(Sys) LiteralConstraint(Var, LK, Loc);
}

void LiteralConstraint::print(llvm::raw_ostream &OS) const
{
   OS << *ConstrainedType << " is a";
   switch (LK) {
   case IntegerLiteral: OS << "n integer literal"; break;
   case FPLiteral: OS << " floating point literal"; break;
   case BoolLiteral: OS << " bool literal"; break;
   case GraphemeClasterLiteral: OS << " grapheme cluster literal"; break;
   case CodePointLiteral: OS << " code point literal"; break;
   case ASCIILiteral: OS << "n ASCII literal"; break;
   case StringLiteral: OS << " string literal"; break;
   case ArrayLiteral: OS << "n array literal"; break;
   case DictionaryLiteral: OS << " dictionary literal"; break;
   case NoneLiteral: OS << " none literal"; break;
   }
}

QualType LiteralConstraint::getDefaultLiteralType(QueryContext &QC)
{
   GetBuiltinAliasQuery::AliasKind RK;
   switch (LK) {
   case LiteralConstraint::IntegerLiteral:
      RK = GetBuiltinAliasQuery::DefaultSignedIntegerLiteralType;
      break;
   case LiteralConstraint::FPLiteral:
      RK = GetBuiltinAliasQuery::DefaultFloatingPointLiteralType;
      break;
   case LiteralConstraint::BoolLiteral:
      RK = GetBuiltinAliasQuery::DefaultBooleanLiteralType;
      break;
   case LiteralConstraint::CodePointLiteral:
   case LiteralConstraint::ASCIILiteral:
   case LiteralConstraint::GraphemeClasterLiteral:
      RK = GetBuiltinAliasQuery::DefaultCharacterLiteralType;
      break;
   case LiteralConstraint::StringLiteral:
      RK = GetBuiltinAliasQuery::DefaultStringLiteralType;
      break;
   case LiteralConstraint::ArrayLiteral:
      RK = GetBuiltinAliasQuery::DefaultArrayLiteralType;
      break;
   case LiteralConstraint::DictionaryLiteral:
      RK = GetBuiltinAliasQuery::DefaultDictionaryLiteralType;
      break;
   case LiteralConstraint::NoneLiteral:
      return QualType();
   }

   AliasDecl *DefaultAlias;
   if (QC.GetBuiltinAlias(DefaultAlias, RK, &QC.Sema->getDeclContext())
       || !DefaultAlias
       || QC.PrepareDeclInterface(DefaultAlias)) {
      switch (LK) {
      case LiteralConstraint::IntegerLiteral:
         return QC.Context.getIntTy();
      case LiteralConstraint::FPLiteral:
         return QC.Context.getDoubleTy();
      case LiteralConstraint::BoolLiteral:
         return QC.Context.getBoolTy();
      case LiteralConstraint::CodePointLiteral:
         return QC.Context.getUInt32Ty();
      case LiteralConstraint::ASCIILiteral:
         return QC.Context.getUInt8Ty();
      case LiteralConstraint::StringLiteral:
         return QC.Context.getUInt8PtrTy();
      case LiteralConstraint::GraphemeClasterLiteral:
      case LiteralConstraint::ArrayLiteral:
      case LiteralConstraint::DictionaryLiteral:
      case LiteralConstraint::NoneLiteral:
         return QualType();
      }
   }

   return DefaultAlias->getType()->asMetaType()->getUnderlyingType();
}

DisjunctionConstraint::DisjunctionConstraint(ArrayRef<Constraint*> Constraints,
                                             Locator Loc,
                                             unsigned defaultValue)
   : Constraint(DisjunctionID, Constraints.front()->getConstrainedType(), Loc),
     NumConstraints((unsigned)Constraints.size()), defaultValue(defaultValue)
{
   std::copy(Constraints.begin(), Constraints.end(),
             getTrailingObjects<Constraint*>());
}

DisjunctionConstraint*
DisjunctionConstraint::Create(ConstraintSystem &Sys,
                              ArrayRef<Constraint*> Constraints,
                              Locator Loc,
                              unsigned defaultValue) {
   size_t Size = totalSizeToAlloc<Constraint*>(Constraints.size());
   void *Mem = Sys.Allocate(Size, alignof(DisjunctionConstraint));

   return new(Mem) DisjunctionConstraint(Constraints, Loc, defaultValue);
}

ArrayRef<Constraint*> DisjunctionConstraint::getConstraints() const
{
   return { getTrailingObjects<Constraint*>(), NumConstraints };
}

void DisjunctionConstraint::print(llvm::raw_ostream &OS) const
{
   unsigned i = 0;
   for (auto *C : getConstraints()) {
      if (i++ != 0) OS << " or ";
      C->print(OS);
   }
}

ConditionalConstraint::ConditionalConstraint(ArrayRef<Constraint*> Constraints,
                                             TypeVariableType *Disjunction,
                                             Locator Loc)
   : Constraint(ConditionalID, Constraints.front()->getConstrainedType(), Loc),
     Disjunction(Disjunction), NumConstraints((unsigned)Constraints.size())
{
   std::copy(Constraints.begin(), Constraints.end(),
             getTrailingObjects<Constraint*>());
}

ConditionalConstraint*
ConditionalConstraint::Create(ConstraintSystem &Sys,
                              ArrayRef<Constraint*> Constraints,
                              TypeVariableType *Disjunction,
                              Locator Loc) {
   size_t Size = totalSizeToAlloc<Constraint*>(Constraints.size());
   void *Mem = Sys.Allocate(Size, alignof(DisjunctionConstraint));

   return new(Mem) ConditionalConstraint(Constraints, Disjunction, Loc);
}

ArrayRef<Constraint*> ConditionalConstraint::getConstraints() const
{
   return { getTrailingObjects<Constraint*>(), NumConstraints };
}

Constraint* ConditionalConstraint::getConstraintAt(unsigned Index) const
{
   assert(Index < NumConstraints && "out of bounds!");
   return reinterpret_cast<Constraint* const*>(this + 1)[Index];
}

void ConditionalConstraint::print(llvm::raw_ostream &OS) const
{
   unsigned i = 0;
   for (auto *C : getConstraints()) {
      if (i++ != 0) OS << " or ";
      C->print(OS);
   }

   OS << " (depends on " << QualType(Disjunction) << ")";
}

ConstraintSystem::ConstraintSystem(QueryContext &QC,
                                   SourceRange Loc,
                                   llvm::raw_ostream *LogStream)
   : QC(QC), Loc(Loc), CG(*this), LogStream(LogStream)
{

}

void ConstraintSystem::setPreferredBinding(TypeVariableType *TypeVar,
                                           QualType T) {
   PreferredBindings[TypeVar] = T;
}

QualType ConstraintSystem::getPreferredBinding(TypeVariableType *TypeVar) const
{
   auto It = PreferredBindings.find(TypeVar);
   if (It != PreferredBindings.end()) {
      return It->getSecond();
   }

   return QualType();
}

ConstraintSystem::Solution::Solution(const AssignmentMapType &AssignmentMap,
                                     unsigned Score)
   : AssignmentMap(AssignmentMap), Score(Score)
{

}

ConstraintSystem::SolverScope::SolverScope(ConstraintSystem &Sys) : Sys(Sys)
{
   PreviousScope = Sys.CurrentSolverScope;
   PreviousNumChanges = Sys.CG.getNumMadeChanges();
   PreviousScore = Sys.CurrentScore;
   PreviousBestScore = Sys.BestScore;
   PreviousBoundTypeVars = Sys.BoundTypeVariables;

   Sys.CurrentSolverScope = this;
}

ConstraintSystem::SolverScope::~SolverScope()
{
   Sys.CurrentScore = PreviousScore;
   Sys.RecentBestScore = Sys.BestScore;
   Sys.BestScore = PreviousBestScore;
   Sys.BoundTypeVariables = PreviousBoundTypeVars;
   Sys.CG.undoChanges(PreviousNumChanges);
   Sys.CurrentSolverScope = PreviousScope;
}

void ConstraintSystem::registerConstraint(Constraint *Cons)
{
   CG.addConstraint(Cons);

   QualType RHSType = Cons->getRHSType();
   if (!RHSType || RHSType->containsTypeVariable()) {
      return;
   }

   if (auto *TB = dyn_cast<TypeBindingConstraint>(Cons)) {
      if (auto *TV = RHSType->asTypeVariableType()) {
         CG.makeEquivalent(Cons->getConstrainedType(), TV);
      }

      auto result = DirectBindingMap.try_emplace(
         Cons->getConstrainedType(), TB->getRHSType());

      (void)result;
      assert((result.second || result.first->getSecond() == TB->getRHSType())
         && "same type variable bound twice!");
   }
}

void ConstraintSystem::removeConstraint(Constraint *C)
{
   CG.removeConstraint(C);
}

TypeVariable ConstraintSystem::newTypeVariable(uint8_t Flags)
{
   auto *T = QC.Context.getTypeVariableType(NextTypeVariable++);
   TypeVariables.push_back(T);

   if (Flags != None) {
      TypeVariableFlagMap[T] = Flags;
   }
   if ((Flags & IsVariadicTemplateParameter) != 0) {
      CG.bindTypeVariable(T, QC.Context.getEmptyTupleType(), -1);
   }

   return T;
}

uint8_t ConstraintSystem::getFlags(TypeVariableType *TypeVar)
{
   auto It = TypeVariableFlagMap.find(TypeVar);
   if (It != TypeVariableFlagMap.end()) {
      return It->getSecond();
   }

   return 0;
}

void ConstraintSystem::addFlag(TypeVariableType *TypeVar, uint8_t Flag)
{
   TypeVariableFlagMap[TypeVar] |= Flag;
}

bool ConstraintSystem::representsTemplateParam(TypeVariableType *TypeVar)
{
   return (getFlags(TypeVar) & IsTemplateParameter) != 0;
}

bool ConstraintSystem::representsVariadicTemplateParam(TypeVariableType*TypeVar)
{
   return (getFlags(TypeVar) & IsVariadicTemplateParameter) != 0;
}

bool ConstraintSystem::canInferTemplateParam(TypeVariableType*TypeVar)
{
   return (getFlags(TypeVar) & CanInferTemplateParam) != 0;
}

bool ConstraintSystem::hasLiteralConstraint(TypeVariableType *TypeVar)
{
   return (getFlags(TypeVar) & HasLiteralConstraint) != 0;
}

bool ConstraintSystem::canBindToReference(TypeVariableType *TypeVar)
{
   return (getFlags(TypeVar) & CanBindToReference) != 0;
}

bool ConstraintSystem::isOverloadChoice(TypeVariableType *TypeVar)
{
   return (getFlags(TypeVar) & IsOverloadChoice) != 0;
}

bool ConstraintSystem::hasConcreteBinding(TypeVariableType *TypeVar)
{
   return (getFlags(TypeVar) & HasConcreteBinding) != 0;
}

bool ConstraintSystem::isAssigned(QualType T)
{
   return !getConcreteType(T)->containsTypeVariable();
}

namespace {

class TypeVariableSubstVisitor: public TypeBuilder<TypeVariableSubstVisitor> {
   /// Reference to the constraint system.
   ConstraintSystem &Sys;

   /// Reference to the constraint graph.
   ConstraintGraph &CG;

   void expandVariadicTypes(ArrayRef<QualType> Types,
                            SmallVectorImpl<QualType> &Result) {
      for (QualType T : Types) {
         auto *TypeVar = T->asTypeVariableType();
         if (!TypeVar || !Sys.representsVariadicTemplateParam(TypeVar)) {
            Result.push_back(visit(T));
            continue;
         }

         auto *Tup = CG.getBinding(TypeVar)->asTupleType();
         for (QualType El : Tup->getContainedTypes()) {
            if (El->isErrorType()) {
               Result.push_back(TypeVar);
            }
            else {
               Result.push_back(visit(El));
            }
         }
      }
   }

public:
   TypeVariableSubstVisitor(SemaPass &SP,
                            ConstraintSystem &Sys,
                            ConstraintGraph &CG,
                            SourceRange SR)
      : TypeBuilder(SP, SR),
        Sys(Sys), CG(CG)
   {}

   QualType visitTypeVariableType(TypeVariableType *T)
   {
      QualType Subst = CG.getBinding(T);
      if (!Subst) {
         return T;
      }

      return Subst;
   }

   QualType visitTupleType(TupleType *T)
   {
      SmallVector<QualType, 2> ElementTypes;
      expandVariadicTypes(T->getContainedTypes(), ElementTypes);

      return SP.Context.getTupleType(ElementTypes);
   }

   QualType visitFunctionType(FunctionType *T)
   {
      SmallVector<QualType, 2> ParamTypes;
      expandVariadicTypes(T->getParamTypes(), ParamTypes);

      return SP.Context.getFunctionType(visit(T->getReturnType()),
                                        ParamTypes, T->getParamInfo(),
                                        T->getRawFlags(), T->isLambdaType());
   }
};

} // anonymous namespace

QualType ConstraintSystem::getConcreteType(QualType T,
                                           TypeVariableType *ConstrainedType) {
   QualType originalType = T;
   if (T->containsTypeVariable()) {
      auto Idx = getVariadicParamIdx(ConstrainedType);
      if (Idx != -1 && T->isTypeVariableType()
      && representsVariadicTemplateParam(T->asTypeVariableType())) {
         QualType TupleTy = CG.getBinding(T->asTypeVariableType());
         auto ContainedTypes = TupleTy->asTupleType()->getContainedTypes();
         if (Idx >= ContainedTypes.size()
         || ContainedTypes[Idx]->isErrorType()) {
            return T;
         }

         T = ContainedTypes[Idx];
      }
      else {
         T = TypeVariableSubstVisitor(*QC.Sema, *this, CG,SourceRange())
            .visit(T);
      }

      if (T != originalType && T->containsTypeVariable()) {
         return getConcreteType(T, nullptr);
      }
   }

   return T;
}

void ConstraintSystem::bindTypeVariable(TypeVariableType *TypeVar,
                                        QualType ConcreteTy,
                                        unsigned OverloadChoice,
                                        bool AllowRebinding) {
   assert((AllowRebinding || !isAssigned(TypeVar))
          && "type variable already bound!");

   ++BoundTypeVariables;
   CG.bindTypeVariable(TypeVar, ConcreteTy, OverloadChoice);
}

bool ConstraintSystem::hasUnassignedTypeVariables() const
{
   return BoundTypeVariables < NextTypeVariable;
}

QualType ConstraintSystem::getConstrainedBinding(TypeVariableType *TypeVar)
{
   auto It = DirectBindingMap.find(TypeVar);
   if (It != DirectBindingMap.end()) {
      return It->getSecond();
   }

   return QualType();
}

unsigned ConstraintSystem::getVariadicParamIdx(TypeVariableType *TypeVar)
{
   auto It = VariadicParamIdx.find(TypeVar);
   if (It != VariadicParamIdx.end()) {
      return It->getSecond();
   }

   return -1;
}

void ConstraintSystem::setVariadicParamIdx(TypeVariableType *TypeVar,
                                           unsigned Idx) {
   VariadicParamIdx[TypeVar] = Idx;
}

void ConstraintSystem::updateStatus(unsigned Status)
{
   switch (static_cast<QueryResult::ResultKind>(Status)) {
   case QueryResult::Dependent:
//      TypeDependent = true;
      break;
   case QueryResult::Error:
      EncounteredError = true;
      break;
   default:
      break;
   }
}

bool ConstraintSystem::isSatisfied(Constraint *C)
{
   switch (C->getKind()) {
#  define CDOT_CONSTRAINT(NAME)                                               \
   case Constraint::NAME##ID: return isSatisfied(cast<NAME##Constraint>(C));
#  include "Constraints.def"
   }
}

bool ConstraintSystem::isSatisfied(TypeBindingConstraint *C)
{
   auto *TypeVar = C->getConstrainedType();
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType RHS = getConcreteType(C->getType(), TypeVar);

   if (LHS->containsTypeVariable() || RHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   return LHS == RHS;
}

bool ConstraintSystem::isSatisfied(TypeEqualityConstraint *C)
{
   auto *TypeVar = C->getConstrainedType();
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType RHS = getConcreteType(C->getType(), TypeVar);

   if (LHS->containsTypeVariable() || RHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   return LHS == RHS;
}

bool ConstraintSystem::isSatisfied(TypeInequalityConstraint *C)
{
   auto *TypeVar = C->getConstrainedType();
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType RHS = getConcreteType(C->getType(), TypeVar);

   if (LHS->containsTypeVariable() || RHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   return LHS != RHS;
}

bool ConstraintSystem::isSatisfied(FunctionReturnTypeConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType RHS = getConcreteType(C->getType());

   if (LHS->containsTypeVariable() || RHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   assert(RHS->isFunctionType() && "not a function type!");
   return LHS == RHS->asFunctionType()->getReturnType();
}

bool ConstraintSystem::isSatisfied(FunctionParamTypeConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType RHS = getConcreteType(C->getType());

   if (LHS->containsTypeVariable() || RHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   assert(RHS->isFunctionType() && "not a function type!");
   RHS = RHS->asFunctionType()->getParamTypes()[C->getParamIndex()];

   if (LHS == RHS) {
      return true;
   }

   auto ConvSeq = QC.Sema->getConversionSequence(LHS, RHS);
   if (!ConvSeq.isValid()) {
//      if (ConvSeq.isDependent()) {
//         TypeDependent = true;
//         return true;
//      }

      return false;
   }

   return ConvSeq.isImplicit();
}

bool ConstraintSystem::isSatisfied(CovarianceConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType RHS = getConcreteType(C->getType());

   if (LHS->containsTypeVariable() || RHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   ArrayRef<QualType> Types;
   if (C->isVariadic()) {
      Types = LHS->asTupleType()->getContainedTypes();
   }
   else {
      Types = LHS;
   }

   for (QualType Ty : Types) {
      if (Ty == RHS) {
         continue;
      }

      auto ConvSeq = QC.Sema->getConversionSequence(Ty, RHS);
      if (!ConvSeq.isValid()) {
//         if (ConvSeq.isDependent()) {
//            TypeDependent = true;
//            continue;
//         }

         return false;
      }

      if (!ConvSeq.isImplicit()) {
         return false;
      }
   }

   return true;
}

bool ConstraintSystem::isSatisfied(InferenceConstraint *C)
{
   // Only used for inference.
   return true;
}

bool ConstraintSystem::isSatisfied(DefaultableConstraint *C)
{
   // Not an actual constraint, but rather a hint to the solver.
   return true;
}

bool ConstraintSystem::isSatisfied(ConformanceConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   if (LHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   return QC.Sema->ConformsTo(LHS, C->getProtoDecl());
}

bool ConstraintSystem::isSatisfied(ClassConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   if (LHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   return LHS->isClass();
}

bool ConstraintSystem::isSatisfied(HasDefaultValueConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   if (LHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   return QC.Sema->hasDefaultValue(LHS);
}

bool ConstraintSystem::isSatisfied(ImplicitConversionConstraint *C)
{
   auto *TypeVar = C->getConstrainedType();
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType RHS = getConcreteType(C->getType(), TypeVar);

   if (LHS->containsTypeVariable() || RHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   if (LHS == RHS) {
      return true;
   }

   auto ConvSeq = QC.Sema->getConversionSequence(LHS, RHS);
   if (!ConvSeq.isValid()) {
//      if (ConvSeq.isDependent()) {
//         TypeDependent = true;
//         return true;
//      }

      return false;
   }

   return ConvSeq.isImplicit();
}

static bool isExpressibleBy(SemaPass &Sema,
                            CanType Ty,
                            SemaPass::InitializableByKind K,
                            bool &Dependent,
                            bool &EncounteredError) {
//   if (Ty->isDependentType()) {
//      Dependent = true;
//      return true;
//   }

   CanType Desugared = Ty->getDesugaredType();

   auto *InitializableByDecl = Sema.getInitializableByDecl(K);
   if (!InitializableByDecl) {
      return false;
   }

   RecordDecl *ConformingRec = nullptr;
   if (auto *RT = Desugared->asRecordType()) {
      bool ConformsTo = Sema.ConformsTo(Desugared, InitializableByDecl);
      if (ConformsTo) {
         ConformingRec = RT->getRecord();
      }
   }
   else if (auto *Ext = Desugared->asExistentialType()) {
      for (auto ET : Ext->getExistentials()) {
         bool ConformsTo = Sema.ConformsTo(ET, InitializableByDecl);
         if (ConformsTo) {
            ConformingRec = ET->getRecord();
            break;
         }
      }
   }
   else {
      return false;
   }

   return ConformingRec != nullptr;
}

bool ConstraintSystem::isSatisfied(LiteralConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   if (LHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   switch (C->getLiteralKind()) {
   case LiteralConstraint::IntegerLiteral: {
      if (LHS->isIntegerType()) {
         return true;
      }

      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::Integer,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::FPLiteral: {
      if (LHS->isFPType()) {
         return true;
      }

      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::Float,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::BoolLiteral: {
      if (LHS->isInt1Ty()) {
         return true;
      }

      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::Bool,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::GraphemeClasterLiteral: {
      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::GraphemeCluster,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::CodePointLiteral: {
      if (LHS->isIntNTy(32, true)) {
         return true;
      }

      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::CodePoint,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::ASCIILiteral: {
      if (LHS->isInt8Ty()) {
         return true;
      }

      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::ASCII,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::StringLiteral: {
      if (LHS->isPointerType() && LHS->getPointeeType()->isInt8Ty()) {
         return true;
      }

      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::String,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::ArrayLiteral: {
      // The element type is handled via additional constraints.
      if (LHS->isArrayType()) {
         return true;
      }

      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::Array,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::DictionaryLiteral: {
      // The key and value types are handled via additional constraints.
      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::Dictionary,
                             TypeDependent, EncounteredError);
   }
   case LiteralConstraint::NoneLiteral: {
      return isExpressibleBy(*QC.Sema, LHS,
                             SemaPass::InitializableByKind::None,
                             TypeDependent, EncounteredError);
   }
   }
}

bool ConstraintSystem::isSatisfied(MemberConstraint *C)
{
   CanType LHS = getConcreteType(C->getConstrainedType());
   CanType MemberType = getConcreteType(C->getMemberType());

   if (LHS->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   LookupOpts Opts = DefaultLookupOpts;
   if (MemberType->isMetaType()) {
      Opts |= LookupOpts::TypeLookup;
   }

   const MultiLevelLookupResult *LookupRes;
   if (auto Err = QC.MultiLevelTypeLookup(LookupRes, LHS, C->getMemberName(),
                                          Opts)) {
      updateStatus(Err.K);
      return true;
   }

   if (LookupRes->empty()) {
      return false;
   }

   if (MemberType->containsTypeVariable()) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   for (auto *ND : LookupRes->allDecls()) {
      QualType NonCanon = QC.Sema->getTypeForDecl(ND);
      if (!NonCanon) {
         continue;
      }

      CanType DeclTy = NonCanon;
//      if (DeclTy->isUnknownAnyType()) {
//         TypeDependent = true;
//         return true;
//      }
      if (DeclTy->isErrorType()) {
         EncounteredError = true;
         return true;
      }

      if (!MemberType->isReferenceType()) {
         DeclTy = DeclTy->removeReference();
      }

      if (DeclTy == MemberType) {
         return true;
      }
   }

   return false;
}

bool ConstraintSystem::isSatisfied(DisjunctionConstraint *C)
{
   for (auto *IC : C->getConstraints()) {
      if (isSatisfied(IC)) {
         return true;
      }
   }

   return false;
}

bool ConstraintSystem::isSatisfied(ConditionalConstraint *C)
{
   auto *DJ = C->getDisjunctionVar();
   if (!isAssigned(DJ)) {
      // Can't tell right now, assume it's satisfied.
      return true;
   }

   auto Choice = CG.getOverloadChoice(DJ);
   assert(Choice != -1 && "no overload selected!");

   return isSatisfied(C->getConstraintAt(Choice));
}

namespace {

class TypeEquivalenceBuilder: public TypeComparer<TypeEquivalenceBuilder> {
   /// Reference to the constraint system.
   ConstraintSystem &Sys;

   /// The original constraint.
   Constraint *C;

   /// Worklist of constraints to simplify.
   SmallVectorImpl<Constraint*> &Worklist;

   /// Whether or not to compare for a strict match of the mutability of pointer /
   /// reference types.
   bool strict;

public:
   TypeEquivalenceBuilder(ConstraintSystem &Sys,
                          Constraint *C,
                          SmallVectorImpl<Constraint*> &Worklist,
                          bool strict)
      : Sys(Sys), C(C), Worklist(Worklist), strict(strict)
   { }

   bool visitTypeVariableType(TypeVariableType *LHS, QualType RHS)
   {
      if (isa<ImplicitConversionConstraint>(C)) {
         Sys.newConstraint<ImplicitConversionConstraint>(LHS, RHS,
                                                         C->getLocator());
      }
      else if (!Sys.isOverloadChoice(LHS)) {
         auto &CG = Sys.getConstraintGraph();
         bool madeChanges = false;

         if (QualType binding = Sys.getConstraintGraph().getBinding(LHS)) {
           if (binding != RHS) {
              return false;
           }
         }
         else {
            Sys.bindTypeVariable(LHS, RHS);
            madeChanges = true;
         }

         if (Sys.hasConcreteBinding(LHS)) {
            auto binding = Sys.getConstrainedBinding(LHS);
            if (binding != RHS) {
               return false;
            }
         }
         else {
            Sys.newConstraint<TypeBindingConstraint>(LHS, RHS, C->getLocator());
            madeChanges = true;
         }

         if (!madeChanges) {
            return true;
         }

         // Revisit the constraints that mention this type variable.
         auto Constraints = CG.getOrAddNode(LHS)->getConstraints();
         for (auto *NextCons : Constraints) {
            if (NextCons != C) {
               Worklist.push_back(NextCons);
            }
         }
      }

      return true;
   }

   bool visitPointerType(PointerType *LHS, QualType RHS)
   {
      if (strict) {
         if (RHS->getTypeID() == Type::PointerTypeID) {
            return visit(LHS->getPointeeType(), RHS->getPointeeType());
         }
      }
      else if (auto *Ptr = RHS->asPointerType()) {
         return visit(LHS->getPointeeType(), Ptr->getPointeeType());
      }

      return false;
   }

   bool visitReferenceType(ReferenceType *LHS, QualType RHS)
   {
      if (strict) {
         if (RHS->getTypeID() == Type::ReferenceTypeID) {
            return visit(LHS->getReferencedType(), RHS->getReferencedType());
         }
      }
      else if (auto *Ptr = RHS->asReferenceType()) {
         return visit(LHS->getReferencedType(), Ptr->getReferencedType());
      }

      return false;
   }

   bool visitMutableReferenceType(MutableReferenceType *LHS, QualType RHS)
   {
      if (strict) {
         if (RHS->getTypeID() == Type::MutableReferenceTypeID) {
            return visit(LHS->getReferencedType(), RHS->getReferencedType());
         }
      }
      else if (auto *Ref = RHS->asMutableReferenceType()) {
         return visit(LHS->getReferencedType(), Ref->getReferencedType());
      }

      return false;
   }
};

} // anonymous namespace

bool ConstraintSystem::simplify(Constraint *C,
                                SmallVectorImpl<Constraint*> &Worklist,
                                SmallPtrSetImpl<Constraint*> &removedConstraints) {
   if (removedConstraints.count(C) != 0) {
      return false;
   }

   switch (C->getKind()) {
   case Constraint::TypeBindingID:
   case Constraint::TypeEqualityID:
   case Constraint::ImplicitConversionID: {
      auto *TypeVar = C->getConstrainedType();
      QualType LHS = getConcreteType(C->getConstrainedType());
      QualType RHS = getConcreteType(C->getRHSType(), TypeVar);

      // Trivial case: both are already assigned, just check if the
      // constraint is satisfied.
      if (!LHS->containsTypeVariable() && !RHS->containsTypeVariable()) {
         return !isSatisfied(C);
      }

      QualType Concrete;
      QualType Inconcrete;

      if (!LHS->containsTypeVariable()) {
         Concrete = LHS;
         Inconcrete = RHS;
      }
      else if (!RHS->containsTypeVariable()) {
         Concrete = RHS;
         Inconcrete = LHS;
      }

      // Can't simplify if both sides contain type variables.
      if (!Concrete) {
         return false;
      }

      // Implicit conversion constraints are only transitive for tuple types.
      bool strict = !isa<ImplicitConversionConstraint>(C);
      if ((isa<ImplicitConversionConstraint>(C)
      && !Concrete->isTupleType()
      && !Concrete->isFunctionType())) {
         return false;
      }

      // Otherwise, we can make simplify based on the structure of the type,
      // e.g. for the constraint
      //    `T0 == (T1, T2)`
      // we can form new equality constraints for T1 and T2 if T0 is known
      // and of tuple type. If the structure doesn't match, we know that the
      // solution is invalid.
      return !TypeEquivalenceBuilder(*this, C, Worklist, strict)
         .visit(Inconcrete, Concrete);
   }
   case Constraint::FunctionReturnTypeID: {
      QualType LHS = getConcreteType(C->getConstrainedType());
      QualType RHS = getConcreteType(C->getRHSType());

      // Trivial case: both are already assigned, just check if the
      // constraint is satisfied.
      if (!LHS->containsTypeVariable() && !RHS->containsTypeVariable()) {
         return !isSatisfied(C);
      }

      // RHS is not resolved yet.
      if (RHS->containsTypeVariable()) {
         return false;
      }

      // Assign the return type of the left hand side to the right hand side
      // type variable.
      bindTypeVariable(C->getConstrainedType(),
                       RHS->asFunctionType()->getReturnType());

      // Revisit the constraints that mention this type variable.
      auto Constraints = CG.getOrAddNode(C->getConstrainedType())
         ->getConstraints();

      Worklist.append(Constraints.begin(), Constraints.end());
      return false;
   }
   case Constraint::FunctionParamTypeID: {
      QualType LHS = getConcreteType(C->getConstrainedType());
      QualType RHS = getConcreteType(C->getRHSType());

      // Trivial case: both are already assigned, just check if the
      // constraint is satisfied.
      if (!LHS->containsTypeVariable() && !RHS->containsTypeVariable()) {
         return !isSatisfied(C);
      }

      // RHS is not resolved yet.
      if (RHS->containsTypeVariable()) {
         return false;
      }

      unsigned Idx = cast<FunctionParamTypeConstraint>(C)->getParamIndex();

      // Assign the return type of the left hand side to the right hand side
      // type variable.
      bindTypeVariable(C->getConstrainedType(),
                       RHS->asFunctionType()->getParamTypes()[Idx]);

      // Revisit the constraints that mention this type variable.
      auto Constraints = CG.getOrAddNode(C->getConstrainedType())
                           ->getConstraints();

      Worklist.append(Constraints.begin(), Constraints.end());
      return false;
   }
   case Constraint::InferenceID: {
      auto *IC = cast<InferenceConstraint>(C);
      auto *TypeVar = IC->getConstrainedType();

      QualType RHS = getConcreteType(IC->getRHSType());
      if (RHS->containsTypeVariable()) {
         return false;
      }

      if (representsVariadicTemplateParam(TypeVar)) {
         auto Idx = getVariadicParamIdx(IC->getRHSType()->asTypeVariableType());
         CG.bindVariadicTypeVariable(TypeVar, RHS, Idx);
      }
      else {
         bindTypeVariable(TypeVar, RHS);
      }

      // Revisit the constraints that mention this type variable.
      auto Constraints = CG.getOrAddNode(TypeVar)->getConstraints();
      Worklist.append(Constraints.begin(), Constraints.end());

      CG.removeConstraint(C);
      removedConstraints.insert(C);

      return false;
   }
   default:
      // Can't simplify these constraints.
      return !isSatisfied(C);
   }
}

bool ConstraintSystem::applyConcreteBindings()
{
   for (auto &binding : DirectBindingMap) {
//      if (QualType concreteType = CG.getBinding(binding.getFirst())) {
//         if (binding.getSecond() != concreteType) {
//            return true;
//         }
//      }
//      else {
//         bindTypeVariable(binding.getFirst(), binding.getSecond());
//      }
      bindTypeVariable(binding.getFirst(), binding.getSecond());
   }

   if (auto *failed = simplifyConstraints()) {
      return true;
   }

   return false;
}

ConstraintSystem::ResultKind
ConstraintSystem::solve(SmallVectorImpl<Solution> &Solutions,
                        bool StopAfterFirstFailure) {
   // Enter a first solver scope.
   auto *OuterScope = new(*this) SolverScope(*this);
   this->StopAfterFirstFailure = StopAfterFirstFailure;

   if (NextTypeVariable == 0) {
      Solutions.emplace_back(AssignmentMapType(), 0);
      return Success;
   }

   if (applyConcreteBindings()) {
      if (!StopAfterFirstFailure) {
         OuterScope->~SolverScope();
      }

      return Failure;
   }

   SmallVector<SolverStep*, 4> Worklist;
   Worklist.push_back(new(*this) SplitterStep(*this, Solutions, Worklist));

   bool PrevFailed = false;
   while (!Worklist.empty()) {
      SolverStep *Next = Worklist.back();
      Worklist.pop_back();

      if (Next->state() == SolverStep::Error) {
         continue;
      }
      else if (Next->state() == SolverStep::Idle) {
         Next->transition(SolverStep::Active);
         Next->execute(PrevFailed);
      }
      else {
         Next->transition(SolverStep::Active);
         Next->resume(PrevFailed);
      }

      if (Next->state() == SolverStep::Error) {
         if (StopAfterFirstFailure) {
            // Don't exit the solver scope here so we can use it for
            // diagnostics.
            return Failure;
         }

         PrevFailed = true;
      }
      else {
         PrevFailed = false;
      }
   }

   // Exit the solver scope.
   OuterScope->~SolverScope();

   if (EncounteredError) {
      return Error;
   }

   if (TypeDependent) {
      return Dependent;
   }

   return Solutions.empty() ? Failure : Success;
}

Constraint *ConstraintSystem::simplifyConstraints(TypeVariableType *Modified)
{
   // We need to use an index here because simplification might modify the
   // vector of active constraints.
   SmallVector<Constraint*, 4> Worklist;
   if (Modified) {
      auto Constraints = CG.getOrAddNode(Modified)->getConstraints();
      Worklist.append(Constraints.begin(), Constraints.end());
   }
   else {
      auto Constraints = CG.getActiveConstraints();
      Worklist.append(Constraints.begin(), Constraints.end());
   }

   unsigned i = 0;
   SmallPtrSet<Constraint*, 2> removedConstraints;

   while (i < Worklist.size()) {
      auto *C = Worklist[i++];
      if (simplify(C, Worklist, removedConstraints)) {
         FailedConstraint = C;
         return C;
      }
   }

   return nullptr;
}

bool
ConstraintSystem::appendCurrentSolution(SmallVectorImpl<Solution> &Solutions,
                                        ArrayRef<TypeVariableType*> TypeVars) {
   Solutions.emplace_back();

   Solution &S = Solutions.back();
   S.Score = CurrentScore;

   for (auto *TypeVar : TypeVars) {
      QualType T = CG.getBinding(TypeVar);
      if (!T || T->containsTypeVariable()) {
         // Not a viable solution.
         Solutions.pop_back();
         return false;
      }

      assert(!T->containsTypeVariable() && "unassigned type variable!");
      S.AssignmentMap.try_emplace(TypeVar, T);

      if (isOverloadChoice(TypeVar)) {
         S.OverloadChoices.try_emplace(TypeVar, CG.getOverloadChoice(TypeVar));
      }
   }

   if (CurrentScore < BestScore) {
      BestScore = CurrentScore;
   }

   return true;
}

template<class T>
T *getFirstConstraint(ConstraintSystem &Sys, TypeVariableType *TV)
{
   for (auto *C : Sys.getConstraintGraph().getOrAddNode(TV)->getConstraints()) {
      if (auto *TC = dyn_cast<T>(C)) {
         return TC;
      }
   }

   return nullptr;
}

static int isBetterBinding(ConstraintSystem &Sys,
                           TypeVariableType *TV,
                           CanType T1, CanType T2) {
   if (Sys.representsTemplateParam(TV)) {
      bool T1IsProto = T1->isProtocol();
      bool T2IsProto = T2->isProtocol();

      if (T1IsProto && !T2IsProto) {
         return -1;
      }
      if (!T1IsProto && T2IsProto) {
         return 1;
      }

      auto *Conv = getFirstConstraint<ImplicitConversionConstraint>(Sys, TV);
      if (!Conv) {
         return 0;
      }

      QualType Covariance = Conv->getType();
      if (T1 == Covariance) {
         return -1;
      }
      if (T2 == Covariance) {
         return 1;
      }

      return 0;
   }

   if (Sys.hasLiteralConstraint(TV)) {
      auto *Lit = getFirstConstraint<LiteralConstraint>(Sys, TV);
      assert(Lit && "does not have a literal constraint!");

      QualType DefaultTy = Lit->getDefaultLiteralType(Sys.QC);
      if (T1 != DefaultTy) {
         return T2 != DefaultTy ? 0 : 1;
      }
      if (T2 != DefaultTy) {
         return -1;
      }

      return 0;
   }

   if (T1->isProtocol()) {
      return T2->isProtocol() ? 0 : 1;
   }

   if (T2->isProtocol()) {
      return T1->isProtocol() ? 0 : -1;
   }

   return 0;
}

ConstraintSystem::SolutionComparisonResult
ConstraintSystem::compareSolutions(const Solution &S1, const Solution &S2)
{
   if (S1.Score != S2.Score) {
      return S2.Score < S1.Score ? BetterSolution : WorseSolution;
   }

   bool AllEqual = true;
   int RelativeScore = 0;

   for (auto *TypeVar : TypeVariables) {
      auto It1 = S1.AssignmentMap.find(TypeVar);
      if (It1 == S1.AssignmentMap.end()) {
         continue;
      }

      CanType T1 = It1->getSecond();
      CanType T2 = S2.AssignmentMap.find(TypeVar)->getSecond();

      if (T1 != T2) {
         AllEqual = false;
         RelativeScore += isBetterBinding(*this, TypeVar, T1, T2);
      }
   }

   if (AllEqual) {
      return EquivalentSolution;
   }

   return RelativeScore == 0 ? EqualSolution
                             : (RelativeScore < 0 ? BetterSolution
                                                  : WorseSolution);
}

void ConstraintSystem::dumpSolution(const Solution &S)
{
   printSolution(S, llvm::errs());
}

void ConstraintSystem::printSolution(const Solution &S, llvm::raw_ostream &OS)
{
   OS << "viable solution (score " << S.Score << "):\n";

   for (auto &AS : S.AssignmentMap) {
      OS << *AS.getFirst() << " = " << AS.getSecond().toDiagString() << "\n";
   }
}

void ConstraintSystem::dumpConstraints()
{
   printConstraints(llvm::errs());
}

void ConstraintSystem::printConstraints(llvm::raw_ostream &OS)
{
   for (auto *C : CG.getActiveConstraints()) {
      C->print(OS);
      OS << "\n";
   }
}