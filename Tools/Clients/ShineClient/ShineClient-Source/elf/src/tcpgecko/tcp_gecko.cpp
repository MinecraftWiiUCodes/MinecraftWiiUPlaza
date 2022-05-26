#pragma once

#include "tcp_gecko.h"
#include <iosuhax.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "../common/common.h"
#include <zlib.h> // Actually must be included before os_functions
#include "../dynamic_libs/os_functions.h"
#include <string.h>
#include <malloc.h>
#include "main.h"
#include "../dynamic_libs/socket_functions.h"
#include "../dynamic_libs/gx2_functions.h"
#include "../dynamic_libs/fs_functions.h"
#include "../utils/logger.h"
#include "hardware_breakpoints.h"
#include "linked_list.h"
#include "address.h"
#include "stack.h"
#include "pause.h"
#include "sd_ip_reader.h"
#include "../patcher/function_patcher_gx2.h"
#include "raw_assembly_cheats.h"
#include "sd_cheats.h"
#include "threads.h"
#include "software_breakpoints.h"

void *client;
void *commandBlock;
bool kernelCopyServiceStarted;

struct pygecko_bss_t {
	int error, line;
	void *thread;
	unsigned char stack[0x6F00];
};

/* TCP Gecko Commands */
#define COMMAND_WRITE_8 0x01
#define COMMAND_WRITE_16 0x02
#define COMMAND_WRITE_32 0x03
#define COMMAND_READ_MEMORY 0x04
#define COMMAND_READ_MEMORY_KERNEL 0x05
#define COMMAND_VALIDATE_ADDRESS_RANGE 0x06
#define COMMAND_MEMORY_DISASSEMBLE 0x08
#define COMMAND_READ_MEMORY_COMPRESSED 0x09 // TODO Remove command when done and integrate in read memory
#define COMMAND_KERNEL_WRITE 0x0B
#define COMMAND_KERNEL_READ 0x0C
#define COMMAND_TAKE_SCREEN_SHOT 0x0D // TODO Finish this
#define COMMAND_UPLOAD_MEMORY 0x41
#define COMMAND_SERVER_STATUS 0x50
#define COMMAND_GET_DATA_BUFFER_SIZE 0x51
#define COMMAND_READ_FILE 0x52
#define COMMAND_READ_DIRECTORY 0x53
#define COMMAND_REPLACE_FILE 0x54 // TODO Test this
#define COMMAND_GET_CODE_HANDLER_ADDRESS 0x55
#define COMMAND_READ_THREADS 0x56
#define COMMAND_ACCOUNT_IDENTIFIER 0x57
//#define COMMAND_WRITE_SCREEN 0x58 // TODO Exception DSI
#define COMMAND_FOLLOW_POINTER 0x60
#define COMMAND_REMOTE_PROCEDURE_CALL 0x70
#define COMMAND_GET_SYMBOL 0x71
#define COMMAND_MEMORY_SEARCH_32 0x72
#define COMMAND_ADVANCED_MEMORY_SEARCH 0x73
#define COMMAND_EXECUTE_ASSEMBLY 0x81
#define COMMAND_PAUSE_CONSOLE 0x82
#define COMMAND_RESUME_CONSOLE 0x83
#define COMMAND_IS_CONSOLE_PAUSED 0x84
#define COMMAND_SERVER_VERSION 0x99
#define COMMAND_GET_OS_VERSION 0x9A
#define COMMAND_SET_DATA_BREAKPOINT 0xA0
#define COMMAND_SET_INSTRUCTION_BREAKPOINT 0xA2
#define COMMAND_TOGGLE_BREAKPOINT 0xA5
#define COMMAND_REMOVE_ALL_BREAKPOINTS 0xA6
#define COMMAND_POKE_REGISTERS 0xA7
#define COMMAND_GET_STACK_TRACE 0xA8
#define COMMAND_GET_ENTRY_POINT_ADDRESS 0xB1
#define COMMAND_RUN_KERNEL_COPY_SERVICE 0xCD
#define COMMAND_IOSU_HAX_READ_FILE 0xD0
#define COMMAND_GET_VERSION_HASH 0xE0
#define COMMAND_PERSIST_ASSEMBLY 0xE1
#define COMMAND_CLEAR_ASSEMBLY 0xE2

#define CHECK_ERROR(cond) if (cond) { bss->line = __LINE__; goto error; }
#define errno2 (*__gh_errno_ptr())
#define MSG_DONT_WAIT 32
#define E_WOULD_BLOCK 6
//#define WRITE_SCREEN_MESSAGE_BUFFER_SIZE 100
#define SERVER_VERSION "06/03/2017"
#define ONLY_ZEROS_READ 0xB0
#define NON_ZEROS_READ 0xBD

#define VERSION_HASH 0x7FB223

/*ZEXTERN int ZEXPORT
deflateEnd OF((z_streamp
strm));
ZEXTERN int ZEXPORT
deflateInit OF((z_streamp
strm,
int level
));
ZEXTERN int ZEXPORT
deflate OF((z_streamp
strm,
int flush
));*/

// ########## Being socket_functions.h ############

static int recvwait(struct pygecko_bss_t *bss, int sock, unsigned char *buffer, int len) {
	int ret;
	while (len > 0) {
		ret = recv(sock, buffer, len, 0);
		CHECK_ERROR(ret < 0);
		len -= ret;
		buffer += ret;
	}
	return 0;

	error:
	bss->error = ret;
	return ret;
}

static int recvbyte(struct pygecko_bss_t *bss, int sock) {
	unsigned char buffer[1];
	int ret;

	ret = recvwait(bss, sock, buffer, 1);
	if (ret < 0) return ret;
	return buffer[0];
}

static int checkbyte(int sock) {
	unsigned char buffer[1];
	int ret;

	ret = recv(sock, buffer, 1, MSG_DONT_WAIT);
	if (ret < 0) return ret;
	if (ret == 0) return -1;
	return buffer[0];
}

static int sendwait(struct pygecko_bss_t *bss, int sock, unsigned char *buffer, int len) {
	int ret;
	while (len > 0) {
		ret = send(sock, buffer, len, 0);
		CHECK_ERROR(ret < 0);
		len -= ret;
		buffer += ret;
	}
	return 0;
	error:
	bss->error = ret;
	return ret;
}

static int sendByte(struct pygecko_bss_t *bss, int sock, unsigned char byte) {
	unsigned char buffer[1];

	buffer[0] = byte;
	return sendwait(bss, sock, buffer, 1);
}

unsigned int receiveString(struct pygecko_bss_t *bss,
						   int clientfd,
						   unsigned char *stringBuffer,
						   unsigned int bufferSize) {
	// Receive the string length
	unsigned char lengthBuffer[4] = {0};
	int ret = recvwait(bss, clientfd, lengthBuffer, sizeof(int));
	ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (string length)")
	unsigned int stringLength = ((unsigned int *) lengthBuffer)[0];

	if (stringLength <= bufferSize) {
		// Receive the actual string
		ret = recvwait(bss, clientfd, stringBuffer, stringLength);
		ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (string)")
	} else {
		OSFatal("String buffer size exceeded");
	}

	return stringLength;
}

// ########## End socket_functions.h ############

/*void writeScreen(char message[100], int secondsDelay) {
	// TODO Does nothing then crashes (in games)?
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);

	OSScreenPutFontEx(0, 14, 1, message);
	OSScreenPutFontEx(1, 14, 1, message);

	sleep(secondsDelay);

	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}*/


/* TODO
 		https://github.com/dimok789/ddd/blob/ca33ad1c759a0b67db33eedcf7fc4537198aad9c/src/discdumper.c#L667-L693
		int ret = FSBindMount(pClient, pCmd, metaDir, "/vol/meta", -1);
		FSBindUnmount(pClient, pCmd, metaDir, -1);
	*/
void considerInitializingFileSystem() {
	if (!client) {
		// Initialize the file system
		int status = FSInit();
		ASSERT_FUNCTION_SUCCEEDED(status, "FSInit")

		// Allocate the client
		client = malloc(FS_CLIENT_SIZE);
		ASSERT_ALLOCATED(client, "Client")

		// Register the client
		status = FSAddClientEx(client, 0, -1);
		ASSERT_FUNCTION_SUCCEEDED(status, "FSAddClientEx")

		// Allocate the command block
		commandBlock = malloc(FS_CMD_BLOCK_SIZE);
		ASSERT_ALLOCATED(commandBlock, "Command block")

		FSInitCmdBlock(commandBlock);
	}
}

#define ERROR_BUFFER_SIZE 150

void reportIllegalCommandByte(int commandByte) {
	char errorBuffer[ERROR_BUFFER_SIZE];
	__os_snprintf(errorBuffer, ERROR_BUFFER_SIZE,
				  "Illegal command byte received: 0x%02x\nServer Version: %s\nIf you see this,\nplease report this bug.",
				  commandByte, SERVER_VERSION);
	OSFatal(errorBuffer);
}

static int processCommands(struct pygecko_bss_t *bss, int clientfd) {
	int ret;

	// Hold the command and the data
	unsigned char buffer[1 + DATA_BUFFER_SIZE];

	// Run the RPC server
	while (true) {
		ret = checkbyte(clientfd);

		if (ret < 0) {
			CHECK_ERROR(errno2 != E_WOULD_BLOCK);
			GX2WaitForVsync();

			continue;
		}

		switch (ret) {
			case COMMAND_WRITE_8: {
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				CHECK_ERROR(ret < 0);

				char *destinationAddress = ((char **) buffer)[0];
				*destinationAddress = buffer[7];
				DCFlushRange(destinationAddress, 1);
				break;
			}
			case COMMAND_WRITE_16: {
				short *destinationAddress;
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				CHECK_ERROR(ret < 0)

				destinationAddress = ((short **) buffer)[0];
				*destinationAddress = ((short *) buffer)[3];
				DCFlushRange(destinationAddress, 2);
				break;
			}
			case COMMAND_WRITE_32: {
				int destinationAddress, value;
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				CHECK_ERROR(ret < 0)

				destinationAddress = ((int *) buffer)[0];
				value = ((int *) buffer)[1];

				kernelCopyData((unsigned char *) destinationAddress, (unsigned char *) &value, sizeof(int));
				break;
			}
			case COMMAND_READ_MEMORY: {
				const unsigned char *startingAddress, *endingAddress;
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				CHECK_ERROR(ret < 0)
				startingAddress = ((const unsigned char **) buffer)[0];
				endingAddress = ((const unsigned char **) buffer)[1];

				while (startingAddress != endingAddress) {
					int length = (int) (endingAddress - startingAddress);

					// Do not smash the buffer
					if (length > DATA_BUFFER_SIZE) {
						length = DATA_BUFFER_SIZE;
					}

					// Figure out if all bytes are zero to possibly avoid sending them
					int rangeIterationIndex = 0;
					for (; rangeIterationIndex < length; rangeIterationIndex++) {
						int character = startingAddress[rangeIterationIndex];

						if (character != 0) {
							break;
						}
					}

					if (rangeIterationIndex == length) {
						// No need to send all zero bytes for performance
						ret = sendByte(bss, clientfd, ONLY_ZEROS_READ);
						CHECK_ERROR(ret < 0)
					} else {
						// TODO Compression of ptr, sending of status, compressed size and data, length: 1 + 4 + len(data)
						buffer[0] = NON_ZEROS_READ;
						memcpy(buffer + 1, startingAddress, length);
						ret = sendwait(bss, clientfd, buffer, length + 1);
						CHECK_ERROR(ret < 0)
					}

					/* 	No exit condition.
						We reconnect client-sided instead as a hacky work-around
						 to gain a little more performance by avoiding the very rare search canceling
					 */

					startingAddress += length;
				}

				break;
			}
			case COMMAND_READ_MEMORY_KERNEL: {
				const unsigned char *startingAddress, *endingAddress, *useKernRead;
				ret = recvwait(bss, clientfd, buffer, 3 * sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (receiving data)")

				int bufferIndex = 0;
				startingAddress = ((const unsigned char **) buffer)[bufferIndex++];
				endingAddress = ((const unsigned char **) buffer)[bufferIndex++];
				useKernRead = ((const unsigned char **) buffer)[bufferIndex];

				while (startingAddress != endingAddress) {
					int length = (int) (endingAddress - startingAddress);

					// Do not smash the buffer
					if (length > DATA_BUFFER_SIZE) {
						length = DATA_BUFFER_SIZE;
					}

					// Figure out if all bytes are zero to possibly avoid sending them
					int rangeIterationIndex = 0;
					for (; rangeIterationIndex < length; rangeIterationIndex++) {
						int character = useKernRead ? readKernelMemory(startingAddress + rangeIterationIndex)
													: startingAddress[rangeIterationIndex];
						if (character != 0) {
							break;
						}
					}

					if (rangeIterationIndex == length) {
						// No need to send all zero bytes for performance
						ret = sendByte(bss, clientfd, ONLY_ZEROS_READ);
						CHECK_ERROR(ret < 0)
					} else {
						buffer[0] = NON_ZEROS_READ;

						if (useKernRead) {
							for (int offset = 0; offset < length; offset += 4) {
								*((int *) (buffer + 1) + offset / 4) = readKernelMemory(startingAddress + offset);
							}
						} else {
							memcpy(buffer + 1, startingAddress, length);
						}

						ret = sendwait(bss, clientfd, buffer, length + 1);
						CHECK_ERROR(ret < 0)
					}

					/* 	No exit condition.
						We reconnect client-sided instead as a hacky work-around
						 to gain a little more performance by avoiding the very rare search canceling
					 */

					startingAddress += length;
				}
				break;

/*				const unsigned char *startingAddress, *endingAddress, *useKernRead;
				ret = recvwait(bss, clientfd, buffer, 3 * sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (receiving data)")

				int bufferIndex = 0;
				startingAddress = ((const unsigned char **) buffer)[bufferIndex++];
				endingAddress = ((const unsigned char **) buffer)[bufferIndex++];
				useKernRead = ((const unsigned char **) buffer)[bufferIndex];

				while (startingAddress != endingAddress) {
					log_printf("Reading memory from %08x to %08x with kernel %i\n", startingAddress, endingAddress,
							   useKernRead);

					unsigned int length = (unsigned int) (endingAddress - startingAddress);

					// Do not smash the buffer
					if (length > DATA_BUFFER_SIZE) {
						length = DATA_BUFFER_SIZE;
					}

					// Figure out if all bytes are zero to possibly avoid sending them
					log_print("Checking for all zero bytes...\n");
					unsigned int rangeIterationIndex = 0;
					for (; rangeIterationIndex < length; rangeIterationIndex++) {
						int character = useKernRead ? readKernelMemory(startingAddress + rangeIterationIndex)
													: startingAddress[rangeIterationIndex];
						if (character != 0) {
							break;
						}
					}

					log_print("Preparing to send...\n");
					if (rangeIterationIndex == length) {
						// No need to send all zero bytes for performance
						log_print("All zero...\n");
						ret = sendByte(bss, clientfd, ONLY_ZEROS_READ);
						ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (only zero bytes read byte)")
						log_print("Sent!\n");
					} else {
						// Send the real bytes now
						log_print("Real bytes...\n");
						buffer[0] = NON_ZEROS_READ;

						if (useKernRead) {
							kernelCopy(buffer + 1, (unsigned char *) startingAddress, length);
							for (unsigned int offset = 0; offset < length; offset += sizeof(int)) {
								*((int *) (buffer + 1) + offset / sizeof(int)) = readKernelMemory(
										startingAddress + offset);
								log_printf("Offset: %x\n", offset);
							}

							log_print("Done kernel reading!\n");
						} else {
							log_print("Memory copying...\n");
							memcpy(buffer + 1, startingAddress, length);
							log_print("Done copying!\n");
						}

						log_print("Sending everything...\n");
						ret = sendwait(bss, clientfd, buffer, length + 1);
						ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (read bytes buffer)")
						log_print("Sent!\n");
					}

					startingAddress += length;
				}

				log_print("Done reading...\n");

				break;*/
			}
			case COMMAND_VALIDATE_ADDRESS_RANGE: {
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				CHECK_ERROR(ret < 0)

				// Retrieve the data
				int startingAddress = ((int *) buffer)[0];
				int endingAddress = ((int *) buffer)[1];

				int isAddressRangeValid = validateAddressRange(startingAddress, endingAddress);

				sendByte(bss, clientfd, (unsigned char) isAddressRangeValid);
				break;
			}
				/*case COMMAND_DISASSEMBLE_RANGE: {
					// Receive the starting, ending address and the disassembler options
					ret = recvwait(bss, clientfd, buffer, 4 + 4 + 4);
					CHECK_ERROR(ret < 0)
					void *startingAddress = ((void **) buffer)[0];
					void *endingAddress = ((void **) buffer)[1];
					int disassemblerOptions = ((int *) buffer)[2];

					// Disassemble
					DisassemblePPCRange(startingAddress, endingAddress, formatDisassembled, OSGetSymbolName,
										(u32) disassemblerOptions);

					// Send the disassembler buffer size
					int length = DISASSEMBLER_BUFFER_SIZE;
					ret = sendwait(bss, clientfd, &length, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (disassembler buffer size)")

					// Send the data
					ret = sendwait(bss, clientfd, disassemblerBufferPointer, length);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (disassembler buffer)")

					// Place the pointer back to the beginning
					disassemblerBuffer = (char *) disassemblerBufferPointer;

					break;
				}*/
			case COMMAND_MEMORY_DISASSEMBLE: {
				// Receive the starting address, ending address and disassembler options
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 3);
				CHECK_ERROR(ret < 0)
				int startingAddress = ((int *) buffer)[0];
				int endingAddress = ((int *) buffer)[1];
				int disassemblerOptions = ((int *) buffer)[2];

				int currentAddress = startingAddress;
				int bufferSize = PPC_DISASM_MAX_BUFFER;
				int integerSize = 4;

				// Disassemble everything
				while (currentAddress < endingAddress) {
					int currentIntegerIndex = 0;

					while ((currentIntegerIndex < (DATA_BUFFER_SIZE / integerSize))
						   && (currentAddress < endingAddress)) {
						int value = *(int *) currentAddress;
						((int *) buffer)[currentIntegerIndex++] = value;
						char *opCodeBuffer = (char *) malloc(bufferSize);
						bool status = DisassemblePPCOpcode((u32 *) currentAddress, opCodeBuffer, (u32) bufferSize,
														   OSGetSymbolName,
														   (u32) disassemblerOptions);

						((int *) buffer)[currentIntegerIndex++] = status;

						if (status == 1) {
							// Send the length of the opCode buffer string
							int length = strlen(opCodeBuffer);
							((int *) buffer)[currentIntegerIndex++] = length;

							// Send the opCode buffer itself
							memcpy(buffer + (currentIntegerIndex * integerSize), opCodeBuffer, length);
							currentIntegerIndex += (roundUpToAligned(length) / integerSize);
						}

						free(opCodeBuffer);
						currentAddress += integerSize;
					}

					int bytesToSend = currentIntegerIndex * integerSize;
					ret = sendwait(bss, clientfd, (unsigned char *) &bytesToSend, sizeof(int));
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (Buffer size)")

					// VALUE(4)|STATUS(4)|LENGTH(4)|DISASSEMBLED(LENGTH)
					ret = sendwait(bss, clientfd, buffer, bytesToSend);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (Buffer)")
				}

				int bytesToSend = 0;
				ret = sendwait(bss, clientfd, (unsigned char *) &bytesToSend, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (No more bytes)")

				break;
			}
				/*case COMMAND_READ_MEMORY_COMPRESSED: {
					// Receive the starting address and length
					ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
					CHECK_ERROR(ret < 0)
					int startingAddress = ((int *) buffer)[0];
					unsigned int inputLength = ((unsigned int *) buffer)[1];

					z_stream stream;
					memset(&stream, 0, sizeof(stream));
					stream.zalloc = Z_NULL;
					stream.zfree = Z_NULL;
					stream.opaque = Z_NULL;

					// Initialize the stream struct
					ret = deflateInit(&stream, Z_BEST_COMPRESSION);
					ASSERT_INTEGER(ret, Z_OK, "deflateInit")

					// Supply the data
					stream.avail_in = inputLength;
					stream.next_in = (Bytef *) startingAddress;
					stream.avail_out = DATA_BUFFER_SIZE;
					void *outputBuffer = (void *) (&buffer + 4);
					stream.next_out = (Bytef *) outputBuffer;

					// Deflate
					ret = deflate(&stream, Z_FINISH);
					ASSERT_INTEGER(ret, Z_OK, "deflate");

					// Finish
					ret = deflateEnd(&stream);
					ASSERT_INTEGER(ret, Z_OK, "deflateEnd");

					// Send the compressed buffer size and content
					int deflatedSize = stream.total_out;
					((int *) buffer)[0] = deflatedSize;
					ret = sendwait(bss, clientfd, buffer, 4 + deflatedSize);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (Compressed data)")

					break;*/

				// https://www.gamedev.net/resources/_/technical/game-programming/in-memory-data-compression-and-decompression-r2279
				/*

				// Setup compressed buffer
				unsigned int compressedBufferSize = length * 2;
				void *compressedBuffer = (void *) OSAllocFromSystem(compressedBufferSize, 0x4);
				ASSERT_ALLOCATED(compressedBuffer, "Compressed buffer")

				unsigned int zlib_handle;
				OSDynLoad_Acquire("zlib125.rpl", (u32 *) &zlib_handle);
				int (*compress2)(char *, int *, const char *, int, int);
				OSDynLoad_FindExport((u32) zlib_handle, 0, "compress2", &compress2);

				int destinationBufferSize;
				int status = compress2((char *) compressedBuffer, &destinationBufferSize,
									   (const char *) rawBuffer, length, Z_DEFAULT_COMPRESSION);

				ret = sendwait(bss, clientfd, &status, 4);
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (status)")

				if (status == Z_OK) {
					// Send the compressed buffer size and content
					((int *) buffer)[0] = destinationBufferSize;
					memcpy(buffer + 4, compressedBuffer, destinationBufferSize);

					ret = sendwait(bss, clientfd, buffer, 4 + destinationBufferSize);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (Compressed data)")
				}

				free(rawBuffer);
				OSFreeToSystem(compressedBuffer);

				break;*/
				// }
			case COMMAND_KERNEL_WRITE: {
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				CHECK_ERROR(ret < 0)

				void *address = ((void **) buffer)[0];
				void *value = ((void **) buffer)[1];

				writeKernelMemory(address, (uint32_t) value);
				break;
			}
			case COMMAND_KERNEL_READ: {
				ret = recvwait(bss, clientfd, buffer, sizeof(int));
				CHECK_ERROR(ret < 0)

				void *address = ((void **) buffer)[0];
				void *value = (void *) readKernelMemory(address);

				*(void **) buffer = value;
				sendwait(bss, clientfd, buffer, sizeof(int));
				break;
			}
			case COMMAND_TAKE_SCREEN_SHOT: {
				// Tell the hook to dump the screen shot now
				shouldTakeScreenShot = true;

				// Tell the client the size of the upcoming image
				ret = sendwait(bss, clientfd, (unsigned char *) &totalImageSize, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (total image size)")

				// Keep sending the image data
				while (remainingImageSize > 0) {
					int bufferPosition = 0;

					// Fill the buffer till it is full
					while (bufferPosition <= DATA_BUFFER_SIZE) {
						// Wait for data to be available
						while (bufferedImageSize == 0) {
							usleep(WAITING_TIME_MILLISECONDS);
						}

						memcpy(buffer + bufferPosition, bufferedImageData, bufferedImageSize);
						bufferPosition += bufferedImageSize;
						bufferedImageSize = 0;
					}

					// Send the size of the current chunk
					ret = sendwait(bss, clientfd, (unsigned char *) &bufferPosition, sizeof(int));
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (image data chunk size)")

					// Send the image data itself
					ret = sendwait(bss, clientfd, buffer, bufferPosition);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (image data)")
				}

				/*GX2ColorBuffer colorBuffer;
				// TODO Initialize colorBuffer!
				GX2Surface surface = colorBuffer.surface;
				void *image_data = surface.image_data;
				u32 image_size = surface.image_size;

				// Send the image size so that the client knows how much to read
				ret = sendwait(bss, clientfd, (unsigned char *) &image_size, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (image size)")

				unsigned int imageBytesSent = 0;
				while (imageBytesSent < image_size) {
					int length = image_size - imageBytesSent;

					// Do not smash the buffer
					if (length > DATA_BUFFER_SIZE) {
						length = DATA_BUFFER_SIZE;
					}

					// Send the image bytes
					memcpy(buffer, image_data, length);
					ret = sendwait(bss, clientfd, buffer, length);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (image bytes)")

					imageBytesSent += length;
				}*/

				break;
			}
			case COMMAND_UPLOAD_MEMORY: {
				// Receive the starting and ending addresses
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				CHECK_ERROR(ret < 0)
				unsigned char *currentAddress = ((unsigned char **) buffer)[0];
				unsigned char *endAddress = ((unsigned char **) buffer)[1];

				while (currentAddress != endAddress) {
					int length;

					length = (int) (endAddress - currentAddress);
					if (length > DATA_BUFFER_SIZE) {
						length = DATA_BUFFER_SIZE;
					}

					ret = recvwait(bss, clientfd, buffer, length);
					CHECK_ERROR(ret < 0)
					
					kernelCopyData(currentAddress, buffer, (unsigned int) length);

					currentAddress += length;
				}
				break;
			}
			case COMMAND_GET_DATA_BUFFER_SIZE: {
				log_printf("COMMAND_GET_DATA_BUFFER_SIZE...\n");
				((int *) buffer)[0] = DATA_BUFFER_SIZE;
				log_printf("Sending buffer size...\n");
				ret = sendwait(bss, clientfd, buffer, sizeof(int));
				log_printf("Sent: %i\n", ret);
				CHECK_ERROR(ret < 0)

				break;
			}
			case COMMAND_READ_FILE: {
				char file_path[FS_MAX_FULLPATH_SIZE] = {0};
				receiveString(bss, clientfd, (unsigned char *) file_path, FS_MAX_FULLPATH_SIZE);

				considerInitializingFileSystem();

				int handle;
				int status = FSOpenFile(client, commandBlock, file_path, "r", &handle, FS_RET_ALL_ERROR);

				if (status == FS_STATUS_OK) {
					// Send the OK status
					((int *) buffer)[0] = status;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (OK status)")

					// Retrieve the file statistics
					FSStat stat;
					ret = FSGetStatFile(client, commandBlock, handle, &stat, FS_RET_ALL_ERROR);
					ASSERT_FUNCTION_SUCCEEDED(ret, "FSGetStatFile")

					// Send the total bytes count
					int totalBytes = (int) stat.size;
					((int *) buffer)[0] = totalBytes;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (total bytes)")

					// Allocate the file bytes buffer
					unsigned int file_buffer_size = 0x2000;
					char *fileBuffer = (char *) OSAllocFromSystem(file_buffer_size, FS_IO_BUFFER_ALIGN);
					ASSERT_ALLOCATED(fileBuffer, "File buffer")

					int totalBytesRead = 0;
					while (totalBytesRead < totalBytes) {
						int bytesRead = FSReadFile(client, commandBlock, fileBuffer, 1, file_buffer_size,
												   handle, 0, FS_RET_ALL_ERROR);
						ASSERT_FUNCTION_SUCCEEDED(bytesRead, "FSReadFile")

						// Send file bytes
						ret = sendwait(bss, clientfd, (unsigned char *) fileBuffer, bytesRead);
						ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (file buffer)")

						totalBytesRead += bytesRead;
					}

					ret = FSCloseFile(client, commandBlock, handle, FS_RET_ALL_ERROR);
					ASSERT_FUNCTION_SUCCEEDED(ret, "FSCloseFile")

					OSFreeToSystem(fileBuffer);
				} else {
					// Send the error status
					((int *) buffer)[0] = status;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (error status)")
				}

				break;
			}
			case COMMAND_READ_DIRECTORY: {
				char directory_path[FS_MAX_FULLPATH_SIZE] = {0};
				receiveString(bss, clientfd, (unsigned char *) directory_path, FS_MAX_FULLPATH_SIZE);

				considerInitializingFileSystem();

				s32 handle;
				FSDirEntry entry;

				ret = FSOpenDir(client, commandBlock, directory_path, &handle, FS_RET_ALL_ERROR);

				if (ret == FS_STATUS_OK) {
					// Send the success status
					((int *) buffer)[0] = ret;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (success status)")

					int entrySize = sizeof(FSDirEntry);

					// Read every entry in the given directory
					while (FSReadDir(client, commandBlock, handle, &entry, -1) == FS_STATUS_OK) {
						// Let the client know how much data is going to be sent (even though this is constant)
						((int *) buffer)[0] = entrySize;
						ret = sendwait(bss, clientfd, buffer, 4);
						ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (data coming)")

						// Send the struct
						ret = sendwait(bss, clientfd, (unsigned char *) &entry, entrySize);
						ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (directory entry)")
					}

					// No more data will be sent, hence a 0 byte
					((int *) buffer)[0] = 0;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (no more data)")

					// Done, close the directory also
					ret = FSCloseDir(client, commandBlock, handle, FS_RET_ALL_ERROR);
					ASSERT_FUNCTION_SUCCEEDED(ret, "FSCloseDir")
				} else {
					// Send the status
					((int *) buffer)[0] = ret;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (error status)")
				}

				break;
			}
			case COMMAND_REPLACE_FILE: {
				// TODO FSOpenFile ACCESS_ERROR

				// Receive the file path
				char file_path[FS_MAX_FULLPATH_SIZE] = {0};
				receiveString(bss, clientfd, (unsigned char *) file_path, FS_MAX_FULLPATH_SIZE);

				considerInitializingFileSystem();

				// Create an empty file for writing. Its contents will be erased
				int handle;
				int status = FSOpenFile(client, commandBlock, file_path, "w", &handle, FS_RET_ALL_ERROR);

				if (status == FS_STATUS_OK) {
					// Send the OK status
					((int *) buffer)[0] = status;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (OK status)")

					// Set the file handle position to the beginning
					ret = FSSetPosFile(client, commandBlock, handle, 0, FS_RET_ALL_ERROR);
					ASSERT_FUNCTION_SUCCEEDED(ret, "FSSetPosFile")

					// Allocate the file bytes buffer
					unsigned int file_buffer_size = 0x2000;
					char *fileBuffer = (char *) OSAllocFromSystem(file_buffer_size, FS_IO_BUFFER_ALIGN);
					ASSERT_ALLOCATED(fileBuffer, "File buffer")

					// Send the maximum file buffer size
					ret = sendwait(bss, clientfd, (unsigned char *) &file_buffer_size, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (maximum file buffer size)")

					while (true) {
						// Receive the data bytes length
						unsigned int dataLength;
						ret = recvwait(bss, clientfd, (unsigned char *) &dataLength, 4);
						ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (File bytes length)")
						ASSERT_MAXIMUM_HOLDS(file_buffer_size, dataLength, "File buffer overrun attempted")

						if (dataLength > 0) {
							// Receive the data
							ret = recvwait(bss, clientfd, (unsigned char *) fileBuffer, dataLength);
							ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (File buffer)")

							// Write the data (and advance file handle position implicitly)
							ret = FSWriteFile(client, commandBlock, fileBuffer, 1,
											  dataLength, handle, 0, FS_RET_ALL_ERROR);
							ASSERT_FUNCTION_SUCCEEDED(ret, "FSWriteFile")
						} else {
							// Done with receiving the new file
							break;
						}
					}

					// Flush the file back
					// ret = FSFlushFile(client, commandBlock, handle, FS_RET_ALL_ERROR);
					// CHECK_FUNCTION_FAILED(ret, "FSFlushFile")

					// Close the file
					ret = FSCloseFile(client, commandBlock, handle, FS_RET_ALL_ERROR);
					ASSERT_FUNCTION_SUCCEEDED(ret, "FSCloseFile")

					// Free the file buffer
					OSFreeToSystem(fileBuffer);
				} else {
					// Send the status
					((int *) buffer)[0] = status;
					ret = sendwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (status)")
				}

				break;
			}
			case COMMAND_IOSU_HAX_READ_FILE: {
				/*log_print("COMMAND_IOSUHAX_READ_FILE");

				// TODO Crashes console on this call
				int returnValue = IOSUHAX_Open(NULL);
				log_print("IOSUHAX_Open Done");
				log_printf("IOSUHAX_Open: %i", returnValue);

				if (returnValue < 0) {
					goto IOSUHAX_OPEN_FAILED;
				}

				int fileSystemFileDescriptor = IOSUHAX_FSA_Open();
				log_printf("IOSUHAX_FSA_Open: %i", fileSystemFileDescriptor);

				if (fileSystemFileDescriptor < 0) {
					goto IOSUHAX_FSA_OPEN_FAILED;
				}

				int fileDescriptor;
				const char *filePath = "/vol/storage_usb/usr/title/0005000e/1010ed00/content/audio/stream/pBGM_GBA_CHEESELAND_F.bfstm";
				returnValue = IOSUHAX_FSA_OpenFile(fileSystemFileDescriptor, filePath, "rb", &fileDescriptor);
				log_printf("IOSUHAX_FSA_OpenFile: %i", returnValue);

				if (returnValue < 0) {
					goto IOSUHAX_OPEN_FILE_FAILED;
				}

				fileStat_s fileStat;
				returnValue = IOSUHAX_FSA_StatFile(fileSystemFileDescriptor, fileDescriptor, &fileStat);
				log_printf("IOSUHAX_FSA_StatFile: %i", returnValue);

				if (returnValue < 0) {
					goto IOSUHAX_READ_FILE_FAILED_CLOSE;
				}

				void *fileBuffer = MEMBucket_alloc(fileStat.size, 4);
				log_printf("File Buffer: %p", fileBuffer);

				if (!fileBuffer) {
					goto IOSUHAX_READ_FILE_FAILED_CLOSE;
				}

				size_t totalBytesRead = 0;
				while (totalBytesRead < fileStat.size) {
					size_t remainingBytes = fileStat.size - totalBytesRead;
					int bytesRead = IOSUHAX_FSA_ReadFile(fileSystemFileDescriptor,
														 fileBuffer + totalBytesRead,
														 0x01,
														 remainingBytes,
														 fileDescriptor,
														 0);
					log_printf("IOSUHAX_FSA_ReadFile: %i", bytesRead);

					if (bytesRead <= 0) {
						goto IOSUHAX_READ_FILE_FAILED_CLOSE;
					} else {
						totalBytesRead += bytesRead;
					}
				}

				log_printf("Bytes read: %i", totalBytesRead);

				IOSUHAX_READ_FILE_FAILED_CLOSE:

				returnValue = IOSUHAX_FSA_CloseFile(fileSystemFileDescriptor, fileDescriptor);
				log_printf("IOSUHAX_FSA_CloseFile: %i", returnValue);

				IOSUHAX_OPEN_FILE_FAILED:

				returnValue = IOSUHAX_FSA_Close(fileSystemFileDescriptor);
				log_printf("IOSUHAX_FSA_Close: %i", returnValue);

				IOSUHAX_FSA_OPEN_FAILED:

				returnValue = IOSUHAX_Close();
				log_printf("IOSUHAX_Close: %i", returnValue);

				IOSUHAX_OPEN_FAILED:*/

				break;
			}
			case COMMAND_GET_VERSION_HASH: {
				((int *) buffer)[0] = VERSION_HASH;
				ret = sendwait(bss, clientfd, buffer, 4);

				break;
			}
			case COMMAND_GET_CODE_HANDLER_ADDRESS: {
				((int *) buffer)[0] = CODE_HANDLER_INSTALL_ADDRESS;
				ret = sendwait(bss, clientfd, buffer, 4);
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (code handler address)")

				break;
			}
			case COMMAND_READ_THREADS: {
				struct node *threads = getAllThreads();
				int threadCount = length(threads);
				log_printf("Thread Count: %i\n", threadCount);

				// Send the thread count
				log_print("Sending thread count...\n");
				((int *) buffer)[0] = threadCount;
				ret = sendwait(bss, clientfd, buffer, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (thread count)");

				// Send the thread addresses and data
				struct node *currentThread = threads;
				while (currentThread != NULL) {
					int data = (int) currentThread->data;
					log_printf("Thread data: %08x\n", data);
					((int *) buffer)[0] = (int) currentThread->data;
					memcpy(buffer + sizeof(int), currentThread->data, THREAD_SIZE);
					log_print("Sending node...\n");
					ret = sendwait(bss, clientfd, buffer, sizeof(int) + THREAD_SIZE);
					ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (thread address and data)")

					currentThread = currentThread->next;
				}

				destroy(threads);

				break;
			}
			case COMMAND_ACCOUNT_IDENTIFIER: {
				// Acquire the RPL
				u32 nn_act_handle;
				OSDynLoad_Acquire("nn_act.rpl", &nn_act_handle);

				// Acquire the functions via their mangled file names
				int (*nn_act_Initialize)(void);
				OSDynLoad_FindExport(nn_act_handle, 0, "Initialize__Q2_2nn3actFv", &nn_act_Initialize);
				ASSERT_ALLOCATED(nn_act_Initialize, "nn_act_Initialize")
				unsigned char (*nn_act_GetSlotNo)(void);
				OSDynLoad_FindExport(nn_act_handle, 0, "GetSlotNo__Q2_2nn3actFv", &nn_act_GetSlotNo);
				ASSERT_ALLOCATED(nn_act_GetSlotNo, "nn_act_GetSlotNo")
				unsigned int (*nn_act_GetPersistentIdEx)(unsigned char);
				OSDynLoad_FindExport(nn_act_handle, 0, "GetPersistentIdEx__Q2_2nn3actFUc", &nn_act_GetPersistentIdEx);
				ASSERT_ALLOCATED(nn_act_GetPersistentIdEx, "nn_act_GetPersistentIdEx")
				int (*nn_act_Finalize)(void);
				OSDynLoad_FindExport(nn_act_handle, 0, "Finalize__Q2_2nn3actFv", &nn_act_Finalize);
				ASSERT_ALLOCATED(nn_act_Finalize, "nn_act_Finalize")

				// Get the identifier
				ret = nn_act_Initialize();
				// ASSERT_INTEGER(ret, 1, "Initializing account library");
				unsigned char slotNumber = nn_act_GetSlotNo();
				unsigned int persistentIdentifier = nn_act_GetPersistentIdEx(slotNumber);
				ret = nn_act_Finalize();
				ASSERT_FUNCTION_SUCCEEDED(ret, "nn_act_Finalize");

				// Send it
				ret = sendwait(bss, clientfd, (unsigned char *) &persistentIdentifier, 4);
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (persistent identifier)")

				break;
			}/*
				case COMMAND_WRITE_SCREEN: {
					char message[WRITE_SCREEN_MESSAGE_BUFFER_SIZE];
					ret = recvwait(bss, clientfd, buffer, 4);
					ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (write screen seconds)")
					int seconds = ((int *) buffer)[0];
					receiveString(bss, clientfd, message, WRITE_SCREEN_MESSAGE_BUFFER_SIZE);
					writeScreen(message, seconds);

					break;
				}*/
			case COMMAND_FOLLOW_POINTER: {
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 2);
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (Pointer address and offsets count)")

				// Retrieve the pointer address and amount of offsets
				int baseAddress = ((int *) buffer)[0];
				int offsetsCount = ((int *) buffer)[1];

				// Receive the offsets
				ret = recvwait(bss, clientfd, buffer, offsetsCount * sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (offsets)")
				int offsets[offsetsCount];
				int offsetIndex = 0;
				for (; offsetIndex < offsetsCount; offsetIndex++) {
					offsets[offsetIndex] = ((int *) buffer)[offsetIndex];
				}

				int destinationAddress = baseAddress;

#define INVALID_ADDRESS -1

				if ((bool) OSIsAddressValid((const void *) destinationAddress)) {
					// Apply pointer offsets
					for (offsetIndex = 0; offsetIndex < offsetsCount; offsetIndex++) {
						int pointerValue = *(int *) destinationAddress;
						int offset = offsets[offsetIndex];
						destinationAddress = pointerValue + offset;

						// Validate the pointer address
						bool isValidDestinationAddress = (bool) OSIsAddressValid((const void *) destinationAddress);

						// Bail out if invalid
						if (!isValidDestinationAddress) {
							destinationAddress = INVALID_ADDRESS;

							break;
						}
					}
				} else if (offsetsCount > 0) {
					// Following pointers failed
					destinationAddress = INVALID_ADDRESS;
				}

				// Return the destination address
				((int *) buffer)[0] = destinationAddress;
				ret = sendwait(bss, clientfd, buffer, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (destination address)")

				break;
			}
			case COMMAND_SERVER_STATUS: {
				ret = sendByte(bss, clientfd, 1);
				CHECK_ERROR(ret < 0)
				break;
			}
			case COMMAND_REMOTE_PROCEDURE_CALL: {
				int r3, r4, r5, r6, r7, r8, r9, r10;

				log_print("Receiving RPC information...\n");
				ret = recvwait(bss, clientfd, buffer, sizeof(int) + 8 * sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "revcwait() Receiving RPC information")
				log_print("RPC information received...\n");

				long long (*function)(int, int, int, int, int, int, int, int);
				function = (long long int (*)(int, int, int, int, int, int, int, int)) ((void **) buffer)[0];
				r3 = ((int *) buffer)[1];
				r4 = ((int *) buffer)[2];
				r5 = ((int *) buffer)[3];
				r6 = ((int *) buffer)[4];
				r7 = ((int *) buffer)[5];
				r8 = ((int *) buffer)[6];
				r9 = ((int *) buffer)[7];
				r10 = ((int *) buffer)[8];

				log_print("Calling function...\n");
				long long result = function(r3, r4, r5, r6, r7, r8, r9, r10);
				log_printf("Function successfully called with return value: 0x%08x 0x%08x\n", (int) (result >> 32),
						   (int) result);

				log_print("Sending result...\n");
				((long long *) buffer)[0] = result;
				ret = sendwait(bss, clientfd, buffer, sizeof(long long));
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait() Sending return value")
				log_print("Result successfully sent...\n");

				break;
			}
			case COMMAND_GET_SYMBOL: {
				int size = recvbyte(bss, clientfd);
				CHECK_ERROR(size < 0)

				ret = recvwait(bss, clientfd, buffer, size);
				CHECK_ERROR(ret < 0)

				/* Identify the RPL name and symbol name */
				char *rplName = (char *) &((int *) buffer)[2];
				char *symbolName = (char *) (&buffer[0] + ((int *) buffer)[1]);

				/* Get the symbol and store it in the buffer */
				u32 module_handle, function_address;
				OSDynLoad_Acquire(rplName, &module_handle);

				char data = (char) recvbyte(bss, clientfd);
				OSDynLoad_FindExport(module_handle, data, symbolName, &function_address);

				((int *) buffer)[0] = (int) function_address;
				ret = sendwait(bss, clientfd, buffer, 4);
				CHECK_ERROR(ret < 0)

				break;
			}
			case COMMAND_MEMORY_SEARCH_32: {
				ret = recvwait(bss, clientfd, buffer, sizeof(int) * 3);
				CHECK_ERROR(ret < 0);
				int address = ((int *) buffer)[0];
				int value = ((int *) buffer)[1];
				int length = ((int *) buffer)[2];
				int index;
				int foundAddress = 0;
				for (index = address; index < address + length; index += sizeof(int)) {
					if (*(int *) index == value) {
						foundAddress = index;
						break;
					}
				}

				((int *) buffer)[0] = foundAddress;
				ret = sendwait(bss, clientfd, buffer, sizeof(int));
				CHECK_ERROR(ret < 0)

				break;
			}
			case COMMAND_ADVANCED_MEMORY_SEARCH: {
				// Receive the initial data
				ret = recvwait(bss, clientfd, buffer, 6 * sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (memory search information)")
				int bufferIndex = 0;
				int startingAddress = ((int *) buffer)[bufferIndex++];
				int length = ((int *) buffer)[bufferIndex++];
				int kernelRead = ((int *) buffer)[bufferIndex++];
				int resultsLimit = ((int *) buffer)[bufferIndex++];
				int aligned = ((int *) buffer)[bufferIndex++];
				int searchBytesCount = ((int *) buffer)[bufferIndex];

				// Receive the search bytes
				char searchBytes[searchBytesCount];
				ret = recvwait(bss, clientfd, (unsigned char *) searchBytes, searchBytesCount);
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (memory search bytes)")

				int iterationIncrement = aligned ? searchBytesCount : 1;
				int searchBytesOccurrences = 0;

				// Perform the bytes search and collect the results
				for (int currentAddress = startingAddress;
					 currentAddress < startingAddress + length;
					 currentAddress += iterationIncrement) {

					int comparisonResult;

					if (kernelRead) {
						comparisonResult = kernelMemoryCompare((char *) currentAddress,
															   searchBytes, searchBytesCount);
					} else {
						comparisonResult = memoryCompare((void *) currentAddress,
														 searchBytes, (size_t) searchBytesCount);
					}
					if (comparisonResult == 0) {
						// Search bytes have been found, add the addresses to the return buffer
						((int *) buffer)[1 + searchBytesOccurrences] = currentAddress;
						searchBytesOccurrences++;

						if ((resultsLimit == searchBytesOccurrences)
							|| (searchBytesOccurrences == ((DATA_BUFFER_SIZE / 4) - 1))) {
							// We bail out
							break;
						}
					}
				}

				((int *) buffer)[0] = searchBytesOccurrences * 4;
				ret = sendwait(bss, clientfd, buffer, 4 + (searchBytesOccurrences * 4));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (Sending search bytes occurrences)")

				break;
			}
			case COMMAND_EXECUTE_ASSEMBLY: {
				receiveString(bss, clientfd, (unsigned char *) buffer, DATA_BUFFER_SIZE);
				executeAssembly(buffer, DATA_BUFFER_SIZE);

				break;
			}
			case COMMAND_PAUSE_CONSOLE: {
				writeConsoleState(PAUSED);

				break;
			}
			case COMMAND_RESUME_CONSOLE: {
				writeConsoleState(RUNNING);

				break;
			}
			case COMMAND_IS_CONSOLE_PAUSED: {
				bool paused = isConsolePaused();
				log_printf("Paused: %d\n", paused);
				ret = sendByte(bss, clientfd, (unsigned char) paused);
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendByte (sending paused console status)")

				break;
			}
			case COMMAND_SERVER_VERSION: {
				char versionBuffer[50];
				strcpy(versionBuffer, SERVER_VERSION);
				int versionLength = strlen(versionBuffer);
				((int *) buffer)[0] = versionLength;
				memcpy(buffer + sizeof(int), versionBuffer, versionLength);

				// Send the length and the version string
				ret = sendwait(bss, clientfd, buffer, sizeof(int) + versionLength);
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (server version)");

				break;
			}
			case COMMAND_GET_OS_VERSION: {
				((int *) buffer)[0] = (int) OS_FIRMWARE;
				ret = sendwait(bss, clientfd, buffer, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (OS version)");

				break;
			}
			case COMMAND_SET_DATA_BREAKPOINT: {
				// Read the data from the client
				ret = recvwait(bss, clientfd, buffer, sizeof(int) + sizeof(bool) * 2);
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (data breakpoint)");

				// Parse the data and set the breakpoint
				int bufferIndex = 0;
				unsigned int address = ((unsigned int *) buffer)[bufferIndex];
				bufferIndex += sizeof(int);
				bool read = buffer[bufferIndex];
				bufferIndex += sizeof(bool);
				bool write = buffer[bufferIndex];
				bufferIndex += sizeof(bool);
				SetDataBreakpoint(address, read, write);
				// setDataBreakpoint(address, read, write);

				break;
			}
			case COMMAND_SET_INSTRUCTION_BREAKPOINT: {
				// Read the address
				ret = recvwait(bss, clientfd, buffer, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (instruction breakpoint)");

				// Parse the address and set the breakpoint
				unsigned int address = ((unsigned int *) buffer)[0];
				setInstructionBreakpoint(address);

				break;
			}
			case COMMAND_TOGGLE_BREAKPOINT: {
				// Read the address
				ret = recvwait(bss, clientfd, buffer, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "recvwait (toggle breakpoint)");
				u32 address = ((unsigned int *) buffer)[0];

				struct Breakpoint *breakpoint = getBreakpoint(address, GENERAL_BREAKPOINTS_COUNT);

				if (breakpoint != NULL) {
					breakpoint = removeBreakpoint(breakpoint);
				} else {
					breakpoint = allocateBreakpoint();

					if (breakpoint != NULL) {
						breakpoint = setBreakpoint(breakpoint, address);
					}
				}

				break;
			}
			case COMMAND_REMOVE_ALL_BREAKPOINTS: {
				removeAllBreakpoints();
				break;
			}
			case COMMAND_GET_STACK_TRACE: {
				log_print("Getting stack trace...\n");
				struct node *stackTrace = getStackTrace(NULL);
				int stackTraceLength = length(stackTrace);

				// Let the client know the length beforehand
				int bufferIndex = 0;
				((int *) buffer)[bufferIndex++] = stackTraceLength;

				struct node *currentStackTraceElement = stackTrace;
				while (currentStackTraceElement != NULL) {
					int address = (int) currentStackTraceElement->data;
					log_printf("Stack trace element address: %08x\n", address);
					((int *) buffer)[bufferIndex++] = (int) currentStackTraceElement->data;

					currentStackTraceElement = currentStackTraceElement->next;
				}

				log_printf("Sending stack trace with length %i\n", stackTraceLength);
				ret = sendwait(bss, clientfd, buffer, sizeof(int) + stackTraceLength);
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (stack trace)");

				break;
			}
			case COMMAND_POKE_REGISTERS: {
				log_print("Receiving poke registers data...\n");
				int gprSize = 4 * 32;
				int fprSize = 8 * 32;
				ret = recvwait(bss, clientfd, buffer, gprSize + fprSize);
				log_print("Poking registers...\n");
				memcpy((void *) crashContext.gpr, (const void *) buffer, gprSize);
				memcpy((void *) crashContext.fpr, (const void *) buffer, fprSize);

				break;
			}
			case COMMAND_GET_ENTRY_POINT_ADDRESS: {
				u32 *entryPointAddress = (u32 * ) * ((u32 * )OS_SPECIFICS->addr_OSTitle_main_entry);
				((u32 *) buffer)[0] = (u32) entryPointAddress;
				ret = sendwait(bss, clientfd, buffer, sizeof(int));
				ASSERT_FUNCTION_SUCCEEDED(ret, "sendwait (Entry point address)");

				break;
			}
			case COMMAND_RUN_KERNEL_COPY_SERVICE: {
				if (!kernelCopyServiceStarted) {
					kernelCopyServiceStarted = true;
					startKernelCopyService();
				}

				break;
			}
			case COMMAND_PERSIST_ASSEMBLY: {
				unsigned int length = receiveString(bss, clientfd, (unsigned char *) buffer, DATA_BUFFER_SIZE);
				persistAssembly(buffer, length);

				break;
			}
			case COMMAND_CLEAR_ASSEMBLY: {
				clearAssembly();

				break;
			}
			default: {
				reportIllegalCommandByte(ret);

				break;
			}
		}
	}

	error:
	bss->error = ret;
	return 0;
}

int sockfd = -1, clientfd = -1, ret = 0, len;
struct sockaddr_in socketAddress;
struct pygecko_bss_t *bss;

static int runTCPGeckoServer(int argc, void *argv) {
	bss = (struct pygecko_bss_t *) argv;

	setup_os_exceptions();
	socket_lib_init();
	initializeUDPLog();

	while (true) {
		socketAddress.sin_family = AF_INET;
		socketAddress.sin_port = 7331;
		socketAddress.sin_addr.s_addr = 0;

		log_printf("socket()...\n");
		sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		CHECK_ERROR(sockfd == -1)

		log_printf("bind()...\n");
		ret = bind(sockfd, (struct sockaddr *) &socketAddress, (s32) 16);
		CHECK_ERROR(ret < 0)

		log_printf("listen()...\n");
		ret = listen(sockfd, (s32) 20);
		CHECK_ERROR(ret < 0)

		while (true) {
			len = 16;
			log_printf("before accept()...\n");
			clientfd = accept(sockfd, (struct sockaddr *) &socketAddress, (s32 * ) & len);
			log_printf("after accept()...\n");
			CHECK_ERROR(clientfd == -1)
			log_printf("commands()...\n");
			ret = processCommands(bss, clientfd);
			CHECK_ERROR(ret < 0)
			socketclose(clientfd);
			clientfd = -1;

			log_printf("GX2WaitForVsync() inner...\n");
			GX2WaitForVsync();
		}

		error:
		log_printf("error, closing connection...\n");
		if (clientfd != -1)
			socketclose(clientfd);
		if (sockfd != -1)
			socketclose(sockfd);
		bss->error = ret;

		// Fix the console freezing when e.g. going to the friend list
		log_printf("GX2WaitForVsync() outer...\n");
		GX2WaitForVsync();
	}

	return 0;
}

static s32 startTCPGeckoThread(s32 argc, void *argv) {
	log_print("Starting TCP Gecko thread...\n");

	// Run the TCP Gecko Installer server
	struct pygecko_bss_t *bss;

	bss = (struct pygecko_bss_t *) memalign(0x40, sizeof(struct pygecko_bss_t));
	if (bss == 0)
		return (s32) 0;
	memset(bss, 0, sizeof(struct pygecko_bss_t));

	if (OSCreateThread(&bss->thread, (s32 (*)(s32, void *)) runTCPGeckoServer, 1, bss,
					   (u32) bss->stack + sizeof(bss->stack),
					   sizeof(bss->stack), 0,
					   0xc) == 1) {
		OSResumeThread(&bss->thread);
	} else {
		free(bss);
	}

	log_print("TCP Gecko thread started...\n");

	// Execute the code handler if it is installed
	if (isCodeHandlerInstalled) {
		log_print("Code handler installed...\n");
		void (*codeHandlerFunction)() = (void (*)()) CODE_HANDLER_INSTALL_ADDRESS;

		while (true) {
			usleep(9000);

			// considerApplyingSDCheats();
			// log_print("Running code handler...\n");
			codeHandlerFunction();
			// log_print("Code handler done executing...\n");

			if (assemblySize > 0) {
				executeAssembly();
			}

			if (areSDCheatsEnabled) {
				considerApplyingSDCheats();
			}
		}
	} else {
		log_print("Code handler not installed...\n");
	}

	return (s32) 0;
}

void startTCPGecko() {
	// log_print("Starting TCP Gecko...\n");
	uint64_t titleID = OSGetTitleID();
	if(titleID == 0x00050000101DBE00){

	// Start ShineClient
int value0=0x00000001;
kernelCopyData((unsigned char *) 0x10034D60,(unsigned char *)&value0,sizeof(value0));
int value1=0x00000001;
kernelCopyData((unsigned char *) 0x10034D5C,(unsigned char *)&value1,sizeof(value1));
int value2=0x00000001;
kernelCopyData((unsigned char *) 0x10034D58,(unsigned char *)&value2,sizeof(value2));
int value3=0x00000001;
kernelCopyData((unsigned char *) 0x10034D54,(unsigned char *)&value3,sizeof(value3));
int value4=0x00000001;
kernelCopyData((unsigned char *) 0x10034D50,(unsigned char *)&value4,sizeof(value4));
int value5=0x00000001;
kernelCopyData((unsigned char *) 0x10034D4C,(unsigned char *)&value5,sizeof(value5));
int value6=0x00000001;
kernelCopyData((unsigned char *) 0x10034D40,(unsigned char *)&value6,sizeof(value6));
writeKernelMemory((void *)0x02D9C8B0,0x4E800421L);
writeKernelMemory((void *)0x02D9CAC0,0x4E800421L);
writeKernelMemory((void *)0x031E5100,0x7C0802A6L);
writeKernelMemory((void *)0x031E5130,0x9001008CL);
writeKernelMemory((void *)0x031E5134,0xD3E10080L);
writeKernelMemory((void *)0x03857BC4,0x9001008CL);
writeKernelMemory((void *)0x03857BC8,0x3C401400L);
writeKernelMemory((void *)0x03857BCC,0x38422000L);
writeKernelMemory((void *)0x03857BD0,0x7D8802A6L);
writeKernelMemory((void *)0x03857BD4,0x91820000L);
writeKernelMemory((void *)0x03857BD8,0x90620004L);
writeKernelMemory((void *)0x03857BDC,0x3D801003L);
writeKernelMemory((void *)0x03857BE0,0x818C4C60L);
writeKernelMemory((void *)0x03857BE4,0x2C0C0000L);
writeKernelMemory((void *)0x03857BE8,0x41820450L);
writeKernelMemory((void *)0x03857BEC,0x3D80109DL);
writeKernelMemory((void *)0x03857BF0,0x818CD8E4L);
writeKernelMemory((void *)0x03857BF4,0x818C0034L);
writeKernelMemory((void *)0x03857BF8,0x2C0C0000L);
writeKernelMemory((void *)0x03857BFC,0x4182043CL);
writeKernelMemory((void *)0x03857C00,0x386000A6L);
writeKernelMemory((void *)0x03857C04,0x3D800248L);
writeKernelMemory((void *)0x03857C08,0x618C6698L);
writeKernelMemory((void *)0x03857C0C,0x7D8903A6L);
writeKernelMemory((void *)0x03857C10,0x4E800421L);
writeKernelMemory((void *)0x03857C14,0x7C641B78L);
writeKernelMemory((void *)0x03857C18,0x38600000L);
writeKernelMemory((void *)0x03857C1C,0x38A00040L);
writeKernelMemory((void *)0x03857C20,0x3D800248L);
writeKernelMemory((void *)0x03857C24,0x618C61BCL);
writeKernelMemory((void *)0x03857C28,0x7D8903A6L);
writeKernelMemory((void *)0x03857C2C,0x4E800421L);
writeKernelMemory((void *)0x03857C30,0x90620008L);
writeKernelMemory((void *)0x03857C34,0x38600000L);
writeKernelMemory((void *)0x03857C38,0x38800024L);
writeKernelMemory((void *)0x03857C3C,0x38A20008L);
writeKernelMemory((void *)0x03857C40,0x3D80028BL);
writeKernelMemory((void *)0x03857C44,0x618C09F8L);
writeKernelMemory((void *)0x03857C48,0x7D8903A6L);
writeKernelMemory((void *)0x03857C4C,0x4E800421L);
writeKernelMemory((void *)0x03857C50,0x90620008L);
writeKernelMemory((void *)0x03857C54,0x3C60109DL);
writeKernelMemory((void *)0x03857C58,0x8063D8E4L);
writeKernelMemory((void *)0x03857C5C,0x80630034L);
writeKernelMemory((void *)0x03857C60,0x80630878L);
writeKernelMemory((void *)0x03857C64,0x38820008L);
writeKernelMemory((void *)0x03857C68,0x3D800304L);
writeKernelMemory((void *)0x03857C6C,0x618CA5D8L);
writeKernelMemory((void *)0x03857C70,0x7D8903A6L);
writeKernelMemory((void *)0x03857C74,0x4E800421L);
writeKernelMemory((void *)0x03857C78,0x3860000BL);
writeKernelMemory((void *)0x03857C7C,0x3D800248L);
writeKernelMemory((void *)0x03857C80,0x618C6698L);
writeKernelMemory((void *)0x03857C84,0x7D8903A6L);
writeKernelMemory((void *)0x03857C88,0x4E800421L);
writeKernelMemory((void *)0x03857C8C,0x7C641B78L);
writeKernelMemory((void *)0x03857C90,0x38600000L);
writeKernelMemory((void *)0x03857C94,0x38A00040L);
writeKernelMemory((void *)0x03857C98,0x3D800248L);
writeKernelMemory((void *)0x03857C9C,0x618C61BCL);
writeKernelMemory((void *)0x03857CA0,0x7D8903A6L);
writeKernelMemory((void *)0x03857CA4,0x4E800421L);
writeKernelMemory((void *)0x03857CA8,0x90620008L);
writeKernelMemory((void *)0x03857CAC,0x38600000L);
writeKernelMemory((void *)0x03857CB0,0x38800025L);
writeKernelMemory((void *)0x03857CB4,0x38A20008L);
writeKernelMemory((void *)0x03857CB8,0x3D80028BL);
writeKernelMemory((void *)0x03857CBC,0x618C09F8L);
writeKernelMemory((void *)0x03857CC0,0x7D8903A6L);
writeKernelMemory((void *)0x03857CC4,0x4E800421L);
writeKernelMemory((void *)0x03857CC8,0x90620008L);
writeKernelMemory((void *)0x03857CCC,0x3C60109DL);
writeKernelMemory((void *)0x03857CD0,0x8063D8E4L);
writeKernelMemory((void *)0x03857CD4,0x80630034L);
writeKernelMemory((void *)0x03857CD8,0x80630878L);
writeKernelMemory((void *)0x03857CDC,0x38820008L);
writeKernelMemory((void *)0x03857CE0,0x3D800304L);
writeKernelMemory((void *)0x03857CE4,0x618CA5D8L);
writeKernelMemory((void *)0x03857CE8,0x7D8903A6L);
writeKernelMemory((void *)0x03857CEC,0x4E800421L);
writeKernelMemory((void *)0x03857CF0,0x3860003EL);
writeKernelMemory((void *)0x03857CF4,0x3D800248L);
writeKernelMemory((void *)0x03857CF8,0x618C6698L);
writeKernelMemory((void *)0x03857CFC,0x7D8903A6L);
writeKernelMemory((void *)0x03857D00,0x4E800421L);
writeKernelMemory((void *)0x03857D04,0x7C641B78L);
writeKernelMemory((void *)0x03857D08,0x38600000L);
writeKernelMemory((void *)0x03857D0C,0x38A00040L);
writeKernelMemory((void *)0x03857D10,0x3D800248L);
writeKernelMemory((void *)0x03857D14,0x618C61BCL);
writeKernelMemory((void *)0x03857D18,0x7D8903A6L);
writeKernelMemory((void *)0x03857D1C,0x4E800421L);
writeKernelMemory((void *)0x03857D20,0x90620008L);
writeKernelMemory((void *)0x03857D24,0x38600000L);
writeKernelMemory((void *)0x03857D28,0x38800026L);
writeKernelMemory((void *)0x03857D2C,0x38A20008L);
writeKernelMemory((void *)0x03857D30,0x3D80028BL);
writeKernelMemory((void *)0x03857D34,0x618C09F8L);
writeKernelMemory((void *)0x03857D38,0x7D8903A6L);
writeKernelMemory((void *)0x03857D3C,0x4E800421L);
writeKernelMemory((void *)0x03857D40,0x90620008L);
writeKernelMemory((void *)0x03857D44,0x3C60109DL);
writeKernelMemory((void *)0x03857D48,0x8063D8E4L);
writeKernelMemory((void *)0x03857D4C,0x80630034L);
writeKernelMemory((void *)0x03857D50,0x80630878L);
writeKernelMemory((void *)0x03857D54,0x38820008L);
writeKernelMemory((void *)0x03857D58,0x3D800304L);
writeKernelMemory((void *)0x03857D5C,0x618CA5D8L);
writeKernelMemory((void *)0x03857D60,0x7D8903A6L);
writeKernelMemory((void *)0x03857D64,0x4E800421L);
writeKernelMemory((void *)0x03857D68,0x3860007CL);
writeKernelMemory((void *)0x03857D6C,0x3D800248L);
writeKernelMemory((void *)0x03857D70,0x618C6698L);
writeKernelMemory((void *)0x03857D74,0x7D8903A6L);
writeKernelMemory((void *)0x03857D78,0x4E800421L);
writeKernelMemory((void *)0x03857D7C,0x7C641B78L);
writeKernelMemory((void *)0x03857D80,0x38600000L);
writeKernelMemory((void *)0x03857D84,0x38A00040L);
writeKernelMemory((void *)0x03857D88,0x3D800248L);
writeKernelMemory((void *)0x03857D8C,0x618C61BCL);
writeKernelMemory((void *)0x03857D90,0x7D8903A6L);
writeKernelMemory((void *)0x03857D94,0x4E800421L);
writeKernelMemory((void *)0x03857D98,0x90620008L);
writeKernelMemory((void *)0x03857D9C,0x38600000L);
writeKernelMemory((void *)0x03857DA0,0x38800027L);
writeKernelMemory((void *)0x03857DA4,0x38A20008L);
writeKernelMemory((void *)0x03857DA8,0x3D80028BL);
writeKernelMemory((void *)0x03857DAC,0x618C09F8L);
writeKernelMemory((void *)0x03857DB0,0x7D8903A6L);
writeKernelMemory((void *)0x03857DB4,0x4E800421L);
writeKernelMemory((void *)0x03857DB8,0x90620008L);
writeKernelMemory((void *)0x03857DBC,0x3C60109DL);
writeKernelMemory((void *)0x03857DC0,0x8063D8E4L);
writeKernelMemory((void *)0x03857DC4,0x80630034L);
writeKernelMemory((void *)0x03857DC8,0x80630878L);
writeKernelMemory((void *)0x03857DCC,0x38820008L);
writeKernelMemory((void *)0x03857DD0,0x3D800304L);
writeKernelMemory((void *)0x03857DD4,0x618CA5D8L);
writeKernelMemory((void *)0x03857DD8,0x7D8903A6L);
writeKernelMemory((void *)0x03857DDC,0x4E800421L);
writeKernelMemory((void *)0x03857DE0,0x38600089L);
writeKernelMemory((void *)0x03857DE4,0x3D800248L);
writeKernelMemory((void *)0x03857DE8,0x618C6698L);
writeKernelMemory((void *)0x03857DEC,0x7D8903A6L);
writeKernelMemory((void *)0x03857DF0,0x4E800421L);
writeKernelMemory((void *)0x03857DF4,0x7C641B78L);
writeKernelMemory((void *)0x03857DF8,0x38600000L);
writeKernelMemory((void *)0x03857DFC,0x38A00040L);
writeKernelMemory((void *)0x03857E00,0x3D800248L);
writeKernelMemory((void *)0x03857E04,0x618C61BCL);
writeKernelMemory((void *)0x03857E08,0x7D8903A6L);
writeKernelMemory((void *)0x03857E0C,0x4E800421L);
writeKernelMemory((void *)0x03857E10,0x90620008L);
writeKernelMemory((void *)0x03857E14,0x38600000L);
writeKernelMemory((void *)0x03857E18,0x38800028L);
writeKernelMemory((void *)0x03857E1C,0x38A20008L);
writeKernelMemory((void *)0x03857E20,0x3D80028BL);
writeKernelMemory((void *)0x03857E24,0x618C09F8L);
writeKernelMemory((void *)0x03857E28,0x7D8903A6L);
writeKernelMemory((void *)0x03857E2C,0x4E800421L);
writeKernelMemory((void *)0x03857E30,0x90620008L);
writeKernelMemory((void *)0x03857E34,0x3C60109DL);
writeKernelMemory((void *)0x03857E38,0x8063D8E4L);
writeKernelMemory((void *)0x03857E3C,0x80630034L);
writeKernelMemory((void *)0x03857E40,0x80630878L);
writeKernelMemory((void *)0x03857E44,0x38820008L);
writeKernelMemory((void *)0x03857E48,0x3D800304L);
writeKernelMemory((void *)0x03857E4C,0x618CA5D8L);
writeKernelMemory((void *)0x03857E50,0x7D8903A6L);
writeKernelMemory((void *)0x03857E54,0x4E800421L);
writeKernelMemory((void *)0x03857E58,0x386000D2L);
writeKernelMemory((void *)0x03857E5C,0x3D800248L);
writeKernelMemory((void *)0x03857E60,0x618C6698L);
writeKernelMemory((void *)0x03857E64,0x7D8903A6L);
writeKernelMemory((void *)0x03857E68,0x4E800421L);
writeKernelMemory((void *)0x03857E6C,0x7C641B78L);
writeKernelMemory((void *)0x03857E70,0x38600000L);
writeKernelMemory((void *)0x03857E74,0x38A00040L);
writeKernelMemory((void *)0x03857E78,0x3D800248L);
writeKernelMemory((void *)0x03857E7C,0x618C61BCL);
writeKernelMemory((void *)0x03857E80,0x7D8903A6L);
writeKernelMemory((void *)0x03857E84,0x4E800421L);
writeKernelMemory((void *)0x03857E88,0x90620008L);
writeKernelMemory((void *)0x03857E8C,0x38600000L);
writeKernelMemory((void *)0x03857E90,0x38800029L);
writeKernelMemory((void *)0x03857E94,0x38A20008L);
writeKernelMemory((void *)0x03857E98,0x3D80028BL);
writeKernelMemory((void *)0x03857E9C,0x618C09F8L);
writeKernelMemory((void *)0x03857EA0,0x7D8903A6L);
writeKernelMemory((void *)0x03857EA4,0x4E800421L);
writeKernelMemory((void *)0x03857EA8,0x90620008L);
writeKernelMemory((void *)0x03857EAC,0x3C60109DL);
writeKernelMemory((void *)0x03857EB0,0x8063D8E4L);
writeKernelMemory((void *)0x03857EB4,0x80630034L);
writeKernelMemory((void *)0x03857EB8,0x80630878L);
writeKernelMemory((void *)0x03857EBC,0x38820008L);
writeKernelMemory((void *)0x03857EC0,0x3D800304L);
writeKernelMemory((void *)0x03857EC4,0x618CA5D8L);
writeKernelMemory((void *)0x03857EC8,0x7D8903A6L);
writeKernelMemory((void *)0x03857ECC,0x4E800421L);
writeKernelMemory((void *)0x03857ED0,0x386000D3L);
writeKernelMemory((void *)0x03857ED4,0x3D800248L);
writeKernelMemory((void *)0x03857ED8,0x618C6698L);
writeKernelMemory((void *)0x03857EDC,0x7D8903A6L);
writeKernelMemory((void *)0x03857EE0,0x4E800421L);
writeKernelMemory((void *)0x03857EE4,0x7C641B78L);
writeKernelMemory((void *)0x03857EE8,0x38600000L);
writeKernelMemory((void *)0x03857EEC,0x38A00040L);
writeKernelMemory((void *)0x03857EF0,0x3D800248L);
writeKernelMemory((void *)0x03857EF4,0x618C61BCL);
writeKernelMemory((void *)0x03857EF8,0x7D8903A6L);
writeKernelMemory((void *)0x03857EFC,0x4E800421L);
writeKernelMemory((void *)0x03857F00,0x90620008L);
writeKernelMemory((void *)0x03857F04,0x38600000L);
writeKernelMemory((void *)0x03857F08,0x3880002AL);
writeKernelMemory((void *)0x03857F0C,0x38A20008L);
writeKernelMemory((void *)0x03857F10,0x3D80028BL);
writeKernelMemory((void *)0x03857F14,0x618C09F8L);
writeKernelMemory((void *)0x03857F18,0x7D8903A6L);
writeKernelMemory((void *)0x03857F1C,0x4E800421L);
writeKernelMemory((void *)0x03857F20,0x90620008L);
writeKernelMemory((void *)0x03857F24,0x3C60109DL);
writeKernelMemory((void *)0x03857F28,0x8063D8E4L);
writeKernelMemory((void *)0x03857F2C,0x80630034L);
writeKernelMemory((void *)0x03857F30,0x80630878L);
writeKernelMemory((void *)0x03857F34,0x38820008L);
writeKernelMemory((void *)0x03857F38,0x3D800304L);
writeKernelMemory((void *)0x03857F3C,0x618CA5D8L);
writeKernelMemory((void *)0x03857F40,0x7D8903A6L);
writeKernelMemory((void *)0x03857F44,0x4E800421L);
writeKernelMemory((void *)0x03857F48,0x386001A6L);
writeKernelMemory((void *)0x03857F4C,0x3D800248L);
writeKernelMemory((void *)0x03857F50,0x618C6698L);
writeKernelMemory((void *)0x03857F54,0x7D8903A6L);
writeKernelMemory((void *)0x03857F58,0x4E800421L);
writeKernelMemory((void *)0x03857F5C,0x7C641B78L);
writeKernelMemory((void *)0x03857F60,0x38600000L);
writeKernelMemory((void *)0x03857F64,0x38A00040L);
writeKernelMemory((void *)0x03857F68,0x3D800248L);
writeKernelMemory((void *)0x03857F6C,0x618C61BCL);
writeKernelMemory((void *)0x03857F70,0x7D8903A6L);
writeKernelMemory((void *)0x03857F74,0x4E800421L);
writeKernelMemory((void *)0x03857F78,0x90620008L);
writeKernelMemory((void *)0x03857F7C,0x38600000L);
writeKernelMemory((void *)0x03857F80,0x3880002BL);
writeKernelMemory((void *)0x03857F84,0x38A20008L);
writeKernelMemory((void *)0x03857F88,0x3D80028BL);
writeKernelMemory((void *)0x03857F8C,0x618C09F8L);
writeKernelMemory((void *)0x03857F90,0x7D8903A6L);
writeKernelMemory((void *)0x03857F94,0x4E800421L);
writeKernelMemory((void *)0x03857F98,0x90620008L);
writeKernelMemory((void *)0x03857F9C,0x3C60109DL);
writeKernelMemory((void *)0x03857FA0,0x8063D8E4L);
writeKernelMemory((void *)0x03857FA4,0x80630034L);
writeKernelMemory((void *)0x03857FA8,0x80630878L);
writeKernelMemory((void *)0x03857FAC,0x38820008L);
writeKernelMemory((void *)0x03857FB0,0x3D800304L);
writeKernelMemory((void *)0x03857FB4,0x618CA5D8L);
writeKernelMemory((void *)0x03857FB8,0x7D8903A6L);
writeKernelMemory((void *)0x03857FBC,0x4E800421L);
writeKernelMemory((void *)0x03857FC0,0x386001B7L);
writeKernelMemory((void *)0x03857FC4,0x3D800248L);
writeKernelMemory((void *)0x03857FC8,0x618C6698L);
writeKernelMemory((void *)0x03857FCC,0x7D8903A6L);
writeKernelMemory((void *)0x03857FD0,0x4E800421L);
writeKernelMemory((void *)0x03857FD4,0x7C641B78L);
writeKernelMemory((void *)0x03857FD8,0x38600000L);
writeKernelMemory((void *)0x03857FDC,0x38A00040L);
writeKernelMemory((void *)0x03857FE0,0x3D800248L);
writeKernelMemory((void *)0x03857FE4,0x618C61BCL);
writeKernelMemory((void *)0x03857FE8,0x7D8903A6L);
writeKernelMemory((void *)0x03857FEC,0x4E800421L);
writeKernelMemory((void *)0x03857FF0,0x90620008L);
writeKernelMemory((void *)0x03857FF4,0x38600000L);
writeKernelMemory((void *)0x03857FF8,0x3880002CL);
writeKernelMemory((void *)0x03857FFC,0x38A20008L);
writeKernelMemory((void *)0x03858000,0x3D80028BL);
writeKernelMemory((void *)0x03858004,0x618C09F8L);
writeKernelMemory((void *)0x03858008,0x7D8903A6L);
writeKernelMemory((void *)0x0385800C,0x4E800421L);
writeKernelMemory((void *)0x03858010,0x90620008L);
writeKernelMemory((void *)0x03858014,0x3C60109DL);
writeKernelMemory((void *)0x03858018,0x8063D8E4L);
writeKernelMemory((void *)0x0385801C,0x80630034L);
writeKernelMemory((void *)0x03858020,0x80630878L);
writeKernelMemory((void *)0x03858024,0x38820008L);
writeKernelMemory((void *)0x03858028,0x3D800304L);
writeKernelMemory((void *)0x0385802C,0x618CA5D8L);
writeKernelMemory((void *)0x03858030,0x7D8903A6L);
writeKernelMemory((void *)0x03858034,0x4E800421L);
writeKernelMemory((void *)0x03858038,0x81820000L);
writeKernelMemory((void *)0x0385803C,0x80620004L);
writeKernelMemory((void *)0x03858040,0x7D8803A6L);
writeKernelMemory((void *)0x03858044,0x4B98D0F0L);
writeKernelMemory((void *)0x031E5130,0x48672A94L);
writeKernelMemory((void *)0x038584CC,0xD3E10080L);
writeKernelMemory((void *)0x038584D0,0x3C401400L);
writeKernelMemory((void *)0x038584D4,0x38422000L);
writeKernelMemory((void *)0x038584D8,0x7D8802A6L);
writeKernelMemory((void *)0x038584DC,0x91820000L);
writeKernelMemory((void *)0x038584E0,0x90620004L);
writeKernelMemory((void *)0x038584E4,0x3D801003L);
writeKernelMemory((void *)0x038584E8,0x818C4C5CL);
writeKernelMemory((void *)0x038584EC,0x2C0C0000L);
writeKernelMemory((void *)0x038584F0,0x41820450L);
writeKernelMemory((void *)0x038584F4,0x3D80109DL);
writeKernelMemory((void *)0x038584F8,0x818CD8E4L);
writeKernelMemory((void *)0x038584FC,0x818C0034L);
writeKernelMemory((void *)0x03858500,0x2C0C0000L);
writeKernelMemory((void *)0x03858504,0x4182043CL);
writeKernelMemory((void *)0x03858508,0x38600022L);
writeKernelMemory((void *)0x0385850C,0x3D800248L);
writeKernelMemory((void *)0x03858510,0x618C6698L);
writeKernelMemory((void *)0x03858514,0x7D8903A6L);
writeKernelMemory((void *)0x03858518,0x4E800421L);
writeKernelMemory((void *)0x0385851C,0x7C641B78L);
writeKernelMemory((void *)0x03858520,0x38600000L);
writeKernelMemory((void *)0x03858524,0x38A00040L);
writeKernelMemory((void *)0x03858528,0x3D800248L);
writeKernelMemory((void *)0x0385852C,0x618C61BCL);
writeKernelMemory((void *)0x03858530,0x7D8903A6L);
writeKernelMemory((void *)0x03858534,0x4E800421L);
writeKernelMemory((void *)0x03858538,0x90620008L);
writeKernelMemory((void *)0x0385853C,0x38600000L);
writeKernelMemory((void *)0x03858540,0x38800024L);
writeKernelMemory((void *)0x03858544,0x38A20008L);
writeKernelMemory((void *)0x03858548,0x3D80028BL);
writeKernelMemory((void *)0x0385854C,0x618C09F8L);
writeKernelMemory((void *)0x03858550,0x7D8903A6L);
writeKernelMemory((void *)0x03858554,0x4E800421L);
writeKernelMemory((void *)0x03858558,0x90620008L);
writeKernelMemory((void *)0x0385855C,0x3C60109DL);
writeKernelMemory((void *)0x03858560,0x8063D8E4L);
writeKernelMemory((void *)0x03858564,0x80630034L);
writeKernelMemory((void *)0x03858568,0x80630878L);
writeKernelMemory((void *)0x0385856C,0x38820008L);
writeKernelMemory((void *)0x03858570,0x3D800304L);
writeKernelMemory((void *)0x03858574,0x618CA5D8L);
writeKernelMemory((void *)0x03858578,0x7D8903A6L);
writeKernelMemory((void *)0x0385857C,0x4E800421L);
writeKernelMemory((void *)0x03858580,0x3860002BL);
writeKernelMemory((void *)0x03858584,0x3D800248L);
writeKernelMemory((void *)0x03858588,0x618C6698L);
writeKernelMemory((void *)0x0385858C,0x7D8903A6L);
writeKernelMemory((void *)0x03858590,0x4E800421L);
writeKernelMemory((void *)0x03858594,0x7C641B78L);
writeKernelMemory((void *)0x03858598,0x38600000L);
writeKernelMemory((void *)0x0385859C,0x38A00040L);
writeKernelMemory((void *)0x038585A0,0x3D800248L);
writeKernelMemory((void *)0x038585A4,0x618C61BCL);
writeKernelMemory((void *)0x038585A8,0x7D8903A6L);
writeKernelMemory((void *)0x038585AC,0x4E800421L);
writeKernelMemory((void *)0x038585B0,0x90620008L);
writeKernelMemory((void *)0x038585B4,0x38600000L);
writeKernelMemory((void *)0x038585B8,0x38800025L);
writeKernelMemory((void *)0x038585BC,0x38A20008L);
writeKernelMemory((void *)0x038585C0,0x3D80028BL);
writeKernelMemory((void *)0x038585C4,0x618C09F8L);
writeKernelMemory((void *)0x038585C8,0x7D8903A6L);
writeKernelMemory((void *)0x038585CC,0x4E800421L);
writeKernelMemory((void *)0x038585D0,0x90620008L);
writeKernelMemory((void *)0x038585D4,0x3C60109DL);
writeKernelMemory((void *)0x038585D8,0x8063D8E4L);
writeKernelMemory((void *)0x038585DC,0x80630034L);
writeKernelMemory((void *)0x038585E0,0x80630878L);
writeKernelMemory((void *)0x038585E4,0x38820008L);
writeKernelMemory((void *)0x038585E8,0x3D800304L);
writeKernelMemory((void *)0x038585EC,0x618CA5D8L);
writeKernelMemory((void *)0x038585F0,0x7D8903A6L);
writeKernelMemory((void *)0x038585F4,0x4E800421L);
writeKernelMemory((void *)0x038585F8,0x3860004BL);
writeKernelMemory((void *)0x038585FC,0x3D800248L);
writeKernelMemory((void *)0x03858600,0x618C6698L);
writeKernelMemory((void *)0x03858604,0x7D8903A6L);
writeKernelMemory((void *)0x03858608,0x4E800421L);
writeKernelMemory((void *)0x0385860C,0x7C641B78L);
writeKernelMemory((void *)0x03858610,0x38600000L);
writeKernelMemory((void *)0x03858614,0x38A00040L);
writeKernelMemory((void *)0x03858618,0x3D800248L);
writeKernelMemory((void *)0x0385861C,0x618C61BCL);
writeKernelMemory((void *)0x03858620,0x7D8903A6L);
writeKernelMemory((void *)0x03858624,0x4E800421L);
writeKernelMemory((void *)0x03858628,0x90620008L);
writeKernelMemory((void *)0x0385862C,0x38600000L);
writeKernelMemory((void *)0x03858630,0x38800026L);
writeKernelMemory((void *)0x03858634,0x38A20008L);
writeKernelMemory((void *)0x03858638,0x3D80028BL);
writeKernelMemory((void *)0x0385863C,0x618C09F8L);
writeKernelMemory((void *)0x03858640,0x7D8903A6L);
writeKernelMemory((void *)0x03858644,0x4E800421L);
writeKernelMemory((void *)0x03858648,0x90620008L);
writeKernelMemory((void *)0x0385864C,0x3C60109DL);
writeKernelMemory((void *)0x03858650,0x8063D8E4L);
writeKernelMemory((void *)0x03858654,0x80630034L);
writeKernelMemory((void *)0x03858658,0x80630878L);
writeKernelMemory((void *)0x0385865C,0x38820008L);
writeKernelMemory((void *)0x03858660,0x3D800304L);
writeKernelMemory((void *)0x03858664,0x618CA5D8L);
writeKernelMemory((void *)0x03858668,0x7D8903A6L);
writeKernelMemory((void *)0x0385866C,0x4E800421L);
writeKernelMemory((void *)0x03858670,0x3860005EL);
writeKernelMemory((void *)0x03858674,0x3D800248L);
writeKernelMemory((void *)0x03858678,0x618C6698L);
writeKernelMemory((void *)0x0385867C,0x7D8903A6L);
writeKernelMemory((void *)0x03858680,0x4E800421L);
writeKernelMemory((void *)0x03858684,0x7C641B78L);
writeKernelMemory((void *)0x03858688,0x38600000L);
writeKernelMemory((void *)0x0385868C,0x38A00040L);
writeKernelMemory((void *)0x03858690,0x3D800248L);
writeKernelMemory((void *)0x03858694,0x618C61BCL);
writeKernelMemory((void *)0x03858698,0x7D8903A6L);
writeKernelMemory((void *)0x0385869C,0x4E800421L);
writeKernelMemory((void *)0x038586A0,0x90620008L);
writeKernelMemory((void *)0x038586A4,0x38600000L);
writeKernelMemory((void *)0x038586A8,0x38800027L);
writeKernelMemory((void *)0x038586AC,0x38A20008L);
writeKernelMemory((void *)0x038586B0,0x3D80028BL);
writeKernelMemory((void *)0x038586B4,0x618C09F8L);
writeKernelMemory((void *)0x038586B8,0x7D8903A6L);
writeKernelMemory((void *)0x038586BC,0x4E800421L);
writeKernelMemory((void *)0x038586C0,0x90620008L);
writeKernelMemory((void *)0x038586C4,0x3C60109DL);
writeKernelMemory((void *)0x038586C8,0x8063D8E4L);
writeKernelMemory((void *)0x038586CC,0x80630034L);
writeKernelMemory((void *)0x038586D0,0x80630878L);
writeKernelMemory((void *)0x038586D4,0x38820008L);
writeKernelMemory((void *)0x038586D8,0x3D800304L);
writeKernelMemory((void *)0x038586DC,0x618CA5D8L);
writeKernelMemory((void *)0x038586E0,0x7D8903A6L);
writeKernelMemory((void *)0x038586E4,0x4E800421L);
writeKernelMemory((void *)0x038586E8,0x38600114L);
writeKernelMemory((void *)0x038586EC,0x3D800248L);
writeKernelMemory((void *)0x038586F0,0x618C6698L);
writeKernelMemory((void *)0x038586F4,0x7D8903A6L);
writeKernelMemory((void *)0x038586F8,0x4E800421L);
writeKernelMemory((void *)0x038586FC,0x7C641B78L);
writeKernelMemory((void *)0x03858700,0x38600000L);
writeKernelMemory((void *)0x03858704,0x38A00040L);
writeKernelMemory((void *)0x03858708,0x3D800248L);
writeKernelMemory((void *)0x0385870C,0x618C61BCL);
writeKernelMemory((void *)0x03858710,0x7D8903A6L);
writeKernelMemory((void *)0x03858714,0x4E800421L);
writeKernelMemory((void *)0x03858718,0x90620008L);
writeKernelMemory((void *)0x0385871C,0x38600000L);
writeKernelMemory((void *)0x03858720,0x38800028L);
writeKernelMemory((void *)0x03858724,0x38A20008L);
writeKernelMemory((void *)0x03858728,0x3D80028BL);
writeKernelMemory((void *)0x0385872C,0x618C09F8L);
writeKernelMemory((void *)0x03858730,0x7D8903A6L);
writeKernelMemory((void *)0x03858734,0x4E800421L);
writeKernelMemory((void *)0x03858738,0x90620008L);
writeKernelMemory((void *)0x0385873C,0x3C60109DL);
writeKernelMemory((void *)0x03858740,0x8063D8E4L);
writeKernelMemory((void *)0x03858744,0x80630034L);
writeKernelMemory((void *)0x03858748,0x80630878L);
writeKernelMemory((void *)0x0385874C,0x38820008L);
writeKernelMemory((void *)0x03858750,0x3D800304L);
writeKernelMemory((void *)0x03858754,0x618CA5D8L);
writeKernelMemory((void *)0x03858758,0x7D8903A6L);
writeKernelMemory((void *)0x0385875C,0x4E800421L);
writeKernelMemory((void *)0x03858760,0x38600136L);
writeKernelMemory((void *)0x03858764,0x3D800248L);
writeKernelMemory((void *)0x03858768,0x618C6698L);
writeKernelMemory((void *)0x0385876C,0x7D8903A6L);
writeKernelMemory((void *)0x03858770,0x4E800421L);
writeKernelMemory((void *)0x03858774,0x7C641B78L);
writeKernelMemory((void *)0x03858778,0x38600000L);
writeKernelMemory((void *)0x0385877C,0x38A00040L);
writeKernelMemory((void *)0x03858780,0x3D800248L);
writeKernelMemory((void *)0x03858784,0x618C61BCL);
writeKernelMemory((void *)0x03858788,0x7D8903A6L);
writeKernelMemory((void *)0x0385878C,0x4E800421L);
writeKernelMemory((void *)0x03858790,0x90620008L);
writeKernelMemory((void *)0x03858794,0x38600000L);
writeKernelMemory((void *)0x03858798,0x38800029L);
writeKernelMemory((void *)0x0385879C,0x38A20008L);
writeKernelMemory((void *)0x038587A0,0x3D80028BL);
writeKernelMemory((void *)0x038587A4,0x618C09F8L);
writeKernelMemory((void *)0x038587A8,0x7D8903A6L);
writeKernelMemory((void *)0x038587AC,0x4E800421L);
writeKernelMemory((void *)0x038587B0,0x90620008L);
writeKernelMemory((void *)0x038587B4,0x3C60109DL);
writeKernelMemory((void *)0x038587B8,0x8063D8E4L);
writeKernelMemory((void *)0x038587BC,0x80630034L);
writeKernelMemory((void *)0x038587C0,0x80630878L);
writeKernelMemory((void *)0x038587C4,0x38820008L);
writeKernelMemory((void *)0x038587C8,0x3D800304L);
writeKernelMemory((void *)0x038587CC,0x618CA5D8L);
writeKernelMemory((void *)0x038587D0,0x7D8903A6L);
writeKernelMemory((void *)0x038587D4,0x4E800421L);
writeKernelMemory((void *)0x038587D8,0x38600137L);
writeKernelMemory((void *)0x038587DC,0x3D800248L);
writeKernelMemory((void *)0x038587E0,0x618C6698L);
writeKernelMemory((void *)0x038587E4,0x7D8903A6L);
writeKernelMemory((void *)0x038587E8,0x4E800421L);
writeKernelMemory((void *)0x038587EC,0x7C641B78L);
writeKernelMemory((void *)0x038587F0,0x38600000L);
writeKernelMemory((void *)0x038587F4,0x38A00040L);
writeKernelMemory((void *)0x038587F8,0x3D800248L);
writeKernelMemory((void *)0x038587FC,0x618C61BCL);
writeKernelMemory((void *)0x03858800,0x7D8903A6L);
writeKernelMemory((void *)0x03858804,0x4E800421L);
writeKernelMemory((void *)0x03858808,0x90620008L);
writeKernelMemory((void *)0x0385880C,0x38600000L);
writeKernelMemory((void *)0x03858810,0x3880002AL);
writeKernelMemory((void *)0x03858814,0x38A20008L);
writeKernelMemory((void *)0x03858818,0x3D80028BL);
writeKernelMemory((void *)0x0385881C,0x618C09F8L);
writeKernelMemory((void *)0x03858820,0x7D8903A6L);
writeKernelMemory((void *)0x03858824,0x4E800421L);
writeKernelMemory((void *)0x03858828,0x90620008L);
writeKernelMemory((void *)0x0385882C,0x3C60109DL);
writeKernelMemory((void *)0x03858830,0x8063D8E4L);
writeKernelMemory((void *)0x03858834,0x80630034L);
writeKernelMemory((void *)0x03858838,0x80630878L);
writeKernelMemory((void *)0x0385883C,0x38820008L);
writeKernelMemory((void *)0x03858840,0x3D800304L);
writeKernelMemory((void *)0x03858844,0x618CA5D8L);
writeKernelMemory((void *)0x03858848,0x7D8903A6L);
writeKernelMemory((void *)0x0385884C,0x4E800421L);
writeKernelMemory((void *)0x03858850,0x38600138L);
writeKernelMemory((void *)0x03858854,0x3D800248L);
writeKernelMemory((void *)0x03858858,0x618C6698L);
writeKernelMemory((void *)0x0385885C,0x7D8903A6L);
writeKernelMemory((void *)0x03858860,0x4E800421L);
writeKernelMemory((void *)0x03858864,0x7C641B78L);
writeKernelMemory((void *)0x03858868,0x38600000L);
writeKernelMemory((void *)0x0385886C,0x38A00040L);
writeKernelMemory((void *)0x03858870,0x3D800248L);
writeKernelMemory((void *)0x03858874,0x618C61BCL);
writeKernelMemory((void *)0x03858878,0x7D8903A6L);
writeKernelMemory((void *)0x0385887C,0x4E800421L);
writeKernelMemory((void *)0x03858880,0x90620008L);
writeKernelMemory((void *)0x03858884,0x38600000L);
writeKernelMemory((void *)0x03858888,0x3880002BL);
writeKernelMemory((void *)0x0385888C,0x38A20008L);
writeKernelMemory((void *)0x03858890,0x3D80028BL);
writeKernelMemory((void *)0x03858894,0x618C09F8L);
writeKernelMemory((void *)0x03858898,0x7D8903A6L);
writeKernelMemory((void *)0x0385889C,0x4E800421L);
writeKernelMemory((void *)0x038588A0,0x90620008L);
writeKernelMemory((void *)0x038588A4,0x3C60109DL);
writeKernelMemory((void *)0x038588A8,0x8063D8E4L);
writeKernelMemory((void *)0x038588AC,0x80630034L);
writeKernelMemory((void *)0x038588B0,0x80630878L);
writeKernelMemory((void *)0x038588B4,0x38820008L);
writeKernelMemory((void *)0x038588B8,0x3D800304L);
writeKernelMemory((void *)0x038588BC,0x618CA5D8L);
writeKernelMemory((void *)0x038588C0,0x7D8903A6L);
writeKernelMemory((void *)0x038588C4,0x4E800421L);
writeKernelMemory((void *)0x038588C8,0x38600139L);
writeKernelMemory((void *)0x038588CC,0x3D800248L);
writeKernelMemory((void *)0x038588D0,0x618C6698L);
writeKernelMemory((void *)0x038588D4,0x7D8903A6L);
writeKernelMemory((void *)0x038588D8,0x4E800421L);
writeKernelMemory((void *)0x038588DC,0x7C641B78L);
writeKernelMemory((void *)0x038588E0,0x38600000L);
writeKernelMemory((void *)0x038588E4,0x38A00040L);
writeKernelMemory((void *)0x038588E8,0x3D800248L);
writeKernelMemory((void *)0x038588EC,0x618C61BCL);
writeKernelMemory((void *)0x038588F0,0x7D8903A6L);
writeKernelMemory((void *)0x038588F4,0x4E800421L);
writeKernelMemory((void *)0x038588F8,0x90620008L);
writeKernelMemory((void *)0x038588FC,0x38600000L);
writeKernelMemory((void *)0x03858900,0x3880002CL);
writeKernelMemory((void *)0x03858904,0x38A20008L);
writeKernelMemory((void *)0x03858908,0x3D80028BL);
writeKernelMemory((void *)0x0385890C,0x618C09F8L);
writeKernelMemory((void *)0x03858910,0x7D8903A6L);
writeKernelMemory((void *)0x03858914,0x4E800421L);
writeKernelMemory((void *)0x03858918,0x90620008L);
writeKernelMemory((void *)0x0385891C,0x3C60109DL);
writeKernelMemory((void *)0x03858920,0x8063D8E4L);
writeKernelMemory((void *)0x03858924,0x80630034L);
writeKernelMemory((void *)0x03858928,0x80630878L);
writeKernelMemory((void *)0x0385892C,0x38820008L);
writeKernelMemory((void *)0x03858930,0x3D800304L);
writeKernelMemory((void *)0x03858934,0x618CA5D8L);
writeKernelMemory((void *)0x03858938,0x7D8903A6L);
writeKernelMemory((void *)0x0385893C,0x4E800421L);
writeKernelMemory((void *)0x03858940,0x81820000L);
writeKernelMemory((void *)0x03858944,0x80620004L);
writeKernelMemory((void *)0x03858948,0x7D8803A6L);
writeKernelMemory((void *)0x0385894C,0x4B98C7ECL);
writeKernelMemory((void *)0x031E5134,0x48673398L);
writeKernelMemory((void *)0x03858950,0x7C0802A6L);
writeKernelMemory((void *)0x03858954,0x3C401003L);
writeKernelMemory((void *)0x03858958,0x38420A00L);
writeKernelMemory((void *)0x0385895C,0x7D8802A6L);
writeKernelMemory((void *)0x03858960,0x91820000L);
writeKernelMemory((void *)0x03858964,0x90620004L);
writeKernelMemory((void *)0x03858968,0x92820008L);
writeKernelMemory((void *)0x0385896C,0x92A2000CL);
writeKernelMemory((void *)0x03858970,0x92C20010L);
writeKernelMemory((void *)0x03858974,0x92E20014L);
writeKernelMemory((void *)0x03858978,0x93020018L);
writeKernelMemory((void *)0x0385897C,0x9322001CL);
writeKernelMemory((void *)0x03858980,0x93420020L);
writeKernelMemory((void *)0x03858984,0x93620024L);
writeKernelMemory((void *)0x03858988,0x93820028L);
writeKernelMemory((void *)0x0385898C,0x93A2002CL);
writeKernelMemory((void *)0x03858990,0x93C20030L);
writeKernelMemory((void *)0x03858994,0x93E20034L);
writeKernelMemory((void *)0x03858998,0x90020038L);
writeKernelMemory((void *)0x0385899C,0x3C401003L);
writeKernelMemory((void *)0x038589A0,0x81824C54L);
writeKernelMemory((void *)0x038589A4,0x2C0C0000L);
writeKernelMemory((void *)0x038589A8,0x41820744L);
writeKernelMemory((void *)0x038589AC,0x38421000L);
writeKernelMemory((void *)0x038589B0,0x38600000L);
writeKernelMemory((void *)0x038589B4,0x3FC00221L);
writeKernelMemory((void *)0x038589B8,0x63DE82C0L);
writeKernelMemory((void *)0x038589BC,0x7FC903A6L);
writeKernelMemory((void *)0x038589C0,0x4E800421L);
writeKernelMemory((void *)0x038589C4,0x7FC903A6L);
writeKernelMemory((void *)0x038589C8,0x7C7F1B78L);
writeKernelMemory((void *)0x038589CC,0x38600000L);
writeKernelMemory((void *)0x038589D0,0x4E800421L);
writeKernelMemory((void *)0x038589D4,0x7C7D1B78L);
writeKernelMemory((void *)0x038589D8,0x48000015L);
writeKernelMemory((void *)0x038589DC,0x00640069L);
writeKernelMemory((void *)0x038589E0,0x00730070L);
writeKernelMemory((void *)0x038589E4,0x006C0061L);
writeKernelMemory((void *)0x038589E8,0x00790000L);
writeKernelMemory((void *)0x038589EC,0x7C8802A6L);
writeKernelMemory((void *)0x038589F0,0x3FC0020BL);
writeKernelMemory((void *)0x038589F4,0x38620028L);
writeKernelMemory((void *)0x038589F8,0x63DE08D4L);
writeKernelMemory((void *)0x038589FC,0x7FC903A6L);
writeKernelMemory((void *)0x03858A00,0x4E800421L);
writeKernelMemory((void *)0x03858A04,0x3D200221L);
writeKernelMemory((void *)0x03858A08,0x61298A80L);
writeKernelMemory((void *)0x03858A0C,0x83420028L);
writeKernelMemory((void *)0x03858A10,0x8002002CL);
writeKernelMemory((void *)0x03858A14,0x7D2903A6L);
writeKernelMemory((void *)0x03858A18,0x81620030L);
writeKernelMemory((void *)0x03858A1C,0x38820008L);
writeKernelMemory((void *)0x03858A20,0x80C20034L);
writeKernelMemory((void *)0x03858A24,0x7FA5EB78L);
writeKernelMemory((void *)0x03858A28,0x80E20038L);
writeKernelMemory((void *)0x03858A2C,0x7FE3FB78L);
writeKernelMemory((void *)0x03858A30,0x8102003CL);
writeKernelMemory((void *)0x03858A34,0x81420040L);
writeKernelMemory((void *)0x03858A38,0x81220044L);
writeKernelMemory((void *)0x03858A3C,0x93420008L);
writeKernelMemory((void *)0x03858A40,0x9002000CL);
writeKernelMemory((void *)0x03858A44,0x91620010L);
writeKernelMemory((void *)0x03858A48,0x90C20014L);
writeKernelMemory((void *)0x03858A4C,0x90E20018L);
writeKernelMemory((void *)0x03858A50,0x9102001CL);
writeKernelMemory((void *)0x03858A54,0x91420020L);
writeKernelMemory((void *)0x03858A58,0x91220024L);
writeKernelMemory((void *)0x03858A5C,0x4E800421L);
writeKernelMemory((void *)0x03858A60,0x48000011L);
writeKernelMemory((void *)0x03858A64,0x0063006FL);
writeKernelMemory((void *)0x03858A68,0x006C006FL);
writeKernelMemory((void *)0x03858A6C,0x00720000L);
writeKernelMemory((void *)0x03858A70,0x7C8802A6L);
writeKernelMemory((void *)0x03858A74,0x7FC903A6L);
writeKernelMemory((void *)0x03858A78,0x38620028L);
writeKernelMemory((void *)0x03858A7C,0x4E800421L);
writeKernelMemory((void *)0x03858A80,0x3D200221L);
writeKernelMemory((void *)0x03858A84,0x6129AE10L);
writeKernelMemory((void *)0x03858A88,0x81620030L);
writeKernelMemory((void *)0x03858A8C,0x80C20034L);
writeKernelMemory((void *)0x03858A90,0x7D2903A6L);
writeKernelMemory((void *)0x03858A94,0x80E20038L);
writeKernelMemory((void *)0x03858A98,0x38820008L);
writeKernelMemory((void *)0x03858A9C,0x8102003CL);
writeKernelMemory((void *)0x03858AA0,0x38A00000L);
writeKernelMemory((void *)0x03858AA4,0x81420040L);
writeKernelMemory((void *)0x03858AA8,0x7FA3EB78L);
writeKernelMemory((void *)0x03858AAC,0x81220044L);
writeKernelMemory((void *)0x03858AB0,0x8002002CL);
writeKernelMemory((void *)0x03858AB4,0x83C20028L);
writeKernelMemory((void *)0x03858AB8,0x91620010L);
writeKernelMemory((void *)0x03858ABC,0x90C20014L);
writeKernelMemory((void *)0x03858AC0,0x90E20018L);
writeKernelMemory((void *)0x03858AC4,0x9102001CL);
writeKernelMemory((void *)0x03858AC8,0x91420020L);
writeKernelMemory((void *)0x03858ACC,0x91220024L);
writeKernelMemory((void *)0x03858AD0,0x9002000CL);
writeKernelMemory((void *)0x03858AD4,0x93C20008L);
writeKernelMemory((void *)0x03858AD8,0x3FC00248L);
writeKernelMemory((void *)0x03858ADC,0x63DE6698L);
writeKernelMemory((void *)0x03858AE0,0x4E800421L);
writeKernelMemory((void *)0x03858AE4,0x7FC903A6L);
writeKernelMemory((void *)0x03858AE8,0x3860012AL);
writeKernelMemory((void *)0x03858AEC,0x4E800421L);
writeKernelMemory((void *)0x03858AF0,0x7FC903A6L);
writeKernelMemory((void *)0x03858AF4,0x7C7A1B78L);
writeKernelMemory((void *)0x03858AF8,0x3860012BL);
writeKernelMemory((void *)0x03858AFC,0x4E800421L);
writeKernelMemory((void *)0x03858B00,0x7FC903A6L);
writeKernelMemory((void *)0x03858B04,0x7C791B78L);
writeKernelMemory((void *)0x03858B08,0x3860012CL);
writeKernelMemory((void *)0x03858B0C,0x4E800421L);
writeKernelMemory((void *)0x03858B10,0x7FC903A6L);
writeKernelMemory((void *)0x03858B14,0x7C781B78L);
writeKernelMemory((void *)0x03858B18,0x3860012DL);
writeKernelMemory((void *)0x03858B1C,0x3FC00248L);
writeKernelMemory((void *)0x03858B20,0x63DE61BCL);
writeKernelMemory((void *)0x03858B24,0x4E800421L);
writeKernelMemory((void *)0x03858B28,0x7F44D378L);
writeKernelMemory((void *)0x03858B2C,0x7C771B78L);
writeKernelMemory((void *)0x03858B30,0x7FC903A6L);
writeKernelMemory((void *)0x03858B34,0x38A00001L);
writeKernelMemory((void *)0x03858B38,0x38600000L);
writeKernelMemory((void *)0x03858B3C,0x4E800421L);
writeKernelMemory((void *)0x03858B40,0x7F24CB78L);
writeKernelMemory((void *)0x03858B44,0x7FC903A6L);
writeKernelMemory((void *)0x03858B48,0x38A00001L);
writeKernelMemory((void *)0x03858B4C,0x7C7A1B78L);
writeKernelMemory((void *)0x03858B50,0x38600000L);
writeKernelMemory((void *)0x03858B54,0x4E800421L);
writeKernelMemory((void *)0x03858B58,0x7F04C378L);
writeKernelMemory((void *)0x03858B5C,0x7FC903A6L);
writeKernelMemory((void *)0x03858B60,0x38A00001L);
writeKernelMemory((void *)0x03858B64,0x7C791B78L);
writeKernelMemory((void *)0x03858B68,0x38600000L);
writeKernelMemory((void *)0x03858B6C,0x4E800421L);
writeKernelMemory((void *)0x03858B70,0x7EE4BB78L);
writeKernelMemory((void *)0x03858B74,0x7FC903A6L);
writeKernelMemory((void *)0x03858B78,0x38A00001L);
writeKernelMemory((void *)0x03858B7C,0x7C781B78L);
writeKernelMemory((void *)0x03858B80,0x38600000L);
writeKernelMemory((void *)0x03858B84,0x4E800421L);
writeKernelMemory((void *)0x03858B88,0x3C801200L);
writeKernelMemory((void *)0x03858B8C,0x60841FBCL);
writeKernelMemory((void *)0x03858B90,0x3D201200L);
writeKernelMemory((void *)0x03858B94,0x81040000L);
writeKernelMemory((void *)0x03858B98,0x3CC01200L);
writeKernelMemory((void *)0x03858B9C,0x3CE01200L);
writeKernelMemory((void *)0x03858BA0,0x3D401200L);
writeKernelMemory((void *)0x03858BA4,0x2C080000L);
writeKernelMemory((void *)0x03858BA8,0x61290FBCL);
writeKernelMemory((void *)0x03858BAC,0x60C61FACL);
writeKernelMemory((void *)0x03858BB0,0x60E71FB0L);
writeKernelMemory((void *)0x03858BB4,0x614A1FB4L);
writeKernelMemory((void *)0x03858BB8,0x7C771B78L);
writeKernelMemory((void *)0x03858BBC,0x83C90000L);
writeKernelMemory((void *)0x03858BC0,0x80A60000L);
writeKernelMemory((void *)0x03858BC4,0x80670000L);
writeKernelMemory((void *)0x03858BC8,0x816A0000L);
writeKernelMemory((void *)0x03858BCC,0x40820178L);
writeKernelMemory((void *)0x03858BD0,0x3D401200L);
writeKernelMemory((void *)0x03858BD4,0x614A1FA0L);
writeKernelMemory((void *)0x03858BD8,0x810A0000L);
writeKernelMemory((void *)0x03858BDC,0x2C080000L);
writeKernelMemory((void *)0x03858BE0,0x4082019CL);
writeKernelMemory((void *)0x03858BE4,0x3D0000FFL);
writeKernelMemory((void *)0x03858BE8,0x38E00001L);
writeKernelMemory((void *)0x03858BEC,0x61080100L);
writeKernelMemory((void *)0x03858BF0,0x90EA0000L);
writeKernelMemory((void *)0x03858BF4,0x91090000L);
writeKernelMemory((void *)0x03858BF8,0x3FC000FFL);
writeKernelMemory((void *)0x03858BFC,0x48000011L);
writeKernelMemory((void *)0x03858C00,0x0063006FL);
writeKernelMemory((void *)0x03858C04,0x006C006FL);
writeKernelMemory((void *)0x03858C08,0x00720000L);
writeKernelMemory((void *)0x03858C0C,0x7C8802A6L);
writeKernelMemory((void *)0x03858C10,0x3D20020BL);
writeKernelMemory((void *)0x03858C14,0x38620028L);
writeKernelMemory((void *)0x03858C18,0x612908D4L);
writeKernelMemory((void *)0x03858C1C,0x7D2903A6L);
writeKernelMemory((void *)0x03858C20,0x4E800421L);
writeKernelMemory((void *)0x03858C24,0x3D200221L);
writeKernelMemory((void *)0x03858C28,0x6129AE10L);
writeKernelMemory((void *)0x03858C2C,0x80E20028L);
writeKernelMemory((void *)0x03858C30,0x8102002CL);
writeKernelMemory((void *)0x03858C34,0x7D2903A6L);
writeKernelMemory((void *)0x03858C38,0x81420030L);
writeKernelMemory((void *)0x03858C3C,0x7FC5F378L);
writeKernelMemory((void *)0x03858C40,0x81220034L);
writeKernelMemory((void *)0x03858C44,0x38820008L);
writeKernelMemory((void *)0x03858C48,0x90E20008L);
writeKernelMemory((void *)0x03858C4C,0x7FA3EB78L);
writeKernelMemory((void *)0x03858C50,0x80E20038L);
writeKernelMemory((void *)0x03858C54,0x3FC00248L);
writeKernelMemory((void *)0x03858C58,0x9102000CL);
writeKernelMemory((void *)0x03858C5C,0x63DEE080L);
writeKernelMemory((void *)0x03858C60,0x8102003CL);
writeKernelMemory((void *)0x03858C64,0x91420010L);
writeKernelMemory((void *)0x03858C68,0x91220014L);
writeKernelMemory((void *)0x03858C6C,0x81420040L);
writeKernelMemory((void *)0x03858C70,0x81220044L);
writeKernelMemory((void *)0x03858C74,0x91420020L);
writeKernelMemory((void *)0x03858C78,0x91220024L);
writeKernelMemory((void *)0x03858C7C,0x90E20018L);
writeKernelMemory((void *)0x03858C80,0x9102001CL);
writeKernelMemory((void *)0x03858C84,0x4E800421L);
writeKernelMemory((void *)0x03858C88,0x7FE4FB78L);
writeKernelMemory((void *)0x03858C8C,0x7FC903A6L);
writeKernelMemory((void *)0x03858C90,0x7F43D378L);
writeKernelMemory((void *)0x03858C94,0x4E800421L);
writeKernelMemory((void *)0x03858C98,0x7FE4FB78L);
writeKernelMemory((void *)0x03858C9C,0x7FC903A6L);
writeKernelMemory((void *)0x03858CA0,0x7F23CB78L);
writeKernelMemory((void *)0x03858CA4,0x4E800421L);
writeKernelMemory((void *)0x03858CA8,0x7FE4FB78L);
writeKernelMemory((void *)0x03858CAC,0x7FC903A6L);
writeKernelMemory((void *)0x03858CB0,0x7F03C378L);
writeKernelMemory((void *)0x03858CB4,0x4E800421L);
writeKernelMemory((void *)0x03858CB8,0x7FE4FB78L);
writeKernelMemory((void *)0x03858CBC,0x7FC903A6L);
writeKernelMemory((void *)0x03858CC0,0x7EE3BB78L);
writeKernelMemory((void *)0x03858CC4,0x3FE02000L);
writeKernelMemory((void *)0x03858CC8,0x63FF00A0L);
writeKernelMemory((void *)0x03858CCC,0x4E800421L);
writeKernelMemory((void *)0x03858CD0,0x815F0000L);
writeKernelMemory((void *)0x03858CD4,0x3D20109CL);
writeKernelMemory((void *)0x03858CD8,0x280A0001L);
writeKernelMemory((void *)0x03858CDC,0x6129D8E4L);
writeKernelMemory((void *)0x03858CE0,0x83C90000L);
writeKernelMemory((void *)0x03858CE4,0x418200E8L);
writeKernelMemory((void *)0x03858CE8,0x2C0A0000L);
writeKernelMemory((void *)0x03858CEC,0x40820038L);
writeKernelMemory((void *)0x03858CF0,0x3D200316L);
writeKernelMemory((void *)0x03858CF4,0x7FC3F378L);
writeKernelMemory((void *)0x03858CF8,0x61296E04L);
writeKernelMemory((void *)0x03858CFC,0x7D2903A6L);
writeKernelMemory((void *)0x03858D00,0x4E800421L);
writeKernelMemory((void *)0x03858D04,0x3D202000L);
writeKernelMemory((void *)0x03858D08,0x33640005L);
writeKernelMemory((void *)0x03858D0C,0x61290090L);
writeKernelMemory((void *)0x03858D10,0x7F830194L);
writeKernelMemory((void *)0x03858D14,0x39400001L);
writeKernelMemory((void *)0x03858D18,0x93890000L);
writeKernelMemory((void *)0x03858D1C,0x93690004L);
writeKernelMemory((void *)0x03858D20,0x915F0000L);
writeKernelMemory((void *)0x03858D24,0x3D200316L);
writeKernelMemory((void *)0x03858D28,0x7FC3F378L);
writeKernelMemory((void *)0x03858D2C,0x61296E04L);
writeKernelMemory((void *)0x03858D30,0x7D2903A6L);
writeKernelMemory((void *)0x03858D34,0x4E800421L);
writeKernelMemory((void *)0x03858D38,0x7C1C1840L);
writeKernelMemory((void *)0x03858D3C,0x408100BCL);
writeKernelMemory((void *)0x03858D40,0x480003ACL);
writeKernelMemory((void *)0x03858D44,0x2C080001L);
writeKernelMemory((void *)0x03858D48,0x40820050L);
writeKernelMemory((void *)0x03858D4C,0x3D401200L);
writeKernelMemory((void *)0x03858D50,0x614A1FA4L);
writeKernelMemory((void *)0x03858D54,0x80EA0000L);
writeKernelMemory((void *)0x03858D58,0x2C070000L);
writeKernelMemory((void *)0x03858D5C,0x41820234L);
writeKernelMemory((void *)0x03858D60,0x39400000L);
writeKernelMemory((void *)0x03858D64,0x614AFEFFL);
writeKernelMemory((void *)0x03858D68,0x7C1E5000L);
writeKernelMemory((void *)0x03858D6C,0x41810278L);
writeKernelMemory((void *)0x03858D70,0x39200002L);
writeKernelMemory((void *)0x03858D74,0x91240000L);
writeKernelMemory((void *)0x03858D78,0x4BFFFE84L);
writeKernelMemory((void *)0x03858D7C,0x3D4000FFL);
writeKernelMemory((void *)0x03858D80,0x614AFF00L);
writeKernelMemory((void *)0x03858D84,0x7C1E5000L);
writeKernelMemory((void *)0x03858D88,0x41810224L);
writeKernelMemory((void *)0x03858D8C,0x395E0100L);
writeKernelMemory((void *)0x03858D90,0x91490000L);
writeKernelMemory((void *)0x03858D94,0x4BFFFE68L);
writeKernelMemory((void *)0x03858D98,0x2C080002L);
writeKernelMemory((void *)0x03858D9C,0x408201B0L);
writeKernelMemory((void *)0x03858DA0,0x3D401200L);
writeKernelMemory((void *)0x03858DA4,0x614A1FA8L);
writeKernelMemory((void *)0x03858DA8,0x810A0000L);
writeKernelMemory((void *)0x03858DAC,0x2C080000L);
writeKernelMemory((void *)0x03858DB0,0x418201C0L);
writeKernelMemory((void *)0x03858DB4,0x3D400001L);
writeKernelMemory((void *)0x03858DB8,0x7C1E5000L);
writeKernelMemory((void *)0x03858DBC,0x4080021CL);
writeKernelMemory((void *)0x03858DC0,0x395E0001L);
writeKernelMemory((void *)0x03858DC4,0x91490000L);
writeKernelMemory((void *)0x03858DC8,0x4BFFFE34L);
writeKernelMemory((void *)0x03858DCC,0x3D202000L);
writeKernelMemory((void *)0x03858DD0,0x7FC3F378L);
writeKernelMemory((void *)0x03858DD4,0x61290090L);
writeKernelMemory((void *)0x03858DD8,0x83890000L);
writeKernelMemory((void *)0x03858DDC,0x83690004L);
writeKernelMemory((void *)0x03858DE0,0x3D200316L);
writeKernelMemory((void *)0x03858DE4,0x61296E04L);
writeKernelMemory((void *)0x03858DE8,0x7D2903A6L);
writeKernelMemory((void *)0x03858DEC,0x4E800421L);
writeKernelMemory((void *)0x03858DF0,0x7C1C1840L);
writeKernelMemory((void *)0x03858DF4,0x4181FF4CL);
writeKernelMemory((void *)0x03858DF8,0x4082000CL);
writeKernelMemory((void *)0x03858DFC,0x7C1B2040L);
writeKernelMemory((void *)0x03858E00,0x4181FF40L);
writeKernelMemory((void *)0x03858E04,0x3D002000L);
writeKernelMemory((void *)0x03858E08,0x3D402000L);
writeKernelMemory((void *)0x03858E0C,0x3D202000L);
writeKernelMemory((void *)0x03858E10,0x3FE0028BL);
writeKernelMemory((void *)0x03858E14,0x3BA00000L);
writeKernelMemory((void *)0x03858E18,0x610800A0L);
writeKernelMemory((void *)0x03858E1C,0x614A0090L);
writeKernelMemory((void *)0x03858E20,0x63FF09F8L);
writeKernelMemory((void *)0x03858E24,0x61290094L);
writeKernelMemory((void *)0x03858E28,0x93A80000L);
writeKernelMemory((void *)0x03858E2C,0x7FE903A6L);
writeKernelMemory((void *)0x03858E30,0x93AA0000L);
writeKernelMemory((void *)0x03858E34,0x38A20048L);
writeKernelMemory((void *)0x03858E38,0x93A90000L);
writeKernelMemory((void *)0x03858E3C,0x38800005L);
writeKernelMemory((void *)0x03858E40,0x93A2004CL);
writeKernelMemory((void *)0x03858E44,0x93420048L);
writeKernelMemory((void *)0x03858E48,0x38600000L);
writeKernelMemory((void *)0x03858E4C,0x3FC0109CL);
writeKernelMemory((void *)0x03858E50,0x63DED8E4L);
writeKernelMemory((void *)0x03858E54,0x4E800421L);
writeKernelMemory((void *)0x03858E58,0x93A2004CL);
writeKernelMemory((void *)0x03858E5C,0x7C691B78L);
writeKernelMemory((void *)0x03858E60,0x93220048L);
writeKernelMemory((void *)0x03858E64,0x7FE903A6L);
writeKernelMemory((void *)0x03858E68,0x38A20048L);
writeKernelMemory((void *)0x03858E6C,0x38800006L);
writeKernelMemory((void *)0x03858E70,0x38600000L);
writeKernelMemory((void *)0x03858E74,0x7D394B78L);
writeKernelMemory((void *)0x03858E78,0x4E800421L);
writeKernelMemory((void *)0x03858E7C,0x93A2004CL);
writeKernelMemory((void *)0x03858E80,0x93020048L);
writeKernelMemory((void *)0x03858E84,0x7C7A1B78L);
writeKernelMemory((void *)0x03858E88,0x7FE903A6L);
writeKernelMemory((void *)0x03858E8C,0x38A20048L);
writeKernelMemory((void *)0x03858E90,0x38800007L);
writeKernelMemory((void *)0x03858E94,0x38600000L);
writeKernelMemory((void *)0x03858E98,0x4E800421L);
writeKernelMemory((void *)0x03858E9C,0x38A20048L);
writeKernelMemory((void *)0x03858EA0,0x93A2004CL);
writeKernelMemory((void *)0x03858EA4,0x7C7B1B78L);
writeKernelMemory((void *)0x03858EA8,0x92E20048L);
writeKernelMemory((void *)0x03858EAC,0x7FE903A6L);
writeKernelMemory((void *)0x03858EB0,0x38800008L);
writeKernelMemory((void *)0x03858EB4,0x38600000L);
writeKernelMemory((void *)0x03858EB8,0x3FE00304L);
writeKernelMemory((void *)0x03858EBC,0x63FFA5D8L);
writeKernelMemory((void *)0x03858EC0,0x4E800421L);
writeKernelMemory((void *)0x03858EC4,0x813E0000L);
writeKernelMemory((void *)0x03858EC8,0x7FE903A6L);
writeKernelMemory((void *)0x03858ECC,0x38820050L);
writeKernelMemory((void *)0x03858ED0,0x81290034L);
writeKernelMemory((void *)0x03858ED4,0x7C7C1B78L);
writeKernelMemory((void *)0x03858ED8,0x80690878L);
writeKernelMemory((void *)0x03858EDC,0x93220050L);
writeKernelMemory((void *)0x03858EE0,0x93A20054L);
writeKernelMemory((void *)0x03858EE4,0x4E800421L);
writeKernelMemory((void *)0x03858EE8,0x813E0000L);
writeKernelMemory((void *)0x03858EEC,0x7FE903A6L);
writeKernelMemory((void *)0x03858EF0,0x38820050L);
writeKernelMemory((void *)0x03858EF4,0x81290034L);
writeKernelMemory((void *)0x03858EF8,0x80690878L);
writeKernelMemory((void *)0x03858EFC,0x93420050L);
writeKernelMemory((void *)0x03858F00,0x93A20054L);
writeKernelMemory((void *)0x03858F04,0x4E800421L);
writeKernelMemory((void *)0x03858F08,0x813E0000L);
writeKernelMemory((void *)0x03858F0C,0x7FE903A6L);
writeKernelMemory((void *)0x03858F10,0x38820050L);
writeKernelMemory((void *)0x03858F14,0x81290034L);
writeKernelMemory((void *)0x03858F18,0x80690878L);
writeKernelMemory((void *)0x03858F1C,0x93620050L);
writeKernelMemory((void *)0x03858F20,0x93A20054L);
writeKernelMemory((void *)0x03858F24,0x4E800421L);
writeKernelMemory((void *)0x03858F28,0x813E0000L);
writeKernelMemory((void *)0x03858F2C,0x38820050L);
writeKernelMemory((void *)0x03858F30,0x7FE903A6L);
writeKernelMemory((void *)0x03858F34,0x81290034L);
writeKernelMemory((void *)0x03858F38,0x80690878L);
writeKernelMemory((void *)0x03858F3C,0x93820050L);
writeKernelMemory((void *)0x03858F40,0x93A20054L);
writeKernelMemory((void *)0x03858F44,0x4E800421L);
writeKernelMemory((void *)0x03858F48,0x480001A4L);
writeKernelMemory((void *)0x03858F4C,0x2C080003L);
writeKernelMemory((void *)0x03858F50,0x408200ACL);
writeKernelMemory((void *)0x03858F54,0x2C050000L);
writeKernelMemory((void *)0x03858F58,0x41820060L);
writeKernelMemory((void *)0x03858F5C,0x2C1E00FEL);
writeKernelMemory((void *)0x03858F60,0x41810090L);
writeKernelMemory((void *)0x03858F64,0x39200004L);
writeKernelMemory((void *)0x03858F68,0x91240000L);
writeKernelMemory((void *)0x03858F6C,0x4BFFFC90L);
writeKernelMemory((void *)0x03858F70,0x39000000L);
writeKernelMemory((void *)0x03858F74,0x38E00001L);
writeKernelMemory((void *)0x03858F78,0x6108FF01L);
writeKernelMemory((void *)0x03858F7C,0x3BC00000L);
writeKernelMemory((void *)0x03858F80,0x90EA0000L);
writeKernelMemory((void *)0x03858F84,0x63DEFF00L);
writeKernelMemory((void *)0x03858F88,0x91090000L);
writeKernelMemory((void *)0x03858F8C,0x4BFFFC70L);
writeKernelMemory((void *)0x03858F90,0x3CE000FEL);
writeKernelMemory((void *)0x03858F94,0x3FC000FFL);
writeKernelMemory((void *)0x03858F98,0x60E7FF00L);
writeKernelMemory((void *)0x03858F9C,0x910A0000L);
writeKernelMemory((void *)0x03858FA0,0x63DEFF00L);
writeKernelMemory((void *)0x03858FA4,0x90E90000L);
writeKernelMemory((void *)0x03858FA8,0x4BFFFC54L);
writeKernelMemory((void *)0x03858FAC,0x39200001L);
writeKernelMemory((void *)0x03858FB0,0x91240000L);
writeKernelMemory((void *)0x03858FB4,0x4BFFFC48L);
writeKernelMemory((void *)0x03858FB8,0x39400000L);
writeKernelMemory((void *)0x03858FBC,0x39000001L);
writeKernelMemory((void *)0x03858FC0,0x614AFEFFL);
writeKernelMemory((void *)0x03858FC4,0x3BC00000L);
writeKernelMemory((void *)0x03858FC8,0x91060000L);
writeKernelMemory((void *)0x03858FCC,0x63DEFFFFL);
writeKernelMemory((void *)0x03858FD0,0x91490000L);
writeKernelMemory((void *)0x03858FD4,0x4BFFFC28L);
writeKernelMemory((void *)0x03858FD8,0x39200003L);
writeKernelMemory((void *)0x03858FDC,0x91240000L);
writeKernelMemory((void *)0x03858FE0,0x4BFFFC1CL);
writeKernelMemory((void *)0x03858FE4,0x3D5EFFFFL);
writeKernelMemory((void *)0x03858FE8,0x91490000L);
writeKernelMemory((void *)0x03858FEC,0x4BFFFC10L);
writeKernelMemory((void *)0x03858FF0,0x395EFF00L);
writeKernelMemory((void *)0x03858FF4,0x91490000L);
writeKernelMemory((void *)0x03858FF8,0x4BFFFC04L);
writeKernelMemory((void *)0x03858FFC,0x2C080004L);
writeKernelMemory((void *)0x03859000,0x40820050L);
writeKernelMemory((void *)0x03859004,0x2C030000L);
writeKernelMemory((void *)0x03859008,0x41820020L);
writeKernelMemory((void *)0x0385900C,0x3D4000FFL);
writeKernelMemory((void *)0x03859010,0x614A00FFL);
writeKernelMemory((void *)0x03859014,0x7C1E5000L);
writeKernelMemory((void *)0x03859018,0x4081002CL);
writeKernelMemory((void *)0x0385901C,0x39200005L);
writeKernelMemory((void *)0x03859020,0x91240000L);
writeKernelMemory((void *)0x03859024,0x4BFFFBD8L);
writeKernelMemory((void *)0x03859028,0x3D400001L);
writeKernelMemory((void *)0x0385902C,0x39000001L);
writeKernelMemory((void *)0x03859030,0x614A00FFL);
writeKernelMemory((void *)0x03859034,0x91070000L);
writeKernelMemory((void *)0x03859038,0x3BC000FFL);
writeKernelMemory((void *)0x0385903C,0x91490000L);
writeKernelMemory((void *)0x03859040,0x4BFFFBBCL);
writeKernelMemory((void *)0x03859044,0x3D5E0001L);
writeKernelMemory((void *)0x03859048,0x91490000L);
writeKernelMemory((void *)0x0385904C,0x4BFFFBB0L);
writeKernelMemory((void *)0x03859050,0x2C080005L);
writeKernelMemory((void *)0x03859054,0x4082002CL);
writeKernelMemory((void *)0x03859058,0x2C0B0000L);
writeKernelMemory((void *)0x0385905C,0x41820068L);
writeKernelMemory((void *)0x03859060,0x3D2000FFL);
writeKernelMemory((void *)0x03859064,0x7C1E4800L);
writeKernelMemory((void *)0x03859068,0x41800078L);
writeKernelMemory((void *)0x0385906C,0x395EFFFFL);
writeKernelMemory((void *)0x03859070,0x3D201200L);
writeKernelMemory((void *)0x03859074,0x61290FBCL);
writeKernelMemory((void *)0x03859078,0x91490000L);
writeKernelMemory((void *)0x0385907C,0x4BFFFB80L);
writeKernelMemory((void *)0x03859080,0x2C080006L);
writeKernelMemory((void *)0x03859084,0x4082FB78L);
writeKernelMemory((void *)0x03859088,0x3D001200L);
writeKernelMemory((void *)0x0385908C,0x39200000L);
writeKernelMemory((void *)0x03859090,0x61081FA0L);
writeKernelMemory((void *)0x03859094,0x3CA01200L);
writeKernelMemory((void *)0x03859098,0x91280000L);
writeKernelMemory((void *)0x0385909C,0x3D001200L);
writeKernelMemory((void *)0x038590A0,0x60A51FA4L);
writeKernelMemory((void *)0x038590A4,0x61081FA8L);
writeKernelMemory((void *)0x038590A8,0x91240000L);
writeKernelMemory((void *)0x038590AC,0x91260000L);
writeKernelMemory((void *)0x038590B0,0x91270000L);
writeKernelMemory((void *)0x038590B4,0x912A0000L);
writeKernelMemory((void *)0x038590B8,0x91250000L);
writeKernelMemory((void *)0x038590BC,0x91280000L);
writeKernelMemory((void *)0x038590C0,0x4BFFFB3CL);
writeKernelMemory((void *)0x038590C4,0x39000001L);
writeKernelMemory((void *)0x038590C8,0x3D2000FFL);
writeKernelMemory((void *)0x038590CC,0x3FC000FFL);
writeKernelMemory((void *)0x038590D0,0x910A0000L);
writeKernelMemory((void *)0x038590D4,0x63DE00FFL);
writeKernelMemory((void *)0x038590D8,0x612A00FEL);
writeKernelMemory((void *)0x038590DC,0x4BFFFF94L);
writeKernelMemory((void *)0x038590E0,0x39200006L);
writeKernelMemory((void *)0x038590E4,0x91240000L);
writeKernelMemory((void *)0x038590E8,0x4BFFFB14L);
writeKernelMemory((void *)0x038590EC,0x3C401003L);
writeKernelMemory((void *)0x038590F0,0x38420A00L);
writeKernelMemory((void *)0x038590F4,0x81820000L);
writeKernelMemory((void *)0x038590F8,0x7D8803A6L);
writeKernelMemory((void *)0x038590FC,0x80620004L);
writeKernelMemory((void *)0x03859100,0x82820008L);
writeKernelMemory((void *)0x03859104,0x82A2000CL);
writeKernelMemory((void *)0x03859108,0x82C20010L);
writeKernelMemory((void *)0x0385910C,0x82E20014L);
writeKernelMemory((void *)0x03859110,0x83020018L);
writeKernelMemory((void *)0x03859114,0x8322001CL);
writeKernelMemory((void *)0x03859118,0x83420020L);
writeKernelMemory((void *)0x0385911C,0x83620024L);
writeKernelMemory((void *)0x03859120,0x83820028L);
writeKernelMemory((void *)0x03859124,0x83A2002CL);
writeKernelMemory((void *)0x03859128,0x83C20030L);
writeKernelMemory((void *)0x0385912C,0x83E20034L);
writeKernelMemory((void *)0x03859130,0x80020038L);
writeKernelMemory((void *)0x03859134,0x4B98BFD0L);
writeKernelMemory((void *)0x031E5100,0x48673850L);
writeKernelMemory((void *)0x04110000,0x3C401400L);
writeKernelMemory((void *)0x04110004,0x7D8802A6L);
writeKernelMemory((void *)0x04110008,0x91820000L);
writeKernelMemory((void *)0x0411000C,0x8182FFFCL);
writeKernelMemory((void *)0x04110010,0x398C0001L);
writeKernelMemory((void *)0x04110014,0x9182FFFCL);
writeKernelMemory((void *)0x04110018,0x2C0C001EL);
writeKernelMemory((void *)0x0411001C,0x40800110L);
writeKernelMemory((void *)0x04110020,0x2C0C0001L);
writeKernelMemory((void *)0x04110024,0x40820110L);
writeKernelMemory((void *)0x04110028,0x3980FFFAL);
writeKernelMemory((void *)0x0411002C,0x91820020L);
writeKernelMemory((void *)0x04110030,0x3980FFFAL);
writeKernelMemory((void *)0x04110034,0x91820024L);
writeKernelMemory((void *)0x04110038,0x3980FFFAL);
writeKernelMemory((void *)0x0411003C,0x91820028L);
writeKernelMemory((void *)0x04110040,0x38600000L);
writeKernelMemory((void *)0x04110044,0x38800001L);
writeKernelMemory((void *)0x04110048,0x38A20010L);
writeKernelMemory((void *)0x0411004C,0x81790034L);
writeKernelMemory((void *)0x04110050,0x818B0054L);
writeKernelMemory((void *)0x04110054,0x81220020L);
writeKernelMemory((void *)0x04110058,0x7D8C4A14L);
writeKernelMemory((void *)0x0411005C,0x91850000L);
writeKernelMemory((void *)0x04110060,0x818B0058L);
writeKernelMemory((void *)0x04110064,0x81220024L);
writeKernelMemory((void *)0x04110068,0x7D8C4A14L);
writeKernelMemory((void *)0x0411006C,0x91850004L);
writeKernelMemory((void *)0x04110070,0x818B005CL);
writeKernelMemory((void *)0x04110074,0x81220028L);
writeKernelMemory((void *)0x04110078,0x7D8C4A14L);
writeKernelMemory((void *)0x0411007C,0x91850008L);
writeKernelMemory((void *)0x04110080,0x3CC0109CL);
writeKernelMemory((void *)0x04110084,0x80C646A0L);
writeKernelMemory((void *)0x04110088,0x3D80028AL);
writeKernelMemory((void *)0x0411008C,0x618CEC5CL);
writeKernelMemory((void *)0x04110090,0x7D8903A6L);
writeKernelMemory((void *)0x04110094,0x4E800421L);
writeKernelMemory((void *)0x04110098,0x480000B1L);
writeKernelMemory((void *)0x0411009C,0x38600000L);
writeKernelMemory((void *)0x041100A0,0x38800000L);
writeKernelMemory((void *)0x041100A4,0x38A20010L);
writeKernelMemory((void *)0x041100A8,0x3CC0109CL);
writeKernelMemory((void *)0x041100AC,0x80C646A0L);
writeKernelMemory((void *)0x041100B0,0x3D80028AL);
writeKernelMemory((void *)0x041100B4,0x618CEC5CL);
writeKernelMemory((void *)0x041100B8,0x7D8903A6L);
writeKernelMemory((void *)0x041100BC,0x4E800421L);
writeKernelMemory((void *)0x041100C0,0x48000089L);
writeKernelMemory((void *)0x041100C4,0x38600000L);
writeKernelMemory((void *)0x041100C8,0x38800002L);
writeKernelMemory((void *)0x041100CC,0x38A20010L);
writeKernelMemory((void *)0x041100D0,0x3CC0109CL);
writeKernelMemory((void *)0x041100D4,0x80C646A0L);
writeKernelMemory((void *)0x041100D8,0x3D80028AL);
writeKernelMemory((void *)0x041100DC,0x618CEC5CL);
writeKernelMemory((void *)0x041100E0,0x7D8903A6L);
writeKernelMemory((void *)0x041100E4,0x4E800421L);
writeKernelMemory((void *)0x041100E8,0x48000061L);
writeKernelMemory((void *)0x041100EC,0x81820028L);
writeKernelMemory((void *)0x041100F0,0x398C0001L);
writeKernelMemory((void *)0x041100F4,0x91820028L);
writeKernelMemory((void *)0x041100F8,0x2C0C0006L);
writeKernelMemory((void *)0x041100FC,0x4180FF44L);
writeKernelMemory((void *)0x04110100,0x81820024L);
writeKernelMemory((void *)0x04110104,0x398C0001L);
writeKernelMemory((void *)0x04110108,0x91820024L);
writeKernelMemory((void *)0x0411010C,0x2C0C0006L);
writeKernelMemory((void *)0x04110110,0x4180FF28L);
writeKernelMemory((void *)0x04110114,0x81820020L);
writeKernelMemory((void *)0x04110118,0x398C0001L);
writeKernelMemory((void *)0x0411011C,0x91820020L);
writeKernelMemory((void *)0x04110120,0x2C0C0006L);
writeKernelMemory((void *)0x04110124,0x4180FF0CL);
writeKernelMemory((void *)0x04110128,0x4800000CL);
writeKernelMemory((void *)0x0411012C,0x39200000L);
writeKernelMemory((void *)0x04110130,0x9122FFFCL);
writeKernelMemory((void *)0x04110134,0x81820000L);
writeKernelMemory((void *)0x04110138,0x7D8803A6L);
writeKernelMemory((void *)0x0411013C,0x3C401003L);
writeKernelMemory((void *)0x04110140,0x38425000L);
writeKernelMemory((void *)0x04110144,0x4E800020L);
writeKernelMemory((void *)0x04110148,0x90620E78L);
writeKernelMemory((void *)0x0411014C,0x80790034L);
writeKernelMemory((void *)0x04110150,0x80630878L);
writeKernelMemory((void *)0x04110154,0x38820E78L);
writeKernelMemory((void *)0x04110158,0x3D800304L);
writeKernelMemory((void *)0x0411015C,0x618CA5D8L);
writeKernelMemory((void *)0x04110160,0x7D8903A6L);
writeKernelMemory((void *)0x04110164,0x4E800420L);
writeKernelMemory((void *)0x04CC0000,0x3C401003L);
writeKernelMemory((void *)0x04CC0004,0x38425000L);
writeKernelMemory((void *)0x04CC0008,0x7C0802A6L);
writeKernelMemory((void *)0x04CC000C,0x90020004L);
writeKernelMemory((void *)0x04CC0010,0x9282FFB8L);
writeKernelMemory((void *)0x04CC0014,0x92A2FFBCL);
writeKernelMemory((void *)0x04CC0018,0x92C2FFC0L);
writeKernelMemory((void *)0x04CC001C,0x92E2FFC4L);
writeKernelMemory((void *)0x04CC0020,0x9302FFC8L);
writeKernelMemory((void *)0x04CC0024,0x9322FFCCL);
writeKernelMemory((void *)0x04CC0028,0x9342FFD0L);
writeKernelMemory((void *)0x04CC002C,0x9362FFD4L);
writeKernelMemory((void *)0x04CC0030,0x9382FFD8L);
writeKernelMemory((void *)0x04CC0034,0x93A2FFDCL);
writeKernelMemory((void *)0x04CC0038,0x93C2FFE0L);
writeKernelMemory((void *)0x04CC003C,0x93E2FFE4L);
writeKernelMemory((void *)0x04CC0040,0x81820A58L);
writeKernelMemory((void *)0x04CC0044,0x398C0001L);
writeKernelMemory((void *)0x04CC0048,0x91820A58L);
writeKernelMemory((void *)0x04CC004C,0x3D804100L);
writeKernelMemory((void *)0x04CC0050,0x9182FF00L);
writeKernelMemory((void *)0x04CC0054,0x3D80419AL);
writeKernelMemory((void *)0x04CC0058,0x9182FF08L);
writeKernelMemory((void *)0x04CC005C,0x3E80102FL);
writeKernelMemory((void *)0x04CC0060,0x8294FA64L);
writeKernelMemory((void *)0x04CC0064,0x3D804130L);
writeKernelMemory((void *)0x04CC0068,0x9182042CL);
writeKernelMemory((void *)0x04CC006C,0x3D80423AL);
writeKernelMemory((void *)0x04CC0070,0x9182FBD8L);
writeKernelMemory((void *)0x04CC0074,0x9182FE50L);
writeKernelMemory((void *)0x04CC0078,0x3D8042F5L);
writeKernelMemory((void *)0x04CC007C,0x9182102CL);
writeKernelMemory((void *)0x04CC0080,0x3D80423AL);
writeKernelMemory((void *)0x04CC0084,0x91820030L);
writeKernelMemory((void *)0x04CC0088,0x3F20109DL);
writeKernelMemory((void *)0x04CC008C,0x8339D8E4L);
writeKernelMemory((void *)0x04CC0090,0x8122FB58L);
writeKernelMemory((void *)0x04CC0094,0x2C090000L);
writeKernelMemory((void *)0x04CC0098,0x418202A8L);
writeKernelMemory((void *)0x04CC009C,0x819900C8L);
writeKernelMemory((void *)0x04CC00A0,0x2C0C0000L);
writeKernelMemory((void *)0x04CC00A4,0x4182029CL);
writeKernelMemory((void *)0x04CC00A8,0x91A201F0L);
writeKernelMemory((void *)0x04CC00AC,0x91C201F4L);
writeKernelMemory((void *)0x04CC00B0,0x91E201F8L);
writeKernelMemory((void *)0x04CC00B4,0x920201FCL);
writeKernelMemory((void *)0x04CC00B8,0x92220200L);
writeKernelMemory((void *)0x04CC00BC,0x92820204L);
writeKernelMemory((void *)0x04CC00C0,0x92A20208L);
writeKernelMemory((void *)0x04CC00C4,0x92C2020CL);
writeKernelMemory((void *)0x04CC00C8,0x92E20210L);
writeKernelMemory((void *)0x04CC00CC,0x93020214L);
writeKernelMemory((void *)0x04CC00D0,0x93220218L);
writeKernelMemory((void *)0x04CC00D4,0x9342021CL);
writeKernelMemory((void *)0x04CC00D8,0x93620220L);
writeKernelMemory((void *)0x04CC00DC,0x93820224L);
writeKernelMemory((void *)0x04CC00E0,0x3D80109EL);
writeKernelMemory((void *)0x04CC00E4,0x818CF84CL);
writeKernelMemory((void *)0x04CC00E8,0x81AC0450L);
writeKernelMemory((void *)0x04CC00EC,0x81CC046CL);
writeKernelMemory((void *)0x04CC00F0,0x81EC0440L);
writeKernelMemory((void *)0x04CC00F4,0x820C042CL);
writeKernelMemory((void *)0x04CC00F8,0x822C0430L);
writeKernelMemory((void *)0x04CC00FC,0x81990034L);
writeKernelMemory((void *)0x04CC0100,0x83AC05F0L);
writeKernelMemory((void *)0x04CC0104,0x815D006CL);
writeKernelMemory((void *)0x04CC0108,0x819D0008L);
writeKernelMemory((void *)0x04CC010C,0x828C0000L);
writeKernelMemory((void *)0x04CC0110,0x82940010L);
writeKernelMemory((void *)0x04CC0114,0x82AC0008L);
writeKernelMemory((void *)0x04CC0118,0x82B50010L);
writeKernelMemory((void *)0x04CC011C,0x82CC0010L);
writeKernelMemory((void *)0x04CC0120,0x82D60010L);
writeKernelMemory((void *)0x04CC0124,0x82EC0018L);
writeKernelMemory((void *)0x04CC0128,0x82F70010L);
writeKernelMemory((void *)0x04CC012C,0x830C0020L);
writeKernelMemory((void *)0x04CC0130,0x83180010L);
writeKernelMemory((void *)0x04CC0134,0x832C0028L);
writeKernelMemory((void *)0x04CC0138,0x83390010L);
writeKernelMemory((void *)0x04CC013C,0x834C0030L);
writeKernelMemory((void *)0x04CC0140,0x835A0010L);
writeKernelMemory((void *)0x04CC0144,0x836C0038L);
writeKernelMemory((void *)0x04CC0148,0x837B0010L);
writeKernelMemory((void *)0x04CC014C,0x838C0040L);
writeKernelMemory((void *)0x04CC0150,0x839C0010L);
writeKernelMemory((void *)0x04CC0154,0x7C0DA040L);
writeKernelMemory((void *)0x04CC0158,0x41820168L);
writeKernelMemory((void *)0x04CC015C,0x7C0EA040L);
writeKernelMemory((void *)0x04CC0160,0x41820160L);
writeKernelMemory((void *)0x04CC0164,0x7C0FA040L);
writeKernelMemory((void *)0x04CC0168,0x41820158L);
writeKernelMemory((void *)0x04CC016C,0x7C10A040L);
writeKernelMemory((void *)0x04CC0170,0x41820150L);
writeKernelMemory((void *)0x04CC0174,0x7C11A040L);
writeKernelMemory((void *)0x04CC0178,0x41820148L);
writeKernelMemory((void *)0x04CC017C,0x7C0DA840L);
writeKernelMemory((void *)0x04CC0180,0x41820148L);
writeKernelMemory((void *)0x04CC0184,0x7C0EA840L);
writeKernelMemory((void *)0x04CC0188,0x41820140L);
writeKernelMemory((void *)0x04CC018C,0x7C0FA840L);
writeKernelMemory((void *)0x04CC0190,0x41820138L);
writeKernelMemory((void *)0x04CC0194,0x7C10A840L);
writeKernelMemory((void *)0x04CC0198,0x41820130L);
writeKernelMemory((void *)0x04CC019C,0x7C11A840L);
writeKernelMemory((void *)0x04CC01A0,0x41820128L);
writeKernelMemory((void *)0x04CC01A4,0x7C0DB040L);
writeKernelMemory((void *)0x04CC01A8,0x41820128L);
writeKernelMemory((void *)0x04CC01AC,0x7C0EB040L);
writeKernelMemory((void *)0x04CC01B0,0x41820120L);
writeKernelMemory((void *)0x04CC01B4,0x7C0FB040L);
writeKernelMemory((void *)0x04CC01B8,0x41820118L);
writeKernelMemory((void *)0x04CC01BC,0x7C10B040L);
writeKernelMemory((void *)0x04CC01C0,0x41820110L);
writeKernelMemory((void *)0x04CC01C4,0x7C11B040L);
writeKernelMemory((void *)0x04CC01C8,0x41820108L);
writeKernelMemory((void *)0x04CC01CC,0x7C0DB840L);
writeKernelMemory((void *)0x04CC01D0,0x41820108L);
writeKernelMemory((void *)0x04CC01D4,0x7C0EB840L);
writeKernelMemory((void *)0x04CC01D8,0x41820100L);
writeKernelMemory((void *)0x04CC01DC,0x7C0FB840L);
writeKernelMemory((void *)0x04CC01E0,0x418200F8L);
writeKernelMemory((void *)0x04CC01E4,0x7C10B840L);
writeKernelMemory((void *)0x04CC01E8,0x418200F0L);
writeKernelMemory((void *)0x04CC01EC,0x7C11B840L);
writeKernelMemory((void *)0x04CC01F0,0x418200E8L);
writeKernelMemory((void *)0x04CC01F4,0x7C0DC040L);
writeKernelMemory((void *)0x04CC01F8,0x418200E8L);
writeKernelMemory((void *)0x04CC01FC,0x7C0EC040L);
writeKernelMemory((void *)0x04CC0200,0x418200E0L);
writeKernelMemory((void *)0x04CC0204,0x7C0FC040L);
writeKernelMemory((void *)0x04CC0208,0x418200D8L);
writeKernelMemory((void *)0x04CC020C,0x7C10C040L);
writeKernelMemory((void *)0x04CC0210,0x418200D0L);
writeKernelMemory((void *)0x04CC0214,0x7C11C040L);
writeKernelMemory((void *)0x04CC0218,0x418200C8L);
writeKernelMemory((void *)0x04CC021C,0x7C0DC840L);
writeKernelMemory((void *)0x04CC0220,0x418200C8L);
writeKernelMemory((void *)0x04CC0224,0x7C0EC840L);
writeKernelMemory((void *)0x04CC0228,0x418200C0L);
writeKernelMemory((void *)0x04CC022C,0x7C0FC840L);
writeKernelMemory((void *)0x04CC0230,0x418200B8L);
writeKernelMemory((void *)0x04CC0234,0x7C10C840L);
writeKernelMemory((void *)0x04CC0238,0x418200B0L);
writeKernelMemory((void *)0x04CC023C,0x7C11C840L);
writeKernelMemory((void *)0x04CC0240,0x418200A8L);
writeKernelMemory((void *)0x04CC0244,0x7C0DD040L);
writeKernelMemory((void *)0x04CC0248,0x418200A8L);
writeKernelMemory((void *)0x04CC024C,0x7C0ED040L);
writeKernelMemory((void *)0x04CC0250,0x418200A0L);
writeKernelMemory((void *)0x04CC0254,0x7C0FD040L);
writeKernelMemory((void *)0x04CC0258,0x41820098L);
writeKernelMemory((void *)0x04CC025C,0x7C10D040L);
writeKernelMemory((void *)0x04CC0260,0x41820090L);
writeKernelMemory((void *)0x04CC0264,0x7C11D040L);
writeKernelMemory((void *)0x04CC0268,0x41820088L);
writeKernelMemory((void *)0x04CC026C,0x7C0DD840L);
writeKernelMemory((void *)0x04CC0270,0x41820088L);
writeKernelMemory((void *)0x04CC0274,0x7C0ED840L);
writeKernelMemory((void *)0x04CC0278,0x41820080L);
writeKernelMemory((void *)0x04CC027C,0x7C0FD840L);
writeKernelMemory((void *)0x04CC0280,0x41820078L);
writeKernelMemory((void *)0x04CC0284,0x7C10D840L);
writeKernelMemory((void *)0x04CC0288,0x41820070L);
writeKernelMemory((void *)0x04CC028C,0x7C11D840L);
writeKernelMemory((void *)0x04CC0290,0x41820068L);
writeKernelMemory((void *)0x04CC0294,0x7C0DE040L);
writeKernelMemory((void *)0x04CC0298,0x41820068L);
writeKernelMemory((void *)0x04CC029C,0x7C0EE040L);
writeKernelMemory((void *)0x04CC02A0,0x41820060L);
writeKernelMemory((void *)0x04CC02A4,0x7C0FE040L);
writeKernelMemory((void *)0x04CC02A8,0x41820058L);
writeKernelMemory((void *)0x04CC02AC,0x7C10E040L);
writeKernelMemory((void *)0x04CC02B0,0x41820050L);
writeKernelMemory((void *)0x04CC02B4,0x7C11E040L);
writeKernelMemory((void *)0x04CC02B8,0x41820048L);
writeKernelMemory((void *)0x04CC02BC,0x48000048L);
writeKernelMemory((void *)0x04CC02C0,0x39400000L);
writeKernelMemory((void *)0x04CC02C4,0x48000040L);
writeKernelMemory((void *)0x04CC02C8,0x39400001L);
writeKernelMemory((void *)0x04CC02CC,0x48000038L);
writeKernelMemory((void *)0x04CC02D0,0x39400002L);
writeKernelMemory((void *)0x04CC02D4,0x48000030L);
writeKernelMemory((void *)0x04CC02D8,0x39400003L);
writeKernelMemory((void *)0x04CC02DC,0x48000028L);
writeKernelMemory((void *)0x04CC02E0,0x39400004L);
writeKernelMemory((void *)0x04CC02E4,0x48000020L);
writeKernelMemory((void *)0x04CC02E8,0x39400005L);
writeKernelMemory((void *)0x04CC02EC,0x48000018L);
writeKernelMemory((void *)0x04CC02F0,0x39400006L);
writeKernelMemory((void *)0x04CC02F4,0x48000010L);
writeKernelMemory((void *)0x04CC02F8,0x39400007L);
writeKernelMemory((void *)0x04CC02FC,0x48000008L);
writeKernelMemory((void *)0x04CC0300,0x39400008L);
writeKernelMemory((void *)0x04CC0304,0x915D006CL);
writeKernelMemory((void *)0x04CC0308,0x81A201F0L);
writeKernelMemory((void *)0x04CC030C,0x81C201F4L);
writeKernelMemory((void *)0x04CC0310,0x81E201F8L);
writeKernelMemory((void *)0x04CC0314,0x820201FCL);
writeKernelMemory((void *)0x04CC0318,0x82220200L);
writeKernelMemory((void *)0x04CC031C,0x82820204L);
writeKernelMemory((void *)0x04CC0320,0x82A20208L);
writeKernelMemory((void *)0x04CC0324,0x82C2020CL);
writeKernelMemory((void *)0x04CC0328,0x82E20210L);
writeKernelMemory((void *)0x04CC032C,0x83020214L);
writeKernelMemory((void *)0x04CC0330,0x83220218L);
writeKernelMemory((void *)0x04CC0334,0x8342021CL);
writeKernelMemory((void *)0x04CC0338,0x83620220L);
writeKernelMemory((void *)0x04CC033C,0x83820224L);
writeKernelMemory((void *)0x04CC0340,0x81990034L);
writeKernelMemory((void *)0x04CC0344,0x2C0C0000L);
writeKernelMemory((void *)0x04CC0348,0x4182003CL);
writeKernelMemory((void *)0x04CC034C,0x8122FC48L);
writeKernelMemory((void *)0x04CC0350,0x2C090000L);
writeKernelMemory((void *)0x04CC0354,0x41820030L);
writeKernelMemory((void *)0x04CC0358,0x38620E78L);
writeKernelMemory((void *)0x04CC035C,0x3D8002A0L);
writeKernelMemory((void *)0x04CC0360,0x618C00A4L);
writeKernelMemory((void *)0x04CC0364,0x7D8903A6L);
writeKernelMemory((void *)0x04CC0368,0x4E800421L);
writeKernelMemory((void *)0x04CC036C,0x48005F39L);
writeKernelMemory((void *)0x04CC0370,0x3C80109CL);
writeKernelMemory((void *)0x04CC0374,0x808465A0L);
writeKernelMemory((void *)0x04CC0378,0x48005F49L);
writeKernelMemory((void *)0x04CC037C,0x39200000L);
writeKernelMemory((void *)0x04CC0380,0x9122FC48L);
writeKernelMemory((void *)0x04CC0384,0x81990034L);
writeKernelMemory((void *)0x04CC0388,0x2C0C0000L);
writeKernelMemory((void *)0x04CC038C,0x4182003CL);
writeKernelMemory((void *)0x04CC0390,0x8122FC44L);
writeKernelMemory((void *)0x04CC0394,0x2C090000L);
writeKernelMemory((void *)0x04CC0398,0x41820030L);
writeKernelMemory((void *)0x04CC039C,0x38620E78L);
writeKernelMemory((void *)0x04CC03A0,0x3D800252L);
writeKernelMemory((void *)0x04CC03A4,0x618C0B58L);
writeKernelMemory((void *)0x04CC03A8,0x7D8903A6L);
writeKernelMemory((void *)0x04CC03AC,0x4E800421L);
writeKernelMemory((void *)0x04CC03B0,0x48005EF5L);
writeKernelMemory((void *)0x04CC03B4,0x3C80109CL);
writeKernelMemory((void *)0x04CC03B8,0x808469D4L);
writeKernelMemory((void *)0x04CC03BC,0x48005F05L);
writeKernelMemory((void *)0x04CC03C0,0x39200000L);
writeKernelMemory((void *)0x04CC03C4,0x9122FC44L);
writeKernelMemory((void *)0x04CC03C8,0x81990034L);
writeKernelMemory((void *)0x04CC03CC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC03D0,0x4182004CL);
writeKernelMemory((void *)0x04CC03D4,0x8122FC3CL);
writeKernelMemory((void *)0x04CC03D8,0x2C090000L);
writeKernelMemory((void *)0x04CC03DC,0x41820040L);
writeKernelMemory((void *)0x04CC03E0,0x38620E78L);
writeKernelMemory((void *)0x04CC03E4,0x3C800000L);
writeKernelMemory((void *)0x04CC03E8,0x80C2014CL);
writeKernelMemory((void *)0x04CC03EC,0x38C6F000L);
writeKernelMemory((void *)0x04CC03F0,0x90C2014CL);
writeKernelMemory((void *)0x04CC03F4,0x3D80029FL);
writeKernelMemory((void *)0x04CC03F8,0x618C92E0L);
writeKernelMemory((void *)0x04CC03FC,0x7D8903A6L);
writeKernelMemory((void *)0x04CC0400,0x4E800421L);
writeKernelMemory((void *)0x04CC0404,0x48005EA1L);
writeKernelMemory((void *)0x04CC0408,0x3C80109CL);
writeKernelMemory((void *)0x04CC040C,0x808465A0L);
writeKernelMemory((void *)0x04CC0410,0x48005EB1L);
writeKernelMemory((void *)0x04CC0414,0x39200000L);
writeKernelMemory((void *)0x04CC0418,0x9122FC3CL);
writeKernelMemory((void *)0x04CC041C,0x81990034L);
writeKernelMemory((void *)0x04CC0420,0x2C0C0000L);
writeKernelMemory((void *)0x04CC0424,0x4182003CL);
writeKernelMemory((void *)0x04CC0428,0x8122FC40L);
writeKernelMemory((void *)0x04CC042C,0x2C090000L);
writeKernelMemory((void *)0x04CC0430,0x41820030L);
writeKernelMemory((void *)0x04CC0434,0x38620E78L);
writeKernelMemory((void *)0x04CC0438,0x3D80028BL);
writeKernelMemory((void *)0x04CC043C,0x618C5158L);
writeKernelMemory((void *)0x04CC0440,0x7D8903A6L);
writeKernelMemory((void *)0x04CC0444,0x4E800421L);
writeKernelMemory((void *)0x04CC0448,0x48005E5DL);
writeKernelMemory((void *)0x04CC044C,0x3C80109CL);
writeKernelMemory((void *)0x04CC0450,0x808465A0L);
writeKernelMemory((void *)0x04CC0454,0x48005E6DL);
writeKernelMemory((void *)0x04CC0458,0x39200000L);
writeKernelMemory((void *)0x04CC045C,0x9122FC40L);
writeKernelMemory((void *)0x04CC0460,0x81220A5CL);
writeKernelMemory((void *)0x04CC0464,0x2C090001L);
writeKernelMemory((void *)0x04CC0468,0x41821C14L);
writeKernelMemory((void *)0x04CC046C,0x8122FD58L);
writeKernelMemory((void *)0x04CC0470,0x2C090001L);
writeKernelMemory((void *)0x04CC0474,0x41820014L);
writeKernelMemory((void *)0x04CC0478,0x3D201099L);
writeKernelMemory((void *)0x04CC047C,0x3D403F00L);
writeKernelMemory((void *)0x04CC0480,0x91497EA8L);
writeKernelMemory((void *)0x04CC0484,0x48000010L);
writeKernelMemory((void *)0x04CC0488,0x3D201099L);
writeKernelMemory((void *)0x04CC048C,0x39400000L);
writeKernelMemory((void *)0x04CC0490,0x91497EA8L);
writeKernelMemory((void *)0x04CC0494,0x8122FD48L);
writeKernelMemory((void *)0x04CC0498,0x2C090000L);
writeKernelMemory((void *)0x04CC049C,0x41820024L);
writeKernelMemory((void *)0x04CC04A0,0x728C0040L);
writeKernelMemory((void *)0x04CC04A4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC04A8,0x41820018L);
writeKernelMemory((void *)0x04CC04AC,0x81990034L);
writeKernelMemory((void *)0x04CC04B0,0x2C0C0000L);
writeKernelMemory((void *)0x04CC04B4,0x4182000CL);
writeKernelMemory((void *)0x04CC04B8,0x39200004L);
writeKernelMemory((void *)0x04CC04BC,0x912C0910L);
writeKernelMemory((void *)0x04CC04C0,0x8122FD44L);
writeKernelMemory((void *)0x04CC04C4,0x2C090000L);
writeKernelMemory((void *)0x04CC04C8,0x418200E4L);
writeKernelMemory((void *)0x04CC04CC,0x728C0080L);
writeKernelMemory((void *)0x04CC04D0,0x2C0C0000L);
writeKernelMemory((void *)0x04CC04D4,0x418200D8L);
writeKernelMemory((void *)0x04CC04D8,0x81990034L);
writeKernelMemory((void *)0x04CC04DC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC04E0,0x418200CCL);
writeKernelMemory((void *)0x04CC04E4,0x818C05F0L);
writeKernelMemory((void *)0x04CC04E8,0x812C006CL);
writeKernelMemory((void *)0x04CC04EC,0x1D290008L);
writeKernelMemory((void *)0x04CC04F0,0x818C0008L);
writeKernelMemory((void *)0x04CC04F4,0x7D89602EL);
writeKernelMemory((void *)0x04CC04F8,0x818C0010L);
writeKernelMemory((void *)0x04CC04FC,0x3D20109EL);
writeKernelMemory((void *)0x04CC0500,0x8129F84CL);
writeKernelMemory((void *)0x04CC0504,0x8149042CL);
writeKernelMemory((void *)0x04CC0508,0x7C0A6000L);
writeKernelMemory((void *)0x04CC050C,0x418200A0L);
writeKernelMemory((void *)0x04CC0510,0x81490430L);
writeKernelMemory((void *)0x04CC0514,0x7C0A6000L);
writeKernelMemory((void *)0x04CC0518,0x41820094L);
writeKernelMemory((void *)0x04CC051C,0x81490440L);
writeKernelMemory((void *)0x04CC0520,0x7C0A6000L);
writeKernelMemory((void *)0x04CC0524,0x41820088L);
writeKernelMemory((void *)0x04CC0528,0x81490450L);
writeKernelMemory((void *)0x04CC052C,0x7C0A6000L);
writeKernelMemory((void *)0x04CC0530,0x4182007CL);
writeKernelMemory((void *)0x04CC0534,0x8149046CL);
writeKernelMemory((void *)0x04CC0538,0x7C0A6000L);
writeKernelMemory((void *)0x04CC053C,0x41820070L);
writeKernelMemory((void *)0x04CC0540,0x39200000L);
writeKernelMemory((void *)0x04CC0544,0x81990034L);
writeKernelMemory((void *)0x04CC0548,0x912C0990L);
writeKernelMemory((void *)0x04CC054C,0x38600000L);
writeKernelMemory((void *)0x04CC0550,0x38800000L);
writeKernelMemory((void *)0x04CC0554,0x3D80028BL);
writeKernelMemory((void *)0x04CC0558,0x618C2B8CL);
writeKernelMemory((void *)0x04CC055C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC0560,0x4E800421L);
writeKernelMemory((void *)0x04CC0564,0x48005D3DL);
writeKernelMemory((void *)0x04CC0568,0x38600000L);
writeKernelMemory((void *)0x04CC056C,0x38800005L);
writeKernelMemory((void *)0x04CC0570,0x38A20D00L);
writeKernelMemory((void *)0x04CC0574,0x3CC0109CL);
writeKernelMemory((void *)0x04CC0578,0x80C646A0L);
writeKernelMemory((void *)0x04CC057C,0x81390034L);
writeKernelMemory((void *)0x04CC0580,0x81890528L);
writeKernelMemory((void *)0x04CC0584,0x91850000L);
writeKernelMemory((void *)0x04CC0588,0x8189052CL);
writeKernelMemory((void *)0x04CC058C,0x91850004L);
writeKernelMemory((void *)0x04CC0590,0x81890530L);
writeKernelMemory((void *)0x04CC0594,0x91850008L);
writeKernelMemory((void *)0x04CC0598,0x3D80028AL);
writeKernelMemory((void *)0x04CC059C,0x618CEC5CL);
writeKernelMemory((void *)0x04CC05A0,0x7D8903A6L);
writeKernelMemory((void *)0x04CC05A4,0x4E800421L);
writeKernelMemory((void *)0x04CC05A8,0x48005CF9L);
writeKernelMemory((void *)0x04CC05AC,0x81990034L);
writeKernelMemory((void *)0x04CC05B0,0x2C0C0000L);
writeKernelMemory((void *)0x04CC05B4,0x41820498L);
writeKernelMemory((void *)0x04CC05B8,0x8122FD38L);
writeKernelMemory((void *)0x04CC05BC,0x2C090000L);
writeKernelMemory((void *)0x04CC05C0,0x4182048CL);
writeKernelMemory((void *)0x04CC05C4,0x9421FF08L);
writeKernelMemory((void *)0x04CC05C8,0x7C0802A6L);
writeKernelMemory((void *)0x04CC05CC,0x900100FCL);
writeKernelMemory((void *)0x04CC05D0,0x92810090L);
writeKernelMemory((void *)0x04CC05D4,0x92A10094L);
writeKernelMemory((void *)0x04CC05D8,0x92C10098L);
writeKernelMemory((void *)0x04CC05DC,0x92E1009CL);
writeKernelMemory((void *)0x04CC05E0,0x930100A0L);
writeKernelMemory((void *)0x04CC05E4,0x932100A4L);
writeKernelMemory((void *)0x04CC05E8,0x934100A8L);
writeKernelMemory((void *)0x04CC05EC,0x936100ACL);
writeKernelMemory((void *)0x04CC05F0,0x938100B0L);
writeKernelMemory((void *)0x04CC05F4,0x93A100B4L);
writeKernelMemory((void *)0x04CC05F8,0x93C100B8L);
writeKernelMemory((void *)0x04CC05FC,0x93E100BCL);
writeKernelMemory((void *)0x04CC0600,0xDB4100C8L);
writeKernelMemory((void *)0x04CC0604,0xDB6100D0L);
writeKernelMemory((void *)0x04CC0608,0xDB8100D8L);
writeKernelMemory((void *)0x04CC060C,0xDBA100E0L);
writeKernelMemory((void *)0x04CC0610,0xDBC100E8L);
writeKernelMemory((void *)0x04CC0614,0xDBE100F0L);
writeKernelMemory((void *)0x04CC0618,0x3FE03E00L);
writeKernelMemory((void *)0x04CC061C,0x813F0000L);
writeKernelMemory((void *)0x04CC0620,0xDB2100C0L);
writeKernelMemory((void *)0x04CC0624,0x2C090001L);
writeKernelMemory((void *)0x04CC0628,0x41820030L);
writeKernelMemory((void *)0x04CC062C,0x3D20025AL);
writeKernelMemory((void *)0x04CC0630,0x61291AD0L);
writeKernelMemory((void *)0x04CC0634,0x7D2903A6L);
writeKernelMemory((void *)0x04CC0638,0x4E800421L);
writeKernelMemory((void *)0x04CC063C,0x3D200229L);
writeKernelMemory((void *)0x04CC0640,0x6129CFFCL);
writeKernelMemory((void *)0x04CC0644,0x38600001L);
writeKernelMemory((void *)0x04CC0648,0x7D2903A6L);
writeKernelMemory((void *)0x04CC064C,0x4E800421L);
writeKernelMemory((void *)0x04CC0650,0x39200001L);
writeKernelMemory((void *)0x04CC0654,0x913F0000L);
writeKernelMemory((void *)0x04CC0658,0x3F200316L);
writeKernelMemory((void *)0x04CC065C,0x3E800383L);
writeKernelMemory((void *)0x04CC0660,0x63396818L);
writeKernelMemory((void *)0x04CC0664,0x6294310CL);
writeKernelMemory((void *)0x04CC0668,0x3EC04066L);
writeKernelMemory((void *)0x04CC066C,0x62D68000L);
writeKernelMemory((void *)0x04CC0670,0x3AE00000L);
writeKernelMemory((void *)0x04CC0674,0x3FC04009L);
writeKernelMemory((void *)0x04CC0678,0x3FE05444L);
writeKernelMemory((void *)0x04CC067C,0x63DE21FBL);
writeKernelMemory((void *)0x04CC0680,0x63FF2EEAL);
writeKernelMemory((void *)0x04CC0684,0x7F2903A6L);
writeKernelMemory((void *)0x04CC0688,0x4E800421L);
writeKernelMemory((void *)0x04CC068C,0x3D20020CL);
writeKernelMemory((void *)0x04CC0690,0x83430034L);
writeKernelMemory((void *)0x04CC0694,0x61293D38L);
writeKernelMemory((void *)0x04CC0698,0x7D2903A6L);
writeKernelMemory((void *)0x04CC069C,0x38610008L);
writeKernelMemory((void *)0x04CC06A0,0x7F44D378L);
writeKernelMemory((void *)0x04CC06A4,0xCBFA0118L);
writeKernelMemory((void *)0x04CC06A8,0xCB7A0120L);
writeKernelMemory((void *)0x04CC06AC,0x3AA00001L);
writeKernelMemory((void *)0x04CC06B0,0xCB9A0128L);
writeKernelMemory((void *)0x04CC06B4,0x3B000000L);
writeKernelMemory((void *)0x04CC06B8,0x4E800421L);
writeKernelMemory((void *)0x04CC06BC,0x3D20020CL);
writeKernelMemory((void *)0x04CC06C0,0x612942D0L);
writeKernelMemory((void *)0x04CC06C4,0x38810008L);
writeKernelMemory((void *)0x04CC06C8,0x38610014L);
writeKernelMemory((void *)0x04CC06CC,0x7D2903A6L);
writeKernelMemory((void *)0x04CC06D0,0x4E800421L);
writeKernelMemory((void *)0x04CC06D4,0x83610014L);
writeKernelMemory((void *)0x04CC06D8,0x3D404330L);
writeKernelMemory((void *)0x04CC06DC,0x3D205980L);
writeKernelMemory((void *)0x04CC06E0,0x3D1B8000L);
writeKernelMemory((void *)0x04CC06E4,0x91410020L);
writeKernelMemory((void *)0x04CC06E8,0x91010024L);
writeKernelMemory((void *)0x04CC06EC,0x61290004L);
writeKernelMemory((void *)0x04CC06F0,0x83810018L);
writeKernelMemory((void *)0x04CC06F4,0x3D000262L);
writeKernelMemory((void *)0x04CC06F8,0xCBC10020L);
writeKernelMemory((void *)0x04CC06FC,0x61083348L);
writeKernelMemory((void *)0x04CC0700,0x91210020L);
writeKernelMemory((void *)0x04CC0704,0x7D0903A6L);
writeKernelMemory((void *)0x04CC0708,0x3D1C8000L);
writeKernelMemory((void *)0x04CC070C,0x83A1001CL);
writeKernelMemory((void *)0x04CC0710,0xC0010020L);
writeKernelMemory((void *)0x04CC0714,0x91410028L);
writeKernelMemory((void *)0x04CC0718,0x9101002CL);
writeKernelMemory((void *)0x04CC071C,0xFFDE0028L);
writeKernelMemory((void *)0x04CC0720,0x3D1D8000L);
writeKernelMemory((void *)0x04CC0724,0xCBA10028L);
writeKernelMemory((void *)0x04CC0728,0x91210028L);
writeKernelMemory((void *)0x04CC072C,0xFFDEF828L);
writeKernelMemory((void *)0x04CC0730,0xC0010028L);
writeKernelMemory((void *)0x04CC0734,0x91410030L);
writeKernelMemory((void *)0x04CC0738,0x91010034L);
writeKernelMemory((void *)0x04CC073C,0xFFBD0028L);
writeKernelMemory((void *)0x04CC0740,0xCBE10030L);
writeKernelMemory((void *)0x04CC0744,0x91210030L);
writeKernelMemory((void *)0x04CC0748,0xFFBDD828L);
writeKernelMemory((void *)0x04CC074C,0xC0010030L);
writeKernelMemory((void *)0x04CC0750,0xFFFF0028L);
writeKernelMemory((void *)0x04CC0754,0xFFFFE028L);
writeKernelMemory((void *)0x04CC0758,0xFC3F07F2L);
writeKernelMemory((void *)0x04CC075C,0xFC3E0FBAL);
writeKernelMemory((void *)0x04CC0760,0x4E800421L);
writeKernelMemory((void *)0x04CC0764,0xFC40F090L);
writeKernelMemory((void *)0x04CC0768,0xFC000890L);
writeKernelMemory((void *)0x04CC076C,0x7E8903A6L);
writeKernelMemory((void *)0x04CC0770,0xFC20F890L);
writeKernelMemory((void *)0x04CC0774,0xFFE00090L);
writeKernelMemory((void *)0x04CC0778,0x4E800421L);
writeKernelMemory((void *)0x04CC077C,0x92C10038L);
writeKernelMemory((void *)0x04CC0780,0x92E1003CL);
writeKernelMemory((void *)0x04CC0784,0xFC40F890L);
writeKernelMemory((void *)0x04CC0788,0x3D404056L);
writeKernelMemory((void *)0x04CC078C,0x39600000L);
writeKernelMemory((void *)0x04CC0790,0xCBE10038L);
writeKernelMemory((void *)0x04CC0794,0x614A8000L);
writeKernelMemory((void *)0x04CC0798,0x93C10040L);
writeKernelMemory((void *)0x04CC079C,0xFC000890L);
writeKernelMemory((void *)0x04CC07A0,0x93E10044L);
writeKernelMemory((void *)0x04CC07A4,0x7E8903A6L);
writeKernelMemory((void *)0x04CC07A8,0xFC20E890L);
writeKernelMemory((void *)0x04CC07AC,0xCB410040L);
writeKernelMemory((void *)0x04CC07B0,0xFFA00090L);
writeKernelMemory((void *)0x04CC07B4,0x91410048L);
writeKernelMemory((void *)0x04CC07B8,0x9161004CL);
writeKernelMemory((void *)0x04CC07BC,0xCB610048L);
writeKernelMemory((void *)0x04CC07C0,0x4E800421L);
writeKernelMemory((void *)0x04CC07C4,0x92C10050L);
writeKernelMemory((void *)0x04CC07C8,0x92E10054L);
writeKernelMemory((void *)0x04CC07CC,0x7F2903A6L);
writeKernelMemory((void *)0x04CC07D0,0xFF800890L);
writeKernelMemory((void *)0x04CC07D4,0xCBC10050L);
writeKernelMemory((void *)0x04CC07D8,0x93C10058L);
writeKernelMemory((void *)0x04CC07DC,0x93E1005CL);
writeKernelMemory((void *)0x04CC07E0,0x3FE00382L);
writeKernelMemory((void *)0x04CC07E4,0x63FFAACCL);
writeKernelMemory((void *)0x04CC07E8,0xCB210058L);
writeKernelMemory((void *)0x04CC07EC,0x4E800421L);
writeKernelMemory((void *)0x04CC07F0,0x3D40109CL);
writeKernelMemory((void *)0x04CC07F4,0x81030034L);
writeKernelMemory((void *)0x04CC07F8,0x3D20028BL);
writeKernelMemory((void *)0x04CC07FC,0x614A46A0L);
writeKernelMemory((void *)0x04CC0800,0x612925B8L);
writeKernelMemory((void *)0x04CC0804,0x80AA0000L);
writeKernelMemory((void *)0x04CC0808,0x38C00000L);
writeKernelMemory((void *)0x04CC080C,0x82C80878L);
writeKernelMemory((void *)0x04CC0810,0x7D2903A6L);
writeKernelMemory((void *)0x04CC0814,0x38810070L);
writeKernelMemory((void *)0x04CC0818,0x93610008L);
writeKernelMemory((void *)0x04CC081C,0x9381000CL);
writeKernelMemory((void *)0x04CC0820,0x38600000L);
writeKernelMemory((void *)0x04CC0824,0x93A10010L);
writeKernelMemory((void *)0x04CC0828,0x93610070L);
writeKernelMemory((void *)0x04CC082C,0x93810074L);
writeKernelMemory((void *)0x04CC0830,0x93A10078L);
writeKernelMemory((void *)0x04CC0834,0x4E800421L);
writeKernelMemory((void *)0x04CC0838,0x7FE903A6L);
writeKernelMemory((void *)0x04CC083C,0x7C771B78L);
writeKernelMemory((void *)0x04CC0840,0x38600010L);
writeKernelMemory((void *)0x04CC0844,0x4E800421L);
writeKernelMemory((void *)0x04CC0848,0xFC5E0732L);
writeKernelMemory((void *)0x04CC084C,0x3D20028AL);
writeKernelMemory((void *)0x04CC0850,0x38A00000L);
writeKernelMemory((void *)0x04CC0854,0xFC3F0772L);
writeKernelMemory((void *)0x04CC0858,0x6129D954L);
writeKernelMemory((void *)0x04CC085C,0x7D2903A6L);
writeKernelMemory((void *)0x04CC0860,0x38800000L);
writeKernelMemory((void *)0x04CC0864,0xFC42C824L);
writeKernelMemory((void *)0x04CC0868,0x92E3000CL);
writeKernelMemory((void *)0x04CC086C,0x92A30000L);
writeKernelMemory((void *)0x04CC0870,0x7C7E1B78L);
writeKernelMemory((void *)0x04CC0874,0x92A30004L);
writeKernelMemory((void *)0x04CC0878,0x93030008L);
writeKernelMemory((void *)0x04CC087C,0x38600000L);
writeKernelMemory((void *)0x04CC0880,0xFC21D024L);
writeKernelMemory((void *)0x04CC0884,0xFC401018L);
writeKernelMemory((void *)0x04CC0888,0xFC21D828L);
writeKernelMemory((void *)0x04CC088C,0xFC401050L);
writeKernelMemory((void *)0x04CC0890,0xFC200818L);
writeKernelMemory((void *)0x04CC0894,0x4E800421L);
writeKernelMemory((void *)0x04CC0898,0x7FE903A6L);
writeKernelMemory((void *)0x04CC089C,0x7C741B78L);
writeKernelMemory((void *)0x04CC08A0,0x38600010L);
writeKernelMemory((void *)0x04CC08A4,0x4E800421L);
writeKernelMemory((void *)0x04CC08A8,0x7C7F1B78L);
writeKernelMemory((void *)0x04CC08AC,0x92A30000L);
writeKernelMemory((void *)0x04CC08B0,0x92A30004L);
writeKernelMemory((void *)0x04CC08B4,0x38810008L);
writeKernelMemory((void *)0x04CC08B8,0x929F000CL);
writeKernelMemory((void *)0x04CC08BC,0x7F43D378L);
writeKernelMemory((void *)0x04CC08C0,0x931F0008L);
writeKernelMemory((void *)0x04CC08C4,0x93010008L);
writeKernelMemory((void *)0x04CC08C8,0x9301000CL);
writeKernelMemory((void *)0x04CC08CC,0x3D20031FL);
writeKernelMemory((void *)0x04CC08D0,0x61293AC4L);
writeKernelMemory((void *)0x04CC08D4,0x7D2903A6L);
writeKernelMemory((void *)0x04CC08D8,0x4E800421L);
writeKernelMemory((void *)0x04CC08DC,0x81410008L);
writeKernelMemory((void *)0x04CC08E0,0x2C0A0000L);
writeKernelMemory((void *)0x04CC08E4,0x41820110L);
writeKernelMemory((void *)0x04CC08E8,0x806A0010L);
writeKernelMemory((void *)0x04CC08EC,0x3F000304L);
writeKernelMemory((void *)0x04CC08F0,0x6318A5D8L);
writeKernelMemory((void *)0x04CC08F4,0x7F9A4670L);
writeKernelMemory((void *)0x04CC08F8,0x7F5A0194L);
writeKernelMemory((void *)0x04CC08FC,0x575A402EL);
writeKernelMemory((void *)0x04CC0900,0x7F5AE050L);
writeKernelMemory((void *)0x04CC0904,0x3D200249L);
writeKernelMemory((void *)0x04CC0908,0x61292100L);
writeKernelMemory((void *)0x04CC090C,0x7D2903A6L);
writeKernelMemory((void *)0x04CC0910,0x4E800421L);
writeKernelMemory((void *)0x04CC0914,0x2C030000L);
writeKernelMemory((void *)0x04CC0918,0x418200DCL);
writeKernelMemory((void *)0x04CC091C,0x38810080L);
writeKernelMemory((void *)0x04CC0920,0x93E10084L);
writeKernelMemory((void *)0x04CC0924,0x7EC3B378L);
writeKernelMemory((void *)0x04CC0928,0x92810080L);
writeKernelMemory((void *)0x04CC092C,0x7F7F2670L);
writeKernelMemory((void *)0x04CC0930,0x7FFF0194L);
writeKernelMemory((void *)0x04CC0934,0x57FF2036L);
writeKernelMemory((void *)0x04CC0938,0x7FFFD850L);
writeKernelMemory((void *)0x04CC093C,0x7F0903A6L);
writeKernelMemory((void *)0x04CC0940,0x4E800421L);
writeKernelMemory((void *)0x04CC0944,0x7F2903A6L);
writeKernelMemory((void *)0x04CC0948,0x7FB92670L);
writeKernelMemory((void *)0x04CC094C,0x7F390194L);
writeKernelMemory((void *)0x04CC0950,0x57392036L);
writeKernelMemory((void *)0x04CC0954,0x7F39E850L);
writeKernelMemory((void *)0x04CC0958,0x4E800421L);
writeKernelMemory((void *)0x04CC095C,0x38800000L);
writeKernelMemory((void *)0x04CC0960,0x3D200318L);
writeKernelMemory((void *)0x04CC0964,0x6129C878L);
writeKernelMemory((void *)0x04CC0968,0x7D2903A6L);
writeKernelMemory((void *)0x04CC096C,0x4E800421L);
writeKernelMemory((void *)0x04CC0970,0x38810070L);
writeKernelMemory((void *)0x04CC0974,0x3D200254L);
writeKernelMemory((void *)0x04CC0978,0x61296850L);
writeKernelMemory((void *)0x04CC097C,0x7D2903A6L);
writeKernelMemory((void *)0x04CC0980,0x4E800421L);
writeKernelMemory((void *)0x04CC0984,0x7C7D1B79L);
writeKernelMemory((void *)0x04CC0988,0x4182006CL);
writeKernelMemory((void *)0x04CC098C,0x807D000CL);
writeKernelMemory((void *)0x04CC0990,0x38E00000L);
writeKernelMemory((void *)0x04CC0994,0x7F26CB78L);
writeKernelMemory((void *)0x04CC0998,0x7F45D378L);
writeKernelMemory((void *)0x04CC099C,0x7FE4FB78L);
writeKernelMemory((void *)0x04CC09A0,0x3D20024AL);
writeKernelMemory((void *)0x04CC09A4,0x6129B328L);
writeKernelMemory((void *)0x04CC09A8,0x7D2903A6L);
writeKernelMemory((void *)0x04CC09AC,0x4E800421L);
writeKernelMemory((void *)0x04CC09B0,0x2C0305FEL);
writeKernelMemory((void *)0x04CC09B4,0x41820040L);
writeKernelMemory((void *)0x04CC09B8,0x38810080L);
writeKernelMemory((void *)0x04CC09BC,0x7EC3B378L);
writeKernelMemory((void *)0x04CC09C0,0x92E10080L);
writeKernelMemory((void *)0x04CC09C4,0x93C10084L);
writeKernelMemory((void *)0x04CC09C8,0x7F0903A6L);
writeKernelMemory((void *)0x04CC09CC,0x4E800421L);
writeKernelMemory((void *)0x04CC09D0,0x3D20024AL);
writeKernelMemory((void *)0x04CC09D4,0x6129E524L);
writeKernelMemory((void *)0x04CC09D8,0x807D000CL);
writeKernelMemory((void *)0x04CC09DC,0x7D2903A6L);
writeKernelMemory((void *)0x04CC09E0,0x7F26CB78L);
writeKernelMemory((void *)0x04CC09E4,0x7F45D378L);
writeKernelMemory((void *)0x04CC09E8,0x7FE4FB78L);
writeKernelMemory((void *)0x04CC09EC,0x38E005FEL);
writeKernelMemory((void *)0x04CC09F0,0x4E800421L);
writeKernelMemory((void *)0x04CC09F4,0x800100FCL);
writeKernelMemory((void *)0x04CC09F8,0x82810090L);
writeKernelMemory((void *)0x04CC09FC,0x82A10094L);
writeKernelMemory((void *)0x04CC0A00,0x82C10098L);
writeKernelMemory((void *)0x04CC0A04,0x82E1009CL);
writeKernelMemory((void *)0x04CC0A08,0x830100A0L);
writeKernelMemory((void *)0x04CC0A0C,0x832100A4L);
writeKernelMemory((void *)0x04CC0A10,0x834100A8L);
writeKernelMemory((void *)0x04CC0A14,0x836100ACL);
writeKernelMemory((void *)0x04CC0A18,0x838100B0L);
writeKernelMemory((void *)0x04CC0A1C,0x83A100B4L);
writeKernelMemory((void *)0x04CC0A20,0x83C100B8L);
writeKernelMemory((void *)0x04CC0A24,0x83E100BCL);
writeKernelMemory((void *)0x04CC0A28,0xCB2100C0L);
writeKernelMemory((void *)0x04CC0A2C,0xCB4100C8L);
writeKernelMemory((void *)0x04CC0A30,0xCB6100D0L);
writeKernelMemory((void *)0x04CC0A34,0xCB8100D8L);
writeKernelMemory((void *)0x04CC0A38,0xCBA100E0L);
writeKernelMemory((void *)0x04CC0A3C,0xCBC100E8L);
writeKernelMemory((void *)0x04CC0A40,0xCBE100F0L);
writeKernelMemory((void *)0x04CC0A44,0x7C0803A6L);
writeKernelMemory((void *)0x04CC0A48,0x382100F8L);
writeKernelMemory((void *)0x04CC0A4C,0x81990034L);
writeKernelMemory((void *)0x04CC0A50,0x2C0C0000L);
writeKernelMemory((void *)0x04CC0A54,0x4182001CL);
writeKernelMemory((void *)0x04CC0A58,0x8122FD34L);
writeKernelMemory((void *)0x04CC0A5C,0x2C090000L);
writeKernelMemory((void *)0x04CC0A60,0x41820010L);
writeKernelMemory((void *)0x04CC0A64,0x3D800411L);
writeKernelMemory((void *)0x04CC0A68,0x7D8903A6L);
writeKernelMemory((void *)0x04CC0A6C,0x4E800421L);
writeKernelMemory((void *)0x04CC0A70,0x81990034L);
writeKernelMemory((void *)0x04CC0A74,0x2C0C0000L);
writeKernelMemory((void *)0x04CC0A78,0x41820118L);
writeKernelMemory((void *)0x04CC0A7C,0x8122FD30L);
writeKernelMemory((void *)0x04CC0A80,0x2C090000L);
writeKernelMemory((void *)0x04CC0A84,0x4182010CL);
writeKernelMemory((void *)0x04CC0A88,0x3C401400L);
writeKernelMemory((void *)0x04CC0A8C,0x38420100L);
writeKernelMemory((void *)0x04CC0A90,0x7D8802A6L);
writeKernelMemory((void *)0x04CC0A94,0x91820000L);
writeKernelMemory((void *)0x04CC0A98,0x81990034L);
writeKernelMemory((void *)0x04CC0A9C,0x3D200100L);
writeKernelMemory((void *)0x04CC0AA0,0x912C01DCL);
writeKernelMemory((void *)0x04CC0AA4,0x81820034L);
writeKernelMemory((void *)0x04CC0AA8,0x398C0001L);
writeKernelMemory((void *)0x04CC0AAC,0x91820034L);
writeKernelMemory((void *)0x04CC0AB0,0x2C0C0030L);
writeKernelMemory((void *)0x04CC0AB4,0x408000C0L);
writeKernelMemory((void *)0x04CC0AB8,0x2C0C0001L);
writeKernelMemory((void *)0x04CC0ABC,0x408200C0L);
writeKernelMemory((void *)0x04CC0AC0,0x3980FFF8L);
writeKernelMemory((void *)0x04CC0AC4,0x91820020L);
writeKernelMemory((void *)0x04CC0AC8,0x3980FFF8L);
writeKernelMemory((void *)0x04CC0ACC,0x91820024L);
writeKernelMemory((void *)0x04CC0AD0,0x3980FFF8L);
writeKernelMemory((void *)0x04CC0AD4,0x91820028L);
writeKernelMemory((void *)0x04CC0AD8,0x38600000L);
writeKernelMemory((void *)0x04CC0ADC,0x38C00000L);
writeKernelMemory((void *)0x04CC0AE0,0x38820010L);
writeKernelMemory((void *)0x04CC0AE4,0x3CA0109CL);
writeKernelMemory((void *)0x04CC0AE8,0x80A546A4L);
writeKernelMemory((void *)0x04CC0AEC,0x81590034L);
writeKernelMemory((void *)0x04CC0AF0,0x818A0528L);
writeKernelMemory((void *)0x04CC0AF4,0x81220020L);
writeKernelMemory((void *)0x04CC0AF8,0x7D8C4A14L);
writeKernelMemory((void *)0x04CC0AFC,0x91840000L);
writeKernelMemory((void *)0x04CC0B00,0x818A052CL);
writeKernelMemory((void *)0x04CC0B04,0x81220024L);
writeKernelMemory((void *)0x04CC0B08,0x7D8C4A14L);
writeKernelMemory((void *)0x04CC0B0C,0x91840004L);
writeKernelMemory((void *)0x04CC0B10,0x818A0530L);
writeKernelMemory((void *)0x04CC0B14,0x81220028L);
writeKernelMemory((void *)0x04CC0B18,0x7D8C4A14L);
writeKernelMemory((void *)0x04CC0B1C,0x91840008L);
writeKernelMemory((void *)0x04CC0B20,0x3D80028BL);
writeKernelMemory((void *)0x04CC0B24,0x618C25B8L);
writeKernelMemory((void *)0x04CC0B28,0x7D8903A6L);
writeKernelMemory((void *)0x04CC0B2C,0x4E800421L);
writeKernelMemory((void *)0x04CC0B30,0x48005771L);
writeKernelMemory((void *)0x04CC0B34,0x81820028L);
writeKernelMemory((void *)0x04CC0B38,0x398C0001L);
writeKernelMemory((void *)0x04CC0B3C,0x91820028L);
writeKernelMemory((void *)0x04CC0B40,0x2C0C0008L);
writeKernelMemory((void *)0x04CC0B44,0x4180FF94L);
writeKernelMemory((void *)0x04CC0B48,0x81820024L);
writeKernelMemory((void *)0x04CC0B4C,0x398C0001L);
writeKernelMemory((void *)0x04CC0B50,0x91820024L);
writeKernelMemory((void *)0x04CC0B54,0x2C0C0008L);
writeKernelMemory((void *)0x04CC0B58,0x4180FF78L);
writeKernelMemory((void *)0x04CC0B5C,0x81820020L);
writeKernelMemory((void *)0x04CC0B60,0x398C0001L);
writeKernelMemory((void *)0x04CC0B64,0x91820020L);
writeKernelMemory((void *)0x04CC0B68,0x2C0C0008L);
writeKernelMemory((void *)0x04CC0B6C,0x4180FF5CL);
writeKernelMemory((void *)0x04CC0B70,0x4800000CL);
writeKernelMemory((void *)0x04CC0B74,0x39800000L);
writeKernelMemory((void *)0x04CC0B78,0x91820034L);
writeKernelMemory((void *)0x04CC0B7C,0x81820000L);
writeKernelMemory((void *)0x04CC0B80,0x7D8803A6L);
writeKernelMemory((void *)0x04CC0B84,0x3C401003L);
writeKernelMemory((void *)0x04CC0B88,0x38425000L);
writeKernelMemory((void *)0x04CC0B8C,0x48000004L);
writeKernelMemory((void *)0x04CC0B90,0x81990034L);
writeKernelMemory((void *)0x04CC0B94,0x2C0C0000L);
writeKernelMemory((void *)0x04CC0B98,0x418214DCL);
writeKernelMemory((void *)0x04CC0B9C,0x8122FC58L);
writeKernelMemory((void *)0x04CC0BA0,0x2C090000L);
writeKernelMemory((void *)0x04CC0BA4,0x418209B4L);
writeKernelMemory((void *)0x04CC0BA8,0x3D80109EL);
writeKernelMemory((void *)0x04CC0BAC,0x818CF84CL);
writeKernelMemory((void *)0x04CC0BB0,0x804C0258L);
writeKernelMemory((void *)0x04CC0BB4,0x3D401002L);
writeKernelMemory((void *)0x04CC0BB8,0x904A00C8L);
writeKernelMemory((void *)0x04CC0BBC,0x804C0254L);
writeKernelMemory((void *)0x04CC0BC0,0x3D401002L);
writeKernelMemory((void *)0x04CC0BC4,0x904A0004L);
writeKernelMemory((void *)0x04CC0BC8,0x804C0178L);
writeKernelMemory((void *)0x04CC0BCC,0x3D401002L);
writeKernelMemory((void *)0x04CC0BD0,0x904A0008L);
writeKernelMemory((void *)0x04CC0BD4,0x804C0174L);
writeKernelMemory((void *)0x04CC0BD8,0x3D401002L);
writeKernelMemory((void *)0x04CC0BDC,0x904A000CL);
writeKernelMemory((void *)0x04CC0BE0,0x804C01A4L);
writeKernelMemory((void *)0x04CC0BE4,0x3D401002L);
writeKernelMemory((void *)0x04CC0BE8,0x904A0010L);
writeKernelMemory((void *)0x04CC0BEC,0x804C01A0L);
writeKernelMemory((void *)0x04CC0BF0,0x3D401002L);
writeKernelMemory((void *)0x04CC0BF4,0x904A0014L);
writeKernelMemory((void *)0x04CC0BF8,0x804C0110L);
writeKernelMemory((void *)0x04CC0BFC,0x3D401002L);
writeKernelMemory((void *)0x04CC0C00,0x904A0018L);
writeKernelMemory((void *)0x04CC0C04,0x804C00FCL);
writeKernelMemory((void *)0x04CC0C08,0x3D401002L);
writeKernelMemory((void *)0x04CC0C0C,0x904A001CL);
writeKernelMemory((void *)0x04CC0C10,0x804C0318L);
writeKernelMemory((void *)0x04CC0C14,0x3D401002L);
writeKernelMemory((void *)0x04CC0C18,0x904A0020L);
writeKernelMemory((void *)0x04CC0C1C,0x804C00F8L);
writeKernelMemory((void *)0x04CC0C20,0x3D401002L);
writeKernelMemory((void *)0x04CC0C24,0x904A0024L);
writeKernelMemory((void *)0x04CC0C28,0x804C0704L);
writeKernelMemory((void *)0x04CC0C2C,0x3D401002L);
writeKernelMemory((void *)0x04CC0C30,0x904A0028L);
writeKernelMemory((void *)0x04CC0C34,0x804C0230L);
writeKernelMemory((void *)0x04CC0C38,0x3D401002L);
writeKernelMemory((void *)0x04CC0C3C,0x904A002CL);
writeKernelMemory((void *)0x04CC0C40,0x804C0234L);
writeKernelMemory((void *)0x04CC0C44,0x3D401002L);
writeKernelMemory((void *)0x04CC0C48,0x904A0030L);
writeKernelMemory((void *)0x04CC0C4C,0x804C0238L);
writeKernelMemory((void *)0x04CC0C50,0x3D401002L);
writeKernelMemory((void *)0x04CC0C54,0x904A0034L);
writeKernelMemory((void *)0x04CC0C58,0x804C0210L);
writeKernelMemory((void *)0x04CC0C5C,0x3D401002L);
writeKernelMemory((void *)0x04CC0C60,0x904A0038L);
writeKernelMemory((void *)0x04CC0C64,0x804C00DCL);
writeKernelMemory((void *)0x04CC0C68,0x3D401002L);
writeKernelMemory((void *)0x04CC0C6C,0x904A003CL);
writeKernelMemory((void *)0x04CC0C70,0x804C00F0L);
writeKernelMemory((void *)0x04CC0C74,0x3D401002L);
writeKernelMemory((void *)0x04CC0C78,0x904A0040L);
writeKernelMemory((void *)0x04CC0C7C,0x804C233CL);
writeKernelMemory((void *)0x04CC0C80,0x3D401002L);
writeKernelMemory((void *)0x04CC0C84,0x904A0044L);
writeKernelMemory((void *)0x04CC0C88,0x804C06DCL);
writeKernelMemory((void *)0x04CC0C8C,0x3D401002L);
writeKernelMemory((void *)0x04CC0C90,0x904A0048L);
writeKernelMemory((void *)0x04CC0C94,0x804C060CL);
writeKernelMemory((void *)0x04CC0C98,0x3D401002L);
writeKernelMemory((void *)0x04CC0C9C,0x904A004CL);
writeKernelMemory((void *)0x04CC0CA0,0x804C064CL);
writeKernelMemory((void *)0x04CC0CA4,0x3D401002L);
writeKernelMemory((void *)0x04CC0CA8,0x904A00CCL);
writeKernelMemory((void *)0x04CC0CAC,0x804C00ACL);
writeKernelMemory((void *)0x04CC0CB0,0x3D401002L);
writeKernelMemory((void *)0x04CC0CB4,0x904A0050L);
writeKernelMemory((void *)0x04CC0CB8,0x804C0350L);
writeKernelMemory((void *)0x04CC0CBC,0x3D401002L);
writeKernelMemory((void *)0x04CC0CC0,0x904A0054L);
writeKernelMemory((void *)0x04CC0CC4,0x804C10E4L);
writeKernelMemory((void *)0x04CC0CC8,0x3D401002L);
writeKernelMemory((void *)0x04CC0CCC,0x904A0058L);
writeKernelMemory((void *)0x04CC0CD0,0x804C10E8L);
writeKernelMemory((void *)0x04CC0CD4,0x3D401002L);
writeKernelMemory((void *)0x04CC0CD8,0x904A005CL);
writeKernelMemory((void *)0x04CC0CDC,0x804C10ECL);
writeKernelMemory((void *)0x04CC0CE0,0x3D401002L);
writeKernelMemory((void *)0x04CC0CE4,0x904A0060L);
writeKernelMemory((void *)0x04CC0CE8,0x804C10F0L);
writeKernelMemory((void *)0x04CC0CEC,0x3D401002L);
writeKernelMemory((void *)0x04CC0CF0,0x904A0064L);
writeKernelMemory((void *)0x04CC0CF4,0x804C10F4L);
writeKernelMemory((void *)0x04CC0CF8,0x3D401002L);
writeKernelMemory((void *)0x04CC0CFC,0x904A0068L);
writeKernelMemory((void *)0x04CC0D00,0x3D8010A1L);
writeKernelMemory((void *)0x04CC0D04,0x818CA6C0L);
writeKernelMemory((void *)0x04CC0D08,0x818C0004L);
writeKernelMemory((void *)0x04CC0D0C,0x3D401002L);
writeKernelMemory((void *)0x04CC0D10,0x814A00C8L);
writeKernelMemory((void *)0x04CC0D14,0x914C01C4L);
writeKernelMemory((void *)0x04CC0D18,0x3D401002L);
writeKernelMemory((void *)0x04CC0D1C,0x814A0028L);
writeKernelMemory((void *)0x04CC0D20,0x914C0C8CL);
writeKernelMemory((void *)0x04CC0D24,0x38404814L);
writeKernelMemory((void *)0x04CC0D28,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0D2C,0x38400001L);
writeKernelMemory((void *)0x04CC0D30,0x904A0000L);
writeKernelMemory((void *)0x04CC0D34,0x38404828L);
writeKernelMemory((void *)0x04CC0D38,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0D3C,0x38400000L);
writeKernelMemory((void *)0x04CC0D40,0x904A0000L);
writeKernelMemory((void *)0x04CC0D44,0x3D401002L);
writeKernelMemory((void *)0x04CC0D48,0x814A0004L);
writeKernelMemory((void *)0x04CC0D4C,0x914C02C4L);
writeKernelMemory((void *)0x04CC0D50,0x3840000CL);
writeKernelMemory((void *)0x04CC0D54,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0D58,0x38400001L);
writeKernelMemory((void *)0x04CC0D5C,0x904A0000L);
writeKernelMemory((void *)0x04CC0D60,0x38400020L);
writeKernelMemory((void *)0x04CC0D64,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0D68,0x3C400000L);
writeKernelMemory((void *)0x04CC0D6C,0x904A0000L);
writeKernelMemory((void *)0x04CC0D70,0x3D401002L);
writeKernelMemory((void *)0x04CC0D74,0x814A0008L);
writeKernelMemory((void *)0x04CC0D78,0x914C03C4L);
writeKernelMemory((void *)0x04CC0D7C,0x384001BCL);
writeKernelMemory((void *)0x04CC0D80,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0D84,0x38400040L);
writeKernelMemory((void *)0x04CC0D88,0x904A0000L);
writeKernelMemory((void *)0x04CC0D8C,0x384001D0L);
writeKernelMemory((void *)0x04CC0D90,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0D94,0x3C400000L);
writeKernelMemory((void *)0x04CC0D98,0x904A0000L);
writeKernelMemory((void *)0x04CC0D9C,0x3D401002L);
writeKernelMemory((void *)0x04CC0DA0,0x814A000CL);
writeKernelMemory((void *)0x04CC0DA4,0x914C0544L);
writeKernelMemory((void *)0x04CC0DA8,0x384002BCL);
writeKernelMemory((void *)0x04CC0DAC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0DB0,0x3C400000L);
writeKernelMemory((void *)0x04CC0DB4,0x60420040L);
writeKernelMemory((void *)0x04CC0DB8,0x904A0000L);
writeKernelMemory((void *)0x04CC0DBC,0x384002D0L);
writeKernelMemory((void *)0x04CC0DC0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0DC4,0x3C400000L);
writeKernelMemory((void *)0x04CC0DC8,0x904A0000L);
writeKernelMemory((void *)0x04CC0DCC,0x3D401002L);
writeKernelMemory((void *)0x04CC0DD0,0x814A0010L);
writeKernelMemory((void *)0x04CC0DD4,0x914C0644L);
writeKernelMemory((void *)0x04CC0DD8,0x384003BCL);
writeKernelMemory((void *)0x04CC0DDC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0DE0,0x3C400000L);
writeKernelMemory((void *)0x04CC0DE4,0x60420040L);
writeKernelMemory((void *)0x04CC0DE8,0x904A0000L);
writeKernelMemory((void *)0x04CC0DEC,0x384003D0L);
writeKernelMemory((void *)0x04CC0DF0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0DF4,0x3C400000L);
writeKernelMemory((void *)0x04CC0DF8,0x904A0000L);
writeKernelMemory((void *)0x04CC0DFC,0x3D401002L);
writeKernelMemory((void *)0x04CC0E00,0x814A0014L);
writeKernelMemory((void *)0x04CC0E04,0x3C400000L);
writeKernelMemory((void *)0x04CC0E08,0x60420744L);
writeKernelMemory((void *)0x04CC0E0C,0x7C426214L);
writeKernelMemory((void *)0x04CC0E10,0x91420000L);
writeKernelMemory((void *)0x04CC0E14,0x3840053CL);
writeKernelMemory((void *)0x04CC0E18,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0E1C,0x3C400000L);
writeKernelMemory((void *)0x04CC0E20,0x60420040L);
writeKernelMemory((void *)0x04CC0E24,0x904A0000L);
writeKernelMemory((void *)0x04CC0E28,0x38400550L);
writeKernelMemory((void *)0x04CC0E2C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0E30,0x3C400000L);
writeKernelMemory((void *)0x04CC0E34,0x6042000CL);
writeKernelMemory((void *)0x04CC0E38,0x904A0000L);
writeKernelMemory((void *)0x04CC0E3C,0x3D401002L);
writeKernelMemory((void *)0x04CC0E40,0x814A0018L);
writeKernelMemory((void *)0x04CC0E44,0x3C400000L);
writeKernelMemory((void *)0x04CC0E48,0x60420844L);
writeKernelMemory((void *)0x04CC0E4C,0x7C426214L);
writeKernelMemory((void *)0x04CC0E50,0x91420000L);
writeKernelMemory((void *)0x04CC0E54,0x3840063CL);
writeKernelMemory((void *)0x04CC0E58,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0E5C,0x3C400000L);
writeKernelMemory((void *)0x04CC0E60,0x60420040L);
writeKernelMemory((void *)0x04CC0E64,0x904A0000L);
writeKernelMemory((void *)0x04CC0E68,0x38400650L);
writeKernelMemory((void *)0x04CC0E6C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0E70,0x3C400000L);
writeKernelMemory((void *)0x04CC0E74,0x904A0000L);
writeKernelMemory((void *)0x04CC0E78,0x3D401002L);
writeKernelMemory((void *)0x04CC0E7C,0x814A001CL);
writeKernelMemory((void *)0x04CC0E80,0x3C400000L);
writeKernelMemory((void *)0x04CC0E84,0x60420944L);
writeKernelMemory((void *)0x04CC0E88,0x7C426214L);
writeKernelMemory((void *)0x04CC0E8C,0x91420000L);
writeKernelMemory((void *)0x04CC0E90,0x3840073CL);
writeKernelMemory((void *)0x04CC0E94,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0E98,0x3C400000L);
writeKernelMemory((void *)0x04CC0E9C,0x60420040L);
writeKernelMemory((void *)0x04CC0EA0,0x904A0000L);
writeKernelMemory((void *)0x04CC0EA4,0x38400750L);
writeKernelMemory((void *)0x04CC0EA8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0EAC,0x3C400000L);
writeKernelMemory((void *)0x04CC0EB0,0x904A0000L);
writeKernelMemory((void *)0x04CC0EB4,0x3D401002L);
writeKernelMemory((void *)0x04CC0EB8,0x814A0020L);
writeKernelMemory((void *)0x04CC0EBC,0x3C400000L);
writeKernelMemory((void *)0x04CC0EC0,0x60420A5CL);
writeKernelMemory((void *)0x04CC0EC4,0x7C426214L);
writeKernelMemory((void *)0x04CC0EC8,0x91420000L);
writeKernelMemory((void *)0x04CC0ECC,0x3840083CL);
writeKernelMemory((void *)0x04CC0ED0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0ED4,0x3C400000L);
writeKernelMemory((void *)0x04CC0ED8,0x60420040L);
writeKernelMemory((void *)0x04CC0EDC,0x904A0000L);
writeKernelMemory((void *)0x04CC0EE0,0x38400850L);
writeKernelMemory((void *)0x04CC0EE4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0EE8,0x3C400000L);
writeKernelMemory((void *)0x04CC0EEC,0x904A0000L);
writeKernelMemory((void *)0x04CC0EF0,0x3D401002L);
writeKernelMemory((void *)0x04CC0EF4,0x814A0024L);
writeKernelMemory((void *)0x04CC0EF8,0x3C400000L);
writeKernelMemory((void *)0x04CC0EFC,0x60420B74L);
writeKernelMemory((void *)0x04CC0F00,0x7C426214L);
writeKernelMemory((void *)0x04CC0F04,0x91420000L);
writeKernelMemory((void *)0x04CC0F08,0x3840093CL);
writeKernelMemory((void *)0x04CC0F0C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0F10,0x3C400000L);
writeKernelMemory((void *)0x04CC0F14,0x60420040L);
writeKernelMemory((void *)0x04CC0F18,0x904A0000L);
writeKernelMemory((void *)0x04CC0F1C,0x38400950L);
writeKernelMemory((void *)0x04CC0F20,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0F24,0x3C400000L);
writeKernelMemory((void *)0x04CC0F28,0x904A0000L);
writeKernelMemory((void *)0x04CC0F2C,0x38400A54L);
writeKernelMemory((void *)0x04CC0F30,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0F34,0x3C400000L);
writeKernelMemory((void *)0x04CC0F38,0x60420040L);
writeKernelMemory((void *)0x04CC0F3C,0x904A0000L);
writeKernelMemory((void *)0x04CC0F40,0x38400A68L);
writeKernelMemory((void *)0x04CC0F44,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0F48,0x3C400000L);
writeKernelMemory((void *)0x04CC0F4C,0x904A0000L);
writeKernelMemory((void *)0x04CC0F50,0x3D401002L);
writeKernelMemory((void *)0x04CC0F54,0x814A002CL);
writeKernelMemory((void *)0x04CC0F58,0x3C400000L);
writeKernelMemory((void *)0x04CC0F5C,0x60420DA4L);
writeKernelMemory((void *)0x04CC0F60,0x7C426214L);
writeKernelMemory((void *)0x04CC0F64,0x91420000L);
writeKernelMemory((void *)0x04CC0F68,0x38400B6CL);
writeKernelMemory((void *)0x04CC0F6C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0F70,0x3C400000L);
writeKernelMemory((void *)0x04CC0F74,0x60420040L);
writeKernelMemory((void *)0x04CC0F78,0x904A0000L);
writeKernelMemory((void *)0x04CC0F7C,0x38400B80L);
writeKernelMemory((void *)0x04CC0F80,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0F84,0x3C400000L);
writeKernelMemory((void *)0x04CC0F88,0x6042000AL);
writeKernelMemory((void *)0x04CC0F8C,0x904A0000L);
writeKernelMemory((void *)0x04CC0F90,0x3D401002L);
writeKernelMemory((void *)0x04CC0F94,0x814A0030L);
writeKernelMemory((void *)0x04CC0F98,0x3C400000L);
writeKernelMemory((void *)0x04CC0F9C,0x60420F0CL);
writeKernelMemory((void *)0x04CC0FA0,0x7C426214L);
writeKernelMemory((void *)0x04CC0FA4,0x91420000L);
writeKernelMemory((void *)0x04CC0FA8,0x38400C84L);
writeKernelMemory((void *)0x04CC0FAC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0FB0,0x3C400000L);
writeKernelMemory((void *)0x04CC0FB4,0x60420040L);
writeKernelMemory((void *)0x04CC0FB8,0x904A0000L);
writeKernelMemory((void *)0x04CC0FBC,0x38400C98L);
writeKernelMemory((void *)0x04CC0FC0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0FC4,0x3C400000L);
writeKernelMemory((void *)0x04CC0FC8,0x60420004L);
writeKernelMemory((void *)0x04CC0FCC,0x904A0000L);
writeKernelMemory((void *)0x04CC0FD0,0x3D401002L);
writeKernelMemory((void *)0x04CC0FD4,0x814A0034L);
writeKernelMemory((void *)0x04CC0FD8,0x3C400000L);
writeKernelMemory((void *)0x04CC0FDC,0x60421024L);
writeKernelMemory((void *)0x04CC0FE0,0x7C426214L);
writeKernelMemory((void *)0x04CC0FE4,0x91420000L);
writeKernelMemory((void *)0x04CC0FE8,0x38400D9CL);
writeKernelMemory((void *)0x04CC0FEC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC0FF0,0x3C400000L);
writeKernelMemory((void *)0x04CC0FF4,0x60420040L);
writeKernelMemory((void *)0x04CC0FF8,0x904A0000L);
writeKernelMemory((void *)0x04CC0FFC,0x38400DB0L);
writeKernelMemory((void *)0x04CC1000,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1004,0x3C400000L);
writeKernelMemory((void *)0x04CC1008,0x60420004L);
writeKernelMemory((void *)0x04CC100C,0x904A0000L);
writeKernelMemory((void *)0x04CC1010,0x3D401002L);
writeKernelMemory((void *)0x04CC1014,0x814A0038L);
writeKernelMemory((void *)0x04CC1018,0x3C400000L);
writeKernelMemory((void *)0x04CC101C,0x6042113CL);
writeKernelMemory((void *)0x04CC1020,0x7C426214L);
writeKernelMemory((void *)0x04CC1024,0x91420000L);
writeKernelMemory((void *)0x04CC1028,0x38400F04L);
writeKernelMemory((void *)0x04CC102C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1030,0x3C400000L);
writeKernelMemory((void *)0x04CC1034,0x60420040L);
writeKernelMemory((void *)0x04CC1038,0x904A0000L);
writeKernelMemory((void *)0x04CC103C,0x38400F18L);
writeKernelMemory((void *)0x04CC1040,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1044,0x3C400000L);
writeKernelMemory((void *)0x04CC1048,0x60420004L);
writeKernelMemory((void *)0x04CC104C,0x904A0000L);
writeKernelMemory((void *)0x04CC1050,0x3D401002L);
writeKernelMemory((void *)0x04CC1054,0x814A003CL);
writeKernelMemory((void *)0x04CC1058,0x3C400000L);
writeKernelMemory((void *)0x04CC105C,0x60421254L);
writeKernelMemory((void *)0x04CC1060,0x7C426214L);
writeKernelMemory((void *)0x04CC1064,0x91420000L);
writeKernelMemory((void *)0x04CC1068,0x3840101CL);
writeKernelMemory((void *)0x04CC106C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1070,0x3C400000L);
writeKernelMemory((void *)0x04CC1074,0x60420040L);
writeKernelMemory((void *)0x04CC1078,0x904A0000L);
writeKernelMemory((void *)0x04CC107C,0x38401030L);
writeKernelMemory((void *)0x04CC1080,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1084,0x3C400000L);
writeKernelMemory((void *)0x04CC1088,0x6042000CL);
writeKernelMemory((void *)0x04CC108C,0x904A0000L);
writeKernelMemory((void *)0x04CC1090,0x3D401002L);
writeKernelMemory((void *)0x04CC1094,0x814A0040L);
writeKernelMemory((void *)0x04CC1098,0x3C400000L);
writeKernelMemory((void *)0x04CC109C,0x6042136CL);
writeKernelMemory((void *)0x04CC10A0,0x7C426214L);
writeKernelMemory((void *)0x04CC10A4,0x91420000L);
writeKernelMemory((void *)0x04CC10A8,0x38401134L);
writeKernelMemory((void *)0x04CC10AC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC10B0,0x3C400000L);
writeKernelMemory((void *)0x04CC10B4,0x60420040L);
writeKernelMemory((void *)0x04CC10B8,0x904A0000L);
writeKernelMemory((void *)0x04CC10BC,0x38401148L);
writeKernelMemory((void *)0x04CC10C0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC10C4,0x3C400000L);
writeKernelMemory((void *)0x04CC10C8,0x60420004L);
writeKernelMemory((void *)0x04CC10CC,0x904A0000L);
writeKernelMemory((void *)0x04CC10D0,0x3D401002L);
writeKernelMemory((void *)0x04CC10D4,0x814A0044L);
writeKernelMemory((void *)0x04CC10D8,0x3C400000L);
writeKernelMemory((void *)0x04CC10DC,0x60421484L);
writeKernelMemory((void *)0x04CC10E0,0x7C426214L);
writeKernelMemory((void *)0x04CC10E4,0x91420000L);
writeKernelMemory((void *)0x04CC10E8,0x3840124CL);
writeKernelMemory((void *)0x04CC10EC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC10F0,0x3C400000L);
writeKernelMemory((void *)0x04CC10F4,0x60420040L);
writeKernelMemory((void *)0x04CC10F8,0x904A0000L);
writeKernelMemory((void *)0x04CC10FC,0x38401260L);
writeKernelMemory((void *)0x04CC1100,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1104,0x3C400000L);
writeKernelMemory((void *)0x04CC1108,0x60420004L);
writeKernelMemory((void *)0x04CC110C,0x904A0000L);
writeKernelMemory((void *)0x04CC1110,0x3D401002L);
writeKernelMemory((void *)0x04CC1114,0x814A0048L);
writeKernelMemory((void *)0x04CC1118,0x3C400000L);
writeKernelMemory((void *)0x04CC111C,0x60420DF4L);
writeKernelMemory((void *)0x04CC1120,0x7C426214L);
writeKernelMemory((void *)0x04CC1124,0x91420000L);
writeKernelMemory((void *)0x04CC1128,0x38401364L);
writeKernelMemory((void *)0x04CC112C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1130,0x3C400000L);
writeKernelMemory((void *)0x04CC1134,0x60420040L);
writeKernelMemory((void *)0x04CC1138,0x904A0000L);
writeKernelMemory((void *)0x04CC113C,0x38401378L);
writeKernelMemory((void *)0x04CC1140,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1144,0x3C400000L);
writeKernelMemory((void *)0x04CC1148,0x904A0000L);
writeKernelMemory((void *)0x04CC114C,0x3D401002L);
writeKernelMemory((void *)0x04CC1150,0x814A004CL);
writeKernelMemory((void *)0x04CC1154,0x3C400000L);
writeKernelMemory((void *)0x04CC1158,0x604216DCL);
writeKernelMemory((void *)0x04CC115C,0x7C426214L);
writeKernelMemory((void *)0x04CC1160,0x91420000L);
writeKernelMemory((void *)0x04CC1164,0x3840147CL);
writeKernelMemory((void *)0x04CC1168,0x7D4C1214L);
writeKernelMemory((void *)0x04CC116C,0x3C400000L);
writeKernelMemory((void *)0x04CC1170,0x60420040L);
writeKernelMemory((void *)0x04CC1174,0x904A0000L);
writeKernelMemory((void *)0x04CC1178,0x38401490L);
writeKernelMemory((void *)0x04CC117C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1180,0x3C400000L);
writeKernelMemory((void *)0x04CC1184,0x6042000AL);
writeKernelMemory((void *)0x04CC1188,0x904A0000L);
writeKernelMemory((void *)0x04CC118C,0x3D401002L);
writeKernelMemory((void *)0x04CC1190,0x814A00CCL);
writeKernelMemory((void *)0x04CC1194,0x3C400000L);
writeKernelMemory((void *)0x04CC1198,0x604217F4L);
writeKernelMemory((void *)0x04CC119C,0x7C426214L);
writeKernelMemory((void *)0x04CC11A0,0x91420000L);
writeKernelMemory((void *)0x04CC11A4,0x38400DECL);
writeKernelMemory((void *)0x04CC11A8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC11AC,0x3C400000L);
writeKernelMemory((void *)0x04CC11B0,0x60420040L);
writeKernelMemory((void *)0x04CC11B4,0x904A0000L);
writeKernelMemory((void *)0x04CC11B8,0x38400E00L);
writeKernelMemory((void *)0x04CC11BC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC11C0,0x3C400000L);
writeKernelMemory((void *)0x04CC11C4,0x6042000AL);
writeKernelMemory((void *)0x04CC11C8,0x904A0000L);
writeKernelMemory((void *)0x04CC11CC,0x3D401002L);
writeKernelMemory((void *)0x04CC11D0,0x814A0050L);
writeKernelMemory((void *)0x04CC11D4,0x3C400000L);
writeKernelMemory((void *)0x04CC11D8,0x6042190CL);
writeKernelMemory((void *)0x04CC11DC,0x7C426214L);
writeKernelMemory((void *)0x04CC11E0,0x91420000L);
writeKernelMemory((void *)0x04CC11E4,0x384016D4L);
writeKernelMemory((void *)0x04CC11E8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC11EC,0x3C400000L);
writeKernelMemory((void *)0x04CC11F0,0x60420040L);
writeKernelMemory((void *)0x04CC11F4,0x904A0000L);
writeKernelMemory((void *)0x04CC11F8,0x384016E8L);
writeKernelMemory((void *)0x04CC11FC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1200,0x3C400000L);
writeKernelMemory((void *)0x04CC1204,0x60420001L);
writeKernelMemory((void *)0x04CC1208,0x904A0000L);
writeKernelMemory((void *)0x04CC120C,0x3D401002L);
writeKernelMemory((void *)0x04CC1210,0x814A0050L);
writeKernelMemory((void *)0x04CC1214,0x3C400000L);
writeKernelMemory((void *)0x04CC1218,0x60421A24L);
writeKernelMemory((void *)0x04CC121C,0x7C426214L);
writeKernelMemory((void *)0x04CC1220,0x91420000L);
writeKernelMemory((void *)0x04CC1224,0x384017ECL);
writeKernelMemory((void *)0x04CC1228,0x7D4C1214L);
writeKernelMemory((void *)0x04CC122C,0x3C400000L);
writeKernelMemory((void *)0x04CC1230,0x60420040L);
writeKernelMemory((void *)0x04CC1234,0x904A0000L);
writeKernelMemory((void *)0x04CC1238,0x38401800L);
writeKernelMemory((void *)0x04CC123C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1240,0x3C400000L);
writeKernelMemory((void *)0x04CC1244,0x904A0000L);
writeKernelMemory((void *)0x04CC1248,0x3D401002L);
writeKernelMemory((void *)0x04CC124C,0x814A0050L);
writeKernelMemory((void *)0x04CC1250,0x3C400000L);
writeKernelMemory((void *)0x04CC1254,0x60421B3CL);
writeKernelMemory((void *)0x04CC1258,0x7C426214L);
writeKernelMemory((void *)0x04CC125C,0x91420000L);
writeKernelMemory((void *)0x04CC1260,0x38401904L);
writeKernelMemory((void *)0x04CC1264,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1268,0x3C400000L);
writeKernelMemory((void *)0x04CC126C,0x60420040L);
writeKernelMemory((void *)0x04CC1270,0x904A0000L);
writeKernelMemory((void *)0x04CC1274,0x38401918L);
writeKernelMemory((void *)0x04CC1278,0x7D4C1214L);
writeKernelMemory((void *)0x04CC127C,0x3C400000L);
writeKernelMemory((void *)0x04CC1280,0x904A0000L);
writeKernelMemory((void *)0x04CC1284,0x3D401002L);
writeKernelMemory((void *)0x04CC1288,0x814A0050L);
writeKernelMemory((void *)0x04CC128C,0x3C400000L);
writeKernelMemory((void *)0x04CC1290,0x60421C54L);
writeKernelMemory((void *)0x04CC1294,0x7C426214L);
writeKernelMemory((void *)0x04CC1298,0x91420000L);
writeKernelMemory((void *)0x04CC129C,0x38401A1CL);
writeKernelMemory((void *)0x04CC12A0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC12A4,0x3C400000L);
writeKernelMemory((void *)0x04CC12A8,0x60420040L);
writeKernelMemory((void *)0x04CC12AC,0x904A0000L);
writeKernelMemory((void *)0x04CC12B0,0x38401A30L);
writeKernelMemory((void *)0x04CC12B4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC12B8,0x3C400000L);
writeKernelMemory((void *)0x04CC12BC,0x6042000AL);
writeKernelMemory((void *)0x04CC12C0,0x904A0000L);
writeKernelMemory((void *)0x04CC12C4,0x3D401002L);
writeKernelMemory((void *)0x04CC12C8,0x814A0050L);
writeKernelMemory((void *)0x04CC12CC,0x3C400000L);
writeKernelMemory((void *)0x04CC12D0,0x60421D6CL);
writeKernelMemory((void *)0x04CC12D4,0x7C426214L);
writeKernelMemory((void *)0x04CC12D8,0x91420000L);
writeKernelMemory((void *)0x04CC12DC,0x38401B34L);
writeKernelMemory((void *)0x04CC12E0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC12E4,0x3C400000L);
writeKernelMemory((void *)0x04CC12E8,0x60420040L);
writeKernelMemory((void *)0x04CC12EC,0x904A0000L);
writeKernelMemory((void *)0x04CC12F0,0x38401B48L);
writeKernelMemory((void *)0x04CC12F4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC12F8,0x3C400000L);
writeKernelMemory((void *)0x04CC12FC,0x6042000BL);
writeKernelMemory((void *)0x04CC1300,0x904A0000L);
writeKernelMemory((void *)0x04CC1304,0x3D401002L);
writeKernelMemory((void *)0x04CC1308,0x814A0050L);
writeKernelMemory((void *)0x04CC130C,0x3C400000L);
writeKernelMemory((void *)0x04CC1310,0x60421E84L);
writeKernelMemory((void *)0x04CC1314,0x7C426214L);
writeKernelMemory((void *)0x04CC1318,0x91420000L);
writeKernelMemory((void *)0x04CC131C,0x38401C4CL);
writeKernelMemory((void *)0x04CC1320,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1324,0x3C400000L);
writeKernelMemory((void *)0x04CC1328,0x60420040L);
writeKernelMemory((void *)0x04CC132C,0x904A0000L);
writeKernelMemory((void *)0x04CC1330,0x38401C60L);
writeKernelMemory((void *)0x04CC1334,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1338,0x3C400000L);
writeKernelMemory((void *)0x04CC133C,0x6042000CL);
writeKernelMemory((void *)0x04CC1340,0x904A0000L);
writeKernelMemory((void *)0x04CC1344,0x3D401002L);
writeKernelMemory((void *)0x04CC1348,0x814A0050L);
writeKernelMemory((void *)0x04CC134C,0x3C400000L);
writeKernelMemory((void *)0x04CC1350,0x6042204CL);
writeKernelMemory((void *)0x04CC1354,0x7C426214L);
writeKernelMemory((void *)0x04CC1358,0x91420000L);
writeKernelMemory((void *)0x04CC135C,0x38401D64L);
writeKernelMemory((void *)0x04CC1360,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1364,0x3C400000L);
writeKernelMemory((void *)0x04CC1368,0x60420040L);
writeKernelMemory((void *)0x04CC136C,0x904A0000L);
writeKernelMemory((void *)0x04CC1370,0x38401D78L);
writeKernelMemory((void *)0x04CC1374,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1378,0x3C400000L);
writeKernelMemory((void *)0x04CC137C,0x6042000DL);
writeKernelMemory((void *)0x04CC1380,0x904A0000L);
writeKernelMemory((void *)0x04CC1384,0x38401E7CL);
writeKernelMemory((void *)0x04CC1388,0x7D4C1214L);
writeKernelMemory((void *)0x04CC138C,0x3C400000L);
writeKernelMemory((void *)0x04CC1390,0x60420040L);
writeKernelMemory((void *)0x04CC1394,0x904A0000L);
writeKernelMemory((void *)0x04CC1398,0x38401E90L);
writeKernelMemory((void *)0x04CC139C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC13A0,0x3C400000L);
writeKernelMemory((void *)0x04CC13A4,0x6042000EL);
writeKernelMemory((void *)0x04CC13A8,0x904A0000L);
writeKernelMemory((void *)0x04CC13AC,0x38402044L);
writeKernelMemory((void *)0x04CC13B0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC13B4,0x3C400000L);
writeKernelMemory((void *)0x04CC13B8,0x60420040L);
writeKernelMemory((void *)0x04CC13BC,0x904A0000L);
writeKernelMemory((void *)0x04CC13C0,0x38402058L);
writeKernelMemory((void *)0x04CC13C4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC13C8,0x3C400000L);
writeKernelMemory((void *)0x04CC13CC,0x6042000FL);
writeKernelMemory((void *)0x04CC13D0,0x904A0000L);
writeKernelMemory((void *)0x04CC13D4,0x3D401002L);
writeKernelMemory((void *)0x04CC13D8,0x814A0054L);
writeKernelMemory((void *)0x04CC13DC,0x3C400000L);
writeKernelMemory((void *)0x04CC13E0,0x60422164L);
writeKernelMemory((void *)0x04CC13E4,0x7C426214L);
writeKernelMemory((void *)0x04CC13E8,0x91420000L);
writeKernelMemory((void *)0x04CC13EC,0x3840215CL);
writeKernelMemory((void *)0x04CC13F0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC13F4,0x3C400000L);
writeKernelMemory((void *)0x04CC13F8,0x60420040L);
writeKernelMemory((void *)0x04CC13FC,0x904A0000L);
writeKernelMemory((void *)0x04CC1400,0x38402170L);
writeKernelMemory((void *)0x04CC1404,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1408,0x3C400000L);
writeKernelMemory((void *)0x04CC140C,0x6042000CL);
writeKernelMemory((void *)0x04CC1410,0x904A0000L);
writeKernelMemory((void *)0x04CC1414,0x3D401002L);
writeKernelMemory((void *)0x04CC1418,0x814A0058L);
writeKernelMemory((void *)0x04CC141C,0x3C400000L);
writeKernelMemory((void *)0x04CC1420,0x6042227CL);
writeKernelMemory((void *)0x04CC1424,0x7C426214L);
writeKernelMemory((void *)0x04CC1428,0x91420000L);
writeKernelMemory((void *)0x04CC142C,0x38402274L);
writeKernelMemory((void *)0x04CC1430,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1434,0x3C400000L);
writeKernelMemory((void *)0x04CC1438,0x60420040L);
writeKernelMemory((void *)0x04CC143C,0x904A0000L);
writeKernelMemory((void *)0x04CC1440,0x38402288L);
writeKernelMemory((void *)0x04CC1444,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1448,0x3C400000L);
writeKernelMemory((void *)0x04CC144C,0x6042000DL);
writeKernelMemory((void *)0x04CC1450,0x904A0000L);
writeKernelMemory((void *)0x04CC1454,0x3D401002L);
writeKernelMemory((void *)0x04CC1458,0x814A005CL);
writeKernelMemory((void *)0x04CC145C,0x3C400000L);
writeKernelMemory((void *)0x04CC1460,0x60422394L);
writeKernelMemory((void *)0x04CC1464,0x7C426214L);
writeKernelMemory((void *)0x04CC1468,0x91420000L);
writeKernelMemory((void *)0x04CC146C,0x3840238CL);
writeKernelMemory((void *)0x04CC1470,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1474,0x3C400000L);
writeKernelMemory((void *)0x04CC1478,0x60420040L);
writeKernelMemory((void *)0x04CC147C,0x904A0000L);
writeKernelMemory((void *)0x04CC1480,0x384023A0L);
writeKernelMemory((void *)0x04CC1484,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1488,0x3C400000L);
writeKernelMemory((void *)0x04CC148C,0x6042000EL);
writeKernelMemory((void *)0x04CC1490,0x904A0000L);
writeKernelMemory((void *)0x04CC1494,0x3D401002L);
writeKernelMemory((void *)0x04CC1498,0x814A0060L);
writeKernelMemory((void *)0x04CC149C,0x3C400000L);
writeKernelMemory((void *)0x04CC14A0,0x604224ACL);
writeKernelMemory((void *)0x04CC14A4,0x7C426214L);
writeKernelMemory((void *)0x04CC14A8,0x91420000L);
writeKernelMemory((void *)0x04CC14AC,0x384024A4L);
writeKernelMemory((void *)0x04CC14B0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC14B4,0x3C400000L);
writeKernelMemory((void *)0x04CC14B8,0x60420040L);
writeKernelMemory((void *)0x04CC14BC,0x904A0000L);
writeKernelMemory((void *)0x04CC14C0,0x384024B8L);
writeKernelMemory((void *)0x04CC14C4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC14C8,0x3C400000L);
writeKernelMemory((void *)0x04CC14CC,0x6042000FL);
writeKernelMemory((void *)0x04CC14D0,0x904A0000L);
writeKernelMemory((void *)0x04CC14D4,0x3D401002L);
writeKernelMemory((void *)0x04CC14D8,0x814A0064L);
writeKernelMemory((void *)0x04CC14DC,0x3C400000L);
writeKernelMemory((void *)0x04CC14E0,0x604225C4L);
writeKernelMemory((void *)0x04CC14E4,0x7C426214L);
writeKernelMemory((void *)0x04CC14E8,0x91420000L);
writeKernelMemory((void *)0x04CC14EC,0x384025BCL);
writeKernelMemory((void *)0x04CC14F0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC14F4,0x3C400000L);
writeKernelMemory((void *)0x04CC14F8,0x60420040L);
writeKernelMemory((void *)0x04CC14FC,0x904A0000L);
writeKernelMemory((void *)0x04CC1500,0x384025D0L);
writeKernelMemory((void *)0x04CC1504,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1508,0x3C400000L);
writeKernelMemory((void *)0x04CC150C,0x6042000FL);
writeKernelMemory((void *)0x04CC1510,0x904A0000L);
writeKernelMemory((void *)0x04CC1514,0x3D401002L);
writeKernelMemory((void *)0x04CC1518,0x814A0068L);
writeKernelMemory((void *)0x04CC151C,0x3C400000L);
writeKernelMemory((void *)0x04CC1520,0x604226DCL);
writeKernelMemory((void *)0x04CC1524,0x7C426214L);
writeKernelMemory((void *)0x04CC1528,0x91420000L);
writeKernelMemory((void *)0x04CC152C,0x384026D4L);
writeKernelMemory((void *)0x04CC1530,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1534,0x3C400000L);
writeKernelMemory((void *)0x04CC1538,0x60420040L);
writeKernelMemory((void *)0x04CC153C,0x904A0000L);
writeKernelMemory((void *)0x04CC1540,0x384026E8L);
writeKernelMemory((void *)0x04CC1544,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1548,0x3C400000L);
writeKernelMemory((void *)0x04CC154C,0x6042000DL);
writeKernelMemory((void *)0x04CC1550,0x904A0000L);
writeKernelMemory((void *)0x04CC1554,0x48000B20L);
writeKernelMemory((void *)0x04CC1558,0x3D80109DL);
writeKernelMemory((void *)0x04CC155C,0x618CF84CL);
writeKernelMemory((void *)0x04CC1560,0x818C0000L);
writeKernelMemory((void *)0x04CC1564,0x3C400000L);
writeKernelMemory((void *)0x04CC1568,0x60420340L);
writeKernelMemory((void *)0x04CC156C,0x7C426214L);
writeKernelMemory((void *)0x04CC1570,0x80420000L);
writeKernelMemory((void *)0x04CC1574,0x3D401002L);
writeKernelMemory((void *)0x04CC1578,0x904A00C8L);
writeKernelMemory((void *)0x04CC157C,0x3C400000L);
writeKernelMemory((void *)0x04CC1580,0x6042000CL);
writeKernelMemory((void *)0x04CC1584,0x7C426214L);
writeKernelMemory((void *)0x04CC1588,0x80420000L);
writeKernelMemory((void *)0x04CC158C,0x3D401002L);
writeKernelMemory((void *)0x04CC1590,0x904A0004L);
writeKernelMemory((void *)0x04CC1594,0x804C0010L);
writeKernelMemory((void *)0x04CC1598,0x3D401002L);
writeKernelMemory((void *)0x04CC159C,0x904A0008L);
writeKernelMemory((void *)0x04CC15A0,0x804C0030L);
writeKernelMemory((void *)0x04CC15A4,0x3D401002L);
writeKernelMemory((void *)0x04CC15A8,0x904A000CL);
writeKernelMemory((void *)0x04CC15AC,0x3C400000L);
writeKernelMemory((void *)0x04CC15B0,0x60420060L);
writeKernelMemory((void *)0x04CC15B4,0x7C426214L);
writeKernelMemory((void *)0x04CC15B8,0x80420000L);
writeKernelMemory((void *)0x04CC15BC,0x3D401002L);
writeKernelMemory((void *)0x04CC15C0,0x904A0010L);
writeKernelMemory((void *)0x04CC15C4,0x3C400000L);
writeKernelMemory((void *)0x04CC15C8,0x60420060L);
writeKernelMemory((void *)0x04CC15CC,0x7C426214L);
writeKernelMemory((void *)0x04CC15D0,0x80420000L);
writeKernelMemory((void *)0x04CC15D4,0x3D401002L);
writeKernelMemory((void *)0x04CC15D8,0x904A0014L);
writeKernelMemory((void *)0x04CC15DC,0x3C400000L);
writeKernelMemory((void *)0x04CC15E0,0x60420060L);
writeKernelMemory((void *)0x04CC15E4,0x7C426214L);
writeKernelMemory((void *)0x04CC15E8,0x80420000L);
writeKernelMemory((void *)0x04CC15EC,0x3D401002L);
writeKernelMemory((void *)0x04CC15F0,0x904A0018L);
writeKernelMemory((void *)0x04CC15F4,0x3C400000L);
writeKernelMemory((void *)0x04CC15F8,0x60420030L);
writeKernelMemory((void *)0x04CC15FC,0x7C426214L);
writeKernelMemory((void *)0x04CC1600,0x80420000L);
writeKernelMemory((void *)0x04CC1604,0x3D401002L);
writeKernelMemory((void *)0x04CC1608,0x904A001CL);
writeKernelMemory((void *)0x04CC160C,0x384002CCL);
writeKernelMemory((void *)0x04CC1610,0x7C426214L);
writeKernelMemory((void *)0x04CC1614,0x80420000L);
writeKernelMemory((void *)0x04CC1618,0x3D401002L);
writeKernelMemory((void *)0x04CC161C,0x904A0020L);
writeKernelMemory((void *)0x04CC1620,0x384002CCL);
writeKernelMemory((void *)0x04CC1624,0x7C426214L);
writeKernelMemory((void *)0x04CC1628,0x80420000L);
writeKernelMemory((void *)0x04CC162C,0x3D401002L);
writeKernelMemory((void *)0x04CC1630,0x904A0024L);
writeKernelMemory((void *)0x04CC1634,0x384002CCL);
writeKernelMemory((void *)0x04CC1638,0x7C426214L);
writeKernelMemory((void *)0x04CC163C,0x80420000L);
writeKernelMemory((void *)0x04CC1640,0x3D401002L);
writeKernelMemory((void *)0x04CC1644,0x904A0028L);
writeKernelMemory((void *)0x04CC1648,0x3C400000L);
writeKernelMemory((void *)0x04CC164C,0x60420004L);
writeKernelMemory((void *)0x04CC1650,0x7C426214L);
writeKernelMemory((void *)0x04CC1654,0x80420000L);
writeKernelMemory((void *)0x04CC1658,0x3D401002L);
writeKernelMemory((void *)0x04CC165C,0x904A002CL);
writeKernelMemory((void *)0x04CC1660,0x3C400000L);
writeKernelMemory((void *)0x04CC1664,0x60420004L);
writeKernelMemory((void *)0x04CC1668,0x7C426214L);
writeKernelMemory((void *)0x04CC166C,0x80420000L);
writeKernelMemory((void *)0x04CC1670,0x3D401002L);
writeKernelMemory((void *)0x04CC1674,0x904A0030L);
writeKernelMemory((void *)0x04CC1678,0x3C400000L);
writeKernelMemory((void *)0x04CC167C,0x60420004L);
writeKernelMemory((void *)0x04CC1680,0x7C426214L);
writeKernelMemory((void *)0x04CC1684,0x80420000L);
writeKernelMemory((void *)0x04CC1688,0x3D401002L);
writeKernelMemory((void *)0x04CC168C,0x904A0034L);
writeKernelMemory((void *)0x04CC1690,0x3C400000L);
writeKernelMemory((void *)0x04CC1694,0x60420004L);
writeKernelMemory((void *)0x04CC1698,0x7C426214L);
writeKernelMemory((void *)0x04CC169C,0x80420000L);
writeKernelMemory((void *)0x04CC16A0,0x3D401002L);
writeKernelMemory((void *)0x04CC16A4,0x904A0038L);
writeKernelMemory((void *)0x04CC16A8,0x3C400000L);
writeKernelMemory((void *)0x04CC16AC,0x60420004L);
writeKernelMemory((void *)0x04CC16B0,0x7C426214L);
writeKernelMemory((void *)0x04CC16B4,0x80420000L);
writeKernelMemory((void *)0x04CC16B8,0x3D401002L);
writeKernelMemory((void *)0x04CC16BC,0x904A003CL);
writeKernelMemory((void *)0x04CC16C0,0x3C400000L);
writeKernelMemory((void *)0x04CC16C4,0x60420004L);
writeKernelMemory((void *)0x04CC16C8,0x7C426214L);
writeKernelMemory((void *)0x04CC16CC,0x80420000L);
writeKernelMemory((void *)0x04CC16D0,0x3D401002L);
writeKernelMemory((void *)0x04CC16D4,0x904A0040L);
writeKernelMemory((void *)0x04CC16D8,0x3C400000L);
writeKernelMemory((void *)0x04CC16DC,0x604202B4L);
writeKernelMemory((void *)0x04CC16E0,0x7C426214L);
writeKernelMemory((void *)0x04CC16E4,0x80420000L);
writeKernelMemory((void *)0x04CC16E8,0x3D401002L);
writeKernelMemory((void *)0x04CC16EC,0x904A0044L);
writeKernelMemory((void *)0x04CC16F0,0x3C400000L);
writeKernelMemory((void *)0x04CC16F4,0x60421034L);
writeKernelMemory((void *)0x04CC16F8,0x7C426214L);
writeKernelMemory((void *)0x04CC16FC,0x80420000L);
writeKernelMemory((void *)0x04CC1700,0x3D401002L);
writeKernelMemory((void *)0x04CC1704,0x904A0048L);
writeKernelMemory((void *)0x04CC1708,0x3C400000L);
writeKernelMemory((void *)0x04CC170C,0x604200A4L);
writeKernelMemory((void *)0x04CC1710,0x7C426214L);
writeKernelMemory((void *)0x04CC1714,0x80420000L);
writeKernelMemory((void *)0x04CC1718,0x3D401002L);
writeKernelMemory((void *)0x04CC171C,0x904A004CL);
writeKernelMemory((void *)0x04CC1720,0x3C400000L);
writeKernelMemory((void *)0x04CC1724,0x604200A8L);
writeKernelMemory((void *)0x04CC1728,0x7C426214L);
writeKernelMemory((void *)0x04CC172C,0x80420000L);
writeKernelMemory((void *)0x04CC1730,0x3D401002L);
writeKernelMemory((void *)0x04CC1734,0x904A00CCL);
writeKernelMemory((void *)0x04CC1738,0x3C400000L);
writeKernelMemory((void *)0x04CC173C,0x60420058L);
writeKernelMemory((void *)0x04CC1740,0x7C426214L);
writeKernelMemory((void *)0x04CC1744,0x80420000L);
writeKernelMemory((void *)0x04CC1748,0x3D401002L);
writeKernelMemory((void *)0x04CC174C,0x904A0050L);
writeKernelMemory((void *)0x04CC1750,0x3C400000L);
writeKernelMemory((void *)0x04CC1754,0x604200E4L);
writeKernelMemory((void *)0x04CC1758,0x7C426214L);
writeKernelMemory((void *)0x04CC175C,0x80420000L);
writeKernelMemory((void *)0x04CC1760,0x3D401002L);
writeKernelMemory((void *)0x04CC1764,0x904A0078L);
writeKernelMemory((void *)0x04CC1768,0x3C400000L);
writeKernelMemory((void *)0x04CC176C,0x60420214L);
writeKernelMemory((void *)0x04CC1770,0x7C426214L);
writeKernelMemory((void *)0x04CC1774,0x80420000L);
writeKernelMemory((void *)0x04CC1778,0x3D401002L);
writeKernelMemory((void *)0x04CC177C,0x904A007CL);
writeKernelMemory((void *)0x04CC1780,0x3C400000L);
writeKernelMemory((void *)0x04CC1784,0x6042026CL);
writeKernelMemory((void *)0x04CC1788,0x7C426214L);
writeKernelMemory((void *)0x04CC178C,0x80420000L);
writeKernelMemory((void *)0x04CC1790,0x3D401002L);
writeKernelMemory((void *)0x04CC1794,0x904A0080L);
writeKernelMemory((void *)0x04CC1798,0x3C400000L);
writeKernelMemory((void *)0x04CC179C,0x60420040L);
writeKernelMemory((void *)0x04CC17A0,0x7C426214L);
writeKernelMemory((void *)0x04CC17A4,0x80420000L);
writeKernelMemory((void *)0x04CC17A8,0x3D401002L);
writeKernelMemory((void *)0x04CC17AC,0x904A0084L);
writeKernelMemory((void *)0x04CC17B0,0x3C400000L);
writeKernelMemory((void *)0x04CC17B4,0x60420054L);
writeKernelMemory((void *)0x04CC17B8,0x7C426214L);
writeKernelMemory((void *)0x04CC17BC,0x80420000L);
writeKernelMemory((void *)0x04CC17C0,0x3D401002L);
writeKernelMemory((void *)0x04CC17C4,0x904A0088L);
writeKernelMemory((void *)0x04CC17C8,0x3C400000L);
writeKernelMemory((void *)0x04CC17CC,0x604200E0L);
writeKernelMemory((void *)0x04CC17D0,0x7C426214L);
writeKernelMemory((void *)0x04CC17D4,0x80420000L);
writeKernelMemory((void *)0x04CC17D8,0x3D401002L);
writeKernelMemory((void *)0x04CC17DC,0x904A008CL);
writeKernelMemory((void *)0x04CC17E0,0x3C400000L);
writeKernelMemory((void *)0x04CC17E4,0x60420124L);
writeKernelMemory((void *)0x04CC17E8,0x7C426214L);
writeKernelMemory((void *)0x04CC17EC,0x80420000L);
writeKernelMemory((void *)0x04CC17F0,0x3D401002L);
writeKernelMemory((void *)0x04CC17F4,0x904A0054L);
writeKernelMemory((void *)0x04CC17F8,0x3C400000L);
writeKernelMemory((void *)0x04CC17FC,0x6042003CL);
writeKernelMemory((void *)0x04CC1800,0x7C426214L);
writeKernelMemory((void *)0x04CC1804,0x80420000L);
writeKernelMemory((void *)0x04CC1808,0x3D401002L);
writeKernelMemory((void *)0x04CC180C,0x904A0058L);
writeKernelMemory((void *)0x04CC1810,0x3C400000L);
writeKernelMemory((void *)0x04CC1814,0x60420038L);
writeKernelMemory((void *)0x04CC1818,0x7C426214L);
writeKernelMemory((void *)0x04CC181C,0x80420000L);
writeKernelMemory((void *)0x04CC1820,0x3D401002L);
writeKernelMemory((void *)0x04CC1824,0x904A005CL);
writeKernelMemory((void *)0x04CC1828,0x3C400000L);
writeKernelMemory((void *)0x04CC182C,0x60420204L);
writeKernelMemory((void *)0x04CC1830,0x7C426214L);
writeKernelMemory((void *)0x04CC1834,0x80420000L);
writeKernelMemory((void *)0x04CC1838,0x3D401002L);
writeKernelMemory((void *)0x04CC183C,0x904A0060L);
writeKernelMemory((void *)0x04CC1840,0x3C400000L);
writeKernelMemory((void *)0x04CC1844,0x60420264L);
writeKernelMemory((void *)0x04CC1848,0x7C426214L);
writeKernelMemory((void *)0x04CC184C,0x80420000L);
writeKernelMemory((void *)0x04CC1850,0x3D401002L);
writeKernelMemory((void *)0x04CC1854,0x904A0064L);
writeKernelMemory((void *)0x04CC1858,0x3C400000L);
writeKernelMemory((void *)0x04CC185C,0x60420358L);
writeKernelMemory((void *)0x04CC1860,0x7C426214L);
writeKernelMemory((void *)0x04CC1864,0x80420000L);
writeKernelMemory((void *)0x04CC1868,0x3D401002L);
writeKernelMemory((void *)0x04CC186C,0x904A0068L);
writeKernelMemory((void *)0x04CC1870,0x3D8010A0L);
writeKernelMemory((void *)0x04CC1874,0x618CA6C0L);
writeKernelMemory((void *)0x04CC1878,0x818C0000L);
writeKernelMemory((void *)0x04CC187C,0x3C400000L);
writeKernelMemory((void *)0x04CC1880,0x60420004L);
writeKernelMemory((void *)0x04CC1884,0x7D8C1214L);
writeKernelMemory((void *)0x04CC1888,0x818C0000L);
writeKernelMemory((void *)0x04CC188C,0x3D401002L);
writeKernelMemory((void *)0x04CC1890,0x814A00C8L);
writeKernelMemory((void *)0x04CC1894,0x3C400000L);
writeKernelMemory((void *)0x04CC1898,0x604201C4L);
writeKernelMemory((void *)0x04CC189C,0x7C426214L);
writeKernelMemory((void *)0x04CC18A0,0x91420000L);
writeKernelMemory((void *)0x04CC18A4,0x3D401002L);
writeKernelMemory((void *)0x04CC18A8,0x814A0028L);
writeKernelMemory((void *)0x04CC18AC,0x3C400000L);
writeKernelMemory((void *)0x04CC18B0,0x60420C8CL);
writeKernelMemory((void *)0x04CC18B4,0x7C426214L);
writeKernelMemory((void *)0x04CC18B8,0x91420000L);
writeKernelMemory((void *)0x04CC18BC,0x38404814L);
writeKernelMemory((void *)0x04CC18C0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC18C4,0x3C400000L);
writeKernelMemory((void *)0x04CC18C8,0x60420001L);
writeKernelMemory((void *)0x04CC18CC,0x904A0000L);
writeKernelMemory((void *)0x04CC18D0,0x38404828L);
writeKernelMemory((void *)0x04CC18D4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC18D8,0x3C400000L);
writeKernelMemory((void *)0x04CC18DC,0x904A0000L);
writeKernelMemory((void *)0x04CC18E0,0x3D401002L);
writeKernelMemory((void *)0x04CC18E4,0x814A0004L);
writeKernelMemory((void *)0x04CC18E8,0x3C400000L);
writeKernelMemory((void *)0x04CC18EC,0x604202C4L);
writeKernelMemory((void *)0x04CC18F0,0x7C426214L);
writeKernelMemory((void *)0x04CC18F4,0x91420000L);
writeKernelMemory((void *)0x04CC18F8,0x3840000CL);
writeKernelMemory((void *)0x04CC18FC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1900,0x3C400000L);
writeKernelMemory((void *)0x04CC1904,0x60420001L);
writeKernelMemory((void *)0x04CC1908,0x904A0000L);
writeKernelMemory((void *)0x04CC190C,0x38400020L);
writeKernelMemory((void *)0x04CC1910,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1914,0x3C400000L);
writeKernelMemory((void *)0x04CC1918,0x904A0000L);
writeKernelMemory((void *)0x04CC191C,0x3D401002L);
writeKernelMemory((void *)0x04CC1920,0x814A0008L);
writeKernelMemory((void *)0x04CC1924,0x3C400000L);
writeKernelMemory((void *)0x04CC1928,0x604203C4L);
writeKernelMemory((void *)0x04CC192C,0x7C426214L);
writeKernelMemory((void *)0x04CC1930,0x91420000L);
writeKernelMemory((void *)0x04CC1934,0x384001BCL);
writeKernelMemory((void *)0x04CC1938,0x7D4C1214L);
writeKernelMemory((void *)0x04CC193C,0x3C400000L);
writeKernelMemory((void *)0x04CC1940,0x60420001L);
writeKernelMemory((void *)0x04CC1944,0x904A0000L);
writeKernelMemory((void *)0x04CC1948,0x384001D0L);
writeKernelMemory((void *)0x04CC194C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1950,0x3C400000L);
writeKernelMemory((void *)0x04CC1954,0x904A0000L);
writeKernelMemory((void *)0x04CC1958,0x3D401002L);
writeKernelMemory((void *)0x04CC195C,0x814A000CL);
writeKernelMemory((void *)0x04CC1960,0x3C400000L);
writeKernelMemory((void *)0x04CC1964,0x60420544L);
writeKernelMemory((void *)0x04CC1968,0x7C426214L);
writeKernelMemory((void *)0x04CC196C,0x91420000L);
writeKernelMemory((void *)0x04CC1970,0x384002BCL);
writeKernelMemory((void *)0x04CC1974,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1978,0x3C400000L);
writeKernelMemory((void *)0x04CC197C,0x60420001L);
writeKernelMemory((void *)0x04CC1980,0x904A0000L);
writeKernelMemory((void *)0x04CC1984,0x384002D0L);
writeKernelMemory((void *)0x04CC1988,0x7D4C1214L);
writeKernelMemory((void *)0x04CC198C,0x3C400000L);
writeKernelMemory((void *)0x04CC1990,0x904A0000L);
writeKernelMemory((void *)0x04CC1994,0x3D401002L);
writeKernelMemory((void *)0x04CC1998,0x814A0010L);
writeKernelMemory((void *)0x04CC199C,0x3C400000L);
writeKernelMemory((void *)0x04CC19A0,0x60420644L);
writeKernelMemory((void *)0x04CC19A4,0x7C426214L);
writeKernelMemory((void *)0x04CC19A8,0x91420000L);
writeKernelMemory((void *)0x04CC19AC,0x384003BCL);
writeKernelMemory((void *)0x04CC19B0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC19B4,0x3C400000L);
writeKernelMemory((void *)0x04CC19B8,0x60420001L);
writeKernelMemory((void *)0x04CC19BC,0x904A0000L);
writeKernelMemory((void *)0x04CC19C0,0x384003D0L);
writeKernelMemory((void *)0x04CC19C4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC19C8,0x3C400000L);
writeKernelMemory((void *)0x04CC19CC,0x904A0000L);
writeKernelMemory((void *)0x04CC19D0,0x3D401002L);
writeKernelMemory((void *)0x04CC19D4,0x814A0014L);
writeKernelMemory((void *)0x04CC19D8,0x3C400000L);
writeKernelMemory((void *)0x04CC19DC,0x60420744L);
writeKernelMemory((void *)0x04CC19E0,0x7C426214L);
writeKernelMemory((void *)0x04CC19E4,0x91420000L);
writeKernelMemory((void *)0x04CC19E8,0x3840053CL);
writeKernelMemory((void *)0x04CC19EC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC19F0,0x3C400000L);
writeKernelMemory((void *)0x04CC19F4,0x60420001L);
writeKernelMemory((void *)0x04CC19F8,0x904A0000L);
writeKernelMemory((void *)0x04CC19FC,0x38400550L);
writeKernelMemory((void *)0x04CC1A00,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1A04,0x3C400000L);
writeKernelMemory((void *)0x04CC1A08,0x904A0000L);
writeKernelMemory((void *)0x04CC1A0C,0x3D401002L);
writeKernelMemory((void *)0x04CC1A10,0x814A0018L);
writeKernelMemory((void *)0x04CC1A14,0x3C400000L);
writeKernelMemory((void *)0x04CC1A18,0x60420844L);
writeKernelMemory((void *)0x04CC1A1C,0x7C426214L);
writeKernelMemory((void *)0x04CC1A20,0x91420000L);
writeKernelMemory((void *)0x04CC1A24,0x3840063CL);
writeKernelMemory((void *)0x04CC1A28,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1A2C,0x3C400000L);
writeKernelMemory((void *)0x04CC1A30,0x60420001L);
writeKernelMemory((void *)0x04CC1A34,0x904A0000L);
writeKernelMemory((void *)0x04CC1A38,0x38400650L);
writeKernelMemory((void *)0x04CC1A3C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1A40,0x3C400000L);
writeKernelMemory((void *)0x04CC1A44,0x904A0000L);
writeKernelMemory((void *)0x04CC1A48,0x3D401002L);
writeKernelMemory((void *)0x04CC1A4C,0x814A001CL);
writeKernelMemory((void *)0x04CC1A50,0x3C400000L);
writeKernelMemory((void *)0x04CC1A54,0x60420944L);
writeKernelMemory((void *)0x04CC1A58,0x7C426214L);
writeKernelMemory((void *)0x04CC1A5C,0x91420000L);
writeKernelMemory((void *)0x04CC1A60,0x3840073CL);
writeKernelMemory((void *)0x04CC1A64,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1A68,0x3C400000L);
writeKernelMemory((void *)0x04CC1A6C,0x60420001L);
writeKernelMemory((void *)0x04CC1A70,0x904A0000L);
writeKernelMemory((void *)0x04CC1A74,0x38400750L);
writeKernelMemory((void *)0x04CC1A78,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1A7C,0x3C400000L);
writeKernelMemory((void *)0x04CC1A80,0x60420002L);
writeKernelMemory((void *)0x04CC1A84,0x904A0000L);
writeKernelMemory((void *)0x04CC1A88,0x3D401002L);
writeKernelMemory((void *)0x04CC1A8C,0x814A0020L);
writeKernelMemory((void *)0x04CC1A90,0x3C400000L);
writeKernelMemory((void *)0x04CC1A94,0x60420A5CL);
writeKernelMemory((void *)0x04CC1A98,0x7C426214L);
writeKernelMemory((void *)0x04CC1A9C,0x91420000L);
writeKernelMemory((void *)0x04CC1AA0,0x3840083CL);
writeKernelMemory((void *)0x04CC1AA4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1AA8,0x3C400000L);
writeKernelMemory((void *)0x04CC1AAC,0x60420001L);
writeKernelMemory((void *)0x04CC1AB0,0x904A0000L);
writeKernelMemory((void *)0x04CC1AB4,0x38400850L);
writeKernelMemory((void *)0x04CC1AB8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1ABC,0x3C400000L);
writeKernelMemory((void *)0x04CC1AC0,0x60420001L);
writeKernelMemory((void *)0x04CC1AC4,0x904A0000L);
writeKernelMemory((void *)0x04CC1AC8,0x3D401002L);
writeKernelMemory((void *)0x04CC1ACC,0x814A0024L);
writeKernelMemory((void *)0x04CC1AD0,0x3C400000L);
writeKernelMemory((void *)0x04CC1AD4,0x60420B74L);
writeKernelMemory((void *)0x04CC1AD8,0x7C426214L);
writeKernelMemory((void *)0x04CC1ADC,0x91420000L);
writeKernelMemory((void *)0x04CC1AE0,0x3840093CL);
writeKernelMemory((void *)0x04CC1AE4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1AE8,0x3C400000L);
writeKernelMemory((void *)0x04CC1AEC,0x60420001L);
writeKernelMemory((void *)0x04CC1AF0,0x904A0000L);
writeKernelMemory((void *)0x04CC1AF4,0x38400950L);
writeKernelMemory((void *)0x04CC1AF8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1AFC,0x3C400000L);
writeKernelMemory((void *)0x04CC1B00,0x60420001L);
writeKernelMemory((void *)0x04CC1B04,0x904A0000L);
writeKernelMemory((void *)0x04CC1B08,0x38400A54L);
writeKernelMemory((void *)0x04CC1B0C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1B10,0x3C400000L);
writeKernelMemory((void *)0x04CC1B14,0x60420001L);
writeKernelMemory((void *)0x04CC1B18,0x904A0000L);
writeKernelMemory((void *)0x04CC1B1C,0x38400A68L);
writeKernelMemory((void *)0x04CC1B20,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1B24,0x3C400000L);
writeKernelMemory((void *)0x04CC1B28,0x904A0000L);
writeKernelMemory((void *)0x04CC1B2C,0x3D401002L);
writeKernelMemory((void *)0x04CC1B30,0x814A002CL);
writeKernelMemory((void *)0x04CC1B34,0x3C400000L);
writeKernelMemory((void *)0x04CC1B38,0x60420DA4L);
writeKernelMemory((void *)0x04CC1B3C,0x7C426214L);
writeKernelMemory((void *)0x04CC1B40,0x91420000L);
writeKernelMemory((void *)0x04CC1B44,0x38400B6CL);
writeKernelMemory((void *)0x04CC1B48,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1B4C,0x3C400000L);
writeKernelMemory((void *)0x04CC1B50,0x60420001L);
writeKernelMemory((void *)0x04CC1B54,0x904A0000L);
writeKernelMemory((void *)0x04CC1B58,0x38400B80L);
writeKernelMemory((void *)0x04CC1B5C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1B60,0x3C400000L);
writeKernelMemory((void *)0x04CC1B64,0x60420002L);
writeKernelMemory((void *)0x04CC1B68,0x904A0000L);
writeKernelMemory((void *)0x04CC1B6C,0x3D401002L);
writeKernelMemory((void *)0x04CC1B70,0x814A0030L);
writeKernelMemory((void *)0x04CC1B74,0x3C400000L);
writeKernelMemory((void *)0x04CC1B78,0x60420F0CL);
writeKernelMemory((void *)0x04CC1B7C,0x7C426214L);
writeKernelMemory((void *)0x04CC1B80,0x91420000L);
writeKernelMemory((void *)0x04CC1B84,0x38400C84L);
writeKernelMemory((void *)0x04CC1B88,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1B8C,0x3C400000L);
writeKernelMemory((void *)0x04CC1B90,0x60420001L);
writeKernelMemory((void *)0x04CC1B94,0x904A0000L);
writeKernelMemory((void *)0x04CC1B98,0x38400C98L);
writeKernelMemory((void *)0x04CC1B9C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1BA0,0x3C400000L);
writeKernelMemory((void *)0x04CC1BA4,0x60420001L);
writeKernelMemory((void *)0x04CC1BA8,0x904A0000L);
writeKernelMemory((void *)0x04CC1BAC,0x3D401002L);
writeKernelMemory((void *)0x04CC1BB0,0x814A0034L);
writeKernelMemory((void *)0x04CC1BB4,0x3C400000L);
writeKernelMemory((void *)0x04CC1BB8,0x60421024L);
writeKernelMemory((void *)0x04CC1BBC,0x7C426214L);
writeKernelMemory((void *)0x04CC1BC0,0x91420000L);
writeKernelMemory((void *)0x04CC1BC4,0x38400D9CL);
writeKernelMemory((void *)0x04CC1BC8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1BCC,0x3C400000L);
writeKernelMemory((void *)0x04CC1BD0,0x60420001L);
writeKernelMemory((void *)0x04CC1BD4,0x904A0000L);
writeKernelMemory((void *)0x04CC1BD8,0x38400DB0L);
writeKernelMemory((void *)0x04CC1BDC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1BE0,0x3C400000L);
writeKernelMemory((void *)0x04CC1BE4,0x60420001L);
writeKernelMemory((void *)0x04CC1BE8,0x904A0000L);
writeKernelMemory((void *)0x04CC1BEC,0x3D401002L);
writeKernelMemory((void *)0x04CC1BF0,0x814A0038L);
writeKernelMemory((void *)0x04CC1BF4,0x3C400000L);
writeKernelMemory((void *)0x04CC1BF8,0x6042113CL);
writeKernelMemory((void *)0x04CC1BFC,0x7C426214L);
writeKernelMemory((void *)0x04CC1C00,0x91420000L);
writeKernelMemory((void *)0x04CC1C04,0x38400F04L);
writeKernelMemory((void *)0x04CC1C08,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1C0C,0x3C400000L);
writeKernelMemory((void *)0x04CC1C10,0x60420001L);
writeKernelMemory((void *)0x04CC1C14,0x904A0000L);
writeKernelMemory((void *)0x04CC1C18,0x38400F18L);
writeKernelMemory((void *)0x04CC1C1C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1C20,0x3C400000L);
writeKernelMemory((void *)0x04CC1C24,0x60420002L);
writeKernelMemory((void *)0x04CC1C28,0x904A0000L);
writeKernelMemory((void *)0x04CC1C2C,0x3D401002L);
writeKernelMemory((void *)0x04CC1C30,0x814A003CL);
writeKernelMemory((void *)0x04CC1C34,0x3C400000L);
writeKernelMemory((void *)0x04CC1C38,0x60421254L);
writeKernelMemory((void *)0x04CC1C3C,0x7C426214L);
writeKernelMemory((void *)0x04CC1C40,0x91420000L);
writeKernelMemory((void *)0x04CC1C44,0x3840101CL);
writeKernelMemory((void *)0x04CC1C48,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1C4C,0x3C400000L);
writeKernelMemory((void *)0x04CC1C50,0x60420001L);
writeKernelMemory((void *)0x04CC1C54,0x904A0000L);
writeKernelMemory((void *)0x04CC1C58,0x38401030L);
writeKernelMemory((void *)0x04CC1C5C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1C60,0x3C400000L);
writeKernelMemory((void *)0x04CC1C64,0x60420005L);
writeKernelMemory((void *)0x04CC1C68,0x904A0000L);
writeKernelMemory((void *)0x04CC1C6C,0x3D401002L);
writeKernelMemory((void *)0x04CC1C70,0x814A0040L);
writeKernelMemory((void *)0x04CC1C74,0x3C400000L);
writeKernelMemory((void *)0x04CC1C78,0x6042136CL);
writeKernelMemory((void *)0x04CC1C7C,0x7C426214L);
writeKernelMemory((void *)0x04CC1C80,0x91420000L);
writeKernelMemory((void *)0x04CC1C84,0x38401134L);
writeKernelMemory((void *)0x04CC1C88,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1C8C,0x3C400000L);
writeKernelMemory((void *)0x04CC1C90,0x60420001L);
writeKernelMemory((void *)0x04CC1C94,0x904A0000L);
writeKernelMemory((void *)0x04CC1C98,0x38401148L);
writeKernelMemory((void *)0x04CC1C9C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1CA0,0x3C400000L);
writeKernelMemory((void *)0x04CC1CA4,0x60420006L);
writeKernelMemory((void *)0x04CC1CA8,0x904A0000L);
writeKernelMemory((void *)0x04CC1CAC,0x3D401002L);
writeKernelMemory((void *)0x04CC1CB0,0x814A0044L);
writeKernelMemory((void *)0x04CC1CB4,0x3C400000L);
writeKernelMemory((void *)0x04CC1CB8,0x60421484L);
writeKernelMemory((void *)0x04CC1CBC,0x7C426214L);
writeKernelMemory((void *)0x04CC1CC0,0x91420000L);
writeKernelMemory((void *)0x04CC1CC4,0x3840124CL);
writeKernelMemory((void *)0x04CC1CC8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1CCC,0x3C400000L);
writeKernelMemory((void *)0x04CC1CD0,0x60420001L);
writeKernelMemory((void *)0x04CC1CD4,0x904A0000L);
writeKernelMemory((void *)0x04CC1CD8,0x38401260L);
writeKernelMemory((void *)0x04CC1CDC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1CE0,0x3C400000L);
writeKernelMemory((void *)0x04CC1CE4,0x60420003L);
writeKernelMemory((void *)0x04CC1CE8,0x904A0000L);
writeKernelMemory((void *)0x04CC1CEC,0x3D401002L);
writeKernelMemory((void *)0x04CC1CF0,0x814A0048L);
writeKernelMemory((void *)0x04CC1CF4,0x3C400000L);
writeKernelMemory((void *)0x04CC1CF8,0x60420DF4L);
writeKernelMemory((void *)0x04CC1CFC,0x7C426214L);
writeKernelMemory((void *)0x04CC1D00,0x91420000L);
writeKernelMemory((void *)0x04CC1D04,0x38401364L);
writeKernelMemory((void *)0x04CC1D08,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1D0C,0x3C400000L);
writeKernelMemory((void *)0x04CC1D10,0x60420001L);
writeKernelMemory((void *)0x04CC1D14,0x904A0000L);
writeKernelMemory((void *)0x04CC1D18,0x38401378L);
writeKernelMemory((void *)0x04CC1D1C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1D20,0x3C400000L);
writeKernelMemory((void *)0x04CC1D24,0x60420004L);
writeKernelMemory((void *)0x04CC1D28,0x904A0000L);
writeKernelMemory((void *)0x04CC1D2C,0x3D401002L);
writeKernelMemory((void *)0x04CC1D30,0x814A004CL);
writeKernelMemory((void *)0x04CC1D34,0x3C400000L);
writeKernelMemory((void *)0x04CC1D38,0x604216DCL);
writeKernelMemory((void *)0x04CC1D3C,0x7C426214L);
writeKernelMemory((void *)0x04CC1D40,0x91420000L);
writeKernelMemory((void *)0x04CC1D44,0x3840147CL);
writeKernelMemory((void *)0x04CC1D48,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1D4C,0x3C400000L);
writeKernelMemory((void *)0x04CC1D50,0x60420001L);
writeKernelMemory((void *)0x04CC1D54,0x904A0000L);
writeKernelMemory((void *)0x04CC1D58,0x38401490L);
writeKernelMemory((void *)0x04CC1D5C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1D60,0x3C400000L);
writeKernelMemory((void *)0x04CC1D64,0x904A0000L);
writeKernelMemory((void *)0x04CC1D68,0x3D401002L);
writeKernelMemory((void *)0x04CC1D6C,0x814A00CCL);
writeKernelMemory((void *)0x04CC1D70,0x3C400000L);
writeKernelMemory((void *)0x04CC1D74,0x604217F4L);
writeKernelMemory((void *)0x04CC1D78,0x7C426214L);
writeKernelMemory((void *)0x04CC1D7C,0x91420000L);
writeKernelMemory((void *)0x04CC1D80,0x38400DECL);
writeKernelMemory((void *)0x04CC1D84,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1D88,0x3C400000L);
writeKernelMemory((void *)0x04CC1D8C,0x60420001L);
writeKernelMemory((void *)0x04CC1D90,0x904A0000L);
writeKernelMemory((void *)0x04CC1D94,0x38400E00L);
writeKernelMemory((void *)0x04CC1D98,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1D9C,0x3C400000L);
writeKernelMemory((void *)0x04CC1DA0,0x904A0000L);
writeKernelMemory((void *)0x04CC1DA4,0x3D401002L);
writeKernelMemory((void *)0x04CC1DA8,0x814A0050L);
writeKernelMemory((void *)0x04CC1DAC,0x3C400000L);
writeKernelMemory((void *)0x04CC1DB0,0x6042190CL);
writeKernelMemory((void *)0x04CC1DB4,0x7C426214L);
writeKernelMemory((void *)0x04CC1DB8,0x91420000L);
writeKernelMemory((void *)0x04CC1DBC,0x384016D4L);
writeKernelMemory((void *)0x04CC1DC0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1DC4,0x3C400000L);
writeKernelMemory((void *)0x04CC1DC8,0x60420001L);
writeKernelMemory((void *)0x04CC1DCC,0x904A0000L);
writeKernelMemory((void *)0x04CC1DD0,0x384016E8L);
writeKernelMemory((void *)0x04CC1DD4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1DD8,0x3C400000L);
writeKernelMemory((void *)0x04CC1DDC,0x904A0000L);
writeKernelMemory((void *)0x04CC1DE0,0x3D401002L);
writeKernelMemory((void *)0x04CC1DE4,0x814A0078L);
writeKernelMemory((void *)0x04CC1DE8,0x3C400000L);
writeKernelMemory((void *)0x04CC1DEC,0x60421A24L);
writeKernelMemory((void *)0x04CC1DF0,0x7C426214L);
writeKernelMemory((void *)0x04CC1DF4,0x91420000L);
writeKernelMemory((void *)0x04CC1DF8,0x384017ECL);
writeKernelMemory((void *)0x04CC1DFC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1E00,0x3C400000L);
writeKernelMemory((void *)0x04CC1E04,0x60420001L);
writeKernelMemory((void *)0x04CC1E08,0x904A0000L);
writeKernelMemory((void *)0x04CC1E0C,0x38401800L);
writeKernelMemory((void *)0x04CC1E10,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1E14,0x38400000L);
writeKernelMemory((void *)0x04CC1E18,0x904A0000L);
writeKernelMemory((void *)0x04CC1E1C,0x3D401002L);
writeKernelMemory((void *)0x04CC1E20,0x814A007CL);
writeKernelMemory((void *)0x04CC1E24,0x914C1B3CL);
writeKernelMemory((void *)0x04CC1E28,0x38400001L);
writeKernelMemory((void *)0x04CC1E2C,0x904C1904L);
writeKernelMemory((void *)0x04CC1E30,0x38400000L);
writeKernelMemory((void *)0x04CC1E34,0x904C1918L);
writeKernelMemory((void *)0x04CC1E38,0x3D401002L);
writeKernelMemory((void *)0x04CC1E3C,0x814A0080L);
writeKernelMemory((void *)0x04CC1E40,0x914C1C54L);
writeKernelMemory((void *)0x04CC1E44,0x38400001L);
writeKernelMemory((void *)0x04CC1E48,0x904C1A1CL);
writeKernelMemory((void *)0x04CC1E4C,0x38400000L);
writeKernelMemory((void *)0x04CC1E50,0x904C1A30L);
writeKernelMemory((void *)0x04CC1E54,0x3D401002L);
writeKernelMemory((void *)0x04CC1E58,0x814A0084L);
writeKernelMemory((void *)0x04CC1E5C,0x914C1D6CL);
writeKernelMemory((void *)0x04CC1E60,0x38401B34L);
writeKernelMemory((void *)0x04CC1E64,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1E68,0x38400001L);
writeKernelMemory((void *)0x04CC1E6C,0x904A0000L);
writeKernelMemory((void *)0x04CC1E70,0x38400000L);
writeKernelMemory((void *)0x04CC1E74,0x904C1B48L);
writeKernelMemory((void *)0x04CC1E78,0x3D401002L);
writeKernelMemory((void *)0x04CC1E7C,0x814A0088L);
writeKernelMemory((void *)0x04CC1E80,0x914C1E84L);
writeKernelMemory((void *)0x04CC1E84,0x38401C4CL);
writeKernelMemory((void *)0x04CC1E88,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1E8C,0x38400001L);
writeKernelMemory((void *)0x04CC1E90,0x904A0000L);
writeKernelMemory((void *)0x04CC1E94,0x38401C60L);
writeKernelMemory((void *)0x04CC1E98,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1E9C,0x38400000L);
writeKernelMemory((void *)0x04CC1EA0,0x904A0000L);
writeKernelMemory((void *)0x04CC1EA4,0x3D401002L);
writeKernelMemory((void *)0x04CC1EA8,0x814A008CL);
writeKernelMemory((void *)0x04CC1EAC,0x914C204CL);
writeKernelMemory((void *)0x04CC1EB0,0x38401D64L);
writeKernelMemory((void *)0x04CC1EB4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1EB8,0x3C400000L);
writeKernelMemory((void *)0x04CC1EBC,0x60420001L);
writeKernelMemory((void *)0x04CC1EC0,0x904A0000L);
writeKernelMemory((void *)0x04CC1EC4,0x38401D78L);
writeKernelMemory((void *)0x04CC1EC8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1ECC,0x3C400000L);
writeKernelMemory((void *)0x04CC1ED0,0x904A0000L);
writeKernelMemory((void *)0x04CC1ED4,0x38401E7CL);
writeKernelMemory((void *)0x04CC1ED8,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1EDC,0x3C400000L);
writeKernelMemory((void *)0x04CC1EE0,0x60420001L);
writeKernelMemory((void *)0x04CC1EE4,0x904A0000L);
writeKernelMemory((void *)0x04CC1EE8,0x38401E90L);
writeKernelMemory((void *)0x04CC1EEC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1EF0,0x3C400000L);
writeKernelMemory((void *)0x04CC1EF4,0x904A0000L);
writeKernelMemory((void *)0x04CC1EF8,0x38402044L);
writeKernelMemory((void *)0x04CC1EFC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1F00,0x3C400000L);
writeKernelMemory((void *)0x04CC1F04,0x60420001L);
writeKernelMemory((void *)0x04CC1F08,0x904A0000L);
writeKernelMemory((void *)0x04CC1F0C,0x38402058L);
writeKernelMemory((void *)0x04CC1F10,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1F14,0x3C400000L);
writeKernelMemory((void *)0x04CC1F18,0x904A0000L);
writeKernelMemory((void *)0x04CC1F1C,0x3D401002L);
writeKernelMemory((void *)0x04CC1F20,0x814A0054L);
writeKernelMemory((void *)0x04CC1F24,0x3C400000L);
writeKernelMemory((void *)0x04CC1F28,0x60422164L);
writeKernelMemory((void *)0x04CC1F2C,0x7C426214L);
writeKernelMemory((void *)0x04CC1F30,0x91420000L);
writeKernelMemory((void *)0x04CC1F34,0x3840215CL);
writeKernelMemory((void *)0x04CC1F38,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1F3C,0x3C400000L);
writeKernelMemory((void *)0x04CC1F40,0x60420001L);
writeKernelMemory((void *)0x04CC1F44,0x904A0000L);
writeKernelMemory((void *)0x04CC1F48,0x38402170L);
writeKernelMemory((void *)0x04CC1F4C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1F50,0x3C400000L);
writeKernelMemory((void *)0x04CC1F54,0x904A0000L);
writeKernelMemory((void *)0x04CC1F58,0x3D401002L);
writeKernelMemory((void *)0x04CC1F5C,0x814A0058L);
writeKernelMemory((void *)0x04CC1F60,0x3C400000L);
writeKernelMemory((void *)0x04CC1F64,0x6042227CL);
writeKernelMemory((void *)0x04CC1F68,0x7C426214L);
writeKernelMemory((void *)0x04CC1F6C,0x91420000L);
writeKernelMemory((void *)0x04CC1F70,0x38402274L);
writeKernelMemory((void *)0x04CC1F74,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1F78,0x3C400000L);
writeKernelMemory((void *)0x04CC1F7C,0x60420001L);
writeKernelMemory((void *)0x04CC1F80,0x904A0000L);
writeKernelMemory((void *)0x04CC1F84,0x38402288L);
writeKernelMemory((void *)0x04CC1F88,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1F8C,0x3C400000L);
writeKernelMemory((void *)0x04CC1F90,0x904A0000L);
writeKernelMemory((void *)0x04CC1F94,0x3D401002L);
writeKernelMemory((void *)0x04CC1F98,0x814A005CL);
writeKernelMemory((void *)0x04CC1F9C,0x3C400000L);
writeKernelMemory((void *)0x04CC1FA0,0x60422394L);
writeKernelMemory((void *)0x04CC1FA4,0x7C426214L);
writeKernelMemory((void *)0x04CC1FA8,0x91420000L);
writeKernelMemory((void *)0x04CC1FAC,0x3840238CL);
writeKernelMemory((void *)0x04CC1FB0,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1FB4,0x3C400000L);
writeKernelMemory((void *)0x04CC1FB8,0x60420001L);
writeKernelMemory((void *)0x04CC1FBC,0x904A0000L);
writeKernelMemory((void *)0x04CC1FC0,0x384023A0L);
writeKernelMemory((void *)0x04CC1FC4,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1FC8,0x3C400000L);
writeKernelMemory((void *)0x04CC1FCC,0x904A0000L);
writeKernelMemory((void *)0x04CC1FD0,0x3D401002L);
writeKernelMemory((void *)0x04CC1FD4,0x814A0060L);
writeKernelMemory((void *)0x04CC1FD8,0x3C400000L);
writeKernelMemory((void *)0x04CC1FDC,0x604224ACL);
writeKernelMemory((void *)0x04CC1FE0,0x7C426214L);
writeKernelMemory((void *)0x04CC1FE4,0x91420000L);
writeKernelMemory((void *)0x04CC1FE8,0x384024A4L);
writeKernelMemory((void *)0x04CC1FEC,0x7D4C1214L);
writeKernelMemory((void *)0x04CC1FF0,0x3C400000L);
writeKernelMemory((void *)0x04CC1FF4,0x60420001L);
writeKernelMemory((void *)0x04CC1FF8,0x904A0000L);
writeKernelMemory((void *)0x04CC1FFC,0x384024B8L);
writeKernelMemory((void *)0x04CC2000,0x7D4C1214L);
writeKernelMemory((void *)0x04CC2004,0x3C400000L);
writeKernelMemory((void *)0x04CC2008,0x904A0000L);
writeKernelMemory((void *)0x04CC200C,0x3D401002L);
writeKernelMemory((void *)0x04CC2010,0x814A0064L);
writeKernelMemory((void *)0x04CC2014,0x3C400000L);
writeKernelMemory((void *)0x04CC2018,0x604225C4L);
writeKernelMemory((void *)0x04CC201C,0x7C426214L);
writeKernelMemory((void *)0x04CC2020,0x91420000L);
writeKernelMemory((void *)0x04CC2024,0x384025BCL);
writeKernelMemory((void *)0x04CC2028,0x7D4C1214L);
writeKernelMemory((void *)0x04CC202C,0x3C400000L);
writeKernelMemory((void *)0x04CC2030,0x60420001L);
writeKernelMemory((void *)0x04CC2034,0x904A0000L);
writeKernelMemory((void *)0x04CC2038,0x384025D0L);
writeKernelMemory((void *)0x04CC203C,0x7D4C1214L);
writeKernelMemory((void *)0x04CC2040,0x3C400000L);
writeKernelMemory((void *)0x04CC2044,0x904A0000L);
writeKernelMemory((void *)0x04CC2048,0x3D401002L);
writeKernelMemory((void *)0x04CC204C,0x814A0068L);
writeKernelMemory((void *)0x04CC2050,0x914C26DCL);
writeKernelMemory((void *)0x04CC2054,0x384026D4L);
writeKernelMemory((void *)0x04CC2058,0x7D4C1214L);
writeKernelMemory((void *)0x04CC205C,0x38400001L);
writeKernelMemory((void *)0x04CC2060,0x904A0000L);
writeKernelMemory((void *)0x04CC2064,0x384026E8L);
writeKernelMemory((void *)0x04CC2068,0x7D4C1214L);
writeKernelMemory((void *)0x04CC206C,0x3C400000L);
writeKernelMemory((void *)0x04CC2070,0x904A0000L);
writeKernelMemory((void *)0x04CC2074,0x3C401003L);
writeKernelMemory((void *)0x04CC2078,0x38425000L);
writeKernelMemory((void *)0x04CC207C,0x3FC010A1L);
writeKernelMemory((void *)0x04CC2080,0x83DE9370L);
writeKernelMemory((void *)0x04CC2084,0x3D400101L);
writeKernelMemory((void *)0x04CC2088,0x394A0101L);
writeKernelMemory((void *)0x04CC208C,0x7C1E5000L);
writeKernelMemory((void *)0x04CC2090,0x4082000CL);
writeKernelMemory((void *)0x04CC2094,0x39400000L);
writeKernelMemory((void *)0x04CC2098,0x91420A28L);
writeKernelMemory((void *)0x04CC209C,0x3FE00382L);
writeKernelMemory((void *)0x04CC20A0,0x63FFC8C0L);
writeKernelMemory((void *)0x04CC20A4,0x48000021L);
writeKernelMemory((void *)0x04CC20A8,0x00A70062L);
writeKernelMemory((void *)0x04CC20AC,0x00530068L);
writeKernelMemory((void *)0x04CC20B0,0x0069006EL);
writeKernelMemory((void *)0x04CC20B4,0x00650043L);
writeKernelMemory((void *)0x04CC20B8,0x006C0069L);
writeKernelMemory((void *)0x04CC20BC,0x0065006EL);
writeKernelMemory((void *)0x04CC20C0,0x00740000L);
writeKernelMemory((void *)0x04CC20C4,0x7CA802A6L);
writeKernelMemory((void *)0x04CC20C8,0x38620260L);
writeKernelMemory((void *)0x04CC20CC,0x38800064L);
writeKernelMemory((void *)0x04CC20D0,0x4CC63242L);
writeKernelMemory((void *)0x04CC20D4,0x7FE903A6L);
writeKernelMemory((void *)0x04CC20D8,0x4E800421L);
writeKernelMemory((void *)0x04CC20DC,0x4800000DL);
writeKernelMemory((void *)0x04CC20E0,0x00A70038L);
writeKernelMemory((void *)0x04CC20E4,0x00200000L);
writeKernelMemory((void *)0x04CC20E8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC20EC,0x38620190L);
writeKernelMemory((void *)0x04CC20F0,0x38800064L);
writeKernelMemory((void *)0x04CC20F4,0x4CC63242L);
writeKernelMemory((void *)0x04CC20F8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC20FC,0x4E800421L);
writeKernelMemory((void *)0x04CC2100,0x4800001DL);
writeKernelMemory((void *)0x04CC2104,0x0045006EL);
writeKernelMemory((void *)0x04CC2108,0x0065006DL);
writeKernelMemory((void *)0x04CC210C,0x00790027L);
writeKernelMemory((void *)0x04CC2110,0x00730020L);
writeKernelMemory((void *)0x04CC2114,0x00480050L);
writeKernelMemory((void *)0x04CC2118,0x003A0000L);
writeKernelMemory((void *)0x04CC211C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2120,0x38620A70L);
writeKernelMemory((void *)0x04CC2124,0x38800064L);
writeKernelMemory((void *)0x04CC2128,0x4CC63242L);
writeKernelMemory((void *)0x04CC212C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2130,0x4E800421L);
writeKernelMemory((void *)0x04CC2134,0x4800000DL);
writeKernelMemory((void *)0x04CC2138,0x00250066L);
writeKernelMemory((void *)0x04CC213C,0x00000000L);
writeKernelMemory((void *)0x04CC2140,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2144,0x38620A90L);
writeKernelMemory((void *)0x04CC2148,0x38800064L);
writeKernelMemory((void *)0x04CC214C,0x4CC63242L);
writeKernelMemory((void *)0x04CC2150,0x819900C8L);
writeKernelMemory((void *)0x04CC2154,0x2C0C0000L);
writeKernelMemory((void *)0x04CC2158,0x41820014L);
writeKernelMemory((void *)0x04CC215C,0x818C01F8L);
writeKernelMemory((void *)0x04CC2160,0x818C0030L);
writeKernelMemory((void *)0x04CC2164,0xC02C000CL);
writeKernelMemory((void *)0x04CC2168,0x48000010L);
writeKernelMemory((void *)0x04CC216C,0x3D800000L);
writeKernelMemory((void *)0x04CC2170,0x91820084L);
writeKernelMemory((void *)0x04CC2174,0xC0220084L);
writeKernelMemory((void *)0x04CC2178,0x7FE903A6L);
writeKernelMemory((void *)0x04CC217C,0x4E800421L);
writeKernelMemory((void *)0x04CC2180,0x48000019L);
writeKernelMemory((void *)0x04CC2184,0x00A70063L);
writeKernelMemory((void *)0x04CC2188,0x00490050L);
writeKernelMemory((void *)0x04CC218C,0x003A0020L);
writeKernelMemory((void *)0x04CC2190,0x00250073L);
writeKernelMemory((void *)0x04CC2194,0x00000000L);
writeKernelMemory((void *)0x04CC2198,0x7CA802A6L);
writeKernelMemory((void *)0x04CC219C,0x38620B80L);
writeKernelMemory((void *)0x04CC21A0,0x38800064L);
writeKernelMemory((void *)0x04CC21A4,0x3CC01023L);
writeKernelMemory((void *)0x04CC21A8,0x38C6459CL);
writeKernelMemory((void *)0x04CC21AC,0x4CC63242L);
writeKernelMemory((void *)0x04CC21B0,0x7FE903A6L);
writeKernelMemory((void *)0x04CC21B4,0x4E800421L);
writeKernelMemory((void *)0x04CC21B8,0x48000031L);
writeKernelMemory((void *)0x04CC21BC,0x00A70063L);
writeKernelMemory((void *)0x04CC21C0,0x00530068L);
writeKernelMemory((void *)0x04CC21C4,0x0069006EL);
writeKernelMemory((void *)0x04CC21C8,0x00650043L);
writeKernelMemory((void *)0x04CC21CC,0x006C0069L);
writeKernelMemory((void *)0x04CC21D0,0x0065006EL);
writeKernelMemory((void *)0x04CC21D4,0x00740020L);
writeKernelMemory((void *)0x04CC21D8,0x00760033L);
writeKernelMemory((void *)0x04CC21DC,0x002E0043L);
writeKernelMemory((void *)0x04CC21E0,0x002E0031L);
writeKernelMemory((void *)0x04CC21E4,0x00330000L);
writeKernelMemory((void *)0x04CC21E8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC21EC,0x38620B50L);
writeKernelMemory((void *)0x04CC21F0,0x38800064L);
writeKernelMemory((void *)0x04CC21F4,0x4CC63242L);
writeKernelMemory((void *)0x04CC21F8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC21FC,0x4E800421L);
writeKernelMemory((void *)0x04CC2200,0x4800002DL);
writeKernelMemory((void *)0x04CC2204,0x00A70063L);
writeKernelMemory((void *)0x04CC2208,0x00430072L);
writeKernelMemory((void *)0x04CC220C,0x00650061L);
writeKernelMemory((void *)0x04CC2210,0x00740065L);
writeKernelMemory((void *)0x04CC2214,0x00640020L);
writeKernelMemory((void *)0x04CC2218,0x00620079L);
writeKernelMemory((void *)0x04CC221C,0x00200043L);
writeKernelMemory((void *)0x04CC2220,0x006C006FL);
writeKernelMemory((void *)0x04CC2224,0x00760065L);
writeKernelMemory((void *)0x04CC2228,0x00720000L);
writeKernelMemory((void *)0x04CC222C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2230,0x38800064L);
writeKernelMemory((void *)0x04CC2234,0x38620AC0L);
writeKernelMemory((void *)0x04CC2238,0x4CC63242L);
writeKernelMemory((void *)0x04CC223C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2240,0x4E800421L);
writeKernelMemory((void *)0x04CC2244,0x48000029L);
writeKernelMemory((void *)0x04CC2248,0x00A70066L);
writeKernelMemory((void *)0x04CC224C,0x003E00A7L);
writeKernelMemory((void *)0x04CC2250,0x00660047L);
writeKernelMemory((void *)0x04CC2254,0x00750061L);
writeKernelMemory((void *)0x04CC2258,0x00720064L);
writeKernelMemory((void *)0x04CC225C,0x00410074L);
writeKernelMemory((void *)0x04CC2260,0x00740061L);
writeKernelMemory((void *)0x04CC2264,0x0063006BL);
writeKernelMemory((void *)0x04CC2268,0x00000000L);
writeKernelMemory((void *)0x04CC226C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2270,0x38800064L);
writeKernelMemory((void *)0x04CC2274,0x386201A0L);
writeKernelMemory((void *)0x04CC2278,0x4CC63242L);
writeKernelMemory((void *)0x04CC227C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2280,0x4E800421L);
writeKernelMemory((void *)0x04CC2284,0x48000025L);
writeKernelMemory((void *)0x04CC2288,0x00A70066L);
writeKernelMemory((void *)0x04CC228C,0x003E00A7L);
writeKernelMemory((void *)0x04CC2290,0x00660041L);
writeKernelMemory((void *)0x04CC2294,0x00750074L);
writeKernelMemory((void *)0x04CC2298,0x006F0053L);
writeKernelMemory((void *)0x04CC229C,0x0077006FL);
writeKernelMemory((void *)0x04CC22A0,0x00720064L);
writeKernelMemory((void *)0x04CC22A4,0x00000000L);
writeKernelMemory((void *)0x04CC22A8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC22AC,0x38800064L);
writeKernelMemory((void *)0x04CC22B0,0x386201D0L);
writeKernelMemory((void *)0x04CC22B4,0x4CC63242L);
writeKernelMemory((void *)0x04CC22B8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC22BC,0x4E800421L);
writeKernelMemory((void *)0x04CC22C0,0x48000021L);
writeKernelMemory((void *)0x04CC22C4,0x00A70066L);
writeKernelMemory((void *)0x04CC22C8,0x003E00A7L);
writeKernelMemory((void *)0x04CC22CC,0x00660041L);
writeKernelMemory((void *)0x04CC22D0,0x00750074L);
writeKernelMemory((void *)0x04CC22D4,0x006F0041L);
writeKernelMemory((void *)0x04CC22D8,0x0069006DL);
writeKernelMemory((void *)0x04CC22DC,0x00000000L);
writeKernelMemory((void *)0x04CC22E0,0x7CA802A6L);
writeKernelMemory((void *)0x04CC22E4,0x38800064L);
writeKernelMemory((void *)0x04CC22E8,0x38620230L);
writeKernelMemory((void *)0x04CC22EC,0x4CC63242L);
writeKernelMemory((void *)0x04CC22F0,0x7FE903A6L);
writeKernelMemory((void *)0x04CC22F4,0x4E800421L);
writeKernelMemory((void *)0x04CC22F8,0x48000021L);
writeKernelMemory((void *)0x04CC22FC,0x00A70066L);
writeKernelMemory((void *)0x04CC2300,0x003E00A7L);
writeKernelMemory((void *)0x04CC2304,0x00660041L);
writeKernelMemory((void *)0x04CC2308,0x00750074L);
writeKernelMemory((void *)0x04CC230C,0x006F0042L);
writeKernelMemory((void *)0x04CC2310,0x006F0077L);
writeKernelMemory((void *)0x04CC2314,0x00000000L);
writeKernelMemory((void *)0x04CC2318,0x7CA802A6L);
writeKernelMemory((void *)0x04CC231C,0x38800064L);
writeKernelMemory((void *)0x04CC2320,0x38620280L);
writeKernelMemory((void *)0x04CC2324,0x4CC63242L);
writeKernelMemory((void *)0x04CC2328,0x7FE903A6L);
writeKernelMemory((void *)0x04CC232C,0x4E800421L);
writeKernelMemory((void *)0x04CC2330,0x48000021L);
writeKernelMemory((void *)0x04CC2334,0x00A70066L);
writeKernelMemory((void *)0x04CC2338,0x003E00A7L);
writeKernelMemory((void *)0x04CC233C,0x0066004BL);
writeKernelMemory((void *)0x04CC2340,0x0069006CL);
writeKernelMemory((void *)0x04CC2344,0x006C0041L);
writeKernelMemory((void *)0x04CC2348,0x00750072L);
writeKernelMemory((void *)0x04CC234C,0x00610000L);
writeKernelMemory((void *)0x04CC2350,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2354,0x38800064L);
writeKernelMemory((void *)0x04CC2358,0x386202A0L);
writeKernelMemory((void *)0x04CC235C,0x4CC63242L);
writeKernelMemory((void *)0x04CC2360,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2364,0x4E800421L);
writeKernelMemory((void *)0x04CC2368,0x4800001DL);
writeKernelMemory((void *)0x04CC236C,0x00A70066L);
writeKernelMemory((void *)0x04CC2370,0x003E00A7L);
writeKernelMemory((void *)0x04CC2374,0x00660054L);
writeKernelMemory((void *)0x04CC2378,0x00500041L);
writeKernelMemory((void *)0x04CC237C,0x00750072L);
writeKernelMemory((void *)0x04CC2380,0x00610000L);
writeKernelMemory((void *)0x04CC2384,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2388,0x38800064L);
writeKernelMemory((void *)0x04CC238C,0x386202C0L);
writeKernelMemory((void *)0x04CC2390,0x4CC63242L);
writeKernelMemory((void *)0x04CC2394,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2398,0x4E800421L);
writeKernelMemory((void *)0x04CC239C,0x48000021L);
writeKernelMemory((void *)0x04CC23A0,0x00A70066L);
writeKernelMemory((void *)0x04CC23A4,0x003E00A7L);
writeKernelMemory((void *)0x04CC23A8,0x0066004DL);
writeKernelMemory((void *)0x04CC23AC,0x0075006CL);
writeKernelMemory((void *)0x04CC23B0,0x00740069L);
writeKernelMemory((void *)0x04CC23B4,0x0042006FL);
writeKernelMemory((void *)0x04CC23B8,0x00770000L);
writeKernelMemory((void *)0x04CC23BC,0x7CA802A6L);
writeKernelMemory((void *)0x04CC23C0,0x38800064L);
writeKernelMemory((void *)0x04CC23C4,0x386202E0L);
writeKernelMemory((void *)0x04CC23C8,0x4CC63242L);
writeKernelMemory((void *)0x04CC23CC,0x7FE903A6L);
writeKernelMemory((void *)0x04CC23D0,0x4E800421L);
writeKernelMemory((void *)0x04CC23D4,0x48000025L);
writeKernelMemory((void *)0x04CC23D8,0x00A70066L);
writeKernelMemory((void *)0x04CC23DC,0x003E00A7L);
writeKernelMemory((void *)0x04CC23E0,0x0066004EL);
writeKernelMemory((void *)0x04CC23E4,0x006F0053L);
writeKernelMemory((void *)0x04CC23E8,0x006C006FL);
writeKernelMemory((void *)0x04CC23EC,0x00770064L);
writeKernelMemory((void *)0x04CC23F0,0x006F0077L);
writeKernelMemory((void *)0x04CC23F4,0x006E0000L);
writeKernelMemory((void *)0x04CC23F8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC23FC,0x38800064L);
writeKernelMemory((void *)0x04CC2400,0x38620370L);
writeKernelMemory((void *)0x04CC2404,0x4CC63242L);
writeKernelMemory((void *)0x04CC2408,0x7FE903A6L);
writeKernelMemory((void *)0x04CC240C,0x4E800421L);
writeKernelMemory((void *)0x04CC2410,0x4800001DL);
writeKernelMemory((void *)0x04CC2414,0x00250064L);
writeKernelMemory((void *)0x04CC2418,0x002C0020L);
writeKernelMemory((void *)0x04CC241C,0x00250064L);
writeKernelMemory((void *)0x04CC2420,0x002C0020L);
writeKernelMemory((void *)0x04CC2424,0x00250064L);
writeKernelMemory((void *)0x04CC2428,0x00000000L);
writeKernelMemory((void *)0x04CC242C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2430,0x38800064L);
writeKernelMemory((void *)0x04CC2434,0x81990034L);
writeKernelMemory((void *)0x04CC2438,0x2C0C0000L);
writeKernelMemory((void *)0x04CC243C,0x41820024L);
writeKernelMemory((void *)0x04CC2440,0x80CC0528L);
writeKernelMemory((void *)0x04CC2444,0x80EC052CL);
writeKernelMemory((void *)0x04CC2448,0x38E70001L);
writeKernelMemory((void *)0x04CC244C,0x810C0530L);
writeKernelMemory((void *)0x04CC2450,0x38620B00L);
writeKernelMemory((void *)0x04CC2454,0x4CC63242L);
writeKernelMemory((void *)0x04CC2458,0x7FE903A6L);
writeKernelMemory((void *)0x04CC245C,0x4E800421L);
writeKernelMemory((void *)0x04CC2460,0x4800002DL);
writeKernelMemory((void *)0x04CC2464,0x00A70038L);
writeKernelMemory((void *)0x04CC2468,0x005A004CL);
writeKernelMemory((void *)0x04CC246C,0x0020002BL);
writeKernelMemory((void *)0x04CC2470,0x00200044L);
writeKernelMemory((void *)0x04CC2474,0x006F0077L);
writeKernelMemory((void *)0x04CC2478,0x006E0020L);
writeKernelMemory((void *)0x04CC247C,0x0074006FL);
writeKernelMemory((void *)0x04CC2480,0x0020006FL);
writeKernelMemory((void *)0x04CC2484,0x00700065L);
writeKernelMemory((void *)0x04CC2488,0x006E0000L);
writeKernelMemory((void *)0x04CC248C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2490,0x38800064L);
writeKernelMemory((void *)0x04CC2494,0x38620CA0L);
writeKernelMemory((void *)0x04CC2498,0x4CC63242L);
writeKernelMemory((void *)0x04CC249C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC24A0,0x4E800421L);
writeKernelMemory((void *)0x04CC24A4,0x48000015L);
writeKernelMemory((void *)0x04CC24A8,0x00550073L);
writeKernelMemory((void *)0x04CC24AC,0x00650066L);
writeKernelMemory((void *)0x04CC24B0,0x0075006CL);
writeKernelMemory((void *)0x04CC24B4,0x00000000L);
writeKernelMemory((void *)0x04CC24B8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC24BC,0x38800064L);
writeKernelMemory((void *)0x04CC24C0,0x38620D60L);
writeKernelMemory((void *)0x04CC24C4,0x4CC63242L);
writeKernelMemory((void *)0x04CC24C8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC24CC,0x4E800421L);
writeKernelMemory((void *)0x04CC24D0,0x48000029L);
writeKernelMemory((void *)0x04CC24D4,0x00A70066L);
writeKernelMemory((void *)0x04CC24D8,0x003E00A7L);
writeKernelMemory((void *)0x04CC24DC,0x00660049L);
writeKernelMemory((void *)0x04CC24E0,0x006E0073L);
writeKernelMemory((void *)0x04CC24E4,0x00740061L);
writeKernelMemory((void *)0x04CC24E8,0x006E0074L);
writeKernelMemory((void *)0x04CC24EC,0x00430072L);
writeKernelMemory((void *)0x04CC24F0,0x00610066L);
writeKernelMemory((void *)0x04CC24F4,0x00740000L);
writeKernelMemory((void *)0x04CC24F8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC24FC,0x38800064L);
writeKernelMemory((void *)0x04CC2500,0x38620C00L);
writeKernelMemory((void *)0x04CC2504,0x4CC63242L);
writeKernelMemory((void *)0x04CC2508,0x7FE903A6L);
writeKernelMemory((void *)0x04CC250C,0x4E800421L);
writeKernelMemory((void *)0x04CC2510,0x48000029L);
writeKernelMemory((void *)0x04CC2514,0x00A70066L);
writeKernelMemory((void *)0x04CC2518,0x003E00A7L);
writeKernelMemory((void *)0x04CC251C,0x00660033L);
writeKernelMemory((void *)0x04CC2520,0x00780033L);
writeKernelMemory((void *)0x04CC2524,0x00430072L);
writeKernelMemory((void *)0x04CC2528,0x00610066L);
writeKernelMemory((void *)0x04CC252C,0x00740069L);
writeKernelMemory((void *)0x04CC2530,0x006E0067L);
writeKernelMemory((void *)0x04CC2534,0x00000000L);
writeKernelMemory((void *)0x04CC2538,0x7CA802A6L);
writeKernelMemory((void *)0x04CC253C,0x38800064L);
writeKernelMemory((void *)0x04CC2540,0x38620CE0L);
writeKernelMemory((void *)0x04CC2544,0x4CC63242L);
writeKernelMemory((void *)0x04CC2548,0x7FE903A6L);
writeKernelMemory((void *)0x04CC254C,0x4E800421L);
writeKernelMemory((void *)0x04CC2550,0x4800002DL);
writeKernelMemory((void *)0x04CC2554,0x00A70066L);
writeKernelMemory((void *)0x04CC2558,0x003E00A7L);
writeKernelMemory((void *)0x04CC255C,0x0066004DL);
writeKernelMemory((void *)0x04CC2560,0x00750074L);
writeKernelMemory((void *)0x04CC2564,0x00650056L);
writeKernelMemory((void *)0x04CC2568,0x006F0069L);
writeKernelMemory((void *)0x04CC256C,0x00630065L);
writeKernelMemory((void *)0x04CC2570,0x00430068L);
writeKernelMemory((void *)0x04CC2574,0x00610074L);
writeKernelMemory((void *)0x04CC2578,0x00000000L);
writeKernelMemory((void *)0x04CC257C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2580,0x38800064L);
writeKernelMemory((void *)0x04CC2584,0x38620D20L);
writeKernelMemory((void *)0x04CC2588,0x4CC63242L);
writeKernelMemory((void *)0x04CC258C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2590,0x4E800421L);
writeKernelMemory((void *)0x04CC2594,0x4800001DL);
writeKernelMemory((void *)0x04CC2598,0x00A70066L);
writeKernelMemory((void *)0x04CC259C,0x003E00A7L);
writeKernelMemory((void *)0x04CC25A0,0x00660043L);
writeKernelMemory((void *)0x04CC25A4,0x0061006EL);
writeKernelMemory((void *)0x04CC25A8,0x0046006CL);
writeKernelMemory((void *)0x04CC25AC,0x00790000L);
writeKernelMemory((void *)0x04CC25B0,0x7CA802A6L);
writeKernelMemory((void *)0x04CC25B4,0x38800064L);
writeKernelMemory((void *)0x04CC25B8,0x38620D90L);
writeKernelMemory((void *)0x04CC25BC,0x4CC63242L);
writeKernelMemory((void *)0x04CC25C0,0x7FE903A6L);
writeKernelMemory((void *)0x04CC25C4,0x4E800421L);
writeKernelMemory((void *)0x04CC25C8,0x48000029L);
writeKernelMemory((void *)0x04CC25CC,0x00A70066L);
writeKernelMemory((void *)0x04CC25D0,0x003E00A7L);
writeKernelMemory((void *)0x04CC25D4,0x0066004EL);
writeKernelMemory((void *)0x04CC25D8,0x006F0046L);
writeKernelMemory((void *)0x04CC25DC,0x0061006CL);
writeKernelMemory((void *)0x04CC25E0,0x006C0044L);
writeKernelMemory((void *)0x04CC25E4,0x0061006DL);
writeKernelMemory((void *)0x04CC25E8,0x00610067L);
writeKernelMemory((void *)0x04CC25EC,0x00650000L);
writeKernelMemory((void *)0x04CC25F0,0x7CA802A6L);
writeKernelMemory((void *)0x04CC25F4,0x38800064L);
writeKernelMemory((void *)0x04CC25F8,0x38620E00L);
writeKernelMemory((void *)0x04CC25FC,0x4CC63242L);
writeKernelMemory((void *)0x04CC2600,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2604,0x4E800421L);
writeKernelMemory((void *)0x04CC2608,0x48000029L);
writeKernelMemory((void *)0x04CC260C,0x00A70066L);
writeKernelMemory((void *)0x04CC2610,0x003E00A7L);
writeKernelMemory((void *)0x04CC2614,0x0066004EL);
writeKernelMemory((void *)0x04CC2618,0x006F0043L);
writeKernelMemory((void *)0x04CC261C,0x00720061L);
writeKernelMemory((void *)0x04CC2620,0x00730068L);
writeKernelMemory((void *)0x04CC2624,0x00410072L);
writeKernelMemory((void *)0x04CC2628,0x0072006FL);
writeKernelMemory((void *)0x04CC262C,0x00770000L);
writeKernelMemory((void *)0x04CC2630,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2634,0x38800064L);
writeKernelMemory((void *)0x04CC2638,0x38620E30L);
writeKernelMemory((void *)0x04CC263C,0x4CC63242L);
writeKernelMemory((void *)0x04CC2640,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2644,0x4E800421L);
writeKernelMemory((void *)0x04CC2648,0x4800001DL);
writeKernelMemory((void *)0x04CC264C,0x00A70066L);
writeKernelMemory((void *)0x04CC2650,0x003E00A7L);
writeKernelMemory((void *)0x04CC2654,0x00660046L);
writeKernelMemory((void *)0x04CC2658,0x00610073L);
writeKernelMemory((void *)0x04CC265C,0x0074005AL);
writeKernelMemory((void *)0x04CC2660,0x00520000L);
writeKernelMemory((void *)0x04CC2664,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2668,0x38800064L);
writeKernelMemory((void *)0x04CC266C,0x38620E60L);
writeKernelMemory((void *)0x04CC2670,0x4CC63242L);
writeKernelMemory((void *)0x04CC2674,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2678,0x4E800421L);
writeKernelMemory((void *)0x04CC267C,0x4800001DL);
writeKernelMemory((void *)0x04CC2680,0x00A70066L);
writeKernelMemory((void *)0x04CC2684,0x003E00A7L);
writeKernelMemory((void *)0x04CC2688,0x00660046L);
writeKernelMemory((void *)0x04CC268C,0x00610073L);
writeKernelMemory((void *)0x04CC2690,0x0074005AL);
writeKernelMemory((void *)0x04CC2694,0x004C0000L);
writeKernelMemory((void *)0x04CC2698,0x7CA802A6L);
writeKernelMemory((void *)0x04CC269C,0x38800064L);
writeKernelMemory((void *)0x04CC26A0,0x38620E80L);
writeKernelMemory((void *)0x04CC26A4,0x4CC63242L);
writeKernelMemory((void *)0x04CC26A8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC26AC,0x4E800421L);
writeKernelMemory((void *)0x04CC26B0,0x48000029L);
writeKernelMemory((void *)0x04CC26B4,0x00A70066L);
writeKernelMemory((void *)0x04CC26B8,0x003E00A7L);
writeKernelMemory((void *)0x04CC26BC,0x00660043L);
writeKernelMemory((void *)0x04CC26C0,0x0061006EL);
writeKernelMemory((void *)0x04CC26C4,0x00420072L);
writeKernelMemory((void *)0x04CC26C8,0x00650061L);
writeKernelMemory((void *)0x04CC26CC,0x006B0041L);
writeKernelMemory((void *)0x04CC26D0,0x006C006CL);
writeKernelMemory((void *)0x04CC26D4,0x00000000L);
writeKernelMemory((void *)0x04CC26D8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC26DC,0x38800064L);
writeKernelMemory((void *)0x04CC26E0,0x38620EB0L);
writeKernelMemory((void *)0x04CC26E4,0x4CC63242L);
writeKernelMemory((void *)0x04CC26E8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC26EC,0x4E800421L);
writeKernelMemory((void *)0x04CC26F0,0x48000021L);
writeKernelMemory((void *)0x04CC26F4,0x00A70066L);
writeKernelMemory((void *)0x04CC26F8,0x003E00A7L);
writeKernelMemory((void *)0x04CC26FC,0x00660055L);
writeKernelMemory((void *)0x04CC2700,0x00500053L);
writeKernelMemory((void *)0x04CC2704,0x00700065L);
writeKernelMemory((void *)0x04CC2708,0x00650064L);
writeKernelMemory((void *)0x04CC270C,0x00000000L);
writeKernelMemory((void *)0x04CC2710,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2714,0x38800064L);
writeKernelMemory((void *)0x04CC2718,0x60430EF0L);
writeKernelMemory((void *)0x04CC271C,0x4CC63242L);
writeKernelMemory((void *)0x04CC2720,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2724,0x4E800421L);
writeKernelMemory((void *)0x04CC2728,0x48000021L);
writeKernelMemory((void *)0x04CC272C,0x00A70066L);
writeKernelMemory((void *)0x04CC2730,0x003E00A7L);
writeKernelMemory((void *)0x04CC2734,0x00660053L);
writeKernelMemory((void *)0x04CC2738,0x00630061L);
writeKernelMemory((void *)0x04CC273C,0x00660066L);
writeKernelMemory((void *)0x04CC2740,0x006F006CL);
writeKernelMemory((void *)0x04CC2744,0x00640000L);
writeKernelMemory((void *)0x04CC2748,0x7CA802A6L);
writeKernelMemory((void *)0x04CC274C,0x38800064L);
writeKernelMemory((void *)0x04CC2750,0x60430F10L);
writeKernelMemory((void *)0x04CC2754,0x4CC63242L);
writeKernelMemory((void *)0x04CC2758,0x7FE903A6L);
writeKernelMemory((void *)0x04CC275C,0x4E800421L);
writeKernelMemory((void *)0x04CC2760,0x4800001DL);
writeKernelMemory((void *)0x04CC2764,0x00A70066L);
writeKernelMemory((void *)0x04CC2768,0x003E00A7L);
writeKernelMemory((void *)0x04CC276C,0x0066004EL);
writeKernelMemory((void *)0x04CC2770,0x0075006BL);
writeKernelMemory((void *)0x04CC2774,0x00650072L);
writeKernelMemory((void *)0x04CC2778,0x00000000L);
writeKernelMemory((void *)0x04CC277C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2780,0x38800064L);
writeKernelMemory((void *)0x04CC2784,0x60430F40L);
writeKernelMemory((void *)0x04CC2788,0x4CC63242L);
writeKernelMemory((void *)0x04CC278C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2790,0x4E800421L);
writeKernelMemory((void *)0x04CC2794,0x48000021L);
writeKernelMemory((void *)0x04CC2798,0x00A70066L);
writeKernelMemory((void *)0x04CC279C,0x003E00A7L);
writeKernelMemory((void *)0x04CC27A0,0x00660042L);
writeKernelMemory((void *)0x04CC27A4,0x00750069L);
writeKernelMemory((void *)0x04CC27A8,0x006C0064L);
writeKernelMemory((void *)0x04CC27AC,0x00650072L);
writeKernelMemory((void *)0x04CC27B0,0x00000000L);
writeKernelMemory((void *)0x04CC27B4,0x7CA802A6L);
writeKernelMemory((void *)0x04CC27B8,0x38800064L);
writeKernelMemory((void *)0x04CC27BC,0x60430F70L);
writeKernelMemory((void *)0x04CC27C0,0x4CC63242L);
writeKernelMemory((void *)0x04CC27C4,0x7FE903A6L);
writeKernelMemory((void *)0x04CC27C8,0x4E800421L);
writeKernelMemory((void *)0x04CC27CC,0x48000011L);
writeKernelMemory((void *)0x04CC27D0,0x00490074L);
writeKernelMemory((void *)0x04CC27D4,0x0065006DL);
writeKernelMemory((void *)0x04CC27D8,0x00000000L);
writeKernelMemory((void *)0x04CC27DC,0x7CA802A6L);
writeKernelMemory((void *)0x04CC27E0,0x38800064L);
writeKernelMemory((void *)0x04CC27E4,0x38620F90L);
writeKernelMemory((void *)0x04CC27E8,0x4CC63242L);
writeKernelMemory((void *)0x04CC27EC,0x7FE903A6L);
writeKernelMemory((void *)0x04CC27F0,0x4E800421L);
writeKernelMemory((void *)0x04CC27F4,0x48000029L);
writeKernelMemory((void *)0x04CC27F8,0x00A70066L);
writeKernelMemory((void *)0x04CC27FC,0x003E00A7L);
writeKernelMemory((void *)0x04CC2800,0x00660047L);
writeKernelMemory((void *)0x04CC2804,0x00650074L);
writeKernelMemory((void *)0x04CC2808,0x00480061L);
writeKernelMemory((void *)0x04CC280C,0x0063006BL);
writeKernelMemory((void *)0x04CC2810,0x00490074L);
writeKernelMemory((void *)0x04CC2814,0x0065006DL);
writeKernelMemory((void *)0x04CC2818,0x00730000L);
writeKernelMemory((void *)0x04CC281C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2820,0x38800064L);
writeKernelMemory((void *)0x04CC2824,0x38620FA0L);
writeKernelMemory((void *)0x04CC2828,0x4CC63242L);
writeKernelMemory((void *)0x04CC282C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2830,0x4E800421L);
writeKernelMemory((void *)0x04CC2834,0x4800002DL);
writeKernelMemory((void *)0x04CC2838,0x00A70066L);
writeKernelMemory((void *)0x04CC283C,0x003E00A7L);
writeKernelMemory((void *)0x04CC2840,0x00660047L);
writeKernelMemory((void *)0x04CC2844,0x00650074L);
writeKernelMemory((void *)0x04CC2848,0x00480061L);
writeKernelMemory((void *)0x04CC284C,0x0063006BL);
writeKernelMemory((void *)0x04CC2850,0x00490074L);
writeKernelMemory((void *)0x04CC2854,0x0065006DL);
writeKernelMemory((void *)0x04CC2858,0x00730032L);
writeKernelMemory((void *)0x04CC285C,0x00000000L);
writeKernelMemory((void *)0x04CC2860,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2864,0x38800064L);
writeKernelMemory((void *)0x04CC2868,0x38620FD0L);
writeKernelMemory((void *)0x04CC286C,0x4CC63242L);
writeKernelMemory((void *)0x04CC2870,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2874,0x4E800421L);
writeKernelMemory((void *)0x04CC2878,0x48000021L);
writeKernelMemory((void *)0x04CC287C,0x00A70066L);
writeKernelMemory((void *)0x04CC2880,0x003E00A7L);
writeKernelMemory((void *)0x04CC2884,0x0066004DL);
writeKernelMemory((void *)0x04CC2888,0x006F0064L);
writeKernelMemory((void *)0x04CC288C,0x004D0065L);
writeKernelMemory((void *)0x04CC2890,0x006E0075L);
writeKernelMemory((void *)0x04CC2894,0x00000000L);
writeKernelMemory((void *)0x04CC2898,0x7CA802A6L);
writeKernelMemory((void *)0x04CC289C,0x38800064L);
writeKernelMemory((void *)0x04CC28A0,0x3862FF10L);
writeKernelMemory((void *)0x04CC28A4,0x4CC63242L);
writeKernelMemory((void *)0x04CC28A8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC28AC,0x4E800421L);
writeKernelMemory((void *)0x04CC28B0,0x48000029L);
writeKernelMemory((void *)0x04CC28B4,0x00A70066L);
writeKernelMemory((void *)0x04CC28B8,0x003E00A7L);
writeKernelMemory((void *)0x04CC28BC,0x00660052L);
writeKernelMemory((void *)0x04CC28C0,0x00610069L);
writeKernelMemory((void *)0x04CC28C4,0x006E0062L);
writeKernelMemory((void *)0x04CC28C8,0x006F0077L);
writeKernelMemory((void *)0x04CC28CC,0x00410072L);
writeKernelMemory((void *)0x04CC28D0,0x006D006FL);
writeKernelMemory((void *)0x04CC28D4,0x00720000L);
writeKernelMemory((void *)0x04CC28D8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC28DC,0x38800064L);
writeKernelMemory((void *)0x04CC28E0,0x3862FF60L);
writeKernelMemory((void *)0x04CC28E4,0x4CC63242L);
writeKernelMemory((void *)0x04CC28E8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC28EC,0x4E800421L);
writeKernelMemory((void *)0x04CC28F0,0x48000015L);
writeKernelMemory((void *)0x04CC28F4,0x0043006FL);
writeKernelMemory((void *)0x04CC28F8,0x006D006DL);
writeKernelMemory((void *)0x04CC28FC,0x0061006EL);
writeKernelMemory((void *)0x04CC2900,0x00640000L);
writeKernelMemory((void *)0x04CC2904,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2908,0x38800064L);
writeKernelMemory((void *)0x04CC290C,0x38621000L);
writeKernelMemory((void *)0x04CC2910,0x4CC63242L);
writeKernelMemory((void *)0x04CC2914,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2918,0x4E800421L);
writeKernelMemory((void *)0x04CC291C,0x48000021L);
writeKernelMemory((void *)0x04CC2920,0x00A70066L);
writeKernelMemory((void *)0x04CC2924,0x003E00A7L);
writeKernelMemory((void *)0x04CC2928,0x00660047L);
writeKernelMemory((void *)0x04CC292C,0x00690076L);
writeKernelMemory((void *)0x04CC2930,0x00650049L);
writeKernelMemory((void *)0x04CC2934,0x00740065L);
writeKernelMemory((void *)0x04CC2938,0x006D0000L);
writeKernelMemory((void *)0x04CC293C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2940,0x38800064L);
writeKernelMemory((void *)0x04CC2944,0x38621010L);
writeKernelMemory((void *)0x04CC2948,0x4CC63242L);
writeKernelMemory((void *)0x04CC294C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2950,0x4E800421L);
writeKernelMemory((void *)0x04CC2954,0x48000029L);
writeKernelMemory((void *)0x04CC2958,0x00A70066L);
writeKernelMemory((void *)0x04CC295C,0x003E00A7L);
writeKernelMemory((void *)0x04CC2960,0x00660045L);
writeKernelMemory((void *)0x04CC2964,0x006E0063L);
writeKernelMemory((void *)0x04CC2968,0x00680061L);
writeKernelMemory((void *)0x04CC296C,0x006E0074L);
writeKernelMemory((void *)0x04CC2970,0x00490074L);
writeKernelMemory((void *)0x04CC2974,0x0065006DL);
writeKernelMemory((void *)0x04CC2978,0x00000000L);
writeKernelMemory((void *)0x04CC297C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2980,0x38800064L);
writeKernelMemory((void *)0x04CC2984,0x38620060L);
writeKernelMemory((void *)0x04CC2988,0x4CC63242L);
writeKernelMemory((void *)0x04CC298C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2990,0x4E800421L);
writeKernelMemory((void *)0x04CC2994,0x48000015L);
writeKernelMemory((void *)0x04CC2998,0x003E0049L);
writeKernelMemory((void *)0x04CC299C,0x0064003AL);
writeKernelMemory((void *)0x04CC29A0,0x00200025L);
writeKernelMemory((void *)0x04CC29A4,0x00640000L);
writeKernelMemory((void *)0x04CC29A8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC29AC,0x38800064L);
writeKernelMemory((void *)0x04CC29B0,0x80C21100L);
writeKernelMemory((void *)0x04CC29B4,0x38621120L);
writeKernelMemory((void *)0x04CC29B8,0x4CC63242L);
writeKernelMemory((void *)0x04CC29BC,0x7FE903A6L);
writeKernelMemory((void *)0x04CC29C0,0x4E800421L);
writeKernelMemory((void *)0x04CC29C4,0x4800001DL);
writeKernelMemory((void *)0x04CC29C8,0x003E004CL);
writeKernelMemory((void *)0x04CC29CC,0x00650076L);
writeKernelMemory((void *)0x04CC29D0,0x0065006CL);
writeKernelMemory((void *)0x04CC29D4,0x003A0020L);
writeKernelMemory((void *)0x04CC29D8,0x00250064L);
writeKernelMemory((void *)0x04CC29DC,0x00000000L);
writeKernelMemory((void *)0x04CC29E0,0x7CA802A6L);
writeKernelMemory((void *)0x04CC29E4,0x38800064L);
writeKernelMemory((void *)0x04CC29E8,0x80C21104L);
writeKernelMemory((void *)0x04CC29EC,0x38620090L);
writeKernelMemory((void *)0x04CC29F0,0x4CC63242L);
writeKernelMemory((void *)0x04CC29F4,0x7FE903A6L);
writeKernelMemory((void *)0x04CC29F8,0x4E800421L);
writeKernelMemory((void *)0x04CC29FC,0x48000015L);
writeKernelMemory((void *)0x04CC2A00,0x003E0049L);
writeKernelMemory((void *)0x04CC2A04,0x0064003AL);
writeKernelMemory((void *)0x04CC2A08,0x00200025L);
writeKernelMemory((void *)0x04CC2A0C,0x00640000L);
writeKernelMemory((void *)0x04CC2A10,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2A14,0x38800064L);
writeKernelMemory((void *)0x04CC2A18,0x80C21030L);
writeKernelMemory((void *)0x04CC2A1C,0x38621040L);
writeKernelMemory((void *)0x04CC2A20,0x4CC63242L);
writeKernelMemory((void *)0x04CC2A24,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2A28,0x4E800421L);
writeKernelMemory((void *)0x04CC2A2C,0x4800001DL);
writeKernelMemory((void *)0x04CC2A30,0x003E0044L);
writeKernelMemory((void *)0x04CC2A34,0x0061006DL);
writeKernelMemory((void *)0x04CC2A38,0x00610067L);
writeKernelMemory((void *)0x04CC2A3C,0x0065003AL);
writeKernelMemory((void *)0x04CC2A40,0x00200025L);
writeKernelMemory((void *)0x04CC2A44,0x00640000L);
writeKernelMemory((void *)0x04CC2A48,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2A4C,0x38800064L);
writeKernelMemory((void *)0x04CC2A50,0x80C21034L);
writeKernelMemory((void *)0x04CC2A54,0x38621070L);
writeKernelMemory((void *)0x04CC2A58,0x4CC63242L);
writeKernelMemory((void *)0x04CC2A5C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2A60,0x4E800421L);
writeKernelMemory((void *)0x04CC2A64,0x4800001DL);
writeKernelMemory((void *)0x04CC2A68,0x003E0043L);
writeKernelMemory((void *)0x04CC2A6C,0x006F0075L);
writeKernelMemory((void *)0x04CC2A70,0x006E0074L);
writeKernelMemory((void *)0x04CC2A74,0x003A0020L);
writeKernelMemory((void *)0x04CC2A78,0x00250064L);
writeKernelMemory((void *)0x04CC2A7C,0x00000000L);
writeKernelMemory((void *)0x04CC2A80,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2A84,0x38800064L);
writeKernelMemory((void *)0x04CC2A88,0x80C21038L);
writeKernelMemory((void *)0x04CC2A8C,0x386210B0L);
writeKernelMemory((void *)0x04CC2A90,0x4CC63242L);
writeKernelMemory((void *)0x04CC2A94,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2A98,0x4E800421L);
writeKernelMemory((void *)0x04CC2A9C,0x4800001DL);
writeKernelMemory((void *)0x04CC2AA0,0x003E0050L);
writeKernelMemory((void *)0x04CC2AA4,0x006C0061L);
writeKernelMemory((void *)0x04CC2AA8,0x00790065L);
writeKernelMemory((void *)0x04CC2AAC,0x0072003AL);
writeKernelMemory((void *)0x04CC2AB0,0x00200025L);
writeKernelMemory((void *)0x04CC2AB4,0x00640000L);
writeKernelMemory((void *)0x04CC2AB8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2ABC,0x38800064L);
writeKernelMemory((void *)0x04CC2AC0,0x80C2103CL);
writeKernelMemory((void *)0x04CC2AC4,0x38620040L);
writeKernelMemory((void *)0x04CC2AC8,0x4CC63242L);
writeKernelMemory((void *)0x04CC2ACC,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2AD0,0x4E800421L);
writeKernelMemory((void *)0x04CC2AD4,0x48000011L);
writeKernelMemory((void *)0x04CC2AD8,0x003E0053L);
writeKernelMemory((void *)0x04CC2ADC,0x0065006EL);
writeKernelMemory((void *)0x04CC2AE0,0x00640000L);
writeKernelMemory((void *)0x04CC2AE4,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2AE8,0x38800064L);
writeKernelMemory((void *)0x04CC2AEC,0x386210F0L);
writeKernelMemory((void *)0x04CC2AF0,0x4CC63242L);
writeKernelMemory((void *)0x04CC2AF4,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2AF8,0x4E800421L);
writeKernelMemory((void *)0x04CC2AFC,0x4800002DL);
writeKernelMemory((void *)0x04CC2B00,0x00A70066L);
writeKernelMemory((void *)0x04CC2B04,0x003E00A7L);
writeKernelMemory((void *)0x04CC2B08,0x00660054L);
writeKernelMemory((void *)0x04CC2B0C,0x006F0067L);
writeKernelMemory((void *)0x04CC2B10,0x0067006CL);
writeKernelMemory((void *)0x04CC2B14,0x00650044L);
writeKernelMemory((void *)0x04CC2B18,0x006F0077L);
writeKernelMemory((void *)0x04CC2B1C,0x006E0066L);
writeKernelMemory((void *)0x04CC2B20,0x0061006CL);
writeKernelMemory((void *)0x04CC2B24,0x006C0000L);
writeKernelMemory((void *)0x04CC2B28,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2B2C,0x38800064L);
writeKernelMemory((void *)0x04CC2B30,0x386200B0L);
writeKernelMemory((void *)0x04CC2B34,0x4CC63242L);
writeKernelMemory((void *)0x04CC2B38,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2B3C,0x4E800421L);
writeKernelMemory((void *)0x04CC2B40,0x4800001DL);
writeKernelMemory((void *)0x04CC2B44,0x00A70066L);
writeKernelMemory((void *)0x04CC2B48,0x003E00A7L);
writeKernelMemory((void *)0x04CC2B4C,0x0066004BL);
writeKernelMemory((void *)0x04CC2B50,0x0069006CL);
writeKernelMemory((void *)0x04CC2B54,0x006C004DL);
writeKernelMemory((void *)0x04CC2B58,0x00650000L);
writeKernelMemory((void *)0x04CC2B5C,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2B60,0x38800064L);
writeKernelMemory((void *)0x04CC2B64,0x386200E0L);
writeKernelMemory((void *)0x04CC2B68,0x4CC63242L);
writeKernelMemory((void *)0x04CC2B6C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2B70,0x4E800421L);
writeKernelMemory((void *)0x04CC2B74,0x4800002DL);
writeKernelMemory((void *)0x04CC2B78,0x00A70066L);
writeKernelMemory((void *)0x04CC2B7C,0x003E00A7L);
writeKernelMemory((void *)0x04CC2B80,0x00660053L);
writeKernelMemory((void *)0x04CC2B84,0x00650074L);
writeKernelMemory((void *)0x04CC2B88,0x00530070L);
writeKernelMemory((void *)0x04CC2B8C,0x00610077L);
writeKernelMemory((void *)0x04CC2B90,0x006E0070L);
writeKernelMemory((void *)0x04CC2B94,0x006F0069L);
writeKernelMemory((void *)0x04CC2B98,0x006E0074L);
writeKernelMemory((void *)0x04CC2B9C,0x00000000L);
writeKernelMemory((void *)0x04CC2BA0,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2BA4,0x38800064L);
writeKernelMemory((void *)0x04CC2BA8,0x38620100L);
writeKernelMemory((void *)0x04CC2BAC,0x4CC63242L);
writeKernelMemory((void *)0x04CC2BB0,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2BB4,0x4E800421L);
writeKernelMemory((void *)0x04CC2BB8,0x48000021L);
writeKernelMemory((void *)0x04CC2BBC,0x00A70066L);
writeKernelMemory((void *)0x04CC2BC0,0x003E00A7L);
writeKernelMemory((void *)0x04CC2BC4,0x00660053L);
writeKernelMemory((void *)0x04CC2BC8,0x00650074L);
writeKernelMemory((void *)0x04CC2BCC,0x00540069L);
writeKernelMemory((void *)0x04CC2BD0,0x006D0065L);
writeKernelMemory((void *)0x04CC2BD4,0x00000000L);
writeKernelMemory((void *)0x04CC2BD8,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2BDC,0x38800064L);
writeKernelMemory((void *)0x04CC2BE0,0x38620130L);
writeKernelMemory((void *)0x04CC2BE4,0x4CC63242L);
writeKernelMemory((void *)0x04CC2BE8,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2BEC,0x4E800421L);
writeKernelMemory((void *)0x04CC2BF0,0x4800000DL);
writeKernelMemory((void *)0x04CC2BF4,0x00500076L);
writeKernelMemory((void *)0x04CC2BF8,0x00500000L);
writeKernelMemory((void *)0x04CC2BFC,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2C00,0x38800064L);
writeKernelMemory((void *)0x04CC2C04,0x38620150L);
writeKernelMemory((void *)0x04CC2C08,0x4CC63242L);
writeKernelMemory((void *)0x04CC2C0C,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2C10,0x4E800421L);
writeKernelMemory((void *)0x04CC2C14,0x4800002DL);
writeKernelMemory((void *)0x04CC2C18,0x00A70066L);
writeKernelMemory((void *)0x04CC2C1C,0x003E00A7L);
writeKernelMemory((void *)0x04CC2C20,0x00660052L);
writeKernelMemory((void *)0x04CC2C24,0x00690070L);
writeKernelMemory((void *)0x04CC2C28,0x00740069L);
writeKernelMemory((void *)0x04CC2C2C,0x00640065L);
writeKernelMemory((void *)0x04CC2C30,0x00410074L);
writeKernelMemory((void *)0x04CC2C34,0x00740061L);
writeKernelMemory((void *)0x04CC2C38,0x0063006BL);
writeKernelMemory((void *)0x04CC2C3C,0x00000000L);
writeKernelMemory((void *)0x04CC2C40,0x7CA802A6L);
writeKernelMemory((void *)0x04CC2C44,0x38800064L);
writeKernelMemory((void *)0x04CC2C48,0x38620160L);
writeKernelMemory((void *)0x04CC2C4C,0x4CC63242L);
writeKernelMemory((void *)0x04CC2C50,0x7FE903A6L);
writeKernelMemory((void *)0x04CC2C54,0x4E800421L);
writeKernelMemory((void *)0x04CC2C58,0x3F800310L);
writeKernelMemory((void *)0x04CC2C5C,0x639C3064L);
writeKernelMemory((void *)0x04CC2C60,0x3FA00310L);
writeKernelMemory((void *)0x04CC2C64,0x63BD3080L);
writeKernelMemory((void *)0x04CC2C68,0x38600001L);
writeKernelMemory((void *)0x04CC2C6C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC2C70,0x4E800421L);
writeKernelMemory((void *)0x04CC2C74,0x7FA903A6L);
writeKernelMemory((void *)0x04CC2C78,0x4E800421L);
writeKernelMemory((void *)0x04CC2C7C,0x3D800000L);
writeKernelMemory((void *)0x04CC2C80,0x918203F4L);
writeKernelMemory((void *)0x04CC2C84,0xC02203F4L);
writeKernelMemory((void *)0x04CC2C88,0x3D8043E0L);
writeKernelMemory((void *)0x04CC2C8C,0x918203F8L);
writeKernelMemory((void *)0x04CC2C90,0xC04203F8L);
writeKernelMemory((void *)0x04CC2C94,0x3D804374L);
writeKernelMemory((void *)0x04CC2C98,0x918203FCL);
writeKernelMemory((void *)0x04CC2C9C,0xC06203FCL);
writeKernelMemory((void *)0x04CC2CA0,0x3D80453BL);
writeKernelMemory((void *)0x04CC2CA4,0x91820408L);
writeKernelMemory((void *)0x04CC2CA8,0xC0C20408L);
writeKernelMemory((void *)0x04CC2CAC,0x3D800311L);
writeKernelMemory((void *)0x04CC2CB0,0x618C0D4CL);
writeKernelMemory((void *)0x04CC2CB4,0x7D8903A6L);
writeKernelMemory((void *)0x04CC2CB8,0x4E800421L);
writeKernelMemory((void *)0x04CC2CBC,0x38600000L);
writeKernelMemory((void *)0x04CC2CC0,0x7F8903A6L);
writeKernelMemory((void *)0x04CC2CC4,0x4E800421L);
writeKernelMemory((void *)0x04CC2CC8,0x7FA903A6L);
writeKernelMemory((void *)0x04CC2CCC,0x4E800421L);
writeKernelMemory((void *)0x04CC2CD0,0x39800000L);
writeKernelMemory((void *)0x04CC2CD4,0x9182040CL);
writeKernelMemory((void *)0x04CC2CD8,0xC022040CL);
writeKernelMemory((void *)0x04CC2CDC,0x91820410L);
writeKernelMemory((void *)0x04CC2CE0,0xC0420410L);
writeKernelMemory((void *)0x04CC2CE4,0x3FA0030EL);
writeKernelMemory((void *)0x04CC2CE8,0x63BD4B54L);
writeKernelMemory((void *)0x04CC2CEC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC2CF0,0x4E800421L);
writeKernelMemory((void *)0x04CC2CF4,0x3D80030FL);
writeKernelMemory((void *)0x04CC2CF8,0x618C974CL);
writeKernelMemory((void *)0x04CC2CFC,0x7D8903A6L);
writeKernelMemory((void *)0x04CC2D00,0x4E800421L);
writeKernelMemory((void *)0x04CC2D04,0x3D80030EL);
writeKernelMemory((void *)0x04CC2D08,0x618C4B24L);
writeKernelMemory((void *)0x04CC2D0C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC2D10,0x4E800421L);
writeKernelMemory((void *)0x04CC2D14,0x3D80030EL);
writeKernelMemory((void *)0x04CC2D18,0x618C9B9CL);
writeKernelMemory((void *)0x04CC2D1C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC2D20,0x4E800421L);
writeKernelMemory((void *)0x04CC2D24,0x3D80030EL);
writeKernelMemory((void *)0x04CC2D28,0x618C6268L);
writeKernelMemory((void *)0x04CC2D2C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC2D30,0x4E800421L);
writeKernelMemory((void *)0x04CC2D34,0x3D80030EL);
writeKernelMemory((void *)0x04CC2D38,0x618C5284L);
writeKernelMemory((void *)0x04CC2D3C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC2D40,0x4E800421L);
writeKernelMemory((void *)0x04CC2D44,0x38600004L);
writeKernelMemory((void *)0x04CC2D48,0x38800005L);
writeKernelMemory((void *)0x04CC2D4C,0x3D80030EL);
writeKernelMemory((void *)0x04CC2D50,0x618C52A0L);
writeKernelMemory((void *)0x04CC2D54,0x7D8903A6L);
writeKernelMemory((void *)0x04CC2D58,0x4E800421L);
writeKernelMemory((void *)0x04CC2D5C,0x82B90104L);
writeKernelMemory((void *)0x04CC2D60,0x3EC0030EL);
writeKernelMemory((void *)0x04CC2D64,0x62DC4BD8L);
writeKernelMemory((void *)0x04CC2D68,0x62D74B6CL);
writeKernelMemory((void *)0x04CC2D6C,0x62D64B3CL);
writeKernelMemory((void *)0x04CC2D70,0x3F000312L);
writeKernelMemory((void *)0x04CC2D74,0x63186B88L);
writeKernelMemory((void *)0x04CC2D78,0x3F40020BL);
writeKernelMemory((void *)0x04CC2D7C,0x635A08D4L);
writeKernelMemory((void *)0x04CC2D80,0x3D80108DL);
writeKernelMemory((void *)0x04CC2D84,0x3D20BFCFL);
writeKernelMemory((void *)0x04CC2D88,0x61295C29L);
writeKernelMemory((void *)0x04CC2D8C,0x912CB504L);
writeKernelMemory((void *)0x04CC2D90,0x2C1E0000L);
writeKernelMemory((void *)0x04CC2D94,0x41820014L);
writeKernelMemory((void *)0x04CC2D98,0x3D800101L);
writeKernelMemory((void *)0x04CC2D9C,0x398C0101L);
writeKernelMemory((void *)0x04CC2DA0,0x7C1E6000L);
writeKernelMemory((void *)0x04CC2DA4,0x40820E7DL);
writeKernelMemory((void *)0x04CC2DA8,0x39200000L);
writeKernelMemory((void *)0x04CC2DAC,0x2C140180L);
writeKernelMemory((void *)0x04CC2DB0,0x41820E29L);
writeKernelMemory((void *)0x04CC2DB4,0x2C144000L);
writeKernelMemory((void *)0x04CC2DB8,0x41820E61L);
writeKernelMemory((void *)0x04CC2DBC,0x48000004L);
writeKernelMemory((void *)0x04CC2DC0,0x81820A14L);
writeKernelMemory((void *)0x04CC2DC4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC2DC8,0x40823525L);
writeKernelMemory((void *)0x04CC2DCC,0x2C0C0003L);
writeKernelMemory((void *)0x04CC2DD0,0x41822E38L);
writeKernelMemory((void *)0x04CC2DD4,0x2C0C0002L);
writeKernelMemory((void *)0x04CC2DD8,0x418227A8L);
writeKernelMemory((void *)0x04CC2DDC,0x2C0C0001L);
writeKernelMemory((void *)0x04CC2DE0,0x41820EC4L);
writeKernelMemory((void *)0x04CC2DE4,0x81820A28L);
writeKernelMemory((void *)0x04CC2DE8,0x3D401095L);
writeKernelMemory((void *)0x04CC2DEC,0x614A3EBCL);
writeKernelMemory((void *)0x04CC2DF0,0x7C0C5000L);
writeKernelMemory((void *)0x04CC2DF4,0x41820010L);
writeKernelMemory((void *)0x04CC2DF8,0x718C0010L);
writeKernelMemory((void *)0x04CC2DFC,0x2C0C0010L);
writeKernelMemory((void *)0x04CC2E00,0x41823358L);
writeKernelMemory((void *)0x04CC2E04,0x3D80108DL);
writeKernelMemory((void *)0x04CC2E08,0x3D20C0A0L);
writeKernelMemory((void *)0x04CC2E0C,0x912CB504L);
writeKernelMemory((void *)0x04CC2E10,0x7EC903A6L);
writeKernelMemory((void *)0x04CC2E14,0x4E800421L);
writeKernelMemory((void *)0x04CC2E18,0x3D80400AL);
writeKernelMemory((void *)0x04CC2E1C,0x91820418L);
writeKernelMemory((void *)0x04CC2E20,0xC0220418L);
writeKernelMemory((void *)0x04CC2E24,0xC0420418L);
writeKernelMemory((void *)0x04CC2E28,0x7EE903A6L);
writeKernelMemory((void *)0x04CC2E2C,0x4E800421L);
writeKernelMemory((void *)0x04CC2E30,0x3D804000L);
writeKernelMemory((void *)0x04CC2E34,0x9182FF04L);
writeKernelMemory((void *)0x04CC2E38,0xC022FF04L);
writeKernelMemory((void *)0x04CC2E3C,0x9182FEFCL);
writeKernelMemory((void *)0x04CC2E40,0xC042FEFCL);
writeKernelMemory((void *)0x04CC2E44,0x7FA903A6L);
writeKernelMemory((void *)0x04CC2E48,0x4E800421L);
writeKernelMemory((void *)0x04CC2E4C,0x3D8010ADL);
writeKernelMemory((void *)0x04CC2E50,0x818C1C58L);
writeKernelMemory((void *)0x04CC2E54,0x2C0C0000L);
writeKernelMemory((void *)0x04CC2E58,0x41820000L);
writeKernelMemory((void *)0x04CC2E5C,0x818C004EL);
writeKernelMemory((void *)0x04CC2E60,0x2C0C0061L);
writeKernelMemory((void *)0x04CC2E64,0x41800018L);
writeKernelMemory((void *)0x04CC2E68,0x2C0C0067L);
writeKernelMemory((void *)0x04CC2E6C,0x41810010L);
writeKernelMemory((void *)0x04CC2E70,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2E74,0x394A0061L);
writeKernelMemory((void *)0x04CC2E78,0x91420260L);
writeKernelMemory((void *)0x04CC2E7C,0x2C0C0041L);
writeKernelMemory((void *)0x04CC2E80,0x41800018L);
writeKernelMemory((void *)0x04CC2E84,0x2C0C0047L);
writeKernelMemory((void *)0x04CC2E88,0x41810010L);
writeKernelMemory((void *)0x04CC2E8C,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2E90,0x394A0061L);
writeKernelMemory((void *)0x04CC2E94,0x91420260L);
writeKernelMemory((void *)0x04CC2E98,0x2C0C0068L);
writeKernelMemory((void *)0x04CC2E9C,0x41800018L);
writeKernelMemory((void *)0x04CC2EA0,0x2C0C006EL);
writeKernelMemory((void *)0x04CC2EA4,0x41810010L);
writeKernelMemory((void *)0x04CC2EA8,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2EAC,0x394A0065L);
writeKernelMemory((void *)0x04CC2EB0,0x91420260L);
writeKernelMemory((void *)0x04CC2EB4,0x2C0C0048L);
writeKernelMemory((void *)0x04CC2EB8,0x41800018L);
writeKernelMemory((void *)0x04CC2EBC,0x2C0C004EL);
writeKernelMemory((void *)0x04CC2EC0,0x41810010L);
writeKernelMemory((void *)0x04CC2EC4,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2EC8,0x394A0065L);
writeKernelMemory((void *)0x04CC2ECC,0x91420260L);
writeKernelMemory((void *)0x04CC2ED0,0x2C0C0070L);
writeKernelMemory((void *)0x04CC2ED4,0x41800018L);
writeKernelMemory((void *)0x04CC2ED8,0x2C0C0075L);
writeKernelMemory((void *)0x04CC2EDC,0x41810010L);
writeKernelMemory((void *)0x04CC2EE0,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2EE4,0x394A0064L);
writeKernelMemory((void *)0x04CC2EE8,0x91420260L);
writeKernelMemory((void *)0x04CC2EEC,0x2C0C0050L);
writeKernelMemory((void *)0x04CC2EF0,0x41800018L);
writeKernelMemory((void *)0x04CC2EF4,0x2C0C0055L);
writeKernelMemory((void *)0x04CC2EF8,0x41810010L);
writeKernelMemory((void *)0x04CC2EFC,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2F00,0x394A0064L);
writeKernelMemory((void *)0x04CC2F04,0x91420260L);
writeKernelMemory((void *)0x04CC2F08,0x2C0C0076L);
writeKernelMemory((void *)0x04CC2F0C,0x41800018L);
writeKernelMemory((void *)0x04CC2F10,0x2C0C007AL);
writeKernelMemory((void *)0x04CC2F14,0x41810010L);
writeKernelMemory((void *)0x04CC2F18,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2F1C,0x394A0036L);
writeKernelMemory((void *)0x04CC2F20,0x91420260L);
writeKernelMemory((void *)0x04CC2F24,0x2C0C0056L);
writeKernelMemory((void *)0x04CC2F28,0x41800018L);
writeKernelMemory((void *)0x04CC2F2C,0x2C0C005AL);
writeKernelMemory((void *)0x04CC2F30,0x41810010L);
writeKernelMemory((void *)0x04CC2F34,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2F38,0x394A0036L);
writeKernelMemory((void *)0x04CC2F3C,0x91420260L);
writeKernelMemory((void *)0x04CC2F40,0x2C0C0030L);
writeKernelMemory((void *)0x04CC2F44,0x41800018L);
writeKernelMemory((void *)0x04CC2F48,0x2C0C0039L);
writeKernelMemory((void *)0x04CC2F4C,0x41810010L);
writeKernelMemory((void *)0x04CC2F50,0x3D4000A7L);
writeKernelMemory((void *)0x04CC2F54,0x394A0037L);
writeKernelMemory((void *)0x04CC2F58,0x91420260L);
writeKernelMemory((void *)0x04CC2F5C,0x38620348L);
writeKernelMemory((void *)0x04CC2F60,0x38820260L);
writeKernelMemory((void *)0x04CC2F64,0x7F4903A6L);
writeKernelMemory((void *)0x04CC2F68,0x4E800421L);
writeKernelMemory((void *)0x04CC2F6C,0x480032B5L);
writeKernelMemory((void *)0x04CC2F70,0x7F8903A6L);
writeKernelMemory((void *)0x04CC2F74,0x4E800421L);
writeKernelMemory((void *)0x04CC2F78,0x2C1E0000L);
writeKernelMemory((void *)0x04CC2F7C,0x41820014L);
writeKernelMemory((void *)0x04CC2F80,0x3D400101L);
writeKernelMemory((void *)0x04CC2F84,0x394A0101L);
writeKernelMemory((void *)0x04CC2F88,0x7C1E5000L);
writeKernelMemory((void *)0x04CC2F8C,0x40820058L);
writeKernelMemory((void *)0x04CC2F90,0x7EC903A6L);
writeKernelMemory((void *)0x04CC2F94,0x4E800421L);
writeKernelMemory((void *)0x04CC2F98,0x3D803F80L);
writeKernelMemory((void *)0x04CC2F9C,0x91820418L);
writeKernelMemory((void *)0x04CC2FA0,0xC0220418L);
writeKernelMemory((void *)0x04CC2FA4,0xC0420418L);
writeKernelMemory((void *)0x04CC2FA8,0x7EE903A6L);
writeKernelMemory((void *)0x04CC2FAC,0x4E800421L);
writeKernelMemory((void *)0x04CC2FB0,0xC022FF00L);
writeKernelMemory((void *)0x04CC2FB4,0x3D804363L);
writeKernelMemory((void *)0x04CC2FB8,0x9182FEFCL);
writeKernelMemory((void *)0x04CC2FBC,0xC042FEFCL);
writeKernelMemory((void *)0x04CC2FC0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC2FC4,0x4E800421L);
writeKernelMemory((void *)0x04CC2FC8,0x38620348L);
writeKernelMemory((void *)0x04CC2FCC,0x38820CA0L);
writeKernelMemory((void *)0x04CC2FD0,0x7F4903A6L);
writeKernelMemory((void *)0x04CC2FD4,0x4E800421L);
writeKernelMemory((void *)0x04CC2FD8,0x48003249L);
writeKernelMemory((void *)0x04CC2FDC,0x7F8903A6L);
writeKernelMemory((void *)0x04CC2FE0,0x4E800421L);
writeKernelMemory((void *)0x04CC2FE4,0x3D80C0A0L);
writeKernelMemory((void *)0x04CC2FE8,0x91820428L);
writeKernelMemory((void *)0x04CC2FEC,0x39600A00L);
writeKernelMemory((void *)0x04CC2FF0,0x8182FD60L);
writeKernelMemory((void *)0x04CC2FF4,0x2C0C0001L);
writeKernelMemory((void *)0x04CC2FF8,0x3D4043E1L);
writeKernelMemory((void *)0x04CC2FFC,0x41820C49L);
writeKernelMemory((void *)0x04CC3000,0x8182FD60L);
writeKernelMemory((void *)0x04CC3004,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3008,0x3D404400L);
writeKernelMemory((void *)0x04CC300C,0x41820C69L);
writeKernelMemory((void *)0x04CC3010,0x81820A00L);
writeKernelMemory((void *)0x04CC3014,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3018,0x4080004CL);
writeKernelMemory((void *)0x04CC301C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3020,0x4E800421L);
writeKernelMemory((void *)0x04CC3024,0x480031D5L);
writeKernelMemory((void *)0x04CC3028,0x3D80BF80L);
writeKernelMemory((void *)0x04CC302C,0x9182F1F0L);
writeKernelMemory((void *)0x04CC3030,0xC002F1F0L);
writeKernelMemory((void *)0x04CC3034,0xC0220A00L);
writeKernelMemory((void *)0x04CC3038,0xFC21002AL);
writeKernelMemory((void *)0x04CC303C,0x48003201L);
writeKernelMemory((void *)0x04CC3040,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3044,0x4E800421L);
writeKernelMemory((void *)0x04CC3048,0x38620348L);
writeKernelMemory((void *)0x04CC304C,0x38820C0AL);
writeKernelMemory((void *)0x04CC3050,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3054,0x4E800421L);
writeKernelMemory((void *)0x04CC3058,0x480031C9L);
writeKernelMemory((void *)0x04CC305C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3060,0x4E800421L);
writeKernelMemory((void *)0x04CC3064,0x3D400101L);
writeKernelMemory((void *)0x04CC3068,0x394A0101L);
writeKernelMemory((void *)0x04CC306C,0x7C1E5000L);
writeKernelMemory((void *)0x04CC3070,0x418200B8L);
writeKernelMemory((void *)0x04CC3074,0x3D8040A0L);
writeKernelMemory((void *)0x04CC3078,0x918203B0L);
writeKernelMemory((void *)0x04CC307C,0x819900C8L);
writeKernelMemory((void *)0x04CC3080,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3084,0x41820028L);
writeKernelMemory((void *)0x04CC3088,0xC18203B0L);
writeKernelMemory((void *)0x04CC308C,0xC00203B4L);
writeKernelMemory((void *)0x04CC3090,0xFC00602AL);
writeKernelMemory((void *)0x04CC3094,0x818203B4L);
writeKernelMemory((void *)0x04CC3098,0x3D204100L);
writeKernelMemory((void *)0x04CC309C,0x7C0C4800L);
writeKernelMemory((void *)0x04CC30A0,0x40800034L);
writeKernelMemory((void *)0x04CC30A4,0xD00203B4L);
writeKernelMemory((void *)0x04CC30A8,0x4800002CL);
writeKernelMemory((void *)0x04CC30AC,0xC18203B0L);
writeKernelMemory((void *)0x04CC30B0,0xC00203B4L);
writeKernelMemory((void *)0x04CC30B4,0xFC006028L);
writeKernelMemory((void *)0x04CC30B8,0x818203B4L);
writeKernelMemory((void *)0x04CC30BC,0x3D20C250L);
writeKernelMemory((void *)0x04CC30C0,0x7C0C4800L);
writeKernelMemory((void *)0x04CC30C4,0x4081000CL);
writeKernelMemory((void *)0x04CC30C8,0xD00203B4L);
writeKernelMemory((void *)0x04CC30CC,0x48000008L);
writeKernelMemory((void *)0x04CC30D0,0x912203B4L);
writeKernelMemory((void *)0x04CC30D4,0x7EC903A6L);
writeKernelMemory((void *)0x04CC30D8,0x4E800421L);
writeKernelMemory((void *)0x04CC30DC,0x3D803F90L);
writeKernelMemory((void *)0x04CC30E0,0x91820418L);
writeKernelMemory((void *)0x04CC30E4,0xC0220418L);
writeKernelMemory((void *)0x04CC30E8,0xC0420418L);
writeKernelMemory((void *)0x04CC30EC,0x7EE903A6L);
writeKernelMemory((void *)0x04CC30F0,0x4E800421L);
writeKernelMemory((void *)0x04CC30F4,0xC02203B4L);
writeKernelMemory((void *)0x04CC30F8,0x3D8042A0L);
writeKernelMemory((void *)0x04CC30FC,0x91820A6CL);
writeKernelMemory((void *)0x04CC3100,0xC0420A6CL);
writeKernelMemory((void *)0x04CC3104,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3108,0x4E800421L);
writeKernelMemory((void *)0x04CC310C,0x38620348L);
writeKernelMemory((void *)0x04CC3110,0x38820A70L);
writeKernelMemory((void *)0x04CC3114,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3118,0x4E800421L);
writeKernelMemory((void *)0x04CC311C,0x48003105L);
writeKernelMemory((void *)0x04CC3120,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3124,0x4E800421L);
writeKernelMemory((void *)0x04CC3128,0x3D400101L);
writeKernelMemory((void *)0x04CC312C,0x394A0101L);
writeKernelMemory((void *)0x04CC3130,0x7C1E5000L);
writeKernelMemory((void *)0x04CC3134,0x41820058L);
writeKernelMemory((void *)0x04CC3138,0x7EC903A6L);
writeKernelMemory((void *)0x04CC313C,0x4E800421L);
writeKernelMemory((void *)0x04CC3140,0x3D803F90L);
writeKernelMemory((void *)0x04CC3144,0x91820418L);
writeKernelMemory((void *)0x04CC3148,0xC0220418L);
writeKernelMemory((void *)0x04CC314C,0xC0420418L);
writeKernelMemory((void *)0x04CC3150,0x7EE903A6L);
writeKernelMemory((void *)0x04CC3154,0x4E800421L);
writeKernelMemory((void *)0x04CC3158,0xC02203B4L);
writeKernelMemory((void *)0x04CC315C,0x3D8042BAL);
writeKernelMemory((void *)0x04CC3160,0x91820A6CL);
writeKernelMemory((void *)0x04CC3164,0xC0420A6CL);
writeKernelMemory((void *)0x04CC3168,0x7FA903A6L);
writeKernelMemory((void *)0x04CC316C,0x4E800421L);
writeKernelMemory((void *)0x04CC3170,0x38620348L);
writeKernelMemory((void *)0x04CC3174,0x38820A90L);
writeKernelMemory((void *)0x04CC3178,0x7F4903A6L);
writeKernelMemory((void *)0x04CC317C,0x4E800421L);
writeKernelMemory((void *)0x04CC3180,0x480030A1L);
writeKernelMemory((void *)0x04CC3184,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3188,0x4E800421L);
writeKernelMemory((void *)0x04CC318C,0x39600A50L);
writeKernelMemory((void *)0x04CC3190,0x8182FD5CL);
writeKernelMemory((void *)0x04CC3194,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3198,0x3D4043E1L);
writeKernelMemory((void *)0x04CC319C,0x614AF000L);
writeKernelMemory((void *)0x04CC31A0,0x41820AA5L);
writeKernelMemory((void *)0x04CC31A4,0x8182FD5CL);
writeKernelMemory((void *)0x04CC31A8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC31AC,0x3D404400L);
writeKernelMemory((void *)0x04CC31B0,0x41820AC5L);
writeKernelMemory((void *)0x04CC31B4,0x81820A50L);
writeKernelMemory((void *)0x04CC31B8,0x7C0C5000L);
writeKernelMemory((void *)0x04CC31BC,0x4080003CL);
writeKernelMemory((void *)0x04CC31C0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC31C4,0x4E800421L);
writeKernelMemory((void *)0x04CC31C8,0x48003031L);
writeKernelMemory((void *)0x04CC31CC,0xC0220A50L);
writeKernelMemory((void *)0x04CC31D0,0x4800306DL);
writeKernelMemory((void *)0x04CC31D4,0x7FA903A6L);
writeKernelMemory((void *)0x04CC31D8,0x4E800421L);
writeKernelMemory((void *)0x04CC31DC,0x38620348L);
writeKernelMemory((void *)0x04CC31E0,0x38820CEAL);
writeKernelMemory((void *)0x04CC31E4,0x7F4903A6L);
writeKernelMemory((void *)0x04CC31E8,0x4E800421L);
writeKernelMemory((void *)0x04CC31EC,0x48003035L);
writeKernelMemory((void *)0x04CC31F0,0x7F8903A6L);
writeKernelMemory((void *)0x04CC31F4,0x4E800421L);
writeKernelMemory((void *)0x04CC31F8,0x39600A60L);
writeKernelMemory((void *)0x04CC31FC,0x8182FD58L);
writeKernelMemory((void *)0x04CC3200,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3204,0x3D4043DDL);
writeKernelMemory((void *)0x04CC3208,0x614AE000L);
writeKernelMemory((void *)0x04CC320C,0x41820A39L);
writeKernelMemory((void *)0x04CC3210,0x8182FD58L);
writeKernelMemory((void *)0x04CC3214,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3218,0x3D404400L);
writeKernelMemory((void *)0x04CC321C,0x41820A59L);
writeKernelMemory((void *)0x04CC3220,0x81820A60L);
writeKernelMemory((void *)0x04CC3224,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3228,0x4080003CL);
writeKernelMemory((void *)0x04CC322C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3230,0x4E800421L);
writeKernelMemory((void *)0x04CC3234,0x48002FC5L);
writeKernelMemory((void *)0x04CC3238,0xC0220A60L);
writeKernelMemory((void *)0x04CC323C,0x48003001L);
writeKernelMemory((void *)0x04CC3240,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3244,0x4E800421L);
writeKernelMemory((void *)0x04CC3248,0x38820D26L);
writeKernelMemory((void *)0x04CC324C,0x38620348L);
writeKernelMemory((void *)0x04CC3250,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3254,0x4E800421L);
writeKernelMemory((void *)0x04CC3258,0x48002FC9L);
writeKernelMemory((void *)0x04CC325C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3260,0x4E800421L);
writeKernelMemory((void *)0x04CC3264,0x39600BE0L);
writeKernelMemory((void *)0x04CC3268,0x8182FD54L);
writeKernelMemory((void *)0x04CC326C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3270,0x3D4043EFL);
writeKernelMemory((void *)0x04CC3274,0x418209D1L);
writeKernelMemory((void *)0x04CC3278,0x8182FD54L);
writeKernelMemory((void *)0x04CC327C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3280,0x3D404400L);
writeKernelMemory((void *)0x04CC3284,0x418209F1L);
writeKernelMemory((void *)0x04CC3288,0x81820BE0L);
writeKernelMemory((void *)0x04CC328C,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3290,0x4080003CL);
writeKernelMemory((void *)0x04CC3294,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3298,0x4E800421L);
writeKernelMemory((void *)0x04CC329C,0x48002F5DL);
writeKernelMemory((void *)0x04CC32A0,0xC0220BE0L);
writeKernelMemory((void *)0x04CC32A4,0x48002F99L);
writeKernelMemory((void *)0x04CC32A8,0x7FA903A6L);
writeKernelMemory((void *)0x04CC32AC,0x4E800421L);
writeKernelMemory((void *)0x04CC32B0,0x38820D96L);
writeKernelMemory((void *)0x04CC32B4,0x38620348L);
writeKernelMemory((void *)0x04CC32B8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC32BC,0x4E800421L);
writeKernelMemory((void *)0x04CC32C0,0x48002F61L);
writeKernelMemory((void *)0x04CC32C4,0x7F8903A6L);
writeKernelMemory((void *)0x04CC32C8,0x4E800421L);
writeKernelMemory((void *)0x04CC32CC,0x39600DF8L);
writeKernelMemory((void *)0x04CC32D0,0x8182FD50L);
writeKernelMemory((void *)0x04CC32D4,0x2C0C0001L);
writeKernelMemory((void *)0x04CC32D8,0x3D4043DFL);
writeKernelMemory((void *)0x04CC32DC,0x614A9000L);
writeKernelMemory((void *)0x04CC32E0,0x41820965L);
writeKernelMemory((void *)0x04CC32E4,0x8182FD50L);
writeKernelMemory((void *)0x04CC32E8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC32EC,0x3D404400L);
writeKernelMemory((void *)0x04CC32F0,0x41820985L);
writeKernelMemory((void *)0x04CC32F4,0x81820DF8L);
writeKernelMemory((void *)0x04CC32F8,0x7C0C5000L);
writeKernelMemory((void *)0x04CC32FC,0x4080004CL);
writeKernelMemory((void *)0x04CC3300,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3304,0x4E800421L);
writeKernelMemory((void *)0x04CC3308,0x48002EF1L);
writeKernelMemory((void *)0x04CC330C,0x3D803EC0L);
writeKernelMemory((void *)0x04CC3310,0x9182F1F0L);
writeKernelMemory((void *)0x04CC3314,0xC002F1F0L);
writeKernelMemory((void *)0x04CC3318,0xC0220DF8L);
writeKernelMemory((void *)0x04CC331C,0xFC21002AL);
writeKernelMemory((void *)0x04CC3320,0x48002F1DL);
writeKernelMemory((void *)0x04CC3324,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3328,0x4E800421L);
writeKernelMemory((void *)0x04CC332C,0x38820E06L);
writeKernelMemory((void *)0x04CC3330,0x38620348L);
writeKernelMemory((void *)0x04CC3334,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3338,0x4E800421L);
writeKernelMemory((void *)0x04CC333C,0x48002EE5L);
writeKernelMemory((void *)0x04CC3340,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3344,0x4E800421L);
writeKernelMemory((void *)0x04CC3348,0x39600E28L);
writeKernelMemory((void *)0x04CC334C,0x8182FD4CL);
writeKernelMemory((void *)0x04CC3350,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3354,0x3D4043DDL);
writeKernelMemory((void *)0x04CC3358,0x418208EDL);
writeKernelMemory((void *)0x04CC335C,0x8182FD4CL);
writeKernelMemory((void *)0x04CC3360,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3364,0x3D404400L);
writeKernelMemory((void *)0x04CC3368,0x4182090DL);
writeKernelMemory((void *)0x04CC336C,0x81820E28L);
writeKernelMemory((void *)0x04CC3370,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3374,0x4080003CL);
writeKernelMemory((void *)0x04CC3378,0x7EC903A6L);
writeKernelMemory((void *)0x04CC337C,0x4E800421L);
writeKernelMemory((void *)0x04CC3380,0x48002E79L);
writeKernelMemory((void *)0x04CC3384,0xC0220E28L);
writeKernelMemory((void *)0x04CC3388,0x48002EB5L);
writeKernelMemory((void *)0x04CC338C,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3390,0x4E800421L);
writeKernelMemory((void *)0x04CC3394,0x38820E36L);
writeKernelMemory((void *)0x04CC3398,0x38620348L);
writeKernelMemory((void *)0x04CC339C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC33A0,0x4E800421L);
writeKernelMemory((void *)0x04CC33A4,0x48002E7DL);
writeKernelMemory((void *)0x04CC33A8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC33AC,0x4E800421L);
writeKernelMemory((void *)0x04CC33B0,0x39600E58L);
writeKernelMemory((void *)0x04CC33B4,0x8182FD48L);
writeKernelMemory((void *)0x04CC33B8,0x2C0C0001L);
writeKernelMemory((void *)0x04CC33BC,0x3D4043EEL);
writeKernelMemory((void *)0x04CC33C0,0x41820885L);
writeKernelMemory((void *)0x04CC33C4,0x8182FD48L);
writeKernelMemory((void *)0x04CC33C8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC33CC,0x3D404401L);
writeKernelMemory((void *)0x04CC33D0,0x418208A5L);
writeKernelMemory((void *)0x04CC33D4,0x81820E58L);
writeKernelMemory((void *)0x04CC33D8,0x7C0C5000L);
writeKernelMemory((void *)0x04CC33DC,0x4080004CL);
writeKernelMemory((void *)0x04CC33E0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC33E4,0x4E800421L);
writeKernelMemory((void *)0x04CC33E8,0x48002E11L);
writeKernelMemory((void *)0x04CC33EC,0x3D80BFA0L);
writeKernelMemory((void *)0x04CC33F0,0x9182F1F0L);
writeKernelMemory((void *)0x04CC33F4,0xC002F1F0L);
writeKernelMemory((void *)0x04CC33F8,0xC0220E58L);
writeKernelMemory((void *)0x04CC33FC,0xFC21002AL);
writeKernelMemory((void *)0x04CC3400,0x48002E3DL);
writeKernelMemory((void *)0x04CC3404,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3408,0x4E800421L);
writeKernelMemory((void *)0x04CC340C,0x38820E66L);
writeKernelMemory((void *)0x04CC3410,0x38620348L);
writeKernelMemory((void *)0x04CC3414,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3418,0x4E800421L);
writeKernelMemory((void *)0x04CC341C,0x48002E05L);
writeKernelMemory((void *)0x04CC3420,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3424,0x4E800421L);
writeKernelMemory((void *)0x04CC3428,0x39600E98L);
writeKernelMemory((void *)0x04CC342C,0x8182FD44L);
writeKernelMemory((void *)0x04CC3430,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3434,0x3D4043EEL);
writeKernelMemory((void *)0x04CC3438,0x4182080DL);
writeKernelMemory((void *)0x04CC343C,0x8182FD44L);
writeKernelMemory((void *)0x04CC3440,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3444,0x3D404401L);
writeKernelMemory((void *)0x04CC3448,0x4182082DL);
writeKernelMemory((void *)0x04CC344C,0x81820E98L);
writeKernelMemory((void *)0x04CC3450,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3454,0x40800044L);
writeKernelMemory((void *)0x04CC3458,0x7EC903A6L);
writeKernelMemory((void *)0x04CC345C,0x4E800421L);
writeKernelMemory((void *)0x04CC3460,0x48002D99L);
writeKernelMemory((void *)0x04CC3464,0xC002F1F0L);
writeKernelMemory((void *)0x04CC3468,0xC0220E98L);
writeKernelMemory((void *)0x04CC346C,0xFC21002AL);
writeKernelMemory((void *)0x04CC3470,0x48002DCDL);
writeKernelMemory((void *)0x04CC3474,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3478,0x4E800421L);
writeKernelMemory((void *)0x04CC347C,0x38820E86L);
writeKernelMemory((void *)0x04CC3480,0x38620348L);
writeKernelMemory((void *)0x04CC3484,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3488,0x4E800421L);
writeKernelMemory((void *)0x04CC348C,0x48002D95L);
writeKernelMemory((void *)0x04CC3490,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3494,0x4E800421L);
writeKernelMemory((void *)0x04CC3498,0x39600EE0L);
writeKernelMemory((void *)0x04CC349C,0x8182FD40L);
writeKernelMemory((void *)0x04CC34A0,0x2C0C0001L);
writeKernelMemory((void *)0x04CC34A4,0x3D4043E2L);
writeKernelMemory((void *)0x04CC34A8,0x4182079DL);
writeKernelMemory((void *)0x04CC34AC,0x8182FD40L);
writeKernelMemory((void *)0x04CC34B0,0x2C0C0000L);
writeKernelMemory((void *)0x04CC34B4,0x3D404400L);
writeKernelMemory((void *)0x04CC34B8,0x418207BDL);
writeKernelMemory((void *)0x04CC34BC,0x81820EE0L);
writeKernelMemory((void *)0x04CC34C0,0x7C0C5000L);
writeKernelMemory((void *)0x04CC34C4,0x4080003CL);
writeKernelMemory((void *)0x04CC34C8,0x7EC903A6L);
writeKernelMemory((void *)0x04CC34CC,0x4E800421L);
writeKernelMemory((void *)0x04CC34D0,0x48002D29L);
writeKernelMemory((void *)0x04CC34D4,0xC0220EE0L);
writeKernelMemory((void *)0x04CC34D8,0x48002D65L);
writeKernelMemory((void *)0x04CC34DC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC34E0,0x4E800421L);
writeKernelMemory((void *)0x04CC34E4,0x38820EB6L);
writeKernelMemory((void *)0x04CC34E8,0x38620348L);
writeKernelMemory((void *)0x04CC34EC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC34F0,0x4E800421L);
writeKernelMemory((void *)0x04CC34F4,0x48002D2DL);
writeKernelMemory((void *)0x04CC34F8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC34FC,0x4E800421L);
writeKernelMemory((void *)0x04CC3500,0x39600EE8L);
writeKernelMemory((void *)0x04CC3504,0x8182FD3CL);
writeKernelMemory((void *)0x04CC3508,0x2C0C0001L);
writeKernelMemory((void *)0x04CC350C,0x3D4043EAL);
writeKernelMemory((void *)0x04CC3510,0x41820735L);
writeKernelMemory((void *)0x04CC3514,0x8182FD3CL);
writeKernelMemory((void *)0x04CC3518,0x2C0C0000L);
writeKernelMemory((void *)0x04CC351C,0x3D404400L);
writeKernelMemory((void *)0x04CC3520,0x41820755L);
writeKernelMemory((void *)0x04CC3524,0x81820EE8L);
writeKernelMemory((void *)0x04CC3528,0x7C0C5000L);
writeKernelMemory((void *)0x04CC352C,0x4080003CL);
writeKernelMemory((void *)0x04CC3530,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3534,0x4E800421L);
writeKernelMemory((void *)0x04CC3538,0x48002CC1L);
writeKernelMemory((void *)0x04CC353C,0xC0220EE8L);
writeKernelMemory((void *)0x04CC3540,0x48002CFDL);
writeKernelMemory((void *)0x04CC3544,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3548,0x4E800421L);
writeKernelMemory((void *)0x04CC354C,0x38820EF6L);
writeKernelMemory((void *)0x04CC3550,0x38620348L);
writeKernelMemory((void *)0x04CC3554,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3558,0x4E800421L);
writeKernelMemory((void *)0x04CC355C,0x48002CC5L);
writeKernelMemory((void *)0x04CC3560,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3564,0x4E800421L);
writeKernelMemory((void *)0x04CC3568,0x39600400L);
writeKernelMemory((void *)0x04CC356C,0x8182FD38L);
writeKernelMemory((void *)0x04CC3570,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3574,0x3D4043EAL);
writeKernelMemory((void *)0x04CC3578,0x418206CDL);
writeKernelMemory((void *)0x04CC357C,0x8182FD38L);
writeKernelMemory((void *)0x04CC3580,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3584,0x3D404400L);
writeKernelMemory((void *)0x04CC3588,0x418206EDL);
writeKernelMemory((void *)0x04CC358C,0x81820400L);
writeKernelMemory((void *)0x04CC3590,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3594,0x4080003CL);
writeKernelMemory((void *)0x04CC3598,0x7EC903A6L);
writeKernelMemory((void *)0x04CC359C,0x4E800421L);
writeKernelMemory((void *)0x04CC35A0,0x48002C59L);
writeKernelMemory((void *)0x04CC35A4,0xC0220400L);
writeKernelMemory((void *)0x04CC35A8,0x48002C95L);
writeKernelMemory((void *)0x04CC35AC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC35B0,0x4E800421L);
writeKernelMemory((void *)0x04CC35B4,0x38820F16L);
writeKernelMemory((void *)0x04CC35B8,0x38620348L);
writeKernelMemory((void *)0x04CC35BC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC35C0,0x4E800421L);
writeKernelMemory((void *)0x04CC35C4,0x48002C5DL);
writeKernelMemory((void *)0x04CC35C8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC35CC,0x4E800421L);
writeKernelMemory((void *)0x04CC35D0,0x39600EA8L);
writeKernelMemory((void *)0x04CC35D4,0x8182FD34L);
writeKernelMemory((void *)0x04CC35D8,0x2C0C0001L);
writeKernelMemory((void *)0x04CC35DC,0x3D4043F0L);
writeKernelMemory((void *)0x04CC35E0,0x41820665L);
writeKernelMemory((void *)0x04CC35E4,0x8182FD34L);
writeKernelMemory((void *)0x04CC35E8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC35EC,0x3D404400L);
writeKernelMemory((void *)0x04CC35F0,0x41820685L);
writeKernelMemory((void *)0x04CC35F4,0x81820EA8L);
writeKernelMemory((void *)0x04CC35F8,0x7C0C5000L);
writeKernelMemory((void *)0x04CC35FC,0x40800044L);
writeKernelMemory((void *)0x04CC3600,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3604,0x4E800421L);
writeKernelMemory((void *)0x04CC3608,0x48002BF1L);
writeKernelMemory((void *)0x04CC360C,0xC002F1F0L);
writeKernelMemory((void *)0x04CC3610,0xC0220EA8L);
writeKernelMemory((void *)0x04CC3614,0xFC21002AL);
writeKernelMemory((void *)0x04CC3618,0x48002C25L);
writeKernelMemory((void *)0x04CC361C,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3620,0x4E800421L);
writeKernelMemory((void *)0x04CC3624,0x38820F46L);
writeKernelMemory((void *)0x04CC3628,0x38620348L);
writeKernelMemory((void *)0x04CC362C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3630,0x4E800421L);
writeKernelMemory((void *)0x04CC3634,0x48002BEDL);
writeKernelMemory((void *)0x04CC3638,0x7F8903A6L);
writeKernelMemory((void *)0x04CC363C,0x4E800421L);
writeKernelMemory((void *)0x04CC3640,0x39600F30L);
writeKernelMemory((void *)0x04CC3644,0x8182FD30L);
writeKernelMemory((void *)0x04CC3648,0x2C0C0001L);
writeKernelMemory((void *)0x04CC364C,0x3D4043EEL);
writeKernelMemory((void *)0x04CC3650,0x418205F5L);
writeKernelMemory((void *)0x04CC3654,0x8182FD30L);
writeKernelMemory((void *)0x04CC3658,0x2C0C0000L);
writeKernelMemory((void *)0x04CC365C,0x3D404400L);
writeKernelMemory((void *)0x04CC3660,0x41820615L);
writeKernelMemory((void *)0x04CC3664,0x81820F30L);
writeKernelMemory((void *)0x04CC3668,0x7C0C5000L);
writeKernelMemory((void *)0x04CC366C,0x4080003CL);
writeKernelMemory((void *)0x04CC3670,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3674,0x4E800421L);
writeKernelMemory((void *)0x04CC3678,0x48002B81L);
writeKernelMemory((void *)0x04CC367C,0xC0220F30L);
writeKernelMemory((void *)0x04CC3680,0x48002BBDL);
writeKernelMemory((void *)0x04CC3684,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3688,0x4E800421L);
writeKernelMemory((void *)0x04CC368C,0x38820F76L);
writeKernelMemory((void *)0x04CC3690,0x38620348L);
writeKernelMemory((void *)0x04CC3694,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3698,0x4E800421L);
writeKernelMemory((void *)0x04CC369C,0x48002B85L);
writeKernelMemory((void *)0x04CC36A0,0x7F8903A6L);
writeKernelMemory((void *)0x04CC36A4,0x4E800421L);
writeKernelMemory((void *)0x04CC36A8,0x3960FF3CL);
writeKernelMemory((void *)0x04CC36AC,0x8182FC58L);
writeKernelMemory((void *)0x04CC36B0,0x2C0C0001L);
writeKernelMemory((void *)0x04CC36B4,0x3D4043EAL);
writeKernelMemory((void *)0x04CC36B8,0x4182058DL);
writeKernelMemory((void *)0x04CC36BC,0x8182FC58L);
writeKernelMemory((void *)0x04CC36C0,0x2C0C0000L);
writeKernelMemory((void *)0x04CC36C4,0x3D404400L);
writeKernelMemory((void *)0x04CC36C8,0x418205ADL);
writeKernelMemory((void *)0x04CC36CC,0x8182FF3CL);
writeKernelMemory((void *)0x04CC36D0,0x7C0C5000L);
writeKernelMemory((void *)0x04CC36D4,0x4080003CL);
writeKernelMemory((void *)0x04CC36D8,0x7EC903A6L);
writeKernelMemory((void *)0x04CC36DC,0x4E800421L);
writeKernelMemory((void *)0x04CC36E0,0x48002B19L);
writeKernelMemory((void *)0x04CC36E4,0xC022FF3CL);
writeKernelMemory((void *)0x04CC36E8,0x48002B55L);
writeKernelMemory((void *)0x04CC36EC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC36F0,0x4E800421L);
writeKernelMemory((void *)0x04CC36F4,0x3882FF16L);
writeKernelMemory((void *)0x04CC36F8,0x38620348L);
writeKernelMemory((void *)0x04CC36FC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3700,0x4E800421L);
writeKernelMemory((void *)0x04CC3704,0x48002B1DL);
writeKernelMemory((void *)0x04CC3708,0x7F8903A6L);
writeKernelMemory((void *)0x04CC370C,0x4E800421L);
writeKernelMemory((void *)0x04CC3710,0x3960FFB4L);
writeKernelMemory((void *)0x04CC3714,0x8182FC54L);
writeKernelMemory((void *)0x04CC3718,0x2C0C0001L);
writeKernelMemory((void *)0x04CC371C,0x3D4043DEL);
writeKernelMemory((void *)0x04CC3720,0x41820525L);
writeKernelMemory((void *)0x04CC3724,0x8182FC54L);
writeKernelMemory((void *)0x04CC3728,0x2C0C0000L);
writeKernelMemory((void *)0x04CC372C,0x3D404400L);
writeKernelMemory((void *)0x04CC3730,0x41820545L);
writeKernelMemory((void *)0x04CC3734,0x8182FFB4L);
writeKernelMemory((void *)0x04CC3738,0x7C0C5000L);
writeKernelMemory((void *)0x04CC373C,0x4080003CL);
writeKernelMemory((void *)0x04CC3740,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3744,0x4E800421L);
writeKernelMemory((void *)0x04CC3748,0x48002AB1L);
writeKernelMemory((void *)0x04CC374C,0xC022FFB4L);
writeKernelMemory((void *)0x04CC3750,0x48002AEDL);
writeKernelMemory((void *)0x04CC3754,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3758,0x4E800421L);
writeKernelMemory((void *)0x04CC375C,0x3882FF66L);
writeKernelMemory((void *)0x04CC3760,0x38620348L);
writeKernelMemory((void *)0x04CC3764,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3768,0x4E800421L);
writeKernelMemory((void *)0x04CC376C,0x48002AB5L);
writeKernelMemory((void *)0x04CC3770,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3774,0x4E800421L);
writeKernelMemory((void *)0x04CC3778,0x39600020L);
writeKernelMemory((void *)0x04CC377C,0x8182FB60L);
writeKernelMemory((void *)0x04CC3780,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3784,0x3D4043E0L);
writeKernelMemory((void *)0x04CC3788,0x394A2000L);
writeKernelMemory((void *)0x04CC378C,0x418204B9L);
writeKernelMemory((void *)0x04CC3790,0x8182FB60L);
writeKernelMemory((void *)0x04CC3794,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3798,0x3D404400L);
writeKernelMemory((void *)0x04CC379C,0x418204D9L);
writeKernelMemory((void *)0x04CC37A0,0x81820020L);
writeKernelMemory((void *)0x04CC37A4,0x7C0C5000L);
writeKernelMemory((void *)0x04CC37A8,0x4080003CL);
writeKernelMemory((void *)0x04CC37AC,0x7EC903A6L);
writeKernelMemory((void *)0x04CC37B0,0x4E800421L);
writeKernelMemory((void *)0x04CC37B4,0x48002A45L);
writeKernelMemory((void *)0x04CC37B8,0xC0220020L);
writeKernelMemory((void *)0x04CC37BC,0x48002A81L);
writeKernelMemory((void *)0x04CC37C0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC37C4,0x4E800421L);
writeKernelMemory((void *)0x04CC37C8,0x3882016AL);
writeKernelMemory((void *)0x04CC37CC,0x38620348L);
writeKernelMemory((void *)0x04CC37D0,0x7F4903A6L);
writeKernelMemory((void *)0x04CC37D4,0x4E800421L);
writeKernelMemory((void *)0x04CC37D8,0x48002A49L);
writeKernelMemory((void *)0x04CC37DC,0x7F8903A6L);
writeKernelMemory((void *)0x04CC37E0,0x4E800421L);
writeKernelMemory((void *)0x04CC37E4,0x39600B30L);
writeKernelMemory((void *)0x04CC37E8,0x8182FB5CL);
writeKernelMemory((void *)0x04CC37EC,0x2C0C0001L);
writeKernelMemory((void *)0x04CC37F0,0x3D4043E0L);
writeKernelMemory((void *)0x04CC37F4,0x41820451L);
writeKernelMemory((void *)0x04CC37F8,0x8182FB5CL);
writeKernelMemory((void *)0x04CC37FC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3800,0x3D404400L);
writeKernelMemory((void *)0x04CC3804,0x41820471L);
writeKernelMemory((void *)0x04CC3808,0x81820B30L);
writeKernelMemory((void *)0x04CC380C,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3810,0x4080003CL);
writeKernelMemory((void *)0x04CC3814,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3818,0x4E800421L);
writeKernelMemory((void *)0x04CC381C,0x480029DDL);
writeKernelMemory((void *)0x04CC3820,0xC0220B30L);
writeKernelMemory((void *)0x04CC3824,0x48002A19L);
writeKernelMemory((void *)0x04CC3828,0x7FA903A6L);
writeKernelMemory((void *)0x04CC382C,0x4E800421L);
writeKernelMemory((void *)0x04CC3830,0x38620348L);
writeKernelMemory((void *)0x04CC3834,0x388201A6L);
writeKernelMemory((void *)0x04CC3838,0x7F4903A6L);
writeKernelMemory((void *)0x04CC383C,0x4E800421L);
writeKernelMemory((void *)0x04CC3840,0x480029E1L);
writeKernelMemory((void *)0x04CC3844,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3848,0x4E800421L);
writeKernelMemory((void *)0x04CC384C,0x396001C8L);
writeKernelMemory((void *)0x04CC3850,0x8182FB58L);
writeKernelMemory((void *)0x04CC3854,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3858,0x3D4043E5L);
writeKernelMemory((void *)0x04CC385C,0x418203E9L);
writeKernelMemory((void *)0x04CC3860,0x8182FB58L);
writeKernelMemory((void *)0x04CC3864,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3868,0x3D404400L);
writeKernelMemory((void *)0x04CC386C,0x41820409L);
writeKernelMemory((void *)0x04CC3870,0x818201C8L);
writeKernelMemory((void *)0x04CC3874,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3878,0x4080003CL);
writeKernelMemory((void *)0x04CC387C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3880,0x4E800421L);
writeKernelMemory((void *)0x04CC3884,0x48002975L);
writeKernelMemory((void *)0x04CC3888,0xC02201C8L);
writeKernelMemory((void *)0x04CC388C,0x480029B1L);
writeKernelMemory((void *)0x04CC3890,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3894,0x4E800421L);
writeKernelMemory((void *)0x04CC3898,0x38620348L);
writeKernelMemory((void *)0x04CC389C,0x388201D6L);
writeKernelMemory((void *)0x04CC38A0,0x7F4903A6L);
writeKernelMemory((void *)0x04CC38A4,0x4E800421L);
writeKernelMemory((void *)0x04CC38A8,0x48002979L);
writeKernelMemory((void *)0x04CC38AC,0x7F8903A6L);
writeKernelMemory((void *)0x04CC38B0,0x4E800421L);
writeKernelMemory((void *)0x04CC38B4,0x39600010L);
writeKernelMemory((void *)0x04CC38B8,0x8182FB54L);
writeKernelMemory((void *)0x04CC38BC,0x2C0C0001L);
writeKernelMemory((void *)0x04CC38C0,0x3D4043EDL);
writeKernelMemory((void *)0x04CC38C4,0x41820381L);
writeKernelMemory((void *)0x04CC38C8,0x8182FB54L);
writeKernelMemory((void *)0x04CC38CC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC38D0,0x3D404400L);
writeKernelMemory((void *)0x04CC38D4,0x418203A1L);
writeKernelMemory((void *)0x04CC38D8,0x81820010L);
writeKernelMemory((void *)0x04CC38DC,0x7C0C5000L);
writeKernelMemory((void *)0x04CC38E0,0x4080003CL);
writeKernelMemory((void *)0x04CC38E4,0x7EC903A6L);
writeKernelMemory((void *)0x04CC38E8,0x4E800421L);
writeKernelMemory((void *)0x04CC38EC,0x4800290DL);
writeKernelMemory((void *)0x04CC38F0,0xC0220010L);
writeKernelMemory((void *)0x04CC38F4,0x48002949L);
writeKernelMemory((void *)0x04CC38F8,0x7FA903A6L);
writeKernelMemory((void *)0x04CC38FC,0x4E800421L);
writeKernelMemory((void *)0x04CC3900,0x38620348L);
writeKernelMemory((void *)0x04CC3904,0x38820236L);
writeKernelMemory((void *)0x04CC3908,0x7F4903A6L);
writeKernelMemory((void *)0x04CC390C,0x4E800421L);
writeKernelMemory((void *)0x04CC3910,0x48002911L);
writeKernelMemory((void *)0x04CC3914,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3918,0x4E800421L);
writeKernelMemory((void *)0x04CC391C,0x39600018L);
writeKernelMemory((void *)0x04CC3920,0x8182FB50L);
writeKernelMemory((void *)0x04CC3924,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3928,0x3D4043EAL);
writeKernelMemory((void *)0x04CC392C,0x41820319L);
writeKernelMemory((void *)0x04CC3930,0x8182FB50L);
writeKernelMemory((void *)0x04CC3934,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3938,0x3D404400L);
writeKernelMemory((void *)0x04CC393C,0x41820339L);
writeKernelMemory((void *)0x04CC3940,0x81820018L);
writeKernelMemory((void *)0x04CC3944,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3948,0x4080003CL);
writeKernelMemory((void *)0x04CC394C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3950,0x4E800421L);
writeKernelMemory((void *)0x04CC3954,0x480028A5L);
writeKernelMemory((void *)0x04CC3958,0xC0220018L);
writeKernelMemory((void *)0x04CC395C,0x480028E1L);
writeKernelMemory((void *)0x04CC3960,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3964,0x4E800421L);
writeKernelMemory((void *)0x04CC3968,0x38620348L);
writeKernelMemory((void *)0x04CC396C,0x38820286L);
writeKernelMemory((void *)0x04CC3970,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3974,0x4E800421L);
writeKernelMemory((void *)0x04CC3978,0x480028A9L);
writeKernelMemory((void *)0x04CC397C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3980,0x4E800421L);
writeKernelMemory((void *)0x04CC3984,0x39600300L);
writeKernelMemory((void *)0x04CC3988,0x8182FB4CL);
writeKernelMemory((void *)0x04CC398C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3990,0x3D4043EBL);
writeKernelMemory((void *)0x04CC3994,0x418202B1L);
writeKernelMemory((void *)0x04CC3998,0x8182FB4CL);
writeKernelMemory((void *)0x04CC399C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC39A0,0x3D404400L);
writeKernelMemory((void *)0x04CC39A4,0x418202D1L);
writeKernelMemory((void *)0x04CC39A8,0x81820300L);
writeKernelMemory((void *)0x04CC39AC,0x7C0C5000L);
writeKernelMemory((void *)0x04CC39B0,0x4080003CL);
writeKernelMemory((void *)0x04CC39B4,0x7EC903A6L);
writeKernelMemory((void *)0x04CC39B8,0x4E800421L);
writeKernelMemory((void *)0x04CC39BC,0x4800283DL);
writeKernelMemory((void *)0x04CC39C0,0xC0220300L);
writeKernelMemory((void *)0x04CC39C4,0x48002879L);
writeKernelMemory((void *)0x04CC39C8,0x7FA903A6L);
writeKernelMemory((void *)0x04CC39CC,0x4E800421L);
writeKernelMemory((void *)0x04CC39D0,0x38620348L);
writeKernelMemory((void *)0x04CC39D4,0x388202A6L);
writeKernelMemory((void *)0x04CC39D8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC39DC,0x4E800421L);
writeKernelMemory((void *)0x04CC39E0,0x48002841L);
writeKernelMemory((void *)0x04CC39E4,0x7F8903A6L);
writeKernelMemory((void *)0x04CC39E8,0x4E800421L);
writeKernelMemory((void *)0x04CC39EC,0x39600308L);
writeKernelMemory((void *)0x04CC39F0,0x8182FB48L);
writeKernelMemory((void *)0x04CC39F4,0x2C0C0001L);
writeKernelMemory((void *)0x04CC39F8,0x3D4043ECL);
writeKernelMemory((void *)0x04CC39FC,0x41820249L);
writeKernelMemory((void *)0x04CC3A00,0x8182FB48L);
writeKernelMemory((void *)0x04CC3A04,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3A08,0x3D404400L);
writeKernelMemory((void *)0x04CC3A0C,0x41820269L);
writeKernelMemory((void *)0x04CC3A10,0x81820308L);
writeKernelMemory((void *)0x04CC3A14,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3A18,0x4080003CL);
writeKernelMemory((void *)0x04CC3A1C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3A20,0x4E800421L);
writeKernelMemory((void *)0x04CC3A24,0x480027D5L);
writeKernelMemory((void *)0x04CC3A28,0xC0220308L);
writeKernelMemory((void *)0x04CC3A2C,0x48002811L);
writeKernelMemory((void *)0x04CC3A30,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3A34,0x4E800421L);
writeKernelMemory((void *)0x04CC3A38,0x38620348L);
writeKernelMemory((void *)0x04CC3A3C,0x388202C6L);
writeKernelMemory((void *)0x04CC3A40,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3A44,0x4E800421L);
writeKernelMemory((void *)0x04CC3A48,0x480027D9L);
writeKernelMemory((void *)0x04CC3A4C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3A50,0x4E800421L);
writeKernelMemory((void *)0x04CC3A54,0x39600310L);
writeKernelMemory((void *)0x04CC3A58,0x8182FB44L);
writeKernelMemory((void *)0x04CC3A5C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3A60,0x3D4043EAL);
writeKernelMemory((void *)0x04CC3A64,0x418201E1L);
writeKernelMemory((void *)0x04CC3A68,0x8182FB44L);
writeKernelMemory((void *)0x04CC3A6C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3A70,0x3D404400L);
writeKernelMemory((void *)0x04CC3A74,0x41820201L);
writeKernelMemory((void *)0x04CC3A78,0x81820310L);
writeKernelMemory((void *)0x04CC3A7C,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3A80,0x4080003CL);
writeKernelMemory((void *)0x04CC3A84,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3A88,0x4E800421L);
writeKernelMemory((void *)0x04CC3A8C,0x4800276DL);
writeKernelMemory((void *)0x04CC3A90,0xC0220310L);
writeKernelMemory((void *)0x04CC3A94,0x480027A9L);
writeKernelMemory((void *)0x04CC3A98,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3A9C,0x4E800421L);
writeKernelMemory((void *)0x04CC3AA0,0x38620348L);
writeKernelMemory((void *)0x04CC3AA4,0x388202E6L);
writeKernelMemory((void *)0x04CC3AA8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3AAC,0x4E800421L);
writeKernelMemory((void *)0x04CC3AB0,0x48002771L);
writeKernelMemory((void *)0x04CC3AB4,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3AB8,0x4E800421L);
writeKernelMemory((void *)0x04CC3ABC,0x39600318L);
writeKernelMemory((void *)0x04CC3AC0,0x8182FB40L);
writeKernelMemory((void *)0x04CC3AC4,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3AC8,0x3D4043E2L);
writeKernelMemory((void *)0x04CC3ACC,0x41820179L);
writeKernelMemory((void *)0x04CC3AD0,0x8182FB40L);
writeKernelMemory((void *)0x04CC3AD4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3AD8,0x3D404400L);
writeKernelMemory((void *)0x04CC3ADC,0x41820199L);
writeKernelMemory((void *)0x04CC3AE0,0x81820318L);
writeKernelMemory((void *)0x04CC3AE4,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3AE8,0x4080003CL);
writeKernelMemory((void *)0x04CC3AEC,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3AF0,0x4E800421L);
writeKernelMemory((void *)0x04CC3AF4,0x48002705L);
writeKernelMemory((void *)0x04CC3AF8,0xC0220318L);
writeKernelMemory((void *)0x04CC3AFC,0x48002741L);
writeKernelMemory((void *)0x04CC3B00,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3B04,0x4E800421L);
writeKernelMemory((void *)0x04CC3B08,0x38620348L);
writeKernelMemory((void *)0x04CC3B0C,0x38820376L);
writeKernelMemory((void *)0x04CC3B10,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3B14,0x4E800421L);
writeKernelMemory((void *)0x04CC3B18,0x48002709L);
writeKernelMemory((void *)0x04CC3B1C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3B20,0x4E800421L);
writeKernelMemory((void *)0x04CC3B24,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3B28,0x4E800421L);
writeKernelMemory((void *)0x04CC3B2C,0x480026CDL);
writeKernelMemory((void *)0x04CC3B30,0x3D804100L);
writeKernelMemory((void *)0x04CC3B34,0x91820420L);
writeKernelMemory((void *)0x04CC3B38,0xC0220420L);
writeKernelMemory((void *)0x04CC3B3C,0x3D8041E0L);
writeKernelMemory((void *)0x04CC3B40,0x91820420L);
writeKernelMemory((void *)0x04CC3B44,0xC0420420L);
writeKernelMemory((void *)0x04CC3B48,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3B4C,0x4E800421L);
writeKernelMemory((void *)0x04CC3B50,0x38820AC0L);
writeKernelMemory((void *)0x04CC3B54,0x38620348L);
writeKernelMemory((void *)0x04CC3B58,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3B5C,0x4E800421L);
writeKernelMemory((void *)0x04CC3B60,0x480026C1L);
writeKernelMemory((void *)0x04CC3B64,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3B68,0x4E800421L);
writeKernelMemory((void *)0x04CC3B6C,0x81990034L);
writeKernelMemory((void *)0x04CC3B70,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3B74,0x41820060L);
writeKernelMemory((void *)0x04CC3B78,0x7EC903A6L);
writeKernelMemory((void *)0x04CC3B7C,0x4E800421L);
writeKernelMemory((void *)0x04CC3B80,0x3D803F80L);
writeKernelMemory((void *)0x04CC3B84,0x91820418L);
writeKernelMemory((void *)0x04CC3B88,0xC0220418L);
writeKernelMemory((void *)0x04CC3B8C,0xC0420418L);
writeKernelMemory((void *)0x04CC3B90,0x7EE903A6L);
writeKernelMemory((void *)0x04CC3B94,0x4E800421L);
writeKernelMemory((void *)0x04CC3B98,0x3D80434CL);
writeKernelMemory((void *)0x04CC3B9C,0x91820424L);
writeKernelMemory((void *)0x04CC3BA0,0xC0220424L);
writeKernelMemory((void *)0x04CC3BA4,0x3D804100L);
writeKernelMemory((void *)0x04CC3BA8,0x9182FEB0L);
writeKernelMemory((void *)0x04CC3BAC,0xC042FEB0L);
writeKernelMemory((void *)0x04CC3BB0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC3BB4,0x4E800421L);
writeKernelMemory((void *)0x04CC3BB8,0x38820B00L);
writeKernelMemory((void *)0x04CC3BBC,0x38620348L);
writeKernelMemory((void *)0x04CC3BC0,0x7F4903A6L);
writeKernelMemory((void *)0x04CC3BC4,0x4E800421L);
writeKernelMemory((void *)0x04CC3BC8,0x48002659L);
writeKernelMemory((void *)0x04CC3BCC,0x7F8903A6L);
writeKernelMemory((void *)0x04CC3BD0,0x4E800421L);
writeKernelMemory((void *)0x04CC3BD4,0x48002724L);
writeKernelMemory((void *)0x04CC3BD8,0x3D8010A1L);
writeKernelMemory((void *)0x04CC3BDC,0x818C9370L);
writeKernelMemory((void *)0x04CC3BE0,0x3D200100L);
writeKernelMemory((void *)0x04CC3BE4,0x7C0C4800L);
writeKernelMemory((void *)0x04CC3BE8,0x4182274CL);
writeKernelMemory((void *)0x04CC3BEC,0x39200001L);
writeKernelMemory((void *)0x04CC3BF0,0x91220A14L);
writeKernelMemory((void *)0x04CC3BF4,0x81590034L);
writeKernelMemory((void *)0x04CC3BF8,0x2C0A0000L);
writeKernelMemory((void *)0x04CC3BFC,0x41820014L);
writeKernelMemory((void *)0x04CC3C00,0x816A0148L);
writeKernelMemory((void *)0x04CC3C04,0x9162FE00L);
writeKernelMemory((void *)0x04CC3C08,0x816A014CL);
writeKernelMemory((void *)0x04CC3C0C,0x9162FDFCL);
writeKernelMemory((void *)0x04CC3C10,0x9122FCFCL);
writeKernelMemory((void *)0x04CC3C14,0x4E800020L);
writeKernelMemory((void *)0x04CC3C18,0x39200000L);
writeKernelMemory((void *)0x04CC3C1C,0x91220A14L);
writeKernelMemory((void *)0x04CC3C20,0x39400000L);
writeKernelMemory((void *)0x04CC3C24,0x9142FCF0L);
writeKernelMemory((void *)0x04CC3C28,0x91421110L);
writeKernelMemory((void *)0x04CC3C2C,0x81590034L);
writeKernelMemory((void *)0x04CC3C30,0x2C0A0000L);
writeKernelMemory((void *)0x04CC3C34,0x4182000CL);
writeKernelMemory((void *)0x04CC3C38,0x39600000L);
writeKernelMemory((void *)0x04CC3C3C,0x916A0814L);
writeKernelMemory((void *)0x04CC3C40,0x4E800020L);
writeKernelMemory((void *)0x04CC3C44,0x7D8B102EL);
writeKernelMemory((void *)0x04CC3C48,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3C4C,0x408126E8L);
writeKernelMemory((void *)0x04CC3C50,0x3D804094L);
writeKernelMemory((void *)0x04CC3C54,0x398C2000L);
writeKernelMemory((void *)0x04CC3C58,0x392B0004L);
writeKernelMemory((void *)0x04CC3C5C,0x7D89112EL);
writeKernelMemory((void *)0x04CC3C60,0x7DA9142EL);
writeKernelMemory((void *)0x04CC3C64,0x7DCB142EL);
writeKernelMemory((void *)0x04CC3C68,0xFDCE6828L);
writeKernelMemory((void *)0x04CC3C6C,0x7DCB152EL);
writeKernelMemory((void *)0x04CC3C70,0x4E800020L);
writeKernelMemory((void *)0x04CC3C74,0x7D8B102EL);
writeKernelMemory((void *)0x04CC3C78,0x7C0C5000L);
writeKernelMemory((void *)0x04CC3C7C,0x408026B8L);
writeKernelMemory((void *)0x04CC3C80,0x3D804094L);
writeKernelMemory((void *)0x04CC3C84,0x398C2000L);
writeKernelMemory((void *)0x04CC3C88,0x392B0004L);
writeKernelMemory((void *)0x04CC3C8C,0x7D89112EL);
writeKernelMemory((void *)0x04CC3C90,0x7DA9142EL);
writeKernelMemory((void *)0x04CC3C94,0x7DCB142EL);
writeKernelMemory((void *)0x04CC3C98,0xFDCE682AL);
writeKernelMemory((void *)0x04CC3C9C,0x7DCB152EL);
writeKernelMemory((void *)0x04CC3CA0,0x4E800020L);
writeKernelMemory((void *)0x04CC3CA4,0x81990034L);
writeKernelMemory((void *)0x04CC3CA8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3CAC,0x41820020L);
writeKernelMemory((void *)0x04CC3CB0,0x3D600100L);
writeKernelMemory((void *)0x04CC3CB4,0x916C0814L);
writeKernelMemory((void *)0x04CC3CB8,0x8162FE00L);
writeKernelMemory((void *)0x04CC3CBC,0x916C0148L);
writeKernelMemory((void *)0x04CC3CC0,0x8162FDFCL);
writeKernelMemory((void *)0x04CC3CC4,0x916C014CL);
writeKernelMemory((void *)0x04CC3CC8,0x80790144L);
writeKernelMemory((void *)0x04CC3CCC,0x3D80108DL);
writeKernelMemory((void *)0x04CC3CD0,0x3D20C5A0L);
writeKernelMemory((void *)0x04CC3CD4,0x912CB504L);
writeKernelMemory((void *)0x04CC3CD8,0x3D80102FL);
writeKernelMemory((void *)0x04CC3CDC,0x814CFA63L);
writeKernelMemory((void *)0x04CC3CE0,0x2C0A0080L);
writeKernelMemory((void *)0x04CC3CE4,0x41820038L);
writeKernelMemory((void *)0x04CC3CE8,0x728A0100L);
writeKernelMemory((void *)0x04CC3CEC,0x2C0A0100L);
writeKernelMemory((void *)0x04CC3CF0,0x418203A0L);
writeKernelMemory((void *)0x04CC3CF4,0x728A0200L);
writeKernelMemory((void *)0x04CC3CF8,0x2C0A0200L);
writeKernelMemory((void *)0x04CC3CFC,0x41820404L);
writeKernelMemory((void *)0x04CC3D00,0x728A0800L);
writeKernelMemory((void *)0x04CC3D04,0x2C0A0800L);
writeKernelMemory((void *)0x04CC3D08,0x41820468L);
writeKernelMemory((void *)0x04CC3D0C,0x728A0400L);
writeKernelMemory((void *)0x04CC3D10,0x2C0A0400L);
writeKernelMemory((void *)0x04CC3D14,0x418205C4L);
writeKernelMemory((void *)0x04CC3D18,0x48000738L);
writeKernelMemory((void *)0x04CC3D1C,0x8182FCFCL);
writeKernelMemory((void *)0x04CC3D20,0x2C0C0001L);
writeKernelMemory((void *)0x04CC3D24,0x41820360L);
writeKernelMemory((void *)0x04CC3D28,0x8182FD00L);
writeKernelMemory((void *)0x04CC3D2C,0x8142FCF8L);
writeKernelMemory((void *)0x04CC3D30,0x1D6CFFFCL);
writeKernelMemory((void *)0x04CC3D34,0x396BFD60L);
writeKernelMemory((void *)0x04CC3D38,0x1D4AFF00L);
writeKernelMemory((void *)0x04CC3D3C,0x7D6B5214L);
writeKernelMemory((void *)0x04CC3D40,0x7D2B102EL);
writeKernelMemory((void *)0x04CC3D44,0x2C090000L);
writeKernelMemory((void *)0x04CC3D48,0x418201E4L);
writeKernelMemory((void *)0x04CC3D4C,0x39200000L);
writeKernelMemory((void *)0x04CC3D50,0x7D2B112EL);
writeKernelMemory((void *)0x04CC3D54,0x2C0BFB40L);
writeKernelMemory((void *)0x04CC3D58,0x40820010L);
writeKernelMemory((void *)0x04CC3D5C,0x3D803F80L);
writeKernelMemory((void *)0x04CC3D60,0x3D20105EL);
writeKernelMemory((void *)0x04CC3D64,0x9189B0A0L);
writeKernelMemory((void *)0x04CC3D68,0x2C0BFD3CL);
writeKernelMemory((void *)0x04CC3D6C,0x4082002CL);
writeKernelMemory((void *)0x04CC3D70,0x81990034L);
writeKernelMemory((void *)0x04CC3D74,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3D78,0x41820020L);
writeKernelMemory((void *)0x04CC3D7C,0x3D203CA3L);
writeKernelMemory((void *)0x04CC3D80,0x6129D70AL);
writeKernelMemory((void *)0x04CC3D84,0x912C0734L);
writeKernelMemory((void *)0x04CC3D88,0x3D801067L);
writeKernelMemory((void *)0x04CC3D8C,0x3D203E26L);
writeKernelMemory((void *)0x04CC3D90,0x6129AD89L);
writeKernelMemory((void *)0x04CC3D94,0x912CACC8L);
writeKernelMemory((void *)0x04CC3D98,0x2C0BFD30L);
writeKernelMemory((void *)0x04CC3D9C,0x40820010L);
writeKernelMemory((void *)0x04CC3DA0,0x81990034L);
writeKernelMemory((void *)0x04CC3DA4,0x39200000L);
writeKernelMemory((void *)0x04CC3DA8,0x912C01DCL);
writeKernelMemory((void *)0x04CC3DAC,0x2C0BFB60L);
writeKernelMemory((void *)0x04CC3DB0,0x40820014L);
writeKernelMemory((void *)0x04CC3DB4,0x3D801090L);
writeKernelMemory((void *)0x04CC3DB8,0x3D200271L);
writeKernelMemory((void *)0x04CC3DBC,0x6129D184L);
writeKernelMemory((void *)0x04CC3DC0,0x912CACB0L);
writeKernelMemory((void *)0x04CC3DC4,0x2C0BFC54L);
writeKernelMemory((void *)0x04CC3DC8,0x40820160L);
writeKernelMemory((void *)0x04CC3DCC,0x81990034L);
writeKernelMemory((void *)0x04CC3DD0,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3DD4,0x41820154L);
writeKernelMemory((void *)0x04CC3DD8,0x38600001L);
writeKernelMemory((void *)0x04CC3DDC,0x3D800248L);
writeKernelMemory((void *)0x04CC3DE0,0x618C6698L);
writeKernelMemory((void *)0x04CC3DE4,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3DE8,0x4E800421L);
writeKernelMemory((void *)0x04CC3DEC,0x7C641B78L);
writeKernelMemory((void *)0x04CC3DF0,0x38600000L);
writeKernelMemory((void *)0x04CC3DF4,0x38A00000L);
writeKernelMemory((void *)0x04CC3DF8,0x3D800248L);
writeKernelMemory((void *)0x04CC3DFC,0x618C61BCL);
writeKernelMemory((void *)0x04CC3E00,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3E04,0x4E800421L);
writeKernelMemory((void *)0x04CC3E08,0x90620E78L);
writeKernelMemory((void *)0x04CC3E0C,0x38600000L);
writeKernelMemory((void *)0x04CC3E10,0x38800005L);
writeKernelMemory((void *)0x04CC3E14,0x38A20E78L);
writeKernelMemory((void *)0x04CC3E18,0x3D80028BL);
writeKernelMemory((void *)0x04CC3E1C,0x618C09F8L);
writeKernelMemory((void *)0x04CC3E20,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3E24,0x4E800421L);
writeKernelMemory((void *)0x04CC3E28,0x48002479L);
writeKernelMemory((void *)0x04CC3E2C,0x38600001L);
writeKernelMemory((void *)0x04CC3E30,0x3D800248L);
writeKernelMemory((void *)0x04CC3E34,0x618C6698L);
writeKernelMemory((void *)0x04CC3E38,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3E3C,0x4E800421L);
writeKernelMemory((void *)0x04CC3E40,0x7C641B78L);
writeKernelMemory((void *)0x04CC3E44,0x38600000L);
writeKernelMemory((void *)0x04CC3E48,0x38A00000L);
writeKernelMemory((void *)0x04CC3E4C,0x3D800248L);
writeKernelMemory((void *)0x04CC3E50,0x618C61BCL);
writeKernelMemory((void *)0x04CC3E54,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3E58,0x4E800421L);
writeKernelMemory((void *)0x04CC3E5C,0x90620E78L);
writeKernelMemory((void *)0x04CC3E60,0x38600000L);
writeKernelMemory((void *)0x04CC3E64,0x38800006L);
writeKernelMemory((void *)0x04CC3E68,0x38A20E78L);
writeKernelMemory((void *)0x04CC3E6C,0x3D80028BL);
writeKernelMemory((void *)0x04CC3E70,0x618C09F8L);
writeKernelMemory((void *)0x04CC3E74,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3E78,0x4E800421L);
writeKernelMemory((void *)0x04CC3E7C,0x48002425L);
writeKernelMemory((void *)0x04CC3E80,0x38600001L);
writeKernelMemory((void *)0x04CC3E84,0x3D800248L);
writeKernelMemory((void *)0x04CC3E88,0x618C6698L);
writeKernelMemory((void *)0x04CC3E8C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3E90,0x4E800421L);
writeKernelMemory((void *)0x04CC3E94,0x7C641B78L);
writeKernelMemory((void *)0x04CC3E98,0x38600000L);
writeKernelMemory((void *)0x04CC3E9C,0x38A00000L);
writeKernelMemory((void *)0x04CC3EA0,0x3D800248L);
writeKernelMemory((void *)0x04CC3EA4,0x618C61BCL);
writeKernelMemory((void *)0x04CC3EA8,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3EAC,0x4E800421L);
writeKernelMemory((void *)0x04CC3EB0,0x90620E78L);
writeKernelMemory((void *)0x04CC3EB4,0x38600000L);
writeKernelMemory((void *)0x04CC3EB8,0x38800007L);
writeKernelMemory((void *)0x04CC3EBC,0x38A20E78L);
writeKernelMemory((void *)0x04CC3EC0,0x3D80028BL);
writeKernelMemory((void *)0x04CC3EC4,0x618C09F8L);
writeKernelMemory((void *)0x04CC3EC8,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3ECC,0x4E800421L);
writeKernelMemory((void *)0x04CC3ED0,0x480023D1L);
writeKernelMemory((void *)0x04CC3ED4,0x38600001L);
writeKernelMemory((void *)0x04CC3ED8,0x3D800248L);
writeKernelMemory((void *)0x04CC3EDC,0x618C6698L);
writeKernelMemory((void *)0x04CC3EE0,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3EE4,0x4E800421L);
writeKernelMemory((void *)0x04CC3EE8,0x7C641B78L);
writeKernelMemory((void *)0x04CC3EEC,0x38600000L);
writeKernelMemory((void *)0x04CC3EF0,0x38A00000L);
writeKernelMemory((void *)0x04CC3EF4,0x3D800248L);
writeKernelMemory((void *)0x04CC3EF8,0x618C61BCL);
writeKernelMemory((void *)0x04CC3EFC,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3F00,0x4E800421L);
writeKernelMemory((void *)0x04CC3F04,0x90620E78L);
writeKernelMemory((void *)0x04CC3F08,0x38600000L);
writeKernelMemory((void *)0x04CC3F0C,0x38800008L);
writeKernelMemory((void *)0x04CC3F10,0x38A20E78L);
writeKernelMemory((void *)0x04CC3F14,0x3D80028BL);
writeKernelMemory((void *)0x04CC3F18,0x618C09F8L);
writeKernelMemory((void *)0x04CC3F1C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC3F20,0x4E800421L);
writeKernelMemory((void *)0x04CC3F24,0x4800237DL);
writeKernelMemory((void *)0x04CC3F28,0x4800015CL);
writeKernelMemory((void *)0x04CC3F2C,0x39200001L);
writeKernelMemory((void *)0x04CC3F30,0x7D2B112EL);
writeKernelMemory((void *)0x04CC3F34,0x2C0BFB40L);
writeKernelMemory((void *)0x04CC3F38,0x40820010L);
writeKernelMemory((void *)0x04CC3F3C,0x39800000L);
writeKernelMemory((void *)0x04CC3F40,0x3D20105EL);
writeKernelMemory((void *)0x04CC3F44,0x9189B0A0L);
writeKernelMemory((void *)0x04CC3F48,0x39800000L);
writeKernelMemory((void *)0x04CC3F4C,0x2C0BFB54L);
writeKernelMemory((void *)0x04CC3F50,0x40820008L);
writeKernelMemory((void *)0x04CC3F54,0x9182FB50L);
writeKernelMemory((void *)0x04CC3F58,0x2C0BFB50L);
writeKernelMemory((void *)0x04CC3F5C,0x40820008L);
writeKernelMemory((void *)0x04CC3F60,0x9182FB54L);
writeKernelMemory((void *)0x04CC3F64,0x39800000L);
writeKernelMemory((void *)0x04CC3F68,0x2C0BFB4CL);
writeKernelMemory((void *)0x04CC3F6C,0x4082000CL);
writeKernelMemory((void *)0x04CC3F70,0x9182FB48L);
writeKernelMemory((void *)0x04CC3F74,0x9182FB44L);
writeKernelMemory((void *)0x04CC3F78,0x2C0BFB48L);
writeKernelMemory((void *)0x04CC3F7C,0x4082000CL);
writeKernelMemory((void *)0x04CC3F80,0x9182FB4CL);
writeKernelMemory((void *)0x04CC3F84,0x9182FB44L);
writeKernelMemory((void *)0x04CC3F88,0x2C0BFB44L);
writeKernelMemory((void *)0x04CC3F8C,0x4082000CL);
writeKernelMemory((void *)0x04CC3F90,0x9182FB4CL);
writeKernelMemory((void *)0x04CC3F94,0x9182FB48L);
writeKernelMemory((void *)0x04CC3F98,0x2C0BFD3CL);
writeKernelMemory((void *)0x04CC3F9C,0x40820024L);
writeKernelMemory((void *)0x04CC3FA0,0x81990034L);
writeKernelMemory((void *)0x04CC3FA4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC3FA8,0x41820018L);
writeKernelMemory((void *)0x04CC3FAC,0x3D203E50L);
writeKernelMemory((void *)0x04CC3FB0,0x912C0734L);
writeKernelMemory((void *)0x04CC3FB4,0x3D801067L);
writeKernelMemory((void *)0x04CC3FB8,0x3D203F60L);
writeKernelMemory((void *)0x04CC3FBC,0x912CACC8L);
writeKernelMemory((void *)0x04CC3FC0,0x2C0BFB60L);
writeKernelMemory((void *)0x04CC3FC4,0x40820014L);
writeKernelMemory((void *)0x04CC3FC8,0x3D801090L);
writeKernelMemory((void *)0x04CC3FCC,0x3D200272L);
writeKernelMemory((void *)0x04CC3FD0,0x6129CF00L);
writeKernelMemory((void *)0x04CC3FD4,0x912CACB0L);
writeKernelMemory((void *)0x04CC3FD8,0x2C0BFC50L);
writeKernelMemory((void *)0x04CC3FDC,0x40820018L);
writeKernelMemory((void *)0x04CC3FE0,0x3C80109CL);
writeKernelMemory((void *)0x04CC3FE4,0x8084684CL);
writeKernelMemory((void *)0x04CC3FE8,0x480022D9L);
writeKernelMemory((void *)0x04CC3FEC,0x39200002L);
writeKernelMemory((void *)0x04CC3FF0,0x91220A14L);
writeKernelMemory((void *)0x04CC3FF4,0x2C0BFC4CL);
writeKernelMemory((void *)0x04CC3FF8,0x40820018L);
writeKernelMemory((void *)0x04CC3FFC,0x3C80109CL);
writeKernelMemory((void *)0x04CC4000,0x8084684CL);
writeKernelMemory((void *)0x04CC4004,0x480022BDL);
writeKernelMemory((void *)0x04CC4008,0x39200003L);
writeKernelMemory((void *)0x04CC400C,0x91220A14L);
writeKernelMemory((void *)0x04CC4010,0x2C0BFC60L);
writeKernelMemory((void *)0x04CC4014,0x40820010L);
writeKernelMemory((void *)0x04CC4018,0x3C80109CL);
writeKernelMemory((void *)0x04CC401C,0x80846350L);
writeKernelMemory((void *)0x04CC4020,0x480022A1L);
writeKernelMemory((void *)0x04CC4024,0x2C0BFC5CL);
writeKernelMemory((void *)0x04CC4028,0x40820010L);
writeKernelMemory((void *)0x04CC402C,0x3C80109CL);
writeKernelMemory((void *)0x04CC4030,0x80846350L);
writeKernelMemory((void *)0x04CC4034,0x4800228DL);
writeKernelMemory((void *)0x04CC4038,0x39200000L);
writeKernelMemory((void *)0x04CC403C,0x2C0BFD38L);
writeKernelMemory((void *)0x04CC4040,0x40820018L);
writeKernelMemory((void *)0x04CC4044,0x9122FD34L);
writeKernelMemory((void *)0x04CC4048,0x9122FD30L);
writeKernelMemory((void *)0x04CC404C,0x81990034L);
writeKernelMemory((void *)0x04CC4050,0x39200000L);
writeKernelMemory((void *)0x04CC4054,0x912C01DCL);
writeKernelMemory((void *)0x04CC4058,0x2C0BFD34L);
writeKernelMemory((void *)0x04CC405C,0x40820018L);
writeKernelMemory((void *)0x04CC4060,0x9122FD38L);
writeKernelMemory((void *)0x04CC4064,0x9122FD30L);
writeKernelMemory((void *)0x04CC4068,0x81990034L);
writeKernelMemory((void *)0x04CC406C,0x39200000L);
writeKernelMemory((void *)0x04CC4070,0x912C01DCL);
writeKernelMemory((void *)0x04CC4074,0x2C0BFD30L);
writeKernelMemory((void *)0x04CC4078,0x4082000CL);
writeKernelMemory((void *)0x04CC407C,0x9122FD38L);
writeKernelMemory((void *)0x04CC4080,0x9122FD34L);
writeKernelMemory((void *)0x04CC4084,0x39200001L);
writeKernelMemory((void *)0x04CC4088,0x9122FCFCL);
writeKernelMemory((void *)0x04CC408C,0x480003CCL);
writeKernelMemory((void *)0x04CC4090,0x8182FCFCL);
writeKernelMemory((void *)0x04CC4094,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4098,0x418203C0L);
writeKernelMemory((void *)0x04CC409C,0x8182FCF8L);
writeKernelMemory((void *)0x04CC40A0,0x2C0C0002L);
writeKernelMemory((void *)0x04CC40A4,0x41820010L);
writeKernelMemory((void *)0x04CC40A8,0x8182FCF8L);
writeKernelMemory((void *)0x04CC40AC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC40B0,0x40820018L);
writeKernelMemory((void *)0x04CC40B4,0x8182FD00L);
writeKernelMemory((void *)0x04CC40B8,0x2C0C000CL);
writeKernelMemory((void *)0x04CC40BC,0x4082000CL);
writeKernelMemory((void *)0x04CC40C0,0x3980FFFFL);
writeKernelMemory((void *)0x04CC40C4,0x9182FD00L);
writeKernelMemory((void *)0x04CC40C8,0x8182FCF8L);
writeKernelMemory((void *)0x04CC40CC,0x2C0C0001L);
writeKernelMemory((void *)0x04CC40D0,0x40820018L);
writeKernelMemory((void *)0x04CC40D4,0x8182FD00L);
writeKernelMemory((void *)0x04CC40D8,0x2C0C0009L);
writeKernelMemory((void *)0x04CC40DC,0x4082000CL);
writeKernelMemory((void *)0x04CC40E0,0x3980FFFFL);
writeKernelMemory((void *)0x04CC40E4,0x9182FD00L);
writeKernelMemory((void *)0x04CC40E8,0x8182FD00L);
writeKernelMemory((void *)0x04CC40EC,0x398C0001L);
writeKernelMemory((void *)0x04CC40F0,0x9182FD00L);
writeKernelMemory((void *)0x04CC40F4,0x39800001L);
writeKernelMemory((void *)0x04CC40F8,0x9182FCFCL);
writeKernelMemory((void *)0x04CC40FC,0x4800035CL);
writeKernelMemory((void *)0x04CC4100,0x8182FCFCL);
writeKernelMemory((void *)0x04CC4104,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4108,0x41820350L);
writeKernelMemory((void *)0x04CC410C,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4110,0x2C0C0002L);
writeKernelMemory((void *)0x04CC4114,0x41820010L);
writeKernelMemory((void *)0x04CC4118,0x8182FCF8L);
writeKernelMemory((void *)0x04CC411C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4120,0x40820018L);
writeKernelMemory((void *)0x04CC4124,0x8182FD00L);
writeKernelMemory((void *)0x04CC4128,0x2C0C0000L);
writeKernelMemory((void *)0x04CC412C,0x4082000CL);
writeKernelMemory((void *)0x04CC4130,0x3980000DL);
writeKernelMemory((void *)0x04CC4134,0x9182FD00L);
writeKernelMemory((void *)0x04CC4138,0x8182FCF8L);
writeKernelMemory((void *)0x04CC413C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4140,0x40820018L);
writeKernelMemory((void *)0x04CC4144,0x8182FD00L);
writeKernelMemory((void *)0x04CC4148,0x2C0C0000L);
writeKernelMemory((void *)0x04CC414C,0x4082000CL);
writeKernelMemory((void *)0x04CC4150,0x3980000AL);
writeKernelMemory((void *)0x04CC4154,0x9182FD00L);
writeKernelMemory((void *)0x04CC4158,0x8182FD00L);
writeKernelMemory((void *)0x04CC415C,0x398CFFFFL);
writeKernelMemory((void *)0x04CC4160,0x9182FD00L);
writeKernelMemory((void *)0x04CC4164,0x39800001L);
writeKernelMemory((void *)0x04CC4168,0x9182FCFCL);
writeKernelMemory((void *)0x04CC416C,0x480002ECL);
writeKernelMemory((void *)0x04CC4170,0x8182FCFCL);
writeKernelMemory((void *)0x04CC4174,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4178,0x418202E0L);
writeKernelMemory((void *)0x04CC417C,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4180,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4184,0x418202D4L);
writeKernelMemory((void *)0x04CC4188,0x2C0C0001L);
writeKernelMemory((void *)0x04CC418C,0x418200B5L);
writeKernelMemory((void *)0x04CC4190,0x2C0C0002L);
writeKernelMemory((void *)0x04CC4194,0x41820009L);
writeKernelMemory((void *)0x04CC4198,0x48000128L);
writeKernelMemory((void *)0x04CC419C,0x8122FD00L);
writeKernelMemory((void *)0x04CC41A0,0x2C09000CL);
writeKernelMemory((void *)0x04CC41A4,0x41820048L);
writeKernelMemory((void *)0x04CC41A8,0x2C09000BL);
writeKernelMemory((void *)0x04CC41AC,0x4182004CL);
writeKernelMemory((void *)0x04CC41B0,0x2C09000AL);
writeKernelMemory((void *)0x04CC41B4,0x41820050L);
writeKernelMemory((void *)0x04CC41B8,0x2C090009L);
writeKernelMemory((void *)0x04CC41BC,0x41820054L);
writeKernelMemory((void *)0x04CC41C0,0x2C090008L);
writeKernelMemory((void *)0x04CC41C4,0x41820058L);
writeKernelMemory((void *)0x04CC41C8,0x2C090007L);
writeKernelMemory((void *)0x04CC41CC,0x4182005CL);
writeKernelMemory((void *)0x04CC41D0,0x2C090006L);
writeKernelMemory((void *)0x04CC41D4,0x41820054L);
writeKernelMemory((void *)0x04CC41D8,0x2C090005L);
writeKernelMemory((void *)0x04CC41DC,0x4182004CL);
writeKernelMemory((void *)0x04CC41E0,0x2C090004L);
writeKernelMemory((void *)0x04CC41E4,0x41820050L);
writeKernelMemory((void *)0x04CC41E8,0x4E800020L);
writeKernelMemory((void *)0x04CC41EC,0x39800009L);
writeKernelMemory((void *)0x04CC41F0,0x9182FD00L);
writeKernelMemory((void *)0x04CC41F4,0x4E800020L);
writeKernelMemory((void *)0x04CC41F8,0x39800008L);
writeKernelMemory((void *)0x04CC41FC,0x9182FD00L);
writeKernelMemory((void *)0x04CC4200,0x4E800020L);
writeKernelMemory((void *)0x04CC4204,0x39800007L);
writeKernelMemory((void *)0x04CC4208,0x9182FD00L);
writeKernelMemory((void *)0x04CC420C,0x4E800020L);
writeKernelMemory((void *)0x04CC4210,0x39800006L);
writeKernelMemory((void *)0x04CC4214,0x9182FD00L);
writeKernelMemory((void *)0x04CC4218,0x4E800020L);
writeKernelMemory((void *)0x04CC421C,0x39800005L);
writeKernelMemory((void *)0x04CC4220,0x9182FD00L);
writeKernelMemory((void *)0x04CC4224,0x4E800020L);
writeKernelMemory((void *)0x04CC4228,0x39800004L);
writeKernelMemory((void *)0x04CC422C,0x9182FD00L);
writeKernelMemory((void *)0x04CC4230,0x4E800020L);
writeKernelMemory((void *)0x04CC4234,0x39800003L);
writeKernelMemory((void *)0x04CC4238,0x9182FD00L);
writeKernelMemory((void *)0x04CC423C,0x4E800020L);
writeKernelMemory((void *)0x04CC4240,0x8122FD00L);
writeKernelMemory((void *)0x04CC4244,0x2C090004L);
writeKernelMemory((void *)0x04CC4248,0x41820030L);
writeKernelMemory((void *)0x04CC424C,0x2C090005L);
writeKernelMemory((void *)0x04CC4250,0x41820034L);
writeKernelMemory((void *)0x04CC4254,0x2C090006L);
writeKernelMemory((void *)0x04CC4258,0x41820038L);
writeKernelMemory((void *)0x04CC425C,0x2C090007L);
writeKernelMemory((void *)0x04CC4260,0x4182003CL);
writeKernelMemory((void *)0x04CC4264,0x2C090008L);
writeKernelMemory((void *)0x04CC4268,0x41820040L);
writeKernelMemory((void *)0x04CC426C,0x2C090009L);
writeKernelMemory((void *)0x04CC4270,0x41820044L);
writeKernelMemory((void *)0x04CC4274,0x4E800020L);
writeKernelMemory((void *)0x04CC4278,0x39800007L);
writeKernelMemory((void *)0x04CC427C,0x9182FD00L);
writeKernelMemory((void *)0x04CC4280,0x4E800020L);
writeKernelMemory((void *)0x04CC4284,0x39800008L);
writeKernelMemory((void *)0x04CC4288,0x9182FD00L);
writeKernelMemory((void *)0x04CC428C,0x4E800020L);
writeKernelMemory((void *)0x04CC4290,0x39800009L);
writeKernelMemory((void *)0x04CC4294,0x9182FD00L);
writeKernelMemory((void *)0x04CC4298,0x4E800020L);
writeKernelMemory((void *)0x04CC429C,0x3980000AL);
writeKernelMemory((void *)0x04CC42A0,0x9182FD00L);
writeKernelMemory((void *)0x04CC42A4,0x4E800020L);
writeKernelMemory((void *)0x04CC42A8,0x3980000BL);
writeKernelMemory((void *)0x04CC42AC,0x9182FD00L);
writeKernelMemory((void *)0x04CC42B0,0x4E800020L);
writeKernelMemory((void *)0x04CC42B4,0x3980000CL);
writeKernelMemory((void *)0x04CC42B8,0x9182FD00L);
writeKernelMemory((void *)0x04CC42BC,0x4E800020L);
writeKernelMemory((void *)0x04CC42C0,0x8182FCF8L);
writeKernelMemory((void *)0x04CC42C4,0x398CFFFFL);
writeKernelMemory((void *)0x04CC42C8,0x9182FCF8L);
writeKernelMemory((void *)0x04CC42CC,0x39800001L);
writeKernelMemory((void *)0x04CC42D0,0x9182FCFCL);
writeKernelMemory((void *)0x04CC42D4,0x48000184L);
writeKernelMemory((void *)0x04CC42D8,0x8182FCFCL);
writeKernelMemory((void *)0x04CC42DC,0x2C0C0001L);
writeKernelMemory((void *)0x04CC42E0,0x41820178L);
writeKernelMemory((void *)0x04CC42E4,0x8182FCF8L);
writeKernelMemory((void *)0x04CC42E8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC42EC,0x41820091L);
writeKernelMemory((void *)0x04CC42F0,0x2C0C0001L);
writeKernelMemory((void *)0x04CC42F4,0x41820009L);
writeKernelMemory((void *)0x04CC42F8,0x48000140L);
writeKernelMemory((void *)0x04CC42FC,0x8182FD00L);
writeKernelMemory((void *)0x04CC4300,0x2C0C0004L);
writeKernelMemory((void *)0x04CC4304,0x41820030L);
writeKernelMemory((void *)0x04CC4308,0x2C0C0005L);
writeKernelMemory((void *)0x04CC430C,0x41820034L);
writeKernelMemory((void *)0x04CC4310,0x2C0C0006L);
writeKernelMemory((void *)0x04CC4314,0x41820038L);
writeKernelMemory((void *)0x04CC4318,0x2C0C0007L);
writeKernelMemory((void *)0x04CC431C,0x4182003CL);
writeKernelMemory((void *)0x04CC4320,0x2C0C0008L);
writeKernelMemory((void *)0x04CC4324,0x41820040L);
writeKernelMemory((void *)0x04CC4328,0x2C0C0009L);
writeKernelMemory((void *)0x04CC432C,0x41820044L);
writeKernelMemory((void *)0x04CC4330,0x4E800020L);
writeKernelMemory((void *)0x04CC4334,0x39800007L);
writeKernelMemory((void *)0x04CC4338,0x9182FD00L);
writeKernelMemory((void *)0x04CC433C,0x4E800020L);
writeKernelMemory((void *)0x04CC4340,0x39800008L);
writeKernelMemory((void *)0x04CC4344,0x9182FD00L);
writeKernelMemory((void *)0x04CC4348,0x4E800020L);
writeKernelMemory((void *)0x04CC434C,0x39800009L);
writeKernelMemory((void *)0x04CC4350,0x9182FD00L);
writeKernelMemory((void *)0x04CC4354,0x4E800020L);
writeKernelMemory((void *)0x04CC4358,0x3980000AL);
writeKernelMemory((void *)0x04CC435C,0x9182FD00L);
writeKernelMemory((void *)0x04CC4360,0x4E800020L);
writeKernelMemory((void *)0x04CC4364,0x3980000BL);
writeKernelMemory((void *)0x04CC4368,0x9182FD00L);
writeKernelMemory((void *)0x04CC436C,0x4E800020L);
writeKernelMemory((void *)0x04CC4370,0x3980000CL);
writeKernelMemory((void *)0x04CC4374,0x9182FD00L);
writeKernelMemory((void *)0x04CC4378,0x4E800020L);
writeKernelMemory((void *)0x04CC437C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4380,0x2C0C0004L);
writeKernelMemory((void *)0x04CC4384,0x41820048L);
writeKernelMemory((void *)0x04CC4388,0x2C0C0005L);
writeKernelMemory((void *)0x04CC438C,0x4182004CL);
writeKernelMemory((void *)0x04CC4390,0x2C0C0006L);
writeKernelMemory((void *)0x04CC4394,0x41820050L);
writeKernelMemory((void *)0x04CC4398,0x2C0C0007L);
writeKernelMemory((void *)0x04CC439C,0x41820054L);
writeKernelMemory((void *)0x04CC43A0,0x2C0C0008L);
writeKernelMemory((void *)0x04CC43A4,0x41820058L);
writeKernelMemory((void *)0x04CC43A8,0x2C0C0009L);
writeKernelMemory((void *)0x04CC43AC,0x4182005CL);
writeKernelMemory((void *)0x04CC43B0,0x2C0C000AL);
writeKernelMemory((void *)0x04CC43B4,0x41820060L);
writeKernelMemory((void *)0x04CC43B8,0x2C0C000BL);
writeKernelMemory((void *)0x04CC43BC,0x41820064L);
writeKernelMemory((void *)0x04CC43C0,0x2C0C000CL);
writeKernelMemory((void *)0x04CC43C4,0x41820068L);
writeKernelMemory((void *)0x04CC43C8,0x4E800020L);
writeKernelMemory((void *)0x04CC43CC,0x39800003L);
writeKernelMemory((void *)0x04CC43D0,0x9182FD00L);
writeKernelMemory((void *)0x04CC43D4,0x4E800020L);
writeKernelMemory((void *)0x04CC43D8,0x39800004L);
writeKernelMemory((void *)0x04CC43DC,0x9182FD00L);
writeKernelMemory((void *)0x04CC43E0,0x4E800020L);
writeKernelMemory((void *)0x04CC43E4,0x39800004L);
writeKernelMemory((void *)0x04CC43E8,0x9182FD00L);
writeKernelMemory((void *)0x04CC43EC,0x4E800020L);
writeKernelMemory((void *)0x04CC43F0,0x39800004L);
writeKernelMemory((void *)0x04CC43F4,0x9182FD00L);
writeKernelMemory((void *)0x04CC43F8,0x4E800020L);
writeKernelMemory((void *)0x04CC43FC,0x39800005L);
writeKernelMemory((void *)0x04CC4400,0x9182FD00L);
writeKernelMemory((void *)0x04CC4404,0x4E800020L);
writeKernelMemory((void *)0x04CC4408,0x39800006L);
writeKernelMemory((void *)0x04CC440C,0x9182FD00L);
writeKernelMemory((void *)0x04CC4410,0x4E800020L);
writeKernelMemory((void *)0x04CC4414,0x39800007L);
writeKernelMemory((void *)0x04CC4418,0x9182FD00L);
writeKernelMemory((void *)0x04CC441C,0x4E800020L);
writeKernelMemory((void *)0x04CC4420,0x39800008L);
writeKernelMemory((void *)0x04CC4424,0x9182FD00L);
writeKernelMemory((void *)0x04CC4428,0x4E800020L);
writeKernelMemory((void *)0x04CC442C,0x39800009L);
writeKernelMemory((void *)0x04CC4430,0x9182FD00L);
writeKernelMemory((void *)0x04CC4434,0x4E800020L);
writeKernelMemory((void *)0x04CC4438,0x8182FCF8L);
writeKernelMemory((void *)0x04CC443C,0x398C0001L);
writeKernelMemory((void *)0x04CC4440,0x9182FCF8L);
writeKernelMemory((void *)0x04CC4444,0x39800001L);
writeKernelMemory((void *)0x04CC4448,0x9182FCFCL);
writeKernelMemory((void *)0x04CC444C,0x4800000CL);
writeKernelMemory((void *)0x04CC4450,0x39800000L);
writeKernelMemory((void *)0x04CC4454,0x9182FCFCL);
writeKernelMemory((void *)0x04CC4458,0x7EC903A6L);
writeKernelMemory((void *)0x04CC445C,0x4E800421L);
writeKernelMemory((void *)0x04CC4460,0x3D804000L);
writeKernelMemory((void *)0x04CC4464,0x91820418L);
writeKernelMemory((void *)0x04CC4468,0xC0220418L);
writeKernelMemory((void *)0x04CC446C,0xC0420418L);
writeKernelMemory((void *)0x04CC4470,0x7EE903A6L);
writeKernelMemory((void *)0x04CC4474,0x4E800421L);
writeKernelMemory((void *)0x04CC4478,0x3D804120L);
writeKernelMemory((void *)0x04CC447C,0x9182F03CL);
writeKernelMemory((void *)0x04CC4480,0xC022F03CL);
writeKernelMemory((void *)0x04CC4484,0x3D804170L);
writeKernelMemory((void *)0x04CC4488,0x9182FEFCL);
writeKernelMemory((void *)0x04CC448C,0xC042FEFCL);
writeKernelMemory((void *)0x04CC4490,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4494,0x4E800421L);
writeKernelMemory((void *)0x04CC4498,0x38820D60L);
writeKernelMemory((void *)0x04CC449C,0x38620348L);
writeKernelMemory((void *)0x04CC44A0,0x7F4903A6L);
writeKernelMemory((void *)0x04CC44A4,0x4E800421L);
writeKernelMemory((void *)0x04CC44A8,0x48001D79L);
writeKernelMemory((void *)0x04CC44AC,0x7F8903A6L);
writeKernelMemory((void *)0x04CC44B0,0x4E800421L);
writeKernelMemory((void *)0x04CC44B4,0x7EC903A6L);
writeKernelMemory((void *)0x04CC44B8,0x4E800421L);
writeKernelMemory((void *)0x04CC44BC,0x3D804000L);
writeKernelMemory((void *)0x04CC44C0,0x91820418L);
writeKernelMemory((void *)0x04CC44C4,0xC0220418L);
writeKernelMemory((void *)0x04CC44C8,0xC0420418L);
writeKernelMemory((void *)0x04CC44CC,0x7EE903A6L);
writeKernelMemory((void *)0x04CC44D0,0x4E800421L);
writeKernelMemory((void *)0x04CC44D4,0x3D804250L);
writeKernelMemory((void *)0x04CC44D8,0x9182F03CL);
writeKernelMemory((void *)0x04CC44DC,0xC022F03CL);
writeKernelMemory((void *)0x04CC44E0,0x3D804170L);
writeKernelMemory((void *)0x04CC44E4,0x9182FEFCL);
writeKernelMemory((void *)0x04CC44E8,0xC042FEFCL);
writeKernelMemory((void *)0x04CC44EC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC44F0,0x4E800421L);
writeKernelMemory((void *)0x04CC44F4,0x38820F90L);
writeKernelMemory((void *)0x04CC44F8,0x38620348L);
writeKernelMemory((void *)0x04CC44FC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4500,0x4E800421L);
writeKernelMemory((void *)0x04CC4504,0x48001D1DL);
writeKernelMemory((void *)0x04CC4508,0x7F8903A6L);
writeKernelMemory((void *)0x04CC450C,0x4E800421L);
writeKernelMemory((void *)0x04CC4510,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4514,0x4E800421L);
writeKernelMemory((void *)0x04CC4518,0x3D803FE6L);
writeKernelMemory((void *)0x04CC451C,0x91820418L);
writeKernelMemory((void *)0x04CC4520,0xC0220418L);
writeKernelMemory((void *)0x04CC4524,0xC0420418L);
writeKernelMemory((void *)0x04CC4528,0x7EE903A6L);
writeKernelMemory((void *)0x04CC452C,0x4E800421L);
writeKernelMemory((void *)0x04CC4530,0x3D804264L);
writeKernelMemory((void *)0x04CC4534,0x9182F03CL);
writeKernelMemory((void *)0x04CC4538,0xC022F03CL);
writeKernelMemory((void *)0x04CC453C,0x3D804280L);
writeKernelMemory((void *)0x04CC4540,0x9182FEFCL);
writeKernelMemory((void *)0x04CC4544,0xC042FEFCL);
writeKernelMemory((void *)0x04CC4548,0x7FA903A6L);
writeKernelMemory((void *)0x04CC454C,0x4E800421L);
writeKernelMemory((void *)0x04CC4550,0x38821000L);
writeKernelMemory((void *)0x04CC4554,0x38620348L);
writeKernelMemory((void *)0x04CC4558,0x7F4903A6L);
writeKernelMemory((void *)0x04CC455C,0x4E800421L);
writeKernelMemory((void *)0x04CC4560,0x48001CC1L);
writeKernelMemory((void *)0x04CC4564,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4568,0x4E800421L);
writeKernelMemory((void *)0x04CC456C,0x38600001L);
writeKernelMemory((void *)0x04CC4570,0x38800008L);
writeKernelMemory((void *)0x04CC4574,0x3D80030EL);
writeKernelMemory((void *)0x04CC4578,0x618C52A0L);
writeKernelMemory((void *)0x04CC457C,0x7D8903A6L);
writeKernelMemory((void *)0x04CC4580,0x4E800421L);
writeKernelMemory((void *)0x04CC4584,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4588,0x4E800421L);
writeKernelMemory((void *)0x04CC458C,0x3D80435AL);
writeKernelMemory((void *)0x04CC4590,0x91820418L);
writeKernelMemory((void *)0x04CC4594,0xC0220418L);
writeKernelMemory((void *)0x04CC4598,0x3D8041D0L);
writeKernelMemory((void *)0x04CC459C,0x91820418L);
writeKernelMemory((void *)0x04CC45A0,0xC0420418L);
writeKernelMemory((void *)0x04CC45A4,0x7EE903A6L);
writeKernelMemory((void *)0x04CC45A8,0x4E800421L);
writeKernelMemory((void *)0x04CC45AC,0x3D80C21DL);
writeKernelMemory((void *)0x04CC45B0,0x618CE000L);
writeKernelMemory((void *)0x04CC45B4,0x9182FF04L);
writeKernelMemory((void *)0x04CC45B8,0xC022FF04L);
writeKernelMemory((void *)0x04CC45BC,0x3D80BEA0L);
writeKernelMemory((void *)0x04CC45C0,0x9182FEFCL);
writeKernelMemory((void *)0x04CC45C4,0x9182FEFCL);
writeKernelMemory((void *)0x04CC45C8,0xC042FEFCL);
writeKernelMemory((void *)0x04CC45CC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC45D0,0x4E800421L);
writeKernelMemory((void *)0x04CC45D4,0x38620348L);
writeKernelMemory((void *)0x04CC45D8,0x38820190L);
writeKernelMemory((void *)0x04CC45DC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC45E0,0x4E800421L);
writeKernelMemory((void *)0x04CC45E4,0x7EA3AB78L);
writeKernelMemory((void *)0x04CC45E8,0x38820348L);
writeKernelMemory((void *)0x04CC45EC,0x38A00001L);
writeKernelMemory((void *)0x04CC45F0,0x38C00001L);
writeKernelMemory((void *)0x04CC45F4,0x38E0FFFFL);
writeKernelMemory((void *)0x04CC45F8,0x39000001L);
writeKernelMemory((void *)0x04CC45FC,0x39200001L);
writeKernelMemory((void *)0x04CC4600,0x3D80030EL);
writeKernelMemory((void *)0x04CC4604,0x618C9534L);
writeKernelMemory((void *)0x04CC4608,0x7D8903A6L);
writeKernelMemory((void *)0x04CC460C,0x4E800421L);
writeKernelMemory((void *)0x04CC4610,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4614,0x4E800421L);
writeKernelMemory((void *)0x04CC4618,0x38600004L);
writeKernelMemory((void *)0x04CC461C,0x38800005L);
writeKernelMemory((void *)0x04CC4620,0x3D80030EL);
writeKernelMemory((void *)0x04CC4624,0x618C52A0L);
writeKernelMemory((void *)0x04CC4628,0x7D8903A6L);
writeKernelMemory((void *)0x04CC462C,0x4E800421L);
writeKernelMemory((void *)0x04CC4630,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4634,0x4E800421L);
writeKernelMemory((void *)0x04CC4638,0x48001BC9L);
writeKernelMemory((void *)0x04CC463C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4640,0x48001C11L);
writeKernelMemory((void *)0x04CC4644,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4648,0x4E800421L);
writeKernelMemory((void *)0x04CC464C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4650,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4654,0x40820018L);
writeKernelMemory((void *)0x04CC4658,0x8182FCF8L);
writeKernelMemory((void *)0x04CC465C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4660,0x4082000CL);
writeKernelMemory((void *)0x04CC4664,0x38820C00L);
writeKernelMemory((void *)0x04CC4668,0x48000008L);
writeKernelMemory((void *)0x04CC466C,0x38820C06L);
writeKernelMemory((void *)0x04CC4670,0x38620348L);
writeKernelMemory((void *)0x04CC4674,0x8182FD60L);
writeKernelMemory((void *)0x04CC4678,0x2C0C0000L);
writeKernelMemory((void *)0x04CC467C,0x41820010L);
writeKernelMemory((void *)0x04CC4680,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4684,0x398C0039L);
writeKernelMemory((void *)0x04CC4688,0x91820C06L);
writeKernelMemory((void *)0x04CC468C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4690,0x4E800421L);
writeKernelMemory((void *)0x04CC4694,0x48001B8DL);
writeKernelMemory((void *)0x04CC4698,0x7F8903A6L);
writeKernelMemory((void *)0x04CC469C,0x4E800421L);
writeKernelMemory((void *)0x04CC46A0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC46A4,0x4E800421L);
writeKernelMemory((void *)0x04CC46A8,0x48001B59L);
writeKernelMemory((void *)0x04CC46AC,0xC022FF08L);
writeKernelMemory((void *)0x04CC46B0,0x48001BA1L);
writeKernelMemory((void *)0x04CC46B4,0x7FA903A6L);
writeKernelMemory((void *)0x04CC46B8,0x4E800421L);
writeKernelMemory((void *)0x04CC46BC,0x8182FD00L);
writeKernelMemory((void *)0x04CC46C0,0x2C0C0001L);
writeKernelMemory((void *)0x04CC46C4,0x40820018L);
writeKernelMemory((void *)0x04CC46C8,0x8182FCF8L);
writeKernelMemory((void *)0x04CC46CC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC46D0,0x4082000CL);
writeKernelMemory((void *)0x04CC46D4,0x38820CE0L);
writeKernelMemory((void *)0x04CC46D8,0x48000008L);
writeKernelMemory((void *)0x04CC46DC,0x38820CE6L);
writeKernelMemory((void *)0x04CC46E0,0x38620348L);
writeKernelMemory((void *)0x04CC46E4,0x8182FD5CL);
writeKernelMemory((void *)0x04CC46E8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC46EC,0x41820010L);
writeKernelMemory((void *)0x04CC46F0,0x3D8000A7L);
writeKernelMemory((void *)0x04CC46F4,0x398C0039L);
writeKernelMemory((void *)0x04CC46F8,0x91820CE6L);
writeKernelMemory((void *)0x04CC46FC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4700,0x4E800421L);
writeKernelMemory((void *)0x04CC4704,0x48001B1DL);
writeKernelMemory((void *)0x04CC4708,0x7F8903A6L);
writeKernelMemory((void *)0x04CC470C,0x4E800421L);
writeKernelMemory((void *)0x04CC4710,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4714,0x4E800421L);
writeKernelMemory((void *)0x04CC4718,0x48001AE9L);
writeKernelMemory((void *)0x04CC471C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4720,0x48001B31L);
writeKernelMemory((void *)0x04CC4724,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4728,0x4E800421L);
writeKernelMemory((void *)0x04CC472C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4730,0x2C0C0002L);
writeKernelMemory((void *)0x04CC4734,0x40820018L);
writeKernelMemory((void *)0x04CC4738,0x8182FCF8L);
writeKernelMemory((void *)0x04CC473C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4740,0x4082000CL);
writeKernelMemory((void *)0x04CC4744,0x38820D20L);
writeKernelMemory((void *)0x04CC4748,0x48000008L);
writeKernelMemory((void *)0x04CC474C,0x38820D26L);
writeKernelMemory((void *)0x04CC4750,0x38620348L);
writeKernelMemory((void *)0x04CC4754,0x8182FD58L);
writeKernelMemory((void *)0x04CC4758,0x2C0C0000L);
writeKernelMemory((void *)0x04CC475C,0x41820010L);
writeKernelMemory((void *)0x04CC4760,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4764,0x398C0039L);
writeKernelMemory((void *)0x04CC4768,0x91820D26L);
writeKernelMemory((void *)0x04CC476C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4770,0x4E800421L);
writeKernelMemory((void *)0x04CC4774,0x48001AADL);
writeKernelMemory((void *)0x04CC4778,0x7F8903A6L);
writeKernelMemory((void *)0x04CC477C,0x4E800421L);
writeKernelMemory((void *)0x04CC4780,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4784,0x4E800421L);
writeKernelMemory((void *)0x04CC4788,0x48001A79L);
writeKernelMemory((void *)0x04CC478C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4790,0x48001AC1L);
writeKernelMemory((void *)0x04CC4794,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4798,0x4E800421L);
writeKernelMemory((void *)0x04CC479C,0x8182FD00L);
writeKernelMemory((void *)0x04CC47A0,0x2C0C0003L);
writeKernelMemory((void *)0x04CC47A4,0x40820018L);
writeKernelMemory((void *)0x04CC47A8,0x8182FCF8L);
writeKernelMemory((void *)0x04CC47AC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC47B0,0x4082000CL);
writeKernelMemory((void *)0x04CC47B4,0x38820D90L);
writeKernelMemory((void *)0x04CC47B8,0x48000008L);
writeKernelMemory((void *)0x04CC47BC,0x38820D96L);
writeKernelMemory((void *)0x04CC47C0,0x38620348L);
writeKernelMemory((void *)0x04CC47C4,0x8182FD54L);
writeKernelMemory((void *)0x04CC47C8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC47CC,0x41820010L);
writeKernelMemory((void *)0x04CC47D0,0x3D8000A7L);
writeKernelMemory((void *)0x04CC47D4,0x398C0039L);
writeKernelMemory((void *)0x04CC47D8,0x91820D96L);
writeKernelMemory((void *)0x04CC47DC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC47E0,0x4E800421L);
writeKernelMemory((void *)0x04CC47E4,0x48001A3DL);
writeKernelMemory((void *)0x04CC47E8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC47EC,0x4E800421L);
writeKernelMemory((void *)0x04CC47F0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC47F4,0x4E800421L);
writeKernelMemory((void *)0x04CC47F8,0x48001A09L);
writeKernelMemory((void *)0x04CC47FC,0xC022FF08L);
writeKernelMemory((void *)0x04CC4800,0x48001A51L);
writeKernelMemory((void *)0x04CC4804,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4808,0x4E800421L);
writeKernelMemory((void *)0x04CC480C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4810,0x2C0C0004L);
writeKernelMemory((void *)0x04CC4814,0x40820018L);
writeKernelMemory((void *)0x04CC4818,0x8182FCF8L);
writeKernelMemory((void *)0x04CC481C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4820,0x4082000CL);
writeKernelMemory((void *)0x04CC4824,0x38820E00L);
writeKernelMemory((void *)0x04CC4828,0x48000008L);
writeKernelMemory((void *)0x04CC482C,0x38820E06L);
writeKernelMemory((void *)0x04CC4830,0x38620348L);
writeKernelMemory((void *)0x04CC4834,0x8182FD50L);
writeKernelMemory((void *)0x04CC4838,0x2C0C0000L);
writeKernelMemory((void *)0x04CC483C,0x41820010L);
writeKernelMemory((void *)0x04CC4840,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4844,0x398C0039L);
writeKernelMemory((void *)0x04CC4848,0x91820E06L);
writeKernelMemory((void *)0x04CC484C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4850,0x4E800421L);
writeKernelMemory((void *)0x04CC4854,0x480019CDL);
writeKernelMemory((void *)0x04CC4858,0x7F8903A6L);
writeKernelMemory((void *)0x04CC485C,0x4E800421L);
writeKernelMemory((void *)0x04CC4860,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4864,0x4E800421L);
writeKernelMemory((void *)0x04CC4868,0x48001999L);
writeKernelMemory((void *)0x04CC486C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4870,0x480019E1L);
writeKernelMemory((void *)0x04CC4874,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4878,0x4E800421L);
writeKernelMemory((void *)0x04CC487C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4880,0x2C0C0005L);
writeKernelMemory((void *)0x04CC4884,0x40820018L);
writeKernelMemory((void *)0x04CC4888,0x8182FCF8L);
writeKernelMemory((void *)0x04CC488C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4890,0x4082000CL);
writeKernelMemory((void *)0x04CC4894,0x38820E30L);
writeKernelMemory((void *)0x04CC4898,0x48000008L);
writeKernelMemory((void *)0x04CC489C,0x38820E36L);
writeKernelMemory((void *)0x04CC48A0,0x38620348L);
writeKernelMemory((void *)0x04CC48A4,0x8182FD4CL);
writeKernelMemory((void *)0x04CC48A8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC48AC,0x41820010L);
writeKernelMemory((void *)0x04CC48B0,0x3D8000A7L);
writeKernelMemory((void *)0x04CC48B4,0x398C0039L);
writeKernelMemory((void *)0x04CC48B8,0x91820E36L);
writeKernelMemory((void *)0x04CC48BC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC48C0,0x4E800421L);
writeKernelMemory((void *)0x04CC48C4,0x4800195DL);
writeKernelMemory((void *)0x04CC48C8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC48CC,0x4E800421L);
writeKernelMemory((void *)0x04CC48D0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC48D4,0x4E800421L);
writeKernelMemory((void *)0x04CC48D8,0x48001929L);
writeKernelMemory((void *)0x04CC48DC,0xC022FF08L);
writeKernelMemory((void *)0x04CC48E0,0x48001971L);
writeKernelMemory((void *)0x04CC48E4,0x7FA903A6L);
writeKernelMemory((void *)0x04CC48E8,0x4E800421L);
writeKernelMemory((void *)0x04CC48EC,0x8182FD00L);
writeKernelMemory((void *)0x04CC48F0,0x2C0C0006L);
writeKernelMemory((void *)0x04CC48F4,0x40820018L);
writeKernelMemory((void *)0x04CC48F8,0x8182FCF8L);
writeKernelMemory((void *)0x04CC48FC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4900,0x4082000CL);
writeKernelMemory((void *)0x04CC4904,0x38820E60L);
writeKernelMemory((void *)0x04CC4908,0x48000008L);
writeKernelMemory((void *)0x04CC490C,0x38820E66L);
writeKernelMemory((void *)0x04CC4910,0x38620348L);
writeKernelMemory((void *)0x04CC4914,0x8182FD48L);
writeKernelMemory((void *)0x04CC4918,0x2C0C0000L);
writeKernelMemory((void *)0x04CC491C,0x41820010L);
writeKernelMemory((void *)0x04CC4920,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4924,0x398C0039L);
writeKernelMemory((void *)0x04CC4928,0x91820E66L);
writeKernelMemory((void *)0x04CC492C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4930,0x4E800421L);
writeKernelMemory((void *)0x04CC4934,0x480018EDL);
writeKernelMemory((void *)0x04CC4938,0x7F8903A6L);
writeKernelMemory((void *)0x04CC493C,0x4E800421L);
writeKernelMemory((void *)0x04CC4940,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4944,0x4E800421L);
writeKernelMemory((void *)0x04CC4948,0x480018B9L);
writeKernelMemory((void *)0x04CC494C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4950,0x48001901L);
writeKernelMemory((void *)0x04CC4954,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4958,0x4E800421L);
writeKernelMemory((void *)0x04CC495C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4960,0x2C0C0007L);
writeKernelMemory((void *)0x04CC4964,0x40820018L);
writeKernelMemory((void *)0x04CC4968,0x8182FCF8L);
writeKernelMemory((void *)0x04CC496C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4970,0x4082000CL);
writeKernelMemory((void *)0x04CC4974,0x38820E80L);
writeKernelMemory((void *)0x04CC4978,0x48000008L);
writeKernelMemory((void *)0x04CC497C,0x38820E86L);
writeKernelMemory((void *)0x04CC4980,0x38620348L);
writeKernelMemory((void *)0x04CC4984,0x8182FD44L);
writeKernelMemory((void *)0x04CC4988,0x2C0C0000L);
writeKernelMemory((void *)0x04CC498C,0x41820010L);
writeKernelMemory((void *)0x04CC4990,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4994,0x398C0039L);
writeKernelMemory((void *)0x04CC4998,0x91820E86L);
writeKernelMemory((void *)0x04CC499C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC49A0,0x4E800421L);
writeKernelMemory((void *)0x04CC49A4,0x4800187DL);
writeKernelMemory((void *)0x04CC49A8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC49AC,0x4E800421L);
writeKernelMemory((void *)0x04CC49B0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC49B4,0x4E800421L);
writeKernelMemory((void *)0x04CC49B8,0x48001849L);
writeKernelMemory((void *)0x04CC49BC,0xC022FF08L);
writeKernelMemory((void *)0x04CC49C0,0x48001891L);
writeKernelMemory((void *)0x04CC49C4,0x7FA903A6L);
writeKernelMemory((void *)0x04CC49C8,0x4E800421L);
writeKernelMemory((void *)0x04CC49CC,0x8182FD00L);
writeKernelMemory((void *)0x04CC49D0,0x2C0C0008L);
writeKernelMemory((void *)0x04CC49D4,0x40820018L);
writeKernelMemory((void *)0x04CC49D8,0x8182FCF8L);
writeKernelMemory((void *)0x04CC49DC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC49E0,0x4082000CL);
writeKernelMemory((void *)0x04CC49E4,0x38820EB0L);
writeKernelMemory((void *)0x04CC49E8,0x48000008L);
writeKernelMemory((void *)0x04CC49EC,0x38820EB6L);
writeKernelMemory((void *)0x04CC49F0,0x38620348L);
writeKernelMemory((void *)0x04CC49F4,0x8182FD40L);
writeKernelMemory((void *)0x04CC49F8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC49FC,0x41820010L);
writeKernelMemory((void *)0x04CC4A00,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4A04,0x398C0039L);
writeKernelMemory((void *)0x04CC4A08,0x91820EB6L);
writeKernelMemory((void *)0x04CC4A0C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4A10,0x4E800421L);
writeKernelMemory((void *)0x04CC4A14,0x4800180DL);
writeKernelMemory((void *)0x04CC4A18,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4A1C,0x4E800421L);
writeKernelMemory((void *)0x04CC4A20,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4A24,0x4E800421L);
writeKernelMemory((void *)0x04CC4A28,0x480017D9L);
writeKernelMemory((void *)0x04CC4A2C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4A30,0x48001821L);
writeKernelMemory((void *)0x04CC4A34,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4A38,0x4E800421L);
writeKernelMemory((void *)0x04CC4A3C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4A40,0x2C0C0009L);
writeKernelMemory((void *)0x04CC4A44,0x40820018L);
writeKernelMemory((void *)0x04CC4A48,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4A4C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4A50,0x4082000CL);
writeKernelMemory((void *)0x04CC4A54,0x38820EF0L);
writeKernelMemory((void *)0x04CC4A58,0x48000008L);
writeKernelMemory((void *)0x04CC4A5C,0x38820EF6L);
writeKernelMemory((void *)0x04CC4A60,0x38620348L);
writeKernelMemory((void *)0x04CC4A64,0x8182FD3CL);
writeKernelMemory((void *)0x04CC4A68,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4A6C,0x41820010L);
writeKernelMemory((void *)0x04CC4A70,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4A74,0x398C0039L);
writeKernelMemory((void *)0x04CC4A78,0x91820EF6L);
writeKernelMemory((void *)0x04CC4A7C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4A80,0x4E800421L);
writeKernelMemory((void *)0x04CC4A84,0x4800179DL);
writeKernelMemory((void *)0x04CC4A88,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4A8C,0x4E800421L);
writeKernelMemory((void *)0x04CC4A90,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4A94,0x4E800421L);
writeKernelMemory((void *)0x04CC4A98,0x48001769L);
writeKernelMemory((void *)0x04CC4A9C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4AA0,0x480017B1L);
writeKernelMemory((void *)0x04CC4AA4,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4AA8,0x4E800421L);
writeKernelMemory((void *)0x04CC4AAC,0x8182FD00L);
writeKernelMemory((void *)0x04CC4AB0,0x2C0C000AL);
writeKernelMemory((void *)0x04CC4AB4,0x40820018L);
writeKernelMemory((void *)0x04CC4AB8,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4ABC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4AC0,0x4082000CL);
writeKernelMemory((void *)0x04CC4AC4,0x38820F10L);
writeKernelMemory((void *)0x04CC4AC8,0x48000008L);
writeKernelMemory((void *)0x04CC4ACC,0x38820F16L);
writeKernelMemory((void *)0x04CC4AD0,0x38620348L);
writeKernelMemory((void *)0x04CC4AD4,0x8182FD38L);
writeKernelMemory((void *)0x04CC4AD8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4ADC,0x41820010L);
writeKernelMemory((void *)0x04CC4AE0,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4AE4,0x398C0039L);
writeKernelMemory((void *)0x04CC4AE8,0x91820F16L);
writeKernelMemory((void *)0x04CC4AEC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4AF0,0x4E800421L);
writeKernelMemory((void *)0x04CC4AF4,0x4800172DL);
writeKernelMemory((void *)0x04CC4AF8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4AFC,0x4E800421L);
writeKernelMemory((void *)0x04CC4B00,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4B04,0x4E800421L);
writeKernelMemory((void *)0x04CC4B08,0x480016F9L);
writeKernelMemory((void *)0x04CC4B0C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4B10,0x48001741L);
writeKernelMemory((void *)0x04CC4B14,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4B18,0x4E800421L);
writeKernelMemory((void *)0x04CC4B1C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4B20,0x2C0C000BL);
writeKernelMemory((void *)0x04CC4B24,0x40820018L);
writeKernelMemory((void *)0x04CC4B28,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4B2C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4B30,0x4082000CL);
writeKernelMemory((void *)0x04CC4B34,0x38820F40L);
writeKernelMemory((void *)0x04CC4B38,0x48000008L);
writeKernelMemory((void *)0x04CC4B3C,0x38820F46L);
writeKernelMemory((void *)0x04CC4B40,0x38620348L);
writeKernelMemory((void *)0x04CC4B44,0x8182FD34L);
writeKernelMemory((void *)0x04CC4B48,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4B4C,0x41820010L);
writeKernelMemory((void *)0x04CC4B50,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4B54,0x398C0039L);
writeKernelMemory((void *)0x04CC4B58,0x91820F46L);
writeKernelMemory((void *)0x04CC4B5C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4B60,0x4E800421L);
writeKernelMemory((void *)0x04CC4B64,0x480016BDL);
writeKernelMemory((void *)0x04CC4B68,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4B6C,0x4E800421L);
writeKernelMemory((void *)0x04CC4B70,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4B74,0x4E800421L);
writeKernelMemory((void *)0x04CC4B78,0x48001689L);
writeKernelMemory((void *)0x04CC4B7C,0xC022FF08L);
writeKernelMemory((void *)0x04CC4B80,0x480016D1L);
writeKernelMemory((void *)0x04CC4B84,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4B88,0x4E800421L);
writeKernelMemory((void *)0x04CC4B8C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4B90,0x2C0C000CL);
writeKernelMemory((void *)0x04CC4B94,0x40820018L);
writeKernelMemory((void *)0x04CC4B98,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4B9C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4BA0,0x4082000CL);
writeKernelMemory((void *)0x04CC4BA4,0x38820F70L);
writeKernelMemory((void *)0x04CC4BA8,0x48000008L);
writeKernelMemory((void *)0x04CC4BAC,0x38820F76L);
writeKernelMemory((void *)0x04CC4BB0,0x38620348L);
writeKernelMemory((void *)0x04CC4BB4,0x8182FD30L);
writeKernelMemory((void *)0x04CC4BB8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4BBC,0x41820010L);
writeKernelMemory((void *)0x04CC4BC0,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4BC4,0x398C0039L);
writeKernelMemory((void *)0x04CC4BC8,0x91820F76L);
writeKernelMemory((void *)0x04CC4BCC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4BD0,0x4E800421L);
writeKernelMemory((void *)0x04CC4BD4,0x4800164DL);
writeKernelMemory((void *)0x04CC4BD8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4BDC,0x4E800421L);
writeKernelMemory((void *)0x04CC4BE0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4BE4,0x4E800421L);
writeKernelMemory((void *)0x04CC4BE8,0x48001619L);
writeKernelMemory((void *)0x04CC4BEC,0x3D8042C0L);
writeKernelMemory((void *)0x04CC4BF0,0x91820F58L);
writeKernelMemory((void *)0x04CC4BF4,0xC0220F58L);
writeKernelMemory((void *)0x04CC4BF8,0x4800166DL);
writeKernelMemory((void *)0x04CC4BFC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4C00,0x4E800421L);
writeKernelMemory((void *)0x04CC4C04,0x8182FD00L);
writeKernelMemory((void *)0x04CC4C08,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4C0C,0x40820018L);
writeKernelMemory((void *)0x04CC4C10,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4C14,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4C18,0x4082000CL);
writeKernelMemory((void *)0x04CC4C1C,0x38820FA0L);
writeKernelMemory((void *)0x04CC4C20,0x48000008L);
writeKernelMemory((void *)0x04CC4C24,0x38820FA6L);
writeKernelMemory((void *)0x04CC4C28,0x38620348L);
writeKernelMemory((void *)0x04CC4C2C,0x8182FC60L);
writeKernelMemory((void *)0x04CC4C30,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4C34,0x4182002CL);
writeKernelMemory((void *)0x04CC4C38,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4C3C,0x398C0041L);
writeKernelMemory((void *)0x04CC4C40,0x91820FA6L);
writeKernelMemory((void *)0x04CC4C44,0x2C140000L);
writeKernelMemory((void *)0x04CC4C48,0x40820018L);
writeKernelMemory((void *)0x04CC4C4C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4C50,0x398C0066L);
writeKernelMemory((void *)0x04CC4C54,0x91820FA6L);
writeKernelMemory((void *)0x04CC4C58,0x39200000L);
writeKernelMemory((void *)0x04CC4C5C,0x9122FC60L);
writeKernelMemory((void *)0x04CC4C60,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4C64,0x4E800421L);
writeKernelMemory((void *)0x04CC4C68,0x480015B9L);
writeKernelMemory((void *)0x04CC4C6C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4C70,0x4E800421L);
writeKernelMemory((void *)0x04CC4C74,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4C78,0x4E800421L);
writeKernelMemory((void *)0x04CC4C7C,0x48001585L);
writeKernelMemory((void *)0x04CC4C80,0xC0220F58L);
writeKernelMemory((void *)0x04CC4C84,0x480015E1L);
writeKernelMemory((void *)0x04CC4C88,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4C8C,0x4E800421L);
writeKernelMemory((void *)0x04CC4C90,0x8182FD00L);
writeKernelMemory((void *)0x04CC4C94,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4C98,0x40820018L);
writeKernelMemory((void *)0x04CC4C9C,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4CA0,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4CA4,0x4082000CL);
writeKernelMemory((void *)0x04CC4CA8,0x38820FD0L);
writeKernelMemory((void *)0x04CC4CAC,0x48000008L);
writeKernelMemory((void *)0x04CC4CB0,0x38820FD6L);
writeKernelMemory((void *)0x04CC4CB4,0x38620348L);
writeKernelMemory((void *)0x04CC4CB8,0x8182FC5CL);
writeKernelMemory((void *)0x04CC4CBC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4CC0,0x4182002CL);
writeKernelMemory((void *)0x04CC4CC4,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4CC8,0x398C0041L);
writeKernelMemory((void *)0x04CC4CCC,0x91820FD6L);
writeKernelMemory((void *)0x04CC4CD0,0x2C140000L);
writeKernelMemory((void *)0x04CC4CD4,0x40820018L);
writeKernelMemory((void *)0x04CC4CD8,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4CDC,0x398C0066L);
writeKernelMemory((void *)0x04CC4CE0,0x91820FD6L);
writeKernelMemory((void *)0x04CC4CE4,0x39200000L);
writeKernelMemory((void *)0x04CC4CE8,0x9122FC5CL);
writeKernelMemory((void *)0x04CC4CEC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4CF0,0x4E800421L);
writeKernelMemory((void *)0x04CC4CF4,0x4800152DL);
writeKernelMemory((void *)0x04CC4CF8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4CFC,0x4E800421L);
writeKernelMemory((void *)0x04CC4D00,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4D04,0x4E800421L);
writeKernelMemory((void *)0x04CC4D08,0x480014F9L);
writeKernelMemory((void *)0x04CC4D0C,0xC0220F58L);
writeKernelMemory((void *)0x04CC4D10,0x48001555L);
writeKernelMemory((void *)0x04CC4D14,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4D18,0x4E800421L);
writeKernelMemory((void *)0x04CC4D1C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4D20,0x2C0C0002L);
writeKernelMemory((void *)0x04CC4D24,0x40820018L);
writeKernelMemory((void *)0x04CC4D28,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4D2C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4D30,0x4082000CL);
writeKernelMemory((void *)0x04CC4D34,0x3882FF10L);
writeKernelMemory((void *)0x04CC4D38,0x48000008L);
writeKernelMemory((void *)0x04CC4D3C,0x3882FF16L);
writeKernelMemory((void *)0x04CC4D40,0x38620348L);
writeKernelMemory((void *)0x04CC4D44,0x8182FC58L);
writeKernelMemory((void *)0x04CC4D48,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4D4C,0x41820010L);
writeKernelMemory((void *)0x04CC4D50,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4D54,0x398C0041L);
writeKernelMemory((void *)0x04CC4D58,0x9182FF16L);
writeKernelMemory((void *)0x04CC4D5C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4D60,0x4E800421L);
writeKernelMemory((void *)0x04CC4D64,0x480014BDL);
writeKernelMemory((void *)0x04CC4D68,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4D6C,0x4E800421L);
writeKernelMemory((void *)0x04CC4D70,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4D74,0x4E800421L);
writeKernelMemory((void *)0x04CC4D78,0x48001489L);
writeKernelMemory((void *)0x04CC4D7C,0xC0220F58L);
writeKernelMemory((void *)0x04CC4D80,0x480014E5L);
writeKernelMemory((void *)0x04CC4D84,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4D88,0x4E800421L);
writeKernelMemory((void *)0x04CC4D8C,0x8182FD00L);
writeKernelMemory((void *)0x04CC4D90,0x2C0C0003L);
writeKernelMemory((void *)0x04CC4D94,0x40820018L);
writeKernelMemory((void *)0x04CC4D98,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4D9C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4DA0,0x4082000CL);
writeKernelMemory((void *)0x04CC4DA4,0x3882FF60L);
writeKernelMemory((void *)0x04CC4DA8,0x48000008L);
writeKernelMemory((void *)0x04CC4DAC,0x3882FF66L);
writeKernelMemory((void *)0x04CC4DB0,0x38620348L);
writeKernelMemory((void *)0x04CC4DB4,0x8182FC54L);
writeKernelMemory((void *)0x04CC4DB8,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4DBC,0x41820010L);
writeKernelMemory((void *)0x04CC4DC0,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4DC4,0x398C0041L);
writeKernelMemory((void *)0x04CC4DC8,0x9182FF66L);
writeKernelMemory((void *)0x04CC4DCC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4DD0,0x4E800421L);
writeKernelMemory((void *)0x04CC4DD4,0x4800144DL);
writeKernelMemory((void *)0x04CC4DD8,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4DDC,0x4E800421L);
writeKernelMemory((void *)0x04CC4DE0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4DE4,0x4E800421L);
writeKernelMemory((void *)0x04CC4DE8,0x48001419L);
writeKernelMemory((void *)0x04CC4DEC,0xC0220F58L);
writeKernelMemory((void *)0x04CC4DF0,0x48001489L);
writeKernelMemory((void *)0x04CC4DF4,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4DF8,0x4E800421L);
writeKernelMemory((void *)0x04CC4DFC,0x8182FD00L);
writeKernelMemory((void *)0x04CC4E00,0x2C0C0004L);
writeKernelMemory((void *)0x04CC4E04,0x40820018L);
writeKernelMemory((void *)0x04CC4E08,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4E0C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4E10,0x4082000CL);
writeKernelMemory((void *)0x04CC4E14,0x38821010L);
writeKernelMemory((void *)0x04CC4E18,0x48000008L);
writeKernelMemory((void *)0x04CC4E1C,0x38821016L);
writeKernelMemory((void *)0x04CC4E20,0x38620348L);
writeKernelMemory((void *)0x04CC4E24,0x8182FC50L);
writeKernelMemory((void *)0x04CC4E28,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4E2C,0x4182002CL);
writeKernelMemory((void *)0x04CC4E30,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4E34,0x398C0042L);
writeKernelMemory((void *)0x04CC4E38,0x91821016L);
writeKernelMemory((void *)0x04CC4E3C,0x2C140000L);
writeKernelMemory((void *)0x04CC4E40,0x40820018L);
writeKernelMemory((void *)0x04CC4E44,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4E48,0x398C0066L);
writeKernelMemory((void *)0x04CC4E4C,0x91821016L);
writeKernelMemory((void *)0x04CC4E50,0x39200000L);
writeKernelMemory((void *)0x04CC4E54,0x9122FC50L);
writeKernelMemory((void *)0x04CC4E58,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4E5C,0x4E800421L);
writeKernelMemory((void *)0x04CC4E60,0x480013C1L);
writeKernelMemory((void *)0x04CC4E64,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4E68,0x4E800421L);
writeKernelMemory((void *)0x04CC4E6C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4E70,0x4E800421L);
writeKernelMemory((void *)0x04CC4E74,0x4800138DL);
writeKernelMemory((void *)0x04CC4E78,0xC0220F58L);
writeKernelMemory((void *)0x04CC4E7C,0x480013FDL);
writeKernelMemory((void *)0x04CC4E80,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4E84,0x4E800421L);
writeKernelMemory((void *)0x04CC4E88,0x8182FD00L);
writeKernelMemory((void *)0x04CC4E8C,0x2C0C0005L);
writeKernelMemory((void *)0x04CC4E90,0x40820018L);
writeKernelMemory((void *)0x04CC4E94,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4E98,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4E9C,0x4082000CL);
writeKernelMemory((void *)0x04CC4EA0,0x38820060L);
writeKernelMemory((void *)0x04CC4EA4,0x48000008L);
writeKernelMemory((void *)0x04CC4EA8,0x38820066L);
writeKernelMemory((void *)0x04CC4EAC,0x38620348L);
writeKernelMemory((void *)0x04CC4EB0,0x8182FC4CL);
writeKernelMemory((void *)0x04CC4EB4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4EB8,0x4182002CL);
writeKernelMemory((void *)0x04CC4EBC,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4EC0,0x398C0042L);
writeKernelMemory((void *)0x04CC4EC4,0x91820066L);
writeKernelMemory((void *)0x04CC4EC8,0x2C140000L);
writeKernelMemory((void *)0x04CC4ECC,0x40820018L);
writeKernelMemory((void *)0x04CC4ED0,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4ED4,0x398C0066L);
writeKernelMemory((void *)0x04CC4ED8,0x91820066L);
writeKernelMemory((void *)0x04CC4EDC,0x39200000L);
writeKernelMemory((void *)0x04CC4EE0,0x9122FC4CL);
writeKernelMemory((void *)0x04CC4EE4,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4EE8,0x4E800421L);
writeKernelMemory((void *)0x04CC4EEC,0x48001335L);
writeKernelMemory((void *)0x04CC4EF0,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4EF4,0x4E800421L);
writeKernelMemory((void *)0x04CC4EF8,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4EFC,0x4E800421L);
writeKernelMemory((void *)0x04CC4F00,0x48001301L);
writeKernelMemory((void *)0x04CC4F04,0xC0220F58L);
writeKernelMemory((void *)0x04CC4F08,0x48001371L);
writeKernelMemory((void *)0x04CC4F0C,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4F10,0x4E800421L);
writeKernelMemory((void *)0x04CC4F14,0x8182FD00L);
writeKernelMemory((void *)0x04CC4F18,0x2C0C0006L);
writeKernelMemory((void *)0x04CC4F1C,0x40820018L);
writeKernelMemory((void *)0x04CC4F20,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4F24,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4F28,0x4082000CL);
writeKernelMemory((void *)0x04CC4F2C,0x388200B0L);
writeKernelMemory((void *)0x04CC4F30,0x48000008L);
writeKernelMemory((void *)0x04CC4F34,0x388200B6L);
writeKernelMemory((void *)0x04CC4F38,0x38620348L);
writeKernelMemory((void *)0x04CC4F3C,0x8182FC48L);
writeKernelMemory((void *)0x04CC4F40,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4F44,0x4182002CL);
writeKernelMemory((void *)0x04CC4F48,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4F4C,0x398C0042L);
writeKernelMemory((void *)0x04CC4F50,0x918200B6L);
writeKernelMemory((void *)0x04CC4F54,0x2C140000L);
writeKernelMemory((void *)0x04CC4F58,0x40820018L);
writeKernelMemory((void *)0x04CC4F5C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4F60,0x398C0066L);
writeKernelMemory((void *)0x04CC4F64,0x918200B6L);
writeKernelMemory((void *)0x04CC4F68,0x39200000L);
writeKernelMemory((void *)0x04CC4F6C,0x9122FC48L);
writeKernelMemory((void *)0x04CC4F70,0x7F4903A6L);
writeKernelMemory((void *)0x04CC4F74,0x4E800421L);
writeKernelMemory((void *)0x04CC4F78,0x480012A9L);
writeKernelMemory((void *)0x04CC4F7C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC4F80,0x4E800421L);
writeKernelMemory((void *)0x04CC4F84,0x7EC903A6L);
writeKernelMemory((void *)0x04CC4F88,0x4E800421L);
writeKernelMemory((void *)0x04CC4F8C,0x48001275L);
writeKernelMemory((void *)0x04CC4F90,0xC0220F58L);
writeKernelMemory((void *)0x04CC4F94,0x480012E5L);
writeKernelMemory((void *)0x04CC4F98,0x7FA903A6L);
writeKernelMemory((void *)0x04CC4F9C,0x4E800421L);
writeKernelMemory((void *)0x04CC4FA0,0x8182FD00L);
writeKernelMemory((void *)0x04CC4FA4,0x2C0C0007L);
writeKernelMemory((void *)0x04CC4FA8,0x40820018L);
writeKernelMemory((void *)0x04CC4FAC,0x8182FCF8L);
writeKernelMemory((void *)0x04CC4FB0,0x2C0C0001L);
writeKernelMemory((void *)0x04CC4FB4,0x4082000CL);
writeKernelMemory((void *)0x04CC4FB8,0x388200E0L);
writeKernelMemory((void *)0x04CC4FBC,0x48000008L);
writeKernelMemory((void *)0x04CC4FC0,0x388200E6L);
writeKernelMemory((void *)0x04CC4FC4,0x38620348L);
writeKernelMemory((void *)0x04CC4FC8,0x8182FC44L);
writeKernelMemory((void *)0x04CC4FCC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC4FD0,0x4182002CL);
writeKernelMemory((void *)0x04CC4FD4,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4FD8,0x398C0042L);
writeKernelMemory((void *)0x04CC4FDC,0x918200E6L);
writeKernelMemory((void *)0x04CC4FE0,0x2C140000L);
writeKernelMemory((void *)0x04CC4FE4,0x40820018L);
writeKernelMemory((void *)0x04CC4FE8,0x3D8000A7L);
writeKernelMemory((void *)0x04CC4FEC,0x398C0066L);
writeKernelMemory((void *)0x04CC4FF0,0x918200E6L);
writeKernelMemory((void *)0x04CC4FF4,0x39200000L);
writeKernelMemory((void *)0x04CC4FF8,0x9122FC44L);
writeKernelMemory((void *)0x04CC4FFC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5000,0x4E800421L);
writeKernelMemory((void *)0x04CC5004,0x4800121DL);
writeKernelMemory((void *)0x04CC5008,0x7F8903A6L);
writeKernelMemory((void *)0x04CC500C,0x4E800421L);
writeKernelMemory((void *)0x04CC5010,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5014,0x4E800421L);
writeKernelMemory((void *)0x04CC5018,0x480011E9L);
writeKernelMemory((void *)0x04CC501C,0xC0220F58L);
writeKernelMemory((void *)0x04CC5020,0x48001259L);
writeKernelMemory((void *)0x04CC5024,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5028,0x4E800421L);
writeKernelMemory((void *)0x04CC502C,0x8182FD00L);
writeKernelMemory((void *)0x04CC5030,0x2C0C0008L);
writeKernelMemory((void *)0x04CC5034,0x40820018L);
writeKernelMemory((void *)0x04CC5038,0x8182FCF8L);
writeKernelMemory((void *)0x04CC503C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5040,0x4082000CL);
writeKernelMemory((void *)0x04CC5044,0x38820100L);
writeKernelMemory((void *)0x04CC5048,0x48000008L);
writeKernelMemory((void *)0x04CC504C,0x38820106L);
writeKernelMemory((void *)0x04CC5050,0x38620348L);
writeKernelMemory((void *)0x04CC5054,0x8182FC40L);
writeKernelMemory((void *)0x04CC5058,0x2C0C0000L);
writeKernelMemory((void *)0x04CC505C,0x4182002CL);
writeKernelMemory((void *)0x04CC5060,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5064,0x398C0042L);
writeKernelMemory((void *)0x04CC5068,0x91820106L);
writeKernelMemory((void *)0x04CC506C,0x2C140000L);
writeKernelMemory((void *)0x04CC5070,0x40820018L);
writeKernelMemory((void *)0x04CC5074,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5078,0x398C0066L);
writeKernelMemory((void *)0x04CC507C,0x91820106L);
writeKernelMemory((void *)0x04CC5080,0x39200000L);
writeKernelMemory((void *)0x04CC5084,0x9122FC40L);
writeKernelMemory((void *)0x04CC5088,0x7F4903A6L);
writeKernelMemory((void *)0x04CC508C,0x4E800421L);
writeKernelMemory((void *)0x04CC5090,0x48001191L);
writeKernelMemory((void *)0x04CC5094,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5098,0x4E800421L);
writeKernelMemory((void *)0x04CC509C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC50A0,0x4E800421L);
writeKernelMemory((void *)0x04CC50A4,0x4800115DL);
writeKernelMemory((void *)0x04CC50A8,0xC0220F58L);
writeKernelMemory((void *)0x04CC50AC,0x480011CDL);
writeKernelMemory((void *)0x04CC50B0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC50B4,0x4E800421L);
writeKernelMemory((void *)0x04CC50B8,0x8182FD00L);
writeKernelMemory((void *)0x04CC50BC,0x2C0C0009L);
writeKernelMemory((void *)0x04CC50C0,0x40820018L);
writeKernelMemory((void *)0x04CC50C4,0x8182FCF8L);
writeKernelMemory((void *)0x04CC50C8,0x2C0C0001L);
writeKernelMemory((void *)0x04CC50CC,0x4082000CL);
writeKernelMemory((void *)0x04CC50D0,0x38820130L);
writeKernelMemory((void *)0x04CC50D4,0x48000008L);
writeKernelMemory((void *)0x04CC50D8,0x38820136L);
writeKernelMemory((void *)0x04CC50DC,0x38620348L);
writeKernelMemory((void *)0x04CC50E0,0x8182FC3CL);
writeKernelMemory((void *)0x04CC50E4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC50E8,0x4182002CL);
writeKernelMemory((void *)0x04CC50EC,0x3D8000A7L);
writeKernelMemory((void *)0x04CC50F0,0x398C0042L);
writeKernelMemory((void *)0x04CC50F4,0x91820136L);
writeKernelMemory((void *)0x04CC50F8,0x2C140000L);
writeKernelMemory((void *)0x04CC50FC,0x40820018L);
writeKernelMemory((void *)0x04CC5100,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5104,0x398C0066L);
writeKernelMemory((void *)0x04CC5108,0x91820136L);
writeKernelMemory((void *)0x04CC510C,0x39200000L);
writeKernelMemory((void *)0x04CC5110,0x9122FC3CL);
writeKernelMemory((void *)0x04CC5114,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5118,0x4E800421L);
writeKernelMemory((void *)0x04CC511C,0x48001105L);
writeKernelMemory((void *)0x04CC5120,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5124,0x4E800421L);
writeKernelMemory((void *)0x04CC5128,0x7EC903A6L);
writeKernelMemory((void *)0x04CC512C,0x4E800421L);
writeKernelMemory((void *)0x04CC5130,0x3D804000L);
writeKernelMemory((void *)0x04CC5134,0x91820418L);
writeKernelMemory((void *)0x04CC5138,0xC0220418L);
writeKernelMemory((void *)0x04CC513C,0xC0420418L);
writeKernelMemory((void *)0x04CC5140,0x7EE903A6L);
writeKernelMemory((void *)0x04CC5144,0x4E800421L);
writeKernelMemory((void *)0x04CC5148,0x3D8042BAL);
writeKernelMemory((void *)0x04CC514C,0x9182F03CL);
writeKernelMemory((void *)0x04CC5150,0xC022F03CL);
writeKernelMemory((void *)0x04CC5154,0x3D804170L);
writeKernelMemory((void *)0x04CC5158,0x9182FEFCL);
writeKernelMemory((void *)0x04CC515C,0xC042FEFCL);
writeKernelMemory((void *)0x04CC5160,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5164,0x4E800421L);
writeKernelMemory((void *)0x04CC5168,0x38820150L);
writeKernelMemory((void *)0x04CC516C,0x38620348L);
writeKernelMemory((void *)0x04CC5170,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5174,0x4E800421L);
writeKernelMemory((void *)0x04CC5178,0x480010A9L);
writeKernelMemory((void *)0x04CC517C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5180,0x4E800421L);
writeKernelMemory((void *)0x04CC5184,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5188,0x4E800421L);
writeKernelMemory((void *)0x04CC518C,0x48001075L);
writeKernelMemory((void *)0x04CC5190,0x3D80432DL);
writeKernelMemory((void *)0x04CC5194,0x91820028L);
writeKernelMemory((void *)0x04CC5198,0xC0220028L);
writeKernelMemory((void *)0x04CC519C,0x480010F1L);
writeKernelMemory((void *)0x04CC51A0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC51A4,0x4E800421L);
writeKernelMemory((void *)0x04CC51A8,0x8182FD00L);
writeKernelMemory((void *)0x04CC51AC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC51B0,0x40820018L);
writeKernelMemory((void *)0x04CC51B4,0x8182FCF8L);
writeKernelMemory((void *)0x04CC51B8,0x2C0C0002L);
writeKernelMemory((void *)0x04CC51BC,0x4082000CL);
writeKernelMemory((void *)0x04CC51C0,0x38820160L);
writeKernelMemory((void *)0x04CC51C4,0x48000008L);
writeKernelMemory((void *)0x04CC51C8,0x38820166L);
writeKernelMemory((void *)0x04CC51CC,0x38620348L);
writeKernelMemory((void *)0x04CC51D0,0x8182FB60L);
writeKernelMemory((void *)0x04CC51D4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC51D8,0x41820010L);
writeKernelMemory((void *)0x04CC51DC,0x3D8000A7L);
writeKernelMemory((void *)0x04CC51E0,0x398C0043L);
writeKernelMemory((void *)0x04CC51E4,0x91820166L);
writeKernelMemory((void *)0x04CC51E8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC51EC,0x4E800421L);
writeKernelMemory((void *)0x04CC51F0,0x48001031L);
writeKernelMemory((void *)0x04CC51F4,0x7F8903A6L);
writeKernelMemory((void *)0x04CC51F8,0x4E800421L);
writeKernelMemory((void *)0x04CC51FC,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5200,0x4E800421L);
writeKernelMemory((void *)0x04CC5204,0x48000FFDL);
writeKernelMemory((void *)0x04CC5208,0xC0220028L);
writeKernelMemory((void *)0x04CC520C,0x48001081L);
writeKernelMemory((void *)0x04CC5210,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5214,0x4E800421L);
writeKernelMemory((void *)0x04CC5218,0x8182FD00L);
writeKernelMemory((void *)0x04CC521C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5220,0x40820018L);
writeKernelMemory((void *)0x04CC5224,0x8182FCF8L);
writeKernelMemory((void *)0x04CC5228,0x2C0C0002L);
writeKernelMemory((void *)0x04CC522C,0x4082000CL);
writeKernelMemory((void *)0x04CC5230,0x388201A0L);
writeKernelMemory((void *)0x04CC5234,0x48000008L);
writeKernelMemory((void *)0x04CC5238,0x388201A6L);
writeKernelMemory((void *)0x04CC523C,0x38620348L);
writeKernelMemory((void *)0x04CC5240,0x8182FB5CL);
writeKernelMemory((void *)0x04CC5244,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5248,0x41820010L);
writeKernelMemory((void *)0x04CC524C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5250,0x398C0043L);
writeKernelMemory((void *)0x04CC5254,0x918201A6L);
writeKernelMemory((void *)0x04CC5258,0x7F4903A6L);
writeKernelMemory((void *)0x04CC525C,0x4E800421L);
writeKernelMemory((void *)0x04CC5260,0x48000FC1L);
writeKernelMemory((void *)0x04CC5264,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5268,0x4E800421L);
writeKernelMemory((void *)0x04CC526C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5270,0x4E800421L);
writeKernelMemory((void *)0x04CC5274,0x48000F8DL);
writeKernelMemory((void *)0x04CC5278,0xC0220028L);
writeKernelMemory((void *)0x04CC527C,0x48001011L);
writeKernelMemory((void *)0x04CC5280,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5284,0x4E800421L);
writeKernelMemory((void *)0x04CC5288,0x8182FD00L);
writeKernelMemory((void *)0x04CC528C,0x2C0C0002L);
writeKernelMemory((void *)0x04CC5290,0x40820018L);
writeKernelMemory((void *)0x04CC5294,0x8182FCF8L);
writeKernelMemory((void *)0x04CC5298,0x2C0C0002L);
writeKernelMemory((void *)0x04CC529C,0x4082000CL);
writeKernelMemory((void *)0x04CC52A0,0x388201D0L);
writeKernelMemory((void *)0x04CC52A4,0x48000008L);
writeKernelMemory((void *)0x04CC52A8,0x388201D6L);
writeKernelMemory((void *)0x04CC52AC,0x38620348L);
writeKernelMemory((void *)0x04CC52B0,0x8182FB58L);
writeKernelMemory((void *)0x04CC52B4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC52B8,0x41820010L);
writeKernelMemory((void *)0x04CC52BC,0x3D8000A7L);
writeKernelMemory((void *)0x04CC52C0,0x398C0043L);
writeKernelMemory((void *)0x04CC52C4,0x918201D6L);
writeKernelMemory((void *)0x04CC52C8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC52CC,0x4E800421L);
writeKernelMemory((void *)0x04CC52D0,0x48000F51L);
writeKernelMemory((void *)0x04CC52D4,0x7F8903A6L);
writeKernelMemory((void *)0x04CC52D8,0x4E800421L);
writeKernelMemory((void *)0x04CC52DC,0x7EC903A6L);
writeKernelMemory((void *)0x04CC52E0,0x4E800421L);
writeKernelMemory((void *)0x04CC52E4,0x48000F1DL);
writeKernelMemory((void *)0x04CC52E8,0xC0220028L);
writeKernelMemory((void *)0x04CC52EC,0x48000FA1L);
writeKernelMemory((void *)0x04CC52F0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC52F4,0x4E800421L);
writeKernelMemory((void *)0x04CC52F8,0x8182FD00L);
writeKernelMemory((void *)0x04CC52FC,0x2C0C0003L);
writeKernelMemory((void *)0x04CC5300,0x40820018L);
writeKernelMemory((void *)0x04CC5304,0x8182FCF8L);
writeKernelMemory((void *)0x04CC5308,0x2C0C0002L);
writeKernelMemory((void *)0x04CC530C,0x4082000CL);
writeKernelMemory((void *)0x04CC5310,0x38820230L);
writeKernelMemory((void *)0x04CC5314,0x48000008L);
writeKernelMemory((void *)0x04CC5318,0x38820236L);
writeKernelMemory((void *)0x04CC531C,0x38620348L);
writeKernelMemory((void *)0x04CC5320,0x8182FB54L);
writeKernelMemory((void *)0x04CC5324,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5328,0x41820010L);
writeKernelMemory((void *)0x04CC532C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5330,0x398C0043L);
writeKernelMemory((void *)0x04CC5334,0x91820236L);
writeKernelMemory((void *)0x04CC5338,0x7F4903A6L);
writeKernelMemory((void *)0x04CC533C,0x4E800421L);
writeKernelMemory((void *)0x04CC5340,0x48000EE1L);
writeKernelMemory((void *)0x04CC5344,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5348,0x4E800421L);
writeKernelMemory((void *)0x04CC534C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5350,0x4E800421L);
writeKernelMemory((void *)0x04CC5354,0x48000EADL);
writeKernelMemory((void *)0x04CC5358,0xC0220028L);
writeKernelMemory((void *)0x04CC535C,0x48000F31L);
writeKernelMemory((void *)0x04CC5360,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5364,0x4E800421L);
writeKernelMemory((void *)0x04CC5368,0x8182FD00L);
writeKernelMemory((void *)0x04CC536C,0x2C0C0004L);
writeKernelMemory((void *)0x04CC5370,0x40820018L);
writeKernelMemory((void *)0x04CC5374,0x8182FCF8L);
writeKernelMemory((void *)0x04CC5378,0x2C0C0002L);
writeKernelMemory((void *)0x04CC537C,0x4082000CL);
writeKernelMemory((void *)0x04CC5380,0x38820280L);
writeKernelMemory((void *)0x04CC5384,0x48000008L);
writeKernelMemory((void *)0x04CC5388,0x38820286L);
writeKernelMemory((void *)0x04CC538C,0x38620348L);
writeKernelMemory((void *)0x04CC5390,0x8182FB50L);
writeKernelMemory((void *)0x04CC5394,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5398,0x41820010L);
writeKernelMemory((void *)0x04CC539C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC53A0,0x398C0043L);
writeKernelMemory((void *)0x04CC53A4,0x91820286L);
writeKernelMemory((void *)0x04CC53A8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC53AC,0x4E800421L);
writeKernelMemory((void *)0x04CC53B0,0x48000E71L);
writeKernelMemory((void *)0x04CC53B4,0x7F8903A6L);
writeKernelMemory((void *)0x04CC53B8,0x4E800421L);
writeKernelMemory((void *)0x04CC53BC,0x7EC903A6L);
writeKernelMemory((void *)0x04CC53C0,0x4E800421L);
writeKernelMemory((void *)0x04CC53C4,0x48000E3DL);
writeKernelMemory((void *)0x04CC53C8,0xC0220028L);
writeKernelMemory((void *)0x04CC53CC,0x48000EC1L);
writeKernelMemory((void *)0x04CC53D0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC53D4,0x4E800421L);
writeKernelMemory((void *)0x04CC53D8,0x8182FD00L);
writeKernelMemory((void *)0x04CC53DC,0x2C0C0005L);
writeKernelMemory((void *)0x04CC53E0,0x40820018L);
writeKernelMemory((void *)0x04CC53E4,0x8182FCF8L);
writeKernelMemory((void *)0x04CC53E8,0x2C0C0002L);
writeKernelMemory((void *)0x04CC53EC,0x4082000CL);
writeKernelMemory((void *)0x04CC53F0,0x388202A0L);
writeKernelMemory((void *)0x04CC53F4,0x48000008L);
writeKernelMemory((void *)0x04CC53F8,0x388202A6L);
writeKernelMemory((void *)0x04CC53FC,0x38620348L);
writeKernelMemory((void *)0x04CC5400,0x8182FB4CL);
writeKernelMemory((void *)0x04CC5404,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5408,0x41820010L);
writeKernelMemory((void *)0x04CC540C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5410,0x398C0043L);
writeKernelMemory((void *)0x04CC5414,0x918202A6L);
writeKernelMemory((void *)0x04CC5418,0x7F4903A6L);
writeKernelMemory((void *)0x04CC541C,0x4E800421L);
writeKernelMemory((void *)0x04CC5420,0x48000E01L);
writeKernelMemory((void *)0x04CC5424,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5428,0x4E800421L);
writeKernelMemory((void *)0x04CC542C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5430,0x4E800421L);
writeKernelMemory((void *)0x04CC5434,0x48000DCDL);
writeKernelMemory((void *)0x04CC5438,0xC0220028L);
writeKernelMemory((void *)0x04CC543C,0x48000E51L);
writeKernelMemory((void *)0x04CC5440,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5444,0x4E800421L);
writeKernelMemory((void *)0x04CC5448,0x8182FD00L);
writeKernelMemory((void *)0x04CC544C,0x2C0C0006L);
writeKernelMemory((void *)0x04CC5450,0x40820018L);
writeKernelMemory((void *)0x04CC5454,0x8182FCF8L);
writeKernelMemory((void *)0x04CC5458,0x2C0C0002L);
writeKernelMemory((void *)0x04CC545C,0x4082000CL);
writeKernelMemory((void *)0x04CC5460,0x388202C0L);
writeKernelMemory((void *)0x04CC5464,0x48000008L);
writeKernelMemory((void *)0x04CC5468,0x388202C6L);
writeKernelMemory((void *)0x04CC546C,0x38620348L);
writeKernelMemory((void *)0x04CC5470,0x8182FB48L);
writeKernelMemory((void *)0x04CC5474,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5478,0x41820010L);
writeKernelMemory((void *)0x04CC547C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5480,0x398C0043L);
writeKernelMemory((void *)0x04CC5484,0x918202C6L);
writeKernelMemory((void *)0x04CC5488,0x7F4903A6L);
writeKernelMemory((void *)0x04CC548C,0x4E800421L);
writeKernelMemory((void *)0x04CC5490,0x48000D91L);
writeKernelMemory((void *)0x04CC5494,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5498,0x4E800421L);
writeKernelMemory((void *)0x04CC549C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC54A0,0x4E800421L);
writeKernelMemory((void *)0x04CC54A4,0x48000D5DL);
writeKernelMemory((void *)0x04CC54A8,0xC0220028L);
writeKernelMemory((void *)0x04CC54AC,0x48000DE1L);
writeKernelMemory((void *)0x04CC54B0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC54B4,0x4E800421L);
writeKernelMemory((void *)0x04CC54B8,0x8182FD00L);
writeKernelMemory((void *)0x04CC54BC,0x2C0C0007L);
writeKernelMemory((void *)0x04CC54C0,0x40820018L);
writeKernelMemory((void *)0x04CC54C4,0x8182FCF8L);
writeKernelMemory((void *)0x04CC54C8,0x2C0C0002L);
writeKernelMemory((void *)0x04CC54CC,0x4082000CL);
writeKernelMemory((void *)0x04CC54D0,0x388202E0L);
writeKernelMemory((void *)0x04CC54D4,0x48000008L);
writeKernelMemory((void *)0x04CC54D8,0x388202E6L);
writeKernelMemory((void *)0x04CC54DC,0x38620348L);
writeKernelMemory((void *)0x04CC54E0,0x8182FB44L);
writeKernelMemory((void *)0x04CC54E4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC54E8,0x41820010L);
writeKernelMemory((void *)0x04CC54EC,0x3D8000A7L);
writeKernelMemory((void *)0x04CC54F0,0x398C0043L);
writeKernelMemory((void *)0x04CC54F4,0x918202E6L);
writeKernelMemory((void *)0x04CC54F8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC54FC,0x4E800421L);
writeKernelMemory((void *)0x04CC5500,0x48000D21L);
writeKernelMemory((void *)0x04CC5504,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5508,0x4E800421L);
writeKernelMemory((void *)0x04CC550C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5510,0x4E800421L);
writeKernelMemory((void *)0x04CC5514,0x48000CEDL);
writeKernelMemory((void *)0x04CC5518,0xC0220028L);
writeKernelMemory((void *)0x04CC551C,0x48000D71L);
writeKernelMemory((void *)0x04CC5520,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5524,0x4E800421L);
writeKernelMemory((void *)0x04CC5528,0x8182FD00L);
writeKernelMemory((void *)0x04CC552C,0x2C0C0008L);
writeKernelMemory((void *)0x04CC5530,0x40820018L);
writeKernelMemory((void *)0x04CC5534,0x8182FCF8L);
writeKernelMemory((void *)0x04CC5538,0x2C0C0002L);
writeKernelMemory((void *)0x04CC553C,0x4082000CL);
writeKernelMemory((void *)0x04CC5540,0x38820370L);
writeKernelMemory((void *)0x04CC5544,0x48000008L);
writeKernelMemory((void *)0x04CC5548,0x38820376L);
writeKernelMemory((void *)0x04CC554C,0x38620348L);
writeKernelMemory((void *)0x04CC5550,0x8182FB40L);
writeKernelMemory((void *)0x04CC5554,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5558,0x41820010L);
writeKernelMemory((void *)0x04CC555C,0x3D8000A7L);
writeKernelMemory((void *)0x04CC5560,0x398C0043L);
writeKernelMemory((void *)0x04CC5564,0x91820376L);
writeKernelMemory((void *)0x04CC5568,0x7F4903A6L);
writeKernelMemory((void *)0x04CC556C,0x4E800421L);
writeKernelMemory((void *)0x04CC5570,0x48000CB1L);
writeKernelMemory((void *)0x04CC5574,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5578,0x4E800421L);
writeKernelMemory((void *)0x04CC557C,0x48000D7CL);
writeKernelMemory((void *)0x04CC5580,0x81990034L);
writeKernelMemory((void *)0x04CC5584,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5588,0x4182001CL);
writeKernelMemory((void *)0x04CC558C,0x3D600100L);
writeKernelMemory((void *)0x04CC5590,0x916C0814L);
writeKernelMemory((void *)0x04CC5594,0x8162FE00L);
writeKernelMemory((void *)0x04CC5598,0x916C0148L);
writeKernelMemory((void *)0x04CC559C,0x8162FDFCL);
writeKernelMemory((void *)0x04CC55A0,0x916C014CL);
writeKernelMemory((void *)0x04CC55A4,0x39800000L);
writeKernelMemory((void *)0x04CC55A8,0x9182FC50L);
writeKernelMemory((void *)0x04CC55AC,0x3D80108DL);
writeKernelMemory((void *)0x04CC55B0,0x3D20C5A0L);
writeKernelMemory((void *)0x04CC55B4,0x912CB504L);
writeKernelMemory((void *)0x04CC55B8,0x3D80102FL);
writeKernelMemory((void *)0x04CC55BC,0x814CFA63L);
writeKernelMemory((void *)0x04CC55C0,0x2C0A0080L);
writeKernelMemory((void *)0x04CC55C4,0x4182003CL);
writeKernelMemory((void *)0x04CC55C8,0x728A0100L);
writeKernelMemory((void *)0x04CC55CC,0x2C0A0100L);
writeKernelMemory((void *)0x04CC55D0,0x41820230L);
writeKernelMemory((void *)0x04CC55D4,0x728A0200L);
writeKernelMemory((void *)0x04CC55D8,0x2C0A0200L);
writeKernelMemory((void *)0x04CC55DC,0x4182025CL);
writeKernelMemory((void *)0x04CC55E0,0x728A0800L);
writeKernelMemory((void *)0x04CC55E4,0x2C0A0800L);
writeKernelMemory((void *)0x04CC55E8,0x41820288L);
writeKernelMemory((void *)0x04CC55EC,0x728A0400L);
writeKernelMemory((void *)0x04CC55F0,0x2C0A0400L);
writeKernelMemory((void *)0x04CC55F4,0x4182033CL);
writeKernelMemory((void *)0x04CC55F8,0x4800040DL);
writeKernelMemory((void *)0x04CC55FC,0x48000418L);
writeKernelMemory((void *)0x04CC5600,0x8182FCECL);
writeKernelMemory((void *)0x04CC5604,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5608,0x4182040CL);
writeKernelMemory((void *)0x04CC560C,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5610,0x2C0C0004L);
writeKernelMemory((void *)0x04CC5614,0x40820400L);
writeKernelMemory((void *)0x04CC5618,0x81990034L);
writeKernelMemory((void *)0x04CC561C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5620,0x418201D4L);
writeKernelMemory((void *)0x04CC5624,0x3D403000L);
writeKernelMemory((void *)0x04CC5628,0x81821030L);
writeKernelMemory((void *)0x04CC562C,0x918AFFF0L);
writeKernelMemory((void *)0x04CC5630,0x81821034L);
writeKernelMemory((void *)0x04CC5634,0x918AFFF8L);
writeKernelMemory((void *)0x04CC5638,0x81821038L);
writeKernelMemory((void *)0x04CC563C,0x918AFFF4L);
writeKernelMemory((void *)0x04CC5640,0x8182103CL);
writeKernelMemory((void *)0x04CC5644,0x1D8C0008L);
writeKernelMemory((void *)0x04CC5648,0x918AFFFCL);
writeKernelMemory((void *)0x04CC564C,0x9421FF88L);
writeKernelMemory((void *)0x04CC5650,0x7C0802A6L);
writeKernelMemory((void *)0x04CC5654,0x3D40109CL);
writeKernelMemory((void *)0x04CC5658,0x3D202FFFL);
writeKernelMemory((void *)0x04CC565C,0x9001007CL);
writeKernelMemory((void *)0x04CC5660,0x614AD8E4L);
writeKernelMemory((void *)0x04CC5664,0x3D002FFFL);
writeKernelMemory((void *)0x04CC5668,0x6129FFFCL);
writeKernelMemory((void *)0x04CC566C,0x80CA0000L);
writeKernelMemory((void *)0x04CC5670,0x6108FFF0L);
writeKernelMemory((void *)0x04CC5674,0x80E90000L);
writeKernelMemory((void *)0x04CC5678,0x3D402FFFL);
writeKernelMemory((void *)0x04CC567C,0x80C60034L);
writeKernelMemory((void *)0x04CC5680,0x3D202FFFL);
writeKernelMemory((void *)0x04CC5684,0x93C10070L);
writeKernelMemory((void *)0x04CC5688,0x614AFFF4L);
writeKernelMemory((void *)0x04CC568C,0x80C600F8L);
writeKernelMemory((void *)0x04CC5690,0x6129FFF8L);
writeKernelMemory((void *)0x04CC5694,0x83C80000L);
writeKernelMemory((void *)0x04CC5698,0x810600C8L);
writeKernelMemory((void *)0x04CC569C,0x93E10074L);
writeKernelMemory((void *)0x04CC56A0,0x3BE00000L);
writeKernelMemory((void *)0x04CC56A4,0x93610064L);
writeKernelMemory((void *)0x04CC56A8,0x93810068L);
writeKernelMemory((void *)0x04CC56AC,0x83690000L);
writeKernelMemory((void *)0x04CC56B0,0x838A0000L);
writeKernelMemory((void *)0x04CC56B4,0x93A1006CL);
writeKernelMemory((void *)0x04CC56B8,0x7FA8382EL);
writeKernelMemory((void *)0x04CC56BC,0x93E10050L);
writeKernelMemory((void *)0x04CC56C0,0x93E10054L);
writeKernelMemory((void *)0x04CC56C4,0x48000011L);
writeKernelMemory((void *)0x04CC56C8,0x00670069L);
writeKernelMemory((void *)0x04CC56CC,0x00760065L);
writeKernelMemory((void *)0x04CC56D0,0x00000000L);
writeKernelMemory((void *)0x04CC56D4,0x7C8802A6L);
writeKernelMemory((void *)0x04CC56D8,0x3D20020BL);
writeKernelMemory((void *)0x04CC56DC,0x38610008L);
writeKernelMemory((void *)0x04CC56E0,0x612908D4L);
writeKernelMemory((void *)0x04CC56E4,0x7D2903A6L);
writeKernelMemory((void *)0x04CC56E8,0x4E800421L);
writeKernelMemory((void *)0x04CC56EC,0x3D200246L);
writeKernelMemory((void *)0x04CC56F0,0x61290E54L);
writeKernelMemory((void *)0x04CC56F4,0x80810008L);
writeKernelMemory((void *)0x04CC56F8,0x7D2903A6L);
writeKernelMemory((void *)0x04CC56FC,0x81210010L);
writeKernelMemory((void *)0x04CC5700,0x8061000CL);
writeKernelMemory((void *)0x04CC5704,0x7F67DB78L);
writeKernelMemory((void *)0x04CC5708,0x81810014L);
writeKernelMemory((void *)0x04CC570C,0x7F86E378L);
writeKernelMemory((void *)0x04CC5710,0x80010018L);
writeKernelMemory((void *)0x04CC5714,0x7FC5F378L);
writeKernelMemory((void *)0x04CC5718,0x8161001CL);
writeKernelMemory((void *)0x04CC571C,0x39010028L);
writeKernelMemory((void *)0x04CC5720,0x81410020L);
writeKernelMemory((void *)0x04CC5724,0x91210030L);
writeKernelMemory((void *)0x04CC5728,0x81210024L);
writeKernelMemory((void *)0x04CC572C,0x90810028L);
writeKernelMemory((void *)0x04CC5730,0x38810048L);
writeKernelMemory((void *)0x04CC5734,0x9061002CL);
writeKernelMemory((void *)0x04CC5738,0x38610050L);
writeKernelMemory((void *)0x04CC573C,0x91210044L);
writeKernelMemory((void *)0x04CC5740,0x93A10048L);
writeKernelMemory((void *)0x04CC5744,0x93E1004CL);
writeKernelMemory((void *)0x04CC5748,0x91810034L);
writeKernelMemory((void *)0x04CC574C,0x90010038L);
writeKernelMemory((void *)0x04CC5750,0x9161003CL);
writeKernelMemory((void *)0x04CC5754,0x91410040L);
writeKernelMemory((void *)0x04CC5758,0x4E800421L);
writeKernelMemory((void *)0x04CC575C,0x81210040L);
writeKernelMemory((void *)0x04CC5760,0x28090007L);
writeKernelMemory((void *)0x04CC5764,0x40810018L);
writeKernelMemory((void *)0x04CC5768,0x3D200382L);
writeKernelMemory((void *)0x04CC576C,0x8061003CL);
writeKernelMemory((void *)0x04CC5770,0x6129ABB4L);
writeKernelMemory((void *)0x04CC5774,0x7D2903A6L);
writeKernelMemory((void *)0x04CC5778,0x4E800421L);
writeKernelMemory((void *)0x04CC577C,0x81210020L);
writeKernelMemory((void *)0x04CC5780,0x28090007L);
writeKernelMemory((void *)0x04CC5784,0x40810018L);
writeKernelMemory((void *)0x04CC5788,0x3D200382L);
writeKernelMemory((void *)0x04CC578C,0x8061001CL);
writeKernelMemory((void *)0x04CC5790,0x6129ABB4L);
writeKernelMemory((void *)0x04CC5794,0x7D2903A6L);
writeKernelMemory((void *)0x04CC5798,0x4E800421L);
writeKernelMemory((void *)0x04CC579C,0x3D40109CL);
writeKernelMemory((void *)0x04CC57A0,0x3D200304L);
writeKernelMemory((void *)0x04CC57A4,0x614AD8E4L);
writeKernelMemory((void *)0x04CC57A8,0x6129A5D8L);
writeKernelMemory((void *)0x04CC57AC,0x814A0000L);
writeKernelMemory((void *)0x04CC57B0,0x7D2903A6L);
writeKernelMemory((void *)0x04CC57B4,0x80C10050L);
writeKernelMemory((void *)0x04CC57B8,0x38810058L);
writeKernelMemory((void *)0x04CC57BC,0x812A0034L);
writeKernelMemory((void *)0x04CC57C0,0x80E10054L);
writeKernelMemory((void *)0x04CC57C4,0x80690878L);
writeKernelMemory((void *)0x04CC57C8,0x90C10058L);
writeKernelMemory((void *)0x04CC57CC,0x90E1005CL);
writeKernelMemory((void *)0x04CC57D0,0x4E800421L);
writeKernelMemory((void *)0x04CC57D4,0x8001007CL);
writeKernelMemory((void *)0x04CC57D8,0x83610064L);
writeKernelMemory((void *)0x04CC57DC,0x83810068L);
writeKernelMemory((void *)0x04CC57E0,0x7C0803A6L);
writeKernelMemory((void *)0x04CC57E4,0x83A1006CL);
writeKernelMemory((void *)0x04CC57E8,0x83C10070L);
writeKernelMemory((void *)0x04CC57EC,0x83E10074L);
writeKernelMemory((void *)0x04CC57F0,0x38210078L);
writeKernelMemory((void *)0x04CC57F4,0x39800001L);
writeKernelMemory((void *)0x04CC57F8,0x9182FCECL);
writeKernelMemory((void *)0x04CC57FC,0x48000218L);
writeKernelMemory((void *)0x04CC5800,0x8182FCECL);
writeKernelMemory((void *)0x04CC5804,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5808,0x4182020CL);
writeKernelMemory((void *)0x04CC580C,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5810,0x2C0C0004L);
writeKernelMemory((void *)0x04CC5814,0x41820018L);
writeKernelMemory((void *)0x04CC5818,0x398C0001L);
writeKernelMemory((void *)0x04CC581C,0x9182FCF0L);
writeKernelMemory((void *)0x04CC5820,0x39800001L);
writeKernelMemory((void *)0x04CC5824,0x9182FCECL);
writeKernelMemory((void *)0x04CC5828,0x480001ECL);
writeKernelMemory((void *)0x04CC582C,0x39800000L);
writeKernelMemory((void *)0x04CC5830,0x9182FCF0L);
writeKernelMemory((void *)0x04CC5834,0x4BFFFFECL);
writeKernelMemory((void *)0x04CC5838,0x8182FCECL);
writeKernelMemory((void *)0x04CC583C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5840,0x418201D4L);
writeKernelMemory((void *)0x04CC5844,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5848,0x2C0C0000L);
writeKernelMemory((void *)0x04CC584C,0x41820018L);
writeKernelMemory((void *)0x04CC5850,0x398CFFFFL);
writeKernelMemory((void *)0x04CC5854,0x9182FCF0L);
writeKernelMemory((void *)0x04CC5858,0x39800001L);
writeKernelMemory((void *)0x04CC585C,0x9182FCECL);
writeKernelMemory((void *)0x04CC5860,0x480001B4L);
writeKernelMemory((void *)0x04CC5864,0x39800004L);
writeKernelMemory((void *)0x04CC5868,0x9182FCF0L);
writeKernelMemory((void *)0x04CC586C,0x4BFFFFECL);
writeKernelMemory((void *)0x04CC5870,0x81820038L);
writeKernelMemory((void *)0x04CC5874,0x398C0001L);
writeKernelMemory((void *)0x04CC5878,0x91820038L);
writeKernelMemory((void *)0x04CC587C,0x2C0C004AL);
writeKernelMemory((void *)0x04CC5880,0x40800048L);
writeKernelMemory((void *)0x04CC5884,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5888,0x41820040L);
writeKernelMemory((void *)0x04CC588C,0x2C0C0010L);
writeKernelMemory((void *)0x04CC5890,0x41820038L);
writeKernelMemory((void *)0x04CC5894,0x2C0C0020L);
writeKernelMemory((void *)0x04CC5898,0x41820030L);
writeKernelMemory((void *)0x04CC589C,0x2C0C002AL);
writeKernelMemory((void *)0x04CC58A0,0x41820028L);
writeKernelMemory((void *)0x04CC58A4,0x2C0C0030L);
writeKernelMemory((void *)0x04CC58A8,0x41820020L);
writeKernelMemory((void *)0x04CC58AC,0x2C0C003AL);
writeKernelMemory((void *)0x04CC58B0,0x41820018L);
writeKernelMemory((void *)0x04CC58B4,0x2C0C0040L);
writeKernelMemory((void *)0x04CC58B8,0x41820010L);
writeKernelMemory((void *)0x04CC58BC,0x2C0C0045L);
writeKernelMemory((void *)0x04CC58C0,0x41820008L);
writeKernelMemory((void *)0x04CC58C4,0x48000150L);
writeKernelMemory((void *)0x04CC58C8,0x8122FCF0L);
writeKernelMemory((void *)0x04CC58CC,0x1D290004L);
writeKernelMemory((void *)0x04CC58D0,0x39291030L);
writeKernelMemory((void *)0x04CC58D4,0x7D89102EL);
writeKernelMemory((void *)0x04CC58D8,0x2C091030L);
writeKernelMemory((void *)0x04CC58DC,0x40820014L);
writeKernelMemory((void *)0x04CC58E0,0x2C0C0002L);
writeKernelMemory((void *)0x04CC58E4,0x4080000CL);
writeKernelMemory((void *)0x04CC58E8,0x394008DCL);
writeKernelMemory((void *)0x04CC58EC,0x7D49112EL);
writeKernelMemory((void *)0x04CC58F0,0x2C091038L);
writeKernelMemory((void *)0x04CC58F4,0x40820014L);
writeKernelMemory((void *)0x04CC58F8,0x2C0C0002L);
writeKernelMemory((void *)0x04CC58FC,0x4080000CL);
writeKernelMemory((void *)0x04CC5900,0x39400041L);
writeKernelMemory((void *)0x04CC5904,0x7D49112EL);
writeKernelMemory((void *)0x04CC5908,0x2C09103CL);
writeKernelMemory((void *)0x04CC590C,0x40820014L);
writeKernelMemory((void *)0x04CC5910,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5914,0x4080000CL);
writeKernelMemory((void *)0x04CC5918,0x39400001L);
writeKernelMemory((void *)0x04CC591C,0x7D49112EL);
writeKernelMemory((void *)0x04CC5920,0x7D89102EL);
writeKernelMemory((void *)0x04CC5924,0x398CFFFFL);
writeKernelMemory((void *)0x04CC5928,0x7D89112EL);
writeKernelMemory((void *)0x04CC592C,0x480000E8L);
writeKernelMemory((void *)0x04CC5930,0x81820038L);
writeKernelMemory((void *)0x04CC5934,0x398C0001L);
writeKernelMemory((void *)0x04CC5938,0x91820038L);
writeKernelMemory((void *)0x04CC593C,0x2C0C004AL);
writeKernelMemory((void *)0x04CC5940,0x40800048L);
writeKernelMemory((void *)0x04CC5944,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5948,0x41820040L);
writeKernelMemory((void *)0x04CC594C,0x2C0C0010L);
writeKernelMemory((void *)0x04CC5950,0x41820038L);
writeKernelMemory((void *)0x04CC5954,0x2C0C0020L);
writeKernelMemory((void *)0x04CC5958,0x41820030L);
writeKernelMemory((void *)0x04CC595C,0x2C0C002AL);
writeKernelMemory((void *)0x04CC5960,0x41820028L);
writeKernelMemory((void *)0x04CC5964,0x2C0C0030L);
writeKernelMemory((void *)0x04CC5968,0x41820020L);
writeKernelMemory((void *)0x04CC596C,0x2C0C003AL);
writeKernelMemory((void *)0x04CC5970,0x41820018L);
writeKernelMemory((void *)0x04CC5974,0x2C0C0040L);
writeKernelMemory((void *)0x04CC5978,0x41820010L);
writeKernelMemory((void *)0x04CC597C,0x2C0C0045L);
writeKernelMemory((void *)0x04CC5980,0x41820008L);
writeKernelMemory((void *)0x04CC5984,0x48000090L);
writeKernelMemory((void *)0x04CC5988,0x8122FCF0L);
writeKernelMemory((void *)0x04CC598C,0x1D290004L);
writeKernelMemory((void *)0x04CC5990,0x39291030L);
writeKernelMemory((void *)0x04CC5994,0x7D89102EL);
writeKernelMemory((void *)0x04CC5998,0x2C091030L);
writeKernelMemory((void *)0x04CC599C,0x40820014L);
writeKernelMemory((void *)0x04CC59A0,0x2C0C08DAL);
writeKernelMemory((void *)0x04CC59A4,0x4081000CL);
writeKernelMemory((void *)0x04CC59A8,0x39400000L);
writeKernelMemory((void *)0x04CC59AC,0x7D49112EL);
writeKernelMemory((void *)0x04CC59B0,0x2C091038L);
writeKernelMemory((void *)0x04CC59B4,0x40820014L);
writeKernelMemory((void *)0x04CC59B8,0x2C0C003FL);
writeKernelMemory((void *)0x04CC59BC,0x4081000CL);
writeKernelMemory((void *)0x04CC59C0,0x39400000L);
writeKernelMemory((void *)0x04CC59C4,0x7D49112EL);
writeKernelMemory((void *)0x04CC59C8,0x2C09103CL);
writeKernelMemory((void *)0x04CC59CC,0x40820028L);
writeKernelMemory((void *)0x04CC59D0,0x3D60109DL);
writeKernelMemory((void *)0x04CC59D4,0x816B1DDCL);
writeKernelMemory((void *)0x04CC59D8,0x2C0B0000L);
writeKernelMemory((void *)0x04CC59DC,0x41820018L);
writeKernelMemory((void *)0x04CC59E0,0x816B0F02L);
writeKernelMemory((void *)0x04CC59E4,0x396BFFFFL);
writeKernelMemory((void *)0x04CC59E8,0x7C0C5800L);
writeKernelMemory((void *)0x04CC59EC,0x40810008L);
writeKernelMemory((void *)0x04CC59F0,0x7D69112EL);
writeKernelMemory((void *)0x04CC59F4,0x7D89102EL);
writeKernelMemory((void *)0x04CC59F8,0x398C0001L);
writeKernelMemory((void *)0x04CC59FC,0x7D89112EL);
writeKernelMemory((void *)0x04CC5A00,0x48000014L);
writeKernelMemory((void *)0x04CC5A04,0x39400000L);
writeKernelMemory((void *)0x04CC5A08,0x9142FCECL);
writeKernelMemory((void *)0x04CC5A0C,0x91420038L);
writeKernelMemory((void *)0x04CC5A10,0x4E800020L);
writeKernelMemory((void *)0x04CC5A14,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5A18,0x4E800421L);
writeKernelMemory((void *)0x04CC5A1C,0x3D804000L);
writeKernelMemory((void *)0x04CC5A20,0x91820418L);
writeKernelMemory((void *)0x04CC5A24,0xC0220418L);
writeKernelMemory((void *)0x04CC5A28,0xC0420418L);
writeKernelMemory((void *)0x04CC5A2C,0x7EE903A6L);
writeKernelMemory((void *)0x04CC5A30,0x4E800421L);
writeKernelMemory((void *)0x04CC5A34,0x3D8042BAL);
writeKernelMemory((void *)0x04CC5A38,0x9182F03CL);
writeKernelMemory((void *)0x04CC5A3C,0xC022F03CL);
writeKernelMemory((void *)0x04CC5A40,0x3D804170L);
writeKernelMemory((void *)0x04CC5A44,0x9182FEFCL);
writeKernelMemory((void *)0x04CC5A48,0xC042FEFCL);
writeKernelMemory((void *)0x04CC5A4C,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5A50,0x4E800421L);
writeKernelMemory((void *)0x04CC5A54,0x38821016L);
writeKernelMemory((void *)0x04CC5A58,0x38620348L);
writeKernelMemory((void *)0x04CC5A5C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5A60,0x4E800421L);
writeKernelMemory((void *)0x04CC5A64,0x480007BDL);
writeKernelMemory((void *)0x04CC5A68,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5A6C,0x4E800421L);
writeKernelMemory((void *)0x04CC5A70,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5A74,0x4E800421L);
writeKernelMemory((void *)0x04CC5A78,0x48000789L);
writeKernelMemory((void *)0x04CC5A7C,0x3D80433AL);
writeKernelMemory((void *)0x04CC5A80,0x918210E0L);
writeKernelMemory((void *)0x04CC5A84,0xC02210E0L);
writeKernelMemory((void *)0x04CC5A88,0x480007DDL);
writeKernelMemory((void *)0x04CC5A8C,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5A90,0x4E800421L);
writeKernelMemory((void *)0x04CC5A94,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5A98,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5A9C,0x4082000CL);
writeKernelMemory((void *)0x04CC5AA0,0x38821040L);
writeKernelMemory((void *)0x04CC5AA4,0x48000008L);
writeKernelMemory((void *)0x04CC5AA8,0x38821042L);
writeKernelMemory((void *)0x04CC5AAC,0x38620348L);
writeKernelMemory((void *)0x04CC5AB0,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5AB4,0x4E800421L);
writeKernelMemory((void *)0x04CC5AB8,0x48000769L);
writeKernelMemory((void *)0x04CC5ABC,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5AC0,0x4E800421L);
writeKernelMemory((void *)0x04CC5AC4,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5AC8,0x4E800421L);
writeKernelMemory((void *)0x04CC5ACC,0x48000735L);
writeKernelMemory((void *)0x04CC5AD0,0xC02210E0L);
writeKernelMemory((void *)0x04CC5AD4,0x48000791L);
writeKernelMemory((void *)0x04CC5AD8,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5ADC,0x4E800421L);
writeKernelMemory((void *)0x04CC5AE0,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5AE4,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5AE8,0x4082000CL);
writeKernelMemory((void *)0x04CC5AEC,0x38821070L);
writeKernelMemory((void *)0x04CC5AF0,0x48000008L);
writeKernelMemory((void *)0x04CC5AF4,0x38821072L);
writeKernelMemory((void *)0x04CC5AF8,0x38620348L);
writeKernelMemory((void *)0x04CC5AFC,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5B00,0x4E800421L);
writeKernelMemory((void *)0x04CC5B04,0x4800071DL);
writeKernelMemory((void *)0x04CC5B08,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5B0C,0x4E800421L);
writeKernelMemory((void *)0x04CC5B10,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5B14,0x4E800421L);
writeKernelMemory((void *)0x04CC5B18,0x480006E9L);
writeKernelMemory((void *)0x04CC5B1C,0xC02210E0L);
writeKernelMemory((void *)0x04CC5B20,0x48000745L);
writeKernelMemory((void *)0x04CC5B24,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5B28,0x4E800421L);
writeKernelMemory((void *)0x04CC5B2C,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5B30,0x2C0C0002L);
writeKernelMemory((void *)0x04CC5B34,0x4082000CL);
writeKernelMemory((void *)0x04CC5B38,0x388210B0L);
writeKernelMemory((void *)0x04CC5B3C,0x48000008L);
writeKernelMemory((void *)0x04CC5B40,0x388210B2L);
writeKernelMemory((void *)0x04CC5B44,0x38620348L);
writeKernelMemory((void *)0x04CC5B48,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5B4C,0x4E800421L);
writeKernelMemory((void *)0x04CC5B50,0x480006D1L);
writeKernelMemory((void *)0x04CC5B54,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5B58,0x4E800421L);
writeKernelMemory((void *)0x04CC5B5C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5B60,0x4E800421L);
writeKernelMemory((void *)0x04CC5B64,0x4800069DL);
writeKernelMemory((void *)0x04CC5B68,0xC02210E0L);
writeKernelMemory((void *)0x04CC5B6C,0x480006F9L);
writeKernelMemory((void *)0x04CC5B70,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5B74,0x4E800421L);
writeKernelMemory((void *)0x04CC5B78,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5B7C,0x2C0C0003L);
writeKernelMemory((void *)0x04CC5B80,0x4082000CL);
writeKernelMemory((void *)0x04CC5B84,0x38820040L);
writeKernelMemory((void *)0x04CC5B88,0x48000008L);
writeKernelMemory((void *)0x04CC5B8C,0x38820042L);
writeKernelMemory((void *)0x04CC5B90,0x38620348L);
writeKernelMemory((void *)0x04CC5B94,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5B98,0x4E800421L);
writeKernelMemory((void *)0x04CC5B9C,0x48000685L);
writeKernelMemory((void *)0x04CC5BA0,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5BA4,0x4E800421L);
writeKernelMemory((void *)0x04CC5BA8,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5BAC,0x4E800421L);
writeKernelMemory((void *)0x04CC5BB0,0x48000651L);
writeKernelMemory((void *)0x04CC5BB4,0x3D804340L);
writeKernelMemory((void *)0x04CC5BB8,0x918210E0L);
writeKernelMemory((void *)0x04CC5BBC,0xC02210E0L);
writeKernelMemory((void *)0x04CC5BC0,0x3D8042DAL);
writeKernelMemory((void *)0x04CC5BC4,0x918210E0L);
writeKernelMemory((void *)0x04CC5BC8,0xC04210E0L);
writeKernelMemory((void *)0x04CC5BCC,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5BD0,0x4E800421L);
writeKernelMemory((void *)0x04CC5BD4,0x8182FCF0L);
writeKernelMemory((void *)0x04CC5BD8,0x2C0C0004L);
writeKernelMemory((void *)0x04CC5BDC,0x4082000CL);
writeKernelMemory((void *)0x04CC5BE0,0x388210F0L);
writeKernelMemory((void *)0x04CC5BE4,0x48000008L);
writeKernelMemory((void *)0x04CC5BE8,0x388210F2L);
writeKernelMemory((void *)0x04CC5BEC,0x38620348L);
writeKernelMemory((void *)0x04CC5BF0,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5BF4,0x4E800421L);
writeKernelMemory((void *)0x04CC5BF8,0x48000629L);
writeKernelMemory((void *)0x04CC5BFC,0x7F8903A6L);
writeKernelMemory((void *)0x04CC5C00,0x4E800421L);
writeKernelMemory((void *)0x04CC5C04,0x480006F4L);
writeKernelMemory((void *)0x04CC5C08,0x81990034L);
writeKernelMemory((void *)0x04CC5C0C,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5C10,0x4182001CL);
writeKernelMemory((void *)0x04CC5C14,0x3D600100L);
writeKernelMemory((void *)0x04CC5C18,0x916C0814L);
writeKernelMemory((void *)0x04CC5C1C,0x8162FE00L);
writeKernelMemory((void *)0x04CC5C20,0x916C0148L);
writeKernelMemory((void *)0x04CC5C24,0x8162FDFCL);
writeKernelMemory((void *)0x04CC5C28,0x916C014CL);
writeKernelMemory((void *)0x04CC5C2C,0x39800000L);
writeKernelMemory((void *)0x04CC5C30,0x9182FC4CL);
writeKernelMemory((void *)0x04CC5C34,0x3D80108DL);
writeKernelMemory((void *)0x04CC5C38,0x3D20C5A0L);
writeKernelMemory((void *)0x04CC5C3C,0x912CB504L);
writeKernelMemory((void *)0x04CC5C40,0x3D80102FL);
writeKernelMemory((void *)0x04CC5C44,0x814CFA63L);
writeKernelMemory((void *)0x04CC5C48,0x2C0A0080L);
writeKernelMemory((void *)0x04CC5C4C,0x4182003CL);
writeKernelMemory((void *)0x04CC5C50,0x728A0100L);
writeKernelMemory((void *)0x04CC5C54,0x2C0A0100L);
writeKernelMemory((void *)0x04CC5C58,0x4182013CL);
writeKernelMemory((void *)0x04CC5C5C,0x728A0200L);
writeKernelMemory((void *)0x04CC5C60,0x2C0A0200L);
writeKernelMemory((void *)0x04CC5C64,0x41820168L);
writeKernelMemory((void *)0x04CC5C68,0x728A0800L);
writeKernelMemory((void *)0x04CC5C6C,0x2C0A0800L);
writeKernelMemory((void *)0x04CC5C70,0x41820194L);
writeKernelMemory((void *)0x04CC5C74,0x728A0400L);
writeKernelMemory((void *)0x04CC5C78,0x2C0A0400L);
writeKernelMemory((void *)0x04CC5C7C,0x4182024CL);
writeKernelMemory((void *)0x04CC5C80,0x48000321L);
writeKernelMemory((void *)0x04CC5C84,0x4800032CL);
writeKernelMemory((void *)0x04CC5C88,0x81821114L);
writeKernelMemory((void *)0x04CC5C8C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5C90,0x41820320L);
writeKernelMemory((void *)0x04CC5C94,0x81821110L);
writeKernelMemory((void *)0x04CC5C98,0x2C0C0003L);
writeKernelMemory((void *)0x04CC5C9C,0x40820314L);
writeKernelMemory((void *)0x04CC5CA0,0x81990034L);
writeKernelMemory((void *)0x04CC5CA4,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5CA8,0x418200E0L);
writeKernelMemory((void *)0x04CC5CAC,0x3D403000L);
writeKernelMemory((void *)0x04CC5CB0,0x81821100L);
writeKernelMemory((void *)0x04CC5CB4,0x918AFFE0L);
writeKernelMemory((void *)0x04CC5CB8,0x81821104L);
writeKernelMemory((void *)0x04CC5CBC,0x918AFFE4L);
writeKernelMemory((void *)0x04CC5CC0,0x8182103CL);
writeKernelMemory((void *)0x04CC5CC4,0x1D8C0008L);
writeKernelMemory((void *)0x04CC5CC8,0x918AFFE8L);
writeKernelMemory((void *)0x04CC5CCC,0x9421FFD0L);
writeKernelMemory((void *)0x04CC5CD0,0x7C0802A6L);
writeKernelMemory((void *)0x04CC5CD4,0x3CE02FFFL);
writeKernelMemory((void *)0x04CC5CD8,0x3D002FFFL);
writeKernelMemory((void *)0x04CC5CDC,0x90010034L);
writeKernelMemory((void *)0x04CC5CE0,0x3D402FFFL);
writeKernelMemory((void *)0x04CC5CE4,0x39200000L);
writeKernelMemory((void *)0x04CC5CE8,0x60E7FFE0L);
writeKernelMemory((void *)0x04CC5CEC,0x6108FFE4L);
writeKernelMemory((void *)0x04CC5CF0,0x614AFFE8L);
writeKernelMemory((void *)0x04CC5CF4,0x80C80000L);
writeKernelMemory((void *)0x04CC5CF8,0x38810018L);
writeKernelMemory((void *)0x04CC5CFC,0x810A0000L);
writeKernelMemory((void *)0x04CC5D00,0x3D40022FL);
writeKernelMemory((void *)0x04CC5D04,0x93E1002CL);
writeKernelMemory((void *)0x04CC5D08,0x3FE0109CL);
writeKernelMemory((void *)0x04CC5D0C,0x80A70000L);
writeKernelMemory((void *)0x04CC5D10,0x63FFD8E4L);
writeKernelMemory((void *)0x04CC5D14,0x9121000CL);
writeKernelMemory((void *)0x04CC5D18,0x614A1518L);
writeKernelMemory((void *)0x04CC5D1C,0x7D4903A6L);
writeKernelMemory((void *)0x04CC5D20,0x38610008L);
writeKernelMemory((void *)0x04CC5D24,0x815F0000L);
writeKernelMemory((void *)0x04CC5D28,0x91210008L);
writeKernelMemory((void *)0x04CC5D2C,0x814A0034L);
writeKernelMemory((void *)0x04CC5D30,0x814A00F8L);
writeKernelMemory((void *)0x04CC5D34,0x814A00C8L);
writeKernelMemory((void *)0x04CC5D38,0x7D4A402EL);
writeKernelMemory((void *)0x04CC5D3C,0x9121001CL);
writeKernelMemory((void *)0x04CC5D40,0x91410018L);
writeKernelMemory((void *)0x04CC5D44,0x4E800421L);
writeKernelMemory((void *)0x04CC5D48,0x811F0000L);
writeKernelMemory((void *)0x04CC5D4C,0x81410008L);
writeKernelMemory((void *)0x04CC5D50,0x3D200304L);
writeKernelMemory((void *)0x04CC5D54,0x6129A5D8L);
writeKernelMemory((void *)0x04CC5D58,0x81080034L);
writeKernelMemory((void *)0x04CC5D5C,0x8161000CL);
writeKernelMemory((void *)0x04CC5D60,0x38810020L);
writeKernelMemory((void *)0x04CC5D64,0x80680878L);
writeKernelMemory((void *)0x04CC5D68,0x7D2903A6L);
writeKernelMemory((void *)0x04CC5D6C,0x91410020L);
writeKernelMemory((void *)0x04CC5D70,0x91610024L);
writeKernelMemory((void *)0x04CC5D74,0x4E800421L);
writeKernelMemory((void *)0x04CC5D78,0x80010034L);
writeKernelMemory((void *)0x04CC5D7C,0x83E1002CL);
writeKernelMemory((void *)0x04CC5D80,0x7C0803A6L);
writeKernelMemory((void *)0x04CC5D84,0x38210030L);
writeKernelMemory((void *)0x04CC5D88,0x39800001L);
writeKernelMemory((void *)0x04CC5D8C,0x91821114L);
writeKernelMemory((void *)0x04CC5D90,0x48000220L);
writeKernelMemory((void *)0x04CC5D94,0x81821114L);
writeKernelMemory((void *)0x04CC5D98,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5D9C,0x41820214L);
writeKernelMemory((void *)0x04CC5DA0,0x81821110L);
writeKernelMemory((void *)0x04CC5DA4,0x2C0C0003L);
writeKernelMemory((void *)0x04CC5DA8,0x41820018L);
writeKernelMemory((void *)0x04CC5DAC,0x398C0001L);
writeKernelMemory((void *)0x04CC5DB0,0x91821110L);
writeKernelMemory((void *)0x04CC5DB4,0x39800001L);
writeKernelMemory((void *)0x04CC5DB8,0x91821114L);
writeKernelMemory((void *)0x04CC5DBC,0x480001F4L);
writeKernelMemory((void *)0x04CC5DC0,0x39800000L);
writeKernelMemory((void *)0x04CC5DC4,0x91821110L);
writeKernelMemory((void *)0x04CC5DC8,0x4BFFFFECL);
writeKernelMemory((void *)0x04CC5DCC,0x81821114L);
writeKernelMemory((void *)0x04CC5DD0,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5DD4,0x418201DCL);
writeKernelMemory((void *)0x04CC5DD8,0x81821110L);
writeKernelMemory((void *)0x04CC5DDC,0x2C0C0000L);
writeKernelMemory((void *)0x04CC5DE0,0x41820018L);
writeKernelMemory((void *)0x04CC5DE4,0x398CFFFFL);
writeKernelMemory((void *)0x04CC5DE8,0x91821110L);
writeKernelMemory((void *)0x04CC5DEC,0x39800001L);
writeKernelMemory((void *)0x04CC5DF0,0x91821114L);
writeKernelMemory((void *)0x04CC5DF4,0x480001BCL);
writeKernelMemory((void *)0x04CC5DF8,0x39800003L);
writeKernelMemory((void *)0x04CC5DFC,0x91821110L);
writeKernelMemory((void *)0x04CC5E00,0x4BFFFFECL);
writeKernelMemory((void *)0x04CC5E04,0x81820038L);
writeKernelMemory((void *)0x04CC5E08,0x398C0001L);
writeKernelMemory((void *)0x04CC5E0C,0x91820038L);
writeKernelMemory((void *)0x04CC5E10,0x2C0C004AL);
writeKernelMemory((void *)0x04CC5E14,0x40800048L);
writeKernelMemory((void *)0x04CC5E18,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5E1C,0x41820040L);
writeKernelMemory((void *)0x04CC5E20,0x2C0C0010L);
writeKernelMemory((void *)0x04CC5E24,0x41820038L);
writeKernelMemory((void *)0x04CC5E28,0x2C0C0020L);
writeKernelMemory((void *)0x04CC5E2C,0x41820030L);
writeKernelMemory((void *)0x04CC5E30,0x2C0C002AL);
writeKernelMemory((void *)0x04CC5E34,0x41820028L);
writeKernelMemory((void *)0x04CC5E38,0x2C0C0030L);
writeKernelMemory((void *)0x04CC5E3C,0x41820020L);
writeKernelMemory((void *)0x04CC5E40,0x2C0C003AL);
writeKernelMemory((void *)0x04CC5E44,0x41820018L);
writeKernelMemory((void *)0x04CC5E48,0x2C0C0040L);
writeKernelMemory((void *)0x04CC5E4C,0x41820010L);
writeKernelMemory((void *)0x04CC5E50,0x2C0C0045L);
writeKernelMemory((void *)0x04CC5E54,0x41820008L);
writeKernelMemory((void *)0x04CC5E58,0x48000158L);
writeKernelMemory((void *)0x04CC5E5C,0x81221110L);
writeKernelMemory((void *)0x04CC5E60,0x1D290004L);
writeKernelMemory((void *)0x04CC5E64,0x39291100L);
writeKernelMemory((void *)0x04CC5E68,0x7D89102EL);
writeKernelMemory((void *)0x04CC5E6C,0x2C091100L);
writeKernelMemory((void *)0x04CC5E70,0x40820014L);
writeKernelMemory((void *)0x04CC5E74,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5E78,0x4080000CL);
writeKernelMemory((void *)0x04CC5E7C,0x39400021L);
writeKernelMemory((void *)0x04CC5E80,0x7D49112EL);
writeKernelMemory((void *)0x04CC5E84,0x2C091104L);
writeKernelMemory((void *)0x04CC5E88,0x40820014L);
writeKernelMemory((void *)0x04CC5E8C,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5E90,0x4080000CL);
writeKernelMemory((void *)0x04CC5E94,0x39400001L);
writeKernelMemory((void *)0x04CC5E98,0x7D49112EL);
writeKernelMemory((void *)0x04CC5E9C,0x2C091108L);
writeKernelMemory((void *)0x04CC5EA0,0x40820018L);
writeKernelMemory((void *)0x04CC5EA4,0x3920103CL);
writeKernelMemory((void *)0x04CC5EA8,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5EAC,0x4080000CL);
writeKernelMemory((void *)0x04CC5EB0,0x39400001L);
writeKernelMemory((void *)0x04CC5EB4,0x7D49112EL);
writeKernelMemory((void *)0x04CC5EB8,0x7D89102EL);
writeKernelMemory((void *)0x04CC5EBC,0x398CFFFFL);
writeKernelMemory((void *)0x04CC5EC0,0x7D89112EL);
writeKernelMemory((void *)0x04CC5EC4,0x480000ECL);
writeKernelMemory((void *)0x04CC5EC8,0x81820038L);
writeKernelMemory((void *)0x04CC5ECC,0x398C0001L);
writeKernelMemory((void *)0x04CC5ED0,0x91820038L);
writeKernelMemory((void *)0x04CC5ED4,0x2C0C004AL);
writeKernelMemory((void *)0x04CC5ED8,0x40800048L);
writeKernelMemory((void *)0x04CC5EDC,0x2C0C0001L);
writeKernelMemory((void *)0x04CC5EE0,0x41820040L);
writeKernelMemory((void *)0x04CC5EE4,0x2C0C0010L);
writeKernelMemory((void *)0x04CC5EE8,0x41820038L);
writeKernelMemory((void *)0x04CC5EEC,0x2C0C0020L);
writeKernelMemory((void *)0x04CC5EF0,0x41820030L);
writeKernelMemory((void *)0x04CC5EF4,0x2C0C002AL);
writeKernelMemory((void *)0x04CC5EF8,0x41820028L);
writeKernelMemory((void *)0x04CC5EFC,0x2C0C0030L);
writeKernelMemory((void *)0x04CC5F00,0x41820020L);
writeKernelMemory((void *)0x04CC5F04,0x2C0C003AL);
writeKernelMemory((void *)0x04CC5F08,0x41820018L);
writeKernelMemory((void *)0x04CC5F0C,0x2C0C0040L);
writeKernelMemory((void *)0x04CC5F10,0x41820010L);
writeKernelMemory((void *)0x04CC5F14,0x2C0C0045L);
writeKernelMemory((void *)0x04CC5F18,0x41820008L);
writeKernelMemory((void *)0x04CC5F1C,0x48000094L);
writeKernelMemory((void *)0x04CC5F20,0x81221110L);
writeKernelMemory((void *)0x04CC5F24,0x1D290004L);
writeKernelMemory((void *)0x04CC5F28,0x39291100L);
writeKernelMemory((void *)0x04CC5F2C,0x7D89102EL);
writeKernelMemory((void *)0x04CC5F30,0x2C091100L);
writeKernelMemory((void *)0x04CC5F34,0x40820014L);
writeKernelMemory((void *)0x04CC5F38,0x2C0C001FL);
writeKernelMemory((void *)0x04CC5F3C,0x4081000CL);
writeKernelMemory((void *)0x04CC5F40,0x3940FFFFL);
writeKernelMemory((void *)0x04CC5F44,0x7D49112EL);
writeKernelMemory((void *)0x04CC5F48,0x2C091104L);
writeKernelMemory((void *)0x04CC5F4C,0x40820014L);
writeKernelMemory((void *)0x04CC5F50,0x2C0C0009L);
writeKernelMemory((void *)0x04CC5F54,0x4081000CL);
writeKernelMemory((void *)0x04CC5F58,0x39400009L);
writeKernelMemory((void *)0x04CC5F5C,0x7D49112EL);
writeKernelMemory((void *)0x04CC5F60,0x2C091108L);
writeKernelMemory((void *)0x04CC5F64,0x4082002CL);
writeKernelMemory((void *)0x04CC5F68,0x3920103CL);
writeKernelMemory((void *)0x04CC5F6C,0x3D60109DL);
writeKernelMemory((void *)0x04CC5F70,0x816B1DDCL);
writeKernelMemory((void *)0x04CC5F74,0x2C0B0000L);
writeKernelMemory((void *)0x04CC5F78,0x41820018L);
writeKernelMemory((void *)0x04CC5F7C,0x816B0F02L);
writeKernelMemory((void *)0x04CC5F80,0x396BFFFFL);
writeKernelMemory((void *)0x04CC5F84,0x7C0C5800L);
writeKernelMemory((void *)0x04CC5F88,0x40810008L);
writeKernelMemory((void *)0x04CC5F8C,0x7D69112EL);
writeKernelMemory((void *)0x04CC5F90,0x7D89102EL);
writeKernelMemory((void *)0x04CC5F94,0x398C0001L);
writeKernelMemory((void *)0x04CC5F98,0x7D89112EL);
writeKernelMemory((void *)0x04CC5F9C,0x48000014L);
writeKernelMemory((void *)0x04CC5FA0,0x39400000L);
writeKernelMemory((void *)0x04CC5FA4,0x91421114L);
writeKernelMemory((void *)0x04CC5FA8,0x91420038L);
writeKernelMemory((void *)0x04CC5FAC,0x4E800020L);
writeKernelMemory((void *)0x04CC5FB0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC5FB4,0x4E800421L);
writeKernelMemory((void *)0x04CC5FB8,0x3D804000L);
writeKernelMemory((void *)0x04CC5FBC,0x91820418L);
writeKernelMemory((void *)0x04CC5FC0,0xC0220418L);
writeKernelMemory((void *)0x04CC5FC4,0xC0420418L);
writeKernelMemory((void *)0x04CC5FC8,0x7EE903A6L);
writeKernelMemory((void *)0x04CC5FCC,0x4E800421L);
writeKernelMemory((void *)0x04CC5FD0,0x3D8042AAL);
writeKernelMemory((void *)0x04CC5FD4,0x9182F03CL);
writeKernelMemory((void *)0x04CC5FD8,0xC022F03CL);
writeKernelMemory((void *)0x04CC5FDC,0x3D804170L);
writeKernelMemory((void *)0x04CC5FE0,0x9182FEFCL);
writeKernelMemory((void *)0x04CC5FE4,0xC042FEFCL);
writeKernelMemory((void *)0x04CC5FE8,0x7FA903A6L);
writeKernelMemory((void *)0x04CC5FEC,0x4E800421L);
writeKernelMemory((void *)0x04CC5FF0,0x38820066L);
writeKernelMemory((void *)0x04CC5FF4,0x38620348L);
writeKernelMemory((void *)0x04CC5FF8,0x7F4903A6L);
writeKernelMemory((void *)0x04CC5FFC,0x4E800421L);
writeKernelMemory((void *)0x04CC6000,0x48000221L);
writeKernelMemory((void *)0x04CC6004,0x7F8903A6L);
writeKernelMemory((void *)0x04CC6008,0x4E800421L);
writeKernelMemory((void *)0x04CC600C,0x7EC903A6L);
writeKernelMemory((void *)0x04CC6010,0x4E800421L);
writeKernelMemory((void *)0x04CC6014,0x480001EDL);
writeKernelMemory((void *)0x04CC6018,0x3D80433AL);
writeKernelMemory((void *)0x04CC601C,0x918210E0L);
writeKernelMemory((void *)0x04CC6020,0xC02210E0L);
writeKernelMemory((void *)0x04CC6024,0x48000241L);
writeKernelMemory((void *)0x04CC6028,0x7FA903A6L);
writeKernelMemory((void *)0x04CC602C,0x4E800421L);
writeKernelMemory((void *)0x04CC6030,0x81821110L);
writeKernelMemory((void *)0x04CC6034,0x2C0C0000L);
writeKernelMemory((void *)0x04CC6038,0x4082000CL);
writeKernelMemory((void *)0x04CC603C,0x38821120L);
writeKernelMemory((void *)0x04CC6040,0x48000008L);
writeKernelMemory((void *)0x04CC6044,0x38821122L);
writeKernelMemory((void *)0x04CC6048,0x38620348L);
writeKernelMemory((void *)0x04CC604C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC6050,0x4E800421L);
writeKernelMemory((void *)0x04CC6054,0x480001CDL);
writeKernelMemory((void *)0x04CC6058,0x7F8903A6L);
writeKernelMemory((void *)0x04CC605C,0x4E800421L);
writeKernelMemory((void *)0x04CC6060,0x7EC903A6L);
writeKernelMemory((void *)0x04CC6064,0x4E800421L);
writeKernelMemory((void *)0x04CC6068,0x48000199L);
writeKernelMemory((void *)0x04CC606C,0xC02210E0L);
writeKernelMemory((void *)0x04CC6070,0x480001F5L);
writeKernelMemory((void *)0x04CC6074,0x7FA903A6L);
writeKernelMemory((void *)0x04CC6078,0x4E800421L);
writeKernelMemory((void *)0x04CC607C,0x81821110L);
writeKernelMemory((void *)0x04CC6080,0x2C0C0001L);
writeKernelMemory((void *)0x04CC6084,0x4082000CL);
writeKernelMemory((void *)0x04CC6088,0x38820090L);
writeKernelMemory((void *)0x04CC608C,0x48000008L);
writeKernelMemory((void *)0x04CC6090,0x38820092L);
writeKernelMemory((void *)0x04CC6094,0x38620348L);
writeKernelMemory((void *)0x04CC6098,0x7F4903A6L);
writeKernelMemory((void *)0x04CC609C,0x4E800421L);
writeKernelMemory((void *)0x04CC60A0,0x48000181L);
writeKernelMemory((void *)0x04CC60A4,0x7F8903A6L);
writeKernelMemory((void *)0x04CC60A8,0x4E800421L);
writeKernelMemory((void *)0x04CC60AC,0x7EC903A6L);
writeKernelMemory((void *)0x04CC60B0,0x4E800421L);
writeKernelMemory((void *)0x04CC60B4,0x4800014DL);
writeKernelMemory((void *)0x04CC60B8,0xC02210E0L);
writeKernelMemory((void *)0x04CC60BC,0x480001A9L);
writeKernelMemory((void *)0x04CC60C0,0x7FA903A6L);
writeKernelMemory((void *)0x04CC60C4,0x4E800421L);
writeKernelMemory((void *)0x04CC60C8,0x81821110L);
writeKernelMemory((void *)0x04CC60CC,0x2C0C0002L);
writeKernelMemory((void *)0x04CC60D0,0x4082000CL);
writeKernelMemory((void *)0x04CC60D4,0x38820040L);
writeKernelMemory((void *)0x04CC60D8,0x48000008L);
writeKernelMemory((void *)0x04CC60DC,0x38820042L);
writeKernelMemory((void *)0x04CC60E0,0x38620348L);
writeKernelMemory((void *)0x04CC60E4,0x7F4903A6L);
writeKernelMemory((void *)0x04CC60E8,0x4E800421L);
writeKernelMemory((void *)0x04CC60EC,0x48000135L);
writeKernelMemory((void *)0x04CC60F0,0x7F8903A6L);
writeKernelMemory((void *)0x04CC60F4,0x4E800421L);
writeKernelMemory((void *)0x04CC60F8,0x7EC903A6L);
writeKernelMemory((void *)0x04CC60FC,0x4E800421L);
writeKernelMemory((void *)0x04CC6100,0x48000101L);
writeKernelMemory((void *)0x04CC6104,0x3D804340L);
writeKernelMemory((void *)0x04CC6108,0x918210E0L);
writeKernelMemory((void *)0x04CC610C,0xC02210E0L);
writeKernelMemory((void *)0x04CC6110,0x3D8042D0L);
writeKernelMemory((void *)0x04CC6114,0x918210E0L);
writeKernelMemory((void *)0x04CC6118,0xC04210E0L);
writeKernelMemory((void *)0x04CC611C,0x7FA903A6L);
writeKernelMemory((void *)0x04CC6120,0x4E800421L);
writeKernelMemory((void *)0x04CC6124,0x81821110L);
writeKernelMemory((void *)0x04CC6128,0x2C0C0003L);
writeKernelMemory((void *)0x04CC612C,0x4082000CL);
writeKernelMemory((void *)0x04CC6130,0x388210F0L);
writeKernelMemory((void *)0x04CC6134,0x48000008L);
writeKernelMemory((void *)0x04CC6138,0x388210F2L);
writeKernelMemory((void *)0x04CC613C,0x38620348L);
writeKernelMemory((void *)0x04CC6140,0x7F4903A6L);
writeKernelMemory((void *)0x04CC6144,0x4E800421L);
writeKernelMemory((void *)0x04CC6148,0x480000D9L);
writeKernelMemory((void *)0x04CC614C,0x7F8903A6L);
writeKernelMemory((void *)0x04CC6150,0x4E800421L);
writeKernelMemory((void *)0x04CC6154,0x480001A4L);
writeKernelMemory((void *)0x04CC6158,0x7EC903A6L);
writeKernelMemory((void *)0x04CC615C,0x4E800421L);
writeKernelMemory((void *)0x04CC6160,0x48000099L);
writeKernelMemory((void *)0x04CC6164,0x3D804126L);
writeKernelMemory((void *)0x04CC6168,0x91820424L);
writeKernelMemory((void *)0x04CC616C,0xC0220424L);
writeKernelMemory((void *)0x04CC6170,0x3D804382L);
writeKernelMemory((void *)0x04CC6174,0x9182FEB0L);
writeKernelMemory((void *)0x04CC6178,0xC042FEB0L);
writeKernelMemory((void *)0x04CC617C,0x7FA903A6L);
writeKernelMemory((void *)0x04CC6180,0x4E800421L);
writeKernelMemory((void *)0x04CC6184,0x38820B80L);
writeKernelMemory((void *)0x04CC6188,0x38620348L);
writeKernelMemory((void *)0x04CC618C,0x7F4903A6L);
writeKernelMemory((void *)0x04CC6190,0x4E800421L);
writeKernelMemory((void *)0x04CC6194,0x4800008DL);
writeKernelMemory((void *)0x04CC6198,0x7F8903A6L);
writeKernelMemory((void *)0x04CC619C,0x4E800421L);
writeKernelMemory((void *)0x04CC61A0,0x7EC903A6L);
writeKernelMemory((void *)0x04CC61A4,0x4E800421L);
writeKernelMemory((void *)0x04CC61A8,0x48000051L);
writeKernelMemory((void *)0x04CC61AC,0x3D804126L);
writeKernelMemory((void *)0x04CC61B0,0x91820424L);
writeKernelMemory((void *)0x04CC61B4,0xC0220424L);
writeKernelMemory((void *)0x04CC61B8,0x3D804378L);
writeKernelMemory((void *)0x04CC61BC,0x9182FEB0L);
writeKernelMemory((void *)0x04CC61C0,0xC042FEB0L);
writeKernelMemory((void *)0x04CC61C4,0x7FA903A6L);
writeKernelMemory((void *)0x04CC61C8,0x4E800421L);
writeKernelMemory((void *)0x04CC61CC,0x38820B50L);
writeKernelMemory((void *)0x04CC61D0,0x38620348L);
writeKernelMemory((void *)0x04CC61D4,0x7F4903A6L);
writeKernelMemory((void *)0x04CC61D8,0x4E800421L);
writeKernelMemory((void *)0x04CC61DC,0x48000045L);
writeKernelMemory((void *)0x04CC61E0,0x7F8903A6L);
writeKernelMemory((void *)0x04CC61E4,0x4E800421L);
writeKernelMemory((void *)0x04CC61E8,0x48000110L);
writeKernelMemory((void *)0x04CC61EC,0x39200000L);
writeKernelMemory((void *)0x04CC61F0,0x91220A64L);
writeKernelMemory((void *)0x04CC61F4,0x48000140L);
writeKernelMemory((void *)0x04CC61F8,0x3D803F60L);
writeKernelMemory((void *)0x04CC61FC,0x48000010L);
writeKernelMemory((void *)0x04CC6200,0x3D803F8AL);
writeKernelMemory((void *)0x04CC6204,0x48000008L);
writeKernelMemory((void *)0x04CC6208,0x3D804000L);
writeKernelMemory((void *)0x04CC620C,0x91820418L);
writeKernelMemory((void *)0x04CC6210,0xC0220418L);
writeKernelMemory((void *)0x04CC6214,0xC0420418L);
writeKernelMemory((void *)0x04CC6218,0x7EE903A6L);
writeKernelMemory((void *)0x04CC621C,0x4E800420L);
writeKernelMemory((void *)0x04CC6220,0x7EA3AB78L);
writeKernelMemory((void *)0x04CC6224,0x38820348L);
writeKernelMemory((void *)0x04CC6228,0x38A00001L);
writeKernelMemory((void *)0x04CC622C,0x38C00001L);
writeKernelMemory((void *)0x04CC6230,0x38E0FFFFL);
writeKernelMemory((void *)0x04CC6234,0x7F0903A6L);
writeKernelMemory((void *)0x04CC6238,0x4E800420L);
writeKernelMemory((void *)0x04CC623C,0xC0020428L);
writeKernelMemory((void *)0x04CC6240,0xC042042CL);
writeKernelMemory((void *)0x04CC6244,0xFC42002AL);
writeKernelMemory((void *)0x04CC6248,0xD0420428L);
writeKernelMemory((void *)0x04CC624C,0x4E800020L);
writeKernelMemory((void *)0x04CC6250,0xC002FBD8L);
writeKernelMemory((void *)0x04CC6254,0xC042042CL);
writeKernelMemory((void *)0x04CC6258,0xFC42002AL);
writeKernelMemory((void *)0x04CC625C,0xD042FBD8L);
writeKernelMemory((void *)0x04CC6260,0x4E800020L);
writeKernelMemory((void *)0x04CC6264,0xC002FE50L);
writeKernelMemory((void *)0x04CC6268,0xC042042CL);
writeKernelMemory((void *)0x04CC626C,0xFC42002AL);
writeKernelMemory((void *)0x04CC6270,0xD042FE50L);
writeKernelMemory((void *)0x04CC6274,0x4E800020L);
writeKernelMemory((void *)0x04CC6278,0xC002102CL);
writeKernelMemory((void *)0x04CC627C,0xC042042CL);
writeKernelMemory((void *)0x04CC6280,0xFC42002AL);
writeKernelMemory((void *)0x04CC6284,0xD042102CL);
writeKernelMemory((void *)0x04CC6288,0x4E800020L);
writeKernelMemory((void *)0x04CC628C,0xC0020030L);
writeKernelMemory((void *)0x04CC6290,0xC042042CL);
writeKernelMemory((void *)0x04CC6294,0xFC42002AL);
writeKernelMemory((void *)0x04CC6298,0xD0420030L);
writeKernelMemory((void *)0x04CC629C,0x4E800020L);
writeKernelMemory((void *)0x04CC62A0,0x90620E78L);
writeKernelMemory((void *)0x04CC62A4,0x80790034L);
writeKernelMemory((void *)0x04CC62A8,0x80630878L);
writeKernelMemory((void *)0x04CC62AC,0x38820E78L);
writeKernelMemory((void *)0x04CC62B0,0x3D800304L);
writeKernelMemory((void *)0x04CC62B4,0x618CA5D8L);
writeKernelMemory((void *)0x04CC62B8,0x7D8903A6L);
writeKernelMemory((void *)0x04CC62BC,0x4E800420L);
writeKernelMemory((void *)0x04CC62C0,0x80790034L);
writeKernelMemory((void *)0x04CC62C4,0x2C030000L);
writeKernelMemory((void *)0x04CC62C8,0x4182006CL);
writeKernelMemory((void *)0x04CC62CC,0x3D803F80L);
writeKernelMemory((void *)0x04CC62D0,0x91820AF8L);
writeKernelMemory((void *)0x04CC62D4,0xC0220AF8L);
writeKernelMemory((void *)0x04CC62D8,0xC0420AF8L);
writeKernelMemory((void *)0x04CC62DC,0x3D80031EL);
writeKernelMemory((void *)0x04CC62E0,0x618CA16CL);
writeKernelMemory((void *)0x04CC62E4,0x7D8903A6L);
writeKernelMemory((void *)0x04CC62E8,0x4E800420L);
writeKernelMemory((void *)0x04CC62EC,0x39200000L);
writeKernelMemory((void *)0x04CC62F0,0x91220A58L);
writeKernelMemory((void *)0x04CC62F4,0x4E800020L);
writeKernelMemory((void *)0x04CC62F8,0x80020004L);
writeKernelMemory((void *)0x04CC62FC,0x8282FFB8L);
writeKernelMemory((void *)0x04CC6300,0x82A2FFBCL);
writeKernelMemory((void *)0x04CC6304,0x82C2FFC0L);
writeKernelMemory((void *)0x04CC6308,0x82E2FFC4L);
writeKernelMemory((void *)0x04CC630C,0x8302FFC8L);
writeKernelMemory((void *)0x04CC6310,0x8322FFCCL);
writeKernelMemory((void *)0x04CC6314,0x8342FFD0L);
writeKernelMemory((void *)0x04CC6318,0x8362FFD4L);
writeKernelMemory((void *)0x04CC631C,0x8382FFD8L);
writeKernelMemory((void *)0x04CC6320,0x83A2FFDCL);
writeKernelMemory((void *)0x04CC6324,0x83C2FFE0L);
writeKernelMemory((void *)0x04CC6328,0x83E2FFE4L);
writeKernelMemory((void *)0x04CC632C,0xCBE20008L);
writeKernelMemory((void *)0x04CC6330,0x7C0803A6L);
writeKernelMemory((void *)0x04CC6334,0x4E800020L);
writeKernelMemory((void *)0x02F5C874,0x38600001L);
writeKernelMemory((void *)0x04A4CC3C,0x4E800421L);
writeKernelMemory((void *)0x04A4CC40,0x3D801003L);
writeKernelMemory((void *)0x04A4CC44,0x818C4B5CL);
writeKernelMemory((void *)0x04A4CC48,0x2C0C0000L);
writeKernelMemory((void *)0x04A4CC4C,0x41820008L);
writeKernelMemory((void *)0x04A4CC50,0x38600000L);
writeKernelMemory((void *)0x04A4CC54,0x4A74E510L);
writeKernelMemory((void *)0x0319B160,0x498B1ADCL);
writeKernelMemory((void *)0x040DCC3C,0x57C3063EL);
writeKernelMemory((void *)0x040DCC40,0x3D801003L);
writeKernelMemory((void *)0x040DCC44,0x818C4D40L);
writeKernelMemory((void *)0x040DCC48,0x2C0C0000L);
writeKernelMemory((void *)0x040DCC4C,0x41820008L);
writeKernelMemory((void *)0x040DCC50,0x38600001L);
writeKernelMemory((void *)0x040DCC54,0x4A4CCC00L);
writeKernelMemory((void *)0x025A9850,0x49B333ECL);
int value7568=0x00000001;
kernelCopyData((unsigned char *) 0x10034B5C,(unsigned char *)&value7568,sizeof(value7568));
int value7569=0x44000000;
kernelCopyData((unsigned char *) 0x10035300,(unsigned char *)&value7569,sizeof(value7569));
int value7570=0x44000000;
kernelCopyData((unsigned char *) 0x10035308,(unsigned char *)&value7570,sizeof(value7570));
int value7571=0x44000000;
kernelCopyData((unsigned char *) 0x10035310,(unsigned char *)&value7571,sizeof(value7571));
int value7572=0x44000000;
kernelCopyData((unsigned char *) 0x10035318,(unsigned char *)&value7572,sizeof(value7572));
int value7573=0x44000000;
kernelCopyData((unsigned char *) 0x10035010,(unsigned char *)&value7573,sizeof(value7573));
int value7574=0x44000000;
kernelCopyData((unsigned char *) 0x10035018,(unsigned char *)&value7574,sizeof(value7574));
int value7575=0x44000000;
kernelCopyData((unsigned char *) 0x100351C8,(unsigned char *)&value7575,sizeof(value7575));
int value7576=0x44000000;
kernelCopyData((unsigned char *) 0x10035A00,(unsigned char *)&value7576,sizeof(value7576));
int value7577=0x44000000;
kernelCopyData((unsigned char *) 0x10035A50,(unsigned char *)&value7577,sizeof(value7577));
int value7578=0x44000000;
kernelCopyData((unsigned char *) 0x10035A60,(unsigned char *)&value7578,sizeof(value7578));
int value7579=0x44000000;
kernelCopyData((unsigned char *) 0x10035B30,(unsigned char *)&value7579,sizeof(value7579));
int value7580=0x44000000;
kernelCopyData((unsigned char *) 0x10035BE0,(unsigned char *)&value7580,sizeof(value7580));
int value7581=0x44000000;
kernelCopyData((unsigned char *) 0x10035DF8,(unsigned char *)&value7581,sizeof(value7581));
int value7582=0x44000000;
kernelCopyData((unsigned char *) 0x10035E28,(unsigned char *)&value7582,sizeof(value7582));
int value7583=0x44010000;
kernelCopyData((unsigned char *) 0x10035E58,(unsigned char *)&value7583,sizeof(value7583));
int value7584=0x44010000;
kernelCopyData((unsigned char *) 0x10035E98,(unsigned char *)&value7584,sizeof(value7584));
int value7585=0x44000000;
kernelCopyData((unsigned char *) 0x10035EE0,(unsigned char *)&value7585,sizeof(value7585));
int value7586=0x44000000;
kernelCopyData((unsigned char *) 0x10035EE8,(unsigned char *)&value7586,sizeof(value7586));
int value7587=0x44000000;
kernelCopyData((unsigned char *) 0x10035EA0,(unsigned char *)&value7587,sizeof(value7587));
int value7588=0x44000000;
kernelCopyData((unsigned char *) 0x10035EA8,(unsigned char *)&value7588,sizeof(value7588));
int value7589=0x44000000;
kernelCopyData((unsigned char *) 0x10035F30,(unsigned char *)&value7589,sizeof(value7589));
int value7590=0x44000000;
kernelCopyData((unsigned char *) 0x10035F30,(unsigned char *)&value7590,sizeof(value7590));
int value7591=0x44000000;
kernelCopyData((unsigned char *) 0x10034F3C,(unsigned char *)&value7591,sizeof(value7591));
int value7592=0x44000000;
kernelCopyData((unsigned char *) 0x10034FB4,(unsigned char *)&value7592,sizeof(value7592));
int value7593=0x44000000;
kernelCopyData((unsigned char *) 0x10035020,(unsigned char *)&value7593,sizeof(value7593));
int value7594=0x00000001;
kernelCopyData((unsigned char *) 0x10036038,(unsigned char *)&value7594,sizeof(value7594));
int value7595=0x00000001;
kernelCopyData((unsigned char *) 0x10036030,(unsigned char *)&value7595,sizeof(value7595));
int value7596=0x41000000;
kernelCopyData((unsigned char *) 0x10035A68,(unsigned char *)&value7596,sizeof(value7596));
int value7597=0x44000000;
kernelCopyData((unsigned char *) 0x10035400,(unsigned char *)&value7597,sizeof(value7597));
writeKernelMemory((void *)0x02E0E7C4,0x4E800020L);
writeKernelMemory((void *)0x04332C38,0x7C0802A6L);
writeKernelMemory((void *)0x04332C3C,0x3D801003L);
writeKernelMemory((void *)0x04332C40,0x818C5A14L);
writeKernelMemory((void *)0x04332C44,0x2C0C0000L);
writeKernelMemory((void *)0x04332C48,0x40820008L);
writeKernelMemory((void *)0x04332C4C,0x4B0D5634L);
writeKernelMemory((void *)0x04332C50,0x4E800020L);
writeKernelMemory((void *)0x0384D0E4,0x7C0802A6L);
writeKernelMemory((void *)0x0384D0E8,0x3D801003L);
writeKernelMemory((void *)0x0384D0EC,0x90EC5A28L);
writeKernelMemory((void *)0x0384D0F0,0x4B953654L);
writeKernelMemory((void *)0x0384CC74,0x3D8002E9L);
writeKernelMemory((void *)0x0384CC78,0x3D601003L);
writeKernelMemory((void *)0x0384CC7C,0x812B5A14L);
writeKernelMemory((void *)0x0384CC80,0x2C090000L);
writeKernelMemory((void *)0x0384CC84,0x41820008L);
writeKernelMemory((void *)0x0384CC88,0x38800001L);
writeKernelMemory((void *)0x0384CC8C,0x4B64F2F4L);
writeKernelMemory((void *)0x0384CCB4,0x57E3063EL);
writeKernelMemory((void *)0x0384CCB8,0x3D601003L);
writeKernelMemory((void *)0x0384CCBC,0x812B5A14L);
writeKernelMemory((void *)0x0384CCC0,0x2C090000L);
writeKernelMemory((void *)0x0384CCC4,0x41820008L);
writeKernelMemory((void *)0x0384CCC8,0x38600001L);
writeKernelMemory((void *)0x0384CCCC,0x4B5EDC88L);
writeKernelMemory((void *)0x0404CCEC,0x7C0802A6L);
writeKernelMemory((void *)0x0404CCF0,0x3D801003L);
writeKernelMemory((void *)0x0404CCF4,0x818C5A14L);
writeKernelMemory((void *)0x0404CCF8,0x2C0C0000L);
writeKernelMemory((void *)0x0404CCFC,0x41820008L);
writeKernelMemory((void *)0x0404CD00,0x38800000L);
writeKernelMemory((void *)0x0404CD04,0x4AE4E5D0L);
writeKernelMemory((void *)0x0404CD08,0x7C0802A6L);
writeKernelMemory((void *)0x0404CD0C,0x3D801003L);
writeKernelMemory((void *)0x0404CD10,0x818C5A18L);
writeKernelMemory((void *)0x0404CD14,0x2C0C0000L);
writeKernelMemory((void *)0x0404CD18,0x41820008L);
writeKernelMemory((void *)0x0404CD1C,0x38800000L);
writeKernelMemory((void *)0x0404CD20,0x4AE4E8A0L);
writeKernelMemory((void *)0x0404CC24,0x3D801003L);
writeKernelMemory((void *)0x0404CC28,0x818C4D60L);
writeKernelMemory((void *)0x0404CC2C,0x2C0C0001L);
writeKernelMemory((void *)0x0404CC30,0x41820008L);
writeKernelMemory((void *)0x0404CC34,0x4E800421L);
writeKernelMemory((void *)0x0404CC38,0x4AD9BB70L);
writeKernelMemory((void *)0x040CCC20,0x7C0802A6L);
writeKernelMemory((void *)0x040CCC24,0x3D801003L);
writeKernelMemory((void *)0x040CCC28,0x818C4D5CL);
writeKernelMemory((void *)0x040CCC2C,0x2C0C0000L);
writeKernelMemory((void *)0x040CCC30,0x41820014L);
writeKernelMemory((void *)0x040CCC34,0x3D8002F5L);
writeKernelMemory((void *)0x040CCC38,0x618C981CL);
writeKernelMemory((void *)0x040CCC3C,0x7D8903A6L);
writeKernelMemory((void *)0x040CCC40,0x4E800420L);
writeKernelMemory((void *)0x040CCC44,0x4AE8C8F4L);
writeKernelMemory((void *)0x040BCC20,0x7C0802A6L);
writeKernelMemory((void *)0x040BCC24,0x3D801003L);
writeKernelMemory((void *)0x040BCC28,0x818C4D54L);
writeKernelMemory((void *)0x040BCC2C,0x2C0C0000L);
writeKernelMemory((void *)0x040BCC30,0x4182000CL);
writeKernelMemory((void *)0x040BCC34,0x38600001L);
writeKernelMemory((void *)0x040BCC38,0x4E800020L);
writeKernelMemory((void *)0x040BCC3C,0x4A65DDD8L);
writeKernelMemory((void *)0x040BCC54,0x38800001L);
writeKernelMemory((void *)0x040BCC58,0x3D801003L);
writeKernelMemory((void *)0x040BCC5C,0x818C4D50L);
writeKernelMemory((void *)0x040BCC60,0x2C0C0000L);
writeKernelMemory((void *)0x040BCC64,0x41820008L);
writeKernelMemory((void *)0x040BCC68,0x3880009CL);
writeKernelMemory((void *)0x040BCC6C,0x4A27274CL);
writeKernelMemory((void *)0x040BCC90,0x9421FF58L);
writeKernelMemory((void *)0x040BCC94,0x3D801003L);
writeKernelMemory((void *)0x040BCC98,0x818C4D4CL);
writeKernelMemory((void *)0x040BCC9C,0x2C0C0000L);
writeKernelMemory((void *)0x040BCCA0,0x4182000CL);
writeKernelMemory((void *)0x040BCCA4,0x382100A8L);
writeKernelMemory((void *)0x040BCCA8,0x4E800020L);
writeKernelMemory((void *)0x040BCCAC,0x4AA99754L);
writeKernelMemory((void *)0x0414CC54,0x38000000L);
writeKernelMemory((void *)0x0414CC58,0x3D801003L);
writeKernelMemory((void *)0x0414CC5C,0x818C4D40L);
writeKernelMemory((void *)0x0414CC60,0x2C0C0000L);
writeKernelMemory((void *)0x0414CC64,0x41820008L);
writeKernelMemory((void *)0x0414CC68,0x38000001L);
writeKernelMemory((void *)0x0414CC6C,0x4B04A6ACL);
writeKernelMemory((void *)0x041BFF04,0x3D801003L);
writeKernelMemory((void *)0x041BFF08,0x818C5A58L);
writeKernelMemory((void *)0x041BFF0C,0x2C0C0010L);
writeKernelMemory((void *)0x041BFF10,0x40800008L);
writeKernelMemory((void *)0x041BFF14,0x4E800020L);
writeKernelMemory((void *)0x041BFF18,0x9421FFB0L);
writeKernelMemory((void *)0x041BFF1C,0x4B035AB4L);
writeKernelMemory((void *)0x0384CE24,0x57E3063EL);
writeKernelMemory((void *)0x0384CE28,0x2C030000L);
writeKernelMemory((void *)0x0384CE2C,0x40820010L);
writeKernelMemory((void *)0x0384CE30,0x3C401003L);
writeKernelMemory((void *)0x0384CE34,0x39800000L);
writeKernelMemory((void *)0x0384CE38,0x91825A14L);
writeKernelMemory((void *)0x0384CE3C,0x4AD311C8L);
writeKernelMemory((void *)0x0340827C,0x48F2A9BCL);
writeKernelMemory((void *)0x031A0740,0x486AC9A4L);
writeKernelMemory((void *)0x02E9BF7C,0x489B0CF8L);
writeKernelMemory((void *)0x02E3A950,0x48A12364L);
writeKernelMemory((void *)0x02E9B2D0,0x491B1A1CL);
writeKernelMemory((void *)0x02E9B5BC,0x491B174CL);
writeKernelMemory((void *)0x02DE87A4,0x49264480L);
writeKernelMemory((void *)0x02F59534,0x491736ECL);
writeKernelMemory((void *)0x0271AA10,0x499A2210L);
writeKernelMemory((void *)0x0232F3B4,0x49D8D8A0L);
writeKernelMemory((void *)0x02B563FC,0x49566894L);
writeKernelMemory((void *)0x03197314,0x48FB5940L);
writeKernelMemory((void *)0x031F59CC,0x48FCA538L);
writeKernelMemory((void *)0x0257E000,0x492CEE24L);
writeKernelMemory((void *)0x04100000,0x4E800421L);
writeKernelMemory((void *)0x04100004,0x3D8004CCL);
writeKernelMemory((void *)0x04100008,0x7D8903A6L);
writeKernelMemory((void *)0x0410000C,0x4E800421L);
writeKernelMemory((void *)0x04100010,0x4AC9C8A4L);
writeKernelMemory((void *)0x04100020,0x4E800421L);
writeKernelMemory((void *)0x04100024,0x3D8004CCL);
writeKernelMemory((void *)0x04100028,0x7D8903A6L);
writeKernelMemory((void *)0x0410002C,0x4E800421L);
writeKernelMemory((void *)0x04100030,0x4AC9CA94L);
writeKernelMemory((void *)0x02D9C8B0,0x49363750L);
writeKernelMemory((void *)0x02D9CAC0,0x49363560L);
// End ShineClient
}

	unsigned int stack = (unsigned int) memalign(0x40, 0x100);
	ASSERT_ALLOCATED(stack, "TCP Gecko stack")
	stack += 0x100;
	void *thread = memalign(0x40, 0x1000);
	ASSERT_ALLOCATED(thread, "TCP Gecko thread")

	int status = OSCreateThread(thread, startTCPGeckoThread, (s32) 1,
								NULL, (s32)(stack + sizeof(stack)),
								sizeof(stack), 0,
								(OS_THREAD_ATTR_AFFINITY_CORE1 | OS_THREAD_ATTR_PINNED_AFFINITY |
								 OS_THREAD_ATTR_DETACH));
	ASSERT_INTEGER(status, 1, "Creating TCP Gecko thread")
	// OSSetThreadName(thread, "TCP Gecko");
	OSResumeThread(thread);

	log_print("TCP Gecko started...\n");
}