#include "PCH.h"

#include "PatchLib.h"
#include "RejectInvalidMaterial.h"

using namespace std;
using namespace RE;
using namespace RE::BSGraphics;
using namespace REL;


namespace RejectInvalidMaterial
{
    struct MaterialStruct
    {
        void* _Unk0[12];
        VertexShader* vertexShader;
        HullShader* hullShader;
        DomainShader* domainShader;
        PixelShader* pixelShader;
    };

    struct RenderPassStruct
    {
        void* _Unk0[1];
        BSShader* shader;
        void* _Unk1[5];
        void* lightArray;
        void* _Unk2[1];
        uint32_t flags;
    };

    static void FixAmbientOnlyLight(BSShader* a_pShader, RenderPassStruct* a_pRenderPass)
    {
        static const uintptr_t LightShader_vtaddr = ID(65824).address();

        // logger::debug("Entering ValidateAmbientOnlyLihgt()");

        assert(a_pShader);
        assert(a_pRenderPass);

        auto vtaddr = reinterpret_cast<uintptr_t*>(a_pShader)[0];

        if (LightShader_vtaddr != vtaddr)
            return;
        if ((a_pRenderPass->flags & 0x2000003) == 0)
            return;
        if (a_pRenderPass->lightArray)
            return;

        logger::debug("Rejecting ambient only flag from light without associated lightArray");

        a_pRenderPass->flags ^= 0x2000000;
    }

    static MaterialStruct* ValidateMaterial(BSShader* a_pShader, MaterialStruct* a_pMaterial, RenderPassStruct* a_pRenderPass)
    {
        static const uintptr_t UtilityShader_vtaddr = ID(417167).address();

        // logger::debug("Entering ValidateMaterial()");

        assert(a_pShader);
        assert(a_pRenderPass);

        if (!a_pMaterial)
            return nullptr;

        auto vtaddr = reinterpret_cast<uintptr_t*>(a_pShader)[0];

        if (UtilityShader_vtaddr == vtaddr)
            return a_pMaterial;

        if (!a_pMaterial->vertexShader) {
            logger::debug("CreateCommandBuffer() yielded null for vertex shader, vtaddr: {:x}", vtaddr - Module::get().base());

            return nullptr;
        }

        if (!a_pMaterial->pixelShader) {
            logger::debug("CreateCommandBuffer() yielded null for pixel shader, vtaddr: {:x}", vtaddr - Module::get().base());

            return nullptr;
        }

        FixAmbientOnlyLight(a_pShader, a_pRenderPass);

        return a_pMaterial;
    }

    namespace GeometryTaskFun
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x28AAE20).address();
        static const auto patch_begin = target + 0x7E;
        static const auto patch_end = target + 0x87;
        static const auto label_invalid = target + 0x9A;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                Xbyak::Label CASE_THEN, CASE_ELSE;

                mov(rcx, ptr[rdi + 0x8]);
                mov(rdx, rax);
                mov(r8, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(ValidateMaterial));
                call(rax);
                test(rax, rax);
                jz(CASE_ELSE);

                L(CASE_THEN);
                mov(rcx, rdi);
                mov(rdx, rax);
                mov(rsi, rax);
                mov(rax, patch_end);
                jmp(rax);

                L(CASE_ELSE);
                mov(rax, label_invalid);
                jmp(rax);
            }
        };

        inline bool InstallHook()
        {
            Patch p;

            return InstallPatch(p, patch_begin, patch_end);
        }
    }

    namespace GeometryTaskFun_1
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x28AAE20).address();
        static const auto patch_begin = target + 0x9A;
        static const auto patch_end = target + 0xA1;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rcx, ptr[rdi + 0x8]);
                mov(rdx, rsi);
                mov(r8, rdi);
                mov(rax, reinterpret_cast<uintptr_t>(ValidateMaterial));
                call(rax);
                mov(rsi, rax);
                mov(rcx, ptr[rbp + 0x420]);
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

    static void* FUN_142813a00_checked(void* a_param1, void** a_param2)
    {
        static const uintptr_t FUN_142813A00_addr = Offset(0x2813A00).address();
        static const auto FUN_142813A00 = reinterpret_cast<void* (*)(void*, void**)>(FUN_142813A00_addr);

        // logger::debug("Entering FUN_142813a00_checked()");

        if (nullptr == a_param2[5]) {
            logger::debug("Skipping CreateCommandBuffer() due to invalid material");

            return nullptr;
        }

        return FUN_142813A00(a_param1, a_param2);
    }

    namespace BSEffectShader_CreateCommandBuffer
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x28D2AF0).address();
        static const auto patch_begin = target + 0x6AB;
        static const auto patch_end = target + 0x6B0;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(FUN_142813a00_checked));
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

    namespace BSWaterShader_CreateCommandBuffer
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x289F9C0).address();
        static const auto patch_begin = target + 0x66D;
        static const auto patch_end = target + 0x672;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(FUN_142813a00_checked));
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

    namespace BSUtilityShader_CreateCommandBuffer
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x28F4500).address();
        static const auto patch_begin = target + 0x478;
        static const auto patch_end = target + 0x47D;

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(FUN_142813a00_checked));
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

    namespace RenderPassImmediately
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x2891040).address();
        static const auto patch_begin = target + 0x3D;
        static const auto patch_end = target + 0x42;

        static bool BeginPass_checked(uint32_t a_flags, BSShader* a_pShader, void* a_pParam, RenderPassStruct* a_pRenderPass)
        {
            static const uintptr_t BeginPass_addr = Offset(0x2891280).address();
            static const auto BeginPass = reinterpret_cast<bool (*)(uint32_t, BSShader*, void*)>(BeginPass_addr);

            assert(a_pShader);
            assert(a_pParam);
            assert(a_pRenderPass);

            FixAmbientOnlyLight(a_pShader, a_pRenderPass);

            return BeginPass(a_flags, a_pShader, a_pParam);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(r9, r14);
                mov(rax, reinterpret_cast<uintptr_t>(BeginPass_checked));
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
        if (GeometryTaskFun::InstallHook())
            logger::debug("Installed GeometryTaskFun hook");
        else
            logger::debug("Failed GeometryTaskFun hook");

        if (GeometryTaskFun_1::InstallHook())
            logger::debug("Installed GeometryTaskFun_1 hook");
        else
            logger::debug("Failed GeometryTaskFun_1 hook");

        if (BSEffectShader_CreateCommandBuffer::InstallHook())
            logger::debug("Installed BSEffectShader_CreateCommandBuffer hook");
        else
            logger::debug("Failed BSEffectShader_CreateCommandBuffer hook");

        if (BSWaterShader_CreateCommandBuffer::InstallHook())
            logger::debug("Installed BSWaterShader_CreateCommandBuffer hook");
        else
            logger::debug("Failed BSWaterShader_CreateCommandBuffer hook");

        if (BSUtilityShader_CreateCommandBuffer::InstallHook())
            logger::debug("Installed BSUtilityShader_CreateCommandBuffer hook");
        else
            logger::debug("Failed BSUtilityShader_CreateCommandBuffer hook");

        if (RenderPassImmediately::InstallHook())
            logger::debug("Installed RenderPassImmediately hook");
        else
            logger::debug("Failed RenderPassImmediately hook");
    }
}
