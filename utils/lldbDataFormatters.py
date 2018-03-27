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
                           '-x "^llvm::Optional<.+>$"')

    # cdot types
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.TupleTypeSynthProvider '
                           '-x "^cdot::TupleType$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.FunctionTypeSynthProvider '
                           '-x "^cdot::FunctionType$"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.QualTypeSynthProvider '
                           '-x "^cdot::QualType"')
    debugger.HandleCommand('type synthetic add -w llvm '
                           '-l lldbDataFormatters.DeclarationNameSynthProvider '
                           '-x "^cdot::DeclarationName$"')

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
        assert self.type_size != 0

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

def OptionalSummaryProvider(valobj, internal_dict):
    if not valobj.GetChildMemberWithName('hasVal').GetValueAsUnsigned(0):
        return 'None'
    underlying_type = valobj.GetType().GetTemplateArgumentType(0)
    storage = valobj.GetChildMemberWithName('storage')
    return str(storage.Cast(underlying_type))

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

class DeclarationNameSynthProvider:
    """ Provider for cdot::DeclarationName """
    def __init__(self, valobj, dict):
        self.valobj = valobj
        process = valobj.GetProcess()
        self.endianness = process.GetByteOrder()
        self.pointer_size = process.GetAddressByteSize()
        self.update()

    def num_children(self):
        return 2

    def get_child_index(self, name):
        return -1

    def has_children(self):
        return True

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None

        Module = self.valobj.GetFrame().GetModule()

        if index == 0:
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