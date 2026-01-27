#include "PCH.h"

#include "DefaultObjectLib.h"
#include "PatchLib.h"
#include "FreeBuildSupport.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace FreeBuildSupport
{
    DefaultObject WorkshopAllowFreeBuild_DO = DefaultObject(
        "WorkshopAllowFreeBuild_DO",
        ENUM_FORM_ID::kKYWD,
        "Keyword to indicate that the Workshop where this keyword is found allows for free building.");

    bool FreeBuild()
    {
        static const uintptr_t g_pWorkshopHdr_addr = ID(737927).address();
        static const auto g_pWorkshopHdr = reinterpret_cast<RefHandle*>(g_pWorkshopHdr_addr);

        // logger::debug("Entering FreeBuild()");

        BGSKeyword* pWorkshopAllowFreeBuild = WorkshopAllowFreeBuild_DO.GetForm<BGSKeyword>();

        if (!pWorkshopAllowFreeBuild)
            return false;

        NiPointer<TESObjectREFR> workshopPtr;
        LookupReferenceByHandle(*g_pWorkshopHdr, workshopPtr);

        if (!workshopPtr)
            return false;

        return workshopPtr->HasKeyword(pWorkshopAllowFreeBuild);
    }

    static bool IsGodModeOrFreeBuild()
    {
        static const uintptr_t IsGodMode_addr = ID(1032309).address();
        static const auto IsGodMode = reinterpret_cast<bool (*)()>(IsGodMode_addr);

        // logger::debug("Entering IsGodModeOrFreeBuild()");

        return IsGodMode() || FreeBuild();
    }

    static void ForEachScrapRewardItemForNonFreeBuild(TESObjectREFR* a_pWorkshopRef, bool a_useComponent, BSScrapArray<TESForm>* a_pScrapItemAry)
    {
        static const uintptr_t ForEachScrapRewardItem_addr = Offset(0x237C70).address();
        static const auto ForEachScrapRewardItem = reinterpret_cast<void (*)(TESObjectREFR*, bool, BSScrapArray<TESForm>*)>(ForEachScrapRewardItem_addr);

        // logger::debug("Entering ForEachScrapRewardItemForNonFreeBuild()");

        assert(a_pWorkshopRef);
        assert(a_pScrapItemAry);

        if (FreeBuild())
            return;

        ForEachScrapRewardItem(a_pWorkshopRef, a_useComponent, a_pScrapItemAry);
    }

    namespace CollectScrapRewords
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x2044C0).address();
        static const auto patch_begin = target + 0x10;
        static const auto patch_end = target + 0x15;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(ForEachScrapRewardItemForNonFreeBuild));
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

    namespace GetBuildCount
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x216590).address();
        static const auto patch_begin = target + 0xE;
        static const auto patch_end = target + 0x13;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace CreateNewReference
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1042671).address();
        static const auto patch_begin = target + 0x143;
        static const auto patch_end = target + 0x148;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace FUN_14021af50
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0x21AF50).address();
        static const auto patch_begin = target + 0x118;
        static const auto patch_end = target + 0x11D;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace ScrapReference
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(636327).address();
        static const auto patch_begin = target + 0x215;
        static const auto patch_end = target + 0x21B;
        static const uintptr_t label_noref = target + 0x34B;
        static const uintptr_t label_freebuild = target + 0x342;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Xbyak::Label LABEL_NOREF, LABEL_FREEBUILD;

                jz(LABEL_NOREF);

                mov(rax, reinterpret_cast<uintptr_t>(FreeBuild));
                call(rax);
                test(al, al);
                jnz(LABEL_FREEBUILD);

                mov(rcx, patch_end);
                jmp(rcx);

                L(LABEL_NOREF);
                mov(rcx, label_noref);
                jmp(rcx);

                L(LABEL_FREEBUILD);
                mov(rcx, label_freebuild);
                jmp(rcx);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace ScrapReference_1
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(636327).address();
        static const auto patch_begin = target + 0x2BF;
        static const auto patch_end = target + 0x2C4;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(ForEachScrapRewardItemForNonFreeBuild));
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

    namespace PlaceCurrentReference
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1058211).address();
        static const auto patch_begin = target + 0x282;
        static const auto patch_end = target + 0x287;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace GetAvailableCount
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(1583245).address();
        static const auto patch_begin = target + 0xA2;
        static const auto patch_end = target + 0xA7;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace RequestScrapRef
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1402505).address();
        static const auto patch_begin = target + 0x58;
        static const auto patch_end = target + 0x5D;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(ForEachScrapRewardItemForNonFreeBuild));
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

    namespace RequestRepairItem
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x223360).address();
        static const auto patch_begin = target + 0xBF;
        static const auto patch_end = target + 0xC4;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace RequestRepairItem_1
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x223360).address();
        static const auto patch_begin = target + 0x2C1;
        static const auto patch_end = target + 0x2C6;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace FUN_14024d640
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0x24d640).address();
        static const auto patch_begin = target + 0x93;
        static const auto patch_end = target + 0x98;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace FUN_140c0b390
    {
        // XXX Status: noctd
        static const uintptr_t target = Offset(0xC0B390).address();
        static const auto patch_begin = target + 0x86;
        static const auto patch_end = target + 0x8B;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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

    namespace FUN_140c0baf0
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xC0BAF0).address();
        static const auto patch_begin = target + 0x85;
        static const auto patch_end = target + 0x8A;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsGodModeOrFreeBuild));
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
        if (CollectScrapRewords::InstallHook())
            logger::debug("Installed CollectScrapRewords hook");
        else
            logger::debug("Failed CollectScrapRewords hook");

        if (GetBuildCount::InstallHook())
            logger::debug("Installed GetBuildCount hook");
        else
            logger::debug("Failed GetBuildCount hook");

        if (CreateNewReference::InstallHook())
            logger::debug("Installed CreateNewReference hook");
        else
            logger::debug("Failed CreateNewReference hook");

        if (FUN_14021af50::InstallHook())
            logger::debug("Installed FUN_14021af50 hook");
        else
            logger::debug("Failed FUN_14021af50 hook");

        if (ScrapReference::InstallHook())
            logger::debug("Installed ScrapReference hook");
        else
            logger::debug("Failed ScrapReference hook");

        if (ScrapReference_1::InstallHook())
            logger::debug("Installed ScrapReference_1 hook");
        else
            logger::debug("Failed ScrapReference_1 hook");

        if (PlaceCurrentReference::InstallHook())
            logger::debug("Installed PlaceCurrentReference hook");
        else
            logger::debug("Failed PlaceCurrentReference hook");

        if (GetAvailableCount::InstallHook())
            logger::debug("Installed GetAvailableCount hook");
        else
            logger::debug("Failed GetAvailableCount hook");

        if (RequestScrapRef::InstallHook())
            logger::debug("Installed RequestScrapRef hook");
        else
            logger::debug("Failed RequestScrapRef hook");

        if (RequestRepairItem::InstallHook())
            logger::debug("Installed RequestRepairItem hook");
        else
            logger::debug("Failed RequestRepairItem hook");

        if (RequestRepairItem_1::InstallHook())
            logger::debug("Installed RequestRepairItem_1 hook");
        else
            logger::debug("Failed RequestRepairItem_1 hook");

        if (FUN_14024d640::InstallHook())
            logger::debug("Installed FUN_14024d640 hook");
        else
            logger::debug("Failed FUN_14024d640 hook");

        if (FUN_140c0b390::InstallHook())
            logger::debug("Installed FUN_140c0b390 hook");
        else
            logger::debug("Failed FUN_140c0b390 hook");

        if (FUN_140c0baf0::InstallHook())
            logger::debug("Installed FUN_140c0baf0 hook");
        else
            logger::debug("Failed FUN_140c0baf0 hook");
    }
}
