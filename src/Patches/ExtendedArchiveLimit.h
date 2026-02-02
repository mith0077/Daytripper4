#pragma once

#include "PCH.h"

#include "ExtendedArchiveLimit/Archive2IndexHook.h"
#include "ExtendedArchiveLimit/Archive2IndexEntryHook.h"
#include "ExtendedArchiveLimit/TextureIndexHook.h"
#include "ExtendedArchiveLimit/TextureIndexEntryHook.h"
#include "ExtendedArchiveLimit/TextureRequestHook.h"


namespace ExtendedArchiveLimit
{
    inline void Install()
    {
        Archive2IndexHook::InstallHook();
        logger::debug("Installed Archive2IndexHook");

        Archive2IndexEntryHook::InstallHook();
        logger::debug("Installed Archive2IndexEntryHook");

        TextureIndexHook::InstallHook();
        logger::debug("Installed TextureIndexHook");

        TextureIndexEntryHook::InstallHook();
        logger::debug("Installed TextureIndexEntryHook");

        TextureRequestHook::InstallHook();
        logger::debug("Installed TextureRequestHook");

        logger::info("Installed ExtendedArchiveLimit patch");
    }
}
