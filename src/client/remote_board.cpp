#include "remote_board.hpp"

RemoteBoard::RemoteBoard(const std::string& ip, unsigned int port)
{
    // auxiliary variables
    uint8_t i;

    // initialize the receiver
    commReceiverInit(&receiver, &statusPacket);

    // make sure the memory table is initialized with zeros
    memset(table.memory, 0, 256);

    // communication setup
    std::error_code ec;
    asio::ip::address validIp = asio::ip::address::from_string(ip, ec);
    if (ec.value() != 0)
    {
        std::cout << "(error) Given IP is invalid" << std::endl;
        return;
    }
    else
    {
        asio::ip::tcp::endpoint endpoint(validIp, port);
        socket = new asio::ip::tcp::socket(asioContext);
        socket->connect(endpoint);

        // the server will only accept connections if a board was found on it
        if (isConnected())
        {
            initAsyncCommunication();

            // make sure the board configuration is properly loaded into memory table buffer
            table.Reg.Setup.BoardModel.value = boardNotFound;
            i = maxRetries;
            while ((i > 0) && (table.Reg.Setup.BoardModel.value == boardNotFound))
            {
                readAll();
                delayMs(10);
                i = i - 1;
            }

            if (table.Reg.Setup.BoardModel.value == boardNotFound)
            {
                // in this case there is some communication problem and it is better
                // to close the socket
                socket->close();
            }
        }
    }
}

RemoteBoard::~RemoteBoard()
{
    close();
    delete socket;
}

bool RemoteBoard::isConnected()
{
    return socket->is_open();
}

void RemoteBoard::readAll()
{
    readDataFromBoard(0, 128);
    readDataFromBoard(128, 128);
}

void RemoteBoard::readMeasurements()
{
    readDataFromBoard(0, 72);
}

void RemoteBoard::sync()
{
    uint8_t address, size;
    size_t i, j;

    if (!syncList.empty())
    {
        commMsgCreateEmptyMsg(&instructionPacket, commMsgTypeMultiWrite);

        for (i = 0; i < syncList.size(); i++)
        {
            address = syncList[i];

            switch (address) {
            case MemoryTableDescription::Reg::Readings::Encoder0::address:
                size = 32;
                break;

            case MemoryTableDescription::Reg::Action::PwmDuty::Ch0::address:
                size = 32;
                break;

            case MemoryTableDescription::Reg::Action::Dac::Ch0::address:
                size = 32;
                break;

            case MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch0::address:
                size = 16;
                break;

            case MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch0::address:
                size = 32;
                break;

            case MemoryTableDescription::Reg::Setup::Pwm::Config::Ch0::address:
                size = 8;
                break;

            case MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address:
                size = 32;
                break;

            case MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch0::address:
                size = 16;
                break;

            default:
                size = registerSize(address);
                break;
            }

            commMsgAppendParameter(&instructionPacket, address);
            commMsgAppendParameter(&instructionPacket, size);

            for (j = 0; j < size; j++)
            {
                commMsgAppendParameter(&instructionPacket, table.memory[address + j]);
            }
        }

        syncList.clear();

        commMsgUpdateChecksumValue(&instructionPacket);
        size = commMsgGetTotalPacketSize(&instructionPacket);
        sendRawData(instructionPacket.buffer, size, true);
    }
}

int RemoteBoard::digitalRead(channel_t channel) const
{
    // Auxiliary variables
    int state = -1;

    switch (channel) {
        case CH0:
            state = table.Reg.Readings.GpioInput.value.bit.Ch0;
            break;
        case CH1:
            state = table.Reg.Readings.GpioInput.value.bit.Ch1;
            break;
        case CH2:
            state = table.Reg.Readings.GpioInput.value.bit.Ch2;
            break;
        case CH3:
            state = table.Reg.Readings.GpioInput.value.bit.Ch3;
            break;
        case CH4:
            state = table.Reg.Readings.GpioInput.value.bit.Ch4;
            break;
        case CH5:
            state = table.Reg.Readings.GpioInput.value.bit.Ch5;
            break;
        case CH6:
            state = table.Reg.Readings.GpioInput.value.bit.Ch6;
            break;
        case CH7:
            state = table.Reg.Readings.GpioInput.value.bit.Ch7;
            break;
        default:
            break;
    }

    return state;
}

