#pragma once

#include "PCH.h"

#include "DefaultObjectLib.h"


namespace FreeBuildSupport
{
    extern DefaultObject WorkshopAllowFreeBuild_DO;

    bool FreeBuild();
    void InstallHook();

    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        InstallHook();

        logger::info("Installed FreeBuildSupport patch");
    }
}
