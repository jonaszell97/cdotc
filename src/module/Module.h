//
// Created by Jonas Zell on 28.11.17.
//

#ifndef CDOT_MODULE2_H
#define CDOT_MODULE2_H

#include <string>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

#include "IdentifierTable.h"
#include "../lex/SourceLocation.h"

namespace cdot {

namespace ast {
   class NamedDecl;
   class CompoundStmt;
} // namespace ast

namespace il {
   class Module;
} // namespace il

namespace module {

class IdentifierTable;

class Module {
public:
   explicit Module(std::string &&name, Module *parentModule = nullptr)
      : parentModule(parentModule), name(move(name)), deserialized(false)
   {

   }

   struct File {
   public:
      File(size_t originalSourceId, std::string &&fileName)
         : originalSourceId(originalSourceId), fileName(move(fileName))
      { }

      size_t getOriginalSourceId() const
      {
         return originalSourceId;
      }

      const std::string &getFileName() const
      {
         return fileName;
      }

      void setSourceId(size_t id) const
      {
         originalSourceId = id;
      }

   private:
      mutable size_t originalSourceId;
      std::string fileName;
   };

   struct Declaration {
      Declaration(unsigned offset)
         : decl(nullptr), deserialized(false), offset(offset)
      {}

      Declaration(ast::NamedDecl *decl)
         : decl(decl), deserialized(true), offset(0)
      {}

      ast::NamedDecl *getDecl() const
      {
         return decl;
      }

      bool isDeserialized() const
      {
         return deserialized;
      }

      unsigned int getOffset() const
      {
         return offset;
      }

      void setDecl(ast::NamedDecl *decl)
      {
         Declaration::decl = decl;
      }

      void setDeserialized(bool deserialized)
      {
         Declaration::deserialized = deserialized;
      }

   private:
      ast::NamedDecl *decl;

      bool deserialized : 1;
      unsigned offset   : 31;
   };

   Module *getParentModule() const { return parentModule; }
   const std::string &getName() const { return name; }
   
   std::string getFullName() const { return getJoinedName('.'); }
   std::string getJoinedName(char seperator) const;

   const std::vector<File> &getContainedFiles() const
   { return containedFiles; }

   ast::CompoundStmt* getDeclRoot() const
   { return declRoot; }

   void addFile(size_t id, std::string &&fileName)
   { containedFiles.emplace_back(id, move(fileName)); }

   void addFile(std::string &&fileName)
   { containedFiles.emplace_back(0, move(fileName)); }

   void setDeclRoot(ast::CompoundStmt* declRoot)
   { Module::declRoot = declRoot; }

   const std::string &getBaseFile() const { return baseFile; }

   void setBaseFile(std::string &&baseFile)
   { Module::baseFile = move(baseFile); }

   const std::string &getModuleMapFile() const { return moduleMapFile; }

   void setModuleMapFile(std::string &&moduleMapFile);

   il::Module *getILModule() const { return ILModule; }
   void setILModule(il::Module *ILModule) { Module::ILModule = ILModule; }

   const std::vector<std::unique_ptr<Module>> &getSubModules() const
   { return SubModules; }

   long long int getTimestamp() const { return timestamp; }

   void setTimestamp(long long int timestamp)
   {
      Module::timestamp = timestamp;
   }

   SourceLocation getSourceLoc() const { return sourceLoc; }

   void setSourceLoc(SourceLocation sourceLoc)
   {
      Module::sourceLoc = sourceLoc;
   }

   Module *getSubModule(llvm::StringRef name) const
   {
      for (auto &M : SubModules)
         if (name.equals(M->getName()))
            return M.get();

      return nullptr;
   }

   Module *getBaseModule()
   {
      auto base = this;
      while (auto parent = base->getParentModule())
         base = parent;

      return base;
   }

   void addSubModule(std::unique_ptr<Module> &&Sub)
   {
      SubModules.emplace_back(move(Sub));
   }

   void addBuildOption(std::string &&opt)
   {
      buildOptions.emplace_back(move(opt));
   }

   const std::vector<std::string> &getBuildOptions() const
   { return buildOptions; }

   void setBuildOptions(std::vector<std::string> &&buildOptions)
   {
      Module::buildOptions = move(buildOptions);
   }

   void setContainedFiles(std::vector<File> &&files)
   {
      containedFiles = move(files);
   }

   bool isDeserialized() const
   {
      return deserialized;
   }

   void setDeserialized(bool deserialized)
   {
      Module::deserialized = deserialized;
   }

   const llvm::StringMap<Declaration> &getDeclarations() const
   {
      return declarations;
   }

   void addDecl(llvm::StringRef key, unsigned offset)
   {
      declarations.try_emplace(key, offset);
   }

   void addDecl(llvm::StringRef key, ast::NamedDecl *decl)
   {
      declarations.try_emplace(key, decl);
   }

   void addDecl(ast::NamedDecl *decl);

   Declaration *getDecl(llvm::StringRef key)
   {
      auto it = declarations.find(key);
      if (it == declarations.end())
         return nullptr;

      return &it->second;
   }

   IdentifierTable &getIdentifierTable()
   {
      assert(identifierTable != nullptr);
      return *identifierTable;
   }

   void setIdentifierTable(std::unique_ptr<IdentifierTable> &&table)
   {
      identifierTable = move(table);
   }

private:
   Module *parentModule = nullptr;
   std::vector<std::unique_ptr<Module>> SubModules;

   std::string name;
   std::vector<File> containedFiles;

   std::string moduleMapFile;
   std::string baseFile;

   std::unique_ptr<IdentifierTable> identifierTable;
   llvm::StringMap<Declaration> declarations;
   ast::CompoundStmt* declRoot = nullptr;
   il::Module *ILModule = nullptr;

   SourceLocation sourceLoc;
   long long timestamp = -1;
   std::vector<std::string> buildOptions;

   bool deserialized : 1;
};

} // namespace module
} // namespace cdot

#endif //CDOT_MODULE2_H
