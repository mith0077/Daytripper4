#include "PCH.h"

#include "PatchLib.h"
#include "RobotModMenu.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace RobotModMenu
{
    namespace FUN_1428b3320
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x28B3320).address();
        static const auto patch_begin = target + 0x0;
        static const auto patch_end = target + 0x5;

        static void Check_BuildABot()
        {
            // logger::debug("Entering Check_BuildABot()");
            static const uintptr_t BuildABot_QEnabled_addr = ID(1045095).address();
            static const auto BuildABot_QEnabled = reinterpret_cast<bool (*)()>(BuildABot_QEnabled_addr);

            static const uintptr_t BuildABot_Update_addr = Offset(0x9E6CE0).address();
            static const auto BuildABot_Update = reinterpret_cast<void (*)()>(BuildABot_Update_addr);

            static const auto qPauseMenu = BSFixedString("PauseMenu");
            static const auto qRobotModMenu = BSFixedString("RobotModMenu");

            if (!BuildABot_QEnabled())
                return;

            if (UI::GetSingleton()->GetMenuOpen(qPauseMenu))
                return;

            if (!UI::GetSingleton()->GetMenuOpen(qRobotModMenu))
                return;

            logger::debug("Calling BuildABot::Update()");

            BuildABot_Update();
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(Check_BuildABot));
                jmp(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    void InstallHook()
    {
        if (FUN_1428b3320::InstallHook())
            logger::debug("Installed FUN_1428b3320 hook");
        else
            logger::debug("Failed FUN_1428b3320 hook");
    }
}
