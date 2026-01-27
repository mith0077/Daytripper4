#include "PCH.h"

#include "PatchLib.h"
#include "DlcPreCulling.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace DlcPreCulling
{
    static uint8_t s_maxLoadorder = 0;

    namespace AddToCullingGroup
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x27E0E10).address();
        static const auto patch_begin = target + 0x14;
        static const auto patch_end = target + 0x19;

        static const uintptr_t FUN_142875720_addr = Offset(0x2875720).address();

        static void AddToCullingGroupForSingleFormId(BSCullingGroup* a_pCullingGroup, BSCullingGroup* a_pCullingGroupAlt, TESFormID a_addingFormId,  uint32_t a_addMode)
        {
            static const uintptr_t Get3DForID_addr = Offset(0x27E0980).address();
            static const auto Get3DForID = reinterpret_cast<NiAVObject* (*)(TESFormID)>(Get3DForID_addr);

            static const uintptr_t Add_addr = Offset(0x1D4F840).address();
            static const auto Add = reinterpret_cast<void (*)(BSCullingGroup*, NiAVObject*, NiBound*, TESFormID)>(Add_addr);

            static const uintptr_t TrackVisibility_addr = Offset(0x27E0C20).address();
            static const auto TrackVisibility = reinterpret_cast<void (*)(TESFormID)>(TrackVisibility_addr);

            auto pAddingAvObject = Get3DForID(a_addingFormId);

            if (!pAddingAvObject)
                return;

            BSCullingGroup* pAddingCullingGroup;
            if (~(pAddingAvObject->GetFlags() >> 40) & 1)
                pAddingCullingGroup = a_pCullingGroup;
            else
                pAddingCullingGroup = a_pCullingGroupAlt;

            if (!pAddingCullingGroup)
                return;

            // logger::debug("Adding culling group, form id: {:08x}", a_addingFormId);

            Add(pAddingCullingGroup, pAddingAvObject, &pAddingAvObject->worldBound, a_addMode);
            TrackVisibility(a_addingFormId);
        }

        static void AddToCullingGroupForMismatchedLoadorder(BSCullingGroup* a_pCullingGroup, BSCullingGroup* a_pCullingGroupAlt)
        {
            static const uintptr_t g_pCullingIDArray_addr = ID(1370583).address();
            static const auto g_pCullingIDArray = reinterpret_cast<BSTArray<TESFormID>*>(g_pCullingIDArray_addr);

            // logger::debug("Entering AddToCullingGroupByFormIdAll()");

            assert(g_pCullingIDArray);

            for (auto formId : *g_pCullingIDArray) {
                auto loadorder = formId >> 24;

                if (0 == loadorder)  // Basegame formid matches always
                    continue;
                if (0xFD == loadorder)  // Combined meshes?
                    continue;

                auto maxLoadorder = s_maxLoadorder;
                maxLoadorder = (maxLoadorder > 0xFC) ? 0xFC : maxLoadorder;

                for (decltype(maxLoadorder) i = 1; i <= maxLoadorder; ++i) {
                    decltype(formId) sweepFormId = i << 24 | (formId & 0xFFFFFF);

                    if (formId == sweepFormId)
                        continue;

                    AddToCullingGroupForSingleFormId(a_pCullingGroup, a_pCullingGroupAlt, sweepFormId, 0);
                }
            }
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, FUN_142875720_addr);
                call(rax);
                mov(rcx, r14);
                mov(rdx, rbp);
                mov(rax, reinterpret_cast<uintptr_t>(AddToCullingGroupForMismatchedLoadorder));
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

    namespace UpdateIDto3DMap
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1162647).address();
        static const auto patch_begin = target + 0xCE;
        static const auto patch_end = target + 0xD3;

        static void UpdateMaxLoadorder(TESFormID a_formId)
        {
            uint8_t loadorder = static_cast<uint8_t>(a_formId >> 24 & 0xFF);

            if (0xFD <= loadorder)
                return;
            if (loadorder <= s_maxLoadorder)
                return;

            logger::debug("Updating max loadorder of culling group to: {:02x}", loadorder);

            s_maxLoadorder = loadorder;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(ecx, ebx);
                mov(rax, reinterpret_cast<uintptr_t>(UpdateMaxLoadorder));
                call(rax);
                mov(rax, ptr[rsp + 0x38]);
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
        if (AddToCullingGroup::InstallHook())
            logger::debug("Installed AddToCullingGroup hook");
        else
            logger::debug("Failed AddToCullingGroup hook");

        if (UpdateIDto3DMap::InstallHook())
            logger::debug("Installed UpdateIDto3DMap hook");
        else
            logger::debug("Failed UpdateIDto3DMap hook");
    }
}
