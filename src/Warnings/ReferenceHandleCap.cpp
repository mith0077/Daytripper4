#include "PCH.h"

#include "Plugin.h"
#include "Settings.h"
#include "DialogLib.h"
#include "ReferenceHandleCap.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace ReferenceHandleCap
{
    constexpr uint32_t HANDLECAP_HARD_LIMIT = 0x100000;

    static uint32_t ReferenceHandleCount()
    {
        logger::debug("Entering ReferenceHandleCount()");

        static const auto pReferenceHandleArray_addr = VariantID(1103816, 2688744, 0x392EAB0).address();
        static const auto pReferenceHandleArray = reinterpret_cast<uint64_t*>(pReferenceHandleArray_addr);

        uint32_t count = 0;

        for (uint32_t i = 0; i < HANDLECAP_HARD_LIMIT; ++i) {
            if ((pReferenceHandleArray[i] >> 26) & 1)
                ++count;
        }

        return count;
    }

    void RunGameDataReadyHook()
    {
        auto iLimit = *Settings::ReferenceHandleLimit_MainMenu;

        if (0 >= iLimit)
            return;
        if (HANDLECAP_HARD_LIMIT <= iLimit)
            return;

        auto limit = static_cast<uint32_t>(iLimit);
        auto count = ReferenceHandleCount();
        auto ratio = static_cast<float>(count) / static_cast<float>(HANDLECAP_HARD_LIMIT);

        logger::info("Checking Reference Handle count (Main Menu): {} ({:.2} %)", count, ratio * 100.0);

        if (count >= limit) {
            logger::warn("WARNING: Reference Handle count {} exceeds main menu limit {}!", count, limit);
            logger::warn("Please uninstall several world expansion mods not to reach hard limit {}.", HANDLECAP_HARD_LIMIT);
            logger::warn("Edit 'ReferenceHandleLimit_LoadGame' value of config file to suppress this message.");

            ostringstream ws;
            ws << "WARNING: Reference Handle capacity is almost drying up!" << endl;
            ws << "Please refer following log file for details:" << endl;
            ws << "    " << "My Games\\Fallout4" << (Module::IsVR() ? "VR\\" : "\\") << Plugin::NAME << ".log";
            ws << endl;

            if (!Module::IsVR())  // Will just hang up
                REX::W32::MessageBoxA(nullptr, ws.str().c_str(), Plugin::NAME, 0);
        }
    }

    void RunPostLoadGameHook()
    {
        auto iLimit = *Settings::ReferenceHandleLimit_LoadGame;

        if (0 >= iLimit)
            return;
        if (HANDLECAP_HARD_LIMIT <= iLimit)
            return;

        auto limit = static_cast<uint32_t>(iLimit);
        auto count = ReferenceHandleCount();
        auto ratio = static_cast<float>(count) / static_cast<float>(HANDLECAP_HARD_LIMIT);

        logger::info("Checking Reference Handle count (Load Game): {} ({:.2} %)", count, ratio * 100.0);

        if (count >= limit) {
            logger::warn("WARNING: Reference Handle count {} exceeds load game limit {}!", count, limit);
            logger::warn("Please uninstall several world expansion mods not to reach hard limit {}.", HANDLECAP_HARD_LIMIT);
            logger::warn("Or edit 'ReferenceHandleLimit_LoadGame' value of config file to suppress this message.");

            ostringstream ws;
            ws << "<u>" << "WARNING: (" << Plugin::NAME << ")" << "</u>" << endl;
            ws << "<p align='left'>";
            ws << "Reference Handle capacity is almost drying up!" << endl;
            ws << "Continuing game can lead to occasional CTD." << endl;
            ws << "Please refer following log file for details:" << endl;
            ws << "</p>" << "<ul><li>";
            ws << "My Games\\Fallout4" << (Module::IsVR() ? "VR\\" : "\\") << Plugin::NAME << ".log";
            ws << "</li></ul>" << endl;

            DialogConfirm(ws.str().c_str());
        }
    }
}
