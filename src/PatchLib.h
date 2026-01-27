#pragma once

#include "PCH.h"

#include <xbyak/xbyak.h>


bool InstallPatch(Xbyak::CodeGenerator& a_patch, std::uintptr_t a_begin, std::uintptr_t a_end, bool a_allowThunk = true);