void RemoteBoard::digitalWrite(channel_t channel, int state)
{
    switch (channel) {
        case CH0:
            table.Reg.Action.GpioOutput.value.bit.Ch0 = state;
            break;
        case CH1:
            table.Reg.Action.GpioOutput.value.bit.Ch1 = state;
            break;
        case CH2:
            table.Reg.Action.GpioOutput.value.bit.Ch2 = state;
            break;
        case CH3:
            table.Reg.Action.GpioOutput.value.bit.Ch3 = state;
            break;
        case CH4:
            table.Reg.Action.GpioOutput.value.bit.Ch4 = state;
            break;
        case CH5:
            table.Reg.Action.GpioOutput.value.bit.Ch5 = state;
            break;
        case CH6:
            table.Reg.Action.GpioOutput.value.bit.Ch6 = state;
            break;
        case CH7:
            table.Reg.Action.GpioOutput.value.bit.Ch7 = state;
            break;
        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Action::GpioOutput::address);
}

int RemoteBoard::getGpioOutputState(channel_t channel) const
{
    // Auxiliary variables
    int state = -1;

    switch (channel) {
        case CH0:
            state = table.Reg.Action.GpioOutput.value.bit.Ch0;
            break;
        case CH1:
            state = table.Reg.Action.GpioOutput.value.bit.Ch1;
            break;
        case CH2:
            state = table.Reg.Action.GpioOutput.value.bit.Ch2;
            break;
        case CH3:
            state = table.Reg.Action.GpioOutput.value.bit.Ch3;
            break;
        case CH4:
            state = table.Reg.Action.GpioOutput.value.bit.Ch4;
            break;
        case CH5:
            state = table.Reg.Action.GpioOutput.value.bit.Ch5;
            break;
        case CH6:
            state = table.Reg.Action.GpioOutput.value.bit.Ch6;
            break;
        case CH7:
            state = table.Reg.Action.GpioOutput.value.bit.Ch7;
            break;
        default:
            break;
    }

    return state;
}

double RemoteBoard::analogRead(channel_t channel) const
{
    // Auxiliary variables
    double value = 0;

    switch (channel) {
        case CH0:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch0.value.asFloat);
            break;
        case CH1:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch1.value.asFloat);
            break;
        case CH2:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch2.value.asFloat);
            break;
        case CH3:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch3.value.asFloat);
            break;
        case CH4:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch4.value.asFloat);
            break;
        case CH5:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch5.value.asFloat);
            break;
        case CH6:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch6.value.asFloat);
            break;
        case CH7:
            value = static_cast<double>(table.Reg.Readings.Adc.Ch7.value.asFloat);
            break;
        default:
            break;
    }

    return value;
}

void RemoteBoard::analogWrite(channel_t channel, double value)
{
    // All DAC operations are done at once to be faster and always trying to keep synchronism
    // That's the reason why all channels will add CH0 address to syncList

    switch (channel) {
        case CH0:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch0.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch0.value.asInteger));
            table.Reg.Action.Dac.Ch0.value.asFloat = static_cast<float>(value);
            break;
        case CH1:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch1.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch1.value.asInteger));
            table.Reg.Action.Dac.Ch1.value.asFloat = static_cast<float>(value);
            break;
        case CH2:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch2.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch2.value.asInteger));
            table.Reg.Action.Dac.Ch2.value.asFloat = static_cast<float>(value);
            break;
        case CH3:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch3.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch3.value.asInteger));
            table.Reg.Action.Dac.Ch3.value.asFloat = static_cast<float>(value);
            break;
        case CH4:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch4.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch4.value.asInteger));
            table.Reg.Action.Dac.Ch4.value.asFloat = static_cast<float>(value);
            break;
        case CH5:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch5.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch5.value.asInteger));
            table.Reg.Action.Dac.Ch5.value.asFloat = static_cast<float>(value);
            break;
        case CH6:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch6.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch6.value.asInteger));
            table.Reg.Action.Dac.Ch6.value.asFloat = static_cast<float>(value);
            break;
        case CH7:
            value = std::max(value, double(table.Reg.Setup.Adc.Range.Min.Ch7.value.asInteger));
            value = std::min(value, double(table.Reg.Setup.Adc.Range.Max.Ch7.value.asInteger));
            table.Reg.Action.Dac.Ch7.value.asFloat = static_cast<float>(value);
            break;
        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Action::Dac::Ch0::address);
}

