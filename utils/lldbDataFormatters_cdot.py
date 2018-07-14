"""
LLDB Formatters for LLVM data types.

Load into LLDB with 'command script import /path/to/lldbDataFormatters.py'
"""

import lldb
import lldb.formatters.Logger

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('type category define -e cdot')
    debugger.HandleCommand('type category disable cdot')
    debugger.HandleCommand('type synthetic add -w cdot '
                           '-l lldbDataFormatters_cdot.ArraySynthProvider '
                           '-x "^Array\\[.+\\]$"')
    debugger.HandleCommand('type synthetic add -w cdot '
                           '-l lldbDataFormatters_cdot.DynamicBufferSynthProvider '
                           '-x "^DynamicBuffer\\[.+\\]$"')
    debugger.HandleCommand('type summary add -w cdot '
                           '-F lldbDataFormatters_cdot.StringSummaryProvider '
                           '-x "^String$"')

class ArraySynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.update() # initialize this provider

    def num_children(self):
        begin = self.begin.GetValueAsUnsigned(0)
        end = self.end.GetValueAsUnsigned(0)
        return (end - begin)/self.type_size

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        # Do bounds checking.
        if index < 0:
            return None
        if index >= self.num_children():
            return None

        offset = index * self.type_size
        return self.begin.CreateChildAtOffset('['+str(index)+']',
                                              offset, self.data_type)

    def update(self):
        self.begin = self.valobj.GetChildMemberWithName('BeginPtr')
        self.end = self.valobj.GetChildMemberWithName('EndPtr')

        self.data_type = self.begin.GetType().GetPointeeType()
        self.type_size = self.data_type.GetByteSize()

        assert self.type_size != 0
        return True

class DynamicBufferSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj
        self.update() # initialize this provider

    def num_children(self):
        return self.size

    def get_child_index(self, name):
        return -1

    def get_child_at_index(self, index):
        # Do bounds checking.
        if index < 0:
            return None
        if index >= self.num_children():
            return None

        offset = index * self.type_size
        return self.data.CreateChildAtOffset('['+str(index)+']',
                                              offset, self.data_type)

    def update(self):
        _data = self.valobj.GetChildMemberWithName('_data')
        _size = self.valobj.GetChildMemberWithName('_size')
        _capacity = self.valobj.GetChildMemberWithName('_capacity')

        self.data_type = _data.GetType().GetPointeeType()
        self.type_size = self.data_type.GetByteSize()

        SmallFlag = 1 << 63
        SmallShift = 63 - 7
        SmallSizeMask = 0b01111111 << SmallShift

        IsSmall = (_capacity.GetValueAsUnsigned(0) & SmallFlag) != 0

        if IsSmall:
            self.size = (_capacity.GetValueAsUnsigned(0) & SmallSizeMask) \
                   >> SmallShift
            self.data = self.valobj
        else:
            self.size = _size.GetValueAsUnsigned(0)
            self.data = _data

        assert self.type_size != 0
        return True

def StringSummaryProvider(valobj, dict):
    _chars = valobj.GetChildMemberWithName('_chars')
    _size = valobj.GetChildMemberWithName('_size')
    _capacity = valobj.GetChildMemberWithName('_capacity')

    char_type = _chars.GetType()

    SmallFlag = 1 << 63
    SmallShift = 63 - 7
    SmallSizeMask = 0b01111111 << SmallShift

    IsSmall = (_capacity.GetValueAsUnsigned(0) & SmallFlag) != 0

    if IsSmall:
        chars = _chars
        size = (_capacity.GetValueAsUnsigned(0) & SmallSizeMask) \
                    >> SmallShift

        strval = ''
        G = chars.GetData().uint8
        for X in range(size):
            V = G[X]
            if V == 0:
                break
            strval = strval + chr(V % 256)

        return '"' + strval + '"'
    else:
        chars = _chars
        size = _size.GetValueAsUnsigned(0)

        try:
            data = chars.GetPointeeData(0, size + 1)
        except:
            return '""'

        error = lldb.SBError()
        strval = data.GetString(error, 0)
        if error.Fail():
            return '<error:' + error.GetCString() + '>'
        else:
            return '"' + strval + '"'