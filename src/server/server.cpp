#include "server.hpp"
#include "utils/is_in.hpp"
#include <string>

// todo
// add support to autoread, maybe using the asio async timers if the resolution problem is resolved
// source: https://randomascii.wordpress.com/2020/10/04/windows-timer-resolution-the-great-rule-change

static t_analog_input_configuration decodeAdcMode(adcMode_t mode)
{
    t_analog_input_configuration quanserMode;

    switch (mode) {
        case adcModeReferencedSingleEnded:
            quanserMode = ANALOG_INPUT_RSE_CONFIGURATION;
            break;

        case adcModeNonReferencedSingleEnded:
            quanserMode = ANALOG_INPUT_NRSE_CONFIGURATION;
            break;

        case adcModeDifferential:
            quanserMode = ANALOG_INPUT_DIFF_CONFIGURATION;
            break;

        case adcModePseudoDifferential:
            quanserMode = ANALOG_INPUT_PDIFF_CONFIGURATION;
            break;
    }

    return quanserMode;
}

static t_encoder_quadrature_mode decodeQuadrature(encoderQuadrature_t quadrature)
{
    t_encoder_quadrature_mode quanserQuadrature;

    switch (quadrature) {
        case quadratureNone:
            quanserQuadrature = ENCODER_QUADRATURE_NONE;
            break;

        case quadrature1X:
            quanserQuadrature = ENCODER_QUADRATURE_1X;
            break;

        case quadrature2X:
            quanserQuadrature = ENCODER_QUADRATURE_2X;
            break;

        case quadrature4X:
            quanserQuadrature = ENCODER_QUADRATURE_4X;
            break;
    }

    return quanserQuadrature;
}

static t_pwm_alignment decodePwmAlign(pwmAlign_t align)
{
    t_pwm_alignment quanserAlign;

    switch (align) {
        case pwmAlignRisingEdge:
            quanserAlign = PWM_LEADING_EDGE_ALIGNED;
            break;

        case pwmAlignFallingEdge:
            quanserAlign = PWM_TRAILING_EDGE_ALIGNED;
            break;

        case pwmAlignCenter:
            quanserAlign = PWM_CENTER_ALIGNED;
            break;
    }

    return quanserAlign;
}

static t_pwm_configuration decodePwmConfig(pwmConfig_t config)
{
    t_pwm_configuration quanserConfig;

    switch (config) {
        case pwmConfigUnipolar:
            quanserConfig = PWM_UNIPOLAR_CONFIGURATION;
            break;

        case pwmConfigBipolar:
            quanserConfig = PWM_BIPOLAR_CONFIGURATION;
            break;

        case pwmConfigPaired:
            quanserConfig = PWM_PAIRED_CONFIGURATION;
            break;

        case pwmConfigComplementary:
            quanserConfig = PWM_COMPLEMENTARY_CONFIGURATION;
            break;
    }

    return quanserConfig;
}

static t_pwm_mode decodePwmMode(pwmMode_t mode)
{
    t_pwm_mode quanserMode;

    switch (mode) {
        case pwmModeDutyCycle:
            quanserMode = PWM_DUTY_CYCLE_MODE;
            break;

        case pwmModeFrequency:
            quanserMode = PWM_FREQUENCY_MODE;
            break;

        case pwmModePeriod:
            quanserMode = PWM_PERIOD_MODE;
            break;

        case pwmModeOneShot:
            quanserMode = PWM_ONE_SHOT_MODE;
            break;

        case pwmModeTime:
            quanserMode = PWM_TIME_MODE;
            break;

        case pwmModeEncoderEmulation:
            quanserMode = PWM_ENCODER_EMULATION_MODE;
            break;

        case pwmModeRaw:
            quanserMode = PWM_RAW_MODE;
    }

    return quanserMode;
}

static t_pwm_polarity decodePwmPolarity(pwmPolarity_t polarity)
{
    t_pwm_polarity quanserPolarity;

    switch (polarity) {
        case pwmPolarityLow:
            quanserPolarity = PWM_ACTIVE_LOW_POLARITY;
            break;

        case pwmPolarityHigh:
            quanserPolarity = PWM_ACTIVE_HIGH_POLARITY;
            break;
    }

    return quanserPolarity;
}

