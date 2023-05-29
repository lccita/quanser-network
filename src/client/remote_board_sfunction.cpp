#include <iostream>
#include <cstring>
#include <cmath>

// S-Function information
#define S_FUNCTION_NAME  quark_sfunction
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

// Aplication libraries
#include "remote_board.hpp"

typedef enum {
    inPortGpioOutCh0 = 0,
    inPortGpioOutCh1,
    inPortGpioOutCh2,
    inPortGpioOutCh3,
    inPortGpioOutCh4,
    inPortGpioOutCh5,
    inPortGpioOutCh6,
    inPortGpioOutCh7,
    inPortPwmCh0,
    inPortPwmCh1,
    inPortPwmCh2,
    inPortPwmCh3,
    inPortPwmCh4,
    inPortPwmCh5,
    inPortPwmCh6,
    inPortPwmCh7,
    inPortDacCh0,
    inPortDacCh1,
    inPortDacCh2,
    inPortDacCh3,
    inPortDacCh4,
    inPortDacCh5,
    inPortDacCh6,
    inPortDacCh7,
    numberOfInputPorts
} inputPort;

typedef enum {
    outPortGpioInCh0 = 0,
    outPortGpioInCh1,
    outPortGpioInCh2,
    outPortGpioInCh3,
    outPortGpioInCh4,
    outPortGpioInCh5,
    outPortGpioInCh6,
    outPortGpioInCh7,
    outPortAdCh0,
    outPortAdCh1,
    outPortAdCh2,
    outPortAdCh3,
    outPortAdCh4,
    outPortAdCh5,
    outPortAdCh6,
    outPortAdCh7,
    outPortEnc0,
    outPortEnc1,
    outPortEnc2,
    outPortEnc3,
    outPortEnc4,
    outPortEnc5,
    outPortEnc6,
    outPortEnc7,
    outPortRealTime,
    numberOfOutputPorts
} outputPort;

