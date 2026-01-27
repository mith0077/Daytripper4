#pragma once

#include "PCH.h"

#include "ExtendedArchiveLimit/Archive2IndexHook.h"
#include "ExtendedArchiveLimit/Archive2IndexEntryHook.h"


namespace ExtendedArchiveLimit
{
    inline void Install()
    {
        Archive2IndexHook::InstallHook();
        logger::debug("Installed Archive2IndexHook");

        Archive2IndexEntryHook::InstallHook();
        logger::debug("Installed Archive2IndexEntryHook");

        logger::info("Installed ExtendedArchiveLimit patch");
    }
}
