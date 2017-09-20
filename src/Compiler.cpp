//
// Created by Jonas Zell on 03.09.17.
//

#include <fstream>
#include "Compiler.h"
#include "Parser.h"
#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Statement/EndOfFileStmt.h"
#include "Preprocessor.h"

namespace cdot {

   Compiler::Compiler(int argc, char **argv)
   {
      compilerLocation = string(argv[0]);
      for (int i = 1; i < argc; ++i) {
         string arg(argv[i]);

         if (arg[0] != '-') {
            sourceFiles.push_back(arg);
         }
         else if (arg == "-ast-dump") {
            astDump = true;
         }
         else if (arg == "-no-std-lib") {
            linkStdLib = false;
         }
         else {
            throw std::runtime_error("Unknown argument " + arg);
         }
      }

      if (sourceFiles.empty()) {
         throw std::runtime_error("No source file specified");
      }
   }

   void Compiler::compile()
   {
      CompoundStmt::SharedPtr root = std::make_shared<CompoundStmt>();

      if (linkStdLib) {
         for (const auto &_import : util::stdLibImports) {
            string fileName = _import + ".dot";
            string file_path = __FILE__;
            string dir_path = file_path.substr(0, file_path.rfind('/')) + "/StdLib/" + fileName;
            std::ifstream ifs(dir_path);
            string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

            Preprocessor pp(content, fileName);
            pp.run();

            auto Import = Parser(fileName, content).parse();
            root->addStatements(Import->getStatements());
            root->addStatement(std::make_shared<EndOfFileStmt>());
         }
      }

      for (auto& fileName : sourceFiles) {
         std::ifstream t(fileName);
         string src((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

         Preprocessor(src, fileName).run(true);

         Parser parser(fileName, src);
         root->addStatements(parser.parse()->getStatements());
      }

      DeclPass decl;
      decl.doInitialPass(root->getStatements());
      decl.visit(root.get());

      TypeCheckPass::connectTree(root.get());
      TypeCheckPass tc;
      CodeGen cg;

      tc.doInitialPass(root->getStatements());
      tc.visit(root.get());

      {
         ConstExprPass ce;
         ce.visit(root.get());
      }

      if (astDump) {
         root->__dump(0);
         std::cout << "\n\n";
      }

      cg.DeclareClasses(root->getStatements());
      cg.visit(root.get());

      cg.finalize();
   }

}