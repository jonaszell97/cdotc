//
// Created by Jonas Zell on 28.11.17.
//

#ifndef CDOT_MODULEPARSER_H
#define CDOT_MODULEPARSER_H

namespace llvm {

class MemoryBuffer;

template<unsigned T>
class SmallString;

} // namespace llvm

namespace cdot {
namespace lex {

template<class T>
class Lexer;

struct Token;

namespace tok {
   enum TokenType: unsigned short;
} // namespace tok

} // namespace lex


namespace parse {
   class Parser;
} // namespace Parse

namespace module {

struct ModuleLexerTraits;
class Module;
class ParserImpl;

class ModuleParser {
public:
   explicit ModuleParser(parse::Parser &parser);

   ~ModuleParser();

   void getMD5(llvm::SmallString<32> &Buf);
   Module *parseModule();

private:
   ParserImpl *pImpl;
};

} // namespace module
} // namespace cdot


#endif //CDOT_MODULEPARSER_H
