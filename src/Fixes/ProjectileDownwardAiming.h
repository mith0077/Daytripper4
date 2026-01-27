#pragma once

#include "PCH.h"


namespace ProjectileDownwardAiming
{
    void InstallHook();

    inline void Install()
    {
        InstallHook();

        logger::info("Installed ProjectileDownwardAiming fix");
    }
}
