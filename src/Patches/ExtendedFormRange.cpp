#include "PCH.h"

#include "ExtendedFormRange.h"

using namespace std;
using namespace REL;


namespace ExtendedFormRange
{
    // and rcx, 0xffffff -> and rcx, 0xffffffff
    static const uintptr_t target = ID(1583129).address();
    static const auto patch_begin = target + 0x101;
    static const auto patch_end = target + 0x102;

    void InstallHook()
    {
        safe_fill(patch_begin, 0xFF, patch_end - patch_begin);
    }
}
