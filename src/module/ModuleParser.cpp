//
// Created by Jonas Zell on 28.11.17.
//

#include "ModuleParser.h"
#include "Module.h"

#include "../lex/Lexer.h"
#include "../AST/SymbolTable.h"

#include <string>


using std::string;
using namespace cdot::lex;

namespace cdot {
namespace module {

class ParserImpl {
public:
   ParserImpl(llvm::MemoryBuffer *Buf, unsigned sourceId)
      : lexer(new Lexer<ModuleLexerTraits>(Buf, sourceId))
   {

   }

   void getMD5(llvm::SmallString<32> &Buf);
   Module *parseModule();

   string parseFileName();
   string parseHash();
   string parseModuleName();
   SymbolTableEntry *parseNextEntry();

private:
   void advance();
   void expect(lex::tok::TokenType ty);
   lex::Token const& currentTok();

   lex::Lexer<ModuleLexerTraits> *lexer;
};

ModuleParser::ModuleParser(llvm::MemoryBuffer *Buf, unsigned sourceId)
   : pImpl(new ParserImpl(Buf, sourceId))
{

}

ModuleParser::~ModuleParser()
{
   delete pImpl;
}

void ModuleParser::getMD5(llvm::SmallString<32> &Buf)
{
   return pImpl->getMD5(Buf);
}

Module* ModuleParser::parseModule()
{
   return pImpl->parseModule();
}

void ParserImpl::advance()
{
   lexer->advance();
}

void ParserImpl::expect(lex::tok::TokenType ty)
{
   lexer->expect(ty);
}

lex::Token const& ParserImpl::currentTok()
{
   return lexer->currentTok();
}

void ParserImpl::getMD5(llvm::SmallString<32> &Buf)
{
   if (!currentTok().is(tok::m_kw_md5))
      return;

   expect(tok::stringliteral);
   Buf += lexer->strRef();
}

Module* ParserImpl::parseModule()
{
   assert(currentTok().oneOf(tok::m_kw_md5, tok::m_kw_file,
                             tok::m_kw_modulename) && "invalid module header");

   string moduleName = parseModuleName();
   advance();

   string fileName = parseFileName();
   advance();

   string md5 = parseHash();
   advance();

   Module *M = Module::get(move(moduleName),
                           move(fileName),
                           move(md5));

   while (!currentTok().is(tok::eof)) {
      M->addEntry(parseNextEntry());
      advance();
   }

   return M;
}

string ParserImpl::parseFileName()
{
   assert(currentTok().is(tok::m_kw_file));
   expect(tok::stringliteral);

   return move(currentTok()._value.strVal);
}

string ParserImpl::parseHash()
{
   assert(currentTok().is(tok::m_kw_md5));
   expect(tok::stringliteral);

   return move(currentTok()._value.strVal);
}

string ParserImpl::parseModuleName()
{
   assert(currentTok().is(tok::m_kw_modulename));
   expect(tok::stringliteral);

   return move(currentTok()._value.strVal);
}

SymbolTableEntry* ParserImpl::parseNextEntry()
{
   return nullptr;
}

} // namespace module
} // namespace cdot