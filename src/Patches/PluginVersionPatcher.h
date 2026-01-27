#pragma once

#include "PCH.h"


namespace PluginVersionPatcher
{
    void InstallHook();

    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        InstallHook();

        logger::info("installed PluginVersionPatcher patch");
    }
}
