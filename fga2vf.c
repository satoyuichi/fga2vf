#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
	float* pfData;
	char line[LINE_MAX];
	unsigned long volume_size;
	int stride = 3;
	int optIndex = 1;
	float x, y, z;

	if (strcmp ("-d", argv[1]) == 0) {
		sHeader.four_cc[3] = 'F';
		stride = 1;
		optIndex++;
	}
	
	pInFD = fopen (argv[optIndex++], "r");
	pOutFD = fopen (argv[optIndex++], "wb");

	fgets (line, LINE_MAX, pInFD);
	sscanf (line, "%hd,%hd,%hd,", &sHeader.x, &sHeader.y, &sHeader.z);
	volume_size = sHeader.x * sHeader.y * sHeader.z;
	fgets (line, LINE_MAX, pInFD);
	fgets (line, LINE_MAX, pInFD);

	pfData = (float*)malloc (sizeof(float) * volume_size * stride);

	for (int i = 0; fgets (line, LINE_MAX, pInFD); i++) {
		sscanf (line, "%f,%f,%f,", &x, &y, &z);
		if (stride == 1) {
			*(pfData + i) = (float)sqrt (x * x + y * y + z * z);
		}
		else {
			*(pfData + i * 3 + 0) = x;
			*(pfData + i * 3 + 1) = y;
			*(pfData + i * 3 + 2) = z;
		}
		/* printf ("%f, %f, %f\n", */
		/* 		*(pfData + i * 3 + 0), */
		/* 		*(pfData + i * 3 + 1), */
		/* 		*(pfData + i * 3 + 2)); */
	}
	
	fwrite (&sHeader, sizeof(sHeader), 1, pOutFD);
	fwrite (pfData, sizeof(pfData[0]), volume_size * stride, pOutFD);

	//	printf ("size: %d, volume_size: %d\n", sizeof(pfData[0]), volume_size * stride);

	free (pfData);
	fclose (pOutFD);
	fclose (pInFD);
	
	return 0;
}
