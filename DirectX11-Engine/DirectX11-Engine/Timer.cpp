/* Copyright (c) 2019, Arm Limited and Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "timer.h"

Timer::Timer() :
	startTime{ Clock::now() },
	previousTick{ Clock::now() }
{}

void Timer::start()
{
	if (!running)
	{
		running = true;
		startTime = Clock::now();
	}
}

void Timer::lap()
{
	lapping = true;
	lapTime = Clock::now();
}

bool Timer::isRunning() const
{
	return running;
}
