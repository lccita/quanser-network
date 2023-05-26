#include <iostream>
#include "server.hpp"

typedef struct {
    std::string boardName;
    unsigned int port;
    bool help;
    bool version;
} param_t;

static bool isNumber(const std::string &s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

static bool isBoardSupported(const std::string &boardName)
{
    bool isSupported = false;
    unsigned int i = 0;
    while (i < supportedBoards.size() && isSupported == false)
    {
        isSupported = boardName.find(supportedBoards[i]) != std::string::npos;
        i = i + 1;
    }

    return isSupported;
}

static bool isPortNumberValid(unsigned int number)
{
    const bool isValid = number > 0 && number < 65536;
    return isValid;
}

static void helpMessage()
{
    std::cout << "Expected running sintax:" << std::endl;
    std::cout << "server.exe --board=NAME --port=2000" << std::endl << std::endl;
    std::cout << "Supported boards are:" << std::endl;

    for (auto item : supportedBoards)
    {
        std::cout << "- " << item << std::endl;
    }
}

static void versionMessage()
{
    std::cout << "quanser-network server v" << serverVersion[0] << "." << serverVersion[1] << std::endl;

    std::cout << "This application enables someone to connect to and control Quanser acquisition data boards remotely through TCP/IP." << std::endl;

    std::cout << "THERE IS NO WARRANTY. Use this application at your own risk." << std::endl << std::endl;

    std::cout << "Written by Jose Roberto Colombo Junior" << std::endl;
}

static void parseArgs(int argc, char **argv, param_t &param)
{
    std::string parameter;
    unsigned int i = 0;
    param.port = 0;
    param.version = false;
    param.help = false;

    while (i < argc && (param.boardName.empty() || param.port == 0))
    {
        parameter = argv[i];
        if (parameter.find("--board") != std::string::npos && param.boardName.empty())
        {
            param.boardName = &parameter[8];
        }
        else if (parameter.find("--port") != std::string::npos && param.port == 0)
        {
            if (isNumber(std::string(&parameter[7])))
                param.port = std::stoi(&parameter[7]);
        }
        else if (parameter.find("--help") != std::string::npos || argc == 1)
        {
            param.help = true;
        }
        else if (parameter.find("--version") != std::string::npos)
        {
            param.version = true;
        }

        // Test next parameter
        i = i + 1;
    }
}

int main(int argc, char **argv)
{
    param_t param;
    parseArgs(argc, argv, param);

    if (param.help)
    {
        helpMessage();
    }
    else if (param.version)
    {
        versionMessage();
    }
    else if (isBoardSupported(param.boardName))
    {
        if (isPortNumberValid(param.port))
        {
            Server server(param.port, param.boardName);

            if (server.isBoardConnected())
            {
                std::cout << "(INFO) Starting server." << std::endl;
                server.run();
                std::cout << "(INFO) Server finished." << std::endl;
            }
        }
        else
        {
            std::cout << "(ERROR) The specified port is invalid. It should be between 1 and 65536." << std::endl;
        }
    }
    else
    {
        std::cout << "(ERROR) The specified board is not supported." << std::endl;
    }

    return 0;
}