Server::Server(unsigned int setPort, std::string& boardName) : port(setPort)
{
    using namespace asio::ip;

    // auxiliary variables
    t_error error;

    // initialize the receiver
    commReceiverInit(&receiver, &instructionPacket);

    // make sure the memory table is initialized with zeros
    memset(table.memory, 0, 256);

    // load default values to the memory table
    loadDefaultValues();

    // try to open the board
    if (boardName.compare("dummy") == 0)
    {
        table.Reg.Setup.BoardModel.value = boardDummy;
    }
    else
    {
        table.Reg.Setup.BoardModel.value = boardNotFound;
        error = hil_open(boardName.c_str(), "0", &card);
        if (error == QERR_NO_ERROR)
        {
            if (boardName.compare("q2_usb") == 0)
            {
                table.Reg.Setup.BoardModel.value = boardQ2USB;
            }
            else if (boardName.compare("q8_usb") == 0)
            {
                table.Reg.Setup.BoardModel.value = boardQ8USB;
            }
        }
    }

    if (table.Reg.Setup.BoardModel.value != boardNotFound)
    {
        communicationStart();
    }
    else
    {
        std::cout << "(ERROR) Board is not connected." << std::endl;
    }
}

Server::~Server()
{
    if (isBoardConnected())
    {
        hil_close(card);
    }

    socket->close();
    delete socket;
    delete acceptor;

}

bool Server::isBoardConnected()
{
    return table.Reg.Setup.BoardModel.value != boardNotFound;
}

bool Server::isRealBoard()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    return boardModel != boardNotFound && boardModel != boardDummy;
}

void Server::run()
{
    if (hasClientConnected)
        communicationAsyncReceive();
    ioContext.run();
    ioContext.restart();
}

void Server::loadDefaultValues()
{
    table.Reg.Setup.Adc.Range.Min.Ch0.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Min.Ch1.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Min.Ch2.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Min.Ch3.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Min.Ch4.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Min.Ch5.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Min.Ch6.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Min.Ch7.value.asInteger = -10;
    table.Reg.Setup.Adc.Range.Max.Ch0.value.asInteger = 10;
    table.Reg.Setup.Adc.Range.Max.Ch1.value.asInteger = 10;
    table.Reg.Setup.Adc.Range.Max.Ch2.value.asInteger = 10;
    table.Reg.Setup.Adc.Range.Max.Ch3.value.asInteger = 10;
    table.Reg.Setup.Adc.Range.Max.Ch4.value.asInteger = 10;
    table.Reg.Setup.Adc.Range.Max.Ch5.value.asInteger = 10;
    table.Reg.Setup.Adc.Range.Max.Ch6.value.asInteger = 10;
    table.Reg.Setup.Adc.Range.Max.Ch7.value.asInteger = 10;
    table.Reg.Setup.Adc.Mode.value.bit.Ch0 = adcModeReferencedSingleEnded;
    table.Reg.Setup.Adc.Mode.value.bit.Ch1 = adcModeReferencedSingleEnded;
    table.Reg.Setup.Adc.Mode.value.bit.Ch2 = adcModeReferencedSingleEnded;
    table.Reg.Setup.Adc.Mode.value.bit.Ch3 = adcModeReferencedSingleEnded;
    table.Reg.Setup.Adc.Mode.value.bit.Ch4 = adcModeReferencedSingleEnded;
    table.Reg.Setup.Adc.Mode.value.bit.Ch5 = adcModeReferencedSingleEnded;
    table.Reg.Setup.Adc.Mode.value.bit.Ch6 = adcModeReferencedSingleEnded;
    table.Reg.Setup.Adc.Mode.value.bit.Ch7 = adcModeReferencedSingleEnded;

    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder0 = quadrature4X;
    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder1 = quadrature4X;
    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder2 = quadrature4X;
    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder3 = quadrature4X;
    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder4 = quadrature4X;
    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder5 = quadrature4X;
    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder6 = quadrature4X;
    table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder7 = quadrature4X;

    table.Reg.Setup.Pwm.Frequency.Ch0.value.asFloat = 20000.0;
    table.Reg.Setup.Pwm.Frequency.Ch1.value.asFloat = 20000.0;
    table.Reg.Setup.Pwm.Frequency.Ch2.value.asFloat = 20000.0;
    table.Reg.Setup.Pwm.Frequency.Ch3.value.asFloat = 20000.0;
    table.Reg.Setup.Pwm.Frequency.Ch4.value.asFloat = 20000.0;
    table.Reg.Setup.Pwm.Frequency.Ch5.value.asFloat = 20000.0;
    table.Reg.Setup.Pwm.Frequency.Ch6.value.asFloat = 20000.0;
    table.Reg.Setup.Pwm.Frequency.Ch7.value.asFloat = 20000.0;

    table.Reg.Setup.Pwm.Config.Ch0.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch0.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch0.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch0.bit.polarity = pwmPolarityHigh;
    table.Reg.Setup.Pwm.Config.Ch1.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch1.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch1.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch1.bit.polarity = pwmPolarityHigh;
    table.Reg.Setup.Pwm.Config.Ch2.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch2.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch2.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch2.bit.polarity = pwmPolarityHigh;
    table.Reg.Setup.Pwm.Config.Ch3.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch3.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch3.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch3.bit.polarity = pwmPolarityHigh;
    table.Reg.Setup.Pwm.Config.Ch4.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch4.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch4.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch4.bit.polarity = pwmPolarityHigh;
    table.Reg.Setup.Pwm.Config.Ch5.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch5.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch5.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch5.bit.polarity = pwmPolarityHigh;
    table.Reg.Setup.Pwm.Config.Ch6.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch6.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch6.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch6.bit.polarity = pwmPolarityHigh;
    table.Reg.Setup.Pwm.Config.Ch7.bit.alignment = pwmAlignRisingEdge;
    table.Reg.Setup.Pwm.Config.Ch7.bit.config = pwmConfigUnipolar;
    table.Reg.Setup.Pwm.Config.Ch7.bit.mode = pwmModeDutyCycle;
    table.Reg.Setup.Pwm.Config.Ch7.bit.polarity = pwmPolarityHigh;

    table.Reg.Setup.Dac.Range.Min.Ch0.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Min.Ch1.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Min.Ch2.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Min.Ch3.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Min.Ch4.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Min.Ch5.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Min.Ch6.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Min.Ch7.value.asInteger = -10;
    table.Reg.Setup.Dac.Range.Max.Ch0.value.asInteger = 10;
    table.Reg.Setup.Dac.Range.Max.Ch1.value.asInteger = 10;
    table.Reg.Setup.Dac.Range.Max.Ch2.value.asInteger = 10;
    table.Reg.Setup.Dac.Range.Max.Ch3.value.asInteger = 10;
    table.Reg.Setup.Dac.Range.Max.Ch4.value.asInteger = 10;
    table.Reg.Setup.Dac.Range.Max.Ch5.value.asInteger = 10;
    table.Reg.Setup.Dac.Range.Max.Ch6.value.asInteger = 10;
    table.Reg.Setup.Dac.Range.Max.Ch7.value.asInteger = 10;

    table.Reg.Setup.WhoAmI.value = MemoryTableDescription::Reg::Setup::WhoAmI::defaultValue;
}

