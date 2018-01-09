////
//// Created by Jonas Zell on 28.11.17.
////
//
//#ifndef CDOT_DELAYEDPARSELOC_H
//#define CDOT_DELAYEDPARSELOC_H
//
//#include "../lex/SourceLocation.h"
//
//namespace cdot {
//
//namespace cl {
//class Record;
//} // namespace cl
//
//class Function;
//
//namespace parse {
//
//class DelayedParseLoc {
//public:
//   enum Kind {
//      RecordDeclID,
//      FunctionDeclID
//   };
//
//   DelayedParseLoc(cl::Record *rec, const SourceLocation &loc)
//      : kind(RecordDeclID), loc(loc), record(rec)
//   { }
//
//   DelayedParseLoc(Function *func, const SourceLocation &loc)
//      : kind(RecordDeclID), loc(loc), func(func)
//   { }
//
//   Kind getKind() const { return kind; }
//   const SourceLocation &getLoc() const { return loc; }
//
//   cl::Record *getRecord() const { return record; }
//   Function *getFunc() const { return func; }
//
//private:
//   Kind kind;
//   SourceLocation loc;
//
//   union {
//      cl::Record *record;
//      Function *func;
//   };
//};
//
//} // namespace parse
//} // namespace cdot
//
//#endif //CDOT_DELAYEDPARSELOC_H
