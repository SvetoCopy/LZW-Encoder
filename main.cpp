#include "LZW.h"

int main(int argc, const char* argv[])
{
	if (argc < 4) {
		printf("Usage: %s mode (1 - encode, 2 - decode) source.* output.*\n", argv[0]);
		return -1;
	}    

	FILE* source = {};
	FILE* output = {};

	fopen_s(&source, argv[2], "rb");
	fopen_s(&output, argv[3], "wb");

	int mode = atoi(argv[1]);

	if		(mode == ENCODE) LZWEncodeData(source, output);
	else if (mode == DECODE) LZWDecodeData(source, output);
	else				     printf("UNKNOWN MODE");

	fclose(output);
	fclose(source);

	return 0;
}
