#include "PCH.h"

#include "SmallFileLib.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace SmallFileLoader
{
    TESFormID GetMaskedFormId(TESForm* a_pForm, TESFile* a_pFile)
    {
        if (!a_pForm)
            return 0;

        if (a_pFile && IsSmallFile(a_pFile))
            return a_pForm->formID & 0xFFF;
        else
            return a_pForm->formID & 0xFFFFFF;
    }

    TESFormID GetMaskedFormId(TESForm* a_pForm)
    {
        static const uintptr_t GetFile_addr = ID(1376557).address();
        static const auto GetFile = reinterpret_cast<TESFile* (*)(TESForm*, uint32_t)>(GetFile_addr);

        if (!a_pForm)
            return 0;

        return GetMaskedFormId(a_pForm, GetFile(a_pForm, 0));
    }

    void AdjustFormIDFileIndex(TESFile* a_pFile, TESFormID* a_pFormId)
    {
        if (!a_pFile)
            return;
        if (!a_pFormId)
            return;

        if (IsSmallFile(a_pFile)) {
            *a_pFormId &= 0x00000FFF;
            *a_pFormId |= a_pFile->smallFileCompileIndex << 12;
            *a_pFormId |= 0xFE << 24;
        }
        else {
            *a_pFormId &= 0x00FFFFFF;
            *a_pFormId |= a_pFile->compileIndex << 24;
        }
    }

    TESFormID GetAdjustedFormIDFileIndex(TESFile* a_pFile, TESFormID a_formId)
    {
        auto formId_ = a_formId;

        AdjustFormIDFileIndex(a_pFile, &formId_);

        return formId_;
    }
}
