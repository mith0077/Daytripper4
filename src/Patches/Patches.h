#pragma once

#include "PCH.h"


namespace Patches
{
    void OnModuleLoad();
    void OnGameDataReady();
    void OnPostLoadGame();

    inline void OnGameDataReady() { }
    inline void OnPostLoadGame() { }
}
