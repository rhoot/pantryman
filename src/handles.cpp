//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include "handles.hpp"

namespace pm
{

    constexpr uint32_t ERR_CAT = uint32_t('hndl');

    Error ERR_INVALID_HANDLE{ERR_CAT + 0, "invalid handle"};
    Error ERR_OUT_OF_HANDLES{ERR_CAT + 1, "out of handles"};

}
