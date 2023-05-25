#ifndef PROTOCOL_H
#define PROTOCOL_H

/**
 ***********************************************************************************************************************
 * @file    protocol.h
 * @author  José Roberto Colombo Junior
 * @brief   Header file of communication protocol.
 ***********************************************************************************************************************
 * @attention This is free software licensed under GPL.
 ***********************************************************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup communication Communication
 * The board is controlled from the computer. Data is sent using the protocol herein defined.
 */

/**@{*/

/**
 * Message max size is 256. The board is usually connected using USB 2.0 with default 12MBit speed If this is the case,
 * packets smaller than 180 bytes will be transferred in a single transaction.
 */
#define commMsgMaxSize     256

/**
 * The maximum value to the length field.
 */
#define commMsgMaxLength      (commMsgMaxSize - 3)

/**
 * The maximum length of the data section (because commProtocolMsgMaxSize -1 of SYNC_HEADER -1 of PROTOCOL_VERSION
 * and -1 of LENGTH, -1 INSTRUCTION and -1 CHECKSUM).
 */
#define commMsgMaxDataLength  (commMsgMaxSize - 5)

/**
 * @brief Field positions of the message.
 */
typedef enum commMsgFields {
    commMsgFieldSyncHeader = 0,  ///< Sync Header field.
    commMsgFieldVersion,         ///< Protocol Version field.
    commMsgFieldType,            ///< Field with the packet type.
    commMsgFieldLength,          ///< Field with length, i.e., how many bytes still must be received.
    commMsgFieldData,            ///< Data section field.
    commMsgFieldChecksum,        ///< The last field is the checksum.
} commMsgFields;

/**
 * Default value to the sync header.
 */
#define commMsgValueSyncHeader      (0xFF)

/**
 * Default value to the version field.
 */
#define commMsgValueVersion         (0xFF)

/**
 * @brief This enumerates all the possible instructions.
 */
typedef enum commMsgType {
    commMsgTypeReboot = 0,      ///< The reboot type packet will make the board to reboot.
    commMsgTypePing,            ///< The ping type packet is used to verify if the device is working.
    commMsgTypeRead,            ///< The read type packet forces to read from the memory table.
    commMsgTypeWrite,           ///< The write type packet forces to write to the memory table.
    commMsgTypeMultiWrite,      ///< The multi write type packet allows multiple writes to the memory table.
    commMsgTypeAnsPing,         ///< The AnsPing packet is the answer to the ping packet.
    commMsgTypeAnsRead,         ///< The AnsRead packet is the answer to the read packet.
    commMsgTypeAnsWrite,        ///< The AnsWrite packet is the answer to the write packet. This may or may not be used.
    commMsgTypeI2cProbe,        ///< The I2cProbe will force the device to probe for an I2C device.
    commMsgTypeI2cAnsProbe,     ///< The I2cAnsProbe is the answer to the I2cProbe.
    commMsgTypeI2cWrite,        ///< The I2cWrite will force the device to write to a I2C device.
    commMsgTypeI2cMemWrite,     ///< The I2cMemWrite will force the device to write to I2C memory device.
    commMsgTypeI2cAnsWrite,     ///< The I2cAnsWrite is the answer to the I2cWrite. This may or may not be used.
    commMsgTypeI2cAnsMemWrite,  ///< The I2cAnsMemWrite is the answer to the I2cMemWrite. This may or may not be used.
    commMsgTypeI2cRead,         ///< The I2cRead will force the device to read from a I2C device.
    commMsgTypeI2cMemRead,      ///< The I2cMemRead will force the device to read from I2C memory device.
    commMsgTypeI2cAnsRead,      ///< The I2cAnsRead is the answer to the I2cRead. This may or may not be used.
    commMsgTypeI2cAnsMemRead,   ///< The I2cAnsMemRead is the answer to the I2cMemRead. This may or may not be used.
    commMsgTypeSize,            ///< How many types are defined.
} commMsgType;

/**
 * @brief Possible errors associated with the protocol message.
 */
typedef enum commMsgError {
    commMsgErrorNone               = 0,          ///< No errors.
    commMsgErrorInvalidLength      = (1 << 0),   ///< The message holds an invalid length.
    commMsgErrorInvalidType        = (1 << 1),   ///< The message holds an invalid type.
    commMsgErrorInvalidChecksum    = (1 << 2),   ///< The message holds an invalid checksum.
    commMsgErrorInvalidRange       = (1 << 3),   ///< The message holds an invalid range.
} commMsgError;

