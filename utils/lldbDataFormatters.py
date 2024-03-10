"""
LLDB Formatters for LLVM data types.

Load into LLDB with 'command script import /path/to/lldbDataFormatters.py'
"""

import lldb
import lldb.formatters.Logger

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('type category define -e llvm -l c++')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.SmallVectorSynthProvider '
                           '-x "^llvm::SmallVectorImpl<.+>$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.SmallVectorSynthProvider '
                           '-x "^llvm::SmallVector<.+,.+>$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.ArrayRefSynthProvider '
                           '-x "^llvm::ArrayRef<.+>$"')
    debugger.HandleCommand('type summary add -w llvm '
                           '-F lldbDataFormatters.OptionalSummaryProvider '
                           '-x "^Optional<.+>$"')
    debugger.HandleCommand('type summary add -w llvm '
                           '-F lldbDataFormatters.SmallStringSummaryProvider '
                           '-x "^llvm::SmallString<.+>$"')
    debugger.HandleCommand('type summary add -w llvm '
                           '-F lldbDataFormatters.StringRefSummaryProvider '
                           '-x "^llvm::StringRef$"')

    # cdot type summaries
    debugger.HandleCommand('type summary add -w llvm '
                           '-F lldbDataFormatters.DeclNameSummaryProvider '
                           '-x "^cdot::DeclarationName$"')
    # debugger.HandleCommand('type summary add -w llvm '
    #                        '-F lldbDataFormatters.QualTypeSummaryProvider '
    #                        '-x "^cdot::QualType$"')
    # debugger.HandleCommand('type summary add -w llvm '
    #                        '-F lldbDataFormatters.SourceTypeSummaryProvider '
    #                        '-x "^cdot::ast::SourceType$"')
    debugger.HandleCommand('type summary add -w llvm '
                           '-F lldbDataFormatters.DeclSummaryProvider '
                           '-x "^cdot::ast::[A-Za-z]*Decl$"')

    # cdot type synths
    # debugger.HandleCommand('type synthetic add -w llvm '
    #                        '-l lldbDataFormatters.TupleTypeSynthProvider '
    #                        '-x "^cdot::TupleType$"')
    # debugger.HandleCommand('type synthetic add -w llvm '
    #                        '-l lldbDataFormatters.TupleTypeExprSynthProvider '
    #                        '-x "^cdot::ast::TupleTypeExpr$"')
    # debugger.HandleCommand('type synthetic add -w llvm '
    #                        '-l lldbDataFormatters.FunctionTypeSynthProvider '
    #                        '-x "^cdot::FunctionType$"')
    # debugger.HandleCommand('type synthetic add -w llvm '
    #                        '-l lldbDataFormatters.LambdaTypeSynthProvider '
    #                        '-x "^cdot::LambdaType$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.QualTypeSynthProvider '
                           '-x "^cdot::QualType"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.DeclarationNameSynthProvider '
                           '-x "^cdot::DeclarationName$"')
    # debugger.HandleCommand('type synthetic add -w llvm '
    #                        '-l lldbDataFormatters.ExprSequenceSynthProvider '
    #                        '-x "^cdot::ast::ExprSequence$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.AnonymousCallExprSynthProvider '
                           '-x "^cdot::ast::AnonymousCallExpr"')
    # debugger.HandleCommand('type synthetic add -w llvm '
    #                        '-l lldbDataFormatters.CompoundStmtSynthProvider '
    #                        '-x "^cdot::ast::CompoundStmt$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.DeclContextUnionSynthProvider '
                           '-x "^cdot::ast::Decl::DeclContextUnion$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l '
                           'lldbDataFormatters.FinalTemplateArgumentListSynthProvider '
                           '-x "^cdot::sema::FinalTemplateArgumentList"')

