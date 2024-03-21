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
	fopen_s(&output, argv[2], "wb");

	FILE* output2 = {};
	fopen_s(&output2, "output2.txt", "w");

    LZWEncodeData(source, output);
	//LZWDecodeData(output, output2);

	fclose(output2);
	fclose(output);
	fclose(source);

	return 0;
}
