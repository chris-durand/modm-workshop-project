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

using namespace Board;

using PushButton = GpioInverted<GpioB4>;

int main()
{
	Board::initialize();
	PushButton::setInput(PushButton::InputType::PullUp);

	MODM_LOG_INFO << "Hello World!\n";

	uint32_t counter{};

	while (true)
	{
		LedUser::toggle();
		MODM_LOG_INFO << "counter: " << counter++ << modm::endl;
		PushButton::read() ? modm::delay(100ms) : modm::delay(500ms);
	}

	return 0;
}
