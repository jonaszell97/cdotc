#ifdef CDOT_PARSE_ATTR_MAIN
case AttrKind::Extern: {
ExternAttr::LangKind lang;
unsigned NumNeededArgs = 1;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               case 0: {
   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  "extern ", 5 /*one of */,
                  1, "C, CXX");

      break;
   }

   auto Ident = currentTok().getIdentifier();
ExternAttr::LangKind _enumKind;
if (Ident=="C") { _enumKind = ExternAttr::C; }
else if (Ident=="CXX") { _enumKind = ExternAttr::CXX; }
else if (Ident=="CPP") { _enumKind = ExternAttr::CXX; }
else if (Ident=="C++") { _enumKind = ExternAttr::CXX; }

   else {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  "extern", 5 /*one of */,
                  1, "C, CXX");

      break;
   }
lang = _enumKind;
break;
}

         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;

               }

               advance();
               if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");

                  if (!findTokOnLine(tok::comma, tok::close_paren)) {
                     return skipAttribute();
                  }
               }

               if (currentTok().is(tok::comma)) advance();
            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) ExternAttr(std::move(lang), SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::Inline: {
InlineAttr::LevelKind level;
unsigned NumNeededArgs = 1;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               case 0: {
   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  "inline ", 5 /*one of */,
                  1, "always, never, hint");

      break;
   }

   auto Ident = currentTok().getIdentifier();
InlineAttr::LevelKind _enumKind;
if (Ident=="always") { _enumKind = InlineAttr::always; }
else if (Ident=="never") { _enumKind = InlineAttr::never; }
else if (Ident=="hint") { _enumKind = InlineAttr::hint; }

   else {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  "inline", 5 /*one of */,
                  1, "always, never, hint");

      break;
   }
level = _enumKind;
break;
}

         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;

               }

               advance();
               if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");

                  if (!findTokOnLine(tok::comma, tok::close_paren)) {
                     return skipAttribute();
                  }
               }

               if (currentTok().is(tok::comma)) advance();
            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) InlineAttr(std::move(level), SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::Implicit: {
unsigned NumNeededArgs = 0;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               
         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;

               }

               advance();
               if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");

                  if (!findTokOnLine(tok::comma, tok::close_paren)) {
                     return skipAttribute();
                  }
               }

               if (currentTok().is(tok::comma)) advance();
            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) ImplicitAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::Thin: {
auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) ThinAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::Opaque: {
unsigned NumNeededArgs = 0;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               
         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;

               }

               advance();
               if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");

                  if (!findTokOnLine(tok::comma, tok::close_paren)) {
                     return skipAttribute();
                  }
               }

               if (currentTok().is(tok::comma)) advance();
            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) OpaqueAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::Align: {
ast::StaticExpr* alignment;
unsigned NumNeededArgs = 1;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               case 0: {
   auto exprResult = parseExprSequence();
   if (exprResult.holdsExpr())
alignment = StaticExpr::Create(Context, exprResult.getExpr());
break;
}

         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;

               }

               advance();
               if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");

                  if (!findTokOnLine(tok::comma, tok::close_paren)) {
                     return skipAttribute();
                  }
               }

               if (currentTok().is(tok::comma)) advance();
            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) AlignAttr(std::move(alignment), SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::DiscardableResult: {
unsigned NumNeededArgs = 0;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               
         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;

               }

               advance();
               if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");

                  if (!findTokOnLine(tok::comma, tok::close_paren)) {
                     return skipAttribute();
                  }
               }

               if (currentTok().is(tok::comma)) advance();
            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) DiscardableResultAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::_Builtin: {
unsigned NumNeededArgs = 0;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               
         default:
            SP.diagnose(err_attribute_arg_count, currentTok().getSourceLoc(),
                        Ident, /*at most*/ 1, NumNeededArgs, ArgNo);

            skipUntilEven(tok::open_paren);
            if (!lookahead().is(tok::eof))
               advance();

            return;

               }

               advance();
               if (!currentTok().oneOf(tok::comma, tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");

                  if (!findTokOnLine(tok::comma, tok::close_paren)) {
                     return skipAttribute();
                  }
               }

               if (currentTok().is(tok::comma)) advance();
            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) _BuiltinAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
#endif
#undef CDOT_PARSE_ATTR_MAIN

#ifdef CDOT_PARSE_ATTR_CHECK
auto Decl = Result.getDecl();
switch(A->getKind()) {
case AttrKind::Extern: {if (isa<CallableDecl>(Decl)) break;
if (isa<GlobalVarDecl>(Decl)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "extern");
}
case AttrKind::Inline: {if (isa<CallableDecl>(Decl)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "inline");
}
case AttrKind::Implicit: {if (isa<CallableDecl>(Decl)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "implicit");
}
case AttrKind::Opaque: {if (isa<RecordDecl>(Decl)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "opaque");
}
case AttrKind::Align: {if (isa<RecordDecl>(Decl)) break;
if (isa<VarDecl>(Decl)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "align");
}
case AttrKind::DiscardableResult: {if (isa<CallableDecl>(Decl)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "discardableResult");
}
case AttrKind::_Builtin: {if (isa<NamedDecl>(Decl)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "_builtin");
}
default: break; 
}
#endif
#undef CDOT_PARSE_ATTR_CHECK

