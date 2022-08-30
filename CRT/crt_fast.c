
/*
By Kannagichan (kannagichan@gmail.com)
*/
#include <stdint.h>
#include "crt.h"

void __attribute__((optimize("-O2"))) CRTx22fast(unsigned char *srcPtr,unsigned char *dstPtr,int width, int height,int srcpitch,int pitch)
{
	const int bytepixel = 4;
	int i;

	int R = 0,G = 0,B = 0,RS,GS,BS;
	int l = 0;
	int x,y;
	int tmp = 0;

	for(y = 0;y < height;y++)
	{
		int ys = (y*srcpitch);
		for(x = 0;x < width;x++)
		{

			i = (x*bytepixel) + ys;
			R = srcPtr[i+0];
			G = srcPtr[i+1];
			B = srcPtr[i+2];

			i+=bytepixel;
			RS = ( ((int)srcPtr[i+0]+R)>>1);
			GS = ( ((int)srcPtr[i+1]+G)>>1);
			BS = ( ((int)srcPtr[i+2]+B)>>1);

			dstPtr[tmp+0] = R&0xF8;
			dstPtr[tmp+1] = G&0xF8;
			dstPtr[tmp+2] = B&0xF8;
			tmp += bytepixel;
			dstPtr[tmp+0] = RS|0x07;
			dstPtr[tmp+1] = GS|0x07;
			dstPtr[tmp+2] = BS|0x07;
			tmp += bytepixel;

		}

		for(x = 0;x < width;x++)
		{
			i = (x*bytepixel) + ys;
			R = srcPtr[i+0];
			G = srcPtr[i+1];
			B = srcPtr[i+2];

			i+=bytepixel;
			RS = ( ((int)srcPtr[i+0]+R)>>1);
			GS = ( ((int)srcPtr[i+1]+G)>>1);
			BS = ( ((int)srcPtr[i+2]+B)>>1);

			dstPtr[tmp+0] = (R>>1)&0xFE;
			dstPtr[tmp+1] = (G>>1)&0xFE;
			dstPtr[tmp+2] = (B>>1)&0xFE;
			tmp += bytepixel;
			dstPtr[tmp+0] = (RS>>1)|1;
			dstPtr[tmp+1] = (GS>>1)|1;
			dstPtr[tmp+2] = (BS>>1)|1;
			tmp += bytepixel;

		}


	}

}