# Pretty printer for llvm::SmallVector/llvm::SmallVectorImpl
class SmallVectorSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj;
        self.update() # initialize this provider

    def num_children(self):
        begin = self.begin.GetValueAsUnsigned(0)
        end = self.end.GetValueAsUnsigned(0)
        return (end - begin)/self.type_size

    def get_child_index(self, name):
        try:
            return int(name.lstrip('[').rstrip(']'))
        except:
            return -1;

    def get_child_at_index(self, index):
        # Do bounds checking.
        if index < 0:
            return None
        if index >= self.num_children():
            return None;

        offset = index * self.type_size
        return self.begin.CreateChildAtOffset('['+str(index)+']',
                                              offset, self.data_type)

    def update(self):
        self.begin = self.valobj.GetChildMemberWithName('BeginX')
        self.end = self.valobj.GetChildMemberWithName('EndX')
        the_type = self.valobj.GetType()
        # If this is a reference type we have to dereference it to get to the
        # template parameter.
        if the_type.IsReferenceType():
            the_type = the_type.GetDereferencedType()

        self.data_type = the_type.GetTemplateArgumentType(0)
        self.type_size = self.data_type.GetByteSize()

class ArrayRefSynthProvider:
    """ Provider for llvm::ArrayRef """
    def __init__(self, valobj, dict):
        self.valobj = valobj;
        self.update() # initialize this provider

    def num_children(self):
        return self.length

    def get_child_index(self, name):
        try:
            return int(name.lstrip('[').rstrip(']'))
        except:
            return -1;

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None;
        offset = index * self.type_size
        return self.data.CreateChildAtOffset('[' + str(index) + ']',
                                             offset, self.data_type)

    def update(self):
        self.data = self.valobj.GetChildMemberWithName('Data')
        length_obj = self.valobj.GetChildMemberWithName('Length')
        self.length = length_obj.GetValueAsUnsigned(0)
        self.data_type = self.data.GetType().GetPointeeType()
        self.type_size = self.data_type.GetByteSize()
        assert self.type_size != 0

class TupleTypeSynthProvider:
    """ Provider for cdot::TupleType """
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.normal_child_count = 3 # type base, FoldingSetNode base, NumTys
        self.update()

    def num_children(self):
        return self.child_count + self.normal_child_count

    def get_child_index(self, name):
        if name == 'NumTys':
            return 1

        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normal_child_count:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize()    \
                  + (index - self.normal_child_count) * self.type_size

        name = '[' + str(index - self.normal_child_count) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.data_type)

    def update(self):
        self.data_type = self.valobj.GetFrame().GetModule()\
                                    .FindFirstType("cdot::QualType")
        self.type_size = self.data_type.GetByteSize()
        self.child_count = self.valobj.GetChildMemberWithName("NumTys")\
                                      .GetValueAsUnsigned(0)
        return True

class TupleTypeExprSynthProvider:
    """ Provider for cdot::ast::TupleTypeExpr """
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.normal_child_count = 2 # TypeExpr base, NumTys
        self.update()

    def num_children(self):
        return self.child_count + self.normal_child_count

    def get_child_index(self, name):
        if name == 'NumTys':
            return 1

        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normal_child_count:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize() \
                 + (index - self.normal_child_count) * self.type_size

        name = '[' + str(index - self.normal_child_count) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.data_type)

    def update(self):
        self.data_type = self.valobj.GetFrame().GetModule() \
            .FindFirstType("cdot::ast::SourceType")
        self.type_size = self.data_type.GetByteSize()
        self.child_count = self.valobj.GetChildMemberWithName("NumTys") \
            .GetValueAsUnsigned(0)
        return True

class FunctionTypeSynthProvider:
    """ Provider for cdot::FunctionType """
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.normal_child_count = 4 # type base, FoldingSetNode base, RetTy,
                                    # NumParams
        self.update()

    def num_children(self):
        return self.child_count + self.normal_child_count

    def get_child_index(self, name):
        if name == 'returnType':
            return 2

        if name == 'NumParams':
            return 3

        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normal_child_count:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize() \
                 + (index - self.normal_child_count) * self.type_size

        name = '[' + str(index - self.normal_child_count) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.data_type)

    def update(self):
        self.data_type = self.valobj.GetFrame().GetModule() \
                                    .FindFirstType("cdot::QualType")
        self.type_size = self.data_type.GetByteSize()
        self.child_count = self.valobj.GetChildMemberWithName("NumParams") \
                               .GetValueAsUnsigned(0)
        return True

