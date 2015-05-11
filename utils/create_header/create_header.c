#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        printf("Must supply three args.\n");
        return -1;
    }
    FILE *ifp = fopen(argv[1], "rb");
    FILE *ofp = fopen(argv[2], "w");
	if (!ifp)
	{
		fprintf(stderr, "Could not open file %s for reading\n", argv[1]);
	}
	if (!ofp)
	{
		fprintf(stderr, "Could not open file %s for writing\n", argv[2]);
	}
    uint8_t data;
    fprintf(ofp, "#include <stdint.h>\n");
    fprintf(ofp, "uint8_t %s_array[] = {\n", argv[3]);
    int count = 0;
    while(fread(&data, 1, 1, ifp))
    {
        fprintf(ofp, "0x%02X,", data);
        if((++count) % 10 == 0)
            fprintf(ofp, "\n");
    }
    fprintf(ofp, "};\n");
    fprintf(ofp, "uint32_t %s_size = %d;\n", argv[3], count);
}
