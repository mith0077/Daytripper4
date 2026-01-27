#include "PCH.h"

#include "PatchLib.h"
#include "DataHandlerDef.h"
#include "SaveLoadGameDef.h"
#include "SaveLoadGameHook.h"

using namespace std;
using namespace RE;
using namespace REL;


// Hooks for save data handler to support small files
namespace SmallFileLoader::SaveLoadGameHook
{
    namespace BGSSaveLoadGameCtor
    {
        // XXX Status: verified
        static const uintptr_t target = ID(337678).address();
        static const auto patch_begin = target + 0x164;
        static const auto patch_end = target + 0x169;

        static void ConstructNormalAndSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame)
        {
            logger::debug("Entering ConstructNormalAndSmallFiles()");

            assert(a_pSaveLoadGame);

            ConstructNormalFiles(a_pSaveLoadGame);
            ConstructSmallFiles(a_pSaveLoadGame);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(ConstructNormalAndSmallFiles));
                call(rax);
                mov(rbx, ptr[rsp + 0x30]);
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

    namespace BGSSaveLoadGameDtor
    {
        // XXX Status: noctd
        static const uintptr_t target = Offset(0xD25420).address();
        static const auto patch_begin = target + 0x11A;
        static const auto patch_end = target + 0x11F;

        static const uintptr_t BGSSaveLoadFormIDMapDtor_addr = Offset(0xD2BC10).address();

        static void DestructNormalAndSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame)
        {
            logger::debug("Entering DestructNormalAndSmallFiles()");

            assert(a_pSaveLoadGame);

            DestructSmallFiles(a_pSaveLoadGame);
            DestructNormalFiles(a_pSaveLoadGame);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, BGSSaveLoadFormIDMapDtor_addr);
                call(rax);
                mov(rcx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(DestructNormalAndSmallFiles));
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

