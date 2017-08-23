//
// Created by Jonas Zell on 09.07.17.
//

#include "Class.h"
#include "../../SymbolTable.h"
#include "../../Expression/Expression.h"
#include "../../../Message/Warning.h"
#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Util.h"
#include "../../../Variant/Type/GenericType.h"

namespace cdot {
namespace cl {

    /**
     * Instantiates a class field
     * @param name
     * @param type
     * @param access_modifier
     */
    Field::Field(string name, Type* type, AccessModifier access_modifier, Expression::SharedPtr def) :
        fieldName(name), fieldType(type), accessModifier(access_modifier), defaultVal(def)
    {

    }

    /**
     * Instantiates a class method
     * @param name
     * @param ret_type
     * @param access_modifier
     * @param arg_names
     * @param arg_types
     * @param arg_defaults
     */
    Method::Method(string name, Type* ret_type, AccessModifier access_modifier, std::vector<string>
        arg_names, std::vector<Type*> arg_types, std::vector<Expression::SharedPtr> arg_defaults,
        std::vector<GenericType*>& generics, bool isStatic) :
            methodName(name), returnType(ret_type), accessModifier(access_modifier),
            argumentDefaults(arg_defaults), argumentNames(arg_names), argumentTypes(arg_types), isStatic(isStatic),
            generics(generics)
    {

    }

    Method::Method(string name, Type *ret_type, std::vector<Type *> argTypes, std::vector<GenericType*>& generics) :
        methodName(name),
        returnType(ret_type),
        argumentTypes(argTypes),
        isStatic(false),
        generics(generics)
    {

    }

    /**
     * Instantiates a class
     * @param class_name
     * @param parent
     * @param implements
     */
    Class::Class(string& class_name, ObjectType* parent, std::vector<ObjectType*>& conformsTo_,
        std::vector<GenericType*>& generics, ClassDecl* decl, bool is_abstract) :
        className(class_name),
        extends(parent),
        declaration(decl),
        conformsTo_(conformsTo_),
        is_abstract(is_abstract),
        generics(generics)
    {
        type = ObjectType::get(class_name);
        for (const auto& gen : generics) {
            type->specifyGenericType(gen->getGenericClassName(), gen->getCovariance());
        }
    }

    Class::Class(string &class_name, std::vector<ObjectType *> &conformsTo_, std::vector<GenericType *> &generics,
            bool isProtocol, ClassDecl *decl) :
        className(class_name),
        extends(ObjectType::get("Any")),
        declaration(decl),
        is_struct(!isProtocol),
        is_protocol(isProtocol),
        conformsTo_(conformsTo_),
        generics(generics)
    {
        type = ObjectType::get(class_name);
        type->isStruct(is_struct);
        parentClass = SymbolTable::getClass("Any");
        for (const auto& gen : generics) {
            type->specifyGenericType(gen->getGenericClassName(), gen->getCovariance());
        }
    }

    /**
     * Adds a field to a class
     * @param name
     * @param type
     * @param access
     * @param is_static
     */
    Field* Class::declareField(
        string name,
        Type *type,
        AccessModifier access,
        Expression::SharedPtr def_val
    ) {
        auto field = std::make_shared<Field>(name, type, access, def_val);
        auto ptr = field.get();

        field->mangledName = "." + className + "." + name;
        fields.emplace_back(name, std::move(field));

        return ptr;
    }

    void Class::defineParentClass() {
        if (extends != nullptr) {
            parentClass = SymbolTable::getClass(extends->getClassName());
            depth = parentClass->depth + 1;

            auto current = parentClass;
            while (current != nullptr) {
                current->extendedBy.push_back(this);
                current = current->parentClass;
            }

            std::vector<ObjectType*> conformsToExcl;
            for (const auto& prot : conformsTo_) {
                if (!parentClass->conformsTo(prot->getClassName())) {
                    conformsToExcl.push_back(prot);
                }
            }

            conformsTo_ = conformsToExcl;
        }
    }

