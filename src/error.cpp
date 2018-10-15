//
// Copyright (c) 2018 Johan Sköld
// License: https://opensource.org/licenses/ISC
//

#include <pantryman/error.hpp>

#include <cassert>

namespace pm
{

    Error ERR_OK;

    ErrorAssert::~ErrorAssert()
    {
        assert(isOk(&err));
    }

    ErrorAssert::operator Error*()
    {
        return &err;
    }

    bool isOk(Error* err)
    {
        return err->code == 0;
    }

} // namespace pm
