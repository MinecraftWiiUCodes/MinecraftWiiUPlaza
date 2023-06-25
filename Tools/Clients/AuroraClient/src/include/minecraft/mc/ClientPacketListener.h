#pragma once 

#include <code/code.h>
#include <inttypes.h>

#include "miscellanious.h"

namespace mc {
    class ClientPacketListener {
	public:
		void send(mboost::shared_ptr<struct Packet> packet) {
	    	code::Func<void, 0x0304A5D8, ClientPacketListener*, mboost::shared_ptr<struct Packet>>()(this, packet);
		}

		void sendCommand(uint32_t commandAddr = 0x10303000) {
	    	code::Func<void, 0x0304A5D8, ClientPacketListener*, uint32_t>()(this, commandAddr);
		}
	};
}