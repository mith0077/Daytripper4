# Daytripper4

Engine fixes for those looking for a happy escape into Virtual Reality.

* [Fallout4](https://www.nexusmods.com/fallout4/mods/91141)

# Build Requirement (tools)
* [Visual Studio](https://visualstudio.microsoft.com/)
* [git](https://git-scm.com)
* [CMake](https://cmake.org)

# Build Dependencies (clone into /external)
* [CommonLibF4 VR enabled](https://github.com/alandtse/CommonLibF4)
* [vcpkg](https://github.com/microsoft/vcpkg)

# Build Dependencies (via vcpkg)
* [args](https://github.com/Taywee/args)
* [rapidcsv](https://github.com/d99kris/rapidcsv/)
* [mmio](https://github.com/Ryan-rsm-McKenzie/mmio)
* [spdlog](https://github.com/gabime/spdlog)
* [Xbyak](https://github.com/herumi/xbyak)
* [catch2](https://github.com/catchorg/Catch2)
* [fmt](https://github.com/fmtlib/fmt)
* [tomlplusplus](https://marzer.github.io/tomlplusplus/)

# End User Dependencies
* [Address Library for F4SE Plugins](https://www.nexusmods.com/fallout4/mods/47327)
* [VR Address Library for F4SEVR](https://www.nexusmods.com/fallout4/mods/64879)
* [F4SE](https://f4se.silverlock.org/)
* [F4SEVR](https://f4se.silverlock.org/)
* [Microsoft Visual C++ Redistributable for Visual Studio 2022](https://aka.ms/vs/17/release/vc_redist.x64.exe)

# Building
> mkdir Daytripper4
> cd Daytripper4
> tar -xf %HOME%\Downloads\Daytripper4_src-x.y.z.zip
> git clone https://github.com/alandtse/CommonLibF4 external/CommonLibF4
> git clone https://github.com/microsoft/vcpkg external/vcpkg
> cmake --preset vs2022-windows-vcpkg
> cmake --build build --config Release

# Credits
* [Ryan McKenzie](https://github.com/Ryan-rsm-McKenzie) - Original author of CommonLibF4.
* [Alan Tse](https://github.com/alandtse) - VR port of CommonLibF4 and splendid pdb files. Boilerplate codes are also cloned from their Buffout4 NG. Includes CMake files, Settings.h and main.cpp.
* [aers](https://github.com/aers) - ProjectileDownwardAiming and ReferenceHandleCap are Fallout 4 ports of their EngineFixesSkyrim64.
* [nightfallstorm](https://github.com/Nightfallstorm) - ESL support for Fallout 4 is heavily inspired by their SkyrimVRESL.

# License
[MIT](LICENSE)
