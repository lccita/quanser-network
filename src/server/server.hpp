#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32
#include <SDKDDKVer.h>
#endif

#include <iostream>
#include <algorithm>
#include <functional>
#include <thread>
#include <system_error>
#include <asio.hpp>
#include <hil.h>
#include "utils/definitions.hpp"
#include "utils/memory_table.hpp"
#include "utils/protocol.h"
#include "utils/receiver.h"

constexpr unsigned int serverVersion[] = {1, 0};

class Server {
public:
    using Callback = std::function<void(void)>;

    Server(unsigned int setPort, std::string& boardName);

    /**
     * Destructor.
     */
    ~Server();

    bool isBoardConnected();

    bool isRealBoard();

    void run();

private:
    uint8_t rxBuffer[256];           // the rx buffer
    commMsg instructionPacket, statusPacket, autoReadStatusPacket;
    commReceiver receiver;
    MemoryTable table;
    bool waitingRx = false; ///< An auxiliary variable to the asynchronous communication.
    bool waitingTx = false; ///< An auxiliary variable to the asynchronous communication.
    bool running = true;    ///< This variable will keep the async communication running.
    unsigned int timeoutMs = 3;
    unsigned int maxRetries = 2;
    t_card card;
    //boost::thread* threadBoardRead;
    Callback callbackUserFinishedReceivingData;
    asio::io_context ioContext;
    asio::ip::tcp::socket *socket;
    asio::ip::tcp::acceptor *acceptor;
    uint16_t port;

    void loadDefaultValues();

    void setDefaultConfig();

    void boardSetAdcRanges();

    void boardSetAdcModes();

    void boardSetEncoderCount();

    void boardSetEncoderQuadrature();

    void boardSetPwmDuty();

    void boardSetPwmFrequency();

    void boardSetPwmConfig();

    void boardSetPwmDeadTime();

    void boardSetDacRanges();

    void boardRead();

    void processWriteInstruction(uint16_t addr, int16_t size, const uint8_t* values);

    void handleWriteInstruction();

    void handleMultWriteInstruction();

    void handleReadInstruction();

    void makeAutoReadMsg();

    bool processPacket();

    void callbackFinishedAccepting(const std::error_code& ec);

    void callbackFinishedReceiving(const std::error_code& ec, size_t bytesTransferred);

    void callbackFinishedSending(const std::error_code& ec, size_t bytesTransferred);

    void communicationStart();
    void communicationAsyncSend(uint8_t* data, uint16_t size);
    void communicationAsyncReceive();
};


#endif //SERVER_H
