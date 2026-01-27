#include "PCH.h"

#include "PatchLib.h"
#include "SmallFileLib.h"
#include "DataHandlerDef.h"
#include "GetFileAtHook.h"

using namespace std;
using namespace RE;
using namespace REL;


// Hooks that have references to GetFileAt(Fallout4VR.exe+0x140122A60)
namespace SmallFileLoader::GetFileAtHook
{
    static const uintptr_t GetFile_addr = ID(1376557).address();
    static const auto GetFile = reinterpret_cast<TESFile* (*)(TESForm*, uint32_t)>(GetFile_addr);

    namespace  LoadAllChildDataForAllQuests
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1214813).address();
        static const auto patch_begin = target + 0x1E5;
        static const auto patch_end = target + 0x24F;

        static void LoadAllQuestDataFromFile(TESFile* a_pFile, BSScrapArray<TESForm*>* a_pQuestArray, BSScrapArray<TESForm*>* a_pChildDataArray)
        {
            logger::debug("Entering LoadAllQuestDataFromFile()");

            static const uintptr_t LoadAllChildDataFromFile_addr = ID(520558).address();
            static const auto LoadAllChildDataFromFile = reinterpret_cast<void (*)(TESForm*, TESFile*, BSScrapArray<TESForm*>*)>(LoadAllChildDataFromFile_addr);

            for (TESForm* pQuest : *a_pQuestArray) {
                LoadAllChildDataFromFile(pQuest, a_pFile, a_pChildDataArray);

                auto pUnkShort_addr = reinterpret_cast<uintptr_t>(pQuest) + 0xF4;
                auto pUnkShort = reinterpret_cast<uint16_t*>(pUnkShort_addr);

                *pUnkShort |= 0x8000;
            }
        }

        static void LoadAllQuestDataFromDataHandler(TESDataHandler* a_pDataHandler, BSScrapArray<TESForm*>* a_pQuestArray, BSScrapArray<TESForm*>* a_pChildDataArray)
        {
            logger::debug("Entering LoadAllQuestDataFromDataHandler()");

            assert(a_pDataHandler);
            assert(a_pQuestArray);
            assert(a_pChildDataArray);

            for (TESFile* pFile : a_pDataHandler->files) {
                if (0xFF == pFile->compileIndex)
                    continue;
                if (a_pDataHandler->activeFile == pFile)
                    continue;

                LoadAllQuestDataFromFile(pFile, a_pQuestArray, a_pChildDataArray);
            }

            auto pActiveFile = a_pDataHandler->activeFile;

            if (pActiveFile)
                LoadAllQuestDataFromFile(pActiveFile, a_pQuestArray, a_pChildDataArray);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                lea(rdx, ptr[rbp - 0x40]);
                lea(r8, ptr[rbp - 0x20]);
                mov(rax, reinterpret_cast<uintptr_t>(LoadAllQuestDataFromDataHandler));
                call(rax);
                mov(ecx, dword[rbp - 0x28]);
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

  namespace WriteHeader
    {
        // NOTE: Fallout4VR.exe's implement should be sufficient,
        //       because Fallout4.exe's doesn't consider snallFiles neither.
        // static const uintptr_t target = Offset(0x2F1CD0).address();
    }

    namespace SavePluginList
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xD28E70).address();
        static const auto patch_begin = target + 0x23;
        static const auto patch_end = target + 0x88;

        static void SaveStringForFiles(TESDataHandler* a_pDataHandler, BGSSaveGameBuffer* a_pSaveGameBuffer)
        {
            logger::debug("Entering SaveStringForFiles()");

            static const uintptr_t ExpandBuffer_addr = ID(1443998).address();
            static const auto ExpandBuffer = reinterpret_cast<void (*)(BGSSaveGameBuffer*, uint32_t)>(ExpandBuffer_addr);

            static const uintptr_t SaveString_addr = ID(1339841).address();
            static const auto SaveString = reinterpret_cast<void (*)(BGSSaveGameBuffer*, const char*, uint32_t)>(SaveString_addr);

            assert(a_pDataHandler);
            assert(a_pSaveGameBuffer);

            {
                auto count = static_cast<uint8_t>(NormalFileCount(a_pDataHandler));
                auto countPos = a_pSaveGameBuffer->bufferPosition;
                auto countSize = 1;

                if (a_pSaveGameBuffer->size < countPos + countSize)
                    ExpandBuffer(a_pSaveGameBuffer, countPos + countSize);

                auto pCountBuf_addr = reinterpret_cast<uintptr_t>(a_pSaveGameBuffer->buffer) + countPos;
                auto pCountBuf = reinterpret_cast<uint8_t*>(pCountBuf_addr);

                *pCountBuf = count;
                a_pSaveGameBuffer->bufferPosition += countSize;

                for (decltype(count) i = 0; i < count; ++i) {
                    auto pFile = GetNormalFileAt(a_pDataHandler, i);
                    assert(pFile);

                    SaveString(a_pSaveGameBuffer, pFile->filename, 0);
                }
            }

            {
                auto count = static_cast<uint16_t>(SmallFileCount(a_pDataHandler));
                auto countPos = a_pSaveGameBuffer->bufferPosition;
                auto countSize = 2;

                if (a_pSaveGameBuffer->size < countPos + countSize)
                    ExpandBuffer(a_pSaveGameBuffer, countPos + countSize);

                auto pCountBuf_addr = reinterpret_cast<uintptr_t>(a_pSaveGameBuffer->buffer) + countPos;
                auto pCountBuf = reinterpret_cast<uint16_t*>(pCountBuf_addr);

                *pCountBuf = count;
                a_pSaveGameBuffer->bufferPosition += countSize;

                for (decltype(count) i = 0; i < count; ++i) {
                    auto pFile = GetSmallFileAt(a_pDataHandler, i);
                    assert(pFile);

                    SaveString(a_pSaveGameBuffer, pFile->filename, 0);
                }
            }
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                lea(rdx, ptr[rsp + 0x20]);
                mov(rax, reinterpret_cast<uintptr_t>(SaveStringForFiles));
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

    static TESFile* GetFileForFormID(TESFormID formId)
    {
        // logger::debug("Entering GetFileForFormID()");

        static const uintptr_t GetFormByNumericID_addr = ID(796114).address();
        static const auto GetFormByNumericID = reinterpret_cast<TESForm* (*)(TESFormID)>(GetFormByNumericID_addr);

        static const uintptr_t GetFileForTempID_addr = Offset(0x139FD0).address();
        static const auto GetFileForTempID = reinterpret_cast<TESFile* (*)(TESFormID)>(GetFileForTempID_addr);

        auto pForm = GetFormByNumericID(formId);

        if (pForm)
            return GetFile(pForm, 0xFFFFFFFF);

        auto fileIndex = formId >> 24 & 0xFF;

        if (0 == fileIndex) {
            auto pTempFile = GetFileForTempID(formId);

            if (pTempFile)
                return pTempFile;
        }

        auto pDataHandler = GetDataHandler();
        assert(pDataHandler);

        return GetCompiledFileForFormId(pDataHandler, formId);
    }

    namespace AddReference
    {
        // XXX Status: verified
        static const uintptr_t target = ID(706698).address();
        static const auto patch_begin = target + 0x2E;
        static const auto patch_end = target + 0x6E;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetFileForFormID));
                call(rax);
                mov(rsi, rax);
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

    namespace AddReference_1
    {
        // XXX Status: verified
        static const uintptr_t target = ID(505969).address();
        static const auto patch_begin = target + 0x39;
        static const auto patch_end = target + 0x79;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetFileForFormID));
                call(rax);
                mov(rsi, rax);
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

    namespace FUN_14017fce0
    {
        // NOTE: Function is deleted in Fallout4.exe
        // static const uintptr_t target = Offset(0x17FCE0).address();
    }

    namespace FUN_1403508ff
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x3508FF).address();
        static const auto patch_begin = target + 0x2D;
        static const auto patch_end = target + 0x4A;

        static TESFile* GetModFileForFormId(void* /* placeholder */, TESFormID a_formId)
        {
            logger::debug("Entering TESFile* GetModFileForFormId()");

            auto pDataHandler = GetDataHandler();
            assert(pDataHandler);

            if (0 == GetNormalIndex(a_formId))
                return nullptr;
            else
                return GetCompiledFileForFormId(pDataHandler, a_formId);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetModFileForFormId));
                call(rax);
                and_(ebp, 0xFFFFFF);
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

    namespace GetAudioFullPath
    {
        // XXX Status: verified
        static const uintptr_t target = ID(506618).address();
        static const auto patch_begin = target + 0x24;
        static const auto patch_end = target + 0xC0;

        static const uintptr_t GetAudioFilename_addr = Offset(0x5FCE20).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                lea(rdx, ptr[rsp + 0x40]);
                mov(r8, r9);
                mov(r9, rbx);
                mov(byte[rsp + 0x20], 0);
                mov(rax, GetAudioFilename_addr);
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

    namespace GetAudioFullPath_1
    {
        // XXX Status: verified
        static const uintptr_t target = ID(506618).address();
        static const auto patch_begin = target + 0xDD;
        static const auto patch_end = target + 0xE5;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetCompiledFileForFormId));
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

    namespace GetWorkshopMenuNodeOriginFileIndex
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xC09E80).address();
        static const auto patch_begin = target + 0x33;
        static const auto patch_end = target + 0x3D;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(edi, esi);
                mov(rax, reinterpret_cast<uintptr_t>(GetCompiledFileForFormId));
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

    namespace GetWorkshopMenuNodeOriginFileIndex_1
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xC09E80).address();
        static const auto patch_begin = target + 0xB4;
        static const auto patch_end = target + 0xBC;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetCompiledFileForFormId));
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

    namespace FUN_140c09ff0
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xC09FF0).address();
        static const auto patch_begin = target + 0x1C;
        static const auto patch_end = target + 0x24;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(GetCompiledFileForFormId));
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

    namespace CloneTESFile
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x1397A0).address();
        static const auto patch_begin = target + 0x16A;
        static const auto patch_end = target + 0x16F;

        static void SetPageCacheForNormalFile(TESFile* a_pFile, void* a_pUnk, uint8_t a_fileIndex)
        {
            logger::debug("Entering SetPageCacheForNormalFile()");

            static const uintptr_t SetPageCache_addr = Offset(0x1BD5530).address();
            static const auto SetPageCache = reinterpret_cast<void (*)(TESFile*, void*, uint8_t)>(SetPageCache_addr);

            assert(a_pFile);
            assert(a_pUnk);

            if (0xFE == a_fileIndex)
                return;

            SetPageCache(a_pFile, a_pUnk, a_fileIndex);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(SetPageCacheForNormalFile));
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

    namespace CloneTESFile_1
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x1397A0).address();
        static const auto patch_begin = target + 0x16F;
        static const auto patch_end = target + 0x176;

        static void CopySmallFileCompileIndex(TESFile* a_pSrcFile, TESFile* a_pDstFile)
        {
            logger::debug("Entering CopySmallFileCompileIndex()");

            assert(a_pSrcFile);
            assert(a_pDstFile);

            a_pDstFile->smallFileCompileIndex = a_pSrcFile->smallFileCompileIndex;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rbp);
                mov(rdx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(CopySmallFileCompileIndex));
                call(rax);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataHandler));
                call(rax);
                mov(rdx, rax);
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

    namespace CloneTESFile_2
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x1397A0).address();
        static const auto patch_begin = target + 0x1BF;
        static const auto patch_end = target + 0x1DC;

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

    namespace ClearData
    {
        // XXX Status: unchecked
        static const uintptr_t target = ID(440700).address();
        static const auto patch_begin = target + 0x94E;
        static const auto patch_end = target + 0x98E;

        static void CleanUpFilesAndSmall(TESDataHandler* a_pDataHandler)
        {
            logger::debug("Entering CleanUpFilesAndSmall()");

            assert(a_pDataHandler);

            CleanUpNormalFiles(a_pDataHandler);
            CleanUpSmallFiles(a_pDataHandler);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, r14);
                mov(rax, reinterpret_cast<uintptr_t>(CleanUpFilesAndSmall));
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

    namespace GetExtCellDataFromFileByEditorID
    {
        // NOTE: Fallout4VR.exe's implement should be sufficient,
        //       because Fallout4.exe's doesn't consider snallFiles neither.
        // static const uintptr_t target = ID(1411233).address();
    }

    void InstallHook()
    {
        if (LoadAllChildDataForAllQuests::InstallHook())
            logger::debug("Installed LoadAllChildDataForAllQuests hook");
        else
            logger::debug("Failed LoadAllChildDataForAllQuests hook");

        if (SavePluginList::InstallHook())
            logger::debug("Installed SavePluginList hook");
        else
            logger::debug("Failed SavePluginList hook");

        if (AddReference::InstallHook())
            logger::debug("Installed AddReference hook");
        else
            logger::debug("Failed AddReference hook");

        if (AddReference_1::InstallHook())
            logger::debug("Installed AddReference_1 hook");
        else
            logger::debug("Failed AddReference_1 hook");

        if (FUN_1403508ff::InstallHook())
            logger::debug("Installed FUN_1403508ff hook");
        else
            logger::debug("Failed FUN_1403508ff hook");

        if (GetAudioFullPath::InstallHook())
            logger::debug("Installed GetAudioFullPath hook");
        else
            logger::debug("Failed GetAudioFullPath hook");

        if (GetAudioFullPath_1::InstallHook())
            logger::debug("Installed GetAudioFullPath_1 hook");
        else
            logger::debug("Failed GetAudioFullPath_1 hook");

        if (GetWorkshopMenuNodeOriginFileIndex::InstallHook())
            logger::debug("Installed GetWorkshopMenuNodeOriginFileIndex hook");
        else
            logger::debug("Failed GetWorkshopMenuNodeOriginFileIndex hook");

        if (GetWorkshopMenuNodeOriginFileIndex_1::InstallHook())
            logger::debug("Installed GetWorkshopMenuNodeOriginFileIndex_1 hook");
        else
            logger::debug("Failed GetWorkshopMenuNodeOriginFileIndex_1 hook");

        if (FUN_140c09ff0::InstallHook())
            logger::debug("Installed FUN_140c09ff0 hook");
        else
            logger::debug("Failed FUN_140c09ff0 hook");

        if (CloneTESFile::InstallHook())
            logger::debug("Installed CloneTESFile hook");
        else
            logger::debug("Failed CloneTESFile hook");

        if (CloneTESFile_1::InstallHook())
            logger::debug("Installed CloneTESFile_1 hook");
        else
            logger::debug("Failed CloneTESFile_1 hook");

        if (CloneTESFile_2::InstallHook())
            logger::debug("Installed CloneTESFile_2 hook");
        else
            logger::debug("Failed CloneTESFile_2 hook");

        if (ClearData::InstallHook())
            logger::debug("Installed ClearData hook");
        else
            logger::debug("Failed ClearData hook");
    }
}