void Server::boardSetAdcRanges()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const int numberOfAdcs = boardInfo[boardModel][featureAdcChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_double mins[8] = {
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch0.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch1.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch2.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch3.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch4.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch5.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch6.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Min.Ch7.value.asInteger)
    };

    const t_double maxs[8] = {
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch0.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch1.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch2.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch3.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch4.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch5.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch6.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Adc.Range.Max.Ch7.value.asInteger)
    };

    hil_set_analog_input_ranges(card, channels, numberOfAdcs, mins, maxs);
}

void Server::boardSetAdcModes()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const int numberOfAdcs = boardInfo[boardModel][featureAdcChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_analog_input_configuration config[8] {
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch0)),
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch1)),
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch2)),
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch3)),
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch4)),
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch5)),
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch6)),
       decodeAdcMode(static_cast<adcMode_t>(table.Reg.Setup.Adc.Mode.value.bit.Ch7))
    };

    hil_set_analog_input_configuration(card, channels, numberOfAdcs, config);
}

void Server::boardSetEncoderCount()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const uint8_t numberOfEncs = boardInfo[boardModel][featureEncoderChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_int32 counts[8] = {
        static_cast<t_int32>(table.Reg.Readings.Encoder0.value.asInteger),
        static_cast<t_int32>(table.Reg.Readings.Encoder1.value.asInteger),
        static_cast<t_int32>(table.Reg.Readings.Encoder2.value.asInteger),
        static_cast<t_int32>(table.Reg.Readings.Encoder3.value.asInteger),
        static_cast<t_int32>(table.Reg.Readings.Encoder4.value.asInteger),
        static_cast<t_int32>(table.Reg.Readings.Encoder5.value.asInteger),
        static_cast<t_int32>(table.Reg.Readings.Encoder6.value.asInteger),
        static_cast<t_int32>(table.Reg.Readings.Encoder7.value.asInteger)
    };

    hil_set_encoder_counts(card, channels, numberOfEncs, counts);
}

void Server::boardSetEncoderQuadrature()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const uint8_t numberOfEncs = boardInfo[boardModel][featureEncoderChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_encoder_quadrature_mode config[8] = {
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder0)),
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder1)),
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder2)),
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder3)),
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder4)),
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder5)),
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder6)),
        decodeQuadrature(static_cast<encoderQuadrature_t>(table.Reg.Setup.Encoder.Quadrature.value.bit.Encoder7))
    };

    hil_set_encoder_quadrature_mode(card, channels, numberOfEncs, config);
}

