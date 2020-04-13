////
//// Created by Jonas Zell on 28.11.17.
////
//
//#include "Persist.h"
//
//#include "Token.h"
//#include "../Support/BitstreamWriter.h"
//#include "../Support/BitstreamReader.h"
//
//namespace cdot {
//namespace lex {
//
//class WriterImpl {
//public:
//   WriterImpl(llvm::ArrayRef<Token> tokens,
//              llvm::raw_ostream &out,
//              unsigned sourceId)
//      : tokens(tokens), out(out), sourceId(sourceId), Writer{}
//   {}
//
//   void Write();
//   void WriteToken(const Token &tok);
//
//private:
//   llvm::ArrayRef<Token> tokens;
//   llvm::raw_ostream &out;
//   unsigned sourceId;
//   serial::BitstreamWriter Writer;
//};
//
//void TokenStreamWriter::WriteTo(llvm::raw_ostream &out)
//{
//   WriterImpl impl(tokens, out, sourceId);
//   impl.Write();
//}
//
//void WriterImpl::Write()
//{
//   // write source id once, since it's the same for all tokens
//   Writer.WriteULEB128(sourceId);
//
//   for (const auto &tok : tokens) {
//      WriteToken(tok);
//   }
//
//   if (tokens.empty() || !tokens.back().is(tok::eof)) {
//      WriteToken(Token(tok::eof, {}, SourceLocation(0, sourceId)));
//   }
//
//   Writer.Finalize(out);
//}
//
//void WriterImpl::WriteToken(const Token &tok)
//{
//   auto kind = tok.getKind();
//   Writer.WriteULEB128(kind);
//
//   Writer.WriteULEB128(tok.getSourceLoc().getOffset());
//
//   if (kind == tok::integerliteral) {
//      Writer.WriteULEB128(tok.getValue().intVal);
//   }
//   else if (kind == tok::fpliteral) {
//      union {
//         double d;
//         size_t i;
//      } un { tok.getValue().floatVal };
//
//      Writer.WriteULEB128(un.i);
//   }
//   else if (tok.oneOf(tok::stringliteral, tok::ident)) {
//      auto &str = tok._value.strVal;
//      Writer.WriteULEB128(str.length());
//
//      for (unsigned char c : str) {
//         Writer.WriteByte(c);
//      }
//   }
//}
//
//class ReaderImpl {
//public:
//   ReaderImpl(llvm::MemoryBuffer *Buf, std::vector<Token> &out)
//      : sourceId(0), out(out), Reader(Buf)
//   {}
//
//   void Read();
//   Token ReadToken();
//
//private:
//   unsigned sourceId;
//   std::vector<Token> &out;
//   serial::BitstreamReader Reader;
//};
//
//void TokenStreamReader::ReadInto(std::vector<Token> &in)
//{
//   ReaderImpl impl(Buf, in);
//   impl.Read();
//}
//
//void ReaderImpl::Read()
//{
//   sourceId = (unsigned)Reader.ReadULEB128();
//   for (;;) {
//      out.push_back(ReadToken());
//      if (out.back().is(tok::eof)) {
//         break;
//      }
//   }
//}
//
//Token ReaderImpl::ReadToken()
//{
//   auto kind = (tok::TokenType)Reader.ReadULEB128();
//
//   SourceLocation loc((unsigned)Reader.ReadULEB128(),
//                      sourceId);
//
//   Variant v;
//   if (kind == tok::integerliteral) {
//      v.kind = VariantType::Int;
//      v.intVal = Reader.ReadULEB128();
//   }
//   else if (kind == tok::fpliteral) {
//      union {
//         size_t i;
//         double d;
//      } un { Reader.ReadULEB128() };
//
//      v.kind = VariantType::Floating;
//      v.floatVal = un.d;
//   }
//   else if (kind == tok::stringliteral || kind == tok::ident) {
//      auto len = Reader.ReadULEB128();
//      string s;
//      s.reserve(len);
//
//      for (size_t i = 0; i < len; ++i) {
//         s += Reader.ReadByte();
//      }
//
//      v = Variant(move(s));
//   }
//
//   return Token(kind, std::move(v), loc);
//}
//
//} // namespace lex
//} // namespace cdot