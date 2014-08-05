#!/usr/bin/env python2
from HTMLParser import HTMLParser
import re

gen_prefix = False

DATA_FILE = "./data/opcodes_8bit.html"
if(gen_prefix):
	DATA_FILE = "./data/opcodes_prefix_cb.html"

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
	l = ["NOP", "LD", "INC", "DEC",
		"RLCA","ADD", "RRCA","STOP",
		"RLA", "XOR", "JR" , "HALT",
		"RST", "CALL","SUB", "PUSH",
		"CP", "RST", "POP", "ADC",
		"SBC", "AND", "DI", "EI",
		"LDH", "CCF", "DAA", "SCF",
		"JP", "RET", "OR", "RRA",
		"PREFIX"]

	if(gen_prefix):
		l = ["RLC", "RRC", "RL", "RR", "SLA", "SRA", "SWAP", "SRL", "BIT", "RES", "SET"]

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
		out += "DATA8_UNSIGNED_INDIRECT"
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
	elif(re.match("^\d", arg)):
		out += arg
	else:
		raise Exception("Invalid arg type", arg)
	return (out, reg)

def gen_op_base(op):
	if(len(op)):
		s = re.split(" |,", op[0])
		if(len(s) == 1):
			return (s[0], "ARG_TYPE_NONE", "REG_A", "ARG_TYPE_NONE", "REG_A")
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

def gen_op(op):
	try:
		size = int(op[1])
		splt = op[2].split("/")
		success = splt[0]
		try:
			fail = splt[1]
		except:
			fail = 0
	except:
		size    = 0
		success = 0
		fail    = 0

	return gen_op_base(op) + (size, success, fail)

count = 0
def gen_struct(op, f):
	global count
	out = "{ .op = (cpu_opcode)" + op[0] +\
		  ", .arg0 = "           + op[1] +\
		  ", .i0 = "             + op[2] +\
		  ", .arg1 = "           + op[3] +\
		  ", .i1 = "             + op[4] +\
		  ", .size = "           + str(op[5]) +\
		  ", .success = "        + str(op[6]) +\
		  ", .fail = "           + str(op[7]) +\
		  "}/*opcode " + hex(count) + "*/"
	count += 1
	f.write(out)

template = {}

"""
op_name => first_arg => second_arg
"""
def add_to_template(op):
	if op[0] not in template:
		template[op[0]] = {}
	d = template[op[0]]
	if op[1] not in d:
		d[op[1]] = set()
	d[op[1]].add(op[3])

""" Write op will create a definition for a the op structure in the
	C code, it has the form: func, arg0, reg0, arg1, reg1
"""
def write_op(op, f):
	op = gen_op(op)
	gen_struct(op, f)
	add_to_template(op)

def gen_list():
	f = open(DATA_FILE)
	t = ""
	for i in f:
		t += i

	parser = Temp()
	parser.feed(t)
	out = [i for i in parser.out if filter_ops(i)]
	return out[1:]

def gen_templates():
	for i in sorted(template.keys()): # Each of the instructon names
		f = open("./src/cpu/instructions/new/" + i.lower() + ".c", "w")
		if(gen_prefix):
			f = open("./src/cpu/instructions/new/prefix_cb/" + i.lower() + ".c", "w")
		f.write("#include \"cpu.h\"\n\n");
		sig = "struct cpu_state *state,\n\t\tenum ARG_TYPE arg0, union REG_INPUT i0,\n\t\tenum ARG_TYPE arg1, union REG_INPUT i1"
		f.write("void " + i + "(" + sig + ")\n{\n")
		print(i)
		count0 = 0
		for j in sorted(template[i].keys()): # Each of the possible arg0
			print("\t" + str(j))
			if count0 == 0:
				mod = "if"
				count0 = 1
			else:
				mod = "else if"
			f.write("\t" + mod + "(arg0 == " + str(j) + ")\n\t{\n")
			count1 = 0
			for k in sorted(template[i][j]): # Each of the arg1
				if count1 == 0:
					mod = "if"
					count1 = 1
				else:
					mod = "else if"
				f.write("\t\t" + mod + "(arg1 == " + str(k) + ")\n\t\t{\n\t\t}\n")
				print("\t\t" + str(k))
			f.write("\t}\n")
		f.write("}")
		f.close()

def main():
	l = gen_list()

	out = open("./src/cpu/opcodes.c", "w")
	out.write("#include \"opcodes.h\"\n\n")
	if(gen_prefix):
		out = open("./src/cpu/prefix_cb_opcodes.g", "w")
	out.write("struct opcode op_table[] = {\n")
	for i in l:
		write_op(i, out)
		out.write(",\n")
	out.write("\n};")

	#gen_templates();


	out.close()

if __name__ == "__main__":
	main()