void RemoteBoard::configAdc(channel_t channel, adcMode_t mode, double minRange, double maxRange)
{
    switch (channel) {
        case Ch0:
            table.Reg.Setup.Adc.Mode.value.bit.Ch0 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch0.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch0.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch1:
            table.Reg.Setup.Adc.Mode.value.bit.Ch1 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch1.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch1.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch2:
            table.Reg.Setup.Adc.Mode.value.bit.Ch2 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch2.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch2.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch3:
            table.Reg.Setup.Adc.Mode.value.bit.Ch3 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch3.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch3.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch4:
            table.Reg.Setup.Adc.Mode.value.bit.Ch4 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch4.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch4.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch5:
            table.Reg.Setup.Adc.Mode.value.bit.Ch5 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch5.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch5.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch6:
            table.Reg.Setup.Adc.Mode.value.bit.Ch6 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch6.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch6.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch7:
            table.Reg.Setup.Adc.Mode.value.bit.Ch7 = mode;
            table.Reg.Setup.Adc.Range.Min.Ch7.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Adc.Range.Max.Ch7.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch0::address);
}

void RemoteBoard::configDac(channel_t channel, double minRange, double maxRange)
{
    switch (channel) {
        case Ch0:
            table.Reg.Setup.Dac.Range.Min.Ch0.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch0.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch1:
            table.Reg.Setup.Dac.Range.Min.Ch1.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch1.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch2:
            table.Reg.Setup.Dac.Range.Min.Ch2.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch2.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch3:
            table.Reg.Setup.Dac.Range.Min.Ch3.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch3.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch4:
            table.Reg.Setup.Dac.Range.Min.Ch4.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch4.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch5:
            table.Reg.Setup.Dac.Range.Min.Ch5.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch5.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch6:
            table.Reg.Setup.Dac.Range.Min.Ch6.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch6.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        case Ch7:
            table.Reg.Setup.Dac.Range.Min.Ch7.value.asInteger = static_cast<int8_t>(minRange);
            table.Reg.Setup.Dac.Range.Max.Ch7.value.asInteger = static_cast<int8_t>(maxRange);
            break;

        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch0::address);
}

void RemoteBoard::pwmSetDuty(channel_t channel, double duty)
{
    duty = std::max(duty, 0.0);
    duty = std::min(duty, 1.0);

    switch (channel) {
        case CH0:
            table.Reg.Action.PwmDuty.Ch0.value.asFloat = static_cast<float>(duty);
            break;
        case CH1:
            table.Reg.Action.PwmDuty.Ch1.value.asFloat = static_cast<float>(duty);
            break;
        case CH2:
            table.Reg.Action.PwmDuty.Ch2.value.asFloat = static_cast<float>(duty);
            break;
        case CH3:
            table.Reg.Action.PwmDuty.Ch3.value.asFloat = static_cast<float>(duty);
            break;
        case CH4:
            table.Reg.Action.PwmDuty.Ch4.value.asFloat = static_cast<float>(duty);
            break;
        case CH5:
            table.Reg.Action.PwmDuty.Ch5.value.asFloat = static_cast<float>(duty);
            break;
        case CH6:
            table.Reg.Action.PwmDuty.Ch6.value.asFloat = static_cast<float>(duty);
            break;
        case CH7:
            table.Reg.Action.PwmDuty.Ch7.value.asFloat = static_cast<float>(duty);
            break;
        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Action::PwmDuty::Ch0::address);
}

