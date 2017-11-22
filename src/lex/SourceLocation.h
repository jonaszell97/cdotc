//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_SOURCELOCATION_H
#define CDOT_SOURCELOCATION_H


namespace cdot {

struct SourceLocation {
   SourceLocation();
   SourceLocation(
      unsigned col,
      const unsigned& line,
      unsigned length,
      const unsigned& sourceId
   );

   unsigned col : 16;
   unsigned line : 16;
   unsigned length : 16;
   unsigned sourceId : 16;

   unsigned getLine() const;
   unsigned getCol() const;
   unsigned getLength() const;
   unsigned getSourceId() const;
};

} // namespace cdot


#endif //CDOT_SOURCELOCATION_H