/**
 * @brief This function verifies if a given instruction code number is valid
 * @param type is a instruction code number
 * @return true if valid and false if not valid
 */
extern bool commMsgIsThisTypeValid(commMsgType type);

/*
 * Examples of CMD messages
 *
 *                                                    +-------------------------+
 *                                                    |       Parameters        |
 * +----------------+------+------+----------+--------+------------------+------+----------+
 * | Value          | 0xFF | 0xFF |   0x21   |  0x03  |       0x00       | 0x01 |   0xDA   |
 * +----------------+------+------+----------+--------+------------------+------+----------+
 * | Interpretation | SYNC | P.V. | CMD_READ | Length | Starting address | QTD  | Checksum |
 * +----------------+------+------+----------+--------+------------------+------+----------+
 * | Position       |  0   |  1   |     2    |    3   |         4        |   5  |    6     |
 * +----------------+------+------+----------+--------+------------------+------+----------+
 *
 * +------+------+-----------+--------+------------------+------+------+----------+
 * | 0xFF | 0xFF |   0x22    |  0x05  |       0x00       | 0x01 | 0x02 |   0xE3   |
 * +------+------+-----------+--------+------------------+------+------+----------+
 * | SYNC | P.V. | CMD_WRITE | Length | Starting address | PARAMETERS  | Checksum |
 * +------+------+-----------+--------+------------------+------+------+----------+
 * |  0   |  1   |     2     |   3    |         4        |   5  |   6  |    7     |
 * +------+------+-----------+--------+------------------+------+------+----------+
 *
 * Turn the board led off: FFFF2203F80FD3
 * +------+------+-----------+--------+------------------+-------------+----------+
 * | 0xFF | 0xFF |   0x22    |  0x03  |       0xF8       |     0x0F    |   0xD3   |
 * +------+------+-----------+--------+------------------+-------------+----------+
 * | SYNC | P.V. | CMD_WRITE | Length | Starting address | PARAMETERS  | Checksum |
 * +------+------+-----------+--------+------------------+-------------+----------+
 * |  0   |  1   |     2     |   3    |         4        |      5      |    6     |
 * +------+------+-----------+--------+------------------+-------------+----------+
 *
 * Turn the board led on: FFFF2203F800E2
 * +------+------+-----------+--------+------------------+-------------+----------+
 * | 0xFF | 0xFF |   0x22    |  0x03  |       0xF8       |     0x00    |   0xE2   |
 * +------+------+-----------+--------+------------------+-------------+----------+
 * | SYNC | P.V. | CMD_WRITE | Length | Starting address | PARAMETERS  | Checksum |
 * +------+------+-----------+--------+------------------+-------------+----------+
 * |  0   |  1   |     2     |   3    |         4        |      5      |    6     |
 * +------+------+-----------+--------+------------------+-------------+----------+
 *
 * +------+------+------+----------+----------+
 * | 0xFF | 0xFF | 0x20 |   0x01   |   0xDE   |
 * +------+------+------+----------+----------+
 * | SYNC | P.V. | PING | Length   | Checksum |
 * +------+------+------+----------+----------+
 * |  0   |  1   |   2  |     3    |    4     |
 * +------+------+------+----------+----------+
 *
 */

/*
 * Examples of ANS messages
 *
 *                                    +-----------------------------------------------+
 *                                    |                    Parameters                 |
 * +------+------+-----------+--------+-------+------------------+------+------+------+----------+
 * | 0xFF | 0xFF |    0x19   |  0x04  | 0x00  |       0x00       | 0x02 | 0x01 | 0x02 |   0xDE   |
 * +------+------+-----------+--------+-------+------------------+------+------+------+----------+
 * | SYNC | P.V. |  ANS_READ | Length | Error | Starting address | QTD  |    DATA     | Checksum |
 * +------+------+-----------+--------+--------------------------+------+------+------+----------+
 * |  0   |  1   |     2     |   3    |   4   |      5           |  6   |  7   |  8   |    9     |
 * +------+------+-----------+--------+--------------------------+------+------+------+----------+
 *
 * +------+------+-----------+--------+-------+----------+
 * | 0xFF | 0xFF |   0x1A    |  0x02  | 0x00  |   0xE2   |
 * +------+------+-----------+--------+-------+----------+
 * | SYNC | P.V. | ANS_WRITE | Length | Error | Checksum |
 * +------+------+-----------+--------+-------+----------+
 * |  0   |  1   |     2     |   3    |   4   |    5     |
 * +------+------+-----------+--------+-------+----------+
 *
 * +------+------+-----------+----------+----------+
 * | 0xFF | 0xFF |   0x30    |   0x01   |   0xCE   |
 * +------+------+-----------+----------+----------+
 * | SYNC | P.V. |  ANS_PING | Length   | Checksum |
 * +------+------+-----------+----------+----------+
 * |  0   |  1   |     2     |     3    |    4     |
 * +------+------+-----------+----------+----------+
 */

