#include "PCH.h"

#include "PatchLib.h"
#include "DefaultObjectLib.h"
#include "DefaultObjectHook.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace DefaultObjectHook
{
    namespace dynamic_initializer_for_SpellEquipSlot
    {
        // XXX Status: verified
        static const auto target = VariantID(943735, 2169250, 0x29B9BC0).address();
        static const auto patch_begin = target + 0x1C;
        static const auto patch_end = target + 0x21;

        static const uintptr_t BGSDefaultObjectCtor_addr = RelocationID(1144014, 2197003, 1144014).address();
        static const auto BGSDefaultObjectCtor = reinterpret_cast<BGSDefaultObject* (*)(BGSDefaultObject*, const char*, ENUM_FORM_ID, const char*)>(BGSDefaultObjectCtor_addr);

        BGSDefaultObject* DefaultObjectInjected(BGSDefaultObject* a_this, const char* a_name, ENUM_FORM_ID a_formType, const char* a_description)
        {
            logger::debug("Entering DefaultObjectInjected()");

            assert(a_this);

            DefaultObject::InitializeAll();

            return BGSDefaultObjectCtor(a_this, a_name, a_formType, a_description);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(DefaultObjectInjected));
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
        if (dynamic_initializer_for_SpellEquipSlot::InstallHook())
            logger::debug("Installed dynamic_initializer_for_SpellEquipSlot hook");
        else
            logger::debug("Failed dynamic_initializer_for_SpellEquipSlot hook");
    }
}
