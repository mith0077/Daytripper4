#include "PCH.h"

#include "SmallFileLib.h"
#include "DataHandlerDef.h"

using namespace std;
using namespace F4SE::stl;
using namespace RE;
using namespace REL;


namespace SmallFileLoader
{
    TESDataHandler** g_pDataHandler = reinterpret_cast<TESDataHandler**>(ID(711558).address());

    // Redefine TESDataHandler members to maintain unified binary (avoid ENABLE_FALLOUT_VR macro)
    static uint32_t* _LoadedModCount(TESDataHandler* a_pDataHandler);
    static TESFile** _LoadedMods(TESDataHandler* a_pDataHandler);

    // Actual position of smallFiles is a_pDataHandler->loadedMods[0xFE]
    static BSTArray<TESFile*>* _GetSmallFiles(TESDataHandler* a_pDataHandler);


    void ConstructSmallFiles(TESDataHandler* a_pDataHandler)
    {
        logger::debug("Entering ConstructSmallFiles()");
        logger::info("*** Start constructing data handler ***");

        assert(a_pDataHandler);

        auto pSmallFiles = new BSTArray<TESFile*>();
        *(_LoadedMods(a_pDataHandler) + 0xFE) = reinterpret_cast<TESFile*>(pSmallFiles);
    }

    void DestructSmallFiles(TESDataHandler* a_pDataHandler)
    {
        logger::debug("Entering DestructSmallFiles()");

        assert(a_pDataHandler);

        delete _GetSmallFiles(a_pDataHandler);
        *(_LoadedMods(a_pDataHandler) + 0xFE) = nullptr;
    }

    void CleanUpNormalFiles(TESDataHandler* a_pDataHandler)
    {
        logger::debug("Entering CleanUpNormalFiles()");

        static const uintptr_t RemoveFromOwnedIDMap_addr = ID(722648).address();
        static const auto RemoveFromOwnedIDMap = reinterpret_cast<void (*)(TESFile*)>(RemoveFromOwnedIDMap_addr);

        assert(a_pDataHandler);

        auto pLoadedModCount = _LoadedModCount(a_pDataHandler);
        auto ppLoadedMods = _LoadedMods(a_pDataHandler);

        assert(pLoadedModCount);
        assert(ppLoadedMods);

        for (remove_pointer<decltype(pLoadedModCount)>::type i = 0; i < *pLoadedModCount; ++i) {
            auto pFile = ppLoadedMods[i];

            if (!pFile)
                continue;

            RemoveFromOwnedIDMap(pFile);
            pFile->compileIndex = 0xFF;
            ppLoadedMods[i] = nullptr;
        }

        *pLoadedModCount = 0;
    }

    void CleanUpSmallFiles(TESDataHandler* a_pDataHandler)
    {
        logger::debug("Entering CleanUpSmallFiles()");

        static const uintptr_t RemoveFromOwnedIDMap_addr = ID(722648).address();
        static const auto RemoveFromOwnedIDMap = reinterpret_cast<void (*)(TESFile*)>(RemoveFromOwnedIDMap_addr);

        assert(a_pDataHandler);

        auto pSmallFiles = _GetSmallFiles(a_pDataHandler);

        if (!pSmallFiles)
            return;

        auto smallFilesSize = pSmallFiles->size();
        for (decltype(smallFilesSize) i = 0; i < smallFilesSize; ++i) {
            auto pFile = pSmallFiles->at(i);

            if (!pFile)
                continue;

            RemoveFromOwnedIDMap(pFile);
            pFile->compileIndex = 0xFF;
        }

        pSmallFiles->clear();
    }

    bool IsInCollection(TESDataHandler* a_pDataHandler, TESFile* a_pFile)
    {
        logger::debug("Entering IsInCollection()");

        assert(a_pDataHandler);

        if (!a_pFile)
            return false;

        auto normalFileCount = NormalFileCount(a_pDataHandler);
        for (decltype(normalFileCount) i = 0; i < normalFileCount; ++i) {
            if (GetNormalFileAt(a_pDataHandler, i) == a_pFile)
                return true;
        }

        auto smallFileCount = SmallFileCount(a_pDataHandler);
        for (decltype(smallFileCount) i = 0; i < smallFileCount; ++i) {
            if (GetSmallFileAt(a_pDataHandler, i) == a_pFile)
                return true;
        }

        return false;
    }

