#pragma once

#include "PCH.h"


namespace RobotModMenu
{
    void InstallHook();

    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        InstallHook();

        logger::info("Installed RobotModMenu fix");
    }
}
