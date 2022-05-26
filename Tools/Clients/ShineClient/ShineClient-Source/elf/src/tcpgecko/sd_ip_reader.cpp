#include "sd_ip_reader.h"

char ipFromSd[16];
bool hasReadIP = false;

void initializeUDPLog() {
	if (!hasReadIP) {
		log_printf("Reading ip from sd card\n");
		hasReadIP = true;

		std::string ipFilePath = std::string(SD_PATH) + WIIU_PATH + "/" + IP_TXT;

		CFile file(ipFilePath, CFile::ReadOnly);
		if (!file.isOpen()) {
			log_printf("File %s not found, using hard-coded\n", ipFilePath.c_str());
			log_init(COMPUTER_IP_ADDRESS);
			return;
		}

		std::string strBuffer;
		strBuffer.resize(file.size());
		file.read((u8 *) &strBuffer[0], strBuffer.size());

		if (strBuffer.length() >= sizeof(ipFromSd)) {
			log_printf("Loading ip from sd failed. String was too long: %s\n", strBuffer.c_str());
			return;
		}

		memcpy(ipFromSd, strBuffer.c_str(), strBuffer.length());
		ipFromSd[strBuffer.length()] = 0;

		log_printf("Successfully read ip from sd! ip is: %s\n", ipFromSd);

		log_init(ipFromSd);
	}

	if (strlen(ipFromSd) > 0) {
		log_init(ipFromSd);
	}
}