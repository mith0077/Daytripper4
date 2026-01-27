#include "PCH.h"

#include "PatchLib.h"
#include "SmallFileLib.h"
#include "GetSmallFileHook.h"

using namespace std;
using namespace RE;
using namespace REL;


// Hooks that have references to GetSmallFile(Fallout4.exe+0x140131f80)
namespace SmallFileLoader::GetSmallFileHook
{
    static const uintptr_t GetFile_addr = ID(1376557).address();
    static const auto GetFile = reinterpret_cast<TESFile* (*)(TESForm*, uint32_t)>(GetFile_addr);

    static const uintptr_t BSsprintf_addr = ID(320670).address();
    static const auto BSsprintf = reinterpret_cast<void (*)(char*, uint64_t, const char*, ...)>(BSsprintf_addr);

    static void SetCreatedNIFFile(TESForm* a_pForm, char* a_pBuffer, uint64_t a_bufferLen)
    {
        // logger::debug("Entering SetCreatedNIFFile()");

        static const uintptr_t strFmtScolNIF_addr = Offset(0x2CA8240).address();
        static const auto strFmtScolNIF = reinterpret_cast<char*>(strFmtScolNIF_addr);

        static const uintptr_t DAT_142c8fe30_addr = ID(24376).address();
        static const auto DAT_142c8fe30 = reinterpret_cast<char*>(DAT_142c8fe30_addr);

        assert(a_pForm);
        assert(a_pBuffer);

        auto pFile = GetFile(a_pForm, 0);
        auto maskedFormId = GetMaskedFormId(a_pForm, pFile);

        const char* filename;

        if (pFile)
            filename = pFile->filename;
        else
            filename = DAT_142c8fe30;

        BSsprintf(a_pBuffer, a_bufferLen, strFmtScolNIF, filename, maskedFormId);
    }

    namespace FUN_1402e2c20
    {
        // XXX Status: noctd
        static const uintptr_t target = Offset(0x2E2C20).address();
        static const auto patch_begin = target + 0x5A;
        static const auto patch_end = target + 0x98;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, r14);
                lea(rdx, ptr[rsp + 0x30]);
                mov(r8d, 0x104);
                mov(rax, reinterpret_cast<uintptr_t>(SetCreatedNIFFile));
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

