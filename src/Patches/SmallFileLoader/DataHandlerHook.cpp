#include "PCH.h"

#include "PatchLib.h"
#include "DataHandlerDef.h"
#include "DataHandlerHook.h"

using namespace std;
using namespace RE;
using namespace REL;


// Hooks for constructor/destructor of TESDataHandler to inject small files
namespace SmallFileLoader::DataHandlerHook
{
    namespace TESDataHandlerCtor
    {
        // XXX Status: verified
        static const uintptr_t target = ID(1403410).address();
        static const auto patch_begin = target + 0x2D1;
        static const auto patch_end = target + 0x2D6;

        static const uintptr_t RegisterClearArchiveRegistryEventSink_addr = ID(928494).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, RegisterClearArchiveRegistryEventSink_addr);
                call(rax);
                mov(rcx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(ConstructSmallFiles));
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

    namespace TESDataHandlerDtor
    {
        // XXX Status: noctd
        static const uintptr_t target = ID(751051).address();
        static const auto patch_begin = target + 0x49;
        static const auto patch_end = target + 0x4E;

        static const uintptr_t UnregisterClearArchiveRegistryEventSink_addr = ID(223944).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, UnregisterClearArchiveRegistryEventSink_addr);
                call(rax);
                mov(rcx, r14);
                mov(rax, reinterpret_cast<uintptr_t>(DestructSmallFiles));
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
        if (TESDataHandlerCtor::InstallHook())
            logger::debug("Installed TESDataHandlerCtor hook");
        else
            logger::debug("Failed TESDataHandlerCtor hook");

        if (TESDataHandlerDtor::InstallHook())
            logger::debug("Installed TESDataHandlerDtor hook");
        else
            logger::debug("Failed TESDataHandlerDtor hook");
    }
}