double RemoteBoard::pwmGetDuty(channel_t channel) const
{
    double duty = 0.0f;

    switch (channel) {
    case CH0:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch0.value.asFloat);
        break;
    case CH1:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch1.value.asFloat);
        break;
    case CH2:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch2.value.asFloat);
        break;
    case CH3:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch3.value.asFloat);
        break;
    case CH4:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch4.value.asFloat);
        break;
    case CH5:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch5.value.asFloat);
        break;
    case CH6:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch6.value.asFloat);
        break;
    case CH7:
        duty = static_cast<double>(table.Reg.Action.PwmDuty.Ch7.value.asFloat);
        break;
    default:
        break;
    }

    return duty;
}

void RemoteBoard::pwmSetFrequency(channel_t channel, double freqInHz)
{
    const uint8_t model = table.Reg.Setup.BoardModel.value;
    const double pwmMinFrequency = boardInfo[model][featurePwmMinFreqHz];
    const double pwmMaxFrequency = boardInfo[model][featurePwmMaxFreqHz];

    freqInHz = std::min(freqInHz, pwmMaxFrequency);
    freqInHz = std::min(freqInHz, pwmMinFrequency);

    switch (channel) {
        case CH0:
            table.Reg.Setup.Pwm.Frequency.Ch0.value.asFloat = static_cast<float>(freqInHz);
            break;
        case CH1:
            table.Reg.Setup.Pwm.Frequency.Ch1.value.asFloat = static_cast<float>(freqInHz);
            break;
        case CH2:
            table.Reg.Setup.Pwm.Frequency.Ch2.value.asFloat = static_cast<float>(freqInHz);
            break;
        case CH3:
            table.Reg.Setup.Pwm.Frequency.Ch3.value.asFloat = static_cast<float>(freqInHz);
            break;
        case CH4:
            table.Reg.Setup.Pwm.Frequency.Ch4.value.asFloat = static_cast<float>(freqInHz);
            break;
        case CH5:
            table.Reg.Setup.Pwm.Frequency.Ch5.value.asFloat = static_cast<float>(freqInHz);
            break;
        case CH6:
            table.Reg.Setup.Pwm.Frequency.Ch6.value.asFloat = static_cast<float>(freqInHz);
            break;
        case CH7:
            table.Reg.Setup.Pwm.Frequency.Ch7.value.asFloat = static_cast<float>(freqInHz);
            break;
        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch0::address);
}

double RemoteBoard::pwmGetFrequency(channel_t channel) const
{
    float value = -1;

    switch (channel) {
        case CH0:
            value = table.Reg.Setup.Pwm.Frequency.Ch0.value.asFloat;
            break;
        case CH1:
            value = table.Reg.Setup.Pwm.Frequency.Ch1.value.asFloat;
            break;
        case CH2:
            value = table.Reg.Setup.Pwm.Frequency.Ch2.value.asFloat;
            break;
        case CH3:
            value = table.Reg.Setup.Pwm.Frequency.Ch3.value.asFloat;
            break;
        case CH4:
            value = table.Reg.Setup.Pwm.Frequency.Ch4.value.asFloat;
            break;
        case CH5:
            value = table.Reg.Setup.Pwm.Frequency.Ch5.value.asFloat;
            break;
        case CH6:
            value = table.Reg.Setup.Pwm.Frequency.Ch6.value.asFloat;
            break;
        case CH7:
            value = table.Reg.Setup.Pwm.Frequency.Ch7.value.asFloat;
            break;
        default:
            break;
    }

    return static_cast<double>(value);
}

