#pragma once

#include "PCH.h"

namespace Fixes
{
    void OnModuleLoad();
    void OnGameDataReady();
    void OnPostLoadGame();

    inline void OnGameDataReady() { }
    inline void OnPostLoadGame() { }
}
