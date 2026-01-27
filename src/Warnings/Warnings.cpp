#include "PCH.h"

#include "Settings.h"
#include "ReferenceHandleCap.h"
#include "Warnings.h"

using namespace REL;


namespace Warnings
{
    void OnGameDataReady()
    {
        if (*Settings::ReferenceHandleCap)
            ReferenceHandleCap::RunGameDataReadyHook();
    }

    void OnPostLoadGame()
    {
        if (*Settings::ReferenceHandleCap)
            ReferenceHandleCap::RunPostLoadGameHook();
    }
}