void Server::boardSetPwmDuty()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const int numberOfPwms = boardInfo[boardModel][featurePwmChannels];

    const t_double values[8] = {
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch0.value.asFloat),
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch1.value.asFloat),
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch2.value.asFloat),
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch3.value.asFloat),
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch4.value.asFloat),
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch5.value.asFloat),
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch6.value.asFloat),
        static_cast<t_double>(table.Reg.Action.PwmDuty.Ch7.value.asFloat)
    };

    const std::vector<pwmMode_t> modes = {
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch0.bit.mode),
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch1.bit.mode),
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch2.bit.mode),
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch3.bit.mode),
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch4.bit.mode),
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch5.bit.mode),
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch6.bit.mode),
        static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch7.bit.mode),
    };

    std::vector<t_uint32> chsInDutyMode;
    std::vector<t_uint32> chsInFreqOrPeriodMode;
    std::vector<t_double> valuesInDutyMode;
    std::vector<t_double> valuesInFreqOrPeriodMode;
    for (int ch = 0; ch < numberOfPwms; ch++)
    {
        if (modes[ch] == pwmModeDutyCycle)
        {
            chsInDutyMode.push_back(ch);
            valuesInDutyMode.push_back(values[ch]);
        }
        else if (isIn(modes[ch], {pwmModeFrequency, pwmModePeriod}))
        {
            chsInFreqOrPeriodMode.push_back(ch);
            valuesInFreqOrPeriodMode.push_back(values[ch]);
        }
        else
        {
            // todo add support to the other modes
        }
    }

    t_uint32 *selectedChannels = chsInDutyMode.data();
    t_uint32 size = static_cast<t_uint32>(chsInDutyMode.size());
    t_double *selectedValues = valuesInDutyMode.data();
    hil_write_pwm(card, selectedChannels, size, selectedValues);

    selectedChannels = chsInFreqOrPeriodMode.data();
    size = static_cast<t_uint32>(chsInFreqOrPeriodMode.size());
    selectedValues = valuesInFreqOrPeriodMode.data();
    hil_set_pwm_duty_cycle(card, selectedChannels, size, selectedValues);
}

void Server::boardSetPwmFrequency()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const int numberOfPwms = boardInfo[boardModel][featurePwmChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_double config[] = {
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch0.value.asFloat),
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch1.value.asFloat),
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch2.value.asFloat),
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch3.value.asFloat),
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch4.value.asFloat),
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch5.value.asFloat),
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch6.value.asFloat),
        static_cast<t_double>(table.Reg.Setup.Pwm.Frequency.Ch7.value.asFloat)
    };

    hil_set_pwm_frequency(card, channels, numberOfPwms, config);
}

void Server::boardSetPwmConfig()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const int numberOfPwms = boardInfo[boardModel][featurePwmChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_pwm_alignment aligns[8] = {
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch0.bit.alignment)),
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch1.bit.alignment)),
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch2.bit.alignment)),
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch3.bit.alignment)),
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch4.bit.alignment)),
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch5.bit.alignment)),
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch6.bit.alignment)),
        decodePwmAlign(static_cast<pwmAlign_t>(table.Reg.Setup.Pwm.Config.Ch7.bit.alignment)),
    };

    const t_pwm_configuration configs[8] = {
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch0.bit.config)),
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch1.bit.config)),
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch2.bit.config)),
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch3.bit.config)),
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch4.bit.config)),
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch5.bit.config)),
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch6.bit.config)),
        decodePwmConfig(static_cast<pwmConfig_t>(table.Reg.Setup.Pwm.Config.Ch7.bit.config)),
    };

    const t_pwm_mode modes[8] = {
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch0.bit.mode)),
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch1.bit.mode)),
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch2.bit.mode)),
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch3.bit.mode)),
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch4.bit.mode)),
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch5.bit.mode)),
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch6.bit.mode)),
        decodePwmMode(static_cast<pwmMode_t>(table.Reg.Setup.Pwm.Config.Ch7.bit.mode)),
    };

    const t_pwm_polarity polarities[8] = {
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch0.bit.polarity)),
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch1.bit.polarity)),
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch2.bit.polarity)),
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch3.bit.polarity)),
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch4.bit.polarity)),
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch5.bit.polarity)),
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch6.bit.polarity)),
        decodePwmPolarity(static_cast<pwmPolarity_t>(table.Reg.Setup.Pwm.Config.Ch7.bit.polarity)),
    };

    hil_set_pwm_mode(card, channels, numberOfPwms, modes);
    hil_set_pwm_configuration(card, channels, numberOfPwms, configs, aligns, polarities);
}

