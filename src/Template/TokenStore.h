//
// Created by Jonas Zell on 15.11.17.
//

#ifndef CDOT_TOKENSTORE_H
#define CDOT_TOKENSTORE_H

#include <vector>

#include "../Token.h"

namespace cdot {

class TokenStore {
public:
   virtual std::vector<Token> getTokens() = 0;
   virtual std::vector<Token> borrow() = 0;
   virtual void giveBack(std::vector<Token> &&tokens) = 0;
};

class SimpleTokenStore: public TokenStore {
public:
   explicit SimpleTokenStore(std::vector<Token> &&tokens);

   std::vector<Token> getTokens() override;
   std::vector<Token> borrow() override;
   void giveBack(std::vector<Token> &&tokens) override;

protected:
   std::vector<Token> tokens;
};

class TmpFileTokenStore: public TokenStore {
public:
   explicit TmpFileTokenStore(std::vector<Token> &&tokens);

   std::vector<Token> getTokens() override;
   std::vector<Token> borrow() override;
   void giveBack(std::vector<Token> &&tokens) override;

protected:
   unsigned fileHandle;
};

} // namespace cdot

#endif //CDOT_TOKENSTORE_H
