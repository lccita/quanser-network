#ifndef QUARK_COMM_RECEIVER_H
#define QUARK_COMM_RECEIVER_H

/**
 ***********************************************************************************************************************
 * @file    receiver.h
 * @author  José Roberto Colombo Junior
 * @brief   Header file of receiver, a state machine capable of receiving valid communication packets.
 ***********************************************************************************************************************
 * @attention This is free software licensed under GPL.
 ***********************************************************************************************************************
 */

#include "protocol.h"
#include "time_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Define here the maximum time (in microseconds) to wait until a timeout has occurred. This parameter is optional and
 * and if disabled, parts of the receiver code will be disabled.
 */
//#define commReceiverTimeoutUs (1000)

/**
 * The receiver is a state machine. Its states are defined here.
 */
typedef enum commReceiverState {
    commReceiverStateLookingForSync = 0,         ///< The receiver is looking for the sync header byte.
    commReceiverStateLookingForProtocolVersion,  ///< The receiver is looking for the protocol version byte.
    commReceiverStateLookingForType,             ///< The receiver is looking for the type byte.
    commReceiverStateLookingForLength,           ///< The receiver is looking for the length.
    commReceiverStateReceivingParameters,        ///< The receiver is receiving parameters, i.e., the contents of data field.
    commReceiverStateLookingForChecksum,         ///< The receiver is looking for the checksum.
    commReceiverStateHoldingValidPacket,         ///< The receiver is holding a valid packet.
} commReceiverState;

/**
 * A struct modeling the receiver.
 */
typedef struct commReceiver {
    commReceiverState state;              ///< The receiver state.
    uint8_t numberOfReceivedParameters;   ///< How many parameters were received.
    commMsg* packet;                      ///< A pointer to a commMsg. This message has to be previously allocated.
#ifdef commReceiverTimeoutUs
    uint32_t timeReceivedHeaderUs;        ///< A variable to store when the header was received.
#endif
} commReceiver;

/**
 * @brief This function must be called before using the receiver.
 * @param self a pointer to a instance of the receiver struct.
 * @param ptr a pointer to a instance of a buffer message.
 */
extern void commReceiverInit(commReceiver* self, commMsg* ptr);

/**
 * @brief This function processes a given byte.
 * @param self a pointer to a instance of the receiver struct.
 * @param newByte is the received byte that should be processed.
 */
extern void commReceiverProcess(commReceiver* self, uint8_t newByte);

/**
 * @brief After a message is acknowledged and processed, this function must be called before trying to process new bytes with
 *        the receiver.
 * @param self a pointer to a instance of the receiver struct.
 */
extern void commReceiverClean(commReceiver* self);

/**
 * @brief This funciton can be used to check if a new message was acknowledged.
 * @param self a pointer to a instance of the receiver struct.
 * @return true if a message was acknowledged.
 */
extern bool commReceiverHasNewMessage(commReceiver* self);

#ifdef __cplusplus
}
#endif

#endif //QUARK_COMM_RECEIVER_H
