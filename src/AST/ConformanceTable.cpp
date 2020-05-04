#include "cdotc/AST/ConformanceTable.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/Support/Log.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot::ast;

namespace cdot {

ConformanceTable::~ConformanceTable()
{
   for (auto& Entry : TestMap) {
      if (Entry.getSecond().is<ConformanceSet*>()) {
         Entry.getSecond().get<ConformanceSet*>()->~DenseSet();
      }
   }
}

bool ConformanceTable::registerConformance(ast::ASTContext& C, RecordDecl* Rec,
                                           ProtocolDecl* P)
{
   auto It = TestMap.find(Rec);

   // if there are no conformances (other than any) for this record, add a
   // new 'Single' entry
   if (It == TestMap.end()) {
      TestMap.try_emplace(Rec, P);
      return true;
   }

   auto& SingleOrMultiple = It->getSecond();

   // there is already a 'Single' entry, check if it refers to the same protocol
   if (SingleOrMultiple.is<ProtocolDecl*>()) {
      if (P == SingleOrMultiple.get<ProtocolDecl*>()) {
         // conformance already declared
         return false;
      }

      // turn the 'Single' entry into a multiple
      auto* Set = new (C) ConformanceSet;
      Set->insert(P);
      Set->insert(SingleOrMultiple.get<ProtocolDecl*>());

      TestMap[Rec] = Set;
      return true;
   }

   // a multiple entry exists already. Do an insert to check if the
   // conformance was already declared
   return SingleOrMultiple.get<ConformanceSet*>()->insert(P).second;
}

#ifndef NDEBUG
static StringRef conformanceKindToString(ConformanceKind K)
{
   switch (K) {
   case ConformanceKind::None:
      return "non-existant";
   case ConformanceKind::Implicit:
      return "implicit";
   case ConformanceKind::Explicit:
      return "explicit";
   case ConformanceKind::Inherited:
      return "inherited";
   case ConformanceKind::Conditional:
      return "conditional";
   }
}
#endif

void ConformanceTable::insertConformance(RecordDecl* Rec, Conformance* Conf)
{
   Conformances[Rec].push_back(Conf);

   LOG(ProtocolConformances, "registered ",
       conformanceKindToString(Conf->getKind()), " conformance of ",
       Rec->getDeclName(), " to protocol ", Conf->getProto(), " ", Conf->getProto()->getFullName(),
       Conf->isConditional() ? " (where " : "",
       Conf->isConditional() ? *Conf->getConstraints()
                             : *Rec->getASTCtx().EmptyConstraintSet,
       Conf->isConditional() ? ")" : "");
}

bool ConformanceTable::addConformance(ASTContext& C, ConformanceKind Kind,
                                      RecordDecl* Decl, ProtocolDecl* P,
                                      ast::DeclContext* introducedIn,
                                      ast::ConstraintSet* constraints,
                                      int depth,
                                      Conformance** NewConf)
{
   assert(Kind != ConformanceKind::None && "invalid conformance kind!");
   if (Kind != ConformanceKind::Conditional) {
      if (!registerConformance(C, Decl, P)) {
         return false;
      }
   }

   assert((!constraints || introducedIn)
      && "constrained conformance should have a DeclContext!");

   Conformance* Conf = new (C) Conformance(Kind, P, constraints, introducedIn, depth);
   insertConformance(Decl, Conf);

   if (NewConf) {
      *NewConf = Conf;
   }

   return true;
}

bool ConformanceTable::addExplicitConformance(ASTContext& C, RecordDecl* Decl,
                                              ProtocolDecl* P)
{
   if (!registerConformance(C, Decl, P))
      return false;

   Conformance* Conf = new (C) Conformance(ConformanceKind::Explicit, P);
   insertConformance(Decl, Conf);

   return true;
}

bool ConformanceTable::addImplicitConformance(ASTContext& C, RecordDecl* Decl,
                                              ProtocolDecl* P)
{
   if (!registerConformance(C, Decl, P))
      return false;

   Conformance* Conf = new (C) Conformance(ConformanceKind::Implicit, P);
   insertConformance(Decl, Conf);

   return true;
}

bool ConformanceTable::addInheritedConformance(ASTContext& C, RecordDecl* Decl,
                                               ProtocolDecl* P)
{
   if (!registerConformance(C, Decl, P))
      return false;

   Conformance* Conf = new (C) Conformance(ConformanceKind::Inherited, P);
   insertConformance(Decl, Conf);

   return true;
}

ConformanceKind ConformanceTable::lookupConformance(RecordDecl* Decl,
                                                    ProtocolDecl* P) const
{
   auto It = Conformances.find(Decl);
   if (It == Conformances.end())
      return ConformanceKind::None;

   for (auto& Conf : It->getSecond()) {
      if (Conf->getProto() == P)
         return Conf->getKind();
   }

   return ConformanceKind::None;
}

Conformance *ConformanceTable::getConformance(RecordDecl *Decl, ProtocolDecl *P) const
{
   auto It = Conformances.find(Decl);
   if (It == Conformances.end())
      return nullptr;

   for (auto& Conf : It->getSecond()) {
      if (Conf->getProto() == P)
         return Conf;
   }

   return nullptr;
}

bool ConformanceTable::conformsTo(RecordDecl* Decl, ProtocolDecl* P) const
{
   auto It = TestMap.find(Decl);
   if (It == TestMap.end()) {
      return false;
   }

   auto& SingleOrMultiple = It->getSecond();

   // there is already a 'Single' entry, check if it refers to the same protocol
   if (SingleOrMultiple.is<ProtocolDecl*>()) {
      return P == SingleOrMultiple.get<ProtocolDecl*>();
   }

   auto& Set = *SingleOrMultiple.get<ConformanceSet*>();
   return Set.find(P) != Set.end();
}

llvm::ArrayRef<Conformance*>
ConformanceTable::getAllConformances(RecordDecl* Decl) const
{
   auto It = Conformances.find(Decl);
   if (It == Conformances.end())
      return {};

   return It->getSecond();
}

std::pair<Conformance* const*, Conformance* const*>
ConformanceTable::getFirstOfKind(RecordDecl* R, ConformanceKind K) const
{
   auto It = Conformances.find(R);
   if (It == Conformances.end())
      return {nullptr, nullptr};

   for (auto& Conf : It->getSecond()) {
      if (Conf->getKind() == K) {
         return {&Conf, It->getSecond().end()};
      }
   }

   return {nullptr, nullptr};
}

ConformanceTable::explicit_iterator
ConformanceTable::explicit_begin(RecordDecl* Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Explicit);
   return explicit_iterator(Pair.first, Pair.second);
}

ConformanceTable::explicit_iterator
ConformanceTable::explicit_end(RecordDecl* Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Explicit);
   return explicit_iterator(Pair.second, Pair.second);
}

ConformanceTable::implicit_iterator
ConformanceTable::implicit_begin(RecordDecl* Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Implicit);
   return implicit_iterator(Pair.first, Pair.second);
}

ConformanceTable::implicit_iterator
ConformanceTable::implicit_end(RecordDecl* Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Implicit);
   return implicit_iterator(Pair.second, Pair.second);
}

ConformanceTable::inherited_iterator
ConformanceTable::inherited_begin(RecordDecl* Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Inherited);
   return inherited_iterator(Pair.first, Pair.second);
}

ConformanceTable::inherited_iterator
ConformanceTable::inherited_end(RecordDecl* Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Inherited);
   return inherited_iterator(Pair.second, Pair.second);
}

} // namespace cdot