    /**
     * Adds a method to a class
     * @param name
     * @param ret_type
     * @param access
     * @param is_static
     * @param arg_names
     * @param arg_types
     * @param arg_defaults
     */
    Method* Class::declareMethod(
        string name,
        Type *ret_type,
        AccessModifier access,
        std::vector<string> arg_names,
        std::vector<Type *> arg_types,
        std::vector<Expression::SharedPtr> arg_defaults,
        std::vector<GenericType *> generics,
        bool isStatic
    ) {
        auto overloads = methods.equal_range(name);
        auto score = util::func_score(arg_types);

        for (auto it = overloads.first; it != overloads.second; ++it) {
            auto& overload = it->second;
//            auto res = util::func_call_compatible(overload->argument_types, arg_types);
//            if (res.is_compatible && res.compat_score >= score) {
//                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot redeclare method " + name + " with a similar name to a "
//                    "previous declaration", caller);
//            }
        }

        if (name == "init") {
            generics = this->generics;
        }

        auto symb = SymbolTable::mangleMethod(className, name, arg_types);
        auto method = std::make_shared<Method>(name, ret_type, access, arg_names,
            arg_types, arg_defaults, generics, isStatic);

        method->mangledName = symb;

        auto ptr = method.get();

        methods.emplace(name, std::move(method));

        if (name != "init" && name != "init.def") {
            auto mangledName = SymbolTable::mangleFunction(name, arg_types, "");
            mangledMethods.emplace(mangledName, ptr);
        }
        else {
            constructors.push_back(ptr);
        }

        return ptr;
    }

    Method* Class::declareMemberwiseInitializer() {
        std::vector<Type*> argTypes;

        for (const auto& field : fields) {
            argTypes.push_back(field.second->fieldType->deepCopy());
        }

        string constrName = "init";
        auto mangled = SymbolTable::mangleMethod(className, constrName, argTypes);
        auto method = std::make_shared<Method>(constrName, type->toRvalue(), argTypes, generics);

        method->mangledName = mangled;
        auto ptr = method.get();

        methods.emplace("init", std::move(method));
        constructors.push_back(ptr);

        memberwiseInitializer = ptr;
        return ptr;
    }

    /**
     * Returns whether or not a class or its base class has a field
     * @param field_name
     * @param is_static
     * @return
     */
    bool Class::hasField(string &field_name) {
        for (const auto& f : fields) {
            if (f.first == field_name) {
                return true;
            }
        }

        if (parentClass != nullptr) {
            return parentClass->hasField(field_name);
        }

        return false;
    }

    /**
     * Returns whether or not a class or its base class has a method
     * @param method_name
     * @param is_static
     * @return
     */
    MethodResult Class::hasMethod(
        string method_name,
        std::vector<Type *> args,
        std::vector<Type *> &concrete_generics,
        bool check_parent,
        bool checkProtocols,
        bool strict
    ) {
        auto overloads = methods.equal_range(method_name);
        MethodResult result;
        int bestMatch = 0;

        if (overloads.first == overloads.second) {
            goto check_parent;
        }

        result.compatibility = CompatibilityType::NO_MATCHING_CALL;

        for (auto it = overloads.first; it != overloads.second; ++it) {
            auto& overload = it->second;
            std::vector<Type*> givenArgs;
            std::vector<Type*>& neededArgs = overload->argumentTypes;

            givenArgs.reserve(args.size());

            size_t i = 0;
            for (const auto& arg : args) {
                if (neededArgs.size() <= i || (!neededArgs.at(i)->isReference() && arg->isLvalue()
                    && !arg->isCStyleArray()))
                {
                    givenArgs.push_back(arg->toRvalue());
                }
                else {
                    givenArgs.push_back(arg);
                }
            }

            auto res = util::func_call_compatible(givenArgs, neededArgs, concrete_generics, overload->generics);

            if (res.perfect_match) {
                result.compatibility = CompatibilityType::COMPATIBLE;
                result.method = overload.get();
                result.neededCasts = res.needed_casts;

                return result;
            }

            if (res.is_compatible && res.compat_score >= bestMatch && !strict) {
                result.compatibility = CompatibilityType::COMPATIBLE;
                result.method = overload.get();
                result.neededCasts = res.needed_casts;

                bestMatch = res.compat_score;
            }
        }

        check_parent:
        if (result.compatibility != CompatibilityType::COMPATIBLE && parentClass != nullptr && check_parent
            && method_name != "init")
        {
            auto parentRes = parentClass->hasMethod(method_name, args, concrete_generics,
                check_parent, checkProtocols, strict);
            if (parentRes.compatibility == CompatibilityType::COMPATIBLE) {
                return parentRes;
            }
        }

        if (result.compatibility != CompatibilityType::COMPATIBLE && checkProtocols) {
            for (const auto& prot : conformsTo_) {
                auto res = SymbolTable::getClass(prot->getClassName())->hasMethod(method_name, args, concrete_generics,
                    check_parent, checkProtocols, strict);

                if (res.compatibility == CompatibilityType::COMPATIBLE) {
                    return res;
                }
            }
        }

        return result;
    }

