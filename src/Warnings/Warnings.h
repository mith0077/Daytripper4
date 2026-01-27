#pragma once

namespace Warnings
{
    void OnModuleLoad();
    void OnGameDataReady();
    void OnPostLoadGame();

    inline void OnModuleLoad() { }
}