void Server::boardSetPwmDeadTime()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const int numberOfPwms = boardInfo[boardModel][featurePwmChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_double risingEdge[8] = {
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch0) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch1) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch2) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch3) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch4) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch5) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch6) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch7) * 1e-9
    };

    const t_double fallingEdge[8] = {
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch0) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch1) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch2) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch3) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch4) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch5) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch6) * 1e-9,
        static_cast<t_double>(table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch7) * 1e-9
    };

    hil_set_pwm_deadband(card, channels, numberOfPwms, risingEdge, fallingEdge);

}

void Server::boardSetDacRanges()
{
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const uint8_t numberOfDacs = boardInfo[boardModel][featureDacChannels];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    const t_double mins[8] = {
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch0.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch1.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch2.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch3.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch4.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch5.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch6.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Min.Ch7.value.asInteger)
    };

    const t_double maxs[8] = {
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch0.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch1.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch2.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch3.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch4.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch5.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch6.value.asInteger),
        static_cast<t_double>(table.Reg.Setup.Dac.Range.Max.Ch7.value.asInteger)
    };

    hil_set_analog_input_ranges(card, channels, numberOfDacs, mins, maxs);
}

void Server::boardRead()
{
    t_boolean digitalBuffer[8];
    t_int32 encoderBuffer[8];
    t_double analogBuffer[8], otherBuffer[8];
    const t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const t_uint32 boardModel = table.Reg.Setup.BoardModel.value;
    const t_uint32 numberOfAdcs = boardInfo[boardModel][featureAdcChannels];
    const t_uint32 numberOfEncoders = boardInfo[boardModel][featureEncoderChannels];
    const t_uint32 numberOfGpioInputs = 8;

    if (boardModel == boardDummy)
    {
        table.Reg.Readings.Adc.Ch0.value.asFloat = table.Reg.Action.Dac.Ch0.value.asFloat;
        table.Reg.Readings.Adc.Ch1.value.asFloat = table.Reg.Action.Dac.Ch1.value.asFloat;
        table.Reg.Readings.Adc.Ch2.value.asFloat = table.Reg.Action.Dac.Ch2.value.asFloat;
        table.Reg.Readings.Adc.Ch3.value.asFloat = table.Reg.Action.Dac.Ch3.value.asFloat;
        table.Reg.Readings.Adc.Ch4.value.asFloat = table.Reg.Action.Dac.Ch4.value.asFloat;
        table.Reg.Readings.Adc.Ch5.value.asFloat = table.Reg.Action.Dac.Ch5.value.asFloat;
        table.Reg.Readings.Adc.Ch6.value.asFloat = table.Reg.Action.Dac.Ch6.value.asFloat;
        table.Reg.Readings.Adc.Ch7.value.asFloat = table.Reg.Action.Dac.Ch7.value.asFloat;
    }
    else
    {
        t_error error = hil_read(card,
            channels, numberOfAdcs,
            channels, numberOfEncoders,
            channels, numberOfGpioInputs,
            channels, 0,  // other channels
            analogBuffer,
            encoderBuffer,
            digitalBuffer,
            otherBuffer
        );

        // now copy data into the memory table
        table.Reg.Readings.Adc.Ch0.value.asFloat = static_cast<float>(analogBuffer[0]);
        table.Reg.Readings.Adc.Ch1.value.asFloat = static_cast<float>(analogBuffer[1]);
        if (numberOfAdcs > 2)
        {
            table.Reg.Readings.Adc.Ch2.value.asFloat = static_cast<float>(analogBuffer[2]);
            table.Reg.Readings.Adc.Ch3.value.asFloat = static_cast<float>(analogBuffer[3]);
            table.Reg.Readings.Adc.Ch4.value.asFloat = static_cast<float>(analogBuffer[4]);
            table.Reg.Readings.Adc.Ch5.value.asFloat = static_cast<float>(analogBuffer[5]);
            table.Reg.Readings.Adc.Ch6.value.asFloat = static_cast<float>(analogBuffer[6]);
            table.Reg.Readings.Adc.Ch7.value.asFloat = static_cast<float>(analogBuffer[7]);
        }

        table.Reg.Readings.Encoder0.value.asInteger = static_cast<int32_t>(encoderBuffer[0]);
        table.Reg.Readings.Encoder1.value.asInteger = static_cast<int32_t>(encoderBuffer[1]);
        if (numberOfEncoders > 2)
        {
            table.Reg.Readings.Encoder2.value.asInteger = static_cast<int32_t>(encoderBuffer[2]);
            table.Reg.Readings.Encoder3.value.asInteger = static_cast<int32_t>(encoderBuffer[3]);
            table.Reg.Readings.Encoder4.value.asInteger = static_cast<int32_t>(encoderBuffer[4]);
            table.Reg.Readings.Encoder5.value.asInteger = static_cast<int32_t>(encoderBuffer[5]);
            table.Reg.Readings.Encoder6.value.asInteger = static_cast<int32_t>(encoderBuffer[6]);
            table.Reg.Readings.Encoder7.value.asInteger = static_cast<int32_t>(encoderBuffer[7]);
        }

        table.Reg.Readings.GpioInput.value.bit.Ch0 = static_cast<uint8_t>(digitalBuffer[0]);
        table.Reg.Readings.GpioInput.value.bit.Ch1 = static_cast<uint8_t>(digitalBuffer[1]);
        if (numberOfGpioInputs > 2)
        {
            table.Reg.Readings.GpioInput.value.bit.Ch2 = static_cast<uint8_t>(digitalBuffer[2]);
            table.Reg.Readings.GpioInput.value.bit.Ch3 = static_cast<uint8_t>(digitalBuffer[3]);
            table.Reg.Readings.GpioInput.value.bit.Ch4 = static_cast<uint8_t>(digitalBuffer[4]);
            table.Reg.Readings.GpioInput.value.bit.Ch5 = static_cast<uint8_t>(digitalBuffer[5]);
            table.Reg.Readings.GpioInput.value.bit.Ch6 = static_cast<uint8_t>(digitalBuffer[6]);
            table.Reg.Readings.GpioInput.value.bit.Ch7 = static_cast<uint8_t>(digitalBuffer[7]);
        }

        (void) error;
    }

}

