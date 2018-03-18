////
//// Created by Jonas Zell on 28.11.17.
////
//
//#include "ModuleParser.h"
//#include "Module.h"
//
//#include "../parse/Parser.h"
//#include "../lex/Lexer.h"
//
//#include "../AST/Passes/Declaration/DeclPass.h"
//#include "../AST/SymbolTable.h"
//
//
//#include "../AST/Statement/Block/CompoundStmt.h"
//#include "../AST/Statement/Declaration/Class/RecordDecl.h"
//#include "../AST/Statement/Declaration/CallableDecl.h"
//#include "../AST/Statement/Declaration/TypedefDecl.h"
//#include "../AST/Passes/Serialization/Serialize.h"
//
//#include <string>
//
//
//using std::string;
//using namespace cdot::lex;
//using namespace cdot::parse;
//using namespace cdot::support;
//
//namespace cdot {
//namespace module {
//
//class ParserImpl {
//public:
//   explicit ParserImpl(Parser &parser)
//      : root(std::make_shared<CompoundStmt>()), parser(parser)
//   {
//
//   }
//
//   void getMD5(llvm::SmallString<32> &Buf);
//   Module *parseModule();
//
//   string parseFileName();
//   string parseHash();
//   std::vector<string> parseModuleName();
//   void parseNextEntry();
//
//   std::vector<string> parseImports();
//
//   Namespace *getOrCreateNamespace(llvm::StringRef name, Namespace *parent)
//   {
//      if (auto ID = SymbolTable::isNamespace(name, {}, {})) {
//         return SymbolTable::getNamespace(ID);
//      }
//
//      auto ID = SymbolTable::declareNamespace(name, false, parent);
//      return SymbolTable::getNamespace(ID);
//   }
//
//private:
//   void advance();
//   void expect(lex::tok::TokenType ty);
//
//   Lexer<> *lex()
//   {
//      return parser.lexer;
//   }
//
//   void parseForwardDecl();
//   Statement* parseTemplate();
//
//   lex::Token const& currentTok();
//
//   CompoundStmt* root;
//   Parser &parser;
//};
//
//ModuleParser::ModuleParser(Parser &parser)
//   : pImpl(new ParserImpl(parser))
//{
//
//}
//
//ModuleParser::~ModuleParser()
//{
//   delete pImpl;
//}
//
//void ModuleParser::getMD5(llvm::SmallString<32> &Buf)
//{
//   return pImpl->getMD5(Buf);
//}
//
//Module* ModuleParser::parseModule()
//{
//   return pImpl->parseModule();
//}
//
//void ParserImpl::advance()
//{
//   parser.advance();
//}
//
//void ParserImpl::expect(lex::tok::TokenType ty)
//{
//   parser.lexer->expect(ty);
//}
//
//lex::Token const& ParserImpl::currentTok()
//{
//   return parser.currentTok();
//}
//
//void ParserImpl::getMD5(llvm::SmallString<32> &Buf)
//{
//   if (!currentTok().is(tok::m_kw_md5))
//      return;
//
//   expect(tok::stringliteral);
//   Buf += parser.lexer->strRef();
//}
//
//Module* ParserImpl::parseModule()
//{
//   auto moduleName = parseModuleName();
//   advance();
//
//   string fileName = parseFileName();
//   advance();
//
//   string md5 = parseHash();
//   advance();
//
//   Module *M = Module::get(move(moduleName),
//                           move(fileName),
//                           move(md5));
//
//   parser.setModule(M);
//
//   while (!currentTok().is(tok::eof)) {
//      parseNextEntry();
//      advance();
//   }
//
//   return M;
//}
//
//string ParserImpl::parseFileName()
//{
//   assert(currentTok().is(tok::m_kw_file));
//   expect(tok::stringliteral);
//
//   return move(currentTok()._value.strVal);
//}
//
//string ParserImpl::parseHash()
//{
//   assert(currentTok().is(tok::m_kw_md5));
//   expect(tok::stringliteral);
//
//   return move(currentTok()._value.strVal);
//}
//
//std::vector<string> ParserImpl::parseModuleName()
//{
//   assert(currentTok().is(tok::m_kw_modulename));
//   expect(tok::ident);
//
//   std::vector<string> moduleName;
//
//   while (1) {
//      moduleName.emplace_back(std::move(lex()->strRef()));
//
//      if (lex()->lookahead().is(tok::period)) {
//         advance();
//         lex()->expect(tok::ident);
//      }
//      else {
//         break;
//      }
//   }
//
//   return moduleName;
//}
//
//Statement* ParserImpl::parseTemplate()
//{
//   lex()->expect(tok::ident);
//   lex()->expect(tok::stringliteral);
//
//   auto buf = llvm::MemoryBuffer::getMemBuffer(lex()->currentTok()._value
//                                                    .getString());
//
//   serial::ASTDeserializer deserializer(*buf);
//   return deserializer.Read();
//}
//
//void ParserImpl::parseForwardDecl()
//{
//   assert(currentTok().is(tok::kw_declare));
//   advance();
//
//   auto kind = currentTok().getKind();
//   lex()->expect(tok::ident);
//
//   switch (kind) {
//      case tok::kw_struct:
//      case tok::kw_class:
//         break;
//      default:
//         llvm_unreachable("bad decl kind");
//   }
//}
//
//void ParserImpl::parseNextEntry()
//{
//   auto next = lex()->lookahead();
//
//   Statement* stmt;
//   if (next.isContextualKeyword("template")) {
//      stmt = parseTemplate();
//   }
//   else {
//      stmt = parser.parseNextStmt();
//   }
//
//   switch (stmt->getTypeID()) {
//      case AstNode::ClassDeclID:
//      case AstNode::EnumDeclID:
//      case AstNode::UnionDeclID:
//      case AstNode::ProtocolDeclID:
//      case AstNode::ExtensionDeclID:
//         parser.decl->visitRecordDecl(cast<RecordDecl>(stmt));
//         break;
//      case AstNode::FunctionDeclID:
//         parser.decl->visitFunctionDecl(cast<FunctionDecl>(stmt));
//         break;
//      case AstNode::TypedefDeclID:
//         parser.decl->visitTypedefDecl(cast<TypedefDecl>(stmt));
//         break;
//      case AstNode::AliasDeclID:
//         parser.decl->visitAliasDecl(cast<AliasDecl>(stmt));
//         break;
//      default:
//         llvm_unreachable("bad decl kind");
//   }
//
//   root->addStatement(move(stmt));
//}
//
//} // namespace module
//} // namespace cdot