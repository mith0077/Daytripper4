#include "PCH.h"

#include "LoadTESInfoHook.h"

using namespace std;
using namespace REL;


namespace SmallFileLoader::LoadTESInfoHook
{
    // and eax, 0x191 -> and eax, 0x391
    static const uintptr_t target = ID(883999).address();
    static const auto patch_begin = target + 0x564;
    static const auto patch_end = target + 0x565;

    void InstallHook()
    {
        safe_fill(patch_begin, 0x03, patch_end - patch_begin);
    }
}
