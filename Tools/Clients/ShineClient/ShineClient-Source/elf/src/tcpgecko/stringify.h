#ifndef TCPGECKO_STRINGIFY_H
#define TCPGECKO_STRINGIFY_H

// http://elixir.free-electrons.com/linux/v2.6.24/source/include/linux/stringify.h#L9
#define __stringify_1(x)    #x
#define __stringify(x)        __stringify_1(x)

#endif