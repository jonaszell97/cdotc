//
// Created by Jonas Zell on 09.10.17.
//

#ifndef CDOT_CALLABLEDECL_H
#define CDOT_CALLABLEDECL_H

#include <llvm/ADT/SmallPtrSet.h>
#include "NamedDecl.h"

#include "../../../Variant/Type/Generic.h"
#include "../../../Basic/Precedence.h"

namespace cdot {

class FunctionType;

namespace ast {

class SemaPass;
class FuncArgDecl;
class StaticExpr;
class CompoundStmt;

class CallableDecl: public NamedDecl, public DeclContext,
                    public llvm::FoldingSetNode {
public:
   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      switch (kind) {
         case FunctionDeclID:
         case MethodDeclID:
         case InitDeclID:
         case DeinitDeclID:
         case EnumCaseDeclID:
            return true;
         default:
            return false;
      }
   }

   static bool classof(DeclContext const* T) { return true; }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, instantiationInfo->specializedTemplate,
              instantiationInfo->templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, CallableDecl *Template,
                       sema::TemplateArgList &list) {
      ID.AddPointer(Template);
      list.Profile(ID);
   }

   friend class TransformImpl;

protected:
   CallableDecl(NodeType typeID,
                AccessModifier am,
                std::string &&name,
                TypeRef* returnType,
                std::vector<FuncArgDecl* > &&args,
                std::vector<StaticExpr* > &&Constraints,
                CompoundStmt* body,
                OperatorInfo op)
      : NamedDecl(typeID, am, move(name), move(Constraints)),
        DeclContext(typeID),
        returnType(returnType), args(move(args)),
        body(body), op(op), Flags(0)
   {

   }

   std::string linkageName;

   FunctionType *functionType = nullptr;

   TypeRef* returnType;
   std::vector<FuncArgDecl* > args;
   CompoundStmt* body;

   std::vector<TemplateParamDecl*> templateParams;
   InstantiationInfo<CallableDecl> *instantiationInfo = nullptr;

   llvm::SmallPtrSet<Type*, 2> thrownTypes;
   size_t methodID = 0;

   OperatorInfo op;

   enum Flag : uint32_t {
      // general flags
      NoThrow  = 1,
      ExternC  = NoThrow << 1,
      Native   = ExternC << 1,
      ConvOp   = Native  << 1,
      External = ConvOp << 1,
      Main     = External << 1,
      Vararg   = Main << 1,
      CVarArg  = Vararg << 1,
      Defined  = CVarArg << 1,

      // method flags
      Abstract         = Defined << 1,
      Alias            = Abstract << 1,
      MutableSelf      = Alias << 1,
      ProtoMethod      = MutableSelf << 1,
      Virtual          = ProtoMethod << 1,
      Override         = Virtual << 1,
      Property         = Override << 1,
      MemberwiseInit   = Property << 1,
      DefaultInit      = MemberwiseInit << 1,
      ProtoDefaultImpl = DefaultInit << 1,
   };

   uint32_t Flags;

   void setFlag(Flag F, bool val)
   {
      if (val)
         Flags |= F;
      else
         Flags &= ~F;
   }

   bool getFlag(Flag F) const
   {
      return (Flags & F) != 0;
   }

public:
   FunctionType *getFunctionType() const
   {
      return functionType;
   }

   void createFunctionType(SemaPass &SP, unsigned flags = 0);

   std::vector<FuncArgDecl* > &getArgs()
   {
      return args;
   }

   std::vector<FuncArgDecl* > const& getArgs() const
   {
      return args;
   }

   void setArgs(std::vector<FuncArgDecl* > &&args)
   {
      CallableDecl::args = move(args);
   }

   void setLinkageName(std::string &&linkageName)
   {
      CallableDecl::linkageName = move(linkageName);
   }

   void setBody(CompoundStmt* body)
   {
      setHasDefinition(body != nullptr);
      this->body = body;
   }

   TypeRef* getReturnType() const
   {
      return returnType;
   }

