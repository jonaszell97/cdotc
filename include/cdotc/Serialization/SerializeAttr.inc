#ifdef CDOT_ATTR_SERIALIZE
void ASTAttrWriter::visitExternAttr(ExternAttr *A) {
   Record.push_back((uint64_t)A->getLang());
}
void ASTAttrWriter::visitInlineAttr(InlineAttr *A) {
   Record.push_back((uint64_t)A->getLevel());
}
void ASTAttrWriter::visitImplicitAttr(ImplicitAttr *A) {
}
void ASTAttrWriter::visitThinAttr(ThinAttr *A) {
}
void ASTAttrWriter::visitOpaqueAttr(OpaqueAttr *A) {
}
void ASTAttrWriter::visitAlignAttr(AlignAttr *A) {
   Record.AddStmt(A->getAlignment());
}
void ASTAttrWriter::visitDiscardableResultAttr(DiscardableResultAttr *A) {
}
void ASTAttrWriter::visitAutoClosureAttr(AutoClosureAttr *A) {
}
void ASTAttrWriter::visitEscapingAttr(EscapingAttr *A) {
}
void ASTAttrWriter::visitTestableAttr(TestableAttr *A) {
}
void ASTAttrWriter::visitCompileTimeAttr(CompileTimeAttr *A) {
}
void ASTAttrWriter::visitNoDeriveAttr(NoDeriveAttr *A) {
   Record.push_back((uint64_t)A->getKind());
}
void ASTAttrWriter::visitStrongAttr(StrongAttr *A) {
}
void ASTAttrWriter::visitVersionDeclAttr(VersionDeclAttr *A) {
   Record.push_back((uint64_t)A->getVersion());
}
void ASTAttrWriter::visitVersionStmtAttr(VersionStmtAttr *A) {
   Record.push_back((uint64_t)A->getVersion());
}
void ASTAttrWriter::visitExternallyInitializedAttr(ExternallyInitializedAttr *A) {
}
void ASTAttrWriter::visitInstantiateAttr(InstantiateAttr *A) {
}
void ASTAttrWriter::visitTransparentAttr(TransparentAttr *A) {
}
void ASTAttrWriter::visit_BuiltinAttr(_BuiltinAttr *A) {
   Record.AddIdentifierRef(&Idents.get(A->getBuiltinName()));
}
void ASTAttrWriter::visit_SemanticsAttr(_SemanticsAttr *A) {
   Record.AddIdentifierRef(&Idents.get(A->getSemanticsKind()));
}
void ASTAttrWriter::visit_DebugAttr(_DebugAttr *A) {
}
#endif
#ifdef CDOT_ATTR_DESERIALIZE
ExternAttr *ASTAttrReader::readExternAttr(SourceRange SR) {
   auto lang = Record.readEnum<ExternAttr::LangKind>();
   return new(C) ExternAttr(std::move(lang), SR);
}
InlineAttr *ASTAttrReader::readInlineAttr(SourceRange SR) {
   auto level = Record.readEnum<InlineAttr::LevelKind>();
   return new(C) InlineAttr(std::move(level), SR);
}
ImplicitAttr *ASTAttrReader::readImplicitAttr(SourceRange SR) {
   return new(C) ImplicitAttr(SR);
}
ThinAttr *ASTAttrReader::readThinAttr(SourceRange SR) {
   return new(C) ThinAttr(SR);
}
OpaqueAttr *ASTAttrReader::readOpaqueAttr(SourceRange SR) {
   return new(C) OpaqueAttr(SR);
}
AlignAttr *ASTAttrReader::readAlignAttr(SourceRange SR) {
   auto alignment = cast<StaticExpr>(Record.readExpr());
   return new(C) AlignAttr(std::move(alignment), SR);
}
DiscardableResultAttr *ASTAttrReader::readDiscardableResultAttr(SourceRange SR) {
   return new(C) DiscardableResultAttr(SR);
}
AutoClosureAttr *ASTAttrReader::readAutoClosureAttr(SourceRange SR) {
   return new(C) AutoClosureAttr(SR);
}
EscapingAttr *ASTAttrReader::readEscapingAttr(SourceRange SR) {
   return new(C) EscapingAttr(SR);
}
TestableAttr *ASTAttrReader::readTestableAttr(SourceRange SR) {
   return new(C) TestableAttr(SR);
}
CompileTimeAttr *ASTAttrReader::readCompileTimeAttr(SourceRange SR) {
   return new(C) CompileTimeAttr(SR);
}
NoDeriveAttr *ASTAttrReader::readNoDeriveAttr(SourceRange SR) {
   auto kind = Record.readEnum<NoDeriveAttr::KindKind>();
   return new(C) NoDeriveAttr(std::move(kind), SR);
}
StrongAttr *ASTAttrReader::readStrongAttr(SourceRange SR) {
   return new(C) StrongAttr(SR);
}
VersionDeclAttr *ASTAttrReader::readVersionDeclAttr(SourceRange SR) {
   auto version = Record.readEnum<VersionDeclAttr::VersionKind>();
   return new(C) VersionDeclAttr(std::move(version), SR);
}
VersionStmtAttr *ASTAttrReader::readVersionStmtAttr(SourceRange SR) {
   auto version = Record.readEnum<VersionStmtAttr::VersionKind>();
   return new(C) VersionStmtAttr(std::move(version), SR);
}
ExternallyInitializedAttr *ASTAttrReader::readExternallyInitializedAttr(SourceRange SR) {
   return new(C) ExternallyInitializedAttr(SR);
}
InstantiateAttr *ASTAttrReader::readInstantiateAttr(SourceRange SR) {
   return new(C) InstantiateAttr(SR);
}
TransparentAttr *ASTAttrReader::readTransparentAttr(SourceRange SR) {
   return new(C) TransparentAttr(SR);
}
_BuiltinAttr *ASTAttrReader::read_BuiltinAttr(SourceRange SR) {
   auto builtinName = Record.getIdentifierInfo()->getIdentifier();
   return new(C) _BuiltinAttr(std::move(builtinName), SR);
}
_SemanticsAttr *ASTAttrReader::read_SemanticsAttr(SourceRange SR) {
   auto semanticsKind = Record.getIdentifierInfo()->getIdentifier();
   return new(C) _SemanticsAttr(std::move(semanticsKind), SR);
}
_DebugAttr *ASTAttrReader::read_DebugAttr(SourceRange SR) {
   return new(C) _DebugAttr(SR);
}
#endif
#undef CDOT_ATTR_SERIALIZE
#undef CDOT_ATTR_DESERIALIZE