    MethodResult Class::hasMethod(
        string method_name,
        std::vector<Type *> args,
        bool check_parent,
        bool checkProtocols,
        bool strict
    ) {
        std::vector<Type*> concreteGenerics;
        return hasMethod(method_name, args, concreteGenerics, check_parent, checkProtocols, strict);
    }

    MethodResult Class::hasMethod(
        string method_name,
        std::vector<Type *> args,
        std::unordered_map<string, Type*>& unorderedGenerics,
        bool check_parent,
        bool checkProtocols,
        bool strict
    ) {
        std::vector<Type*> concreteGenerics;
        concreteGenerics.reserve(generics.size());

        for (const auto& gen : generics) {
            if (unorderedGenerics.find(gen->getGenericClassName()) != unorderedGenerics.end()) {
                concreteGenerics.push_back(unorderedGenerics[gen->getGenericClassName()]->deepCopy());
            }
            else {
                return MethodResult();
            }
        }

        return hasMethod(method_name, args, concreteGenerics, check_parent, checkProtocols, strict);
    }

    MethodResult Class::ancestorHasMethod(string &name, std::vector<Type *> &args) {
        if (parentClass != nullptr) {
            MethodResult method;
            if ((method = parentClass->hasMethod(name, args)).compatibility == CompatibilityType::COMPATIBLE) {
                return method;
            }
        }

        for (const auto& impl : conformsTo_) {
            Class* interface = SymbolTable::getClass(impl->getClassName());
            MethodResult method;
            if ((method = parentClass->hasMethod(name, args)).compatibility == CompatibilityType::COMPATIBLE) {
                return method;
            }
        }

        return MethodResult();
    }

    /**
     * Returns a method, if it exists
     * @param method_name
     * @param is_static
     * @return
     */
    Method* Class::getMethod(string method_name) {

        for (auto& method : methods) {
            if (method.second->mangledName == method_name) {
                return method.second.get();
            }
        }

        if (parentClass != nullptr) {
            return parentClass->getMethod(method_name);
        }

        return nullptr;
    }

    /**
     * Returns a field, if it exists
     * @param field_name
     * @param is_static
     * @return
     */
    Field* Class::getField(string &field_name) {
        for (const auto& f : fields) {
            if (f.first == field_name) {
                return f.second.get();
            }
        }

        if (parentClass != nullptr) {
            return parentClass->getField(field_name);
        }

        return nullptr;
    }

    ObjectType* Class::getType() {
        return cast<ObjectType>(type->deepCopy());
    }

    bool Class::conformsTo(string &name) {
        for (const auto& proto : conformsTo_) {
            if (proto->getClassName() == name) {
                return true;
            }
            if (SymbolTable::getClass(proto->getClassName())->conformsTo(name)) {
                return true;
            }
        }

        if (parentClass != nullptr && parentClass->conformsTo(name)) {
            return true;
        }

        return false;
    }

    namespace {

