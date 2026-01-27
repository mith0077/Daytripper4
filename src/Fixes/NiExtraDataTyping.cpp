#include "PCH.h"

#include "PatchLib.h"
#include "NiExtraDataTyping.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace NiExtraDataTyping
{
    static bool IsNiExtraData(NiRefObject* a_pObject)
    {
        static const auto srcType = reinterpret_cast<void*>(NiRefObject::RTTI.address());
        static const auto targetType = reinterpret_cast<void*>(NiExtraData::RTTI.address());

        return nullptr != __RTDynamicCast(a_pObject, 0, srcType, targetType, false);
    }

    namespace InsertExtraData
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(956675).address();
        static const auto patch_begin = target + 0x78;
        static const auto patch_end = target + 0x7D;
        static const auto erase_begin = target + 0x82;
        static const auto erase_end = target + 0x84;

        static bool Add_checked(NiExtraDataContainer* a_pExtraDataContainer, NiExtraData* a_pExtraData)
        {
            static const uintptr_t Add_addr = ID(1513432).address();
            static const auto Add = reinterpret_cast<bool (*)(NiExtraDataContainer*, NiExtraData*)>(Add_addr);

            // logger::debug("Entering Add_checked()");

            assert(a_pExtraDataContainer);
            assert(a_pExtraData);

            auto vtaddr = reinterpret_cast<uintptr_t*>(a_pExtraData)[0];

            if (!IsNiExtraData(a_pExtraData)) {
                logger::debug("Skipping InsertExtraData() for non-NiExtraData object, vtaddr: {:x}", vtaddr);

                return false;
            }

            Add(a_pExtraDataContainer, a_pExtraData);

            return true;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(Add_checked));
                call(rax);
                mov(rcx, patch_end);
                jmp(rcx);
            }
        };

        inline bool InstallHook()
        {
            safe_fill(erase_begin, NOP, erase_end - erase_begin);

            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace LinkObject
    {
        // XXX Status: verified
        static const uintptr_t target = ID(19042).address();
        static const auto patch_begin = target + 0x59;
        static const auto patch_end = target + 0x5E;
        static const auto label_continue = target + 0xA7;

        static NiObject* GetObjectFromLinkID_checked(NiStream* a_pStream)
        {
            static const uintptr_t GetObjectFromLinkID_addr = ID(493636).address();
            static const auto GetObjectFromLinkID = reinterpret_cast<NiObject* (*)(NiStream*)>(GetObjectFromLinkID_addr);

            auto pNiObject = GetObjectFromLinkID(a_pStream);

            if (!pNiObject)
                return nullptr;

            auto vtaddr = reinterpret_cast<uintptr_t*>(pNiObject)[0];

            if (!IsNiExtraData((pNiObject))) {
                logger::debug("Skipping LinkObject() for non-NiExtraData object, vtaddr: {:x}", vtaddr);

                return nullptr;
            }

            return pNiObject;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Xbyak::Label CASE_THEN, CASE_ELSE;

                mov(rax, reinterpret_cast<uintptr_t>(GetObjectFromLinkID_checked));
                call(rax);
                test(rax, rax);
                jz(CASE_ELSE);

                L(CASE_THEN);
                mov(rcx, patch_end);
                jmp(rcx);

                L(CASE_ELSE);
                mov(rcx, label_continue);
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
        if (InsertExtraData::InstallHook())
            logger::debug("Installed InsertExtraData hook");
        else
            logger::debug("Failed InsertExtraData hook");

        if (LinkObject::InstallHook())
            logger::debug("Installed LinkObject hook");
        else
            logger::debug("Failed LinkObject hook");
    }
}