void Server::processWriteInstruction(uint16_t addr, int16_t size, const uint8_t* values)
{
    const uint16_t firstAddress = addr;
    uint8_t sizeInBytes;
    t_boolean booleanValues[8];
    t_double doubleValues[8];
    t_uint32 channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const uint8_t boardModel = table.Reg.Setup.BoardModel.value;
    const uint8_t numberOfDacs = boardInfo[boardModel][featureDacChannels];
    const uint8_t numberOfGpioOutputs = 8;

    /*
     * The first parameter is the first register address. So it is
     * necessary to subtract 1 from the total number of parameters.
     */
    int32_t length = size;

    while (length > 0)
    {
        switch(addr) {
            case MemoryTableDescription::Reg::Readings::Encoder0::address:
                sizeInBytes = 8 * sizeof(MemoryTableDescription::Reg::Readings::Encoder0::value);
                memcpy((void*) table.Reg.Readings.Encoder0.value.asBit8, &values[addr - firstAddress], sizeInBytes);
                boardSetEncoderCount();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Action::PwmDuty::Ch0::address:
                sizeInBytes = 8 * sizeof(MemoryTableDescription::Reg::Action::PwmDuty::Ch0::value);
                memcpy((void*) table.Reg.Action.PwmDuty.Ch0.value.asBit8, &values[addr - firstAddress], sizeInBytes);
                boardSetPwmDuty();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Action::Dac::Ch0::address:
                sizeInBytes = 8 * sizeof(MemoryTableDescription::Reg::Action::Dac::Ch0::value);
                memcpy((void*) table.Reg.Action.Dac.Ch0.value.asBit8, &values[addr - firstAddress], sizeInBytes);
                doubleValues[0] = static_cast<double>(table.Reg.Action.Dac.Ch0.value.asFloat);
                doubleValues[1] = static_cast<double>(table.Reg.Action.Dac.Ch1.value.asFloat);
                doubleValues[2] = static_cast<double>(table.Reg.Action.Dac.Ch2.value.asFloat);
                doubleValues[3] = static_cast<double>(table.Reg.Action.Dac.Ch3.value.asFloat);
                doubleValues[4] = static_cast<double>(table.Reg.Action.Dac.Ch4.value.asFloat);
                doubleValues[5] = static_cast<double>(table.Reg.Action.Dac.Ch5.value.asFloat);
                doubleValues[6] = static_cast<double>(table.Reg.Action.Dac.Ch6.value.asFloat);
                doubleValues[7] = static_cast<double>(table.Reg.Action.Dac.Ch7.value.asFloat);
                hil_write_analog(card, channels, numberOfDacs, doubleValues);
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Action::GpioOutput::address:
                sizeInBytes = sizeof(MemoryTableDescription::Reg::Action::GpioOutput::value);
                memcpy(&table.Reg.Action.GpioOutput.value.all, &values[addr - firstAddress], sizeInBytes);
                booleanValues[0] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch0 > 0);
                booleanValues[1] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch1 > 0);
                booleanValues[2] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch2 > 0);
                booleanValues[3] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch3 > 0);
                booleanValues[4] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch4 > 0);
                booleanValues[5] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch5 > 0);
                booleanValues[6] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch6 > 0);
                booleanValues[7] = static_cast<t_boolean>(table.Reg.Action.GpioOutput.value.bit.Ch7 > 0);
                hil_write_digital(card, channels, numberOfGpioOutputs, booleanValues);
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch0::address:
                // 16 because it includes all the maxs and mins of the 8 channels
                sizeInBytes = 16 * sizeof(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch0::value);
                memcpy(&table.Reg.Setup.Adc.Range.Max.Ch0.value.asBit8, &values[addr - firstAddress], sizeInBytes);
                boardSetAdcRanges();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Setup::Adc::Mode::address:
                sizeInBytes = sizeof(MemoryTableDescription::Reg::Setup::Adc::Mode::value);
                memcpy(&table.Reg.Setup.Adc.Mode.value.all, &values[addr - firstAddress], sizeInBytes);
                boardSetAdcModes();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Setup::Encoder::Quadrature::address:
                sizeInBytes = sizeof(MemoryTableDescription::Reg::Setup::Encoder::Quadrature::value::all);
                memcpy(&table.Reg.Setup.Encoder.Quadrature.value.all, &values[addr - firstAddress], sizeInBytes);
                boardSetEncoderQuadrature();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch0::address:
                // 16 because it includes all the maxs and mins of the 8 channels
                sizeInBytes = 16 * sizeof(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch0::value);
                memcpy(&table.Reg.Setup.Dac.Range.Max.Ch0.value.asBit8, &values[addr - firstAddress], sizeInBytes);
                boardSetDacRanges();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch0::address:
                sizeInBytes = 8 * sizeof(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch0::value::asFloat);
                memcpy(&table.Reg.Setup.Pwm.Frequency.Ch0.value.asBit8, &values[addr - firstAddress], sizeInBytes);
                boardSetPwmFrequency();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Setup::Pwm::Config::Ch0::address:
                sizeInBytes = 8 * sizeof(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch0::all);
                memcpy(&table.Reg.Setup.Pwm.Config.Ch0.all, &values[addr - firstAddress], sizeInBytes);
                boardSetPwmConfig();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            case MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address:
                // there are 16 vales there to be updated (8x falling and 8x rising)
                sizeInBytes = 16 * sizeof(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::Ch0);
                memcpy(&table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch0, &values[addr - firstAddress], sizeInBytes);
                boardSetPwmDeadTime();
                length = length - sizeInBytes;
                addr = addr + sizeInBytes;
                break;

            default:
                // If arrived here, then the loop will repeat and it is required
                // to update the addr and length
                length = length - 1;
                addr = addr + 1;
                break;
        }
    }
}

