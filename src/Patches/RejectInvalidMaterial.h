#pragma once

#include "PCH.h"


namespace RejectInvalidMaterial
{
    void InstallHook();

    inline void Install()
    {
        InstallHook();

        logger::info("Installed RejectInvalidMaterial patch");
    }
}
