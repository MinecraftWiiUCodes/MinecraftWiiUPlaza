#ifndef TCPGECKO_ADDRESS_H
#define TCPGECKO_ADDRESS_H

#include <stdbool.h>

int validateAddressRange(int starting_address, int ending_address);

bool isValidDataAddress(int address);

int roundUpToAligned(int number);

#endif