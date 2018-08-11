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

         
         while (ArgNo < 1) {
            switch (ArgNo) {
            default: llvm_unreachable("no default argument");
case 0: {
level = InlineAttr::InlineAttr::LevelKind::hint;

ArgNo++;
break;
}

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
case AttrKind::AutoClosure: {
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
Attrs.push_back(new (Context) AutoClosureAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::Escaping: {
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
Attrs.push_back(new (Context) EscapingAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::Testable: {
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
Attrs.push_back(new (Context) TestableAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::CompileTime: {
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
Attrs.push_back(new (Context) CompileTimeAttr(SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::VersionDecl: {
VersionDeclAttr::VersionKind version;
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
                  "version ", 5 /*one of */,
                  1, "None, Windows, Darwin, macOS, Linux, POSIX");

      break;
   }

   auto Ident = currentTok().getIdentifier();
VersionDeclAttr::VersionKind _enumKind;
if (Ident=="None") { _enumKind = VersionDeclAttr::None; }
else if (Ident=="Windows") { _enumKind = VersionDeclAttr::Windows; }
else if (Ident=="Darwin") { _enumKind = VersionDeclAttr::Darwin; }
else if (Ident=="macOS") { _enumKind = VersionDeclAttr::macOS; }
else if (Ident=="Linux") { _enumKind = VersionDeclAttr::Linux; }
else if (Ident=="POSIX") { _enumKind = VersionDeclAttr::POSIX; }

   else {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  "version", 5 /*one of */,
                  1, "None, Windows, Darwin, macOS, Linux, POSIX");

      break;
   }
version = _enumKind;
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
Attrs.push_back(new (Context) VersionDeclAttr(std::move(version), SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::VersionStmt: {
VersionStmtAttr::VersionKind version;
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
                  "version ", 5 /*one of */,
                  1, "None, Windows, Darwin, macOS, Linux, POSIX");

      break;
   }

   auto Ident = currentTok().getIdentifier();
VersionStmtAttr::VersionKind _enumKind;
if (Ident=="None") { _enumKind = VersionStmtAttr::None; }
else if (Ident=="Windows") { _enumKind = VersionStmtAttr::Windows; }
else if (Ident=="Darwin") { _enumKind = VersionStmtAttr::Darwin; }
else if (Ident=="macOS") { _enumKind = VersionStmtAttr::macOS; }
else if (Ident=="Linux") { _enumKind = VersionStmtAttr::Linux; }
else if (Ident=="POSIX") { _enumKind = VersionStmtAttr::POSIX; }

   else {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  "version", 5 /*one of */,
                  1, "None, Windows, Darwin, macOS, Linux, POSIX");

      break;
   }
version = _enumKind;
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
Attrs.push_back(new (Context) VersionStmtAttr(std::move(version), SourceRange(AttrLoc, EndLoc)));
 break;}
case AttrKind::_Builtin: {
std::string builtinName;
unsigned NumNeededArgs = 1;

         unsigned ArgNo = 0;

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_paren)) {
               switch (ArgNo++) {
               case 0: {
   if (!currentTok().is(tok::stringliteral)) {
      SP.diagnose(err_attribute_bad_arg, currentTok().getSourceLoc(),
                  "_builtin", 2 /*string literal */,
                  1);

      break;
   }
builtinName = prepareStringLiteral(currentTok());
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

         
         while (ArgNo < 1) {
            switch (ArgNo) {
            default: llvm_unreachable("no default argument");
case 0: {
builtinName = R"__()__";
ArgNo++;
break;
}

            }
         }

         if (ArgNo != NumNeededArgs) {
            SP.diagnose(err_attribute_arg_count, AttrLoc, Ident, /*at least*/ 0,
                        NumNeededArgs, ArgNo);
            break;
         }
      auto EndLoc = currentTok().getSourceLoc();
Attrs.push_back(new (Context) _BuiltinAttr(std::move(builtinName), SourceRange(AttrLoc, EndLoc)));
 break;}
#endif
#undef CDOT_PARSE_ATTR_MAIN

#ifdef CDOT_PARSE_ATTR_CHECK
auto D = Result.getDecl();
switch(A->getKind()) {
case AttrKind::Extern: {if (isa<CallableDecl>(D)) break;
if (isa<GlobalVarDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "extern");
}
case AttrKind::Inline: {if (isa<CallableDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "inline");
}
case AttrKind::Implicit: {if (isa<CallableDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "implicit");
}
case AttrKind::Opaque: {if (isa<RecordDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "opaque");
}
case AttrKind::Align: {if (isa<RecordDecl>(D)) break;
if (isa<VarDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "align");
}
case AttrKind::DiscardableResult: {if (isa<CallableDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "discardableResult");
}
case AttrKind::AutoClosure: {if (isa<FuncArgDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "autoclosure");
}
case AttrKind::Escaping: {if (isa<FuncArgDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "escaping");
}
case AttrKind::Testable: {if (isa<Decl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "testable");
}
case AttrKind::CompileTime: {if (isa<Decl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "compiletime");
}
case AttrKind::VersionDecl: {if (isa<Decl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "version");
}
case AttrKind::_Builtin: {if (isa<NamedDecl>(D)) break;
SP.diagnose(err_attribute_not_valid_here,            A->getSourceRange(), "_builtin");
}
default: break; 
}
#endif
#undef CDOT_PARSE_ATTR_CHECK

