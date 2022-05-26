/****************************************************************************
 * Copyright (C) 2017 Maschell, BullyWiiPlaza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include <stdlib.h> // malloc()/free()
#include <gd.h> // image library
#include "../utils/function_patcher.h"
#include "../utils/logger.h"
#include "function_patcher_gx2.h"
#include <gd.h>
#include <string.h> // memcpy()

static volatile int executionCounter = 0;

bool shouldTakeScreenShot = false;
unsigned int remainingImageSize = 0;
unsigned int totalImageSize = 0;
int bufferedImageSize = 0;
void *bufferedImageData = NULL;

declareFunctionHook(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32
		scan_target) {
	if (executionCounter > 120) {
		GX2Surface surface = colorBuffer->surface;
		log_printf("GX2CopyColorBufferToScanBuffer {surface width:%d, height:%d, image size:%d, image data:%x}\n",
				   surface.width, surface.height, surface.image_size, surface.image_data);

		if (shouldTakeScreenShot) {
			void *imageData = surface.image_data;
			totalImageSize = surface.image_size;
			remainingImageSize = totalImageSize;
			int bufferSize = IMAGE_BUFFER_SIZE;

			while (remainingImageSize > 0) {
				bufferedImageData = malloc(bufferSize);
				u32 imageSizeRead = totalImageSize - remainingImageSize;
				memcpy(bufferedImageData, imageData + imageSizeRead, bufferSize);
				bufferedImageSize = bufferSize;

				// Wait while the data is not read yet
				while (bufferedImageSize > 0) {
					usleep(WAITING_TIME_MILLISECONDS);
				}

				free(bufferedImageData);
				remainingImageSize -= bufferSize;
			}

			shouldTakeScreenShot = false;
		}
		/*s32 format = surface.format;

		gdImagePtr gdImagePtr = 0;
		bool no_convert;
		u8 *image_data = NULL;
		int img_size = 0;
		if (format == 0x1A) {
			UnormR8G8B8A82Yuv420p(&image_data, surface.image_data, &img_size, surface.width, surface.height,
								  surface.pitch);
		} else if (format == 0x19) {
			no_convert = true;
			UnormR8G8B8A8TogdImage(&gdImagePtr, surface.image_data, surface.width, surface.height, surface.pitch);
		} else {
			no_convert = true;
		}

		u32 imd_size = 0;
		void *data = gdImageJpegPtr(gdImagePtr, &imd_size, 95);
		if (data) {
			JpegData jpeg;
			jpeg.img_size = imd_size;
			jpeg.img_data = data;
			jpeg.img_id = 0;
		}*/

		executionCounter = 0;
	}

	executionCounter++;

	real_GX2CopyColorBufferToScanBuffer(colorBuffer, scan_target);
}

FunctionHook method_hooks_gx2[] __attribute__((section(".data"))) = {
		// makeFunctionHook(GX2CopyColorBufferToScanBuffer, LIB_GX2, STATIC_FUNCTION)
};

u32 method_hooks_size_gx2 __attribute__((section(".data"))) = sizeof(method_hooks_gx2) / sizeof(FunctionHook);

volatile unsigned int method_calls_gx2[sizeof(method_hooks_gx2) / sizeof(FunctionHook) *
									   FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));