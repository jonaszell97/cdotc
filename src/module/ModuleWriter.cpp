//
// Created by Jonas Zell on 28.11.17.
//

#include <ctime>
#include "ModuleWriter.h"
#include "Module.h"

#include "../AST/Passes/SemanticAnalysis/Record/Record.h"
#include "../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../AST/Passes/SemanticAnalysis/Record/Protocol.h"
#include "../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../AST/Passes/SemanticAnalysis/Record/Union.h"

#include "../AST/Passes/SemanticAnalysis/Function.h"
#include "../Support/WriterBase.h"

using namespace cdot::support;

namespace cdot {
namespace module {

class WriterImpl: public WriterBase<WriterImpl> {
public:
   WriterImpl(Module *M, llvm::raw_ostream &out)
      : WriterBase(out), M(M)
   {
   }

   void DoWrite();
   void WriteEntry(SymbolTableEntry *Entry);

   void WriteRecordCommon(const Record &R);
   void WriteClass(const Class &C);
   void WriteStruct(const Struct &S);
   void WriteEnum(const Enum &E);
   void WriteUnion(const Union &U);
   void WriteProtocol(const Protocol &P);

   void WriteFunction(const Function &F);
   void WriteFunctions(const std::vector<Function*> &Funcs);
   void WriteGlobalVariable(const Variable &V);

   void WriteTypedef(const Typedef &TD);

   void WriteType(const QualType &ty);
   void WriteType(Type *Ty);

   void WriteAccess(AccessModifier AM);
   

private:
   Module *M;
};

void ModuleWriter::WriteTo(llvm::raw_ostream &out)
{
   WriterImpl W(M, out);
   W.DoWrite();
}

void WriterImpl::DoWrite()
{
   out << '\n';
   out << "module_name \"" << M->getName() << "\"\n";
   out << "file_name   \"" << M->getFileName() << "\"\n";
   out << "md5         \"" << M->getMd5() << "\"\n\n";

   for (const auto &E : M->getEntries()) {
      WriteEntry(E);
   }

   std::time_t rawtime;
   std::tm* timeinfo;
   char buffer [80];

   std::time(&rawtime);
   timeinfo = std::localtime(&rawtime);

   std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S",timeinfo);

   out << "\n// created at " << buffer;
}

void WriterImpl::WriteEntry(SymbolTableEntry *Entry)
{
   switch (Entry->getKind()) {
      case SymbolTableEntry::RecordID: {
         auto R = Entry->getRecord();
         switch (R->getTypeID()) {
            case Record::ClassID:
               WriteClass(*cast<Class>(R)); break;
            case Record::StructID:
               WriteStruct(*cast<Struct>(R)); break;
            case Record::EnumID:
               WriteEnum(*cast<Enum>(R)); break;
            case Record::ProtocolID:
               WriteProtocol(*cast<Protocol>(R)); break;
            case Record::UnionID:
               WriteUnion(*cast<Union>(R)); break;
         }

         break;
      }
      case SymbolTableEntry::FunctionID:
         WriteFunctions(Entry->getFunctions()); break;
      case SymbolTableEntry::VariableID:
         WriteGlobalVariable(*Entry->getVariable()); break;
      case SymbolTableEntry::TypedefID:
         WriteTypedef(*Entry->getTypedef()); break;
      default:
         break;
   }
}

void WriterImpl::WriteRecordCommon(const Record &R)
{

}

void WriterImpl::WriteClass(const Class &C)
{

}

void WriterImpl::WriteStruct(const Struct &S)
{

}

void WriterImpl::WriteEnum(const Enum &E)
{

}

void WriterImpl::WriteProtocol(const Protocol &P)
{

}

void WriterImpl::WriteUnion(const Union &U)
{

}

void WriterImpl::WriteFunctions(const std::vector<Function *> &Funcs)
{

}

void WriterImpl::WriteFunction(const Function &F)
{

}

void WriterImpl::WriteTypedef(const Typedef &TD)
{

}

void WriterImpl::WriteGlobalVariable(const Variable &V)
{

}

} // namespace module
} // namespace cdot