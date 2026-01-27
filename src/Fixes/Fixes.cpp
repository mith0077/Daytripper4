#include "PCH.h"

#include "Settings.h"
#include "BookMenuText.h"
#include "DlcPreCulling.h"
#include "GrenadeProjectile.h"
#include "NiExtraDataTyping.h"
#include "ProjectileDownwardAiming.h"
#include "RobotModMenu.h"
#include "WetnessMaterialSsr.h"
#include "Fixes.h"

using namespace REL;


namespace Fixes
{
    void OnModuleLoad()
    {
        if (Module::IsVR() && *Settings::BookMenuText)
            BookMenuText::Install();
        if (Module::IsVR() && *Settings::DlcPreCulling)
            DlcPreCulling::Install();
        if (Module::IsVR() && *Settings::GrenadeProjectile)
            GrenadeProjectile::Install();
        if (Module::IsVR() && *Settings::NiExtraDataTyping)
            NiExtraDataTyping::Install();
        if (*Settings::ProjectileDownwardAiming)
            ProjectileDownwardAiming::Install();
        if (Module::IsVR() && *Settings::RobotModMenu)
            RobotModMenu::Install();
        if (Module::IsVR() && *Settings::WetnessMaterialSsr)
            WetnessMaterialSsr::Install();
    }
}
