#include "PCH.h"

#include "Plugin.h"
#include "DialogLib.h"
#include "PatchLib.h"

using namespace std;
using namespace RE;
using namespace REL;


void DialogNotify(const char* a_message)
{
    if (!a_message)
        a_message = "";

    SendHUDMessage::ShowHUDMessage(a_message, nullptr, true, true);
}

static MessageMenuManager* GetMessageMenuManager()
{
    static const uintptr_t MessageMenuManagerPtr_addr = RelocationID(959572, IsAE() ? 4796373 : 2689087, 959572).address();

    assert(MessageMenuManagerPtr_addr);

    return *reinterpret_cast<MessageMenuManager**>(MessageMenuManagerPtr_addr);
}

static void CreateMessage(
    MessageMenuManager*  a_this,
    const char*          a_headerText,
    const char*          a_bodyText,
    IMessageBoxCallback* a_callback,
    WARNING_TYPES        a_warningContext,
    const char*          a_button1Text = nullptr,
    const char*          a_button2Text = nullptr,
    const char*          a_button3Text = nullptr,
    const char*          a_button4Text = nullptr,
    bool                 a_ensureUnique = false)
{
    static const uintptr_t Create_addr = RelocationID(89563, 2249456, 89563).address();
    static const auto Create = reinterpret_cast<decltype(&CreateMessage)>(Create_addr);

    return Create(a_this, a_headerText, a_bodyText, a_callback, a_warningContext, a_button1Text, a_button2Text, a_button3Text, a_button4Text, a_ensureUnique);
}

void DialogConfirm(const char* a_message)
{
    if (!a_message)
        a_message = "";

    auto pMessageMenuManager = GetMessageMenuManager();

    if (!pMessageMenuManager)
        return;

    CreateMessage(pMessageMenuManager, Plugin::NAME, a_message, nullptr, WARNING_TYPES::kInGameMessage, "OK");
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

    auto pMessageMenuManager = GetMessageMenuManager();

    if (!pMessageMenuManager)
        return false;

    DialogYesNoCallback clbk;
    CreateMessage(pMessageMenuManager, Plugin::NAME, a_message, &clbk, WARNING_TYPES::kInGameMessage, "Yes", "No");

    return clbk.buttonIdx;
}
