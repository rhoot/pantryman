//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/controller.hpp>
#include "controller.hpp"

#include <type_traits>

namespace pm
{

    namespace
    {
        constexpr uint32_t ERR_GROUP = uint32_t('ctrl');
    } // namespace

    Error ERR_NOT_CONNECTED{ERR_GROUP + 0, "controller not connected"};
    Error ERR_INVALID_CONTROLLER{ERR_GROUP + 1, "invalid controller index"};

    ControllerButton operator| (ControllerButton a, ControllerButton b)
    {
        using Type = typename std::underlying_type<ControllerButton>::type;
        return ControllerButton(Type(a) | Type(b));
    }

    ControllerButton operator& (ControllerButton a, ControllerButton b)
    {
        using Type = typename std::underlying_type<ControllerButton>::type;
        return ControllerButton(Type(a) & Type(b));
    }

    bool isDown(ControllerButton pressed, ControllerButton button)
    {
        return (pressed & button) == button;
    }

    bool isUp(ControllerButton pressed, ControllerButton button)
    {
        return (pressed & button) == ControllerButton::NONE;
    }

    float stickAxis(int16_t value)
    {
        return (float(value) + 0.5f) / (float(INT16_MAX) + 0.5f);
    }

    float triggerAxis(uint8_t value)
    {
        return float(value) / float(UINT8_MAX);
    }

} // namespace pm
