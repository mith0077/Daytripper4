#include "PCH.h"

#include "PatchLib.h"
#include "GrenadeProjectile.h"

using namespace std;
using namespace RE;
using namespace REL;


namespace GrenadeProjectile
{
    static const uintptr_t BSRandomFloat_addr = ID(1118937).address();
    static const auto BSRandomFloat = reinterpret_cast<float (*)(float, float)>(BSRandomFloat_addr);

    namespace AddInitialVelocity
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x103E140).address();
        static const auto patch_begin = target + 0x12C;
        static const auto patch_end = target + 0x131;

        static bool UsePhysicalThrowing(PlayerCharacter* a_pPlayerCharactor, Projectile* a_pProjectile)
        {
            logger::debug("Entering UsePhysicalThrowing()");

            assert(a_pPlayerCharactor);
            assert(a_pProjectile);

            return a_pProjectile->equipIndex.index == 2;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rdx, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(UsePhysicalThrowing));
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

    namespace AddInitialVelocity_1
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x103E140).address();
        static const auto patch_begin = target + 0x177;
        static const auto patch_end = target + 0x17C;

        static TESObjectWEAP::Data* GetWeaponData(Projectile* a_pProjectile)
        {
            logger::debug("Entering GetWeaponData()");

            if (!a_pProjectile)
                return nullptr;

            auto inst = static_cast<TESObjectWEAP::Data*>(a_pProjectile->weaponSource.instanceData.get());

            if (inst)
                return inst;

            auto weap = static_cast<TESObjectWEAP*>(a_pProjectile->weaponSource.object);

            if (weap)
                return &weap->weaponData;

            return nullptr;
        }

        static void AddVelocity_mult(void* a_pVoid, float* a_pFloat, uint8_t a_uint8, Projectile* a_pProjectile)
        {
            static const uintptr_t AddVelocity_addr = Offset(0x1E15630).address();
            static const auto AddVelocity = reinterpret_cast<void (*)(void*, float*, uint8_t)>(AddVelocity_addr);

            logger::debug("Entering AddVelocity_mult()");

            assert(a_pFloat);
            assert(a_pProjectile);

            if (!a_pProjectile->IsMine()) {
                a_pFloat[0] *= a_pProjectile->speedMult;
                a_pFloat[1] *= a_pProjectile->speedMult;
                a_pFloat[2] *= a_pProjectile->speedMult;
            }

            auto pWeaponData = GetWeaponData(a_pProjectile);
            auto pAimModel = pWeaponData ? pWeaponData->aimModel : nullptr;
            auto pRangedData = pWeaponData ? pWeaponData->rangedData : nullptr;

            if (pAimModel && pRangedData && pRangedData->numProjectiles > 1) {
                float minConeDeg = pAimModel ->aimModelData.aimModelMinConeDegrees;
                minConeDeg = (minConeDeg > 0.0f) ? minConeDeg : 0.0f;
                minConeDeg = (minConeDeg < 15.0f) ? minConeDeg : 15.0f;

                float randMin = 1.0f - minConeDeg * static_cast<float>(numbers::pi) / 180.0f;
                float randMax = 1.0f + minConeDeg * static_cast<float>(numbers::pi) / 180.0f;

                a_pFloat[0] *= BSRandomFloat(randMin, randMax);
                a_pFloat[1] *= BSRandomFloat(randMin, randMax);
                a_pFloat[2] *= BSRandomFloat(randMin, randMax);
            }

            AddVelocity(a_pVoid, a_pFloat, a_uint8);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(r9, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(AddVelocity_mult));
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

    namespace AddInitialVelocity_2
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0x103E140).address();
        static const auto patch_begin = target + 0x1E4;
        static const auto patch_end = target + 0x1E9;

        static void AddAngularVelocity_mult(void* a_pVoid, float* a_pFloat, uint8_t a_uint8, Projectile* a_pProjectile)
        {
            static const uintptr_t AddAngularVelocity_addr = Offset(0x1E156D0).address();
            static const auto AddAngularVelocity = reinterpret_cast<void (*)(void*, float*, uint8_t)>(AddAngularVelocity_addr);

            logger::debug("Entering AddAngularVelocity_mult()");

            assert(a_pFloat);
            assert(a_pProjectile);

            if (!a_pProjectile->IsMine()) {
                float randMin = -1.0f;
                float randMax = 1.0f;

                a_pFloat[0] *= BSRandomFloat(randMin, randMax);
                a_pFloat[1] *= BSRandomFloat(randMin, randMax);
                a_pFloat[2] *= BSRandomFloat(randMin, randMax);
            }

            AddAngularVelocity(a_pVoid, a_pFloat, a_uint8);
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(r9, rsi);
                mov(rax, reinterpret_cast<uintptr_t>(AddAngularVelocity_mult));
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

    namespace LaunchGrenade
    {
        // XXX Status: verified
        static const uintptr_t target = Offset(0xEFA340).address();
        static const auto patch_begin = target + 0x102;
        static const auto patch_end = target + 0x107;

        static TESObjectWEAP::Data* GetWeaponData(ProjectileLaunchData* a_pLaunchData)
        {
            logger::debug("Entering GetWeaponData()");

            if (!a_pLaunchData)
                return nullptr;

            auto weap = static_cast<TESObjectWEAP*>(a_pLaunchData->fromWeapon.object);

            if (weap)
                return &weap->weaponData;

            return nullptr;
        }

        static void* LaunchProjectile_mult(Projectile* a_pProjectile, ProjectileLaunchData* a_pLaunchData)
        {
            static const uintptr_t LaunchProjectile_addr = ID(1452334).address();
            static const auto LaunchProjectile = reinterpret_cast<void* (*)(Projectile*, ProjectileLaunchData*)>(LaunchProjectile_addr);

            logger::debug("Entering LaunchProjectile_mult()");

            assert(a_pProjectile);
            assert(a_pLaunchData);

            auto pWeaponData = GetWeaponData(a_pLaunchData);
            auto pRangedData = pWeaponData ? pWeaponData->rangedData : nullptr;

            uint8_t numProjectiles = 1;

            if (pRangedData && pRangedData->numProjectiles > 1)
                numProjectiles = pRangedData->numProjectiles;

            void* ret = nullptr;

            for (uint8_t i = 0; i < numProjectiles; ++i)
                ret = LaunchProjectile(a_pProjectile, a_pLaunchData);

            return ret;
        }

        struct Patch : Xbyak::CodeGenerator
        {
            Patch()
            {
                mov(rax, reinterpret_cast<uintptr_t>(LaunchProjectile_mult));
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
        if (AddInitialVelocity::InstallHook())
            logger::debug("Installed AddInitialVelocity hook");
        else
            logger::debug("Failed AddInitialVelocity hook");

        if (AddInitialVelocity_1::InstallHook())
            logger::debug("Installed AddInitialVelocity_1 hook");
        else
            logger::debug("Failed AddInitialVelocity_1 hook");

        if (AddInitialVelocity_2::InstallHook())
            logger::debug("Installed AddInitialVelocity_2 hook");
        else
            logger::debug("Failed AddInitialVelocity_2 hook");

        if (LaunchGrenade::InstallHook())
            logger::debug("Installed LaunchGrenade hook");
        else
            logger::debug("Failed LaunchGrenade hook");
    }
}
