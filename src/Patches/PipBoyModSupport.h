#pragma once

#include "PCH.h"


namespace PipBoyModSupport
{
    void InstallHook();

    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        InstallHook();

        logger::info("Installed PipBoyModSupport patch");
    }
}
