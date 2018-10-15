//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#pragma once

#include <cstdint>

namespace pm
{

    constexpr uint16_t MAX_WINDOW_TITLE_LEN = 255;

    struct CreateWindowArgs
    {
        uint16_t width;
        uint16_t height;
        uint8_t index;
        char title[MAX_WINDOW_TITLE_LEN + 1];
    };

} // namespace pm
