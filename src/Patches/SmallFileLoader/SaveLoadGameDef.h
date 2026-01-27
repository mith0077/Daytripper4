#pragma once

#include "PCH.h"


/* === SUMMARY OF BGSSaveLoadGame DATA MODEL WITH SMALL FILE SUPPORT ===
**
** VR version of BGSSaveLoadGame has only uint8_t to uint8_t mappings
** for saveDataFormPrefix <-> loadOrderFormPrefix. These structures are
** insufficient to support small files.
** So they are wiped out entirely and replaced by following two array.
**
** > BGSSaveLoadGame::normalFiles (offset 0x0)
**   - An instance of BSTArray mapping normal save data prefix to TESFile*
**
**     * Ex. TESFile* saveFile12 = normalFiles[0x12]
**
** > BGSSaveLoadGame::smallFiles (offset 0x18)
**   - An instance of BSTArray mapping small save data prefix to TESFile*
**
**     * Ex. TESFile* saveFileFE123 = smallFiles[0x123]
*/


namespace SmallFileLoader
{
    class BGSSaveLoadGame;

    extern BGSSaveLoadGame** g_pSaveLoadGame;
    BGSSaveLoadGame* GetSaveLoadGame();

    void ConstructNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame);
    void ConstructSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame);
    void DestructNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame);
    void DestructSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame);
    void CleanUpNormalFiles(BGSSaveLoadGame* a_pSaveLoadGame);
    void CleanUpSmallFiles(BGSSaveLoadGame* a_pSaveLoadGame);
    bool IsInCollection(BGSSaveLoadGame* a_pSaveLoadGame, RE::TESFile* a_pFile);
    uint8_t AddNormalFile(BGSSaveLoadGame* a_pSaveLoadGame, RE::TESFile* a_pFile);
    uint16_t AddSmallFile(BGSSaveLoadGame* a_pSaveLoadGame, RE::TESFile* a_pFile);
    uint8_t NormalFileCount(BGSSaveLoadGame* a_pSaveLoadGame);
    uint16_t SmallFileCount(BGSSaveLoadGame* a_pSaveLoadGame);
    RE::TESFile* GetNormalFileAt(BGSSaveLoadGame* a_pSaveLoadGame, uint8_t a_normalIndex);
    RE::TESFile* GetSmallFileAt(BGSSaveLoadGame* a_pSaveLoadGame, uint16_t a_smallIndex);
    RE::TESFormID GetConvertedFormId(BGSSaveLoadGame* a_pSaveLoadGame, RE::TESFormID a_formId);


    inline BGSSaveLoadGame* GetSaveLoadGame()
    {
        if (!g_pSaveLoadGame)
            return nullptr;

        return *g_pSaveLoadGame;
    }
}