/**
 * @brief A struct modeling a message.
 */
typedef struct commMsg {
    uint8_t buffer[commMsgMaxSize];  ///< Reserve static memory for message raw data.
} commMsg;

/**
 * @brief This function cleans the message object.
 * @param self a pointer to a message object.
 */
extern void commMsgClean(commMsg *self);

/**
 * @brief This function is used to set the value of a field in the message.
 * @param self a pointer to a message object.
 * @param field what field to set.
 * @param value what value to insert in the specified field.
 * @warning this function should not be used to set the data field. Use the commMsgAppendParameter or
 *          commMsgInsertParameter functions instead.
 */
extern void commMsgSetField(commMsg *self, commMsgFields field, uint8_t value);

/**
 * @brief This function is used to get the value of a field in the message.
 * @param self a pointer to a message object.
 * @param field what field to set.
 * @return the value in the field.
 * @warning this function should not be used to get the data field. Use the commMsgGetParameter or
 *          commMsgGetAddrOfParameters functions instead.
 */
extern uint8_t commMsgGetField(commMsg *self, commMsgFields field);

/**
 * @brief It appends a new byte to the end of the packet's data section. This function also increases the
 *        packet's length by 1. If the data section reached the maximum length, this function does nothing.
 * @param self a pointer to a message object.
 * @param value is the new value to be added.
 */
extern void commMsgAppendParameter(commMsg *self, uint8_t value);

/**
 * @brief This function insert a new byte to some index of the data section. If the given index is not valid, then
 *        nothing will be written.
 * @param self a pointer to a message object.
 * @param index the position to write the data.
 * @param value is the value to be written.
 */
extern void commMsgInsertParameter(commMsg *self, uint8_t index, uint8_t value);

/**
 * @brief The function retries a parameter from data section. If the given index is bigger than the maximum size of
 *        data length, then 0 is returned.
 * @param self a pointer to a message object.
 * @param index is the index of the parameter to be read.
 * @return the content of the index.
 */
extern uint8_t commMsgGetParameter(commMsg *self, uint8_t index);

/**
 * This function returns the address of the data section.
 * @param self a pointer to a message object.
 * @return the address of the data section.
 */
extern uint8_t *commMsgGetAddrOfParameters(commMsg *self);

/**
 * This function returns the number of parameters (it is NOT the length of the data section).
 * @param self a pointer to a message object.
 * @return the number of parameters.
 */
extern uint8_t commMsgGetNumberOfParameters(commMsg *self);

/**
 * @brief This function return the memory address of the packet data (be careful!).
 * @param self a pointer to a message object.
 * @return a pointer to the packet data.
 */
extern uint8_t *commMsgGetAddrOfPacket(commMsg *self);

/**
 * @brief This function returns the total packet size (it is not the size of reserved memory).
 * @param self a pointer to a message object.
 * @return the packet total size.
 */
extern uint8_t commMsgGetTotalPacketSize(commMsg *self);

/**
 * @brief This automatically calculates the packet checksum and writes it to the proper position.
 * @param self a pointer to a message object.
 */
extern void commMsgUpdateChecksumValue(commMsg *self);

/**
 * @brief This function calculates the packet checksum and return it.
 * @param self a pointer to a message object.
 * @param the checksum of the packet
 */
extern uint8_t commMsgCalculateChecksum(commMsg *self);

/**
 * This verifies if the packet checksum is valid.
 * @param self a pointer to a message object.
 * @return true if valid and false if not valid.
 */

extern bool commMsgIsChecksumValid(commMsg *self);

/**
 *
 * @param self a pointer to a message object.
 * @param type the message type.
 */
extern void commMsgCreateEmptyMsg(commMsg *self, commMsgType type);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif //PROTOCOL_H
