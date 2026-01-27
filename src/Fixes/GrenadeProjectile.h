#pragma once

#include "PCH.h"


namespace GrenadeProjectile
{
    void InstallHook();

    inline void Install()
    {
        InstallHook();

        logger::info("Installed GrenadeProjectile fix");
    }
}
