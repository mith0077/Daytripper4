#pragma once

#include "PCH.h"


namespace EslExtensionSupport
{
    void InstallHook();

    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        InstallHook();

        logger::info("Installed EslExtensionSupport patch");
    }
}