    namespace BuildCreatedNIFName
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0x2E4620).address();
        static const auto patch_begin = target + 0x14;
        static const auto patch_end = target + 0x5F;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(SetCreatedNIFFile));
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

    namespace GetGroupBlockKey
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1299873).address();
        static const auto patch_begin = target + 0x2A;
        static const auto patch_end = target + 0x4B;

        static uint32_t GetGroupBlockByForm(TESForm* a_pForm)
        {
            // logger::debug("Entering GetGroupBlockByForm()");

            assert(a_pForm);

            return GetMaskedFormId(a_pForm) % 10;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetGroupBlockByForm));
                sub(rsp, 0x28);  // Allocate shadow store with padding
                call(rax);
                add(rsp, 0x28);
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

    namespace GetGroupSubBlockKey
    {
        // XXX Status: verified
        static const uintptr_t target = ID(501879).address();
        static const auto patch_begin = target + 0x2A;
        static const auto patch_end = target + 0x4E;

        static uint32_t GetGroupSubBlockByForm(TESForm* a_pForm)
        {
            // logger::debug("Entering GetGroupSubBlockByForm()");

            assert(a_pForm);

            return (GetMaskedFormId(a_pForm) % 100) / 10;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetGroupSubBlockByForm));
                sub(rsp, 0x28);  // Allocate shadow store with padding
                call(rax);
                add(rsp, 0x28);
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

    namespace GetFaceCustomizationTextureFileName
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0x5A02D0).address();
        static const auto patch_begin = target + 0x5C;
        static const auto patch_end = target + 0xAE;

        static bool SetFaceCustomizationFilename(TESForm* a_pForm, char* a_pBuffer, uint64_t a_bufferLen, int a_textureIndex)
        {
            logger::debug("Entering SetFaceCustomizationFilename()");

            static const uintptr_t strFmtFaceCustomizationD_addr = Offset(0x2CEEE20).address();
            static const auto strFmtFaceCustomizationD = reinterpret_cast<char*>(strFmtFaceCustomizationD_addr);

            static const uintptr_t strFmtFaceCustomizationMsn_addr = Offset(0x2CEEE60).address();
            static const auto strFmtFaceCustomizationMsn = reinterpret_cast<char*>(strFmtFaceCustomizationMsn_addr);

            static const uintptr_t strFmtFaceCustomizationS_addr = Offset(0x2CEEEA8).address();
            static const auto strFmtFaceCustomizationS = reinterpret_cast<char*>(strFmtFaceCustomizationS_addr);

            assert(a_pForm);
            assert(a_pBuffer);

            auto pFile = GetFile(a_pForm, 0);

            if (!pFile)
                return false;

            auto maskedFormId = GetMaskedFormId(a_pForm, pFile);
            auto filename = pFile->filename;

            const char* strFmtFaceCustomization = nullptr;

            switch(a_textureIndex) {
                case 0:
                    strFmtFaceCustomization = strFmtFaceCustomizationD;

                    break;
                case 1:
                    strFmtFaceCustomization = strFmtFaceCustomizationMsn;

                    break;
                case 7:
                    strFmtFaceCustomization = strFmtFaceCustomizationS;

                    break;
            }

            if (strFmtFaceCustomization)
                BSsprintf(a_pBuffer, a_bufferLen, strFmtFaceCustomization, filename, maskedFormId);

            return true;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rbx);
                mov(rdx, rbp);
                mov(r8, rsi);
                mov(r9d, edi);
                mov(rax, reinterpret_cast<uintptr_t>(SetFaceCustomizationFilename));
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

    namespace GetFaceGenGeomFileName
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(1135980).address();
        static const auto patch_begin = target + 0x4c;
        static const auto patch_end = target + 0x94;

        static bool SetFaceGeomPath(TESForm* a_pForm, char* a_pBuffer)
        {
            logger::debug("Entering SetFaceGeomPath()");

            static const uint64_t bufferLen = 0x104;

            static const uintptr_t strFmtFaceGeomData_addr = Offset(0x2CEEEE8).address();
            static const auto strFmtFaceGeomData = reinterpret_cast<char*>(strFmtFaceGeomData_addr);

            assert(a_pForm);
            assert(a_pBuffer);

            auto pFile = GetFile(a_pForm, 0);

            if (!pFile)
                return false;

            auto maskedFormId = GetMaskedFormId(a_pForm, pFile);
            auto filename = pFile->filename;

            BSsprintf(a_pBuffer, bufferLen, strFmtFaceGeomData, filename, maskedFormId);

            return true;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rbx);
                mov(rdx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(SetFaceGeomPath));
                call(rax);
                mov(rbx, ptr[rsp + 0x40]);
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

    namespace FUN_140681a80
    {
        // XXX Status: noctd
        static const uintptr_t target = Offset(0x681A80).address();
        static const auto patch_begin = target + 0x258;
        static const auto patch_end = target + 0x323;

        static void SetFaceCustomizationPath(TESFile* a_pFile, TESFormID a_formId, char* a_pBuffer, void* a_pUnk)
        {
            logger::debug("Entering SetFaceCustomizationPath()");

            static const uint64_t bufferLen = 0x104;

            static const uintptr_t strFmtFaceCustomizationD_addr = Offset(0x2D1FE08).address();
            static const auto strFmtFaceCustomizationD = reinterpret_cast<char*>(strFmtFaceCustomizationD_addr);

            static const uintptr_t strFmtFaceCustomizationMsn_addr = Offset(0x2D1FE50).address();
            static const auto strFmtFaceCustomizationMsn = reinterpret_cast<char*>(strFmtFaceCustomizationMsn_addr);

            static const uintptr_t strFmtFaceCustomizationS_addr = Offset(0x2D1FE98).address();
            static const auto strFmtFaceCustomizationS = reinterpret_cast<char*>(strFmtFaceCustomizationS_addr);

            assert(a_pFile);
            assert(a_pBuffer);
            assert(a_pUnk);

            const auto unkVTable_ptr = *reinterpret_cast<void**>(a_pUnk);
            const auto unkVTable_addr = reinterpret_cast<uintptr_t>(unkVTable_ptr);
            const auto unkVFunPtr_addr = unkVTable_addr + 0x170;
            const auto unkVFun = *reinterpret_cast<void (**)(void*, uint32_t, char*)>(unkVFunPtr_addr);

            auto maskedFormId = a_formId;

            if (IsSmallFile(a_pFile))
                maskedFormId &= 0xFFF;
            else
                maskedFormId &= 0xFFFFFF;

            auto filename = a_pFile->filename;

            BSsprintf(a_pBuffer, bufferLen, strFmtFaceCustomizationD, filename, maskedFormId);
            unkVFun(a_pUnk, 0, a_pBuffer);

            BSsprintf(a_pBuffer, bufferLen, strFmtFaceCustomizationMsn, filename, maskedFormId);
            unkVFun(a_pUnk, 1, a_pBuffer);

            BSsprintf(a_pBuffer, bufferLen, strFmtFaceCustomizationS, filename, maskedFormId);
            unkVFun(a_pUnk, 7, a_pBuffer);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rax);
                mov(rax, ptr[rsi]);
                mov(edx, ptr[rax + 0x14]);
                lea(r8, ptr[rsp + 0x30]);
                mov(r9, ptr[rdi + 0x68]);
                mov(rax, reinterpret_cast<uintptr_t>(SetFaceCustomizationPath));
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

    namespace GetTextReplaceFormID
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0xB8F150).address();
        static const auto patch_begin = target + 0x9;
        static const auto patch_end = target + 0x50;

        static void GetTextReplaceFormID_(HolotapeMenu* a_pHolotapeMenu, char* a_textReplacement, Scaleform::GFx::Value* a_pGfxValue)
        {
            logger::debug("Entering GetTextReplaceFormID_()");

            static const uintptr_t GetTextReplaceForm_addr = ID(1054073).address();
            static const auto GetTextReplaceForm = reinterpret_cast<TESForm* (*)(HolotapeMenu*, char*)>(GetTextReplaceForm_addr);

            assert(a_pHolotapeMenu);
            assert(a_textReplacement);
            assert(a_pGfxValue);

            auto pForm = GetTextReplaceForm(a_pHolotapeMenu, a_textReplacement);

            if (!pForm)
                return;

            auto maskedFormId = GetMaskedFormId(pForm);

            *a_pGfxValue = Scaleform::GFx::Value(maskedFormId);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetTextReplaceFormID_));
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

    namespace GetAudioFilename
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x5FCE20).address();
        static const auto patch_begin = target + 0x7B;
        static const auto patch_end = target + 0x84;

        static TESFormID GetMaskedFormId_(TESForm* a_pForm)
        {
            // logger::debug("Entering GetMaskedFormId_()");

            return GetMaskedFormId(a_pForm);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(GetMaskedFormId_));
                call(rax);
                mov(ebx, eax);
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
        if (FUN_1402e2c20::InstallHook())
            logger::debug("Installed FUN_1402e2c20 hook");
        else
            logger::debug("Failed FUN_1402e2c20 hook");

        if (BuildCreatedNIFName::InstallHook())
            logger::debug("Installed BuildCreatedNIFName hook");
        else
            logger::debug("Failed BuildCreatedNIFName hook");

        if (GetGroupBlockKey::InstallHook())
            logger::debug("Installed GetGroupBlockKey hook");
        else
            logger::debug("Failed GetGroupBlockKey hook");

        if (GetGroupSubBlockKey::InstallHook())
            logger::debug("Installed GetGroupSubBlockKey hook");
        else
            logger::debug("Failed GetGroupSubBlockKey hook");

        if (GetFaceCustomizationTextureFileName::InstallHook())
            logger::debug("Installed GetFaceCustomizationTextureFileName hook");
        else
            logger::debug("Failed GetFaceCustomizationTextureFileName hook");

        if (GetFaceGenGeomFileName::InstallHook())
            logger::debug("Installed GetFaceGenGeomFileName hook");
        else
            logger::debug("Failed GetFaceGenGeomFileName hook");

        if (FUN_140681a80::InstallHook())
            logger::debug("Installed FUN_140681a80 hook");
        else
            logger::debug("Failed FUN_140681a80 hook");

        if (GetTextReplaceFormID::InstallHook())
            logger::debug("Installed GetTextReplaceFormID hook");
        else
            logger::debug("Failed GetTextReplaceFormID hook");

        if (GetAudioFilename::InstallHook())
            logger::debug("Installed GetAudioFilename hook");
        else
            logger::debug("Failed GetAudioFilename hook");
    }
}
