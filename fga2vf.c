#include <stdio.h>
#include <stdlib.h>

#define LINE_MAX (1024)

typedef struct {
	char four_cc[4];
	unsigned short x;
	unsigned short y;
	unsigned short z;
}S_VF_HEADER;

int main (int argc, char** argv)
{
	S_VF_HEADER sHeader = {
		{'V', 'F', '_', 'V'},
		0,
		0,
		0
	};
	FILE* pInFD;
	FILE* pOutFD;
	float* pData;
	char line[LINE_MAX];
	unsigned long volume_size;
	
	pInFD = fopen (argv[1], "r");
	pOutFD = fopen (argv[2], "wb");

	fgets (line, LINE_MAX, pInFD);
	sscanf (line, "%hd,%hd,%hd,", &sHeader.x, &sHeader.y, &sHeader.z);
	volume_size = sHeader.x * sHeader.y * sHeader.z;
	fgets (line, LINE_MAX, pInFD);
	fgets (line, LINE_MAX, pInFD);

	pData = (float*)malloc (sizeof(float) * volume_size);

	for (int i = 0; i < volume_size; i++) {
		fgets (line, LINE_MAX, pInFD);
		sscanf (line, "%f,%f,%f,", pData + i + 0, pData + i + 1, pData + i + 2);
	}
	
	fwrite (&sHeader, sizeof(sHeader), 1, pOutFD);
	fwrite (pData, sizeof(float), volume_size, pOutFD);

	free (pData);
	fclose (pOutFD);
	fclose (pInFD);
	
	return 0;
}
