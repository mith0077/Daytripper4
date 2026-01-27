#pragma once

#include "PCH.h"


class __declspec(novtable) DefaultObject : public RE::TESForm
{
public:
    DefaultObject(const char* a_name, RE::ENUM_FORM_ID a_formType, const char* a_description);
    static void InitializeAll();

    template <class T = TESForm>
    T* GetForm()
    {
        return form ? form->As<T>() : nullptr;
    }

    // members
    TESForm*                                     form;          // 20
    REX::EnumSet<RE::ENUM_FORM_ID, std::uint8_t> type;          // 28
    RE::BSFixedString                            formEditorID;  // 30

protected:
    void InitializeOne();

    static std::vector<DefaultObject*> instances_;
    static std::mutex instancesMtx_;

    const char* name_;
    RE::ENUM_FORM_ID formType_;
    const char* description_;
};
