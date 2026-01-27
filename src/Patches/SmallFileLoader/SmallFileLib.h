#pragma once

#include "PCH.h"


namespace  SmallFileLoader
{
    bool IsSmallFile(RE::TESFile* a_pFile);
    bool IsSmallFormId(RE::TESFormID a_formId);
    uint8_t GetNormalIndex(RE::TESFormID a_formId);
    uint16_t GetSmallIndex(RE::TESFormID a_formId);

    RE::TESFormID GetMaskedFormId(RE::TESForm* a_pForm, RE::TESFile* a_pFile);
    RE::TESFormID GetMaskedFormId(RE::TESForm* a_pForm);
    void AdjustFormIDFileIndex(RE::TESFile* a_pFile, RE::TESFormID* a_pFormId);
    RE::TESFormID GetAdjustedFormIDFileIndex(RE::TESFile* a_pFile, RE::TESFormID a_formId);

    inline bool IsSmallFile(RE::TESFile* a_pFile)
    {
        return a_pFile && (a_pFile->flags & RE::TESFile::RecordFlag::kSmallFile);
    }

    inline bool IsSmallFormId(RE::TESFormID a_formId)
    {
        return 0xFE == GetNormalIndex(a_formId);
    }

    inline uint8_t GetNormalIndex(RE::TESFormID a_formId)
    {
        return static_cast<uint8_t>(a_formId >> 24 & 0xFF);
    }

    inline uint16_t GetSmallIndex(RE::TESFormID a_formId)
    {
        if (!IsSmallFormId(a_formId))
            return 0;

        return static_cast<uint16_t>(a_formId >> 12 & 0xFFF);
    }
}
