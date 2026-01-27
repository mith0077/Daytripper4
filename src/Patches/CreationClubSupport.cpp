#include "PCH.h"

#include "PatchLib.h"
#include "CreationClubLib.h"
#include "CreationClubSupport.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace CreationClubSupport
{
    namespace WriteOut
    {
        // XXX Status: unchecked
        static const uintptr_t target = ID(420121).address();
        static const auto patch_begin = target + 0x1BD;
        static const auto patch_end = target + 0x1C2;

        static bool IsDLCMasterOrCCFile(const char* a_filename)
        {
            static const uintptr_t IsDLCMaster_addr = ID(1088335).address();
            static const auto IsDLCMaster = reinterpret_cast<bool (*)(const char*)>(IsDLCMaster_addr);

            logger::debug("Entering IsDLCMasterOrCCFile()");

            auto pCcc = CreationClubContents::GetInstance();
            assert(pCcc);

            return IsDLCMaster(a_filename) || pCcc->IsCreationClubFile(a_filename);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(IsDLCMasterOrCCFile));
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

    namespace SortAndLoadPluginsFromPluginsFile
    {
        // XXX Status: verified
        static const uintptr_t target = ID(196618).address();
        static const auto patch_begin = target + 0x238;
        static const auto patch_end = target + 0x23F;

        static void FilterCCFiles(BSScrapArray<TESFile*>* a_pFilesAccu, bool a_initialized)
        {
            logger::debug("Entering FilterCCFiles()");

            assert(a_pFilesAccu);

            auto pDataHandler = TESDataHandler::GetSingleton(false);
            assert(pDataHandler);

            auto pCcc = CreationClubContents::GetInstance();
            assert(pCcc);

            for (auto sCcc : *pCcc) {
                for (auto pFile : pDataHandler->files) {
                    if (!pFile)
                        continue;
                    if (!(pFile->flags & TESFile::RecordFlag::kMaster))
                        continue;
                    if (0 != _stricmp(pFile->filename, sCcc.c_str()))
                        continue;

                    if (!a_initialized)
                        pFile->flags |= TESFile::RecordFlag::kChecked;

                    a_pFilesAccu->push_back(pFile);

                    break;
                }
            }
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                lea(rcx, ptr[rbp - 0x39]);
                mov(edx, r12d);
                mov(rax, reinterpret_cast<uintptr_t>(FilterCCFiles));
                call(rax);
                mov(rdx, ptr[rbp - 0x11]);
                mov(ecx, ptr[rbp - 0x1]);
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

    void InstallHook()
    {
        if (WriteOut::InstallHook())
            logger::debug("Installed WriteOut hook");
        else
            logger::debug("Failed WriteOut hook");

        if (SortAndLoadPluginsFromPluginsFile::InstallHook())
            logger::debug("Installed SortAndLoadPluginsFromPluginsFile hook");
        else
            logger::debug("Failed SortAndLoadPluginsFromPluginsFile hook");
    }
}
