/****************************************************************************
 * Copyright (C) 2016 Maschell
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/function_patcher.h"

// The dynamically allocated buffer size for the image copy
#define IMAGE_BUFFER_SIZE 100

// The time the producer and consumer wait while there is nothing to do
#define WAITING_TIME_MILLISECONDS 1

// Flag for telling the hook whether to dump a screen shot
extern bool shouldTakeScreenShot;

// Indication for the consumer how many bytes are there to read in total
extern unsigned int totalImageSize;

// Indication for the consumer how many bytes are left to read
extern unsigned int remainingImageSize;

// Indication for the consumer how many bytes can be read from the buffer at once
extern int bufferedImageSize;

// The actual image data buffer for the consumer to consume
extern void *bufferedImageData;

extern FunctionHook method_hooks_gx2[];

extern u32 method_hooks_size_gx2;

extern volatile unsigned int method_calls_gx2[];

#ifdef __cplusplus
}
#endif