typedef enum {
    positionServerIp = 0,
    positionServerPort,
    positionSampleTime,
    positionAutoreadInterval,
    positionAdcModeCh0,
    positionAdcRangeMinCh0,
    positionAdcRangeMaxCh0,
    positionAdcModeCh1,
    positionAdcRangeMinCh1,
    positionAdcRangeMaxCh1,
    positionAdcModeCh2,
    positionAdcRangeMinCh2,
    positionAdcRangeMaxCh2,
    positionAdcModeCh3,
    positionAdcRangeMinCh3,
    positionAdcRangeMaxCh3,
    positionAdcModeCh4,
    positionAdcRangeMinCh4,
    positionAdcRangeMaxCh4,
    positionAdcModeCh5,
    positionAdcRangeMinCh5,
    positionAdcRangeMaxCh5,
    positionAdcModeCh6,
    positionAdcRangeMinCh6,
    positionAdcRangeMaxCh6,
    positionAdcModeCh7,
    positionAdcRangeMinCh7,
    positionAdcRangeMaxCh7,
    positionPwmAlignmentCh0,
    positionPwmConfigCh0,
    positionPwmModeCh0,
    positionPwmPolarityCh0,
    positionPwmFrequencyCh0,
    positionPwmDeadtimeRisingCh0,
    positionPwmDeadtimeFallingCh0,
    positionPwmFinalValueCh0,
    positionPwmAlignmentCh1,
    positionPwmConfigCh1,
    positionPwmModeCh1,
    positionPwmPolarityCh1,
    positionPwmFrequencyCh1,
    positionPwmDeadtimeRisingCh1,
    positionPwmDeadtimeFallingCh1,
    positionPwmFinalValueCh1,
    positionPwmAlignmentCh2,
    positionPwmConfigCh2,
    positionPwmModeCh2,
    positionPwmPolarityCh2,
    positionPwmFrequencyCh2,
    positionPwmDeadtimeRisingCh2,
    positionPwmDeadtimeFallingCh2,
    positionPwmFinalValueCh2,
    positionPwmAlignmentCh3,
    positionPwmConfigCh3,
    positionPwmModeCh3,
    positionPwmPolarityCh3,
    positionPwmFrequencyCh3,
    positionPwmDeadtimeRisingCh3,
    positionPwmDeadtimeFallingCh3,
    positionPwmFinalValueCh3,
    positionPwmAlignmentCh4,
    positionPwmConfigCh4,
    positionPwmModeCh4,
    positionPwmPolarityCh4,
    positionPwmFrequencyCh4,
    positionPwmDeadtimeRisingCh4,
    positionPwmDeadtimeFallingCh4,
    positionPwmFinalValueCh4,
    positionPwmAlignmentCh5,
    positionPwmConfigCh5,
    positionPwmModeCh5,
    positionPwmPolarityCh5,
    positionPwmFrequencyCh5,
    positionPwmDeadtimeRisingCh5,
    positionPwmDeadtimeFallingCh5,
    positionPwmFinalValueCh5,
    positionPwmAlignmentCh6,
    positionPwmConfigCh6,
    positionPwmModeCh6,
    positionPwmPolarityCh6,
    positionPwmFrequencyCh6,
    positionPwmDeadtimeRisingCh6,
    positionPwmDeadtimeFallingCh6,
    positionPwmFinalValueCh6,
    positionPwmAlignmentCh7,
    positionPwmConfigCh7,
    positionPwmModeCh7,
    positionPwmPolarityCh7,
    positionPwmFrequencyCh7,
    positionPwmDeadtimeRisingCh7,
    positionPwmDeadtimeFallingCh7,
    positionPwmFinalValueCh7,
    positionEnc0Quadrature,
    positionEnc0ResetCount,
    positionEnc1Quadrature,
    positionEnc1ResetCount,
    positionEnc2Quadrature,
    positionEnc2ResetCount,
    positionEnc3Quadrature,
    positionEnc3ResetCount,
    positionEnc4Quadrature,
    positionEnc4ResetCount,
    positionEnc5Quadrature,
    positionEnc5ResetCount,
    positionEnc6Quadrature,
    positionEnc6ResetCount,
    positionEnc7Quadrature,
    positionEnc7ResetCount,
    positionDacRangeMinCh0,
    positionDacRangeMaxCh0,
    positionDacRangeMinCh1,
    positionDacRangeMaxCh1,
    positionDacRangeMinCh2,
    positionDacRangeMaxCh2,
    positionDacRangeMinCh3,
    positionDacRangeMaxCh3,
    positionDacRangeMinCh4,
    positionDacRangeMaxCh4,
    positionDacRangeMinCh5,
    positionDacRangeMaxCh5,
    positionDacRangeMinCh6,
    positionDacRangeMaxCh6,
    positionDacRangeMinCh7,
    positionDacRangeMaxCh7,
    numberOfParameters
} parameterPosition;

static int waitingForData = 1;
static void newDataArrived()
{
    waitingForData = 0;
}

/*
 * The sizes information is used by Simulink to determine the S-function
 * block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    // Auxiliary variables
    uint32_t i;

    // Set the expected number parameters
    ssSetNumSFcnParams(S, parameterPosition::numberOfParameters);

    // Verify parameter count
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch will be reported by Simulink */
    }

    // Any parameter is not tunable because they will be used to configure the board
    for (i = 0; i < parameterPosition::numberOfParameters; i++)
        ssSetSFcnParamTunable(S, i, SS_PRM_NOT_TUNABLE);

    // Set the number of continuous and discrete states of the block
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 1);

    // Set the input ports and remove direct feed through
    if (!ssSetNumInputPorts(S, inputPort::numberOfInputPorts)) return;
    for (i = 0; i < inputPort::numberOfInputPorts; i++)
    {
        ssSetInputPortWidth(S, i, 1);
        ssSetInputPortDirectFeedThrough(S, i, 0);
    }

    // All the output ports have one dimension
    if (!ssSetNumOutputPorts(S, outputPort::numberOfOutputPorts)) return;
    for (i = 0; i < outputPort::numberOfOutputPorts; i++)
        ssSetOutputPortWidth(S, i, 1);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1);  // remote_board object
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(
        S, 
        SS_OPTION_EXCEPTION_FREE_CODE       // it doesn't use routines which can throw exceptions
        | SS_OPTION_DISCRETE_VALUED_OUTPUT
        | SS_OPTION_PLACE_ASAP              // typically used by devices connecting to hardware
    );
}

#define MDL_INITIALIZE_CONDITIONS
static void mdlInitializeConditions(SimStruct* S)
{
    real_T* x0 = ssGetRealDiscStates(S);

    for (int i = 0; i < ssGetNumDiscStates(S); i++)
        x0[i] = 0.0;
}