        void inheritProtocols(std::vector<ObjectType*>& protocols, ObjectType* current, bool initial = true) {
            if (!initial) {
                protocols.push_back(current);
            }

            auto prot = SymbolTable::getClass(current->getClassName());
            for (const auto& proto : prot->getConformedToProtocols()) {
                inheritProtocols(protocols, proto, false);
            }
        }

    }

    /**
     * Checks a class for correct implementation of all interfaces
     * @return An error, if it occurs
     */
    void Class::finalize() {
        if (finalized) {
            return;
        }
        
        if (is_protocol) {

            for (const auto& field : fields) {
                if (!field.second->hasGetter && !field.second->hasSetter) {
                    throw "Protocol property " + field.first +  " has to require either a getter or a setter";
                }
            }

            findVirtualMethods();
            finalized = true;

            return;
        }

        if (parentClass != nullptr) {
            parentClass->finalize();
        }

        for (const auto& proto : conformsTo_) {
            inheritProtocols(conformsTo_, proto);
        }

        for (const auto& interf : conformsTo_) {

            auto& interfaceName = interf->getClassName();
            Class* interface = SymbolTable::getClass(interfaceName);

            for (auto& field : interface->fields) {
                if (!hasField(field.first)) {
                    throw "Class " + className + " does not correctly implement interface " + interfaceName +
                        ": Required field " + field.first + " is missing";
                }

                for (const auto& f : fields) {
                    if (f.first == field.first) {
                        if (field.second->hasGetter && !f.second->hasGetter) {
                            throw "Protocol " + interfaceName + " requires member " + field.first + " to define a "
                                "getter method";
                        }
                        if (field.second->hasSetter && !f.second->hasSetter) {
                            throw "Protocol " + interfaceName + " requires member " + field.first + " to define a "
                                "setter method";
                        }

                        field.second->isInheritedField = true;
                        break;
                    }
                }
            }

            protocolMethods.emplace(interfaceName, std::vector<string>());
            auto& int_methods = protocolMethods[interfaceName];
            auto& concreteGenerics = interf->getConcreteGenericTypes();
            concreteGenerics.emplace("Self", type);

            int i = 0;
            for (const auto& method : interface->methods) {
                std::vector<Type*> argTypes;
                argTypes.reserve(method.second->argumentTypes.size());

                for (const auto& arg : method.second->argumentTypes) {
                    auto needed = arg->deepCopy();
                    Type::resolveGeneric(&needed, concreteGenerics);

                    argTypes.push_back(needed);
                }

                auto methodRes = hasMethod(method.second->methodName, argTypes, true, false, false);
                if (methodRes.compatibility != CompatibilityType::COMPATIBLE) {
                    throw "Class " + className + " does not correctly implement interface " + interfaceName +
                        ": Required method " + method.second->methodName + " is missing or has incompatible signature";
                }

                auto& given = methodRes.method->returnType;
                auto needed = method.second->returnType;

                Type::resolveGeneric(&needed, concreteGenerics);

                if (!given->implicitlyCastableTo(needed)) {
                    throw "Class " + className + " does not correctly implement interface " + interfaceName +
                        ": Required method " + method.second->methodName + " has incompatible return type (Expected " +
                        needed->toString() + ", found " + given->toString() + ")";
                }

                int_methods.push_back(methodRes.method->mangledName);

                for (const auto& m : mangledMethods) {
                    if (m.second->methodName == method.second->methodName) {
                        methodRes.method->isProtocolMethod = true;
                        methodRes.method->protocolName = interfaceName;
                    }
                }

                ++i;
            }
        }

        for (auto& field : fields) {
            field.second->llvmType = field.second->fieldType->getLlvmType();

            if (parentClass != nullptr && parentClass->hasField(field.first)) {
                field.second->isInheritedField = true;
            }
        }

        findVirtualMethods();

        finalized = true;
    }

