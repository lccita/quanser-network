#include "protocol.h"
#include <string.h>
#define AS_U8(x) ((uint8_t)x)

static uint8_t commMsgGetChecksumPosition(commMsg* self)
{
    //                                    +-------------------------+
    //                                    |    In length = 1 + N    |
    // +------+----+-------------+--------+--------------+----------+
    // | SYNC | PV | Instruction | Length | N parameters | CHECKSUM |
    // +------+----+-------------+--------+--------------+----------+
    //     0    1         2          3         4 + N         5 + N

    return self->buffer[AS_U8(commMsgFieldLength)] + 3;
}

bool commMsgIsThisTypeValid(commMsgType type)
{
    return (type < commMsgTypeSize);
}

void commMsgClean(commMsg* self)
{
    commMsgSetField(self, commMsgFieldLength, 2);
}

void commMsgSetField(commMsg* self, commMsgFields field, uint8_t value)
{
    uint8_t index;
    if (field == commMsgFieldChecksum)
    {
        index = commMsgGetChecksumPosition(self);
    }
    else
    {
        index = AS_U8(field);
    }

    self->buffer[index] = value;
}

uint8_t commMsgGetField(commMsg* self, commMsgFields field)
{
    uint8_t index;
    if (field == commMsgFieldChecksum)
    {
        index = commMsgGetChecksumPosition(self);
    }
    else
    {
        index = AS_U8(field);
    }
    return self->buffer[AS_U8(index)];
}

void commMsgAppendParameter(commMsg* self, uint8_t value)
{
    const uint8_t initialLength = commMsgGetField(self, commMsgFieldLength);
    const uint8_t numberOfParameters = commMsgGetNumberOfParameters(self);

    if (numberOfParameters < commMsgMaxDataLength)
    {
        self->buffer[AS_U8(commMsgFieldData) + numberOfParameters] = value;
        commMsgSetField(self, commMsgFieldLength, initialLength + 1);
    }
}

void commMsgAppendParameters(commMsg *self, void* data, uint8_t len)
{
    const uint8_t initialLength = commMsgGetField(self, commMsgFieldLength);
    const uint8_t numberOfParameters = commMsgGetNumberOfParameters(self);
    memcpy(&self->buffer[AS_U8(commMsgFieldData) + numberOfParameters], data, len);
    commMsgSetField(self, commMsgFieldLength, initialLength + len);
}

void commMsgInsertParameter(commMsg* self, uint8_t index, uint8_t value)
{
    if (index < commMsgMaxDataLength)
    {
        self->buffer[AS_U8(commMsgFieldData) + index] = value;
    }
}

uint8_t commMsgGetParameter(commMsg* self, uint8_t index)
{
    uint8_t value = 0;

    if (index < commMsgMaxDataLength)
    {
        value = self->buffer[AS_U8(commMsgFieldData) + index];
    }

    return value;
}

uint8_t* commMsgGetAddrOfParameters(commMsg* self)
{
    return &self->buffer[AS_U8(commMsgFieldData)];
}

uint8_t commMsgGetNumberOfParameters(commMsg* self)
{
    return commMsgGetField(self, commMsgFieldLength) - 1;
}

uint8_t* commMsgGetAddrOfPacket(commMsg* self)
{
    return self->buffer;
}

uint8_t commMsgGetTotalPacketSize(commMsg* self)
{
    return commMsgGetField(self, commMsgFieldLength) + 4;
}

void commMsgUpdateChecksumValue(commMsg* self)
{
    const uint8_t checksum = commMsgCalculateChecksum(self);
    commMsgSetField(self, commMsgFieldChecksum, checksum);
}

uint8_t commMsgCalculateChecksum(commMsg* self)
{
    uint8_t calculatedChecksum = 0;
    const uint8_t msgSize = commMsgGetTotalPacketSize(self) - 1; // Do not count the checksum

    uint8_t i;
    for (i = AS_U8(commMsgFieldType); i < msgSize; i++)
    {
        calculatedChecksum = calculatedChecksum + self->buffer[i];
    }
    calculatedChecksum = (~calculatedChecksum) & 0xFF;

    return calculatedChecksum;
}

bool commMsgIsChecksumValid(commMsg* self)
{
    return (commMsgCalculateChecksum(self) == commMsgGetField(self, commMsgFieldChecksum));
}

void commMsgCreateEmptyMsg(commMsg* self, commMsgType type)
{
    // Ask the type of message
    commMsgSetField(self, commMsgFieldSyncHeader, commMsgValueSyncHeader);
    commMsgSetField(self, commMsgFieldVersion, commMsgValueVersion);
    commMsgSetField(self, commMsgFieldType, type);
    commMsgSetField(self, commMsgFieldLength, 1);
}
