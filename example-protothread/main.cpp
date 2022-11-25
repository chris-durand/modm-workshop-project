/*
 * Copyright (c) 2022, Christopher Durand
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <modm/board.hpp>
#include <modm/driver/display/ssd1306.hpp>
#include <modm/processing/protothread.hpp>
#include <modm/processing/timer.hpp>

using namespace Board;
using namespace modm::literals;

using Scl = GpioA9;
using Sda = GpioA8;
using I2c = I2cMaster2;

modm::Ssd1306<I2c> display;

struct DisplayThread : modm::pt::Protothread
{
    modm::PeriodicTimer timer{500ms};
    uint32_t counter{};

    bool run()
    {
        PT_BEGIN();

        I2c::connect<Scl::Scl, Sda::Sda>(I2c::PullUps::External);
        I2c::initialize<SystemClock, 100_kHz>();

        PT_CALL(display.initialize());
        display.setFont(modm::font::FixedWidth5x8);
        display << "Hello World!";
        PT_CALL(display.writeDisplay());

        while (true) {
            display.setCursor(0,20);
            display << "counter: " << counter++;
            PT_CALL(display.writeDisplay());

            PT_WAIT_UNTIL(timer.execute());
        }
        PT_END();
    }
};

struct LedThread : modm::pt::Protothread
{
    modm::Timeout timer;

    bool run()
    {
        PT_BEGIN();
        while (true) {
            LedUser::set();
            timer.restart(500ms);
            PT_WAIT_UNTIL(timer.isExpired());

            LedUser::reset();
            timer.restart(500ms);
            PT_WAIT_UNTIL(timer.isExpired());

            LedUser::set();
            timer.restart(100ms);
            PT_WAIT_UNTIL(timer.isExpired());

            LedUser::reset();
            timer.restart(100ms);
            PT_WAIT_UNTIL(timer.isExpired());
        }
        PT_END();
    }
};

DisplayThread displayThread;
LedThread ledThread;

int main()
{
    Board::initialize();

    MODM_LOG_INFO << "protothread example\n";

    while (true)
    {
        displayThread.run();
        ledThread.run();
    }

    return 0;
}