void RemoteBoard::configPwm(channel_t channel, pwmAlign_t align, pwmConfig_t config, pwmMode_t mode, pwmPolarity_t polarity)
{
    switch (channel) {
        case Ch0:
            table.Reg.Setup.Pwm.Config.Ch0.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch0.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch0.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch0.bit.polarity = polarity;
            break;

        case Ch1:
            table.Reg.Setup.Pwm.Config.Ch1.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch1.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch1.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch1.bit.polarity = polarity;
            break;

        case Ch2:
            table.Reg.Setup.Pwm.Config.Ch2.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch2.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch2.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch2.bit.polarity = polarity;
            break;

        case Ch3:
            table.Reg.Setup.Pwm.Config.Ch3.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch3.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch3.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch3.bit.polarity = polarity;
            break;

        case Ch4:
            table.Reg.Setup.Pwm.Config.Ch4.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch4.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch4.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch4.bit.polarity = polarity;
            break;

        case Ch5:
            table.Reg.Setup.Pwm.Config.Ch5.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch5.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch5.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch5.bit.polarity = polarity;
            break;

        case Ch6:
            table.Reg.Setup.Pwm.Config.Ch6.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch6.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch6.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch6.bit.polarity = polarity;
            break;

        case Ch7:
            table.Reg.Setup.Pwm.Config.Ch7.bit.mode = mode;
            table.Reg.Setup.Pwm.Config.Ch7.bit.config = config;            table.Reg.Setup.Pwm.Config.Ch7.bit.alignment = align;
            table.Reg.Setup.Pwm.Config.Ch7.bit.polarity = polarity;
            break;

        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch0::address);
}

void RemoteBoard::setPwmDeadTime(channel_t channel, double risingEdgeTimeNs, double fallingEdgeTimeNs)
{
    switch (channel) {
        case Ch0:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch0 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch0 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        case Ch1:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch1 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch1 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        case Ch2:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch2 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch2 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        case Ch3:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch3 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch3 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        case Ch4:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch4 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch4 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        case Ch5:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch5 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch5 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        case Ch6:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch6 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch6 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        case Ch7:
            table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch7 = static_cast<uint16_t>(risingEdgeTimeNs);
            table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch7 = static_cast<uint16_t>(fallingEdgeTimeNs);
            break;

        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address);
}

long int RemoteBoard::encoderRead(unsigned int encoderNumber) const
{
    int32_t count;
    switch (encoderNumber) {
        case 0:
            count = table.Reg.Readings.Encoder0.value.asInteger;
            break;
        case 1:
            count = table.Reg.Readings.Encoder1.value.asInteger;
            break;
        case 2:
            count = table.Reg.Readings.Encoder2.value.asInteger;
            break;
        case 3:
            count = table.Reg.Readings.Encoder3.value.asInteger;
            break;
        case 4:
            count = table.Reg.Readings.Encoder4.value.asInteger;
            break;
        case 5:
            count = table.Reg.Readings.Encoder5.value.asInteger;
            break;
        case 6:
            count = table.Reg.Readings.Encoder6.value.asInteger;
            break;
        case 7:
            count = table.Reg.Readings.Encoder7.value.asInteger;
            break;
        default:
            count = 0;
            break;
    }

    return static_cast<long int>(count);
}

void RemoteBoard::encoderSetCount(unsigned int encoderNumber, unsigned int count)
{
    const auto newValue = static_cast<int32_t>(count);
    switch (encoderNumber) {
        case 0:
            table.Reg.Readings.Encoder0.value.asInteger = newValue;
            break;
        case 1:
            table.Reg.Readings.Encoder1.value.asInteger = newValue;
            break;
        case 2:
            table.Reg.Readings.Encoder2.value.asInteger = newValue;
            break;
        case 3:
            table.Reg.Readings.Encoder3.value.asInteger = newValue;
            break;
        case 4:
            table.Reg.Readings.Encoder4.value.asInteger = newValue;
            break;
        case 5:
            table.Reg.Readings.Encoder5.value.asInteger = newValue;
            break;
        case 6:
            table.Reg.Readings.Encoder6.value.asInteger = newValue;
            break;
        case 7:
            table.Reg.Readings.Encoder7.value.asInteger = newValue;
            break;
        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Readings::Encoder0::address);
}

