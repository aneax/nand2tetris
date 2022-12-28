import gdb
import re


class Helpers:

    @staticmethod
    def word_to_int(value):
        pass


class NP(gdb.Command):

    def __init__(self):
        super(NP, self).__init__(
            "np", gdb.COMMAND_DATA, gdb.COMPLETE_EXPRESSION)

    def invoke(self, args, from_tty):
        argv = gdb.string_to_argv(args)
        for arg in argv:
            line = gdb.execute("p " + arg, from_tty=False, to_string=True)
            line = line.replace("=", "= " + arg + " =", 1)
            gdb.write(line)


NP()


class BitPrinter:

    """Pretty Printer for Bit class in Hardware Simulator
    Attributes:
        val: bit class object
    """

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val['set_']


def make_array(ptr, size):
    arr = []
    for i in range(size):
        arr.append(int(str(ptr[i])))
    return arr


def negate_bit(ptr, size):
    arr = []
    for i in range(size):
        arr.append(not int(str(ptr[i])))
    return arr


def add_one(ptr, size):
    arr = []
    carry = 1
    for i in range(size):
        val = ptr[i]+carry
        carry = int(val/2)
        bit = val % 2
        arr.append(bit)
    return arr


def reverse_bits(arr, size):
    bin_val = ""
    for i in reversed(range(size)):
        bin_val += str(arr[i])
    return bin_val


def get_string(arr, size, isAddress=False, printBinary=False):
    # print(arr[size-1])
    sign = ''
    irev = None
    if int(str(arr[size-1])) == 1 and not isAddress:
        sign = '-'
        # print("\nline 88", WordPrinter.reverse_bits(arr, size))
        neg_arr = negate_bit(arr, size)
        # print("negated", len(neg_arr), neg_arr)
        act_arr = add_one(neg_arr, size)
        # print("1 added", len(act_arr), act_arr)
        rev = reverse_bits(act_arr, size)
        irev = int(rev[1:], 2)
    else:
        rev = reverse_bits(arr, size)
        sign = '+'
        if isAddress:
            irev = int(rev, 2)
        else:
            irev = int(rev[1:], 2)
    # irev = ctypes.c_uint16(irev).value
    retval = ''
    if printBinary:
        retval = "0b"+rev+", "
    retval += sign + str(irev)

    return retval


class WordPrinter:

    def __init__(self, val):
        self.val = val
        self.WordSize = int(gdb.parse_and_eval('WordSize'))

    def to_string(self):
        # print(self.val.type.name, self.val.dynamic_type)
        # print(gdb.types.get_basic_type(self.val.type))
        # print(gdb.types.get_basic_type(self.val.type).name)
        # if self.val.type.name is not None:
        str_v = self.val['_M_elems']
        # gdb.set_convenience_variable('_tmp', self.val.reference_value())
        # size = gdb.parse_and_eval('$_tmp.size()')
        return get_string(str_v, self.WordSize)
        # else:
        #    print(self.val.type.fields())
        #    return None

    def display_hint(self):
        return "Word"


class WordSpanPrinter:

    def __init__(self, val):
        self.val = val
        self.WordSize = int(gdb.parse_and_eval('WordSize'))

    def to_string(self):
        ptr = self.val['_M_ptr']
        arr = []
        for i in range(self.WordSize):
            arr.append(int(str(ptr[i])))

        return get_string(ptr, self.WordSize)


class AddressSpanPrinter():

    def __init__(self, val):
        self.val = val
        self.WordSize = int(gdb.parse_and_eval('MemoryAddressSize'))

    def to_string(self):
        ptr = self.val['_M_ptr']
        arr = []
        for i in range(self.WordSize):
            arr.append(int(str(ptr[i])))

        return get_string(ptr, self.WordSize, True)


class RAMPrinter:

    def __init__(self, val):
        self.val = val

    def to_string(self):
        pass


