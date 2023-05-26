#include <cmath>
#include "client/remote_board.hpp"

RemoteBoard *board;
uint64_t tstart;

void calledPeriodically()
{
    // This function will run automatically at a constant rate
    uint64_t now = getTimeUs();

    // It is not required to use board.readMeasurements() because
    // data will be sent automatically by the server
    double v = board->analogRead(CH1);

    // You can also send commands to the board
    double nowMs = static_cast<double>(now - tstart) * 0.001;
    board->analogWrite(CH1, sin(nowMs));
    board->sync();

    printf("Voltage = %.2f V at time %.2f ms\n", v, nowMs);
}

int main()
{
    std::string ip = "127.0.0.1";
    unsigned int port = 2000;
    board = new RemoteBoard(ip, port);

    if (board->isConnected())
    {
        double periodSecs = 0.05;
        uint64_t durationUs = 1000000;

        board->enableAutoRead(periodSecs, calledPeriodically);
        board->sync();

        tstart = getTimeUs();

        // wait until the control task is not finished
        while ((tstart + durationUs) > getTimeUs())
        {
            (void) tstart;
        }

        board->close();
    }

    return 0;
}

