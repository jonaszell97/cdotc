//
// Created by Jonas Zell on 05.01.18.
//

#include "Mangle.h"

#include "AST/Decl.h"
#include "AST/PrettyPrinter.h"
#include "IL/Constants.h"
#include "Module/Module.h"
#include "Sema/SemaPass.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::module;
using namespace cdot::sema;
using namespace cdot::ast;

using std::string;

namespace cdot {
namespace {

static const unsigned UnknownArity = ~0U;

class ItaniumLikeMangler {
   llvm::raw_ostream &OS;
   llvm::StringMap<unsigned> ModuleSubstitutions;

   unsigned SeqID = 0;
   llvm::DenseMap<uintptr_t, unsigned> Substitutions;
   bool DefaultDeinit = false;

   bool shouldMangle(const NamedDecl *D);

   void mangleVarDecl(const VarDecl *D);

   void mangleLocalName(const NamedDecl *D);
   void mangleTemplateName(const NamedDecl *D);
   void mangleUnscopedName(const NamedDecl *D);

   void mangleUnqualifiedName(const NamedDecl *D)
   {
      mangleUnqualifiedName(D, D->getDeclName().getManglingName(),
                            UnknownArity);
   }

   void mangleUnqualifiedName(const NamedDecl *ND, DeclarationName Name,
                              unsigned KnownArity);

   void mangleUnscopedTemplateName(const NamedDecl *D);
   void mangleNestedName(const NamedDecl *D, const DeclContext *Ctx,
                         bool NoFunction = false);

   void mangleNestedName(const NamedDecl *D,
                         const FinalTemplateArgumentList *templateArgs);

   void mangleTemplatePrefix(const NamedDecl *ND, bool NoFunction = false);

   void mangleTemplateArgs(const NamedDecl *D);
   void mangleTemplateArgs(const FinalTemplateArgumentList &list);
   void mangleTemplateArg(const ResolvedTemplateArg &Arg);

   void mangleModuleName(const ModuleDecl *M);
   void mangleModuleNamePrefix(llvm::StringRef Name);

   void mangleSourceName(const IdentifierInfo *II);

   void mangleInitType(const InitDecl *D);
   void mangleDeinitType(const DeinitDecl *D);

   void mangleTemplateParameter(unsigned Idx);

   void mangleOperatorName(DeclarationName DN, unsigned Arity);

   void mangleQualifiers(Qualifiers Quals);
   void mangleExpression(const Expression *E);

   bool mangleSubstitution(const NamedDecl *ND);
   bool mangleSubstitution(QualType T);
   bool mangleSubstitution(uintptr_t Ptr);

   void addSubstitution(const NamedDecl *D);
   void addSubstitution(QualType T);
   void addSubstitution(uintptr_t Ptr);

#  define CDOT_TYPE(Name, Parent)    \
   void mangleType(const Name *T);
#  include "AST/Types.def"

public:
   explicit ItaniumLikeMangler(llvm::raw_ostream &OS,
                               bool DefaultDeinit = false)
      : OS(OS), DefaultDeinit(DefaultDeinit)
   {}

   void mangle(const NamedDecl *D);
   void mangleCallOffset(int64_t NonVirtual, int64_t Virtual);
   void mangleNumber(const llvm::APSInt &Value);
   void mangleNumber(int64_t Number);
   void mangleFloat(const llvm::APFloat &F);
   void mangleFunctionEncoding(const CallableDecl *D);
   void mangleSeqID(unsigned SeqID);
   void mangleName(const NamedDecl *D);
   void mangleType(QualType T);

   void manglePrefix(const DeclContext *DC, bool NoFunction = false);
   void manglePrefix(const DeclContext *DC,
                     const FinalTemplateArgumentList &TemplateArgs);

   void mangleBareFunctionType(const CallableDecl *D);
   void mangleBareFunctionType(const FunctionType *FT, bool MangleReturnType,
                               const CallableDecl *C = nullptr);

