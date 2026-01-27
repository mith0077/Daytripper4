#include "PCH.h"

#include "Settings.h"
#include "CreationClubSupport.h"
#include "EslExtensionSupport.h"
#include "ExtendedArchiveLimit.h"
#include "ExtendedFormRange.h"
#include "FreeBuildSupport.h"
#include "PipBoyModSupport.h"
#include "PluginVersionPatcher.h"
#include "RejectInvalidMaterial.h"
#include "SmallFileLoader.h"
#include "Patches.h"

using namespace REL;


namespace Patches
{
    void OnModuleLoad()
    {
        if (Module::IsVR() && *Settings::CreationClubSupport)
            CreationClubSupport::Install();
        if (Module::IsVR() && *Settings::EslExtensionSupport)
            EslExtensionSupport::Install();
        if (*Settings::ExtendedArchiveLimit)
            ExtendedArchiveLimit::Install();
        if (Module::IsVR() && *Settings::ExtendedFormRange)
            ExtendedFormRange::Install();
        if (Module::IsVR() && *Settings::FreeBuildSupport)
            FreeBuildSupport::Install();
        if (Module::IsVR() && *Settings::PipBoyModSupport)
            PipBoyModSupport::Install();
        if (Module::IsVR() && *Settings::PluginVersionPatcher)
            PluginVersionPatcher::Install();
        if (Module::IsVR() && *Settings::RejectInvalidMaterial)
            RejectInvalidMaterial::Install();
        if (Module::IsVR() && *Settings::SmallFileLoader)
            SmallFileLoader::Install();
    }
}
