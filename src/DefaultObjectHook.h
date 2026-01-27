#pragma once

#include "PCH.h"


namespace DefaultObjectHook
{
    void InstallHook();

    inline void Install()
    {
        InstallHook();

        logger::info("Installed DefaultObject hook");
    }
}
