#include <iostream>
#include "utils/memory_table.hpp"

MemoryTable table;

void test_address(uint8_t address, const void* memory_address)
{
    void* tableFirstAddr = &table;
    uint64_t number = reinterpret_cast<uint64_t>(memory_address) - reinterpret_cast<uint64_t>(tableFirstAddr);

    if (&table.memory[address] != memory_address)
    {
        std::cout << "address " << int(address) << " has failed (it is in " << number << ")" << std::endl;
    }
}

int main (void)
{
    std::cout << "(INFO) Application to test the addresses of the Memory Table" << std::endl;

    // Readings section
    test_address(MemoryTableDescription::Reg::Readings::GpioInput::address,                   (void*) &table.Reg.Readings.GpioInput.value.all);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch0::address,                    (void*) &table.Reg.Readings.Adc.Ch0.value);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch1::address,                    (void*) &table.Reg.Readings.Adc.Ch1.value);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch2::address,                    (void*) &table.Reg.Readings.Adc.Ch2.value);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch3::address,                    (void*) &table.Reg.Readings.Adc.Ch3.value);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch4::address,                    (void*) &table.Reg.Readings.Adc.Ch4.value);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch5::address,                    (void*) &table.Reg.Readings.Adc.Ch5.value);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch6::address,                    (void*) &table.Reg.Readings.Adc.Ch6.value);
    test_address(MemoryTableDescription::Reg::Readings::Adc::Ch7::address,                    (void*) &table.Reg.Readings.Adc.Ch7.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder0::address,                    (void*) &table.Reg.Readings.Encoder0.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder1::address,                    (void*) &table.Reg.Readings.Encoder1.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder2::address,                    (void*) &table.Reg.Readings.Encoder2.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder3::address,                    (void*) &table.Reg.Readings.Encoder3.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder4::address,                    (void*) &table.Reg.Readings.Encoder4.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder5::address,                    (void*) &table.Reg.Readings.Encoder5.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder6::address,                    (void*) &table.Reg.Readings.Encoder6.value);
    test_address(MemoryTableDescription::Reg::Readings::Encoder7::address,                    (void*) &table.Reg.Readings.Encoder7.value);

    // Action section
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch0::address,                  (void*) &table.Reg.Action.PwmDuty.Ch0.value);
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch1::address,                  (void*) &table.Reg.Action.PwmDuty.Ch1.value);
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch2::address,                  (void*) &table.Reg.Action.PwmDuty.Ch2.value);
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch3::address,                  (void*) &table.Reg.Action.PwmDuty.Ch3.value);
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch4::address,                  (void*) &table.Reg.Action.PwmDuty.Ch4.value);
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch5::address,                  (void*) &table.Reg.Action.PwmDuty.Ch5.value);
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch6::address,                  (void*) &table.Reg.Action.PwmDuty.Ch6.value);
    test_address(MemoryTableDescription::Reg::Action::PwmDuty::Ch7::address,                  (void*) &table.Reg.Action.PwmDuty.Ch7.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch0::address,                      (void*) &table.Reg.Action.Dac.Ch0.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch1::address,                      (void*) &table.Reg.Action.Dac.Ch1.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch2::address,                      (void*) &table.Reg.Action.Dac.Ch2.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch3::address,                      (void*) &table.Reg.Action.Dac.Ch3.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch4::address,                      (void*) &table.Reg.Action.Dac.Ch4.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch5::address,                      (void*) &table.Reg.Action.Dac.Ch5.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch6::address,                      (void*) &table.Reg.Action.Dac.Ch6.value);
    test_address(MemoryTableDescription::Reg::Action::Dac::Ch7::address,                      (void*) &table.Reg.Action.Dac.Ch7.value);
    test_address(MemoryTableDescription::Reg::Action::GpioOutput::address,                    (void*) &table.Reg.Action.GpioOutput.value);

    // Setup section
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch0::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch0.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch1::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch1.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch2::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch2.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch3::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch3.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch4::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch4.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch5::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch5.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch6::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch6.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Max::Ch7::address,           (void*) &table.Reg.Setup.Adc.Range.Max.Ch7.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch0::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch0.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch1::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch1.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch2::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch2.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch3::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch3.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch4::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch4.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch5::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch5.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch6::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch6.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Range::Min::Ch7::address,           (void*) &table.Reg.Setup.Adc.Range.Min.Ch7.value);
    test_address(MemoryTableDescription::Reg::Setup::Adc::Mode::address,                      (void*) &table.Reg.Setup.Adc.Mode.value);
    test_address(MemoryTableDescription::Reg::Setup::Encoder::Quadrature::address,            (void*) &table.Reg.Setup.Encoder.Quadrature.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch0::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch0.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch1::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch1.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch2::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch2.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch3::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch3.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch4::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch4.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch5::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch5.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch6::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch6.value);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Frequency::Ch7::address,            (void*) &table.Reg.Setup.Pwm.Frequency.Ch7.value);

    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch0::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch0.all);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch1::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch1.all);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch2::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch2.all);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch3::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch3.all);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch4::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch4.all);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch5::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch5.all);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch6::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch6.all);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::Config::Ch7::address,               (void*) &table.Reg.Setup.Pwm.Config.Ch7.all);

    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address,     (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch0);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address+2,   (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch1);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address+4,   (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch2);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address+6,   (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch3);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address+8,   (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch4);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address+10,  (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch5);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address+12,  (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch6);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::FallingEdge::address+14,  (void*) &table.Reg.Setup.Pwm.DeadTime.FallingEdge.Ch7);

    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address,     (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch0);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address+2,   (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch1);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address+4,   (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch2);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address+6,   (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch3);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address+8,   (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch4);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address+10,  (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch5);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address+12,  (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch6);
    test_address(MemoryTableDescription::Reg::Setup::Pwm::DeadTime::RisingEdge::address+14,  (void*) &table.Reg.Setup.Pwm.DeadTime.RisingEdge.Ch7);

    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch0::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch0.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch1::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch1.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch2::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch2.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch3::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch3.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch4::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch4.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch5::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch5.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch6::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch6.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Max::Ch7::address,           (void*) &table.Reg.Setup.Dac.Range.Max.Ch7.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch0::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch0.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch1::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch1.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch2::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch2.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch3::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch3.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch4::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch4.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch5::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch5.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch6::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch6.value);
    test_address(MemoryTableDescription::Reg::Setup::Dac::Range::Min::Ch7::address,           (void*) &table.Reg.Setup.Dac.Range.Min.Ch7.value);
    test_address(MemoryTableDescription::Reg::Setup::AutoRead::Period::address,               (void*) &table.Reg.Setup.AutoRead.Period.value);
    test_address(MemoryTableDescription::Reg::Setup::AutoRead::FirstReg::address,             (void*) &table.Reg.Setup.AutoRead.FirstReg.value);
    test_address(MemoryTableDescription::Reg::Setup::AutoRead::Length::address,               (void*) &table.Reg.Setup.AutoRead.Length.value);
    test_address(MemoryTableDescription::Reg::Setup::BoardModel::address,                     (void*) &table.Reg.Setup.BoardModel.value);
    test_address(MemoryTableDescription::Reg::Setup::WhoAmI::address,                         (void*) &table.Reg.Setup.WhoAmI.value);

    return 0;
}



