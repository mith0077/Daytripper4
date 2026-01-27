#include "PCH.h"

#include "SmallFileLib.h"
#include "SaveLoadGameDef.h"

using namespace std;
using namespace F4SE::stl;
using namespace RE;
using namespace REL;


namespace SmallFileLoader
{
    BGSSaveLoadGame** g_pSaveLoadGame = reinterpret_cast<BGSSaveLoadGame**>(ID(177947).address());

    static BSTArray<TESFile*>* _GetNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame);
    static BSTArray<TESFile*>* _GetSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame);

    static const uintptr_t BSTArrayHeapAllocatorCtor_addr = ID(613024).address();
    static const auto BSTArrayHeapAllocatorCtor = reinterpret_cast<void* (*)(void*)>(BSTArrayHeapAllocatorCtor_addr);

    static const uintptr_t BSTArrayBaseCtor_addr = ID(1438336).address();
    static const auto BSTArrayBaseCtor = reinterpret_cast<void* (*)(void*)>(BSTArrayBaseCtor_addr);

    static const uintptr_t BSTArrayHeapAllocatorDtor_addr = ID(758406).address();
    static const auto BSTArrayHeapAllocatorDtor = reinterpret_cast<void (*)(void*)>(BSTArrayHeapAllocatorDtor_addr);

    static const uintptr_t BSTArrayBaseDtor_addr = ID(174389).address();
    static const auto BSTArrayBaseDtor = reinterpret_cast<void (*)(void*)>(BSTArrayBaseDtor_addr);


    void ConstructNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        logger::debug("Entering ConstructNormalFiles()");

        assert(a_pSaveLoadGame);

        auto normalFiles_addr = reinterpret_cast<uintptr_t>(_GetNormalFiles(a_pSaveLoadGame));

        BSTArrayHeapAllocatorCtor(reinterpret_cast<void*>(normalFiles_addr));
        BSTArrayBaseCtor(reinterpret_cast<void*>(normalFiles_addr + 0x10));
    }

    void ConstructSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        logger::debug("Entering ConstructSmallFiles()");

        assert(a_pSaveLoadGame);

        auto smallFiles_addr = reinterpret_cast<uintptr_t>(_GetSmallFiles(a_pSaveLoadGame));

        BSTArrayHeapAllocatorCtor(reinterpret_cast<void*>(smallFiles_addr));
        BSTArrayBaseCtor(reinterpret_cast<void*>(smallFiles_addr + 0x10));
    }

    void DestructNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        logger::debug("Entering DestructNormalFiles()");

        assert(a_pSaveLoadGame);

        auto normalFiles_addr = reinterpret_cast<uintptr_t>(_GetNormalFiles(a_pSaveLoadGame));

        BSTArrayBaseDtor(reinterpret_cast<void*>(normalFiles_addr + 0x10));
        BSTArrayHeapAllocatorDtor(reinterpret_cast<void*>(normalFiles_addr));
    }

    void DestructSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        logger::debug("Entering DestructSmallFiles()");

        assert(a_pSaveLoadGame);

        auto smallFiles_addr = reinterpret_cast<uintptr_t>(_GetSmallFiles(a_pSaveLoadGame));

        BSTArrayBaseDtor(reinterpret_cast<void*>(smallFiles_addr + 0x10));
        BSTArrayHeapAllocatorDtor(reinterpret_cast<void*>(smallFiles_addr));
    }

    void CleanUpNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        logger::debug("Entering CleanUpNormalFiles()");

        assert(a_pSaveLoadGame);

        auto pNormalFiles = _GetNormalFiles(a_pSaveLoadGame);

        if (!pNormalFiles)
            return;

        pNormalFiles->clear();
    }

    void CleanUpSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        logger::debug("Entering CleanUpSmallFiles()");

        assert(a_pSaveLoadGame);

        auto pSmallFiles = _GetSmallFiles(a_pSaveLoadGame);

        if (!pSmallFiles)
            return;

        pSmallFiles->clear();
    }

    bool IsInCollection(BGSSaveLoadGame* a_pSaveLoadGame, TESFile* a_pFile)
    {
        logger::debug("Entering IsInCollection()");

        assert(a_pSaveLoadGame);

        if (!a_pFile)
            return false;

        auto normalFileCount = NormalFileCount(a_pSaveLoadGame);
        for (decltype(normalFileCount) i = 0; i < normalFileCount; ++i) {
            if (GetNormalFileAt(a_pSaveLoadGame, i) == a_pFile)
                return true;
        }

        auto smallFileCount = SmallFileCount(a_pSaveLoadGame);
        for (decltype(smallFileCount) i = 0; i < smallFileCount; ++i) {
            if (GetSmallFileAt(a_pSaveLoadGame, i) == a_pFile)
                return true;
        }

        return false;
    }

    uint8_t AddNormalFile(BGSSaveLoadGame* a_pSaveLoadGame, TESFile* a_pFile)
    {
        logger::debug("Entering AddNormalFile()");

        constexpr uint8_t failIndex = 0xFF;

        assert(a_pSaveLoadGame);

        auto currentCount = NormalFileCount(a_pSaveLoadGame);

        if (0xFE == currentCount) {
            report_and_fail("Regular plugin count exceeded hard limit 253, aborting.");

            return failIndex;
        }

        _GetNormalFiles(a_pSaveLoadGame)->push_back(a_pFile);

        return currentCount;
    }

    uint16_t AddSmallFile(BGSSaveLoadGame* a_pSaveLoadGame, TESFile* a_pFile)
    {
        logger::debug("Entering AddSmallFile()");

        constexpr uint16_t failIndex = 0xFFFF;

        assert(a_pSaveLoadGame);

        if (!_GetSmallFiles(a_pSaveLoadGame))
            return failIndex;

        auto currentCount = SmallFileCount(a_pSaveLoadGame);

        if (0x1000 == currentCount) {
            report_and_fail("Light plugin count exceeded hard limit 4096, aborting.");

            return failIndex;
        }

        _GetSmallFiles(a_pSaveLoadGame)->push_back(a_pFile);

        return currentCount;
    }

    uint8_t NormalFileCount(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        assert(a_pSaveLoadGame);

        auto pNormalFiles = _GetNormalFiles(a_pSaveLoadGame);

        return pNormalFiles ? static_cast<uint8_t>(pNormalFiles->size()) : 0;
    }

    uint16_t SmallFileCount(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        assert(a_pSaveLoadGame);

        auto pSmallFiles = _GetSmallFiles(a_pSaveLoadGame);

        return pSmallFiles ? static_cast<uint16_t>(pSmallFiles->size()) : 0;
    }

    TESFile* GetNormalFileAt(BGSSaveLoadGame* a_pSaveLoadGame, uint8_t a_fileIndex)
    {
        assert(a_pSaveLoadGame);

        if (!_GetNormalFiles(a_pSaveLoadGame))
            return nullptr;
        if (NormalFileCount(a_pSaveLoadGame) <= a_fileIndex)
            return nullptr;

        return _GetNormalFiles(a_pSaveLoadGame)->at(a_fileIndex);
    }

    TESFile* GetSmallFileAt(BGSSaveLoadGame* a_pSaveLoadGame, uint16_t a_smallIndex)
    {
        assert(a_pSaveLoadGame);

        if (!_GetSmallFiles(a_pSaveLoadGame))
            return nullptr;
        if (SmallFileCount(a_pSaveLoadGame) <= a_smallIndex)
            return nullptr;

        return _GetSmallFiles(a_pSaveLoadGame)->at(a_smallIndex);
    }

    TESFormID GetConvertedFormId(BGSSaveLoadGame* a_pSaveLoadGame, TESFormID a_formId)
    {
        assert(a_pSaveLoadGame);

        BSTArray<TESFile*>* pFileArray;
        uint32_t fileIndex;

        if (IsSmallFormId(a_formId)) {
            pFileArray = _GetSmallFiles(a_pSaveLoadGame);
            fileIndex = GetSmallIndex(a_formId);
        }
        else {
            pFileArray = _GetNormalFiles(a_pSaveLoadGame);
            fileIndex = GetNormalIndex(a_formId);
        }

        TESFormID convertedFormId = 0;

        do {
            if (pFileArray->size() <= fileIndex)
                break;

            auto pFile = pFileArray->at(fileIndex);

            if (!pFile)
                break;

            convertedFormId = GetAdjustedFormIDFileIndex(pFile, a_formId);
        }
        while (false);

        if (a_formId != convertedFormId)
            logger::debug("Applying save data form id conversion: {:08x} -> {:08x}", a_formId, convertedFormId);

        return convertedFormId;
    }


    static BSTArray<TESFile*>* _GetNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        assert(a_pSaveLoadGame);

        auto normalFiles_addr = reinterpret_cast<uintptr_t>(a_pSaveLoadGame);

        return reinterpret_cast<BSTArray<TESFile*>*>(normalFiles_addr);
    }

    static BSTArray<TESFile*>* _GetSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame)
    {
        assert(a_pSaveLoadGame);

        auto smallFiles_addr = reinterpret_cast<uintptr_t>(a_pSaveLoadGame) + 0x18;

        return reinterpret_cast<BSTArray<TESFile*>*>(smallFiles_addr);
    }
}
