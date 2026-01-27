#pragma once

#include "PCH.h"


namespace WetnessMaterialSsr
{
    void InstallHook();

    inline void Install()
    {
        InstallHook();

        logger::info("Installed WetnessMaterialSsr fix");
    }
}
