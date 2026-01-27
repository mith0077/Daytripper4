#include "PCH.h"

#include "PatchLib.h"
#include "BookMenuText.h"

using namespace std;
using namespace RE;
using namespace REL;
using namespace REX;


namespace BookMenuText
{
    struct Interface3D_RendererStruct  // NOTE: VR alignment differ from FO4's
    {
        void* Unk0[14];
        EnumSet<Interface3D::PostEffect, std::int32_t> postfx; // 070
        void* Unk1[53];
        BSFixedString name;                                    // 220
    };

    namespace Offscreen_SetPostEffect
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1187686).address();
        static const auto patch_begin = target + 0x0;
        static const auto patch_end = target + 0xc;

        static void SetPostEffectUnlessBookMenu(Interface3D_RendererStruct* a_pRenderer, Interface3D::PostEffect a_postEffect)
        {
            static const char* strBookMenuModel = "WSBookMenuModel";

            logger::debug("Entering SetPostEffectUnlessBookMenu()");

            assert(a_pRenderer);

            logger::debug("Rendeerer: {}, param: {}", a_pRenderer->name.c_str(), static_cast<int32_t>(a_postEffect));

            if (a_pRenderer->name == strBookMenuModel)
                a_pRenderer->postfx = Interface3D::PostEffect::kNone;
            else
                a_pRenderer->postfx = a_postEffect;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(SetPostEffectUnlessBookMenu));
                jmp(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace fBookModelY_Data
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x37B7158).address();
        static const auto patch_begin = target + 0x8;
        static const auto patch_end = target + 0xC;

        static const float fSensibleDefault = 41.5f;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                dd(*reinterpret_cast<const uint32_t*>(&fSensibleDefault));
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace fBookModelPitch_Data
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x37B71A0).address();
        static const auto patch_begin = target + 0x8;
        static const auto patch_end = target + 0xC;

        static const float fSensibleDefault = -90.01f;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                dd(*reinterpret_cast<const uint32_t*>(&fSensibleDefault));
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    void InstallHook()
    {
        if (Offscreen_SetPostEffect::InstallHook())
            logger::debug("Installed Offscreen_SetPostEffect hook");
        else
            logger::debug("Failed Offscreen_SetPostEffect hook");

        if (fBookModelY_Data::InstallHook())
            logger::debug("Installed fBookModelY_Data hook");
        else
            logger::debug("Failed fBookModelY_Data hook");

        if (fBookModelPitch_Data::InstallHook())
            logger::debug("Installed fBookModelPitch_Data hook");
        else
            logger::debug("Failed fBookModelPitch_Data hook");
    }
}
