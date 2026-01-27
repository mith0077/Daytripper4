#pragma once

#include "PCH.h"


namespace NiExtraDataTyping
{
    void InstallHook();

    inline void Install()
    {
        InstallHook();

        logger::info("Installed NiExtraDataTyping fix");
    }
}
