#!/usr/bin/env python2.7

def lfsr(value, width):
	t = (value & 0x1) ^ ((value >> 1) & 0x1)
	value = (t << (width - 1)) | (value >> 1)
	return value
	
def print_value(init):
	x = "{0:b}".format(init)
	x = '0' * (15 - len(x)) + x
	print(x)
	
def gen_table(width):
	out = []
	init = (1 << width) - 1
	out.append(init)
	init = lfsr(init, width)
	while(init != ((1 << width) - 1)):
		out.append(init)
		init = lfsr(init, width)
	return out
	
def write_table(fp, width):
	table = gen_table(width)
	fp.write("const uint8_t noise_table_{}[] ={{\n".format(width))
	count = 0
	for x in table:
		fp.write("{},".format(x & 0x1))
		if (count + 1) % 64 == 0:
			count = 0
			fp.write("\n")
		count += 1
	fp.write("\n};\n")
def main():

    with open("noise_table.h", "w") as fp:
        fp.write("#ifndef NOISE_TABLE_H\n")
        fp.write("#define NOISE_TABLE_H\n")
        write_table(fp, 7)
        write_table(fp, 15)
        fp.write("#endif\n")

if __name__ == "__main__":
	main()