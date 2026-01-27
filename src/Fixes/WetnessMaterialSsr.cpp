#include "PCH.h"

#include "PatchLib.h"
#include "WetnessMaterialSsr.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace WetnessMaterialSsr
{
    namespace SetupDataLightingShader
    {
        // XXX Status: verified
        static const uintptr_t target = ID(559354).address();
        static const auto patch_begin = target + 0x438;
        static const auto patch_end = target + 0x443;

        static void SetWetnessControlScreenSpaceReflectionsChecked(BSShaderMaterial* a_pShaderMaterial)
        {
            static const uintptr_t BSLightingShaderMaterialEnvmap_vtaddr = ID(1013115).address();

            logger::debug("Entering SetWetnessControlScreenSpaceReflectionsChecked()");

            assert(a_pShaderMaterial);

            auto vtaddr = *reinterpret_cast<uintptr_t*>(a_pShaderMaterial);

            if (BSLightingShaderMaterialEnvmap_vtaddr != vtaddr) {
                logger::debug("Argument is not an enviromnet material, rejecting: {:x}", vtaddr);

                return;
            }

            reinterpret_cast<uint8_t*>(a_pShaderMaterial)[0xC5] = 1;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, ptr[rdi + 0x58]);
                mov(rax, reinterpret_cast<uintptr_t>(SetWetnessControlScreenSpaceReflectionsChecked));
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
        if (SetupDataLightingShader::InstallHook())
            logger::debug("Installed SetupDataLightingShader hook");
        else
            logger::debug("Failed SetupDataLightingShader hook");
    }
}