void Server::handleWriteInstruction()
{
    uint8_t* parameters = commMsgGetAddrOfParameters(&instructionPacket);
    uint16_t addr = parameters[0];

    /*
     * The first parameter is the first register address. So it is
     * necessary to subtract 1 from the total number of parameters.
     */
    int16_t length = commMsgGetNumberOfParameters(&instructionPacket);
    length = length - 1;

    processWriteInstruction(addr, length, &parameters[1]);
}


void Server::handleMultWriteInstruction()
{
    const uint8_t total_length = commMsgGetField(&instructionPacket, commMsgFieldLength) - 1;
    uint8_t* parameters = commMsgGetAddrOfParameters(&instructionPacket);
    uint8_t i = 0;
    uint8_t size;
    uint16_t addr;
    uint8_t* values;

    do
    {
        addr = parameters[i];
        size = parameters[i + 1];
        values = &parameters[i + 2];
        processWriteInstruction(addr, size, values);
        i = i + size + 2;

    } while (i < total_length);
}

void Server::handleReadInstruction()
{
    uint8_t i;
    uint8_t first_reg = commMsgGetParameter(&instructionPacket, 0);
    uint8_t length = commMsgGetParameter(&instructionPacket, 1);

    // update the measurements
    boardRead();

    commMsgCreateEmptyMsg(&statusPacket, commMsgTypeAnsRead);
    commMsgAppendParameter(&statusPacket, 0);          // insert error
    commMsgAppendParameter(&statusPacket, first_reg);  // insert the first register read
    commMsgAppendParameter(&statusPacket, length);     // insert how many registers were read

    if (length > commMsgMaxLength)
    {
        length = commMsgMaxLength;
        commMsgInsertParameter(&statusPacket, 0, commMsgErrorInvalidRange);
    }

    for (i = 0; i < length; i++)
    {
        commMsgAppendParameter(&statusPacket, table.memory[first_reg + i]);
    }

    commMsgUpdateChecksumValue(&statusPacket);
}

