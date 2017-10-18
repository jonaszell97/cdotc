//
// Created by Jonas Zell on 10.10.17.
//

#ifndef CDOT_RECORD_H
#define CDOT_RECORD_H

#include <string>
#include <llvm/IR/IRBuilder.h>
#include "../../../../Token.h"

class CodeGen;
enum class AccessModifier : unsigned int;

using std::string;

namespace cdot {
namespace cl {

   class Record {
   public:
      virtual bool isClass()
      {
         return false;
      }

      virtual bool isNonUnion()
      {
         return false;
      }

      virtual bool isStruct()
      {
         return false;
      }

      virtual bool isProtocol()
      {
         return false;
      }

      virtual bool isUnion()
      {
         return false;
      }

      virtual bool isEnum()
      {
         return false;
      }

      virtual bool isRawEnum()
      {
         return false;
      }

      virtual size_t getSize()
      {
         return occupiedBytes;
      }

      virtual short getAlignment()
      {
         return alignment;
      }

      void setAlignment(short align)
      {
         alignment = align;
         manualAlignment = true;
      }

      string& getName()
      {
         return recordName;
      }

      void isDeclared(bool decl)
      {
         is_declared = decl;
      }

      bool isDeclared()
      {
         return is_declared;
      }

      virtual void generateTypeInfo(CodeGen &CGM) {}
      virtual void generateMemoryLayout(CodeGen &CGM) {}
      virtual void generateProtocolMemoryLayout(CodeGen &CGM) {}
      virtual void generateVTables(CodeGen &CGM) {}

      const string& getDeclarationNamespace()
      {
         return declarationNamespace;
      }

      AccessModifier getAccess();
      bool isPrivate();
      bool isProtected();

      string getTypeName();
      int getNameSelector();

      virtual bool conformsTo(const string &protoName)
      {
         return false;
      }

      virtual bool isGeneric()
      {
         return false;
      }

      virtual bool isRefcounted()
      {
         return false;
      }

      size_t getTypeID()
      {
         return typeID;
      }

      void addUse()
      {
         ++uses;
      }

      size_t& getNumUses()
      {
         return uses;
      }

      const SourceLocation& getSourceLoc()
      {
         return loc;
      }

      template<typename T>
      T* getAs()
      {
         return static_cast<T*>(this);
      }

   protected:
      static size_t lastTypeID;

      size_t uses = 0;

      AccessModifier access;
      size_t typeID;

      Record(
         const string &name,
         AccessModifier access,
         const SourceLocation &loc
      );

      string recordName;
      string declarationNamespace;

      size_t occupiedBytes = 0;
      short alignment = 1;
      bool manualAlignment = false;

      bool is_declared = false;

      SourceLocation loc;

   };
}
}

#endif //CDOT_RECORD_H
