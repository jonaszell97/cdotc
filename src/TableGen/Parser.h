//
// Created by Jonas Zell on 01.02.18.
//

#ifndef TABLEGEN_PARSER_H
#define TABLEGEN_PARSER_H

#include "lex/Lexer.h"
#include "Basic/IdentifierInfo.h"
#include "Message/Diagnostics.h"

#include <llvm/Support/Allocator.h>

namespace cdot {
namespace tblgen {

class RecordKeeper;
class Record;
class Class;
class TableGen;
class Value;
class Type;

class Parser {
public:
   Parser(TableGen &TG,
          llvm::MemoryBuffer &Buf,
          unsigned sourceId);

   ~Parser();

   bool parse();

   RecordKeeper &getRK()
   {
      return *GlobalRK;
   }

private:
   TableGen &TG;
   lex::Lexer lex;
   Class *currentClass = nullptr;

   std::unique_ptr<RecordKeeper> GlobalRK;
   RecordKeeper *RK;

   bool encounteredError = false;

   llvm::StringMap<Value*> ForEachVals;

   void parseNextDecl();

   void parseClass();
   void parseTemplateParams(Class *C,
                            llvm::SmallVectorImpl<size_t> &fieldParameters);
   void parseBases(Class *C);
   void parseFieldDecl(Class *C);

   void parseRecord();
   void parseBases(Record *R);
   void parseFieldDef(Record *R);

   void parseValue();
   void parseForEach();
   void parsePrint();
   void parseNamespace();

   llvm::StringRef tryParseIdentifier();

   Type *parseType();
   Value *parseExpr(Type *contextualTy = nullptr);
   Value *parseFunction(Type *contextualTy = nullptr);

   void parseTemplateArgs(std::vector<Value*> &args,
                          llvm::SmallVectorImpl<SourceLocation> &locs,
                          Class *forClass);

   struct NamespaceScope {
      NamespaceScope(Parser &P, RecordKeeper *NewRK)
         : P(P), RK(P.RK)
      {
         P.RK = NewRK;
      }

      ~NamespaceScope()
      {
         P.RK = RK;
      }

   private:
      Parser &P;
      RecordKeeper *RK;
   };

   struct ForEachScope {
      ForEachScope(Parser &P, llvm::StringRef name, Value *V)
         : P(P), name(name)
      {
         P.ForEachVals.try_emplace(name, V);
      }

      ~ForEachScope()
      {
         P.ForEachVals.erase(name);
      }

   private:
      Parser &P;
      llvm::StringRef name;
   };

   Value *getForEachVal(llvm::StringRef name)
   {
      auto it = ForEachVals.find(name);
      if (it != ForEachVals.end())
         return it->getValue();

      return nullptr;
   }

   lex::Token const& currentTok() const
   {
      return lex.currentTok();
   }

   void advance(bool ignoreNewline = true, bool ignoreWhitespace = true)
   {
      return lex.advance(ignoreNewline, !ignoreWhitespace);
   }

   lex::Token peek(bool ignoreNewline = true, bool ignoreWhitespace = true)
   {
      return lex.lookahead(ignoreNewline, !ignoreWhitespace);
   }

   template<class ...Args>
   void expect(Args ...kinds)
   {
      advance();
      if (!currentTok().oneOf(kinds...))
         diag::err(diag::err_generic_error)
            << "unexpected token " + currentTok().toString()
            << lex.getSourceLoc() << diag::term;
   }

   diag::DiagnosticBuilder err(diag::MessageKind err)
   {
      encounteredError |= diag::isError(err);
      return diag::DiagnosticBuilder(err);
   }
};

} // namespace tblgen
} // namespace cdot


#endif //TABLEGEN_PARSER_H
