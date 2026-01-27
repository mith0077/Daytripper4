#include "PCH.h"

#include "PatchLib.h"
#include "PipBoyModSupport.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace PipBoyModSupport
{
    namespace GetIsValidInventoryItem
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x0B40EF0).address();
        static const auto patch_begin = target + 0x1E;
        static const auto patch_end = target + 0x23;

        static bool IsValidItemOrPipBoy(BGSInventoryItem* a_pInventoryItem, uint32_t a_stackIndex)
        {
            static const uintptr_t IsValidItem_addr = ID(1567424).address();
            static const auto IsValidItem = reinterpret_cast<bool (*)(BGSInventoryItem*, uint32_t)>(IsValidItem_addr);

            static const uintptr_t ppPipBoyObject_addr = Offset(0x5A3E2B8).address() + 0x20;
            static const auto ppPipBoyObject = reinterpret_cast<TESBoundObject**>(ppPipBoyObject_addr);

            // logger::debug("Entering IsValidItemOrPipBoy()");

            assert(a_pInventoryItem);

            if (IsValidItem(a_pInventoryItem, a_stackIndex))
                return true;
            else if (*ppPipBoyObject == a_pInventoryItem->object)
                return true;
            else
                return false;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsValidItemOrPipBoy));
                call(rax);
                mov(rcx, patch_end);
                jmp(rcx);
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
        if (GetIsValidInventoryItem::InstallHook())
            logger::debug("Installed GetIsValidInventoryItem hook");
        else
            logger::debug("Failed GetIsValidInventoryItem hook");
    }
}