/*
 * This function is used to specify the sample time(s) for your
 * S-function. You must register the same number of sample times as
 * specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    const double sampleTimeSecs = mxGetScalar(ssGetSFcnParam(S, positionSampleTime)) / 1000;
    ssSetSampleTime(S, 0, sampleTimeSecs);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

// Change to #undef to remove function
#define MDL_START
static void mdlStart(SimStruct *S)
{
    // Reserve memory to the board
    std::string serverIp = mxArrayToString(ssGetSFcnParam(S, positionServerIp));
    uint16_t serverPort = static_cast<uint16_t>(mxGetScalar(ssGetSFcnParam(S, positionServerPort)));
    auto board = new RemoteBoard(serverIp, serverPort);
    ssGetPWork(S)[0] = reinterpret_cast<void*>(board);

    // todo Verify if the board was open and return error if not
    if (board->isConnected())
    {
        // Apply ADC configuration
        adcMode_t adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh0)));
        double minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh0));
        double maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh0));
        board->configAdc(Ch0, adcModeValue, minRange, maxRange);

        adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh1)));
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh1));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh1));
        board->configAdc(Ch1, adcModeValue, minRange, maxRange);

        adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh2)));
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh2));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh2));
        board->configAdc(Ch2, adcModeValue, minRange, maxRange);

        adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh3)));
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh3));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh3));
        board->configAdc(Ch3, adcModeValue, minRange, maxRange);

        adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh4)));
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh4));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh4));
        board->configAdc(Ch4, adcModeValue, minRange, maxRange);

        adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh5)));
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh5));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh5));
        board->configAdc(Ch5, adcModeValue, minRange, maxRange);

        adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh6)));
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh6));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh6));
        board->configAdc(Ch6, adcModeValue, minRange, maxRange);

        adcModeValue = static_cast<adcMode_t>(mxGetScalar(ssGetSFcnParam(S, positionAdcModeCh7)));
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh7));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh7));
        board->configAdc(Ch7, adcModeValue, minRange, maxRange);

        // Apply PWM configuration
        double frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh0));
        pwmAlign_t pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh0)));
        pwmConfig_t pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh0)));
        pwmMode_t pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh0)));
        pwmPolarity_t pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh0)));
        double risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh0));
        double fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh0));
        board->pwmSetFrequency(Ch0, frequency);
        board->configPwm(Ch0, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch0, risingEdgeTime, fallingEdgeTime);

        frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh1));
        pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh1)));
        pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh1)));
        pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh1)));
        pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh1)));
        risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh1));
        fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh1));
        board->pwmSetFrequency(Ch1, frequency);
        board->configPwm(Ch1, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch1, risingEdgeTime, fallingEdgeTime);

        frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh2));
        pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh2)));
        pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh2)));
        pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh2)));
        pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh2)));
        risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh2));
        fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh2));
        board->pwmSetFrequency(Ch2, frequency);
        board->configPwm(Ch2, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch2, risingEdgeTime, fallingEdgeTime);

        frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh3));
        pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh3)));
        pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh3)));
        pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh3)));
        pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh3)));
        risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh3));
        fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh3));
        board->pwmSetFrequency(Ch3, frequency);
        board->configPwm(Ch3, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch3, risingEdgeTime, fallingEdgeTime);

        frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh4));
        pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh4)));
        pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh4)));
        pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh4)));
        pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh4)));
        risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh4));
        fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh4));
        board->pwmSetFrequency(Ch4, frequency);
        board->configPwm(Ch4, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch4, risingEdgeTime, fallingEdgeTime);

        frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh5));
        pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh5)));
        pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh5)));
        pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh5)));
        pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh5)));
        risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh5));
        fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh5));
        board->pwmSetFrequency(Ch5, frequency);
        board->configPwm(Ch5, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch5, risingEdgeTime, fallingEdgeTime);

        frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh6));
        pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh6)));
        pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh6)));
        pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh6)));
        pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh6)));
        risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh6));
        fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh6));
        board->pwmSetFrequency(Ch6, frequency);
        board->configPwm(Ch6, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch6, risingEdgeTime, fallingEdgeTime);

        frequency = mxGetScalar(ssGetSFcnParam(S, positionPwmFrequencyCh7));
        pwmAlignValue = static_cast<pwmAlign_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmAlignmentCh7)));
        pwmConfigValue = static_cast<pwmConfig_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmConfigCh7)));
        pwmModeValue = static_cast<pwmMode_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmModeCh7)));
        pwmPolarityValue = static_cast<pwmPolarity_t>(mxGetScalar(ssGetSFcnParam(S, positionPwmPolarityCh7)));
        risingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeRisingCh7));
        fallingEdgeTime = mxGetScalar(ssGetSFcnParam(S, positionPwmDeadtimeFallingCh7));
        board->pwmSetFrequency(Ch7, frequency);
        board->configPwm(Ch7, pwmAlignValue, pwmConfigValue, pwmModeValue, pwmPolarityValue);
        board->setPwmDeadTime(Ch7, risingEdgeTime, fallingEdgeTime);

        // Apply DAC configuration
        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh0));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh0));
        board->configDac(Ch0, minRange, maxRange);

        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh1));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh1));
        board->configDac(Ch1, minRange, maxRange);

        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh2));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh2));
        board->configDac(Ch2, minRange, maxRange);

        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh3));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh3));
        board->configDac(Ch3, minRange, maxRange);

        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh4));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh4));
        board->configDac(Ch4, minRange, maxRange);

        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh5));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh5));
        board->configDac(Ch5, minRange, maxRange);

        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh6));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh6));
        board->configDac(Ch6, minRange, maxRange);

        minRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMinCh7));
        maxRange = mxGetScalar(ssGetSFcnParam(S, positionAdcRangeMaxCh7));
        board->configDac(Ch7, minRange, maxRange);

        // Apply the encoder configuration
        auto quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc0Quadrature)));
        board->encoderSetQuadrature(Ch0, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc0ResetCount)) > 0)
        {
            board->encoderSetCount(Ch0, 0);
        }

        quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc1Quadrature)));
        board->encoderSetQuadrature(Ch1, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc1ResetCount)) > 0)
        {
            board->encoderSetCount(Ch1, 0);
        }

        quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc2Quadrature)));
        board->encoderSetQuadrature(Ch2, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc2ResetCount)) > 0)
        {
            board->encoderSetCount(Ch2, 0);
        }

        quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc3Quadrature)));
        board->encoderSetQuadrature(Ch3, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc3ResetCount)) > 0)
        {
            board->encoderSetCount(Ch3, 0);
        }

        quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc4Quadrature)));
        board->encoderSetQuadrature(Ch4, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc4ResetCount)) > 0)
        {
            board->encoderSetCount(Ch4, 0);
        }

        quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc5Quadrature)));
        board->encoderSetQuadrature(Ch5, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc5ResetCount)) > 0)
        {
            board->encoderSetCount(Ch5, 0);
        }

        quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc6Quadrature)));
        board->encoderSetQuadrature(Ch6, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc6ResetCount)) > 0)
        {
            board->encoderSetCount(Ch6, 0);
        }

        quadrature = static_cast<encoderQuadrature_t>(mxGetScalar(ssGetSFcnParam(S, positionEnc7Quadrature)));
        board->encoderSetQuadrature(Ch7, quadrature);
        if (mxGetScalar(ssGetSFcnParam(S, positionEnc7ResetCount)) > 0)
        {
            board->encoderSetCount(Ch7, 0);
        }

        board->sync();
    }

    // Enable the auto-read
    const double interval = mxGetScalar(ssGetSFcnParam(S, positionAutoreadInterval));
    board->enableAutoRead(interval, newDataArrived);
    board->sync();
}

#define MDL_UPDATE
static void mdlUpdate(SimStruct* S, int_T tid)
{
    // Auxiliary variables
    UNUSED_ARG(tid);
    InputRealPtrsType uPtrs;
    real_T* blockState = ssGetRealDiscStates(S);

    // Load the board configuration and board driver
    auto board = reinterpret_cast<RemoteBoard*>(ssGetPWork(S)[0]);

    // Handle the gpio output
    // CH0
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh0);
    board->digitalWrite(CH0, static_cast<int>(*uPtrs[0] > 0));

    // CH1
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh1);
    board->digitalWrite(CH1, static_cast<int>(*uPtrs[0] > 0));

    // CH2
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh2);
    board->digitalWrite(CH2, static_cast<int>(*uPtrs[0] > 0));

    // CH3
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh3);
    board->digitalWrite(CH3, static_cast<int>(*uPtrs[0] > 0));

    // CH4
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh4);
    board->digitalWrite(CH4, static_cast<int>(*uPtrs[0] > 0));

    // CH5
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh5);
    board->digitalWrite(CH5, static_cast<int>(*uPtrs[0] > 0));

    // CH6
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh6);
    board->digitalWrite(CH6, static_cast<int>(*uPtrs[0] > 0));

    // CH7
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortGpioOutCh7);
    board->digitalWrite(CH7, static_cast<int>(*uPtrs[0] > 0));

    // Handle the pwm
    // CH0
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh0);
    board->pwmSetDuty(CH0, static_cast<double>(*uPtrs[0]));

    // CH1
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh1);
    board->pwmSetDuty(CH1, static_cast<double>(*uPtrs[0]));

    // CH2
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh2);
    board->pwmSetDuty(CH2, static_cast<double>(*uPtrs[0]));

    // CH3
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh3);
    board->pwmSetDuty(CH3, static_cast<double>(*uPtrs[0]));

    // CH4
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh4);
    board->pwmSetDuty(CH4, static_cast<double>(*uPtrs[0]));

    // CH5
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh5);
    board->pwmSetDuty(CH5, static_cast<double>(*uPtrs[0]));

    // CH6
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh6);
    board->pwmSetDuty(CH6, static_cast<double>(*uPtrs[0]));

    // CH7
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortPwmCh7);
    board->pwmSetDuty(CH7, static_cast<double>(*uPtrs[0]));

    // Handle the DAC
    // CH0
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh0);
    board->pwmSetDuty(CH0, static_cast<double>(*uPtrs[0]));

    // CH1
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh1);
    board->pwmSetDuty(CH1, static_cast<double>(*uPtrs[0]));

    // CH2
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh2);
    board->pwmSetDuty(CH2, static_cast<double>(*uPtrs[0]));

    // CH3
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh3);
    board->pwmSetDuty(CH3, static_cast<double>(*uPtrs[0]));

    // CH4
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh4);
    board->pwmSetDuty(CH4, static_cast<double>(*uPtrs[0]));

    // CH5
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh5);
    board->pwmSetDuty(CH5, static_cast<double>(*uPtrs[0]));

    // CH6
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh6);
    board->pwmSetDuty(CH6, static_cast<double>(*uPtrs[0]));

    // CH7
    uPtrs = ssGetInputPortRealSignalPtrs(S, inPortDacCh7);
    board->pwmSetDuty(CH7, static_cast<double>(*uPtrs[0]));

    // Sync the board
    board->sync();

    // Increase mdlUpdate count (used to force simulink to run this function)
    blockState[0] = blockState[0] + 1;
}

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    // Auxiliary variables
    UNUSED_ARG(tid);
    real_T* yGpioInCh0 = ssGetOutputPortRealSignal(S, outPortGpioInCh0);
    real_T* yGpioInCh1 = ssGetOutputPortRealSignal(S, outPortGpioInCh1);
    real_T* yGpioInCh2 = ssGetOutputPortRealSignal(S, outPortGpioInCh2);
    real_T* yGpioInCh3 = ssGetOutputPortRealSignal(S, outPortGpioInCh3);
    real_T* yGpioInCh4 = ssGetOutputPortRealSignal(S, outPortGpioInCh4);
    real_T* yGpioInCh5 = ssGetOutputPortRealSignal(S, outPortGpioInCh5);
    real_T* yGpioInCh6 = ssGetOutputPortRealSignal(S, outPortGpioInCh6);
    real_T* yGpioInCh7 = ssGetOutputPortRealSignal(S, outPortGpioInCh7);
    real_T* yAdCh0 = ssGetOutputPortRealSignal(S, outPortAdCh0);
    real_T* yAdCh1 = ssGetOutputPortRealSignal(S, outPortAdCh1);
    real_T* yAdCh2 = ssGetOutputPortRealSignal(S, outPortAdCh2);
    real_T* yAdCh3 = ssGetOutputPortRealSignal(S, outPortAdCh3);
    real_T* yAdCh4 = ssGetOutputPortRealSignal(S, outPortAdCh4);
    real_T* yAdCh5 = ssGetOutputPortRealSignal(S, outPortAdCh5);
    real_T* yAdCh6 = ssGetOutputPortRealSignal(S, outPortAdCh6);
    real_T* yAdCh7 = ssGetOutputPortRealSignal(S, outPortAdCh7);
    real_T* yEnc0 = ssGetOutputPortRealSignal(S, outPortEnc0);
    real_T* yEnc1 = ssGetOutputPortRealSignal(S, outPortEnc1);
    real_T* yEnc2 = ssGetOutputPortRealSignal(S, outPortEnc2);
    real_T* yEnc3 = ssGetOutputPortRealSignal(S, outPortEnc3);
    real_T* yEnc4 = ssGetOutputPortRealSignal(S, outPortEnc4);
    real_T* yEnc5 = ssGetOutputPortRealSignal(S, outPortEnc5);
    real_T* yEnc6 = ssGetOutputPortRealSignal(S, outPortEnc6);
    real_T* yEnc7 = ssGetOutputPortRealSignal(S, outPortEnc7);
    real_T* yRealTime = ssGetOutputPortRealSignal(S, outPortRealTime);

    // Load the board configuration and board driver
    auto board = reinterpret_cast<RemoteBoard*>(ssGetPWork(S)[0]);

    // Wait for new data to arrive
    while (waitingForData > 0) {}
    waitingForData = 1;

    // Handle digital input
    *yGpioInCh0 = static_cast<real_T>(board->digitalRead(CH0));
    *yGpioInCh1 = static_cast<real_T>(board->digitalRead(CH1));
    *yGpioInCh2 = static_cast<real_T>(board->digitalRead(CH2));
    *yGpioInCh3 = static_cast<real_T>(board->digitalRead(CH3));
    *yGpioInCh4 = static_cast<real_T>(board->digitalRead(CH4));
    *yGpioInCh5 = static_cast<real_T>(board->digitalRead(CH5));
    *yGpioInCh6 = static_cast<real_T>(board->digitalRead(CH6));
    *yGpioInCh7 = static_cast<real_T>(board->digitalRead(CH7));

    // Handle encoders
    *yEnc0 = static_cast<real_T>(board->encoderRead(CH0));
    *yEnc1 = static_cast<real_T>(board->encoderRead(CH1));
    *yEnc2 = static_cast<real_T>(board->encoderRead(CH2));
    *yEnc3 = static_cast<real_T>(board->encoderRead(CH3));
    *yEnc4 = static_cast<real_T>(board->encoderRead(CH4));
    *yEnc5 = static_cast<real_T>(board->encoderRead(CH5));
    *yEnc6 = static_cast<real_T>(board->encoderRead(CH6));
    *yEnc7 = static_cast<real_T>(board->encoderRead(CH7));

    // Handle A/D
    *yAdCh0 = static_cast<real_T>(board->analogRead(CH0));
    *yAdCh1 = static_cast<real_T>(board->analogRead(CH1));
    *yAdCh2 = static_cast<real_T>(board->analogRead(CH2));
    *yAdCh3 = static_cast<real_T>(board->analogRead(CH3));
    *yAdCh4 = static_cast<real_T>(board->analogRead(CH4));
    *yAdCh5 = static_cast<real_T>(board->analogRead(CH5));
    *yAdCh6 = static_cast<real_T>(board->analogRead(CH6));
    *yAdCh7 = static_cast<real_T>(board->analogRead(CH7));

    // Put this time instant in output port
    *yRealTime = 1e-6 * static_cast<double>(getTimeUs());
}

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
    auto board = reinterpret_cast<RemoteBoard*>(ssGetPWork(S)[0]);

    // Apply the final duty cycle
    board->pwmSetDuty(CH0, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh0)));
    board->pwmSetDuty(CH1, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh1)));
    board->pwmSetDuty(CH2, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh2)));
    board->pwmSetDuty(CH3, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh3)));
    board->pwmSetDuty(CH4, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh4)));
    board->pwmSetDuty(CH5, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh5)));
    board->pwmSetDuty(CH6, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh6)));
    board->pwmSetDuty(CH7, mxGetScalar(ssGetSFcnParam(S, positionPwmFinalValueCh7)));
    board->disableAutoRead();
    board->sync();
    delayMs(1);

    board->close();
    delete board;
}

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

