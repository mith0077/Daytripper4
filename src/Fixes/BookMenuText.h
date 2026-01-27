#pragma once

#include "PCH.h"


namespace BookMenuText
{
    void InstallHook();

    inline void Install()
    {
        InstallHook();

        logger::info("Installed BookMenuText fix");
    }
}
