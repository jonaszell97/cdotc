//
// Created by Jonas Zell on 15.11.17.
//

#include "TokenStore.h"

#include <llvm/Support/ErrorHandling.h>

using cdot::lex::Token;

namespace cdot {

SimpleTokenStore::SimpleTokenStore(std::vector<Token> &&tokens)
   : tokens(move(tokens))
{

}

std::vector<Token> SimpleTokenStore::getTokens()
{
   return tokens;
}

std::vector<Token> SimpleTokenStore::borrow()
{
   return move(tokens);
}

void SimpleTokenStore::giveBack(std::vector<Token> &&tokens)
{
   this->tokens = move(tokens);
}

TmpFileTokenStore::TmpFileTokenStore(std::vector<Token> &&tokens)
{

}

std::vector<Token> TmpFileTokenStore::getTokens()
{
   llvm_unreachable("not implemented");
}

std::vector<Token> TmpFileTokenStore::borrow()
{
   llvm_unreachable("not implemented");
}

void TmpFileTokenStore::giveBack(std::vector<Token> &&tokens)
{
   llvm_unreachable("not implemented");
}

} // namespace cdot