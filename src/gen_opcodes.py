#!/usr/bin/env python2
from HTMLParser import HTMLParser
import re

class Temp(HTMLParser):
	def __init__(self):
		HTMLParser.__init__(self)
		self.out = [[]]
	def handle_starttag(self, tag, attrs):
		if(tag == "td"):
			self.out.append([])
	def handle_endtag(self, tag):
		pass
	def handle_data(self, data):
		self.out[-1].append(data)

def start_with_list(x, l):
	for i in l:
		if(x.startswith(i)):
			return 1
	return 0

def filter_ops(x):
	l = ["NOP", "LD",  "INC", "DEC",
		 "RLCA","ADD", "RRCA","STOP",
		 "RLA", "XOR", "JR" , "HALT",
		 "RST", "CALL","SUB", "PUSH",
		 "CP",  "RST", "POP", "ADC",
		 "SBC", "AND", "DI",  "EI",
		 "LDH", "CCF", "DAA", "SCF",
		 "JP", "RET", "OR", "RRA",
		 "PREFIX"]
	if(len(x) == 0):
		return 1
	if(start_with_list(x[0], l)):
		return 1
	return 0

def arg_type(arg):
	out = "ARG_TYPE_"
	reg = "REG_A"
	if(re.match("^[A-Z]$", arg)):
		out += "REG8"
		reg = "REG_" + arg
	elif(re.match("^[A-Z]{2}$", arg)):
		out += "REG16"
		reg = "REG16_" + arg
	elif(re.match("^\([A-Z]\)$", arg)):
		out += "REG8_INDIRECT"
		reg = "REG_" + arg[1:-1]
	elif(re.match("^\([A-Z]{2}\)$", arg)):
		out += "REG16_INDIRECT"
		reg = "REG16_" + arg[1:-1]
	elif(arg == "d8"):
		out += "DATA8"
	elif(arg == "d16"):
		out += "DATA16"
	elif(arg == "r8"):
		out += "REL8"
	elif(arg == "SP+r8"):
		out += "REL8_ADD_SP"
	elif(arg == "a8"):
		out += "DATA8_UNSIGNED"
	elif(arg == "a16"):
		out += "DATA16_UNSIGNED"
	elif(arg == "(a8)"):
		out += "DATA16_UNSIGNED_INDIRECT"
	elif(arg == "(a16)"):
		out += "DATA16_UNSIGNED_INDIRECT"
	elif(arg == "0"):
		out += "DATA8"
	elif(re.match("^.{1,2}H$", arg)):
		out += arg
	elif(arg == "(HL+)"):
		out += "HL_INDIRECT_INC"
	elif(arg == "(HL-)"):
		out += "HL_INDIRECT_DEC"
	else:
		raise Exception("Invalid arg type", arg)
	return (out, reg)

def gen_op(op):
	if(len(op)):
		s = re.split(" |,", op[0])
		if(s[0] == "NOP"):
			return ("NOP", "ARG_TYPE_NONE", "REG_A", "ARG_TYPE_NONE", "REG_A")
		if(s[0] == "PREFIX"):
			return ("PREFIX_CB", "ARG_TYPE_NONE", "REG_A", "ARG_TYPE_NONE", "REG_A")
		if(len(s) == 2):
			if s[1] in ["Z", "NZ", "NC"]:
				return (s[0], "ARG_TYPE_" + s[1], "REG_A", "ARG_TYPE_NONE", "REG_A")
			a0 = arg_type(s[1])
			return (s[0], a0[0], a0[1], "ARG_TYPE_NONE", "REG_A")
		if(len(s) == 3):
			a0 = arg_type(s[1])
			a1 = arg_type(s[2])
			if s[1] in ["NC", "Z", "NZ"]:
				return s[0], "ARG_TYPE_" + s[1], "REG_A", a1[0], a1[1]
			return (s[0], a0[0], a0[1], a1[0], a1[1])
	return ("INVALID", "ARG_TYPE_NONE", "REG_A", "ARG_TYPE_NONE", "REG_A")

count = 0
def gen_struct(op, f):
	global count
	out = "{ .op = (cpu_opcode)"   + op[0] +\
		  ", .arg0 = " + op[1] +\
		  ", .i0 = "   + op[2] +\
		  ", .arg1 = " + op[3] +\
		  ", .i1 = "   + op[4] +\
		  "}/*opcode " + hex(count) + "*/"
	count += 1
	f.write(out)

""" Write op will create a definition for a the op structure in the
	C code, it has the form: func, arg0, reg0, arg1, reg1
"""
def write_op(op, f):
	op = gen_op(op)
	gen_struct(op, f)

def gen_list():
	f = open("./data/opcodes_8bit.html")
	t = ""
	for i in f:
		t += i

	parser = Temp()
	parser.feed(t)
	out = [i for i in parser.out if filter_ops(i)]
	return out[1:]

def main():
	l = gen_list()

	out = open("./src/opcodes.h", "w")
	out.write("struct opcode op_table[] = {\n")
	for i in l:
		write_op(i, out)
		out.write(",\n")
	out.write("\n};")
	"""
	for i in t:
		print(i)
	"""

	out.close()

if __name__ == "__main__":
	main()