   void setReturnType(TypeRef* returnType)
   {
      CallableDecl::returnType = returnType;
   }

   const std::string &getLinkageName() const
   {
      return linkageName;
   }

   CompoundStmt* const& getBody() const
   {
      return body;
   }

   size_t getMethodID() const
   {
      return methodID;
   }

   void setMethodID(size_t methodID)
   {
      CallableDecl::methodID = methodID;
   }

   OperatorInfo &getOperator()
   {
      return op;
   }

   OperatorInfo const& getOperator() const
   {
      return op;
   }

   bool isOperator() const
   {
      return op.getPrecedenceGroup().isValid();
   }

   bool isConversionOp() const { return getFlag(ConvOp); }
   void setIsConversionOp(bool conversionOp) { setFlag(ConvOp, conversionOp); }

   bool isMain() const { return getFlag(Main); }
   void setIsMain(bool main) { setFlag(Main, main); }

   bool isExternal() const { return getFlag(External); }
   void setExternal(bool external) { setFlag(External, external); }

   bool isExternC() const { return getFlag(ExternC); }
   void setExternC(bool externC) { setFlag(ExternC, externC); }

   bool isNative() const { return getFlag(Native); }
   void setNative(bool native) { setFlag(Native, native); }

   bool isVararg() const { return getFlag(Vararg); }
   void setVararg(bool vararg) { setFlag(Vararg, vararg); }

   bool isCstyleVararg() const { return getFlag(CVarArg); }
   void setCstyleVararg(bool cstyleVararg) { setFlag(CVarArg, cstyleVararg); }

   uint32_t getFunctionFlags() const { return Flags; }
   void setFunctionFlags(uint32_t Flags) { CallableDecl::Flags = Flags; }

   const llvm::SmallPtrSet<Type*, 2> &getThrownTypes() const
   { return thrownTypes; }

   void addThrownType(Type *ty) { thrownTypes.insert(ty); }

   bool throws(Type *ty) const
   {
      return thrownTypes.find(ty) != thrownTypes.end();
   }

   bool throws() const { return !thrownTypes.empty(); }

   bool isNoThrow() const { return getFlag(NoThrow); }
   void isNoThrow(bool nothrow) { setFlag(NoThrow, nothrow); }

   void setInstantiationInfo(InstantiationInfo<CallableDecl> *instantiationInfo)
   {
      CallableDecl::instantiationInfo = instantiationInfo;
   }

   void setTemplateParams(std::vector<TemplateParamDecl *> &&templateParams)
   {
      CallableDecl::templateParams = move(templateParams);
   }

   llvm::ArrayRef<TemplateParamDecl *> getTemplateParams() const
   {
      return templateParams;
   }

   bool isInstantiation() const
   {
      return instantiationInfo != nullptr;
   }

   const SourceLocation &getInstantiatedFrom() const
   {
      return instantiationInfo->instantiatedFrom;
   }

   CallableDecl* getSpecializedTemplate() const
   {
      return instantiationInfo->specializedTemplate;
   }

   const sema::TemplateArgList &getTemplateArgs() const
   {
      return instantiationInfo->templateArgs;
   }

   llvm::StringRef getNameWithoutFix() const;
};

class TypeRef;
class CompoundStmt;

class FunctionDecl : public CallableDecl {
public:
   FunctionDecl(AccessModifier am,
                std::string &&funcName,
                std::vector<FuncArgDecl* > &&args,
                TypeRef* returnType,
                std::vector<StaticExpr* > &&Constraints,
                CompoundStmt* body,
                OperatorInfo op)
      : CallableDecl(FunctionDeclID, am, move(funcName), returnType,
                     move(args), move(Constraints), body, op)
   {

   }

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == FunctionDeclID;
   }

   static DeclContext *castToDeclContext(FunctionDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<FunctionDecl*>(D));
   }

   static FunctionDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<FunctionDecl*>(const_cast<DeclContext*>(Ctx));
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CALLABLEDECL_H
