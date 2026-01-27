#include "PCH.h"

#include "PatchLib.h"

using namespace std;
using namespace REL;


bool InstallPatch(Xbyak::CodeGenerator& a_patch, uintptr_t a_begin, uintptr_t a_end, bool a_allowThunk)
{
    if (a_begin > a_end)
        return false;

    a_patch.ready();
    auto writeSize = a_end - a_begin;
    auto patchSize = a_patch.getSize();

    if (writeSize >= patchSize)
        ;
    else if (!a_allowThunk)
        return false;
    else if (5 > writeSize)
        return false;

    safe_fill(a_begin, NOP, writeSize);

    if (writeSize >= patchSize)
        safe_write(a_begin, a_patch.getCode(), patchSize);
    else {
        auto& trampoline = F4SE::GetTrampoline();
        trampoline.write_branch<5>(a_begin, trampoline.allocate(a_patch));
    }

    return true;
}
