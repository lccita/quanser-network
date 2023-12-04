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

#ifdef WIN32
extern "C" NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);
extern "C" NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);
#endif

constexpr unsigned int serverVersion[] = {1, 0};

class Server {
public:
    using Callback = std::function<void(void)>;

    /**
     * The server class main constructor.
     * @param setPort is the port number that the server will be listening for client connections.
     * @param boardName is a string with the board name. @see supportedBoards
     */
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
    bool hasClientConnected = false;
    unsigned int timeoutMs = 3;
    unsigned int maxRetries = 2;
    t_card card;
    Callback callbackUserFinishedReceivingData;
    asio::io_context ioContext;
    asio::ip::tcp::socket *socket;
    asio::ip::tcp::acceptor *acceptor;
    uint16_t port;
    asio::high_resolution_timer *autoReadTimer;

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

    bool processPacket();

    void callbackFinishedAccepting(const std::error_code& ec);

    void callbackFinishedReceiving(const std::error_code& ec, size_t bytesTransferred);

    void callbackFinishedSending(const std::error_code& ec, size_t bytesTransferred);

    void communicationStart();

    void communicationAsyncSend(uint8_t* data, uint16_t size);

    void communicationAsyncReceive();
};


#endif //SERVER_H
