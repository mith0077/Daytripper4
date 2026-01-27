#include "PCH.h"

#include "PatchLib.h"
#include "SmallFileLib.h"
#include "AdjustFormIDFileIndexHook.h"

using namespace std;
using namespace RE;
using namespace REL;


// Hooks that have references to AdjustFormIDFileIndex(Fallout4.exe+0x140131FA0)
namespace SmallFileLoader::AdjustFormIDFileIndexHook
{
    namespace AddCompileIndex
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1315637).address();
        static const auto patch_begin = target + 0x39;
        static const auto patch_end = target + 0x4F;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rdx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(AdjustFormIDFileIndex));
                call(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace GetExtCellDataFromFileByEditorID
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(1411233).address();
        static const auto patch_begin = target + 0x281;
        static const auto patch_end = target + 0x294;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(edx, esi);
                mov(rax, reinterpret_cast<uintptr_t>(GetAdjustedFormIDFileIndex));
                call(rax);
                mov(ecx, eax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace ReadFormHeader
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1583129).address();
        static const auto patch_begin = target + 0x8D;
        static const auto patch_end = target + 0xF6;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
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

    namespace ReadFormHeader_1
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1583129).address();
        static const auto patch_begin = target + 0x102;
        static const auto patch_end = target + 0x112;

        static const uintptr_t IsDefaultForm_addr = Offset(0x15AE20).address();
        static const auto IsDefaultForm = reinterpret_cast<bool (*)(TESFormID)>(IsDefaultForm_addr);

        static void CheckDefaultAndAdjustCurrentFormIDFileIndex(TESFormID a_formId, TESFile* a_pFile)
        {
            // logger::debug("Entering CheckDefaultAndAdjustCurrentFormIDFileIndex()");

            if (!a_pFile)
                return;

            TESFile* pMasterFile = nullptr;

            do {
                if (!a_pFile->masterPtrs)
                    break;

                auto masterIndex = GetNormalIndex(a_pFile->currentform.formID);

                if (masterIndex + 1u > a_pFile->masterCount)
                    break;
                if (0xFF == masterIndex)
                    break;

                pMasterFile = a_pFile->masterPtrs[masterIndex];
            }
            while (false);

            if (!pMasterFile)
                pMasterFile = a_pFile;

            if (IsDefaultForm(a_formId))
                a_pFile->currentform.formID &= 0x00FFFFFF;
            else
                AdjustFormIDFileIndex(pMasterFile, &a_pFile->currentform.formID);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rdx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(CheckDefaultAndAdjustCurrentFormIDFileIndex));
                call(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace GetFormByString
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1484174).address();
        static const auto patch_begin = target + 0x13E;
        static const auto patch_end = target + 0x154;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(edx, edi);
                mov(rax, reinterpret_cast<uintptr_t>(GetAdjustedFormIDFileIndex));
                call(rax);
                mov(edi, eax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace InitItem
    {
        // XXX Status: unchecked
        static const uintptr_t target = ID(1309570).address();
        static const auto patch_begin = target + 0xF0;
        static const auto patch_end = target + 0x106;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(edx, esi);
                mov(rax, reinterpret_cast<uintptr_t>(GetAdjustedFormIDFileIndex));
                call(rax);
                mov(esi, eax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace ReadForm
    {
        static const uintptr_t target = ID(112295).address();

        // TODO: Something relaed to LoadSharedPlayerData() that will read data
        // from unknown file "SharedPlayer.dat". Is this dead code block?
    }

    namespace FUN_141445d20
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0x1445D20).address();
        static const auto patch_begin = target + 0x3D;
        static const auto patch_end = target + 0x50;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(edx, edi);
                mov(rax, reinterpret_cast<uintptr_t>(GetAdjustedFormIDFileIndex));
                call(rax);
                mov(ecx, eax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace ConstructObject
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1333622).address();
        static const auto patch_begin = target + 0x1B8;
        static const auto patch_end = target + 0x1EC;
        static const auto jmp_target = target + 0x1013;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Xbyak::Label CASE_THEN, CASE_ELSE;

                jz(CASE_ELSE);

                L(CASE_THEN);
                mov(rax, jmp_target);
                jmp(rax);

                L(CASE_ELSE);
                mov(rax, patch_end);
                jmp(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace ConstructObject_1
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1333622).address();
        static const auto patch_begin = target + 0x503;
        static const auto patch_end = target + 0x519;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(edx, ebx);
                mov(rax, reinterpret_cast<uintptr_t>(GetAdjustedFormIDFileIndex));
                call(rax);
                mov(ecx, eax);
                mov(edx, dword[r15]);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace ConstructObject_2
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1333622).address();
        static const auto patch_begin = target + 0x5A3;
        static const auto patch_end = target + 0x5BD;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(edx, ebx);
                mov(rax, reinterpret_cast<uintptr_t>(GetAdjustedFormIDFileIndex));
                call(rax);
                mov(ecx, eax);
                mov(rdx, qword[rbp + 0xE8]);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace RunCellEditorIDLookUp
    {
        // NOTE: Fallout4VR.exe's implement should be sufficient,
        //       because Fallout4.exe's doesn't consider snallFiles neither.
        // static const uintptr_t target = Offset(0x11B220).address();
    }

    void InstallHook()
    {
        if (AddCompileIndex::InstallHook())
            logger::debug("Installed AddCompileIndex hook");
        else
            logger::debug("Failed AddCompileIndex hook");

        if (GetExtCellDataFromFileByEditorID::InstallHook())
            logger::debug("Installed GetExtCellDataFromFileByEditorID hook");
        else
            logger::debug("Failed GetExtCellDataFromFileByEditorID hook");

        if (ReadFormHeader::InstallHook())
            logger::debug("Installed ReadFormHeader hook");
        else
            logger::debug("Failed ReadFormHeader hook");

        if (ReadFormHeader_1::InstallHook())
            logger::debug("Installed ReadFormHeader_1 hook");
        else
            logger::debug("Failed ReadFormHeader_1 hook");

        if (GetFormByString::InstallHook())
            logger::debug("Installed GetFormByString hook");
        else
            logger::debug("Failed GetFormByString hook");

        if (InitItem::InstallHook())
            logger::debug("Installed InitItem hook");
        else
            logger::debug("Failed InitItem hook");

        if (FUN_141445d20::InstallHook())
            logger::debug("Installed FUN_141445d20 hook");
        else
            logger::debug("Failed FUN_141445d20 hook");

        if (ConstructObject::InstallHook())
            logger::debug("Installed ConstructObject hook");
        else
            logger::debug("Failed ConstructObject hook");

        if (ConstructObject_1::InstallHook())
            logger::debug("Installed ConstructObject_1 hook");
        else
            logger::debug("Failed ConstructObject_1 hook");

        if (ConstructObject_2::InstallHook())
            logger::debug("Installed ConstructObject_2 hook");
        else
            logger::debug("Failed ConstructObject_2 hook");
    }
}
