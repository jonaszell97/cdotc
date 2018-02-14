//
// Created by Jonas Zell on 13.01.18.
//

#include "ModuleMap.h"
#include "Module.h"

#include "../lex/Lexer.h"
#include "../Message/Diagnostics.h"
#include "../Files/FileUtils.h"

#include "../Support/Casting.h"
#include "../Support/WriterBase.h"
#include "../Support/SerializerBase.h"
#include "../Files/FileManager.h"
#include "../Basic/IdentifierInfo.h"

#include <chrono>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>

using std::string;
using namespace cdot::lex;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace module {
namespace {

class ModuleMapSerializer: public SerializerBase<ModuleMapSerializer> {
public:
   explicit ModuleMapSerializer(Module &M)
   {
      serialize(&M);
   }

private:
   void serialize(Module *M)
   {
      WriteString(M->getName());
      WriteString(M->getModuleMapFile());

      WriteList(M->getContainedFiles(), &ModuleMapSerializer::WriteFile);
      WriteList(M->getBuildOptions(), &ModuleMapSerializer::WriteString);

      Writer.WriteSLEB128(M->getTimestamp());

      WriteList(M->getSubModules(), &ModuleMapSerializer::serializeUniquePtr);
   }

   void WriteFile(Module::File const& F)
   {
      Writer.WriteULEB128(F.getOriginalSourceId());
      WriteString(F.getFileName());
   }

   void serializeUniquePtr(std::unique_ptr<Module> const& M)
   {
      serialize(M.get());
   }
};

class ModuleMapDeserializer: public DeserializerBase<ModuleMapDeserializer> {
public:
   explicit ModuleMapDeserializer(llvm::MemoryBuffer *Buf)
      : DeserializerBase(Buf)
   { }

   std::unique_ptr<Module> deserialize(Module *parentModule = nullptr)
   {
      auto name = ReadString();
      auto mmap = ReadString();

      auto files = ReadList<Module::File>(&ModuleMapDeserializer::ReadFile);
      auto opts = ReadList<string>(&ModuleMapDeserializer::ReadString);

      auto timestamp = Reader.ReadSLEB128();

      auto M = std::make_unique<Module>(move(name), parentModule);

      M->setModuleMapFile(move(mmap));
      M->setTimestamp(timestamp);
      M->setBuildOptions(move(opts));
      M->setContainedFiles(move(files));

      auto numSubModules = Reader.ReadULEB128();
      for (size_t i = 0; i < numSubModules; ++i) {
         auto Sub = deserialize(M.get());
         M->addSubModule(move(Sub));
      }

      return M;
   }

   Module::File ReadFile()
   {
      auto id = Reader.ReadULEB128();
      auto file = ReadString();

      return { id, move(file) };
   }
};

class ModuleParser {
public:
   static Module *getModule(llvm::StringRef moduleMapFile)
   {
      llvm::SmallString<128> ScratchBuf;
      ScratchBuf += moduleMapFile;
      ScratchBuf += ".lock";

      // check module cache

      auto mapLastModified = fs::getLastModifiedTime(moduleMapFile);
      auto it = ModuleCache.find(moduleMapFile);
      if (it != ModuleCache.end() && it->second.timestamp > mapLastModified)
         return it->second.M.get();

      // if a serialized version of the module file exists, and the module map
      // was not modified, use that one instead

      auto serialLastModified = fs::getLastModifiedTime(ScratchBuf.str());
      if (serialLastModified > mapLastModified) {
         auto Buf = llvm::MemoryBuffer::getFile(ScratchBuf.str());
         auto M = ModuleMapDeserializer(Buf.get().get()).deserialize();

         return emplace(moduleMapFile, move(M));
      }

      auto file = fs::FileManager::openFile(moduleMapFile, true);
      return ModuleParser(*file.second, moduleMapFile, file.first)
         .DoParse();
   }

private:
   struct CachedModule {
      std::unique_ptr<Module> M;
      long timestamp;
   };

   static llvm::StringMap<CachedModule> ModuleCache;

   static Module *emplace(llvm::StringRef key, std::unique_ptr<Module> &&M)
   {
      auto ptr = M.get();
      auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
         std::chrono::system_clock::now().time_since_epoch()).count();

      ModuleCache[key] = CachedModule{ move(M), now };
      return ptr;
   }

   llvm::StringRef moduleMapFile;
   cdot::IdentifierTable Idents;
   Lexer lexer;

   explicit ModuleParser(llvm::MemoryBuffer &Buf,
                         llvm::StringRef moduleMapFile,
                         size_t sourceId)
      : moduleMapFile(moduleMapFile), Idents(16), lexer(Idents, &Buf, sourceId)
   {
      lexer.lex();
   }

   Module *DoParse()
   {
      while (!currentTok().is(tok::kw_module)) {
         lexer.advance();
      }

      auto M = parseModule(nullptr);
      auto ptr = M.get();
      emplace(moduleMapFile, move(M));

      return ptr;
   }

   void advance(bool ignore_newline = true, bool sw = false)
   { lexer.advance(ignore_newline, sw); }

   Token lookahead() { return lexer.lookahead(); }
   Token const& currentTok() { return lexer.currentTok(); }

   std::unique_ptr<Module> parseModule(Module *parentModule)
   {
      lexer.expect(tok::ident);

      auto loc = currentTok().getSourceLoc();
      auto moduleName = currentTok().getIdentifierInfo()->getIdentifier();
      auto M = std::make_unique<Module>(moduleName.str(), parentModule);

      M->setSourceLoc(loc);
      M->setModuleMapFile(moduleMapFile.str());

      lexer.expect(tok::open_brace);
      advance();

      while (!currentTok().is(tok::close_brace)) {
         switch (currentTok().getKind()) {
            case tok::kw_module: {
               auto Sub = parseModule(M.get());
               M->addSubModule(move(Sub));

               break;
            }
            case tok::kw_source_files:
               parseSourceFiles(M.get());
               break;
            case tok::kw_timestamp: {
               lexer.expect(tok::integerliteral);
               M->setTimestamp(std::stoll(currentTok().getText()));

               break;
            }
            case tok::kw_build_options:
               parseBuildOptions(M.get());
               break;
            default:
               diag::err(err_generic_error)
                  << "unexpected token " + currentTok().toString()
                  << lexer.getSourceLoc() << diag::term;
         }

         advance();
      }

      return M;
   }

   void parseSourceFiles(Module *M)
   {
      lexer.expect(tok::open_square);
      advance();

      while (!currentTok().is(tok::close_square)) {
         if (!currentTok().is(tok::stringliteral)) {
            diag::err(err_generic_error)
               << "expected string literal"
               << lexer.getSourceLoc() << diag::cont;

            return;
         }

         llvm::SmallString<128> file;
         file += currentTok().getIdentifierInfo()->getIdentifier();

         // error will be handled later
         llvm::sys::fs::make_absolute(fs::getPath(moduleMapFile), file);

         llvm::SmallVector<string, 4> allFiles;
         fs::getAllMatchingFiles(file.str(), allFiles);

         for (auto &f : allFiles)
            M->addFile(move(f));

         advance();

         if (currentTok().is(tok::comma))
            advance();
      }
   }

   void parseBuildOptions(Module *M)
   {
      lexer.expect(tok::open_square);
      advance();

      while (!currentTok().is(tok::close_square)) {
         if (!currentTok().is(tok::stringliteral)) {
            diag::err(err_generic_error)
               << "expected string literal"
               << lexer.getSourceLoc() << diag::cont;

            return;
         }

         M->addBuildOption(lexer.currentTok().getText());

         advance();
         if (currentTok().is(tok::comma))
            advance();
      }
   }
};

llvm::StringMap<ModuleParser::CachedModule> ModuleParser::ModuleCache;

} // anonymous namespace

Module *tryParseModuleMap(llvm::StringRef moduleMapFile)
{
   return ModuleParser::getModule(moduleMapFile);
}

void serializeModuleMap(Module *M)
{
   while (auto Outer = M->getParentModule())
      M = Outer;

   llvm::SmallString<128> ScratchBuf;
   ScratchBuf += M->getModuleMapFile();
   ScratchBuf += ".lock";

   std::error_code ec;
   llvm::raw_fd_ostream file(ScratchBuf.str(), ec, llvm::sys::fs::F_RW);

   ModuleMapSerializer(*M).finalize(file);
}

} // namespace module
} // namespace cdot