#include "PCH.h"

#include "PatchLib.h"
#include "SmallFileLib.h"
#include "DataHandlerDef.h"
#include "SetCompileIndexHook.h"

using namespace std;
using namespace RE;
using namespace REL;


// Hooks that have references to SetCompileIndex(Fallout4.exe+0x1401304E0)
namespace SmallFileLoader::SetCompileIndexHook
{
    static void SetCompileIndex(TESFile* a_pFile, uint8_t a_compileIndex)
    {
        logger::debug("Entering SetCompileIndex()");

        static const uintptr_t DAT_145935480_addr = Offset(0X5935480).address();
        static const auto DAT_145935480 = reinterpret_cast<void**>(DAT_145935480_addr);
        static const uintptr_t SetPageCache_addr = Offset(0x1BD5530).address();
        static const auto SetPageCache = reinterpret_cast<void (*)(BSFile*, void*, uint8_t)>(SetPageCache_addr);

        assert(a_pFile);

        a_pFile->fileHeaderInfo.nextFormID &= 0x00FFFFFF;
        a_pFile->compileIndex = a_compileIndex;
        a_pFile->fileHeaderInfo.nextFormID |= (a_compileIndex << 0x18);

        if (a_pFile->file && DAT_145935480 &&  0xFE != a_compileIndex)
            SetPageCache(a_pFile->file, *DAT_145935480, a_compileIndex);
    }

    static void AddCompiledFileToHandler(TESDataHandler* a_pDataHandler, TESFile* a_pFile)
    {
        logger::debug("Entering AddCompiledFileToHandler()");

        assert(a_pDataHandler);
        assert(a_pFile);

        if (IsInCollection(a_pDataHandler, a_pFile))
            ;
        else if (IsSmallFile(a_pFile)) {
            auto smallFileCompleIndex = AddSmallFile(a_pDataHandler, a_pFile);

            if (0xFFFF != smallFileCompleIndex) {
                logger::info("Added small file: {}, index: fe{:03x}", a_pFile->filename, smallFileCompleIndex);

                SetCompileIndex(a_pFile, 0xFE);
                a_pFile->smallFileCompileIndex = static_cast<uint16_t>(smallFileCompleIndex);
            }
        }
        else {
            auto compileIndex = AddNormalFile(a_pDataHandler, a_pFile);

            if (0xFF != compileIndex) {
                logger::info("Added normal file: {}, index: {:02x}", a_pFile->filename, compileIndex);

                SetCompileIndex(a_pFile, compileIndex);
                a_pFile->smallFileCompileIndex = 0;
            }
        }
    }

    namespace AddCompiledFile
    {
        // XXX Status: verified
        static const uintptr_t target = ID(118217).address();
        static const auto patch_begin = target + 0x6;
        static const auto patch_end = target + 0x4D;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(AddCompiledFileToHandler));
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

    static void AddCompiledAndRegisterAdditionalArchivesForFile(TESDataHandler* a_pDataHandler, TESFile* a_pFile)
    {
        logger::debug("Entering AddCompiledAndRegisterAdditionalArchivesForFile()");

        static const uintptr_t RegisterAdditionalArchivesForFile_addr = Offset(0x124EA0).address();
        static const auto RegisterAdditionalArchivesForFile = reinterpret_cast<void (*)(char*, uint32_t)>(RegisterAdditionalArchivesForFile_addr);

        assert(a_pDataHandler);
        assert(a_pFile);

        AddCompiledFileToHandler(a_pDataHandler, a_pFile);
        RegisterAdditionalArchivesForFile(a_pFile->filename, a_pFile->compileIndex);
    }

    namespace CompileFiles
    {
        // XXX Status: verified
        static const uintptr_t target = ID(57137).address();
        static const auto erase_begin = target + 0x346;
        static const auto erase_end = target + 0x355;
        static const auto patch_begin = target + 0x387;
        static const auto patch_end = target + 0x3AA;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(rdx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(AddCompiledAndRegisterAdditionalArchivesForFile));
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

    namespace CompileFiles_1
    {
        // XXX Status: verified
        static const uintptr_t target = ID(57137).address();
        static const auto patch_begin = target + 0x3CD;
        static const auto patch_end = target + 0x3F4;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(rdx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(AddCompiledAndRegisterAdditionalArchivesForFile));
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

