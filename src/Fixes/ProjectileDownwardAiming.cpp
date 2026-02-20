#include "PCH.h"

#include "PatchLib.h"
#include "ProjectileDownwardAiming.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace ProjectileDownwardAiming
{
    namespace UpdateImpl
    {
        // XXX Status: verified
        static const uintptr_t target = RelocationID(1470408, 2236880, 1470408).address();
        static const auto patch_begin = target + VariantOffset(0x928, 0xC8C, 0x921).offset();
        static const auto patch_end = target + VariantOffset(0x966, 0xCCA, 0x95C).offset();

        constexpr float MAX_RAYCAST_DISTANCE = 256.0;
        constexpr bool SKIP_Z_COMPARISON = true;

        static bool GetVrHmdPos(Actor* a_pActor, NiPoint3& a_posHmd)
        {
            static const auto strVrHmdNode = BSFixedString("HmdNode");

            if (!Module::IsVR())
                return false;

            auto pActorNode = a_pActor->Get3D();

            if (!pActorNode)
                return false;

            auto pControlNode = pActorNode->parent;

            if (!pControlNode)
                return false;

            // auto pHmdNode = pControlNode->GetObjectByName(strVrHmdNode);  // XXX broken
            auto pHmdNode = BSUtilities::GetObjectByName(pControlNode, strVrHmdNode, true, true);

            if (!pHmdNode)
                return false;

            a_posHmd = pHmdNode->world.translate;

            return true;
        }

        static void WriteRayCastSource(NiPoint3* a_pPosRaySrc, Projectile* a_pProjectile, Actor* a_pShooter)
        {
            logger::debug("Entering WriteRayCastSource()");

            assert(a_pPosRaySrc);
            assert(a_pProjectile);
            assert(a_pShooter);

            NiPoint3& posRayDst = a_pProjectile->data.location;

            NiPoint3 posShootOrigin, posPlaceholder;
            if (!a_pShooter->IsPlayerRef())
                posShootOrigin = a_pShooter->data.location;
            else if (GetVrHmdPos(a_pShooter, posShootOrigin))
                ; // NOTE: GetEyeVector() won't work in VR, so take HMD instead
            else
                a_pShooter->GetEyeVector(posShootOrigin, posPlaceholder, true);

            if (posRayDst.GetDistance(posShootOrigin) > MAX_RAYCAST_DISTANCE) {
                // Disable ray casting by setting Src identical to Dst
                a_pPosRaySrc->x = posRayDst.x;
                a_pPosRaySrc->y = posRayDst.y;
                a_pPosRaySrc->z = posRayDst.z;
            }
            else if (!SKIP_Z_COMPARISON && posRayDst.z > posShootOrigin.z) {
                // NOTE: Ray casting on z-axis was disabled originally, if projectile
                //       is above player. Won't necessary now for we took eye position.
                a_pPosRaySrc->x = posShootOrigin.x;
                a_pPosRaySrc->y = posShootOrigin.y;
                a_pPosRaySrc->z = posRayDst.z;
            }
            else {
                a_pPosRaySrc->x = posShootOrigin.x;
                a_pPosRaySrc->y = posShootOrigin.y;
                a_pPosRaySrc->z = posShootOrigin.z;
            }
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                auto reg_self = Module::IsVR() ? r14 : r15;
                auto stack_raySrc = Module::IsNG() ? 0x40 : 0x60;

                lea(rcx, ptr[rsp + stack_raySrc]);
                mov(rdx, reg_self);
                mov(r8, rbx);
                mov(rax, reinterpret_cast<uintptr_t>(WriteRayCastSource));
                call(rax);
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
        if (UpdateImpl::InstallHook())
            logger::debug("Installed UpdateImpl hook");
        else
            logger::debug("Failed UpdateImpl hook");
    }
}
