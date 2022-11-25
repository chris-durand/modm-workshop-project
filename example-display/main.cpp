/*
 * Copyright (c) 2014-2017, Niklas Hauser
 * Copyright (c) 2017, Sascha Schade
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

using namespace Board;

using Scl = GpioA9;
using Sda = GpioA8;
using I2c = I2cMaster2;

int
main()
{
	Board::initialize();

	I2c::connect<Scl::Scl, Sda::Sda>(I2c::PullUps::External);
	I2c::initialize<SystemClock, 100_kHz>();

	modm::Ssd1306<I2c> display;

	MODM_LOG_INFO << "SSD1306 display test\n";

	display.initializeBlocking();
	display.setFont(modm::font::FixedWidth5x8);
	display << "Hello World!";
	display.update();

	modm::ShortPeriodicTimer timer(1s);
	uint16_t counter(0);

	while (true)
	{
		if (timer.execute())
		{
			display.setCursor(0,20);
			display << counter++;
			display.update();
		}
	}

	return 0;
}