class LambdaTypeSynthProvider:
    """ Provider for cdot::LambdaType """
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.normal_child_count = 1 # Function base
        self.update()

    def num_children(self):
        return self.child_count + self.normal_child_count

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normal_child_count:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize() \
                 + (index - self.normal_child_count) * self.type_size

        name = '[' + str(index - self.normal_child_count) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.data_type)

    def update(self):
        self.data_type = self.valobj.GetFrame().GetModule() \
            .FindFirstType("cdot::QualType")
        self.type_size = self.data_type.GetByteSize()
        self.child_count = self.valobj.GetChildMemberWithName("NumParams") \
            .GetValueAsUnsigned(0)
        return True

def OptionalSummaryProvider(valobj, internal_dict):
    if not valobj.GetChildMemberWithName('hasVal').GetValueAsUnsigned(0):
        return 'None'
    underlying_type = valobj.GetType().GetTemplateArgumentType(0)
    storage = valobj.GetChildMemberWithName('storage')
    return str(storage.Cast(underlying_type))

def SmallStringSummaryProvider(valobj, internal_dict):
    num_elements = valobj.GetNumChildren()
    res = "\""
    for i in range(0, num_elements):
        res += valobj.GetChildAtIndex(i).GetValue().strip("'")
    res += "\""
    return res

def StringRefSummaryProvider(valobj, internal_dict):
    data_ptr = valobj.GetChildAtIndex(0)
    length = valobj.GetChildAtIndex(1).GetValueAsUnsigned()
    data = data_ptr.GetPointeeData(0, length)

    error = lldb.SBError()
    res = ""

    for i in range(0, length):
        res += chr(data.GetUnsignedInt8(error, i))
        if error.Fail():
            return '<error: ' + error.GetCString() + '>'

    return res

savedStringValues = dict()

def stringifyValue(valobj, typeName, methodName):
    address = valobj.AddressOf().GetValueAsUnsigned(0)
    if address == 0:
        return "<null>"

    return hex(address)
    #
    # if address in savedStringValues:
    #     return savedStringValues[address]
    #
    # expr = "((::cdot::" + typeName + "*)" + str(address) + ")->" + methodName + "()"
    #
    # options = lldb.SBExpressionOptions()
    # result = valobj.GetFrame().EvaluateExpression(expr, options)
    #
    # res = result.GetSummary()
    # savedStringValues[address] = res
    #
    # return res

def QualTypeSummaryProvider(valobj, internal_dict):
    opaquePtr = valobj.GetChildAtIndex(0).GetValueAsUnsigned(0)
    if opaquePtr == 0:
        return "<null>"

    Module = valobj.GetFrame().GetModule()
    typeType = Module.FindFirstType('cdot::Type').GetPointerType()

    return str(valobj.CreateValueFromAddress("II", opaquePtr, typeType))

    # return stringifyValue(valobj, "QualType", "toString")

def SourceTypeSummaryProvider(valobj, internal_dict):
    return valobj.GetChildAtIndex(1).GetSummary()

def DeclSummaryProvider(valobj, internal_dict):
    Name = valobj.GetChildMemberWithName('Name')
    if not Name:
        return hex(valobj.AddressOf().GetValueAsUnsigned(0))

    return Name.GetSummary()

# Extended Discriminator
# NormalIdentifier = 0
# ConstructorName = 1
# DestructorName = 2
# InfixOperatorName = 3
# PrefixOperatorName = 4
# PostfixOperatorName = 5
# ConversionOperatorName = 6

# Stored Discriminator
# StoredIdentifier = 0
# StoredConstrutor = 1
# StoredDestructor = 2
# OtherStoredName  = 3

# PtrMask = 0x3