    namespace CompileFiles_2
    {
        // XXX Status: verified
        static const uintptr_t target = ID(57137).address();
        static const auto patch_begin = target + 0x410;
        static const auto patch_end = target + 0x448;

        static void OpenTESForFiles(TESDataHandler* a_pDataHandler)
        {
            logger::debug("Entering OpenTESForFiles()");

            static const uintptr_t OpenTES_addr = ID(993864).address();
            static const auto OpenTES = reinterpret_cast<bool (*)(TESFile*, uint32_t, bool)>(OpenTES_addr);

            static const uintptr_t DAT_145935170_addr = Offset(0x5935170).address();
            static const auto pDAT_145935170 = reinterpret_cast<uint32_t*>(DAT_145935170_addr);

            assert(a_pDataHandler);

            auto normalFileCount = NormalFileCount(a_pDataHandler);
            for (decltype(normalFileCount) i = 0; i < normalFileCount; ++i) {
                auto pFile = GetNormalFileAt(a_pDataHandler, i);

                if (OpenTES(pFile, 0, false))
                    *pDAT_145935170 += pFile->fileHeaderInfo.formCount;
            }

            auto smallFileCount = SmallFileCount(a_pDataHandler);
            for (decltype(smallFileCount) i = 0; i < smallFileCount; ++i) {
                auto pFile = GetSmallFileAt(a_pDataHandler, i);

                if (OpenTES(pFile, 0, false))
                    *pDAT_145935170 += pFile->fileHeaderInfo.formCount;
            }
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(OpenTESForFiles));
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

    namespace CompileFiles_3
    {
        // XXX Status: verified
        static const uintptr_t target = ID(57137).address();
        static const auto patch_begin = target + 0x475;
        static const auto patch_end = target + 0x4A6;

        static void ConstructObjectListForFiles(TESDataHandler* a_pDataHandler)
        {
            logger::debug("Entering ConstructObjectListForFiles()");

            static const uintptr_t ConstructObjectList_addr = Offset(0x120350).address();
            static const auto ConstructObjectList = reinterpret_cast<bool (*)(TESDataHandler*, TESFile*, bool)>(ConstructObjectList_addr);

            assert(a_pDataHandler);

            bool constructAny = false;

            for (TESFile* pFile : a_pDataHandler->files) {
                if (!pFile)
                    break;
                if (0xFF == pFile->compileIndex)
                    continue;
                if (pFile == a_pDataHandler->activeFile)
                    continue;

                ConstructObjectList(a_pDataHandler, pFile, !constructAny);
                constructAny = true;
            }

            if (a_pDataHandler->activeFile) {
                ConstructObjectList(a_pDataHandler, a_pDataHandler->activeFile, !constructAny);
                constructAny = true;
            }
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(ConstructObjectListForFiles));
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

    namespace HotLoadPlugin
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0x123490).address();
        static const auto patch_begin = target + 0x9D7;
        static const auto patch_end = target + 0x9F4;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rsi);
                mov(rdx, r13);
                mov(rax, reinterpret_cast<uintptr_t>(AddCompiledFileToHandler));
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
        if (AddCompiledFile::InstallHook())
            logger::debug("Installed AddCompiledFile hook");
        else
            logger::debug("Failed AddCompiledFile hook");

        if (CompileFiles::InstallHook())
            logger::debug("Installed CompileFiles hook");
        else
            logger::debug("Failed CompileFiles hook");

        if (CompileFiles_1::InstallHook())
            logger::debug("Installed CompileFiles_1 hook");
        else
            logger::debug("Failed CompileFiles_1 hook");

        if (CompileFiles_2::InstallHook())
            logger::debug("Installed CompileFiles_2 hook");
        else
            logger::debug("Failed CompileFiles_2 hook");

        if (CompileFiles_3::InstallHook())
            logger::debug("Installed CompileFiles_3 hook");
        else
            logger::debug("Failed CompileFiles_3 hook");

        if (HotLoadPlugin::InstallHook())
            logger::debug("Installed HotLoadPlugin hook");
        else
            logger::debug("Failed HotLoadPlugin hook");
    }
}