void Server::makeAutoReadMsg()
{
    uint8_t i;
    const uint8_t first_reg = table.Reg.Setup.AutoRead.FirstReg.value;
    const uint8_t length = table.Reg.Setup.AutoRead.Length.value;

    commMsgCreateEmptyMsg(&autoReadStatusPacket, commMsgTypeAnsRead);
    commMsgAppendParameter(&autoReadStatusPacket, 0);          // insert error
    commMsgAppendParameter(&autoReadStatusPacket, first_reg);  // insert the first register read
    commMsgAppendParameter(&autoReadStatusPacket, length);     // insert how many registers were read

    for (i = 0; i < length; i++)
    {
        commMsgAppendParameter(&autoReadStatusPacket, table.memory[first_reg + i]);
    }

    commMsgUpdateChecksumValue(&autoReadStatusPacket);
}

bool Server::processPacket()
{
    bool generatedStatusPacket = false;

    switch (commMsgGetField(&instructionPacket, commMsgFieldType)) {

        case static_cast<uint8_t>(commMsgTypeRead):
            handleReadInstruction();
            generatedStatusPacket = true;
            break;

        case static_cast<uint8_t>(commMsgTypeWrite):
            handleWriteInstruction();
            break;

        case static_cast<uint8_t>(commMsgTypeMultiWrite):
            handleMultWriteInstruction();
            break;

        default:
            break;
    }

    return generatedStatusPacket;
}

void Server::callbackFinishedReceiving(const std::error_code& ec, size_t bytesTransferred)
{
    // auxiliary variables
    size_t i;

    if ((ec == asio::error::eof) || (ec == asio::error::connection_reset))
    {
        socket->cancel();
        acceptor->cancel();
        socket->close(); 
        acceptor->close();
        delete socket;
        delete acceptor;
        communicationStart();
        std::cout << "The client has disconnected" << std::endl;
    }
    else // i.e., received data
    {
        // process the received bytes
        for (i = 0; i < bytesTransferred; i++)
        {
            commReceiverProcess(&receiver, rxBuffer[i]);

            if (commReceiverHasNewMessage(&receiver))
            {
                bool generatedStatusPacket = processPacket();
                if (generatedStatusPacket)
                {
                    communicationAsyncSend(
                        commMsgGetAddrOfPacket(&statusPacket),
                        commMsgGetTotalPacketSize(&statusPacket)
                    );
                }

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
    if (hasClientConnected)
        communicationAsyncReceive();
}

void Server::callbackFinishedSending(const std::error_code& ec, size_t bytesTransferred)
{
    (void) ec;
    (void) bytesTransferred;

    // because all bytes were transferred, we release the waiting
    if (waitingTx) waitingTx = false;
}

void Server::callbackFinishedAccepting(const std::error_code& ec)
{
    (void) ec;

    hasClientConnected = true;
    communicationAsyncReceive();
    std::cout << "(INFO) A client has connected" << std::endl;
}

void Server::communicationStart()
{
    hasClientConnected = false;
    socket = new asio::ip::tcp::socket(ioContext);
    acceptor = new asio::ip::tcp::acceptor(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

    acceptor->async_accept(
        *socket,
        std::bind(
            &Server::callbackFinishedAccepting,
            this,
            std::placeholders::_1  // asio::placeholders::error_code
        )
    );
}

void Server::communicationAsyncSend(uint8_t* data, uint16_t size)
{
    asio::async_write(
        *socket,
        asio::buffer(data, size),
        std::bind(
            &Server::callbackFinishedSending,
            this,
            std::placeholders::_1,  // asio::placeholders::error
            std::placeholders::_2   // asio::placeholders::bytes_transferred
        )
    );
}

void Server::communicationAsyncReceive()
{
    socket->async_read_some(
        asio::buffer(rxBuffer, 256),
        std::bind(
            &Server::callbackFinishedReceiving,
            this,
            std::placeholders::_1,  // asio::placeholders::error,
            std::placeholders::_2   // asio::placeholders::bytes_transferred
        )
    );
}
