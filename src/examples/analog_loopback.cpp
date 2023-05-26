#include "client/remote_board.hpp"

int main()
{
    std::string ip = "127.0.0.1";
    unsigned int port = 2000;
    RemoteBoard board(ip, port);

    if (board.isConnected())
    {
        board.analogWrite(CH1, 3.9);
        board.sync();

        board.readMeasurements();
        double adcValue = board.analogRead(CH1);
        std::cout << "ADC VALUE = " << adcValue << " V\n";

        board.close();
    }

    return 0;
}
