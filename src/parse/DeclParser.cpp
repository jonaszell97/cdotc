////
//// Created by Jonas Zell on 28.11.17.
////
//
//#include "DeclParser.h"
//
//#include "../module/Module.h"
//#include "../lex/Lexer.h"
//#include "../Variant/Type/QualType.h"
//#include "DelayedParseLoc.h"
//
//#include "../AST/Passes/SemanticAnalysis/Record/Record.h"
//#include "../AST/Passes/SemanticAnalysis/Record/Class.h"
//#include "../AST/Passes/SemanticAnalysis/Record/Union.h"
//#include "../AST/Passes/SemanticAnalysis/Record/Protocol.h"
//#include "../AST/Passes/SemanticAnalysis/Record/Enum.h"
//
//#include "../Variant/Type/PointerType.h"
//#include "../Variant/Type/VoidType.h"
//#include "../Variant/Type/AutoType.h"
//#include "../Variant/Type/FunctionType.h"
//#include "../Variant/Type/TupleType.h"
//
//#include "../Message/Diagnostics.h"
//
//using namespace cdot::module;
//using namespace cdot::cl;
//using namespace cdot::lex;
//using namespace cdot::diag;
//
//namespace cdot {
//namespace parse {
//
//class ParserImpl {
//public:
//   ParserImpl(module::Module *M,
//              llvm::MemoryBuffer *Buf,
//              unsigned sourceId)
//      : lexer(Buf, sourceId), M(M)
//   { }
//
//   void ParseDecls();
//
//private:
//   void parseNextDecl();
//
//   typedef unsigned short ModifierTy;
//
//   enum DeclModifiers: ModifierTy {
//      Public      = 1,
//      Private     = 1 << 1,
//      Protected   = 1 << 2,
//      Static      = 1 << 3,
//      Mutating    = 1 << 4,
//      Operator    = 1 << 5,
//      Abstract    = 1 << 6,
//      Final       = 1 << 7,
//      Sealed      = 1 << 8,
//   };
//
//   void parseRecordCommon(Record *R);
//   void parseStructCommon(Struct *S);
//
//   void parseClass();
//   void parseEnum();
//   void parseStruct();
//   void parseUnion();
//   void parseProtocol();
//
//   void parseTypedef();
//   void parseGlobalVariable();
//   void parseNamespace();
//
//   void parseExtension();
//
//   void parseFunction();
//
//   AccessModifier maybeParseAccessModifier()
//   {
//      AccessModifier am = AccessModifier::DEFAULT;
//      switch (lexer.currentTok().getKind()) {
//         case tok::kw_public:
//            am = AccessModifier::PUBLIC;
//            break;
//         case tok::kw_private:
//            am = AccessModifier::PRIVATE;
//            break;
//         case tok::kw_protected:
//            am = AccessModifier::PROTECTED;
//            break;
//         default:
//            break;
//      }
//
//      if (am != AccessModifier::DEFAULT)
//         lexer.advance();
//
//      return am;
//   }
//
//   void pushNamespace(Namespace *NS)
//   {
//      NamespaceStack.push(NS);
//   }
//
//   void popNamespace()
//   {
//      NamespaceStack.pop();
//   }
//
//   string getNSQualifiedName(const string &declName) const
//   {
//      if (NamespaceStack.empty())
//         return "";
//
//      string res = NamespaceStack.top()->name;
//      res += '.';
//      res += declName;
//
//      return res;
//   }
//
//   size_t getNamespaceID() const
//   {
//      return NamespaceStack.empty() ? 0 : NamespaceStack.top()->id;
//   }
//
//   Token const& currentTok() const
//   {
//      return lexer.currentTok();
//   }
//
//   Token lookahead(bool ignoreNewline = true)
//   {
//      return lexer.lookahead(ignoreNewline);
//   }
//
//   void advance(bool ignoreNewline = true)
//   {
//      lexer.advance(ignoreNewline);
//   }
//
//   void tokenError()
//   {
//      diag::err(err_generic_error)
//         << "unexpected token "
//            + tok::tokenTypeToString(currentTok().getKind())
//         << &lexer << diag::term;
//   }
//
//   template<class... Rest>
//   void errorIfNot(tok::TokenType ty, Rest... rest)
//   {
//      if (currentTok().getKind() == ty)
//         return;
//
//      errorIfNot(rest...);
//   }
//
//   void errorIfNot(tok::TokenType ty)
//   {
//      if (currentTok().getKind() == ty)
//         return;
//
//      tokenError();
//   }
//
//   template<class... Rest>
//   void advanceWhile(tok::TokenType ty, Rest... rest)
//   {
//      while (lexer.advanceIf(ty, rest...));
//   }
//
//   lex::Lexer<> lexer;
//
//   Module *M;
//
//   std::stack<Namespace*> NamespaceStack;
//};
//
//void ParserImpl::ParseDecls()
//{
//   while (1) {
//      while (!currentTok().is_keyword() && !currentTok().is(tok::eof)) {
//         lexer.advance();
//      }
//
//      if (currentTok().is(tok::eof))
//         break;
//
//      parseNextDecl();
//      lexer.advance();
//   }
//}
//
//void ParserImpl::parseNextDecl()
//{
//   assert(currentTok().is_keyword() && "should only be called on a keyword");
//   tok::TokenType relevantToken = tok::sentinel;
//
//   switch (currentTok().getKind()) {
//      case tok::kw_def:
//         return parseFunction();
//      case tok::kw_typedef:
//         return parseTypedef();
//      case tok::kw_namespace:
//         return parseNamespace();
//      case tok::kw_var:
//      case tok::kw_let:
//         return parseGlobalVariable();
//      case tok::kw_struct:
//         return parseStruct();
//      case tok::kw_class:
//         return parseClass();
//      case tok::kw_enum:
//         return parseEnum();
//      case tok::kw_union:
//         return parseUnion();
//      case tok::kw_protocol:
//         return parseProtocol();
//      case tok::kw_extend:
//         return parseExtension();
//      case tok::kw_public:
//      case tok::kw_abstract:
//      case tok::kw_private:
//         {
//            Lexer<>::StateSaveGuard guard(&lexer);
//            while (relevantToken == tok::sentinel) {
//               if (currentTok().oneOf(tok::kw_struct, tok::kw_enum,
//                                      tok::kw_class, tok::kw_union,
//                                      tok::kw_protocol, tok::kw_let,
//                                      tok::kw_var, tok::kw_def,
//                                      tok::kw_typedef, tok::kw_extend)) {
//                  relevantToken = currentTok().getKind();
//                  break;
//               }
//
//               lexer.advance();
//            }
//         }
//         switch (relevantToken) {
//            case tok::kw_struct:
//               return parseStruct();
//            case tok::kw_class:
//               return parseClass();
//            case tok::kw_enum:
//               return parseEnum();
//            case tok::kw_union:
//               return parseUnion();
//            case tok::kw_protocol:
//               return parseProtocol();
//            case tok::kw_extend:
//               return parseExtension();
//            case tok::kw_typedef:
//               return parseTypedef();
//            case tok::kw_var:
//            case tok::kw_let:
//               return parseGlobalVariable();
//            case tok::kw_def:
//               return parseFunction();
//            default:
//               break;
//         }
//         LLVM_FALLTHROUGH;
//      default:
//         return;
//   }
//}
//
//void ParserImpl::parseGlobalVariable()
//{
//   auto access = maybeParseAccessModifier();
//   (void)access;
//   lexer.advanceIf(tok::kw_public, tok::kw_private, tok::kw_protected);
//
//   auto isConst = currentTok().is(tok::kw_let);
//   lexer.expect(tok::ident);
//
//   QualType type(AutoType::get());
//   auto ident = move(lexer.strRef());
//
//   type.isConst(isConst);
//}
//
//void ParserImpl::parseTypedef()
//{
//   auto access = maybeParseAccessModifier();
//   lexer.advanceIf(tok::kw_public, tok::kw_private, tok::kw_protected,
//                   tok::kw_typedef);
//   lexer.advanceIf(tok::kw_public, tok::kw_private, tok::kw_protected,
//                   tok::kw_typedef);
//
//   auto alias = move(lexer.strRef());
//
//   lexer.expect(tok::equals);
//
//   auto Entry = SymbolTable::declareTypedef(alias, AutoType::get(), {},
//                                            access);
//
//   M->addEntry(Entry);
//}
//
//void ParserImpl::parseFunction()
//{
//   lexer.advanceIf(tok::kw_public, tok::kw_private, tok::kw_protected,
//                   tok::kw_def);
//   lexer.advanceIf(tok::kw_public, tok::kw_private, tok::kw_protected,
//                   tok::kw_def);
//
//   lexer.expect(tok::ident);
//   auto funcName = move(lexer.strRef());
//
//}
//
//void ParserImpl::parseRecordCommon(Record *R)
//{
//
//}
//
//void ParserImpl::parseStructCommon(Struct *S)
//{
//
//}
//
//void ParserImpl::parseStruct()
//{
//
//}
//
//void ParserImpl::parseClass()
//{ }
//
//void ParserImpl::parseEnum()
//{
//
//}
//
//void ParserImpl::parseUnion()
//{
//
//}
//
//void ParserImpl::parseProtocol()
//{ }
//
//void ParserImpl::parseExtension()
//{
//
//}
//
//void ParserImpl::parseNamespace()
//{
//
//}
//
//void DeclParser::ParseDecls()
//{
//   ParserImpl impl(M, Buf, sourceId);
//   return impl.ParseDecls();
//}
//
//} // namespace parse
//} // namespace cdot