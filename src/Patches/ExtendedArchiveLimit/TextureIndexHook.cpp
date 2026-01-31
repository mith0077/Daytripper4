#include "PCH.h"

#include "PatchLib.h"
#include "ExtendedArchiveDef.h"
#include "TextureIndexHook.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace ExtendedArchiveLimit::TextureIndexHook
{
    namespace TextureIndexCtor
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(1150937, 0, 1150937).address();
        static const auto patch_begin = target + 0xC8;
        static const auto patch_end = target + 0xCD;

        static const uintptr_t BSReadWriteLockCtor_addr = RelocationID(695801, 0, 695801).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, BSReadWriteLockCtor_addr);
                call(rax);
                mov(rcx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(ConstructTextureIndexEx));
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

    namespace TextureIndexDtor
    {
        // XXX Status: unchecked
        static const uintptr_t target = VariantID(160645, 0, 0x1D32DB0).address();
        static const auto patch_begin = target + 0xD;
        static const auto patch_end = target + 0x12;

        static const uintptr_t BSTArrayDtor_addr = RelocationID(1238367, 0, 1238367).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, BSTArrayDtor_addr);
                call(rax);
                mov(rcx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(DestructTextureIndexEx));
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

    namespace ManagerCtor
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(0, 2275505, 0).address();
        static const auto patch_begin = target + 0x2CA;
        static const auto patch_end = target + 0x2CF;

        static const uintptr_t BSReadWriteLockCtor_addr = RelocationID(0, 2267896, 0).address();

        // FIXME: Address Library v1.11.191 has wrong ID for this function
        static const uintptr_t BSReadWriteLockCtor_addr_1_11_191 = Offset(0x1658FC0).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                if (Version(1, 11, 191, 0) == Module::get().version())
                    mov(rax, BSReadWriteLockCtor_addr_1_11_191);
                else
                    mov(rax, BSReadWriteLockCtor_addr);

                call(rax);
                lea(rcx, ptr[rdi + 0x98DA0]);
                mov(rax, reinterpret_cast<uintptr_t>(ConstructTextureIndexEx));
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

    namespace ManagerDtor
    {
        // XXX Status: unchecked
        static const uintptr_t target = VariantID(492059, 2275512, 0x1D32F50).address();
        static const auto patch_begin = target + VariantOffset(0x1BC, 0x1AD, 0x1BC).offset();
        static const auto patch_end = target + VariantOffset(0x1C1, 0x1B2, 0x1C1).offset();

        static const uintptr_t BSTArrayDtor_addr = RelocationID(1238367, 2275508, 1238367).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, BSTArrayDtor_addr);
                call(rax);
                Module::IsNG() ? lea(rcx, ptr[rdi + 0x98DA0]) : lea(rcx, ptr[r15 + 0x98DA0]);
                mov(rax, reinterpret_cast<uintptr_t>(DestructTextureIndexEx));
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
        static const uintptr_t target = RelocationID(1004193, 0, 1004193).address();
        static const auto patch_begin = target + 0x27;
        static const auto patch_end = target + 0x4A;

        static void CopyDataFileNameIdFrom(TextureIndex::Index* a_pTexIndex, uint16_t a_indexTo, BSResource::ID* a_pIdFrom)
        {
            assert(a_pTexIndex);
            assert(a_pIdFrom);

            auto idTo = GetTextureDataFileNameIdEx(a_pTexIndex, a_indexTo);

            idTo.file = a_pIdFrom->file;
            idTo.ext = a_pIdFrom->ext;
            idTo.dir = a_pIdFrom->dir;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rbx);
                mov(edx, edi);
                mov(r8, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(CopyDataFileNameIdFrom));
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

    namespace GetDataFileIndex
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(775452, 0, 775452).address();
        static const auto patch_begin = target + 0x55;
        static const auto patch_end = target + 0x61;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                push(rax);
                push(r9);
                push(r10);
                push(r11);
                sub(rsp, 0x20);
                mov(edx, r9d);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileNameIdEx));
                call(rax);
                lea(r8, ptr[rax - 0x38]);
                mov(ecx, ptr[r8 + 0x38]);
                add(rsp, 0x20);
                pop(r11);
                pop(r10);
                pop(r9);
                pop(rax);
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

    namespace GetDataFileIndex_1
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(775452, 0, 775452).address();
        static const auto patch_begin = target + 0xAF;
        static const auto patch_end = target + 0xC1;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                push(rax);
                push(r9);
                sub(rsp, 0x20);
                mov(rcx, r11);
                mov(edx, r9d);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileNameIdEx));
                call(rax);
                lea(r8, ptr[rax - 0x38]);
                mov(ecx, ptr[r8 + 0x38]);
                mov(edx, ptr[r8 + 0x3C]);
                add(rsp, 0x20);
                pop(r9);
                pop(rax);
                mov(r10, patch_end);
                jmp(r10);
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
        // XXX Status: verified
        static const uintptr_t target = VariantID(1388147, 2275558, 0x1D38430).address();
        static const auto patch_begin = target + VariantOffset(0x406, 0x33A, 0x406).offset();
        static const auto patch_end = target + VariantOffset(0x40A, 0x33E, 0x40A).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? mov(r15d, r13d) : mov(r9d, r12d);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end, false);
        }
    }

    namespace ProcessEvent_1
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(0, 2275558, 0).address();
        static const auto patch_begin = target + 0x1D4;
        static const auto patch_end = target + 0x1E6;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Xbyak::Label PATCH_END;

                push(r8);
                push(r9);
                push(r10);
                push(r11);
                sub(rsp, 0x20);
                lea(rcx, ptr[r14 - 0x40]);
                mov(edx, r8d);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileNameIdEx));
                call(rax);
                lea(rdx, ptr[rax - 0x38]);
                mov(eax, ptr[rdx + 0x38]);
                mov(ecx, ptr[rdx + 0x3C]);
                add(rsp, 0x20);
                pop(r11);
                pop(r10);
                pop(r9);
                pop(r8);
                jmp(ptr[rip + PATCH_END]);

                L(PATCH_END);
                dq(patch_end);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace ProcessEvent_2
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(0, 2275558, 0).address();
        static const auto patch_begin = target + 0x229;
        static const auto patch_end = target + 0x23B;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Xbyak::Label PATCH_END;

                push(r8);
                push(r9);
                push(r10);
                push(r11);
                sub(rsp, 0x20);
                lea(rcx, ptr[r14 - 0x40]);
                mov(edx, eax);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileNameIdEx));
                call(rax);
                lea(rdx, ptr[rax - 0x38]);
                mov(eax, ptr[rdx + 0x38]);
                mov(ecx, ptr[rdx + 0x3C]);
                add(rsp, 0x20);
                pop(r11);
                pop(r10);
                pop(r9);
                pop(r8);
                jmp(ptr[rip + PATCH_END]);

                L(PATCH_END);
                dq(patch_end);
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
        // XXX Status: verified
        static const uintptr_t target = RelocationID(0, 2275558, 0).address();
        static const auto patch_begin = target + 0x3B2;
        static const auto patch_end = target + 0x3EA;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                lea(rcx, ptr[r14 - 0x40]);
                mov(edx, r15d);
                lea(r8, ptr[rsi + 0x20]);
                mov(rax, reinterpret_cast<uintptr_t>(AddDataFile::CopyDataFileNameIdFrom));
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
        if (Module::IsNG())
            ;  // NOTE: TextureIndexCtor is inlined to ManagerCtor in NG
        else if (TextureIndexCtor::InstallHook())
            logger::debug("Installed TextureIndexCtor hook");
        else
            logger::debug("Failed TextureIndexCtor hook");

        if (Module::IsNG())
            ;  // NOTE: Function is deleted in NG.
        else if (TextureIndexDtor::InstallHook())
            logger::debug("Installed TextureIndexDtor hook");
        else
            logger::debug("Failed TextureIndexDtor hook");

        if (!Module::IsNG())
            ;  // NOTE: TextureIndexCtor is inlined to ManagerCtor in NG
        else if (ManagerCtor::InstallHook())
            logger::debug("Installed ManagerCtor hook");
        else
            logger::debug("Failed TextureIndexCtor hook");

        if (ManagerDtor::InstallHook())
            logger::debug("Installed ManagerDtor hook");
        else
            logger::debug("Failed TextureIndexDtor hook");

        if (Module::IsNG())
            ;  // NOTE: AddDataFile is inlined to ProcessEvent in NG
        else if (AddDataFile::InstallHook())
            logger::debug("Installed AddDataFile hook");
        else
            logger::debug("Failed AddDataFile hook");

        if (Module::IsNG())
            ;  // NOTE: GetDataFileIndex is inlined to ProcessEvent in NG
        else if (GetDataFileIndex::InstallHook())
            logger::debug("Installed GetDataFileIndex hook");
        else
            logger::debug("Failed GetDataFileIndex hook");

        if (Module::IsNG())
            ;  // NOTE: GetDataFileIndex is inlined to ProcessEvent in NG
        else if (GetDataFileIndex_1::InstallHook())
            logger::debug("Installed GetDataFileIndex_1 hook");
        else
            logger::debug("Failed GetDataFileIndex_1 hook");

        if (ProcessEvent::InstallHook())
            logger::debug("Installed ProcessEvent hook");
        else
            logger::debug("Failed ProcessEvent hook");

        if (!Module::IsNG())
            ;  // NOTE: GetDataFileIndex is inlined to ProcessEvent in NG
        else if (ProcessEvent_1::InstallHook())
            logger::debug("Installed ProcessEvent_1 hook");
        else
            logger::debug("Failed ProcessEvent_1 hook");

        if (!Module::IsNG())
            ;  // NOTE: GetDataFileIndex is inlined to ProcessEvent in NG
        else if (ProcessEvent_2::InstallHook())
            logger::debug("Installed ProcessEvent_2 hook");
        else
            logger::debug("Failed ProcessEvent_2 hook");

        if (!Module::IsNG())
            ;  // NOTE: AddDataFile is inlined to ProcessEvent in NG
        else if (ProcessEvent_3::InstallHook())
            logger::debug("Installed ProcessEvent_3 hook");
        else
            logger::debug("Failed ProcessEvent_3 hook");
    }
}