class RAM8Printer:

    def __init__(self, val, pad=''):
        self.val = val

    def to_string(self):
        gdb.set_convenience_variable(
            '_tmp_ram8', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram8.size()')
        vstr = ''
        for it in range(size):
            istr = f"{it}:{str(self.val['data']['_M_elems'][it]):>6}"
            vstr += istr + ', '
        return vstr[:-2]


class RAM8Printer_v2:

    def __init__(self, val):
        self.val = val

    def make_array(self):
        gdb.set_convenience_variable(
            '_tmp_ram8', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram8.size()')
        arr = []
        st = '['
        for it in range(size):
            istr = int(str(self.val['data']['_M_elems'][it]))
            arr.append(istr)
            st += f"{istr:>6},"
        st = st[:-2]
        st += ']'
        return arr, st

    def to_string(self):
        arr, st = self.make_array()
        return st


class RAM64Printer:

    def __init__(self, val, pad='', base_index=0):
        self.val = val
        self.pad = pad
        self.base_index = base_index
        self.multiplier = 8
        self.childpad = self.pad + ' '

    def to_string(self):
        gdb.set_convenience_variable(
            '_tmp_ram64', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram64.size()')
        vstr = '\n' if self.pad == '' else '\n'
        for it in range(size):
            printer = RAM8Printer(
                self.val['data']['_M_elems'][it], self.childpad)
            pad = it * self.multiplier + self.base_index
            in_str = f"{self.pad}{it}:{pad:>5} -- {printer.to_string()}"
            vstr += in_str + '\n'
        return vstr[:-1]


class RAM64Printer_v2:

    def __init__(self, val, base_index=0):
        self.val = val
        self.base_index = base_index
        self.multiplier = 8

    def make_array(self):
        gdb.set_convenience_variable(
            '_tmp_ram64', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram64.size()')
        arr = []
        st = '\n' if self.base_index == 0 else ''
        for it in range(size):
            printer = RAM8Printer_v2(
                self.val['data']['_M_elems'][it])
            pad = it * self.multiplier + self.base_index
            c_arr, c_st = printer.make_array()
            arr.append(c_arr)
            st += f"{pad:<5}:{c_st}"
            st += ',\n'
        return arr, st

    def to_string(self):
        arr, st = self.make_array()
        return st


class RAM512Printer:

    def __init__(self, val, pad='', base_index=0):
        self.val = val
        self.pad = pad
        self.multiplier = 64
        self.base_index = base_index
        self.childpad = self.pad + ' '

    def to_string(self):
        gdb.set_convenience_variable(
            '_tmp_ram64', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram64.size()')
        vstr = '\n' if self.pad == '' else '\n'
        for it in range(size):
            child_bi = self.base_index + it*self.multiplier
            printer = RAM64Printer(
                self.val['data']['_M_elems'][it], self.childpad, child_bi)
            in_str = f"{self.pad}{it}:{child_bi:<5}\
                --{printer.to_string()}"
            vstr += in_str + '\n'
        return vstr


class RAM512Printer_v2:

    def __init__(self, val, base_index=0):
        self.val = val
        self.multiplier = 64
        self.base_index = base_index

    def make_array(self):
        gdb.set_convenience_variable(
            '_tmp_ram64', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram64.size()')
        arr = []
        st = ''
        for it in range(size):
            child_bi = self.base_index + it*self.multiplier
            printer = RAM64Printer_v2(
                self.val['data']['_M_elems'][it], child_bi)
            c_arr, c_st = printer.make_array()
            st += c_st
            arr.append(c_arr)
        return arr, st

    def to_string(self):
        arr, st = self.make_array()
        return st


class RAM4KPrinter:

    def __init__(self, val, pad='', base_index=0):
        self.val = val
        self.pad = pad
        self.multiplier = 512
        self.base_index = base_index
        self.childpad = self.pad + ' '

    def to_string(self):
        gdb.set_convenience_variable(
            '_tmp_ram512', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram512.size()')
        vstr = '\n' if self.pad == '' else '\n'
        for it in range(size):
            child_bi = self.base_index + self.multiplier * it
            printer = RAM512Printer(
                self.val['data']['_M_elems'][it], self.childpad, child_bi)
            in_str = f"{self.pad}{it}:{child_bi:5}--{printer.to_string()}"
            vstr += in_str + '\n'
        return vstr


class RAM4KPrinter_v2:

    def __init__(self, val, base_index=0):
        self.val = val
        self.multiplier = 64
        self.base_index = base_index

    def make_array(self):
        gdb.set_convenience_variable(
            '_tmp_ram4k', self.val['data'].reference_value())
        size = gdb.parse_and_eval('$_tmp_ram4k.size()')
        arr = []
        st = ''
        for it in range(size):
            child_bi = self.base_index + it*self.multiplier
            printer = RAM512Printer_v2(
                self.val['data']['_M_elems'][it], child_bi)
            c_arr, c_st = printer.make_array()
            st += c_st
            arr.append(c_arr)
        return arr, st

    def to_string(self):
        arr, st = self.make_array()
        return st


class MemoryPrinter:

    def __init__(self, val):
        self.val = val

    def to_string(self):
        pass


class ScreenPrinter:
    pass


class ComputerPrinter:
    pass


def str_lookup_function(val):
    lookup_tag = val.type.tag
    if lookup_tag is None:
        return None
    regex = re.compile("^std::array<hack::defs::Bit, 16>$")
    if regex.match(lookup_tag):
        # print("Regex Matched: ", lookup_tag)
        return WordPrinter(val)
    # print("Regex Failed:", )
    print(val.type.fields())
    return None


def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("nand2tetris")
    pp.add_printer('Bit', '^hack::defs::Bit$', BitPrinter)
    pp.add_printer('Word', '^std::array<hack::defs::Bit, 16>$', WordPrinter)
    pp.add_printer(
        'SpanWord', '^std::span<hack::defs::Bit const, 16>$', WordSpanPrinter)
    pp.add_printer(
        'SpanAddress',
        '^std::span<hack::defs::Bit const, 15>$',
        AddressSpanPrinter)

    # pp.add_printer(
    #     'RAM8', '^hack::defs::RAMx<std::array<hack::defs::Bit, 16>, 8, 3>$',
    #     RAM8Printer)
    pp.add_printer(
        'RAM8', '^hack::defs::RAMx<std::array<hack::defs::Bit, 16>, 8, 3>$',
        RAM8Printer_v2)
    pp.add_printer(
        'RAM64',
        '^hack::defs::RAMx<hack::defs::RAMx<std::array<hack::defs::Bit, 16>, 8, 3>, 8, 6>$',
        RAM64Printer_v2)

    pp.add_printer(
        'RAM512',
        '^hack::defs::RAMx<hack::defs::RAMx<hack::defs::RAMx<std::array<hack::defs::Bit, 16>, 8, 3>, 8, 6>, 8, 9>$',
        RAM512Printer_v2)

    pp.add_printer(
        'RAM4K',
        '^hack::defs::RAMx<hack::defs::RAMx<hack::defs::RAMx<hack::defs::RAMx<std::array<hack::defs::Bit, 16>, 8, 3>, 8, 6>, 8, 9>, 8, 12>$',
        RAM4KPrinter_v2
    )
    gdb.printing.register_pretty_printer(
        gdb.current_objfile(), pp, replace=True)


build_pretty_printer()
# gdb.pretty_printers.append(str_lookup_function)
