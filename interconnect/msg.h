#ifndef MSG_H
#define MSG_H

#include <cstdint>

struct msg
{
    uint32_t opcode;
    uint32_t QoS;
};


#endif