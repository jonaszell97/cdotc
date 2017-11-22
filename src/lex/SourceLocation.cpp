//
// Created by Jonas Zell on 16.11.17.
//

#include "SourceLocation.h"

namespace cdot {

SourceLocation::SourceLocation()
   : col(0), line(0), length(0), sourceId(0)
{

}

SourceLocation::SourceLocation(unsigned col,
                               const unsigned &line,
                               unsigned length,
                               const unsigned &sourceId)
   : col(col), line(line), length(length), sourceId(sourceId)
{
}

unsigned SourceLocation::getCol() const
{
   return col;
}

unsigned SourceLocation::getLine() const
{
   return line;
}

unsigned SourceLocation::getLength() const
{
   return length;
}

unsigned SourceLocation::getSourceId() const
{
   return sourceId;
}

} // namespace cdot