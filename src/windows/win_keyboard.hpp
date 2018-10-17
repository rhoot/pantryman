//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/platform.hpp>

#if PM_OS_WINDOWS

#include <pantryman/keyboard.hpp>
#include "../events.hpp"

namespace pm
{

    Key translateKey(uint32_t vk, bool extended);

} // namespace pm

#endif // PM_OS_WINDOWS
