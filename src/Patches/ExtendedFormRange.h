#pragma once

#include "PCH.h"


namespace ExtendedFormRange
{
    void InstallHook();

    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        InstallHook();

        logger::info("Installed ExtendedFormRange patch");
    }
}
