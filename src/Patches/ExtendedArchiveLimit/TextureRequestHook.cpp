#include "PCH.h"

#include "PatchLib.h"
#include "ExtendedArchiveDef.h"
#include "TextureRequestHook.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace ExtendedArchiveLimit::TextureRequestHook
{
    namespace DecompressStreamedLoad
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(1296411, 2275576, 1296411).address();
        static const auto patch_begin = target + VariantOffset(0x62, 0x3D, 0x62).offset();
        static const auto patch_end = target + VariantOffset(0x67, 0x42, 0x67).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? mov(rcx, r14) : mov(rcx, r15);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                Module::IsNG() ? mov(r8d, eax) : mov(ecx, eax);
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

    namespace HandleModifyRequest
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(585923, 0, 585923).address();
        static const auto patch_begin = target + 0x1D0;
        static const auto patch_end = target + 0x1D8;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(dx, word[rsp + 0x4A]);
                mov(rax, reinterpret_cast<uintptr_t>(SetTextureDataFileIndexEx));
                call(rax);
                mov(rcx, rdi);
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

    namespace LoadChunks
    {
        // XXX Status: unchecked
        static const uintptr_t target = RelocationID(979945, 2275550, 979945).address();
        static const auto patch_begin = target + 0xA;
        static const auto patch_end = target + 0x11;
        static const auto erase_begin = target + VariantOffset(0x27, 0x32, 0x27).offset();
        static const auto erase_end = target + VariantOffset(0x2B, 0x36, 0x2B).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rbx, rcx);
                mov(rdi, rdx);
                mov(rcx, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                mov(rdx, rdi);
                mov(rcx, rbx);
                mov(ebx, eax);
                cmp(dword[rdx + 0x104], 0x1);
                mov(rax, patch_end);
                jmp(rax);
            }
        };

        inline bool InstallHook()
        {
            safe_fill(erase_begin, NOP, erase_end - erase_begin);

            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace StartStreamingChunks
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(844246, 0, 844246).address();
        static const auto patch_begin = target + 0x3D;
        static const auto patch_end = target + 0x42;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, r14);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                mov(edx, eax);
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

    namespace ThreadProc
    {
        // XXX Status: verified
        static const uintptr_t target = VariantID(989173, 2275577, 0x1D3A4D0).address();
        static const auto patch_begin = target + VariantOffset(0x48F, 0x9AD, 0x48F).offset();
        static const auto patch_end = target + VariantOffset(0x494, 0x9B2, 0x494).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? mov(rcx, r13) : mov(rcx, r14);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                Module::IsNG() ? mov(r8d, eax) : mov(ecx, eax);
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

    namespace ThreadProc_1
    {
        // XXX Status: verified
        static const uintptr_t target = VariantID(0, 2275577, 0x0).address();
        static const auto patch_begin = target + 0x10CB;
        static const auto patch_end = target + 0x10D0;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, r13);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                mov(r8d, eax);
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

    namespace ThreadProc_2
    {
        // XXX Status: verified
        static const uintptr_t target = VariantID(0, 2275577, 0x0).address();
        static const auto patch_begin = target + 0x82E;
        static const auto patch_end = target + 0x836;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(dx, word[rbp - 0x16]);
                mov(rax, reinterpret_cast<uintptr_t>(SetTextureDataFileIndexEx));
                call(rax);
                mov(rcx, r14);
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

    namespace ChunkDescOpEq
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(547291, 2275458, 547291).address();
        static const auto patch_begin = target + 0x0;
        static const auto patch_end = target + 0x26;

        static BSTextureStreamer::ChunkDesc* CopyChunkDesc(BSTextureStreamer::ChunkDesc* a_pDstChunk, BSTextureStreamer::ChunkDesc* a_pSrcChunk)
        {
            // logger::debug("Entering CopyChunkDesc()");

            assert(a_pDstChunk);
            assert(a_pSrcChunk);

            a_pDstChunk->dataFileOffset = a_pSrcChunk->dataFileOffset;
            a_pDstChunk->size = a_pSrcChunk->size;
            a_pDstChunk->uncompressedSize = a_pSrcChunk->uncompressedSize;
            a_pDstChunk->mipFirst = a_pSrcChunk->mipFirst;
            a_pDstChunk->mipLast = a_pSrcChunk->mipLast;
            a_pDstChunk->padding = a_pSrcChunk->padding;

            return a_pDstChunk;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(CopyChunkDesc));
                jmp(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace StreamingTextureUpgrade
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(580617, 2277276, 580617).address();
        static const auto patch_begin = target + VariantOffset(0x11E, 0x117, 0x11E).offset();
        static const auto patch_end = target + VariantOffset(0x126, 0x11F, 0x126).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? mov(rcx, ptr[rbp + 0x18]) : mov(rcx, ptr[r14 + 0x18]);
                movzx(edx, byte[rcx + 0x4]);
                mov(rax, ptr[rsp + 0x68]);
                add(edx, dword[rax + 0x14]);  // chunks[0].padding
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

    void InstallHook()
    {
        if (DecompressStreamedLoad::InstallHook())
            logger::debug("Installed DecompressStreamedLoad hook");
        else
            logger::debug("Failed DecompressStreamedLoad hook");

        if (Module::IsNG())
            ;  // NOTE: HandleModifyRequest is inlined to ThreadProc in NG
        else if (HandleModifyRequest::InstallHook())
            logger::debug("Installed HandleModifyRequest hook");
        else
            logger::debug("Failed HandleModifyRequest hook");

        if (LoadChunks::InstallHook())
            logger::debug("Installed LoadChunks hook");
        else
            logger::debug("Failed LoadChunks hook");

        if (Module::IsNG())
            ;  // NOTE: StartStreamingChunks is inlined to ThreadProc in NG
        else if (StartStreamingChunks::InstallHook())
            logger::debug("Installed StartStreamingChunks hook");
        else
            logger::debug("Failed StartStreamingChunks hook");

        if (ThreadProc::InstallHook())
            logger::debug("Installed ThreadProc hook");
        else
            logger::debug("Failed ThreadProc hook");

        if (!Module::IsNG())
            ;  // NOTE: StartStreamingChunks is inlined to ThreadProc in NG
        else if (ThreadProc_1::InstallHook())
            logger::debug("Installed ThreadProc_1 hook");
        else
            logger::debug("Failed ThreadProc_1 hook");

        if (!Module::IsNG())
            ;  // NOTE: HandleModifyRequest is inlined to ThreadProc in NG
        else if (ThreadProc_2::InstallHook())
            logger::debug("Installed ThreadProc_2 hook");
        else
            logger::debug("Failed ThreadProc_2 hook");

        if (ChunkDescOpEq::InstallHook())
            logger::debug("Installed ChunkDescOpEq hook");
        else
            logger::debug("Failed ChunkDescOpEq hook");

        if (StreamingTextureUpgrade::InstallHook())
            logger::debug("Installed StreamingTextureUpgrade hook");
        else
            logger::debug("Failed StreamingTextureUpgrade hook");
    }
}