def DeclNameSummaryProvider(valobj, internal_dict):
    process = valobj.GetProcess()
    endianness = process.GetByteOrder()
    pointer_size = process.GetAddressByteSize()

    Val = valobj.GetChildAtIndex(0) \
        .GetChildAtIndex(0) \
        .GetValueAsUnsigned(0)

    Kind = Val & 3

    Module = valobj.GetFrame().GetModule()
    res = ""
    if Kind == 0:
        if Val in savedStringValues:
            return savedStringValues[Val]

        identifierInfoTy = Module.FindFirstType('cdot::IdentifierInfo')
        identifierInfoPtr = valobj.GetChildAtIndex(2)
        identifierInfo = identifierInfoPtr.CreateValueFromAddress(
            "II", identifierInfoPtr.GetValueAsUnsigned(0), identifierInfoTy)

        stringRef = identifierInfo.GetChildAtIndex(0)
        res = stringRef.GetSummary()
        savedStringValues[Val] = res

        return res

    return stringifyValue(valobj, "DeclarationName", "toString")

class DeclarationNameSynthProvider:
    """ Provider for cdot::DeclarationName """
    def __init__(self, valobj, dict):
        self.valobj = valobj
        process = valobj.GetProcess()
        self.endianness = process.GetByteOrder()
        self.pointer_size = process.GetAddressByteSize()
        self.normalChildCount = 1 # union { Val, StoredKind }
        self.update()

    def num_children(self):
        return self.normalChildCount + 2

    def get_child_index(self, name):
        return -1

    def has_children(self):
        return True

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if (index < self.normalChildCount):
            return self.valobj.GetChildAtIndex(index)

        Module = self.valobj.GetFrame().GetModule()

        if index == self.normalChildCount:
            # get StoredKind type through second member of first field
            UnionVal = self.valobj.GetChildAtIndex(0)
            StoredKindVal = UnionVal.GetChildAtIndex(1)
            KindTy = StoredKindVal.GetType()

            return self.valobj.CreateValueFromData(
                "Kind", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size, [self.Kind]),
                KindTy)

        # Stored IdentifierInfo*
        if self.Kind == 0:
            DeclNameTy = Module.FindFirstType('cdot::IdentifierInfo') \
                               .GetPointerType()
            return self.valobj.CreateValueFromData(
                "II", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size, [self.Val]),
                DeclNameTy)

        # Stored Initializer Name
        if self.Kind == 1:
            return self.valobj.CreateValueFromData(
                "InitTy", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size, [self.Val & ~3]),
                Module.FindFirstType('cdot::QualType'))

        # Stored Deinitializer Name
        if self.Kind == 2:
            return self.valobj.CreateValueFromData(
                "DeinitTy", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size, [self.Val & ~3]),
                Module.FindFirstType('cdot::QualType'))

        # Other Name
        if self.Kind == 3:
            return self.valobj.CreateValueFromData(
                "Other", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size, [self.Val & ~3]),
                Module.FindFirstType(
                    'cdot::DeclarationNameInfo').GetPointerType())

        return None

    def update(self):
        self.Val = self.valobj.GetChildAtIndex(0) \
            .GetChildAtIndex(0)                   \
            .GetValueAsUnsigned(0)

        self.Kind = self.Val & 3
        return True

class QualTypeSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        process = valobj.GetProcess()
        self.endianness = process.GetByteOrder()
        self.pointer_size = process.GetAddressByteSize()
        self.PtrMask = 0x7
        self.update()

    def num_children(self):
        return 2

    def has_children(self):
        return True

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        Module = self.valobj.GetFrame().GetModule()

        if index == 0:
            TypeTy = Module.FindFirstType("cdot::Type").GetPointerType()
            return self.valobj.CreateValueFromData(
                "Ty", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size,
                    [self.Val & ~self.PtrMask]), TypeTy)

        if index == 1:
            UnsignedTy = Module.FindFirstType("unsigned int")
            return self.valobj.CreateValueFromData(
                "Quals", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size,
                    [self.Val & self.PtrMask]), UnsignedTy)

        return None


    def update(self):
        self.Val = self.valobj.GetChildAtIndex(0).GetChildAtIndex(
            0).GetValueAsUnsigned(0)
        return False

class ExprSequenceSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.SeqElTy = self.valobj.GetFrame().GetModule().FindFirstType(
            "cdot::ast::SequenceElement")
        self.typeSize = self.SeqElTy.GetByteSize()
        self.normalChildCount = 2 # Expression base, NumElements
        self.numElements = self.valobj.GetChildMemberWithName(
            "NumFragments").GetValueAsUnsigned(0)
        self.update()

    def num_children(self):
        return self.normalChildCount + self.numElements

    def has_children(self):
        return True

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normalChildCount:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize() \
                 + (index - self.normalChildCount) * self.typeSize

        name = '[' + str(index - self.normalChildCount) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.SeqElTy)

    def update(self):
        return True

class CompoundStmtSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.StmtTy = self.valobj.GetFrame().GetModule().FindFirstType(
            "cdot::ast::Statement").GetPointerType()
        self.typeSize = self.StmtTy.GetByteSize()
        self.normalChildCount = 5 # Statement base, numStmts, preserveScope,
                                  # LBraceLoc, RBraceLoc
        self.numElements = self.valobj.GetChildMemberWithName(
            "numStmts").GetValueAsUnsigned(0)
        self.update()

    def num_children(self):
        return self.normalChildCount + self.numElements

    def has_children(self):
        return True

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normalChildCount:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize() \
                 + (index - self.normalChildCount) * self.typeSize

        name = '[' + str(index - self.normalChildCount) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.StmtTy)

    def update(self):
        return True

class AnonymousCallExprSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.StmtTy = self.valobj.GetFrame().GetModule().FindFirstType(
            "cdot::ast::Expression").GetPointerType()
        self.typeSize = self.StmtTy.GetByteSize()
        self.normalChildCount = 5 # Expression base, parenRange, ParentExpr, FnTy, numArgs, isPrimitiveInit
        self.numElements = self.valobj.GetChildMemberWithName(
            "NumArgs").GetValueAsUnsigned(0)
        self.update()

    def num_children(self):
        return self.normalChildCount + self.numElements

    def has_children(self):
        return True

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normalChildCount:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize() \
                 + (index - self.normalChildCount) * self.typeSize

        name = '[' + str(index - self.normalChildCount) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.StmtTy)

    def update(self):
        return True

class DeclContextUnionSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.rawValue = self.valobj.GetChildAtIndex(0).GetChildAtIndex(
            0).GetValueAsUnsigned(0)
        self.mask = 0x4 # llvm::PointerIntPair uses the highest bit available
        self.contextTy = self.valobj.GetFrame().GetModule().FindFirstType(
            "cdot::ast::DeclContext").GetPointerType()
        self.multipleTy = self.valobj.GetFrame().GetModule().FindFirstType(
            "cdot::ast::Decl::MultipleDeclContext").GetPointerType()
        process = valobj.GetProcess()
        self.endianness = process.GetByteOrder()
        self.pointer_size = process.GetAddressByteSize()
        self.update()

    def num_children(self):
        return 1

    def has_children(self):
        return True

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if not self.isMultiple:
            return self.valobj.CreateChildAtOffset("Single", 0, self.contextTy)
        else:
            return self.valobj.CreateValueFromData(
                "Multiple", lldb.SBData.CreateDataFromUInt64Array(
                    self.endianness, self.pointer_size,
                    [self.rawValue & ~self.mask]), self.multipleTy)

    def update(self):
        self.isMultiple = (self.rawValue & self.mask) != 0
        return False

class FinalTemplateArgumentListSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.normal_child_count = 4 # FoldingSetNode base, NumArgs, Dependent,
                                    # RuntimeParam
        self.update()

    def num_children(self):
        return self.child_count + self.normal_child_count

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        if index < self.normal_child_count:
            return self.valobj.GetChildAtIndex(index)

        offset = self.valobj.GetType().GetPointeeType().GetByteSize() \
                 + (index - self.normal_child_count) * self.type_size

        name = '[' + str(index - self.normal_child_count) + ']'
        return self.valobj.CreateChildAtOffset(name, offset, self.data_type)

    def update(self):
        self.data_type = self.valobj.GetFrame().GetModule() \
            .FindFirstType("cdot::sema::TemplateArgument")
        self.type_size = self.data_type.GetByteSize()
        self.child_count = self.valobj.GetChildMemberWithName("NumArgs") \
            .GetValueAsUnsigned(0)
        return True