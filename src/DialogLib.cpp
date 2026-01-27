#include "PCH.h"

#include "Plugin.h"
#include "DialogLib.h"

using namespace std;
using namespace RE;


void DialogNotify(const char* a_message)
{
    if (!a_message)
        a_message = "";

    SendHUDMessage::ShowHUDMessage(a_message, nullptr, true, true);
}

void DialogConfirm(const char* a_message)
{
    if (!a_message)
        a_message = "";

    auto pMessageMenuManager = MessageMenuManager::GetSingleton();

    if (!pMessageMenuManager)
        return;

    pMessageMenuManager->Create(Plugin::NAME, a_message, nullptr, WARNING_TYPES::kInGameMessage, "OK");
}

class DialogYesNoCallback : public IMessageBoxCallback
{
    public:
        uint8_t buttonIdx;

        DialogYesNoCallback() : buttonIdx(0) { }

        virtual void operator()(uint8_t a_buttonIdx)
        {
            buttonIdx = a_buttonIdx;
        }
};

bool DialogYesNo(const char* a_message)
{
    if (!a_message)
        a_message = "";

    auto pMessageMenuManager = MessageMenuManager::GetSingleton();

    if (!pMessageMenuManager)
        return false;

    DialogYesNoCallback clbk;
    pMessageMenuManager->Create(Plugin::NAME, a_message, &clbk, WARNING_TYPES::kInGameMessage, "Yes", "No");

    return clbk.buttonIdx;
}
