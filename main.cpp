#include "Encode.h"

int main(int argc, const char* argv[])
{
	if (argc < 3) {
		printf("Usage: %s source.* output.lzw\n", argv[0]);
		return -1;
	}    

	FILE* source = {};
	fopen_s(&source, argv[1], "r");

	FILE* output = {};
	fopen_s(&output, argv[2], "w");

    LZWEncodeData(source, output);

	fclose(output);
	fclose(source);

	return 0;
}
