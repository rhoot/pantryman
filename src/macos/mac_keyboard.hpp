//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/platform.hpp>

#if PM_OS_MACOS

#include <pantryman/keyboard.hpp>
#include "../events.hpp"

namespace pm
{

    Key translateKey(uint16_t vk);
    MetaKeyFlags::Type translateMeta(uint32_t meta);

} // namespace pm

#endif // PM_OS_MACOS
