#include "receiver.h"

void commReceiverInit(commReceiver* self, commMsg* ptr)
{
    self->packet = ptr;
    self->state = commReceiverStateLookingForSync;
    self->numberOfReceivedParameters = 0;
#ifdef commReceiverTimeoutUs
    self->timeReceivedHeaderUs = 0;
#endif
}

void commReceiverProcess(commReceiver* self, uint8_t newByte) {
#ifdef commReceiverTimeoutUs
    // auxiliary variables
    uint32_t ellapsedTime;

    /*  Check timeout
     *
     *  If the receiver state is different from State::LOOKING_FOR_SYNC, then a message was
     *  being received. In this case, we must check timeout to avoid communication errors.
     *  If a timeout is verified, then the receiver must restart and release the actual
     *  message.
     */
    if (self->state != commReceiverStateLookingForSync)
    {
        ellapsedTime = getTimeUs() - self->timeReceivedHeaderUs;
        if (ellapsedTime > commReceiverTimeoutUs)
        {
            commReceiverClean(self);
        }
    }
#endif

    switch (self->state) {
        case commReceiverStateLookingForSync:
            if (newByte == commMsgValueSyncHeader)
            {
                self->state = commReceiverStateLookingForProtocolVersion;
                commMsgSetField(self->packet, commMsgFieldSyncHeader, commMsgValueSyncHeader);

#ifdef commReceiverTimeoutUs
                // Start to count timeout time
                self->timeReceivedHeaderUs = getTimeUs();
#endif
            }
            break;

        case commReceiverStateLookingForProtocolVersion:
            if (newByte == commMsgValueVersion)
            {
                self->state = commReceiverStateLookingForType;
                commMsgSetField(self->packet, commMsgFieldVersion, commMsgValueVersion);
            }
            else
            {
                self->state = commReceiverStateLookingForSync;
            }
            break;

        case commReceiverStateLookingForType:
            if (commMsgIsThisTypeValid(newByte))
            {
                self->state = commReceiverStateLookingForLength;
                commMsgSetField(self->packet, commMsgFieldType, newByte);
            }
            else
            {
                self->state = commReceiverStateLookingForSync;
            }

            break;

        case commReceiverStateLookingForLength:
            if (newByte < commMsgMaxLength)
            {
                // Then the new_byte is the content of length field
                commMsgSetField(self->packet, commMsgFieldLength, newByte);

                if (newByte == 1)
                {
                    self->state = commReceiverStateLookingForChecksum;
                }
                else
                {
                    self->state = commReceiverStateReceivingParameters;
                    self->numberOfReceivedParameters = 0;
                }
            }
            else
            {
                self->state = commReceiverStateLookingForSync;
            }
            break;

        case commReceiverStateReceivingParameters:
            commMsgInsertParameter(self->packet, self->numberOfReceivedParameters, newByte);
            self->numberOfReceivedParameters = self->numberOfReceivedParameters + 1;

            if ((self->numberOfReceivedParameters + 1) == commMsgGetField(self->packet, commMsgFieldLength))
            {
                self->state = commReceiverStateLookingForChecksum;
            }
            break;

        case commReceiverStateLookingForChecksum:
            // Copy the received byte to the checksum field
            commMsgSetField(self->packet, commMsgFieldChecksum, newByte);
            if (commMsgIsChecksumValid(self->packet))
            {
                self->state = commReceiverStateHoldingValidPacket;
            }
            else
            {
                self->state = commReceiverStateLookingForSync;
            }
            break;

        case commReceiverStateHoldingValidPacket:
            break;
    }
}

void commReceiverClean(commReceiver* self)
{
    self->state = commReceiverStateLookingForSync;
    self->numberOfReceivedParameters = 0;
}

bool commReceiverHasNewMessage(commReceiver* self)
{
    return self->state == commReceiverStateHoldingValidPacket;
}
