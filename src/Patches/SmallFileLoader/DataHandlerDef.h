#pragma once

#include "PCH.h"


/* === SUMMARY OF TESDataHandler DATA MODEL WITH SMALL FILE SUPPORT ===
**
** Data structure is basically identical to original VR's; loadedModCount at
** 0FC0, loadedMods[0xFF] at 0FC8. Only loadedMods[0xFE] is modified.
**
** > TESDataHandler::loadedMods:
**   - 254th element of array is now pointer to small file sub-array,
**     that is an instance of BSTArray, holding up to 4096 elements of TESFile.
**
**     * Ex. smallFileFE010 = (BSTArray*)(loadedMods[0xFE])->get(0x010)
*/


namespace SmallFileLoader
{
    extern RE::TESDataHandler** g_pDataHandler;
    RE::TESDataHandler* GetDataHandler();

    void ConstructSmallFiles(RE::TESDataHandler* a_pDataHandler);
    void DestructSmallFiles(RE::TESDataHandler* a_pDataHandler);
    void CleanUpNormalFiles(RE::TESDataHandler* a_pDataHandler);
    void CleanUpSmallFiles(RE::TESDataHandler* a_pDataHandler);
    bool IsInCollection(RE::TESDataHandler* a_pDataHandler, RE::TESFile* a_pFile);
    uint8_t AddNormalFile(RE::TESDataHandler* a_pDataHandler, RE::TESFile* a_pFile);
    uint16_t AddSmallFile(RE::TESDataHandler* a_pDataHandler, RE::TESFile* a_pFile);
    uint8_t NormalFileCount(RE::TESDataHandler* a_pDataHandler);
    uint16_t SmallFileCount(RE::TESDataHandler* a_pDataHandler);
    RE::TESFile* GetNormalFileAt(RE::TESDataHandler* a_pDataHandler, uint8_t a_normalIndex);
    RE::TESFile* GetSmallFileAt(RE::TESDataHandler* a_pDataHandler, uint16_t a_smallIndex);
    RE::TESFile* GetCompiledFileForFormId(RE::TESDataHandler* a_pDataHandler, RE::TESFormID a_formId);


    inline RE::TESDataHandler* GetDataHandler()
    {
        if (!g_pDataHandler)
            return nullptr;

        return *g_pDataHandler;
    }
}