    /**
     * Returns whether or not a protected property of this class can be accessed from within the given class
     * @param class_context
     * @return
     */
    bool Class::protectedPropAccessibleFrom(string &class_context) {
        if (class_context == className) {
            return true;
        }

        for (auto child : extendedBy) {
            if (child->protectedPropAccessibleFrom(class_context)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Returns whether or not a protected property of this class can be accessed from within the given class
     * @param class_context
     * @return
     */
    bool Class::privatePropAccessibleFrom(string &class_context) {
        return class_context == className;
    }

    /**
     * Returns whether or not this class extends the given class
     * @return
     */
    bool Class::is_base_class_of(string& child) {
        auto base = SymbolTable::getClass(child);
        auto current = base;
        while (current != nullptr) {
            if (this == current) {
                return true;
            }
            current = current->parentClass;
        }

        return false;
    }

    void Class::findVirtualMethods() {
        if (is_abstract) {
            for (const auto& method : mangledMethods) {
                virtualMethods.emplace_back(method.first, method.second->mangledName);
            }

            return;
        }

        Class* base = this;
        int depth = 0;
        while (base->parentClass != nullptr) {
            base = base->parentClass;
            ++depth;
        }

        while (base != this) {
            for (const auto &method : base->mangledMethods) {
                auto current = this;
                while (current != nullptr) {
                    if (current == base) {
                        break;
                    }

                    if (current->mangledMethods.find(method.first) != current->mangledMethods.end()) {
                        if (!util::in_pair_vector(virtualMethods, method.first)) {
                            virtualMethods.emplace_back(method.first,
                                current->mangledMethods[method.first]->mangledName);
                        }
                        if (!util::in_pair_vector(base->virtualMethods, method.first)) {
                            base->virtualMethods.emplace_back(method.first,
                                base->mangledMethods[method.first]->mangledName);
                        }

                        // place it in the protocol vtable as well as the normal one
                        if (method.second->isProtocolMethod) {
                            auto& protoName = method.second->protocolName;
                            protocolMethods[protoName].push_back(current->mangledMethods[method.first]->mangledName);
                        }

                        break;
                    }

                    current = current->parentClass;
                }
            }

            --depth;

            base = this;
            for (int i = 0; i < depth; ++i) {
                base = base->parentClass;
            }
        }
    }

    void Class::generateMemoryLayout(llvm::IRBuilder<>& Builder) {

        if (layoutGenerated) {
            return;
        }

        if (is_protocol) {
            return generateProtocolMemoryLayout(Builder);
        }

        short alignment = 1;

        size_t i = 0;
        bool hasVtable = false;
        if (!virtualMethods.empty()) {
            // virtual table for the inheritance tree, no protocol methods included
            hasVtable = true;
            vtableOffsets.emplace(className, i++);
            memoryLayout.push_back(
                llvm::ArrayType::get(Builder.getInt8PtrTy(), virtualMethods.size())->getPointerTo()
            );
        }

        if (parentClass != nullptr) {
            parentClass->generateMemoryLayout(Builder);
            auto& parentLayout = parentClass->memoryLayout;

            baseClassOffsets.insert(parentClass->baseClassOffsets.begin(), parentClass->baseClassOffsets.end());
            baseClassOffsets.emplace(parentClass->className, 0);

            if (!is_struct) {
                alignment = parentClass->alignment;
            }

            // dont copy default padding
            if (!parentClass->emptyLayout) {
                auto isVirtual = !virtualMethods.empty();
                auto it = !isVirtual ? parentLayout.begin() : ++parentLayout.begin();

                memoryLayout.insert(memoryLayout.end(), it, parentLayout.end());
                fieldOffsets.insert(parentClass->fieldOffsets.begin(), parentClass->fieldOffsets.end());
                vtableOffsets.insert(parentClass->vtableOffsets.begin(), parentClass->vtableOffsets.end());

                i += parentLayout.size();

                // parents vtable
                if (isVirtual) {
                    i -= 1;
                }
            }
        }

        if (hasVtable) {
            baseClassOffsets.emplace(className, i - 1);
        }
        else {
            baseClassOffsets.emplace(className, i);
        }

        auto classType = ObjectType::getStructureType(className);
        auto unqualPtr = llvm::PointerType::getUnqual(classType);

        for (const auto& fieldPair : fields) {
            const auto& field = fieldPair.second;

            if (field->isInheritedField) {
                continue;
            }

            if (field->fieldType->getAlignment() > alignment) {
                alignment = field->fieldType->getAlignment();
            }

            if (isa<ObjectType>(field->fieldType) && field->fieldType->getClassName() == className) {
                field->llvmType = unqualPtr;
            }
            else {
                field->llvmType = field->fieldType->getAllocaType();
            }

            memoryLayout.push_back(field->llvmType);
            fieldOffsets.emplace(field->fieldName, i++);
        }

        if (memoryLayout.empty()) {
            memoryLayout.push_back(Builder.getInt8PtrTy());
            emptyLayout = true;
        }

        if (is_struct) {
            this->alignment = alignment;
        }

        layoutGenerated = true;
    }

    void Class::generateProtocolMemoryLayout(llvm::IRBuilder<> &Builder) {

        // vtable
        memoryLayout.push_back(
            llvm::ArrayType::get(Builder.getInt8PtrTy(), methods.size())->getPointerTo()
        );

        // pointer to concrete type conforming to the protocol
        memoryLayout.push_back(
            Builder.getInt8PtrTy()
        );
    }

    namespace {

        void emplaceProtoVtables(
            unordered_map<string, string>& synonymousVtables,
            string& originalProto,
            string& currentProto
        ) {
            synonymousVtables.emplace(currentProto, originalProto);

            auto Protocol = SymbolTable::getClass(currentProto);
            for (const auto& prot : Protocol->getConformedToProtocols()) {
                emplaceProtoVtables(synonymousVtables, originalProto, prot->getClassName());
            }
        }

    }

    void Class::generateVTables(llvm::IRBuilder<> &Builder, llvm::Module &Module) {
        if (is_protocol) {
            size_t i = 0;
            for (const auto& method : methods) {
                methodOffsets.emplace(method.second->mangledName, i++);
            }

            return;
        }

        if (!virtualMethods.empty()) {
            std::vector<llvm::Constant*> vMethods;
            vMethods.reserve(virtualMethods.size());

            size_t i = 0;
            for (const auto& method : virtualMethods) {
                methodOffsets.emplace(method.second, i++);
                vMethods.push_back(llvm::cast<llvm::Constant>(
                    Builder.CreateBitCast(getMethod(method.second)->llvmFunc, Builder.getInt8PtrTy())
                ));
            }

            auto vtableType = llvm::ArrayType::get(Builder.getInt8PtrTy(), virtualMethods.size());
            vtable = new llvm::GlobalVariable(Module, vtableType, true, llvm::GlobalValue::ExternalLinkage,
                llvm::ConstantArray::get(vtableType, vMethods), "." + className + ".vtbl"
            );
        }

        for (const auto& prot : protocolMethods) {
            auto& methods = prot.second;

            std::vector<llvm::Constant*> vMethods;
            vMethods.reserve(prot.second.size());

            for (const auto& method : methods) {
                vMethods.push_back(llvm::cast<llvm::Constant>(
                    Builder.CreateBitCast(getMethod(method)->llvmFunc, Builder.getInt8PtrTy()))
                );
            }

            auto vtableType = llvm::ArrayType::get(Builder.getInt8PtrTy(), methods.size());
            auto glob = new llvm::GlobalVariable(Module, vtableType, true,
                llvm::GlobalValue::ExternalLinkage, llvm::ConstantArray::get(vtableType, vMethods),
                "." + className + "." + prot.first + ".vtbl"
            );

            auto protoName = prot.first;
            protocolVtables.emplace(prot.first, glob);
        }
    }

    bool Class::is_virtual(Method* method) {
        if (util::in_pair_vector(virtualMethods, SymbolTable::mangleFunction(method->methodName,
            method->argumentTypes, ""))) {
            return true;
        }

        for (const auto& child : extendedBy) {
            if (child->is_virtual(method)) {
                return true;
            }
        }

        return false;
    }

} // namespace cl
} // namespace cdot