void RemoteBoard::encoderSetQuadrature(unsigned int encoderNumber, encoderQuadrature_t quadrature)
{
    switch (encoderNumber) {
        case 0:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder0 = quadrature;
            break;
        case 1:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder1 = quadrature;
            break;
        case 2:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder2 = quadrature;
            break;
        case 3:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder3 = quadrature;
            break;
        case 4:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder4 = quadrature;
            break;
        case 5:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder5 = quadrature;
            break;
        case 6:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder6 = quadrature;
            break;
        case 7:
            table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder7 = quadrature;
            break;
        default:
            break;
    }

    addToSyncList(MemoryTableDescription::Reg::Setup::Encoder::Quadrature::address);
}

void RemoteBoard::enableAutoRead(double periodSecs)
{
    periodSecs = std::max(std::abs(periodSecs), 0.001);
    table.Reg.Setup.AutoRead.Period.value.asBit32 = static_cast<uint32_t>(1000000 * periodSecs);
    addToSyncList(MemoryTableDescription::Reg::Setup::AutoRead::Period::address);
}

void RemoteBoard::enableAutoRead(double periodSecs, Callback function)
{
    // set the user callback
    callbackUserFinishedReceivingData = std::move(function);

    // and enable the autoread
    enableAutoRead(periodSecs);
}

void RemoteBoard::disableAutoRead()
{
    table.Reg.Setup.AutoRead.Length.value = 0;
    addToSyncList(MemoryTableDescription::Reg::Setup::AutoRead::Length::address);
}

bool RemoteBoard::isAutoReadEnabled() const
{
    return table.Reg.Setup.AutoRead.Length.value > 0;
}

void RemoteBoard::close()
{
    if (isAutoReadEnabled())
    {
        // then the auto-read was enabled some time, just to make sure, let's disable it
        disableAutoRead();
        sync();
    }

    if (isConnected())
    {
        running = false;
        socket->close();
        asioContext.stop();

        // let's wait some time until the thread ends itself
        delayMs(50);
        //thread_async_communication->join();

        asioContext.restart();
    }
}

void RemoteBoard::addToSyncList(uint8_t address)
{
    /*
     * In order to improve the synchronism of applying control signals, any memory access to the section "Action" will
     * force to update the entire section. The encoders count also required this procedure.
     */

    // auxiliary variables
    std::vector<uint8_t>::iterator it;

    // if the element is not in the list
    it = find(syncList.begin(), syncList.end(), address);
    if (it == syncList.end())
    {
        syncList.push_back(address);
    }
}

void RemoteBoard::sendRawData(uint8_t* data, uint8_t size, bool withTimeoutProtection)
{
    // auxiliary variables
    waitingTx = withTimeoutProtection;
    uint64_t deadlineUs;

    // schedule a write operation that will invoke the callback only when all bytes were sent
    socket->async_send(
        asio::buffer(data, size),
        std::bind(
            &RemoteBoard::callbackFinishedSending,
            this,
            std::placeholders::_1,  // asio::placeholders::error
            std::placeholders::_2   // asio::placeholders::bytes_transferred
        )
    );

    // here we set the timer to wait for some time
    if (withTimeoutProtection)
    {
        deadlineUs = getTimeUs() + 1000 * timeoutMs;
        while ((deadlineUs > getTimeUs()) && waitingTx) {}
    }
}

void RemoteBoard::readDataFromBoard(uint8_t firstRegister, uint8_t howMany)
{
    // auxiliary variables
    unsigned int retries = maxRetries;
    uint64_t deadline_us;
    uint8_t size;

    // build a new instruction packet
    commMsgCreateEmptyMsg(&instructionPacket, commMsgTypeRead);
    commMsgAppendParameter(&instructionPacket, firstRegister);
    commMsgAppendParameter(&instructionPacket, howMany);
    commMsgUpdateChecksumValue(&instructionPacket);

    // get the instruction packet information
    size = commMsgGetTotalPacketSize(&instructionPacket);

    // send the data and wait for the response
    waitingRx = true;
    while ((retries > 0) && (waitingRx))
    {
        // request data from the board
        sendRawData(instructionPacket.buffer, size, false);

        // calculate the deadline
        deadline_us = getTimeUs() + 1000 * timeoutMs;

        // now let's wait something to happen
        while ((deadline_us > getTimeUs()) && (waitingRx)) {}

        retries = retries - 1;
    }
}

