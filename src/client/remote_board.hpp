#ifndef CLIENT_H
#define CLIENT_H

#ifdef WIN32
#include <SDKDDKVer.h>
#endif

#include <cstdint>
#include <iostream>
#include <algorithm>
#include <functional>
#include <thread>
#include <system_error>
#include <asio.hpp>
#include "utils/definitions.hpp"
#include "utils/memory_table.hpp"
#include "utils/protocol.h"
#include "utils/receiver.h"

class RemoteBoard {
public:
    using Callback = std::function<void(void)>;

    RemoteBoard(const std::string& ip, unsigned int port);
    ~RemoteBoard();

    bool isConnected();

    void readAll();

    void readMeasurements();

    void sync();

    /**
     * This method reads the logic state of a digital input pin.
     *
     * @param channel the INPUT GPIO channel. Example: CH1.
     * @return HIGH or LOW.
     */
    int digitalRead(channel_t channel) const;

    /**
     * This method writes HIGH or LOW value to a output digital pin.
     *
     * @param channel the OUTPUT GPIO channel. Example: CH3.
     * @param state HIGH or LOW.
     */
    void digitalWrite(channel_t channel, int state);

    int getGpioOutputState(channel_t channel) const;

    /**
     * This method reads the analog value on an analog channel.
     * @param channel the ANALOG channel. Example: CH3.
     * @return the voltage (in V) on the analog pin.
     */
    double analogRead(channel_t channel) const;

    void analogWrite(channel_t channel, double value);

    /**
     * This method sets the duty cycle of some PWM channel.
     * @param channel the PWM channel. Example: CH3.
     * @param duty a real number between 0 and 1.
     */
    void pwmSetDuty(channel_t channel, double duty);

    double pwmGetDuty(channel_t channel) const;

    /**
     * This method sets the PWM frequency.
     * @param channel the PWM channel. Example: CH0.
     * @param freqInHz the frequency (in Hz) of the PWM signal.
     */
    void pwmSetFrequency(channel_t channel, double freqInHz);

    /**
     * This method returns the actual PWM frequency.
     * @param channel the PWM channel. Example: CH5.
     * @return a real number representing the PWM frequency (in Hz).
     */
    double pwmGetFrequency(channel_t channel) const;

    /**
     * This method reads the encoder pulse count value.
     * @param encoderNumber is the encoder number. Depending on the board, it can 0, 1, 2, ..., 7.
     * @return the encoder pulse count value.
     */
    long int encoderRead(unsigned int encoderNumber) const;

    /**
     * This method resets the encoder count value.
     * @param encoderNumber is the encoder number. Depending on the board, it can 0, 1, 2, ..., 7.
     * param count is the new pulse count value.
     */
    void encoderSetCount(unsigned int encoderNumber, unsigned int count);

    /**
     * This method enables the encoder.
     * @param encoderNumber is the encoder number. It can 1, 2 or 3.
     * @param quadrature is the encoder quadrature. It can be QuadratureNone, Quadrature1X, Quadrature2X or Quadrature4X.
     */
    void encoderSetQuadrature(unsigned int encoderNumber, encoderQuadrature_t quadrature);

    /**
     * It enables the auto read feature. It will make the daq send all measurements periodically.
     *
     * @param periodSecs is the period (in seconds) between the updates.
     * @param function is a function defined by the user that will be called when new data is read from the
     *        microcontroller. This function signature must be void function(void).
     *
     * @note The period has resolution of 1 millisecond.
     */
    void enableAutoRead(double periodSecs);

    /**
     * It enables the auto read feature. It will make the daq send all measurements periodically.
     *
     * @param periodSecs is the period (in seconds) between the updates.
     * @param function is a function defined by the user that will be called when new data is read from the
     *        microcontroller. This function signature must be void function(void).
     *
     * @note The period has resolution of 1 millisecond.
     */
    void enableAutoRead(double periodSecs, Callback function);

    /**
     * It disables the auto read feature.
     */
    void disableAutoRead();

    bool isAutoReadEnabled() const;

    /**
     * This method closes the communication with the remote board.
     */
    void close();

private:
    uint8_t rxBuffer[256];
    commMsg instructionPacket, statusPacket;
    commReceiver receiver;
    MemoryTable table;
    bool waitingRx = false; ///< An auxiliary variable to the asynchronous communication.
    bool waitingTx = false; ///< An auxiliary variable to the asynchronous communication.
    bool running = true;    ///< This variable will keep the async communication running.
    unsigned int timeoutMs = 3;
    unsigned int maxRetries = 2;

    asio::io_context asioContext;
    asio::ip::tcp::socket *socket;

    std::thread* threadAsyncCommunication;
    Callback callbackUserFinishedReceivingData;

    std::vector<uint8_t> syncList;

    void addToSyncList(uint8_t address);

    /**
     * This function is used to send a message. It does not send,
     * but it provides a pointer with the message data and how many
     * bytes need to be sent.
     *
     * @param data is a pointer to the message raw data.
     * @param size is how many bytes need to be sent.
     */
    void sendRawData(uint8_t *data, uint8_t size, bool withTimeoutProtection);

    void readDataFromBoard(uint8_t firstRegister, uint8_t howMany);

    void writeDataToBoard(uint8_t firstRegister, uint8_t *data, uint8_t howMany);

    void decodeReadAnswer();

    void decodeStatusPacket();

    void initAsyncCommunication();

    void threadRun();

    void callbackFinishedReceiving(const std::error_code& ec, size_t bytesTransferred);

    void callbackFinishedSending(const std::error_code& ec, size_t bytesTransferred);
};

#endif // CLIENT_H