    uint8_t AddNormalFile(TESDataHandler* a_pDataHandler, TESFile* a_pFile)
    {
        logger::debug("Entering AddNormalFile()");

        constexpr uint8_t failIndex = 0xFF;

        assert(a_pDataHandler);

        if (!a_pFile)
            return failIndex;

        auto currentCount = NormalFileCount(a_pDataHandler);

        if (0xFE == currentCount) {
            report_and_fail("Regular plugin count exceeded hard limit 253, aborting.");

            return failIndex;
        }

        *(_LoadedMods(a_pDataHandler) + currentCount) = a_pFile;
        *_LoadedModCount(a_pDataHandler) = currentCount + 1;

        return currentCount;
    }

    uint16_t AddSmallFile(TESDataHandler* a_pDataHandler, TESFile* a_pFile)
    {
        logger::debug("Entering AddSmallFile()");

        constexpr uint16_t failIndex = 0xFFFF;

        assert(a_pDataHandler);

        if (!a_pFile)
            return failIndex;
        if (!_GetSmallFiles(a_pDataHandler))
            return failIndex;

        auto currentCount = SmallFileCount(a_pDataHandler);

        if (0x1000 == currentCount) {
            report_and_fail("Light plugin count exceeded hard limit 4096, aborting.");

            return failIndex;
        }

        _GetSmallFiles(a_pDataHandler)->push_back(a_pFile);

        return currentCount;
    }

    uint8_t NormalFileCount(TESDataHandler* a_pDataHandler)
    {
        assert(a_pDataHandler);

        auto loadedModCount = *_LoadedModCount(a_pDataHandler);

        return loadedModCount <= 254 ? static_cast<uint8_t>(loadedModCount) : 254;
    }

    uint16_t SmallFileCount(TESDataHandler* a_pDataHandler)
    {
        assert(a_pDataHandler);

        auto pSmallFiles = _GetSmallFiles(a_pDataHandler);

        return pSmallFiles ? static_cast<uint16_t>(pSmallFiles->size()) : 0;
    }

    TESFile* GetNormalFileAt(TESDataHandler* a_pDataHandler, uint8_t a_normalIndex)
    {
        assert(a_pDataHandler);

        if (NormalFileCount(a_pDataHandler) <= a_normalIndex)
            return nullptr;

        return _LoadedMods(a_pDataHandler)[a_normalIndex];
    }

    TESFile* GetSmallFileAt(TESDataHandler* a_pDataHandler, uint16_t a_smallIndex)
    {
        assert(a_pDataHandler);

        if (!_GetSmallFiles(a_pDataHandler))
            return nullptr;
        if (SmallFileCount(a_pDataHandler) <= a_smallIndex)
            return nullptr;

        return _GetSmallFiles(a_pDataHandler)->at(a_smallIndex);
    }

    TESFile* GetCompiledFileForFormId(TESDataHandler* a_pDataHandler, TESFormID a_formId)
    {
        assert(a_pDataHandler);

        if (IsSmallFormId(a_formId))
            return GetSmallFileAt(a_pDataHandler, GetSmallIndex(a_formId));
        else
            return GetNormalFileAt(a_pDataHandler, GetNormalIndex(a_formId));
    }


    inline static uint32_t* _LoadedModCount(TESDataHandler* a_pDataHandler)
    {
        constexpr uintptr_t offset = 0xFC0;

        assert(a_pDataHandler);

        auto addr = reinterpret_cast<uintptr_t>(a_pDataHandler) + offset;

        return reinterpret_cast<uint32_t*>(addr);
    }

    inline static TESFile** _LoadedMods(TESDataHandler* a_pDataHandler)
    {
        constexpr uintptr_t offset = 0xFC8;

        assert(a_pDataHandler);

        auto addr = reinterpret_cast<uintptr_t>(a_pDataHandler) + offset;

        return reinterpret_cast<TESFile**>(addr);
    }

    inline static BSTArray<TESFile*>* _GetSmallFiles(TESDataHandler* a_pDataHandler)
    {
        assert(a_pDataHandler);

        // Repurpose normalFiles[0xFE] into smallFiles pointer
        auto pFileFE = _LoadedMods(a_pDataHandler) + 0xFE;

        if (!pFileFE)
            return nullptr;
        else
            return *reinterpret_cast<BSTArray<TESFile*>**>(pFileFE);
    }
}
