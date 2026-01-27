#include "PCH.h"

#include "PatchLib.h"
#include "EslExtensionSupport.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace EslExtensionSupport
{
    namespace LoadTESInfo
    {
        // XXX Status: verified
        static const uintptr_t target = ID(883999).address();
        static const auto patch_begin = target + 0x56D;
        static const auto patch_end = target + 0x5A0;

        static void SetFileFlagByExtension(TESFile* a_pFile)
        {
            // logger::debug("Entering SetFileFlagByExtension()");

            assert(a_pFile);

            auto strName = a_pFile->filename;

            if (!strName)
                return;

            auto lenName = strlen(strName);

            if (lenName < 3)
                return;

            auto strExt = strName + lenName - 3;

            auto isEsm = 0 == _stricmp(strExt, "esm");
            auto isEsl = 0 == _stricmp(strExt, "esl");

            if (isEsm || isEsl)
                a_pFile->flags |= TESFile::RecordFlag::kMaster;
            if (isEsl)
                a_pFile->flags |= TESFile::RecordFlag::kSmallFile;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(SetFileFlagByExtension));
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

    namespace FindAndInsertFiles
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1258854).address();
        static const auto patch_begin = target + 0xDA;
        static const auto patch_end = target + 0x11B;

        static int CheckExtension(const char* a_pExtension)
        {
            // logger::debug("Entering FindAndInsertFiles::CheckExtension()");

            assert(a_pExtension);

            if (0 == _stricmp(a_pExtension, "esm"))
                return 0;
            if (0 == _stricmp(a_pExtension, "esp"))
                return 0;

            return _stricmp(a_pExtension, "esl");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
                call(rax);
                test(eax, eax);
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

    namespace GetPluginFilesFromManifest
    {
        // XXX Status: unchecked
        static const uintptr_t target = ID(72534).address();
        static const auto patch_begin = target + 0x1D8;
        static const auto patch_end = target + 0x1F2;

        static int CheckExtension(const char* a_pExtension)
        {
            logger::debug("Entering GetPluginFilesFromManifest::CheckExtension()");

            assert(a_pExtension);

            if (0 == _stricmp(a_pExtension, ".esm"))
                return 0;
            if (0 == _stricmp(a_pExtension, ".esp"))
                return 0;

            return _stricmp(a_pExtension, ".esl");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
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

    namespace DeleteFilesUsingManifest
    {
        // XXX Status: unchecked
        static const uintptr_t target = ID(922377).address();
        static const auto patch_begin = target + 0x206;
        static const auto patch_end = target + 0x234;

        static int CheckExtension(const char* a_pExtension)
        {
            logger::debug("Entering DeleteFilesUsingManifest::CheckExtension()");

            assert(a_pExtension);

            if (0 == _stricmp(a_pExtension, ".esm"))
                return 0;
            if (0 == _stricmp(a_pExtension, ".esp"))
                return 0;
            if (0 == _stricmp(a_pExtension, ".esl"))
                return 0;

            return _stricmp(a_pExtension, ".ba2");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
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

    namespace OpenMasterArchives
    {
        // XXX Status: unchecked
        static const uintptr_t target = Offset(0x1BD9CD0).address();
        static const auto patch_begin = target + 0x1D6;
        static const auto patch_end = target + 0x1F3;

        static const char* CheckExtension(const char* a_pFileName)
        {
            logger::debug("Entering OpenMasterArchives::CheckExtension()");

            assert(a_pFileName);

            if (0 == strstr(a_pFileName, ".esp"))
                return nullptr;
            if (0 == strstr(a_pFileName, ".esl"))
                return nullptr;

            return strstr(a_pFileName, ".esm");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
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

    namespace RegisterBaseArchivesForFile
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1279060).address();
        static const auto patch_begin = target + 0x60;
        static const auto patch_end = target + 0x88;

        static int CheckExtension(const char* a_pExtension)
        {
            // logger::debug("Entering RegisterBaseArchivesForFile::CheckExtension()");

            assert(a_pExtension);

            if (0 == _stricmp(a_pExtension, ".ESP"))
                return 0;
            if (0 == _stricmp(a_pExtension, ".ESM"))
                return 0;

            return _stricmp(a_pExtension, ".ESL");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
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

    namespace RegisterAdditionalArchivesForFile
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x124EA0).address();
        static const auto patch_begin = target + 0x72;
        static const auto patch_end = target + 0x9A;

        static int CheckExtension(const char* a_pExtension)
        {
            logger::debug("Entering RegisterAdditionalArchivesForFile::CheckExtension()");

            assert(a_pExtension);

            if (0 == _stricmp(a_pExtension, ".ESP"))
                return 0;
            if (0 == _stricmp(a_pExtension, ".ESM"))
                return 0;

            return _stricmp(a_pExtension, ".ESL");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
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

    namespace FUN_140b76c10
    {
        // XXX Status: unchecked
        static const uintptr_t target = ID(144913).address();
        static const auto patch_begin = target + 0x16B;
        static const auto patch_end = target + 0x185;

        static int CheckExtension(const char* a_pExtension)
        {
            logger::debug("Entering FUN_140b76c10::CheckExtension()");

            assert(a_pExtension);

            if (0 == _stricmp(a_pExtension, ".ESM"))
                return 0;
            if (0 == _stricmp(a_pExtension, ".ESP"))
                return 0;

            return _stricmp(a_pExtension, ".ESL");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
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

    namespace FUN_140d8d7e0
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(814359).address();
        static const auto patch_begin = target + 0x2E;
        static const auto patch_end = target + 0x49;

        static const char* CheckExtension(const char* a_pExtension)
        {
            static const uintptr_t BSstristr_addr = ID(722016).address();
            static const auto BSstristr = reinterpret_cast<const char* (*)(const char*, const char*)>(BSstristr_addr);

            // logger::debug("Entering FUN_140d8d7e0::CheckExtension()");

            assert(a_pExtension);

            const char* found;

            found = BSstristr(a_pExtension, ".ESP");

            if (found)
                return found;

            found = BSstristr(a_pExtension, ".ESL");

            if (found)
                return found;

            return BSstristr(a_pExtension, ".ESM");
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CheckExtension));
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
        if (LoadTESInfo::InstallHook())
            logger::debug("Installed LoadTESInfo hook");
        else
            logger::debug("Failed LoadTESInfo hook");

        if (FindAndInsertFiles::InstallHook())
            logger::debug("Installed FindAndInsertFiles hook");
        else
            logger::debug("Failed FindAndInsertFiles hook");

        if (GetPluginFilesFromManifest::InstallHook())
            logger::debug("Installed GetPluginFilesFromManifest hook");
        else
            logger::debug("Failed GetPluginFilesFromManifest hook");

        if (DeleteFilesUsingManifest::InstallHook())
            logger::debug("Installed DeleteFilesUsingManifest hook");
        else
            logger::debug("Failed DeleteFilesUsingManifest hook");

        if (OpenMasterArchives::InstallHook())
            logger::debug("Installed OpenMasterArchives hook");
        else
            logger::debug("Failed OpenMasterArchives hook");

        if (RegisterBaseArchivesForFile::InstallHook())
            logger::debug("Installed RegisterBaseArchivesForFile hook");
        else
            logger::debug("Failed RegisterBaseArchivesForFile hook");

        if (RegisterAdditionalArchivesForFile::InstallHook())
            logger::debug("Installed RegisterAdditionalArchivesForFile hook");
        else
            logger::debug("Failed RegisterAdditionalArchivesForFile hook");

        if (FUN_140b76c10::InstallHook())
            logger::debug("Installed FUN_140b76c10 hook");
        else
            logger::debug("Failed FUN_140b76c10 hook");

        if (FUN_140d8d7e0::InstallHook())
            logger::debug("Installed FUN_140d8d7e0 hook");
        else
            logger::debug("Failed FUN_140d8d7e0 hook");
    }
}
