#pragma once

#include "../common/fs_defs.h"
#include "../utils/logger.h"
#include "../dynamic_libs/os_functions.h"
#include <gd.h>

typedef struct
{
	u32 img_size;
	u32 img_id;
	void * img_data;
} JpegData;

typedef struct _R8G8B8A8_COLOR {
	u8 R, G, B, A;
} R8G8B8A8_COLOR;

void UnormR8G8B8A8TogdImage(gdImagePtr *gdImgTmp, void *image_data, u32 width, u32 rows, u32 pitch) {
	*gdImgTmp = gdImageCreateTrueColor(width / 2, rows / 2);

	R8G8B8A8_COLOR *buffer = (R8G8B8A8_COLOR *) image_data;
	R8G8B8A8_COLOR val;
	for (u32 row = 0; row < rows; ++row) {
		for (u32 x = 0; x < width; ++x) {
			val = buffer[row * pitch + x];
			gdImageSetPixel(*gdImgTmp, x / 2, row / 2, gdTrueColor(val.R, val.G, val.B));
			++x;
		}
		++row;
	}
}

void UnormR8G8B8A82Yuv420p(u8 **destination_, void *image_data, int *dest_img_size, u32 width, u32 height, u32 pitch) {
	u32 image_size = width * height;
	u32 upos = image_size;
	u32 vpos = upos + upos / 4;
	*dest_img_size = (vpos + upos / 4);
	if (*destination_) {
		free(destination_);
	}
	*destination_ = (u8 *) malloc(sizeof(u8) * *dest_img_size);
	u8 *destination = *destination_;
	if (!destination) {
		*dest_img_size = 0;
		return;
	}
	log_printf("allocated %d \n", *dest_img_size);

	R8G8B8A8_COLOR *buffer = (R8G8B8A8_COLOR *) image_data;

	u32 i = 0;

	for (u32 line = 0; line < height; ++line) {
		if (!(line % 2)) {
			for (u32 x = 0; x < width; x += 2) {
				u8 r = buffer[line * pitch + x].R;
				u8 g = buffer[line * pitch + x].G;
				u8 b = buffer[line * pitch + x].B;

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;

				destination[upos++] = ((-38 * r + -74 * g + 112 * b) >> 8) + 128;
				destination[vpos++] = ((112 * r + -94 * g + -18 * b) >> 8) + 128;

				r = buffer[line * pitch + x].R;
				g = buffer[line * pitch + x].G;
				b = buffer[line * pitch + x].B;

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
			}
		} else {
			for (u32 x = 0; x < width; x += 1) {
				u8 r = buffer[line * pitch + x].R;
				u8 g = buffer[line * pitch + x].G;
				u8 b = buffer[line * pitch + x].B;

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
			}
		}
	}
	sleep(1);
	log_printf("done %d \n", *dest_img_size);
}

void UnormR10G10B10A2TogdImage(gdImagePtr *gdImgTmp, void *image_data, u32 width, u32 rows, u32 pitch) {
	u32 *buffer = (u32 *) image_data;
	u32 val = 0;
	for (u32 row = 0; row < rows; ++row) {
		for (u32 x = 0; x < width; ++x) {
			/*
				R ((test >> 24) & 0xFF))
				G ((test >> 14) & 0xFF))
				B ((test >> 4) & 0xFF))
				alpha (test & 0x3);
			*/
			val = buffer[row * pitch + x];
			gdImageSetPixel(*gdImgTmp, x, row, gdTrueColor(((val >> 24) & 0xFF),
														   ((val >> 14) & 0xFF),
														   ((val >> 4) & 0xFF)));
		}
	}
}