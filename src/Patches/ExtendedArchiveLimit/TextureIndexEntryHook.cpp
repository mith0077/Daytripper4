#include "PCH.h"

#include "PatchLib.h"
#include "ExtendedArchiveDef.h"
#include "TextureIndexEntryHook.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace ExtendedArchiveLimit::TextureIndexEntryHook
{
    namespace CreateStreamingDDSTexture
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(823682, 2277293, 823682).address();
        static const auto patch_begin = target + VariantOffset(0xA1, 0x91, 0xA1).offset();
        static const auto patch_end = target + VariantOffset(0xAA, 0x9A, 0xAA).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                mov(word[r14 + 0x12], ax);
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

    namespace CreateStreamingDDSTexture_1
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(823682, 2277293, 823682).address();
        static const auto patch_begin = target + VariantOffset(0x187, 0x17C, 0x187).offset();
        static const auto patch_end = target + VariantOffset(0x18C, 0x181, 0x18C).offset();

        uint8_t IsStreamDataAvailable(TextureIndex::Index::Entry* a_pTexEntry)
        {
            if (GetTextureDataFileIndexEx(a_pTexEntry) > 255)
                return 0;
            else
                return 1;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(IsStreamDataAvailable));
                call(rax);
                // HACK: Disable SteramingTextureUpgrade for extended texture archives
                or_(byte[r15 + 0x3F], al);
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

    namespace CreateStreamingTexture
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(917602, 2276914, 917602).address();
        static const auto patch_begin = target + 0x53;
        static const auto patch_end = target + 0x5A;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                mov(ebx, eax);
                mov(rcx, rsi);
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

    namespace CreateStreamingTextureArraySlice
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(165734, 2276937, 165734).address();
        static const auto patch_begin = target + VariantOffset(0x4F, 0x5F, 0x4F).offset();
        static const auto patch_end = target + VariantOffset(0x58, 0x68, 0x58).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                mov(word[r14 + 0x12], ax);
                mov(rcx, rsi);
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

    namespace RequestPrefetch
    {
        // XXX Status: unchecked
        static const uintptr_t target = RelocationID(1140745, 2275565, 1140745).address();
        static const auto patch_begin = target + VariantOffset(0xDD, 0xDE, 0xDD).offset();
        static const auto patch_end = target + VariantOffset(0xE6, 0xE8, 0xE6).offset();

        static const uintptr_t LockForRead_addr = RelocationID(1573164, 2267897, 1573164).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, LockForRead_addr);
                call(rax);
                mov(rcx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(GetTextureDataFileIndexEx));
                call(rax);
                Module::IsNG() ? mov(r9d, eax) : mov(ecx, eax);
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

    namespace ProcessEvent
    {
        // XXX Status: verified
        static const uintptr_t target = VariantID(1388147, 2275558, 0x1D38430).address();
        static const auto patch_begin = target + VariantOffset(0x255, 0x2BB, 0x255).offset();
        static const auto patch_end = target + VariantOffset(0x25A, 0x2C0, 0x25A).offset();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Module::IsNG() ? lea(rcx, ptr[rsp + 0x30]) : lea(rcx, ptr[rsp + 0x50]);
                Module::IsNG() ? mov(edx, r13d) : mov(edx, r12d);
                mov(rax, reinterpret_cast<uintptr_t>(SetTextureDataFileIndexEx));
                call(rax);
                mov(rcx, rsi);
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

    namespace NativeDescCtor
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(964490, 2275502, 964490).address();
        static const auto patch_begin = target + 0x74;
        static const auto patch_end = target + 0x79;

        static const uintptr_t BSFixedStringCtor_addr = RelocationID(999093, 2268667, 999093).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, BSFixedStringCtor_addr);
                call(rax);
                mov(rcx, rdi);
                mov(rdx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(CopyTextureDataFileIndexEx));
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

    namespace NativeDescOpEq
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(1553349, 2275515, 1553349).address();
        static const auto patch_begin = target + VariantOffset(0x59, 0x58, 0x59).offset();
        static const auto patch_end = target + VariantOffset(0x5E, 0x5D, 0x5E).offset();

        static const uintptr_t FixedStringOpEq_addr = RelocationID(533176, 2268672, 533176).address();

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, FixedStringOpEq_addr);
                call(rax);
                mov(rcx, rdi);
                mov(rdx, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(CopyTextureDataFileIndexEx));
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

    namespace ChunkDescOpEq
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(547291, 0, 547291).address();
        static const auto patch_begin = target + VariantOffset(0x0, 0, 0x0).offset();
        static const auto patch_end = target + VariantOffset(0x26, 0, 0x26).offset();

        static BSTextureStreamer::ChunkDesc* CopyChunk(BSTextureStreamer::ChunkDesc* a_pDstChunk, BSTextureStreamer::ChunkDesc* a_pSrcChunk)
        {
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
                mov(rax, reinterpret_cast<uintptr_t>(CopyChunk));
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
        if (CreateStreamingDDSTexture::InstallHook())
            logger::debug("Installed CreateStreamingDDSTexture hook");
        else
            logger::debug("Failed CreateStreamingDDSTexture hook");

        if (CreateStreamingDDSTexture_1::InstallHook())
            logger::debug("Installed CreateStreamingDDSTexture_1 hook");
        else
            logger::debug("Failed CreateStreamingDDSTexture_1 hook");

        if (CreateStreamingTexture::InstallHook())
            logger::debug("Installed CreateStreamingTexture hook");
        else
            logger::debug("Failed CreateStreamingTexture hook");

        if (CreateStreamingTextureArraySlice::InstallHook())
            logger::debug("Installed CreateStreamingTextureArraySlice hook");
        else
            logger::debug("Failed CreateStreamingTextureArraySlice hook");

        if (RequestPrefetch::InstallHook())
            logger::debug("Installed RequestPrefetch hook");
        else
            logger::debug("Failed RequestPrefetch hook");

        if (ProcessEvent::InstallHook())
            logger::debug("Installed ProcessEvent hook");
        else
            logger::debug("Failed ProcessEvent hook");

        if (NativeDescCtor::InstallHook())
            logger::debug("Installed NativeDescCtor hook");
        else
            logger::debug("Failed NativeDescCtor hook");

        if (NativeDescOpEq::InstallHook())
            logger::debug("Installed NativeDescOpEq hook");
        else
            logger::debug("Failed NativeDescOpEq hook");

        if (ChunkDescOpEq::InstallHook())
            logger::debug("Installed ChunkDescOpEq hook");
        else
            logger::debug("Failed ChunkDescOpEq hook");
    }
}
