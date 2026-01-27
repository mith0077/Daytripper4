#include "PCH.h"

#include "PatchLib.h"
#include "PluginVersionPatcher.h"

using namespace std;
using namespace REL;


namespace PluginVersionPatcher
{
    namespace SetDependenciesChecked
    {
        static const uintptr_t target = ID(702680).address();
        static const auto patch_begin = target + 0x36;
        static const auto patch_end = target + 0x3E;

        static const uintptr_t Literal1_00_addr = Offset(0x2C7F640).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, Literal1_00_addr);
                movss(xmm6, ptr[rcx]);
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
        if (SetDependenciesChecked::InstallHook())
            logger::debug("Installed SetDependenciesChecked hook");
        else
            logger::debug("Failed SetDependenciesChecked hook");
    }
}
