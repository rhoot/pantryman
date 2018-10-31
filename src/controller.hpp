//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <pantryman/controller.hpp>

namespace pm
{

    ControllerButton operator| (ControllerButton a, ControllerButton b);
    ControllerButton operator& (ControllerButton a, ControllerButton b);

} // namespace pm
