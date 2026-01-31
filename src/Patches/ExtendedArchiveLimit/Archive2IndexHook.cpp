#include "PCH.h"

#include "PatchLib.h"
#include "ExtendedArchiveDef.h"
#include "Archive2IndexHook.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace ExtendedArchiveLimit::Archive2IndexHook
{
    namespace Archive2IndexCtor
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(762294, 2269342, 762294).address();
        static const auto patch_begin = target + VariantOffset(0x132, 0x1C5, 0x132).offset();
        static const auto patch_end = target + VariantOffset(0x13A, 0x1CA, 0x13A).offset();

        static const uintptr_t BSReadWriteLockCtor_addr = RelocationID(695801, 2267896, 695801).address();

        // FIXME: Address Library v1.11.191 has wrong ID for this function
        static const uintptr_t BSReadWriteLockCtor_addr_1_11_191 = Offset(0x1658FC0).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                auto reg_self = Module::IsNG() ? r14 : rbx;

                if (Version(1, 11, 191, 0) == Module::get().version())
                    mov(rax, BSReadWriteLockCtor_addr_1_11_191);
                else
                    mov(rax, BSReadWriteLockCtor_addr);

                call(rax);
                mov(rcx, reg_self);
                mov(rax, reinterpret_cast<uintptr_t>(ConstructIndexEx));
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

    namespace Archive2IndexCtor_1
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(762294, 2269342, 762294).address();
        static const auto patch_begin = target + VariantOffset(0x29, 0x35, 0x29).offset();
        static const auto patch_end = target + VariantOffset(0x2E, 0x3A, 0x2E).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // BSTSmallIndexScatterTable(0x200) -> BSTSmallIndexScatterTable(0xFFFF)
                Module::IsNG() ? mov(ecx, 0xFFFF) : mov(edx, 0xFFFF);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace Archive2IndexCtor_2
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(0, 2269342, 0).address();
        static const auto patch_begin = target + 0x88;
        static const auto patch_end = target + 0x8D;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                // BSTSmallIndexScatterTable(0x200) -> BSTSmallIndexScatterTable(0xFFFF)
                mov(ecx, 0xFFFF);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace Archive2IndexDtor
    {
        // XXX Status: noctd
        static const uintptr_t target = RelocationID(928707, 2269348, 928707).address();
        static const auto patch_begin = target + 0x15;
        static const auto patch_end = target + 0x30;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                auto reg_aindex = Module::IsNG() ? rbx : rdi;

                mov(r14, rcx);
                mov(rsi, ptr[rcx + 0x1C30]);
                mov(reg_aindex, rsi);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace Archive2IndexDtor_1
    {
        // XXX Status: noctd
        static const uintptr_t target = RelocationID(928707, 2269348, 928707).address();
        static const auto patch_begin = target + 0x30;
        static const auto patch_end = target + 0x38;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                auto reg_aindex = Module::IsNG() ? rbx : rdi;

                mov(rcx, r14);
                lea(rdx, ptr[reg_aindex - 0x1]);
                mov(rax, reinterpret_cast<uintptr_t>(GetAsyncDataFileEx));
                call(rax);
                mov(rcx, ptr[rax]);
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

    namespace Archive2IndexDtor_2
    {
        // XXX Status: noctd
        static const uintptr_t target = RelocationID(928707, 2269348, 928707).address();
        static const auto patch_begin = target + VariantOffset(0x71, 0x90, 0x71).offset();
        static const auto patch_end = target + VariantOffset(0x79, 0x98, 0x79).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, r14);
                lea(edx, ptr[rsi - 0x1]);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileEx));
                call(rax);
                mov(rcx, ptr[rax]);
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

    namespace Archive2IndexDtor_3
    {
        // XXX Status: noctd
        static const uintptr_t target = RelocationID(928707, 2269348, 928707).address();
        static const auto patch_begin = target + VariantOffset(0xAF, 0xDF, 0xAF).offset();
        static const auto patch_end = target + VariantOffset(0xB4, 0xE4, 0xB4).offset();

        static const uintptr_t DeleteTable_addr = RelocationID(815131, 2268031, 815131).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, DeleteTable_addr);
                call(rax);
                mov(rcx, r14);
                mov(rax, reinterpret_cast<uintptr_t>(DestructIndexEx));
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

    namespace AddDataFile
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(270048, 2269366, 270048).address();
        static const auto patch_begin = target + 0x18;
        static const auto patch_end = target + 0x26;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rbx, rdx);
                mov(rbp, r8);
                mov(rcx, rdi);
                mov(edx, esi);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileEx));
                call(rax);
                mov(rcx, rax);
                mov(rdx, rbx);
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

    namespace AddDataFile_1
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(270048, 2269366, 270048).address();
        static const auto patch_begin = target + 0x2B;
        static const auto patch_end = target + 0x3C;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(edx, esi);
                mov(rax, reinterpret_cast<uintptr_t>(GetAsyncDataFileEx));
                call(rax);
                mov(rcx, ptr[rbx]);
                mov(rdx, rax);
                mov(rax, ptr[rcx]);
                mov(r8, patch_end);
                jmp(r8);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace AddDataFile_2
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(270048, 2269366, 270048).address();
        static const auto patch_begin = target + 0x4E;
        static const auto patch_end = target + 0x56;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                mov(edx, esi);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileNameIdEx));
                call(rax);
                lea(rdx, ptr[rax - 0x1030]);
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

    namespace GetDataFileAt
    {
        // XXX Status: unchecked
        static const auto target = VariantID(548529, 0, 0x1BF7EC0).address();
        static const auto patch_begin = target + 0x17;
        static const auto patch_end = target + 0x25;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(ptr[rsp + 0x30], rdx);
                mov(edx, r8d);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileEx));
                call(rax);
                mov(rcx, ptr[rsp + 0x30]);
                mov(rdx, rax);
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

    namespace GetAsyncDataFileAt
    {
        // XXX Status: unchecked
        static const auto target = VariantID(375552, 0, 0x1BF7E80).address();
        static const auto patch_begin = target + 0x17;
        static const auto patch_end = target + 0x28;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(ptr[rsp + 0x30], rdx);
                mov(edx, r8d);
                mov(rax, reinterpret_cast<uintptr_t>(GetAsyncDataFileEx));
                call(rax);
                mov(rcx, ptr[rsp + 0x30]);
                mov(rdx, rax);
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

    namespace GetDataFileIndex
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(721459, 2269403, 721459).address();
        static const auto patch_begin = target + VariantOffset(0x53, 0x55, 0x53).offset();
        static const auto patch_end = target + VariantOffset(0x5B, 0x5D, 0x5B).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                auto reg_index = Module::IsNG() ? r8d : r9d;
                auto reg_dest = Module::IsNG() ? rdx : r8;

                push(rax);
                push(r8);
                push(r9);
                push(r10);
                push(r11);
                sub(rsp, 0x20);  // Allocate shadow store
                mov(rcx, r11);
                mov(edx, reg_index);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileNameIdEx));
                call(rax);
                add(rsp, 0x20);
                pop(r11);
                pop(r10);
                pop(r9);
                pop(r8);
                lea(reg_dest, ptr[rax - 0x1030]);
                pop(rax);
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

    namespace Retrieve
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(1298455, 2269311, 1298455).address();
        static const auto patch_begin = target + VariantOffset(0xD6, 0xC8, 0xD6).offset();
        static const auto patch_end = target + VariantOffset(0xDB, 0xCD, 0xDB).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rdx, rax);
                lea(r8, ptr[rbp + 0x154]);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileEx));
                call(rax);
                mov(rdx, ptr[rax]);
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

    namespace AsyncRetrieve
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(788223, 2269323, 788223).address();
        static const auto patch_begin = target + VariantOffset(0xB5, 0xAD, 0xB5).offset();
        static const auto patch_end = target + VariantOffset(0xBD, 0xB5, 0xBD).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rdx, rax);
                mov(rax, reinterpret_cast<uintptr_t>(GetAsyncDataFileEx));
                call(rax);
                mov(rdx, ptr[rax]);
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

    namespace ProcessEvent
    {
        // XXX Status: noctd
        static const auto target = VariantID(366360, 2269413, 0x1BF94D0).address();
        static const auto patch_begin = target + VariantOffset(0xF4, 0x94, 0xF4).offset();
        static const auto patch_end = target + VariantOffset(0xF9, 0x99, 0xF9).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                sub(rcx, 0x8);
                mov(rdx, rbp);
                mov(rax, reinterpret_cast<uintptr_t>(GetDataFileEx));
                call(rax);
                mov(rcx, ptr[rax]);
                mov(rdx, rax);
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

    namespace ProcessEvent_1
    {
        // XXX Status: noctd
        static const auto target = VariantID(366360, 2269413, 0x1BF94D0).address();
        static const auto patch_begin = target + VariantOffset(0xFE, 0x9E, 0xFE).offset();
        static const auto patch_end = target + VariantOffset(0x103, 0xA3, 0x103).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(ptr[rdx], r14);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace ProcessEvent_2
    {
        // XXX Status: noctd
        static const auto target = VariantID(366360, 2269413, 0x1BF94D0).address();
        static const auto patch_begin = target + VariantOffset(0x136, 0xD6, 0x136).offset();
        static const auto patch_end = target + VariantOffset(0x13E, 0xDE, 0x13E).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdi);
                sub(rcx, 0x8);
                mov(rdx, rbp);
                mov(rax, reinterpret_cast<uintptr_t>(GetAsyncDataFileEx));
                call(rax);
                mov(rcx, ptr[rax]);
                mov(rdx, rax);
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

    namespace ProcessEvent_3
    {
        // XXX Status: noctd
        static const auto target = VariantID(366360, 2269413, 0x1BF94D0).address();
        static const auto patch_begin = target + VariantOffset(0x143, 0xE3, 0x143).offset();
        static const auto patch_end = target + VariantOffset(0x14B, 0xEB, 0x14B).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(ptr[rdx], r14);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace FUN_141b709d0
    {
        // XXX Status: unchecked
        static const uintptr_t target = VariantID(893405, 2269287, 0x1BEFDE0).address();
        static const auto patch_begin = target + VariantOffset(0x6B, 0x6B, 0x6B).offset();
        static const auto patch_end = target + VariantOffset(0x73, 0x73, 0x73).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rdx);

                // TODO: Retrieve a_index in uint16_t (currently uint8_t)
                //       Currently, code is deemed unreachable
                mov(edx, eax);
                mov(rax, reinterpret_cast<uintptr_t>(GetAsyncDataFileEx));
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
        if (Archive2IndexCtor::InstallHook())
            logger::debug("Installed Archive2IndexCtor hook");
        else
            logger::debug("Failed Archive2IndexCtor hook");

        if (Archive2IndexCtor_1::InstallHook())
            logger::debug("Installed Archive2IndexCtor_1 hook");
        else
            logger::debug("Failed Archive2IndexCtor_1 hook");

        if (!Module::IsNG())
            ;  // NOTE: NG and AE only
        else if (Archive2IndexCtor_2::InstallHook())
            logger::debug("Installed Archive2IndexCtor_2 hook");
        else
            logger::debug("Failed Archive2IndexCtor_2 hook");

        if (Archive2IndexDtor::InstallHook())
            logger::debug("Installed Archive2IndexDtor hook");
        else
            logger::debug("Failed Archive2IndexDtor hook");

        if (Archive2IndexDtor_1::InstallHook())
            logger::debug("Installed Archive2IndexDtor_1 hook");
        else
            logger::debug("Failed Archive2IndexDtor_1 hook");

        if (Archive2IndexDtor_2::InstallHook())
            logger::debug("Installed Archive2IndexDtor_2 hook");
        else
            logger::debug("Failed Archive2IndexDtor_2 hook");

        if (Archive2IndexDtor_3::InstallHook())
            logger::debug("Installed Archive2IndexDtor_3 hook");
        else
            logger::debug("Failed Archive2IndexDtor_3 hook");

        if (AddDataFile::InstallHook())
            logger::debug("Installed AddDataFile hook");
        else
            logger::debug("Failed AddDataFile hook");

        if (AddDataFile_1::InstallHook())
            logger::debug("Installed AddDataFile_1 hook");
        else
            logger::debug("Failed AddDataFile_1 hook");

        if (AddDataFile_2::InstallHook())
            logger::debug("Installed AddDataFile_2 hook");
        else
            logger::debug("Failed AddDataFile_2 hook");

        if (Module::IsNG())
            ;  // NOTE: Function is deleted in NG.
        else if (GetDataFileAt::InstallHook())
            logger::debug("Installed GetDataFileAt hook");
        else
            logger::debug("Failed GetDataFileAt hook");

        if (Module::IsNG())
            ;  // NOTE: Function is deleted in NG.
        else if (GetAsyncDataFileAt::InstallHook())
            logger::debug("Installed GetAsyncDataFileAt hook");
        else
            logger::debug("Failed GetAsyncDataFileAt hook");

        if (GetDataFileIndex::InstallHook())
            logger::debug("Installed GetDataFileIndex hook");
        else
            logger::debug("Failed GetDataFileIndex hook");

        if (Retrieve::InstallHook())
            logger::debug("Installed Retrieve hook");
        else
            logger::debug("Failed Retrieve hook");

        if (AsyncRetrieve::InstallHook())
            logger::debug("Installed AsyncRetrieve hook");
        else
            logger::debug("Failed AsyncRetrieve hook");

        if (ProcessEvent::InstallHook())
            logger::debug("Installed ProcessEvent hook");
        else
            logger::debug("Failed ProcessEvent hook");

        if (ProcessEvent_1::InstallHook())
            logger::debug("Installed ProcessEvent_1 hook");
        else
            logger::debug("Failed ProcessEvent_1 hook");

        if (ProcessEvent_2::InstallHook())
            logger::debug("Installed ProcessEvent_2 hook");
        else
            logger::debug("Failed ProcessEvent_2 hook");

        if (ProcessEvent_3::InstallHook())
            logger::debug("Installed ProcessEvent_3 hook");
        else
            logger::debug("Failed ProcessEvent_3 hook");

        if (FUN_141b709d0::InstallHook())
            logger::debug("Installed FUN_141b709d0 hook");
        else
            logger::debug("Failed FUN_141b709d0 hook");
    }
}
