//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_SOURCELOCATION_H
#define CDOT_SOURCELOCATION_H


namespace cdot {

struct SourceLocation {
   SourceLocation() : offset(0), sourceId(0) {}
   SourceLocation(unsigned offset,
                  unsigned sourceId)
      : offset(offset), sourceId(sourceId)
   {}

   unsigned offset : 32;
   unsigned sourceId : 32;

   unsigned getOffset() const { return offset; }
   unsigned getSourceId() const { return sourceId; }

   operator bool() const { return sourceId != 0; }
};

} // namespace cdot


#endif //CDOT_SOURCELOCATION_H