   void mangleBareFunctionType(SemaPass &SP, FunctionType *FT,
                               bool MangleReturnType, const CallableDecl *C,
                               const FinalTemplateArgumentList &TemplateArgs);
};

} // anonymous namespace

bool ItaniumLikeMangler::shouldMangle(const NamedDecl *D)
{
   return !D->isExternC();
}

void ItaniumLikeMangler::mangle(const NamedDecl *D)
{
   if (!shouldMangle(D)) {
      OS << D->getDeclName().getManglingName();
      return;
   }

   // <mangled-name> ::= _C <encoding>
   //                ::= <data name>
   //                ::= <special-name>
   if (D->isExternCXX()) {
      OS << "_Z";
   }
   else {
      OS << "_C";
   }

   if (auto Fn = dyn_cast<CallableDecl>(D))
      mangleFunctionEncoding(Fn);
   else
      mangleVarDecl(cast<VarDecl>(D));
}

void ItaniumLikeMangler::mangleFunctionEncoding(const CallableDecl *D)
{
   // <encoding> ::= <function name> <bare-function-type>
   mangleName(D);
   mangleBareFunctionType(D);
}

void ItaniumLikeMangler::mangleVarDecl(const VarDecl *D)
{
   mangleName(D);
}

void ItaniumLikeMangler::mangleBareFunctionType(const CallableDecl *D)
{
   bool mangleReturnType = false;
   if (D->isInstantiation()) {
      if (!isa<InitDecl>(D) && !isa<DeinitDecl>(D))
         mangleReturnType = true;

//      D = D->getSpecializedTemplate();
   }

   mangleBareFunctionType(D->getFunctionType(), mangleReturnType, D);
}

static bool isLocalContext(const DeclContext *Ctx)
{
   return isa<CallableDecl>(Ctx);
}

static const RecordDecl *GetLocalClassDecl(const Decl *D)
{
   const DeclContext *DC = D->getNonTransparentDeclContext();
   while (!isa<NamespaceDecl>(DC) && !isa<ModuleDecl>(DC)) {
      if (isLocalContext(DC))
         return dyn_cast<RecordDecl>(D);
      D = cast<Decl>(DC);
      DC = D->getNonTransparentDeclContext();
   }

   return nullptr;
}

static bool isStdNamespace(DeclContext *Ctx)
{
   auto NS = dyn_cast<NamespaceDecl>(Ctx);
   return NS && !NS->isAnonymousNamespace() && NS->getDeclName().isStr("std");
}

void ItaniumLikeMangler::mangleName(const NamedDecl *D)
{
   //  <name> ::= [<module-name>] <nested-name>
   //         ::= [<module-name>] <unscoped-name>
   //         ::= [<module-name>] <unscoped-template-name> <template-args>
   //         ::= <local-name>
   auto Ctx = D->getNonTransparentDeclContext();

   if (isLocalContext(Ctx))
      return mangleLocalName(D);

   if ((D->isExternCXX() && isStdNamespace(Ctx))) {
      if (D->isInstantiation()) {
         mangleUnscopedTemplateName(D);
         mangleTemplateArgs(D);
         return;
      }

      mangleUnscopedName(D);
      return;
   }

   mangleNestedName(D, Ctx);
}

void ItaniumLikeMangler::mangleModuleName(const ModuleDecl *M)
{
   // Implement the C++ Modules TS name mangling proposal; see
   //     https://gcc.gnu.org/wiki/cxx-modules?action=AttachFile
   //
   //   <module-name> ::= W <unscoped-name>+ E
   //                 ::= W <module-subst> <unscoped-name>* E
   OS << 'W';
   mangleModuleNamePrefix(M->getFullName());
   OS << 'E';
}

void ItaniumLikeMangler::mangleModuleNamePrefix(llvm::StringRef Name)
{
   //  <module-subst> ::= _ <seq-id>          # 0 < seq-id < 10
   //                 ::= W <seq-id - 10> _   # otherwise
   auto It = ModuleSubstitutions.find(Name);
   if (It != ModuleSubstitutions.end()) {
      if (It->second < 10)
         OS << '_' << static_cast<char>('0' + It->second);
      else
         OS << 'W' << (It->second - 10) << '_';
      return;
   }

   // FIXME: Preserve hierarchy in module names rather than flattening
   // them to strings; use Module*s as substitution keys.
   auto Parts = Name.rsplit('.');
   if (Parts.second.empty())
      Parts.second = Parts.first;
   else
      mangleModuleNamePrefix(Parts.first);

   OS << Parts.second.size() << Parts.second;
   ModuleSubstitutions.insert({Name, ModuleSubstitutions.size()});
}

void ItaniumLikeMangler::mangleTemplateName(const NamedDecl *D)
{
   if (isa<ModuleDecl>(D->getNonTransparentDeclContext())
         || (D->isExternCXX()
             && isStdNamespace(D->getNonTransparentDeclContext()))) {
      mangleUnscopedTemplateName(D);
      mangleTemplateArgs(D);
   }
   else {
      mangleNestedName(D, &D->getTemplateArgs());
   }
}

void ItaniumLikeMangler::mangleUnscopedName(const NamedDecl *D)
{
   //  <unscoped-name> ::= <unqualified-name>
   //                  ::= St <unqualified-name>   # ::std:: in extern(C++)
   if (D->isExternCXX() && isStdNamespace(D->getNonTransparentDeclContext())) {
      OS << "St";
   }

   mangleUnqualifiedName(D);
}

void ItaniumLikeMangler::mangleUnscopedTemplateName(const NamedDecl *D)
{
   //     <unscoped-template-name> ::= <unscoped-name>
   //                              ::= <substitution>
   if (mangleSubstitution(D))
      return;

   mangleUnscopedName(D);
   addSubstitution(D);
}

void ItaniumLikeMangler::mangleFloat(const llvm::APFloat &F)
{
   // ABI:
   //   Floating-point literals are encoded using a fixed-length
   //   lowercase hexadecimal string corresponding to the internal
   //   representation (IEEE on Itanium), high-order bytes first,
   //   without leading zeroes. For example: "Lf bf800000 E" is -1.0f
   //   on Itanium.
   // The 'without leading zeroes' thing seems to be an editorial
   // mistake; see the discussion on cxx-abi-dev beginning on
   // 2012-01-16.

   // Our requirements here are just barely weird enough to justify
   // using a custom algorithm instead of post-processing APInt::toString().

   llvm::APInt valueBits = F.bitcastToAPInt();
   unsigned numCharacters = (valueBits.getBitWidth() + 3) / 4;
   assert(numCharacters != 0);

   // Allocate a buffer of the right number of characters.
   llvm::SmallVector<char, 20> buffer(numCharacters);

   // Fill the buffer left-to-right.
   for (unsigned stringIndex = 0; stringIndex != numCharacters; ++stringIndex) {
      // The bit-index of the next hex digit.
      unsigned digitBitIndex = 4 * (numCharacters - stringIndex - 1);

      // Project out 4 bits starting at 'digitIndex'.
      uint64_t hexDigit = valueBits.getRawData()[digitBitIndex / 64];
      hexDigit >>= (digitBitIndex % 64);
      hexDigit &= 0xF;

      // Map that over to a lowercase hex digit.
      static const char charForHex[16] = {
         '0', '1', '2', '3', '4', '5', '6', '7',
         '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
      };
      buffer[stringIndex] = charForHex[hexDigit];
   }

   OS.write(buffer.data(), numCharacters);
}

void ItaniumLikeMangler::mangleNumber(const llvm::APSInt &Value)
{
   if (Value.isSigned() && Value.isNegative()) {
      OS << 'n';
      Value.abs().print(OS, /*signed*/ false);
   } else {
      Value.print(OS, /*signed*/ false);
   }
}

void ItaniumLikeMangler::mangleNumber(int64_t Number)
{
   //  <number> ::= [n] <non-negative decimal integer>
   if (Number < 0) {
      OS << 'n';
      Number = -Number;
   }

   OS << Number;
}

void ItaniumLikeMangler::mangleCallOffset(int64_t NonVirtual, int64_t Virtual)
{
   //  <call-offset>  ::= h <nv-offset> _
   //                 ::= v <v-offset> _
   //  <nv-offset>    ::= <offset number>        # non-virtual base override
   //  <v-offset>     ::= <offset number> _ <virtual offset number>
   //                      # virtual base override, with vcall offset
   if (!Virtual) {
      OS << 'h';
      mangleNumber(NonVirtual);
      OS << '_';
      return;
   }

   OS << 'v';
   mangleNumber(NonVirtual);
   OS << '_';
   mangleNumber(Virtual);
   OS << '_';
}

void ItaniumLikeMangler::mangleUnqualifiedName(const NamedDecl *ND,
                                               DeclarationName Name,
                                               unsigned KnownArity) {
   unsigned Arity = KnownArity;
   //  <unqualified-name> ::= <operator-name>
   //                     ::= <ctor-dtor-name>
   //                     ::= <source-name>

   switch (Name.getKind()) {
   case DeclarationName::NormalIdentifier: {
      auto *II = Name.getIdentifierInfo();
      if (II) {
         mangleSourceName(II);
      }
      else {
         // must be anonymous namespace
         assert(isa<NamespaceDecl>(ND));
         OS << "12_GLOBAL__N_1";
      }

      break;
   }
   case DeclarationName::InstantiationName:
      mangleUnqualifiedName(ND, Name.getInstantiationName(), KnownArity);
      break;
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName: {
      mangleInitType(cast<InitDecl>(ND));
      break;
   }
   case DeclarationName::DestructorName: {
      mangleDeinitType(cast<DeinitDecl>(ND));
      break;
   }
   case DeclarationName::ConversionOperatorName:
      if (ND && Arity == UnknownArity) {
         Arity = (unsigned)cast<CallableDecl>(ND)->getArgs().size();
      }

      LLVM_FALLTHROUGH;
   case DeclarationName::InfixOperatorName:
   case DeclarationName::PrefixOperatorName:
   case DeclarationName::PostfixOperatorName:
      mangleOperatorName(Name, Arity);
      break;
   case DeclarationName::AccessorName: {
      auto AccKind = Name.getAccessorKind();
      auto AccName = Name.getAccessorName();

      switch (AccKind) {
      case DeclarationName::Getter:
         OS << "Ga";
         break;
      case DeclarationName::Setter:
         OS << "Sa";
         break;
      }

      mangleSourceName(AccName);
      break;
   }
   case DeclarationName::ClosureArgumentName:
      OS << "Ca" << Name.getClosureArgumentIdx();
      break;
   case DeclarationName::SubscriptName:
      switch (Name.getSubscriptKind()) {
      case DeclarationName::SubscriptKind::Getter:
         OS << "Gs";
         return;
      case DeclarationName::SubscriptKind::Setter:
         OS << "Ss";
         return;
      default:
         break;
      }

      LLVM_FALLTHROUGH;
   case DeclarationName::ExtensionName:
   case DeclarationName::LocalVarName:
   case DeclarationName::PackExpansionName:
   case DeclarationName::OperatorDeclName:
   case DeclarationName::MacroName:
   case DeclarationName::ErrorName:
      llvm_unreachable("should never be mangled!");
   }
}

void ItaniumLikeMangler::mangleSourceName(const IdentifierInfo *II)
{
   // <source-name> ::= <positive length number> <identifier>
   // <number> ::= [n] <non-negative decimal integer>
   // <identifier> ::= <unqualified source code identifier>
   OS << II->getLength() << II->getIdentifier();
}

void ItaniumLikeMangler::mangleNestedName(const NamedDecl *D,
                                          const DeclContext *Ctx,
                                          bool NoFunction) {
   // <nested-name>
   //   ::= N [<CV-qualifiers>] [<ref-qualifier>] <prefix> <unqualified-name> E
   //   ::= N [<CV-qualifiers>] [<ref-qualifier>] <template-prefix>
   //       <template-args> E

   OS << 'N';
   if (D->isInstantiation()) {
      mangleTemplatePrefix(D, NoFunction);
      mangleTemplateArgs(D);
   }
   else {
      manglePrefix(Ctx, NoFunction);
      mangleUnqualifiedName(D);
   }
   OS << 'E';
}

void ItaniumLikeMangler::mangleNestedName(const NamedDecl *D,
                                          const FinalTemplateArgumentList *) {
   // <nested-name> ::= N [<CV-qualifiers>] <template-prefix> <template-args> E

   OS << 'N';

   mangleTemplatePrefix(D);
   mangleTemplateArgs(D);

   OS << 'E';
}

void ItaniumLikeMangler::mangleLocalName(const NamedDecl *D)
{
   // <local-name> := Z <function encoding> E <entity name> [<discriminator>]
   //              := Z <function encoding> E s [<discriminator>]
   // <local-name> := Z <function encoding> E d [ <parameter number> ]
   //                 _ <entity name>
   // <discriminator> := _ <non-negative number>

   OS << 'Z';

   auto Ctx = D->getNonTransparentDeclContext();
   mangleFunctionEncoding(cast<CallableDecl>(Ctx));

   OS << 'E';

   if (D->isInstantiation()) {
      mangleTemplatePrefix(D, true);
      mangleTemplateArgs(D);
   }
   else if (auto RD = GetLocalClassDecl(D)) {
      // Mangle the name relative to the closest enclosing function.
      // equality ok because RD derived from ND above
      if (D == RD)  {
         mangleUnqualifiedName(RD);
      }
      else {
         const NamedDecl *ND = cast<NamedDecl>(D);
         mangleNestedName(ND, ND->getNonTransparentDeclContext(),
                          true /*NoFunction*/);
      }
   }
   else {
      mangleUnqualifiedName(D);
   }

//   if (const NamedDecl *ND = dyn_cast<NamedDecl>(RD ? RD : D)) {
//      unsigned disc;
//      if (Context.getNextDiscriminator(ND, disc)) {
//         if (disc < 10)
//            Out << '_' << disc;
//         else
//            Out << "__" << disc << '_';
//      }
//   }
}

void ItaniumLikeMangler::manglePrefix(const DeclContext *DC, bool NoFunction)
{
   //  <prefix> ::= <prefix> <unqualified-name>
   //           ::= <template-prefix> <template-args>
   //           ::= <template-param>
   //           ::= # empty
   //           ::= <substitution>

   DC = DC->lookThroughExtension();

   while (!isa<NamedDecl>(DC))
      DC = DC->getParentCtx();

   if (isa<ModuleDecl>(DC))
      return mangleModuleName(cast<ModuleDecl>(DC));

   if (NoFunction && isLocalContext(DC))
      return;

   const NamedDecl *ND = cast<NamedDecl>(DC);
   if (mangleSubstitution(ND))
      return;

   // Check if we have a template.
   if (ND->isInstantiation()) {
      mangleTemplatePrefix(ND);
      mangleTemplateArgs(ND);
   } else {
      manglePrefix(ND->getNonTransparentDeclContext(), NoFunction);
      mangleUnqualifiedName(ND);
   }

   addSubstitution(ND);
}

void ItaniumLikeMangler::manglePrefix(const DeclContext *DC,
                                      const FinalTemplateArgumentList &Args) {
//   assert(!isLocalContext(DC));

   const NamedDecl *ND = cast<NamedDecl>(DC);
   mangleTemplatePrefix(ND);
   mangleTemplateArgs(Args);
}

void ItaniumLikeMangler::mangleTemplatePrefix(const NamedDecl *ND,
                                              bool NoFunction) {
   // <template-prefix> ::= <prefix> <template unqualified-name>
   //                   ::= <template-param>
   //                   ::= <substitution>
   if (mangleSubstitution(ND))
      return;

   manglePrefix(ND->getNonTransparentDeclContext(), NoFunction);
   mangleUnqualifiedName(ND);

   addSubstitution(ND);
}

void ItaniumLikeMangler::mangleOperatorName(DeclarationName DN,
                                            unsigned Arity) {
   switch (DN.getKind()) {
   case DeclarationName::ConversionOperatorName: {
      // <operator-name> ::= cv <type>    # (cast)
      OS << "cv";
      mangleType(DN.getConversionOperatorType());
      break;
   }
   case DeclarationName::InfixOperatorName: {
      // <operator-name> ::= Xi <name>    # (infix operator)
      OS << "Xi";
      mangleSourceName(DN.getInfixOperatorName());
      break;
   }
   case DeclarationName::PrefixOperatorName: {
      // <operator-name> ::= Xp <name>    # (prefix operator)
      OS << "Xp";
      mangleSourceName(DN.getPrefixOperatorName());
      break;
   }
   case DeclarationName::PostfixOperatorName: {
      // <operator-name> ::= Xo <name>    # (postfix operator)
      OS << "Xo";
      mangleSourceName(DN.getPostfixOperatorName());
      break;
   }
   default:
      llvm_unreachable("not an operator name!");
   }
}

void ItaniumLikeMangler::mangleQualifiers(Qualifiers)
{
   // <CV-qualifiers> ::= [r] [V] [K]    # restrict (C99), volatile, const
}

static bool isTypeSubstitutable(Qualifiers Quals, QualType Ty)
{
//   if (Quals)
//      return true;
//   if (Ty->isBuiltinType())
//      return false;
//
//   return true;
   return Ty->getTypeID() != Type::BuiltinTypeID;
}

void ItaniumLikeMangler::mangleType(QualType T)
{
   bool isSubstitutable = isTypeSubstitutable(T.getQuals(), T);
   if (isSubstitutable && mangleSubstitution(T))
      return;

   switch (T->getTypeID()) {
#  define CDOT_TYPE(Name, Parent)                            \
   case Type::Name##ID: mangleType(cast<Name>(T)); break;
#  include "AST/Types.def"
   }

   // Add the substitution.
   if (isSubstitutable)
      addSubstitution(T);
}

void ItaniumLikeMangler::mangleType(const BuiltinType *T)
{
   //  <type>         ::= <builtin-type>
   //  <builtin-type> ::= v  # void
   //                 ::= w  # wchar_t
   //                 ::= b  # bool
   //                 ::= c  # char
   //                 ::= a  # signed char
   //                 ::= h  # unsigned char
   //                 ::= s  # short
   //                 ::= t  # unsigned short
   //                 ::= i  # int
   //                 ::= j  # unsigned int
   //                 ::= l  # long
   //                 ::= m  # unsigned long
   //                 ::= x  # long long, __int64
   //                 ::= y  # unsigned long long, __int64
   //                 ::= n  # __int128
   //                 ::= o  # unsigned __int128
   //                 ::= f  # float
   //                 ::= d  # double
   //                 ::= e  # long double, __float80
   //                 ::= g  # __float128
   // UNSUPPORTED:    ::= Dd # IEEE 754r decimal floating point (64 bits)
   // UNSUPPORTED:    ::= De # IEEE 754r decimal floating point (128 bits)
   // UNSUPPORTED:    ::= Df # IEEE 754r decimal floating point (32 bits)
   //                 ::= Dh # IEEE 754r half-precision floating point (16 bits)
   //                 ::= DF <number> _ # ISO/IEC TS 18661 binary floating point type _FloatN (N bits);
   //                 ::= Di # char32_t
   //                 ::= Ds # char16_t
   //                 ::= Dn # std::nullptr_t (i.e., decltype(nullptr))
   //                 ::= u <source-name>    # vendor extended type

   switch (T->getKind()) {
   case BuiltinType::Void:
      OS << 'v';
      break;
   case BuiltinType::i1:
      OS << 'b';
      break;
   case BuiltinType::u8:
      OS << 'h';
      break;
   case BuiltinType::u16:
      OS << 't';
      break;
   case BuiltinType::u32:
      OS << 'j';
      break;
   case BuiltinType::u64:
      OS << 'm'; // or 'y' for long long
      break;
   case BuiltinType::u128:
      OS << 'o';
      break;
   case BuiltinType::i8:
      OS << 'a';
      break;
   case BuiltinType::i16:
      OS << 's';
      break;
   case BuiltinType::i32:
      OS << 'i';
      break;
   case BuiltinType::i64:
      OS << 'l'; // or 'x' for long long
      break;
   case BuiltinType::i128:
      OS << 'n';
      break;
   case BuiltinType::f16:
      OS << "Dh";
      break;
   case BuiltinType::f32:
      OS << 'f';
      break;
   case BuiltinType::f64:
      OS << 'd';
      break;
   case BuiltinType::f80:
      OS << 'e';
      break;
   case BuiltinType::f128:
      OS << 'g';
      break;
   default:
      llvm_unreachable("bad builtin type kind!");
   }
}

void ItaniumLikeMangler::mangleType(const FunctionType *T)
{
   // <type>          ::= <function-type>
   // <function-type> ::= [<CV-qualifiers>] F [Y]
   //                      <bare-function-type> [<ref-qualifier>] E

   if (!T->throws()) {
      OS << "Do";
   }

   OS << 'F';
   mangleBareFunctionType(T, /*MangleReturnType=*/true);
   OS << 'E';
}

void ItaniumLikeMangler::mangleBareFunctionType(const FunctionType *FT,
                                                bool MangleReturnType,
                                                const CallableDecl *CD) {
   // <bare-function-type> ::= <signature type>+
   if (MangleReturnType) {
      mangleType(FT->getReturnType());
   }

   if (FT->getNumParams() == 0 && !FT->isCStyleVararg()) {
      //   <builtin-type> ::= v   # void
      OS << 'v';
      return;
   }

   unsigned NumParams = 0;
   for (auto &Param : FT->getParamTypes()) {
      mangleType(Param);
      ++NumParams;
   }

   if (CD && !CD->isExternC() && !CD->isExternCXX()) {
      auto Args = CD->getArgs();
      for (unsigned i = 0; i < NumParams; ++i) {
         IdentifierInfo *Label;
         if (i < Args.size()) {
            Label = Args[i]->getLabel();
         }
         else {
            Label = nullptr;
         }

         if (Label) {
            OS << 'L' << Label->getLength() << Label->getIdentifier();
         }
         else {
            OS << "L0";
         }
      }
   }

   // <builtin-type>      ::= z  # ellipsis
   if (FT->isCStyleVararg())
      OS << 'z';
}

void ItaniumLikeMangler::mangleBareFunctionType(SemaPass &SP,
                                                FunctionType *FT,
                                                bool MangleReturnType,
                                                const CallableDecl *C,
                                                const FinalTemplateArgumentList &TemplateArgs) {
   MultiLevelFinalTemplateArgList Args(TemplateArgs);
   auto *Ty = cast<FunctionType>(
      SP.resolveDependencies(FT, Args, const_cast<CallableDecl*>(C)));

   return mangleBareFunctionType(Ty, MangleReturnType, C);
}

void ItaniumLikeMangler::mangleType(const RecordType *T)
{
   mangleName(T->getRecord());
}

void ItaniumLikeMangler::mangleType(const DependentRecordType *T)
{
   OS << 'N';
   mangleTemplatePrefix(T->getRecord());
   mangleTemplateArgs(T->getTemplateArgs());
   OS << 'E';
}

void ItaniumLikeMangler::mangleType(const ArrayType *T)
{
   // <type>       ::= <array-type>
   // <array-type> ::= A <positive dimension number> _ <element type>
   //              ::= A [<dimension expression>] _ <element type>
   OS << 'A' << T->getNumElements() << '_';
   mangleType(T->getElementType());
}

void ItaniumLikeMangler::mangleType(const DependentSizeArrayType *T)
{
   OS << 'A';
   mangleExpression(T->getSizeExpr());
   OS << '_';
   mangleType(T->getElementType());
}

void ItaniumLikeMangler::mangleType(const InferredSizeArrayType *T)
{
   OS << "A_";
   mangleType(T->getElementType());
}

void ItaniumLikeMangler::mangleType(const DependentNameType *T)
{
   std::string str;
   {
      llvm::raw_string_ostream OS(str);
      OS << T->getNameSpec();
   }

   this->OS << "Dt" << str.size() << str;
}

void ItaniumLikeMangler::mangleType(const PointerType *T)
{
   // <type> ::= P <type>   # pointer-to
   OS << "KP";
   mangleType(T->getPointeeType());
}

void ItaniumLikeMangler::mangleType(const MutablePointerType *T)
{
   // <type> ::= P <type>   # pointer-to
   OS << 'P';
   mangleType(T->getPointeeType());
}

void ItaniumLikeMangler::mangleType(const ReferenceType *T)
{
   // <type> ::= R <type>   # reference-to
   OS << "KR";
   mangleType(T->getReferencedType());
}

void ItaniumLikeMangler::mangleType(const MutableReferenceType *T)
{
   // <type> ::= R <type>   # reference-to
   OS << 'R';
   mangleType(T->getReferencedType());
}

void ItaniumLikeMangler::mangleType(const MutableBorrowType *T)
{
   // <type> ::= R <type>   # reference-to
   OS << 'R';
   mangleType(T->getBorrowedType());
}

void ItaniumLikeMangler::mangleType(const BoxType *T)
{
   OS << 'X';
   mangleType(T->getBoxedType());
}

void ItaniumLikeMangler::mangleType(const TokenType*)
{
   OS << "To";
}

void ItaniumLikeMangler::mangleType(const LambdaType *T)
{
   mangleType((FunctionType*)T);
}

void ItaniumLikeMangler::mangleType(const GenericType *T)
{
   OS << 'G';
   mangleTemplateParameter(T->getIndex());

   if (T->getParam()->isVariadic())
      OS << 'z';
}

void ItaniumLikeMangler::mangleType(const AssociatedType *T)
{
   assert(T->getDecl()->isImplementation() && "mangling protocol method!");
   mangleType(T->getActualType());
}

void ItaniumLikeMangler::mangleType(const TupleType *T)
{
   OS << 'T';

   if (T->getArity() == 0) {
      //   <builtin-type> ::= v   # void
      OS << 'v';
      return;
   }

   for (auto &Param : T->getContainedTypes()) {
      mangleType(Param);
   }
}

void ItaniumLikeMangler::mangleType(const MetaType *T)
{
   OS << 'M';
   mangleType(T->getUnderlyingType());
}

void ItaniumLikeMangler::mangleType(const TypedefType *T)
{
   OS << 'Y';
   mangleName(T->getTypedef());
}

void ItaniumLikeMangler::mangleExpression(const Expression *E)
{
   string constraintString;
   llvm::raw_string_ostream sout(constraintString);
   ast::PrettyPrinter printer(sout);
   printer.print(const_cast<Expression*>(E));
}

void ItaniumLikeMangler::mangleInitType(const InitDecl *Decl)
{
   // <ctor-dtor-name> ::= C1  # complete object constructor
   //                  ::= C2  # base object constructor
   //                  ::= CI1 <type> # complete inheriting constructor
   //                  ::= CI2 <type> # base inheriting constructor
   //
   OS << 'C';
   switch (Decl->getCtorKind()) {
   case ConstructorKind::Complete:
      OS << '1';
      break;
   case ConstructorKind::Base:
      OS << '2';
      break;
   }
}

void ItaniumLikeMangler::mangleDeinitType(const DeinitDecl *)
{
   // <ctor-dtor-name> ::= D0  # deleting destructor
   //                  ::= D1  # complete object destructor
   //                  ::= D2  # base object destructor
   //                  ::= D3  # default deinitializer (ADDED)
   if (DefaultDeinit) {
      OS << "D3";
   }
   else {
      OS << "D0";
   }
}

void ItaniumLikeMangler::mangleTemplateArgs(const NamedDecl *D)
{
   // <template-args> ::= I <template-arg>+ E
   mangleTemplateArgs(D->getTemplateArgs());
}

void ItaniumLikeMangler::mangleTemplateArgs(const FinalTemplateArgumentList &list)
{
   OS << 'I';
   for (auto &Arg : list)
      mangleTemplateArg(Arg);

   OS << 'E';
}

void ItaniumLikeMangler::mangleTemplateArg(const ResolvedTemplateArg &Arg)
{
   // <template-arg> ::= <type>              # type or template
   //                ::= X <expression> E    # expression
   //                ::= <expr-primary>      # simple expressions
   //                ::= J <template-arg>* E # argument pack

   if (Arg.isVariadic()) {
      OS << 'J';
      for (auto &VA : Arg.getVariadicArgs())
         mangleTemplateArg(VA);
      OS << 'E';
   }
   else if (Arg.isType()) {
      mangleType(Arg.getType());
   }
   else {
      OS << *Arg.getValue();
   }
}

void ItaniumLikeMangler::mangleTemplateParameter(unsigned Idx)
{
   // <template-param> ::= T_    # first template parameter
   //                  ::= T <parameter-2 non-negative number> _
   if (Idx == 0)
      OS << "T_";
   else
      OS << 'T' << (Idx - 1) << '_';
}

void ItaniumLikeMangler::mangleSeqID(unsigned SeqID)
{
   if (SeqID == 1)
      OS << '0';
   else if (SeqID > 1) {
      SeqID--;

      // <seq-id> is encoded in base-36, using digits and upper case letters.
      char Buffer[7]; // log(2**32) / log(36) ~= 7
      llvm::MutableArrayRef<char> BufferRef(Buffer);
      llvm::MutableArrayRef<char>::reverse_iterator I = BufferRef.rbegin();

      for (; SeqID != 0; SeqID /= 36) {
         unsigned C = SeqID % 36;
         *I++ = (char)(C < 10 ? '0' + C : 'A' + C - 10);
      }

      OS.write(I.base(), I - BufferRef.rbegin());
   }
   OS << '_';
}

// <substitution> ::= S <seq-id> _
//                ::= S_
bool ItaniumLikeMangler::mangleSubstitution(const NamedDecl *ND)
{
   return mangleSubstitution(reinterpret_cast<uintptr_t>(ND));
}

bool ItaniumLikeMangler::mangleSubstitution(QualType T)
{
   if (const RecordType *RT = T->asRecordType())
      return mangleSubstitution(RT->getRecord());
   if (const TypedefType *TT = T->asTypedefType())
      return mangleSubstitution(TT->getTypedef());

   return mangleSubstitution(
      reinterpret_cast<uintptr_t>(T.getAsOpaquePtr()));
}

bool ItaniumLikeMangler::mangleSubstitution(uintptr_t Ptr)
{
   llvm::DenseMap<uintptr_t, unsigned>::iterator I = Substitutions.find(Ptr);
   if (I == Substitutions.end())
      return false;

   unsigned SeqID = I->second;
   OS << 'S';
   mangleSeqID(SeqID);

   return true;
}

void ItaniumLikeMangler::addSubstitution(QualType T)
{
   uintptr_t TypePtr = reinterpret_cast<uintptr_t>(T.getAsOpaquePtr());
   addSubstitution(TypePtr);
}

void ItaniumLikeMangler::addSubstitution(const NamedDecl *D)
{
   uintptr_t TypePtr = reinterpret_cast<uintptr_t>(D);
   addSubstitution(TypePtr);
}

void ItaniumLikeMangler::addSubstitution(uintptr_t Ptr)
{
   Substitutions[Ptr] = SeqID++;
}

void SymbolMangler::mangle(const NamedDecl *ND,
                           llvm::raw_ostream &OS) const {
   (void)SP;

   auto it = Cache.find(ND);
   if (it != Cache.end()) {
      OS << it->getSecond();
      return;
   }

   std::string str;
   llvm::raw_string_ostream SS(str);

   ItaniumLikeMangler Mangler(SS);
   Mangler.mangle(ND);

   OS << SS.str();
   Cache.try_emplace(ND, move(str));
}

void SymbolMangler::manglePrefix(const DeclContext *DC,
                                 llvm::raw_ostream &OS) const {
   auto it = PrefixCache.find(DC);
   if (it != PrefixCache.end()) {
      OS << it->getSecond();
      return;
   }

   auto *ND = cast<NamedDecl>(const_cast<DeclContext*>(DC));
   std::string str;
   {
      llvm::raw_string_ostream SS(str);
      manglePrefix(cast<DeclContext>(ND->getSpecializedTemplate()),
                   ND->getTemplateArgs(), SS);
   }

   OS << str;
   PrefixCache.try_emplace(DC, move(str));
}

void SymbolMangler::manglePrefix(const DeclContext *DC,
                                 const FinalTemplateArgumentList &TemplateArgs,
                                 llvm::raw_ostream &OS) const {
   ItaniumLikeMangler Mangler(OS);
   Mangler.manglePrefix(DC, TemplateArgs);

   if (auto *CD = dyn_cast<CallableDecl>(DC)) {
      Mangler.mangleBareFunctionType(CD);
   }
}

llvm::StringRef SymbolMangler::getPrefix(const DeclContext *DC) const
{
   assert(isa<NamedDecl>(DC) && cast<NamedDecl>(DC)->isInstantiation());

   auto It = PrefixCache.find(DC);
   if (It != PrefixCache.end())
      return It->getSecond();

   {
      std::string str;
      llvm::raw_string_ostream OS(str);

      manglePrefix(DC, OS);
   }

   return PrefixCache[DC];
}

void SymbolMangler::mangleVTable(const RecordDecl *R,
                                 llvm::raw_ostream &OS) const {
   // <special-name> ::= TV <type>  # virtual table
   ItaniumLikeMangler Mangler(OS);
   OS << "_CTV";
   Mangler.mangleName(R);
}

void SymbolMangler::manglePTable(const RecordDecl *R,
                                 const ProtocolDecl *P,
                                 llvm::raw_ostream &OS) const {
   // <special-name> ::= TP <type> <protocol>  # protocol virtual table
   ItaniumLikeMangler Mangler(OS);
   OS << "_CTP";
   Mangler.mangleName(R);
   Mangler.mangleName(P);
}

void SymbolMangler::mangleTypeInfo(const QualType &T,
                                   llvm::raw_ostream &OS) const {
   // <special-name> ::= TI <type>  # typeinfo structure
   ItaniumLikeMangler Mangler(OS);
   OS << "_CTI";
   Mangler.mangleType(T);
}

void SymbolMangler::mangleTypeName(const QualType &T,
                                   llvm::raw_ostream &OS) const {
   ItaniumLikeMangler Mangler(OS);
   Mangler.mangleType(T);
}

void SymbolMangler::mangleDefaultDeinitializer(RecordDecl *R,
                                               llvm::raw_ostream &OS) const {
   std::string str;
   llvm::raw_string_ostream SS(str);

   ItaniumLikeMangler Mangler(SS, true);
   Mangler.mangle(R->getDeinitializer());

   OS << SS.str();
}

} // namespace cdot