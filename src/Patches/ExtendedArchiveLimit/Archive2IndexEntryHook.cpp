#include "PCH.h"

#include "PatchLib.h"
#include "ExtendedArchiveDef.h"
#include "Archive2IndexEntryHook.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace ExtendedArchiveLimit::Archive2IndexEntryHook
{
    namespace RegisterBSA
    {
        // XXX Status: unchecked
        static const uintptr_t target = RelocationID(17474, 2269416, 17474).address();
        static const auto patch_begin = target + VariantOffset(0x49D, 0x2E1, 0x49D).offset();
        static const auto patch_end = target + VariantOffset(0x4A1, 0x2E5, 0x4A1).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? nop() : mov(r9d, r12d);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace RegisterBSA_1
    {
        // XXX Status: unchecked
        static const uintptr_t target = RelocationID(17474, 2269416, 17474).address();
        static const auto patch_begin = target + VariantOffset(0x238, 0x1B9, 0x238).offset();
        static const auto patch_end = target + VariantOffset(0x23E, 0x1BF, 0x23E).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? mov(rcx, ptr[rbp + 0x2d0]) : lea(rcx, ptr[rsp + 0x38]);
                Module::IsNG() ? mov(edx, ptr[rsp + 0x34]) : mov(edx, r12d);
                mov(rax, reinterpret_cast<uintptr_t>(SetDataFileIndexEx));
                call(rax);
                Module::IsNG() ? mov(ecx, ptr[rbp - 0x74]) : mov(ecx, ptr[rbp - 0x44]);
                shr(ecx, 0x4);
                test(cl, 0x1);
                mov(rdx, patch_end);
                jmp(rdx);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace RegisterBSA_2
    {
        // XXX Status: unchecked
        static const uintptr_t target = RelocationID(17474, 2269416, 17474).address();
        static const auto patch_begin = target + VariantOffset(0x4FE, 0x33B, 0x4FE).offset();
        static const auto patch_end = target + VariantOffset(0x504, 0x341, 0x504).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                cmp(byte[rsp + 0x46], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace RegisterBSA_3
    {
        // XXX Status: unchecked
        static const uintptr_t target = RelocationID(17474, 2269416, 17474).address();
        static const auto patch_begin = target + VariantOffset(0x51A, 0x35F, 0x51A).offset();
        static const auto patch_end = target + VariantOffset(0x522, 0x367, 0x522).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                Module::IsNG() ? cmp(byte[rbp + 0x156], 0x0) : cmp(byte[rbp + 0x176], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace AddDataFromReader
    {
        // XXX Status: verified
        static const uintptr_t target = VariantID(960903, 2269367, 0x1BF5D00).address();
        static const auto patch_begin = target + VariantOffset(0x207, 0xF5, 0x207).offset();
        static const auto patch_end = target + VariantOffset(0x20C, 0xFA, 0x20C).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? lea(rcx, ptr[rsp + 0x30]) : lea(rcx, ptr[rsp + 0x40]);
                Module::IsNG() ? mov(edx, r12d) : mov(edx, edi);
                mov(rax, reinterpret_cast<uintptr_t>(SetDataFileIndexEx));
                call(rax);
                mov(rcx, r15);
                mov(rax, patch_end);
                jmp(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace AddDataFromReader_1
    {
        // XXX Status: verified
        static const uintptr_t target = VariantID(960903, 2269367, 0x1BF5D00).address();
        static const auto patch_begin = target + VariantOffset(0x421, 0x1AE, 0x421).offset();
        static const auto patch_end = target + VariantOffset(0x429, 0x1B6, 0x429).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                Module::IsNG() ? cmp(byte[rbp + 0xAE], r13b) : cmp(byte[rbp + 0xCE], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace AddDataFromReader_2
    {
        // XXX Status: verified
        static const uintptr_t target = VariantID(960903, 2269367, 0x1BF5D00).address();
        static const auto patch_begin = target + VariantOffset(0x44B, 0x1D0, 0x44B).offset();
        static const auto patch_end = target + VariantOffset(0x451, 0x1D6, 0x451).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                Module::IsNG() ? cmp(byte[rsp + 0x3E], r13b) : cmp(byte[rsp + 0x4E], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace Retrieve
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(1298455, 2269311, 1298455).address();
        static const auto patch_begin = target + 0xB5;
        static const auto patch_end = target + 0xBC;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                lea(rcx, ptr[rbp + 0x148]);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileIndexEx));
                call(rax);
                mov(rcx, ptr[rbp + 0x170]);
                mov(rdx, patch_end);
                jmp(rdx);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace AsyncRetrieve
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(788223, 2269323, 788223).address();
        static const auto patch_begin = target + 0x8C;
        static const auto patch_end = target + 0x93;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                auto isAE = Version(1, 11, 191, 0) == Module::get().version();

                isAE ? lea(rcx, ptr[rdi + 0x148]) : lea(rcx, ptr[rsi + 0x148]);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileIndexEx));
                call(rax);
                isAE ? mov(rcx, ptr[rdi + 0x170]) : mov(rcx, ptr[rsi + 0x170]);
                mov(rdx, patch_end);
                jmp(rdx);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace ReplicateTo
    {
        // XXX Status: noctd
        static const uintptr_t target = RelocationID(873417, 2269310, 873417).address();
        static const auto patch_begin = target + VariantOffset(0x5E, 0xF3, 0x5E).offset();
        static const auto patch_end = target + VariantOffset(0x63, 0xF9, 0x63).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                Module::IsNG() ? cmp(byte[rsp + 0x3E], 0x0) : cmp(byte[rbx + 0xE], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace ReplicateDirTo
    {
        // XXX Status: noctd
        static const uintptr_t target = RelocationID(338420, 2269319, 338420).address();
        static const auto patch_begin = target + VariantOffset(0x19D, 0x190, 0x19D).offset();
        static const auto patch_end = target + VariantOffset(0x1A2, 0x195, 0x1A2).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                cmp(byte[rdi + 0xE], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace Add
    {
        // XXX Status: noctd
        static const uintptr_t target = VariantID(190722, 0, 0x1BF5B50).address();
        static const auto patch_begin = target + 0x5B;
        static const auto patch_end = target + 0x60;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                cmp(byte[rbx + 0xE], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace Register
    {
        // XXX Status: noctd
        static const uintptr_t target = VariantID(1325822, 2269309, 0x1BF0CD0).address();
        static const auto patch_begin = target + 0xD7;
        static const auto patch_end = target + 0xDD;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                cmp(byte[rsp + 0x4E], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace Register1
    {
        // XXX Status: noctd
        static const uintptr_t target = RelocationID(1339549, 2269308, 1339549).address();
        static const auto patch_begin = target + 0xE5;
        static const auto patch_end = target + 0xEA;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                cmp(byte[rbp - 0x22], 0x0);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace QSize
    {
        // XXX Status: noctd
        static const uintptr_t target = VariantID(963665, 2269318, 0x1BF14E0).address();
        static const auto patch_begin = target + 0x5C;
        static const auto patch_end = target + 0x64;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // NOTE: word cmp to byte cmp; unused upper byte is extended index now
                cmp(byte[rbx + 0x156], 0x0);
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
        if (RegisterBSA::InstallHook())
            logger::debug("Installed RegisterBSA hook");
        else
            logger::debug("Failed RegisterBSA hook");

        if (RegisterBSA_1::InstallHook())
            logger::debug("Installed RegisterBSA_1 hook");
        else
            logger::debug("Failed RegisterBSA_1 hook");

        if (RegisterBSA_2::InstallHook())
            logger::debug("Installed RegisterBSA_2 hook");
        else
            logger::debug("Failed RegisterBSA_2 hook");

        if (RegisterBSA_3::InstallHook())
            logger::debug("Installed RegisterBSA_3 hook");
        else
            logger::debug("Failed RegisterBSA_3 hook");

        if (AddDataFromReader::InstallHook())
            logger::debug("Installed AddDataFromReader hook");
        else
            logger::debug("Failed AddDataFromReader hook");

        if (AddDataFromReader_1::InstallHook())
            logger::debug("Installed AddDataFromReader_1 hook");
        else
            logger::debug("Failed AddDataFromReader_1 hook");

        if (AddDataFromReader_2::InstallHook())
            logger::debug("Installed AddDataFromReader_2 hook");
        else
            logger::debug("Failed AddDataFromReader_2 hook");

        if (Retrieve::InstallHook())
            logger::debug("Installed Retrieve hook");
        else
            logger::debug("Failed Retrieve hook");

        if (AsyncRetrieve::InstallHook())
            logger::debug("Installed AsyncRetrieve hook");
        else
            logger::debug("Failed AsyncRetrieve hook");

        if (ReplicateTo::InstallHook())
            logger::debug("Installed ReplicateTo hook");
        else
            logger::debug("Failed ReplicateTo hook");

        if (ReplicateDirTo::InstallHook())
            logger::debug("Installed ReplicateDirTo hook");
        else
            logger::debug("Failed ReplicateDirTo hook");

        if (Module::IsNG())
            ;  // NOTE: Function is deleted in NG.
        else if (Add::InstallHook())
            logger::debug("Installed Add hook");
        else
            logger::debug("Failed Add hook");

        if (Register::InstallHook())
            logger::debug("Installed Register hook");
        else
            logger::debug("Failed Register hook");

        if (Register1::InstallHook())
            logger::debug("Installed Register1 hook");
        else
            logger::debug("Failed Register1 hook");

        if (QSize::InstallHook())
            logger::debug("Installed QSize hook");
        else
            logger::debug("Failed QSize hook");
    }
}
