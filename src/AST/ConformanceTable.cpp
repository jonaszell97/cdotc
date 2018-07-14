//
// Created by Jonas Zell on 16.04.18.
//

#include "ConformanceTable.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"

using namespace cdot::ast;

namespace cdot {

ConformanceTable::~ConformanceTable()
{
   for (auto &Entry : TestMap) {
      if (Entry.getSecond().is<ConformanceSet*>()) {
         Entry.getSecond().get<ConformanceSet*>()->~DenseSet();
      }
   }
}

bool ConformanceTable::registerConformance(ast::ASTContext &C,
                                           RecordDecl *Rec,
                                           ProtocolDecl *P) {
   if (P->isAny())
      return false;

   auto It = TestMap.find(Rec);

   // if there are no conformances (other than any) for this record, add a
   // new 'Single' entry
   if (It == TestMap.end()) {
      TestMap.try_emplace(Rec, P);
      return true;
   }

   auto &SingleOrMultiple = It->getSecond();

   // there is already a 'Single' entry, check if it refers to the same protocol
   if (SingleOrMultiple.is<ProtocolDecl*>()) {
      if (P == SingleOrMultiple.get<ProtocolDecl*>()) {
         // conformance already declared
         return false;
      }

      // turn the 'Single' entry into a multiple
      auto *Set = new(C) ConformanceSet;
      Set->insert(P);
      Set->insert(SingleOrMultiple.get<ProtocolDecl*>());

      TestMap[Rec] = Set;
      return true;
   }

   // a multiple entry exists already. Do an insert to check if the
   // conformance was already declared
   return SingleOrMultiple.get<ConformanceSet*>()->insert(P).second;
}

void ConformanceTable::insertConformance(RecordDecl *Rec, Conformance *Conf)
{
   Conformances[Rec].push_back(Conf);
}

bool ConformanceTable::addExplicitConformance(ASTContext &C,
                                              RecordDecl *Decl,
                                              ProtocolDecl *P) {
   if (!registerConformance(C, Decl, P))
      return false;

   Conformance *Conf = new (C) Conformance(ConformanceKind::Explicit, P);
   insertConformance(Decl, Conf);

   return true;
}

bool ConformanceTable::addImplicitConformance(ASTContext &C,
                                              RecordDecl *Decl,
                                              ProtocolDecl *P) {
   if (!registerConformance(C, Decl, P))
      return false;

   Conformance *Conf = new (C) Conformance(ConformanceKind::Implicit, P);
   insertConformance(Decl, Conf);

   return true;
}

bool ConformanceTable::addInheritedConformance(ASTContext &C,
                                               RecordDecl *Decl,
                                               ProtocolDecl *P) {
   if (!registerConformance(C, Decl, P))
      return false;

   Conformance *Conf = new (C) Conformance(ConformanceKind::Inherited, P);
   insertConformance(Decl, Conf);

   return true;
}

ConformanceKind ConformanceTable::lookupConformance(RecordDecl *Decl,
                                                    ProtocolDecl *P) const {
   auto It = Conformances.find(Decl);
   if (It == Conformances.end())
      return ConformanceKind::None;

   for (auto &Conf : It->getSecond()) {
      if (Conf->getProto() == P)
         return Conf->getKind();
   }

   return ConformanceKind::None;
}

bool ConformanceTable::conformsTo(RecordDecl *Decl, ProtocolDecl *P) const
{
   if (P->isAny())
      return true;

   auto It = TestMap.find(Decl);
   if (It == TestMap.end()) {
      return false;
   }

   auto &SingleOrMultiple = It->getSecond();

   // there is already a 'Single' entry, check if it refers to the same protocol
   if (SingleOrMultiple.is<ProtocolDecl*>()) {
      return P == SingleOrMultiple.get<ProtocolDecl*>();
   }

   auto &Set = *SingleOrMultiple.get<ConformanceSet*>();
   return Set.find(P) != Set.end();
}

llvm::ArrayRef<Conformance*>
ConformanceTable::getAllConformances(RecordDecl *Decl) const
{
   auto It = Conformances.find(Decl);
   if (It == Conformances.end())
      return {};

   return It->getSecond();
}

std::pair<Conformance *const*, Conformance *const*>
ConformanceTable::getFirstOfKind(RecordDecl *R, ConformanceKind K) const
{
   auto It = Conformances.find(R);
   if (It == Conformances.end())
      return { nullptr, nullptr };
   
   for (auto &Conf : It->getSecond()) {
      if (Conf->getKind() == K) {
         return { &Conf, It->getSecond().end() };
      }
   }
   
   return { nullptr, nullptr };
}

ConformanceTable::explicit_iterator
ConformanceTable::explicit_begin(RecordDecl *Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Explicit);
   return explicit_iterator(Pair.first, Pair.second);
}

ConformanceTable::explicit_iterator
ConformanceTable::explicit_end(RecordDecl *Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Explicit);
   return explicit_iterator(Pair.second, Pair.second);
}

ConformanceTable::implicit_iterator
ConformanceTable::implicit_begin(RecordDecl *Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Implicit);
   return implicit_iterator(Pair.first, Pair.second);
}

ConformanceTable::implicit_iterator
ConformanceTable::implicit_end(RecordDecl *Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Implicit);
   return implicit_iterator(Pair.second, Pair.second);
}

ConformanceTable::inherited_iterator
ConformanceTable::inherited_begin(RecordDecl *Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Inherited);
   return inherited_iterator(Pair.first, Pair.second);
}

ConformanceTable::inherited_iterator
ConformanceTable::inherited_end(RecordDecl *Rec) const
{
   auto Pair = getFirstOfKind(Rec, ConformanceKind::Inherited);
   return inherited_iterator(Pair.second, Pair.second);
}

} // namespace cdot