    namespace Load
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xD24DA0).address();
        static const auto patch_begin = target + 0x85;
        static const auto patch_end = target + 0xB1;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetSaveLoadGame));
                call(rax);
                mov(rcx, rax);
                mov(rax, reinterpret_cast<uintptr_t>(GetConvertedFormId));
                call(rax);
                mov(edx, eax);
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

    namespace LoadPluginList
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xD28F30).address();
        static const auto patch_begin = target + 0x3E;
        static const auto patch_end = target + 0x1AB;

        static uint32_t GetFormatVersion(BGSLoadGameBuffer* a_pLoadGameBuffer)
        {
            logger::debug("Entering GetFormatVersion()");

            assert(a_pLoadGameBuffer);

            void* vtable = *reinterpret_cast<void**>(a_pLoadGameBuffer);
            uintptr_t GetFormatVersionVFun_addr = reinterpret_cast<uintptr_t>(vtable) + 0x8;
            auto GetFormatVersionVFun = *reinterpret_cast<uint32_t (**)(BGSLoadGameBuffer*)>(GetFormatVersionVFun_addr);

            return GetFormatVersionVFun(a_pLoadGameBuffer);
        }

        static bool LoadPluginListSubr(BGSSaveLoadGame* a_pSaveLoadGame, BGSLoadGameBuffer* a_pLoadGameBuffer, BSScrapArray<BSFixedString>* a_pScrapArray)
        {
            logger::debug("Entering LoadPluginListSubr()");
            logger::info("*** Start reading plugin list from savedata ***");

            static const uintptr_t GetListFile_addr = ID(93802).address();
            static const auto GetListFile = reinterpret_cast<TESFile* (*)(TESDataHandler*, const char*)>(GetListFile_addr);

            static const uintptr_t LoadDataEndian_addr = ID(1451631).address();
            static const auto LoadDataEndian = reinterpret_cast<void (*)(BGSLoadGameBuffer*, void*, uint32_t)>(LoadDataEndian_addr);

            static const uintptr_t LoadStringA_addr = ID(947002).address();
            static const auto LoadStringA = reinterpret_cast<void (*)(BGSLoadGameBuffer*, char*)>(LoadStringA_addr);

            static const uintptr_t SaveGameWarning_addr = ID(415396).address();
            static const auto SaveGameWarning = reinterpret_cast<void (*)(const char*, const char*)>(SaveGameWarning_addr);

            static const auto strSaveGameErrorFmt = reinterpret_cast<const char*>(Offset(0x2D73980).address());

            assert(a_pSaveLoadGame);
            assert(a_pScrapArray);

            bool result = true;
            char buffer[272];

            CleanUpNormalFiles(a_pSaveLoadGame);
            CleanUpSmallFiles(a_pSaveLoadGame);

            uint8_t normalFileCount = 0;
            LoadDataEndian(a_pLoadGameBuffer, &normalFileCount, 1);

            decltype(normalFileCount) lastCompileIndex = 0xFF;
            for (decltype(normalFileCount) i = 0; i < normalFileCount; ++i) {
                LoadStringA(a_pLoadGameBuffer, buffer);
                auto pFile = GetListFile(GetDataHandler(), buffer);

                if (pFile && 0xFE == pFile->compileIndex)
                    logger::info("Binding normal save plugin to small load order: {:02x} -> fe{:03x} ({})", NormalFileCount(a_pSaveLoadGame), pFile->smallFileCompileIndex, buffer);
                else if (pFile && 0xFF != pFile->compileIndex) {
                    lastCompileIndex = pFile->compileIndex;

                    logger::info("Binding normal save plugin to normal load order: {:02x} -> {:02x} ({})", NormalFileCount(a_pSaveLoadGame), pFile->compileIndex, buffer);
                }
                // HACK: compileIndex of Fallout4_VR.esm become 0xFF somehow
                else if (pFile && 0 == _stricmp("Fallout4_VR.esm", buffer))  {
                    pFile->compileIndex = ++lastCompileIndex;

                    logger::info("Binding normal save plugin to normal load order: {:02x} -> {:02x} ({})", NormalFileCount(a_pSaveLoadGame), pFile->compileIndex, buffer);
                }
                else {
                    pFile = nullptr;
                    result = false;

                    SaveGameWarning(strSaveGameErrorFmt, buffer);
                    a_pScrapArray->push_back(BSFixedString(buffer));

                    logger::info("Failed to bind normal save plugin: {:02x} ({})", NormalFileCount(a_pSaveLoadGame), buffer);
                }

                AddNormalFile(a_pSaveLoadGame, pFile);
            }

            auto formatVersion = GetFormatVersion(a_pLoadGameBuffer);

            logger::info("Detected save data sub-version: {} (>=68 supports small files)", formatVersion);

            if (formatVersion <= 0x43)
                return result;

            uint16_t smallFileCount = 0;
            LoadDataEndian(a_pLoadGameBuffer, &smallFileCount, 2);

            for (decltype(smallFileCount) i = 0; i < smallFileCount; ++i) {
                LoadStringA(a_pLoadGameBuffer, buffer);
                auto pFile = GetListFile(GetDataHandler(), buffer);

                if (pFile && 0xFE == pFile->compileIndex)
                    logger::info("Binding small save plugin to small load order: fe{:03x} -> fe{:03x} ({})", SmallFileCount(a_pSaveLoadGame), pFile->smallFileCompileIndex, buffer);
                else if (pFile && 0xFF != pFile->compileIndex)
                    logger::info("Binding small save plugin to normal load order: fe{:03x} -> {:02x} ({})", SmallFileCount(a_pSaveLoadGame), pFile->compileIndex, buffer);
                else {
                    pFile = nullptr;
                    result = false;

                    SaveGameWarning(strSaveGameErrorFmt, buffer);
                    a_pScrapArray->push_back(BSFixedString(buffer));

                    logger::info("Failed to bind small save plugin: fe{:03x} ({})", SmallFileCount(a_pSaveLoadGame), buffer);
                }

                AddSmallFile(a_pSaveLoadGame, pFile);
            }

            return result;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rbp);
                lea(rdx, ptr[rsp + 0x30]);
                mov(r8, r14);
                mov(rax, reinterpret_cast<uintptr_t>(LoadPluginListSubr));
                call(rax);
                mov(r15, rax);
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

    namespace GetVersionInfo
    {
        // XXX Status: verified
        static const uintptr_t target = ID(606257).address();

        inline bool InstallHook()
        {
            uint8_t newVersion = 0x44;

            safe_write(target, &newVersion, 1);

            return true;
        }
    }

    namespace AddChangedFormID
    {

    }

    void InstallHook()
    {
        if (BGSSaveLoadGameCtor::InstallHook())
            logger::debug("Installed BGSSaveLoadGameCtor hook");
        else
            logger::debug("Failed BGSSaveLoadGameCtor hook");

        if (BGSSaveLoadGameDtor::InstallHook())
            logger::debug("Installed BGSSaveLoadGameDtor hook");
        else
            logger::debug("Failed BGSSaveLoadGameDtor hook");

        if (Load::InstallHook())
            logger::debug("Installed Load hook");
        else
            logger::debug("Failed Load hook");

        if (LoadPluginList::InstallHook())
            logger::debug("Installed LoadPluginList hook");
        else
            logger::debug("Failed LoadPluginList hook");

        if (GetVersionInfo::InstallHook())
            logger::debug("Installed GetVersionInfo hook");
        else
            logger::debug("Failed GetVersionInfo hook");
    }
}
