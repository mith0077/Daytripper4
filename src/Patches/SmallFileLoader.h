#pragma once

#include "PCH.h"

#include "SmallFileLoader/AdjustFormIDFileIndexHook.h"
#include "SmallFileLoader/DataHandlerHook.h"
#include "SmallFileLoader/GetFileAtHook.h"
#include "SmallFileLoader/GetSmallFileHook.h"
#include "SmallFileLoader/LoadTESInfoHook.h"
#include "SmallFileLoader/SaveLoadGameHook.h"
#include "SmallFileLoader/SetCompileIndexHook.h"


namespace SmallFileLoader
{
    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        AdjustFormIDFileIndexHook::InstallHook();
        logger::debug("Installed AdjustFormIDFileIndexHook");

        DataHandlerHook::InstallHook();
        logger::debug("Installed DataHandlerHook");

        GetFileAtHook::InstallHook();
        logger::debug("Installed GetFileAtHook");

        GetSmallFileHook::InstallHook();
        logger::debug("Installed GetSmallFileHook");

        LoadTESInfoHook::InstallHook();
        logger::debug("Installed LoadTESInfoHook");

        SaveLoadGameHook::InstallHook();
        logger::debug("Installed SaveLoadGameHook");

        SetCompileIndexHook::InstallHook();
        logger::debug("Installed SetCompileIndexHook");

        logger::info("installed SmallFileLoader patch");
    }
}
