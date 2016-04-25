#include "colorConvert.h"
#include <string.h>

void NV21TOYUV420SP(const unsigned char *src,const unsigned char *dst,int ySize)
{
	memcpy(dst,src,ySize);
	int uvSize = ySize>>1;
	int uSize = uvSize>>1;
	memcpy(dst+ySize,src+ySize+1,uvSize-1);
	unsigned char *nvcur = src+ySize;
	unsigned char *yuvcur = dst+ySize+1;
	int i=0;
	while(i<uSize)
	{
		(*yuvcur)=(*nvcur);
		yuvcur+=2;
		nvcur+=2;
		++i;
	}
}
void NV21TOYUV420P(const unsigned char *src,const unsigned char *dst,int ySize)
{
	memcpy(dst,src,ySize);
	int uSize = ySize>>2;
	unsigned char *srcucur = src+ySize+1;
	unsigned char *srcvcur = src+ySize;
	unsigned char *dstucur = dst+ySize;
	unsigned char *dstvcur = dst+ySize+uSize;
	int i=0;
	while(i<uSize)
	{
		(*dstucur)=(*srcucur);
		(*dstvcur)=(*srcvcur);
		srcucur+=2;
		srcvcur+=2;
		++dstucur;
		++dstvcur;
		++i;
	}
}
void YUV420SPTOYUV420P(const unsigned char *src,const unsigned char *dst,int ySize)
{
	memcpy(dst,src,ySize);
	int uSize = ySize>>2;
	unsigned char *srcucur = src+ySize;
	unsigned char *srcvcur = src+ySize+1;
	unsigned char *dstucur = dst+ySize;
	unsigned char *dstvcur = dst+ySize+uSize;
	int i=0;
	while(i<uSize)
	{
		(*dstucur)=(*srcucur);
		(*dstvcur)=(*srcvcur);
		srcucur+=2;
		srcvcur+=2;
		++dstucur;
		++dstvcur;
		++i;
	}
}
void NV21TOARGB(const unsigned char *src,const unsigned int *dst,int width,int height)
{
	int frameSize = width * height;

	int i = 0, j = 0,yp = 0;
	int uvp = 0, u = 0, v = 0;
	int y1192 = 0, r = 0, g = 0, b = 0;
	unsigned int *target=dst;
	for (j = 0, yp = 0; j < height; j++)
	{
		uvp = frameSize + (j >> 1) * width;
		u = 0;
		v = 0;
		for (i = 0; i < width; i++, yp++)
		{
			int y = (0xff & ((int) src[yp])) - 16;
			if (y < 0)
				y = 0;
			if ((i & 1) == 0)
			{
				v = (0xff & src[uvp++]) - 128;
				u = (0xff & src[uvp++]) - 128;
			}

			y1192 = 1192 * y;
			r = (y1192 + 1634 * v);
			g = (y1192 - 833 * v - 400 * u);
			b = (y1192 + 2066 * u);

			if (r < 0) r = 0; else if (r > 262143) r = 262143;
			if (g < 0) g = 0; else if (g > 262143) g = 262143;
			if (b < 0) b = 0; else if (b > 262143) b = 262143;
			target[yp] = 0xff000000 | ((r << 6) & 0xff0000) | ((g >> 2) & 0xff00) | ((b >> 10) & 0xff);
		}
	}
}