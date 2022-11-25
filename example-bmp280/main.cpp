/*
 * Copyright (c) 2022, Christopher Durand
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#undef  MODM_LOG_LEVEL
#define MODM_LOG_LEVEL modm::log::INFO

#include <modm/board.hpp>
#include <modm/driver/pressure/bmp280.hpp>
#include <modm/driver/pressure/bmp280_transport.hpp>

using namespace Board;

using Scl = GpioA15;
using Sda = GpioB7;
using I2c = I2cMaster1;

using modm::bmp280;

int main()
{
	Board::initialize();

	I2c::connect<Scl::Scl, Sda::Sda>(I2c::PullUps::External);
	I2c::initialize<SystemClock, 100_kHz>();

	using Transport = modm::Bmp280TransportI2c<I2c>;

	modm::Bmp280Data data;
	modm::Bmp280<Transport> bmp280{data, 0x76};

	MODM_LOG_INFO << "BMP280 I2C test\n";
	MODM_LOG_INFO << "Ping, success: " << RF_CALL_BLOCKING(bmp280.ping()) << modm::endl;
	MODM_LOG_INFO << "Initializing, success: " << RF_CALL_BLOCKING(bmp280.initialize()) << modm::endl;

	modm::delay(500ms);

	while (true)
	{
		LedUser::toggle();
		RF_CALL_BLOCKING(bmp280.readout());

		MODM_LOG_INFO.printf("Temperature: %2.1f\n", data.getTemperature());
		MODM_LOG_INFO.printf("Pressure: %2.1f\n", data.getPressure() * 0.01f);

		modm::delay(500ms);
	}

	return 0;
}