void RemoteBoard::writeDataToBoard(uint8_t firstRegister, uint8_t *data, uint8_t howMany)
{
    // Auxiliary variables
    uint8_t i, size;

    // Build a new instruction packet
    commMsgCreateEmptyMsg(&instructionPacket, commMsgTypeWrite);
    commMsgAppendParameter(&instructionPacket, firstRegister);
    for (i = 0; i < howMany; i++)
    {
        commMsgAppendParameter(&instructionPacket, data[i]);
    }
    commMsgUpdateChecksumValue(&instructionPacket);

    // Send the instruction packet
    size = commMsgGetTotalPacketSize(&instructionPacket);
    sendRawData(instructionPacket.buffer, size, true);
}

void RemoteBoard::decodeReadAnswer()
{
    /*
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
     */

    const uint16_t first_register = commMsgGetParameter(&statusPacket, 1);
    const uint16_t size = commMsgGetParameter(&statusPacket, 2);
    uint8_t* parameters = commMsgGetAddrOfParameters(&statusPacket);
    memcpy(&table.memory[first_register], &parameters[3], size);
}

void RemoteBoard::decodeStatusPacket()
{
    switch (commMsgGetField(&statusPacket, commMsgFieldType)) {
        case static_cast<uint8_t>(commMsgTypeAnsWrite):
            // What to do?
            break;

        case static_cast<uint8_t>(commMsgTypeAnsRead):
            decodeReadAnswer();
            break;

        default:
            break;
    }
}

void RemoteBoard::initAsyncCommunication()
{
    // initialize a new thread to keep the asio_io_context running
    threadAsyncCommunication = new std::thread(std::bind(&RemoteBoard::threadRun, this));
    threadAsyncCommunication->detach();
    delayMs(50);
}

void RemoteBoard::threadRun()
{
    while ( running )
    {
        // it may happen that the context is empty, and it requires to be restarted
        // so let's schedule a async_read to make sure it does not stop again

        socket->async_read_some(
            asio::buffer(rxBuffer, 256),
            std::bind(
                &RemoteBoard::callbackFinishedReceiving,
                this,
                std::placeholders::_1,  // asio::placeholders::error
                std::placeholders::_2   // asio::placeholders::bytes_transferred
            )
        );

        asioContext.run();
        asioContext.restart();
    }
}

void RemoteBoard::callbackFinishedReceiving(const std::error_code& ec, size_t bytesTransferred)
{
    // prevent warnings about unused variable ec
    (void) ec;

    // auxiliary variables
    size_t i;

    // if it is not a canceled operation
    if (bytesTransferred > 0)
    {
        // process the received bytes
        for (i = 0; i < bytesTransferred; i++)
        {
            commReceiverProcess(&receiver, rxBuffer[i]);

            if (commReceiverHasNewMessage(&receiver))
            {
                decodeStatusPacket();
                if (callbackUserFinishedReceivingData)
                {
                    callbackUserFinishedReceivingData();
                }

                if (waitingRx)
                {
                    waitingRx = false;   // release the user thread
                }

                commReceiverClean(&receiver);
            }
        }
    }

    // trigger another async read before exiting to make sure that asio_context.run() is still running
    socket->async_read_some(
        asio::buffer(rxBuffer, 256),
        bind(
            &RemoteBoard::callbackFinishedReceiving,
            this,
            std::placeholders::_1,  // asio::placeholders::error
            std::placeholders::_2   // asio::placeholders::bytes_transferred
        )
    );
}

void RemoteBoard::callbackFinishedSending(const std::error_code& ec, size_t bytesTransferred)
{
    (void) ec;
    (void) bytesTransferred;

    // because all bytes were transferred, we release the waiting
    if (waitingTx) waitingTx = false;
}
