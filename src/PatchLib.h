#pragma once

#include "PCH.h"

#include <xbyak/xbyak.h>

inline constexpr REL::Version RUNTIME_1_11_191{ 1, 11, 191, 0 };
inline bool IsAE() { return REL::Module::get().version() == RUNTIME_1_11_191; };

bool InstallPatch(Xbyak::CodeGenerator& a_patch, std::uintptr_t a_begin, std::uintptr_t a_end, bool a_allowThunk = true);
