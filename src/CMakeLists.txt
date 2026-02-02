# ---- Options ----

macro(find_commonlib_path)
    if(CommonLibName AND NOT ${CommonLibName} STREQUAL "")
        # Check extern
        find_path(CommonLibPath
            include/REL/Relocation.h
            PATHS ${ROOT_DIR}/external/${CommonLibName}/${CommonLibName}/)

        if(${CommonLibPath} STREQUAL "CommonLibPath-NOTFOUND")
            # Check path
            set_from_environment(${CommonLibName}Path)
            set(CommonLibPath ${${CommonLibName}Path})
        endif()
    endif()
endmacro()

set(CommonLibName "CommonLibF4")
set_root_directory()

find_commonlib_path()
message(
    STATUS
    "Building ${PROJECT_NAME} ${PROJECT_VERSION} for ${FalloutVersion} with ${CommonLibName} at ${CommonLibPath}."
)

set(SOURCE_DIR "${ROOT_DIR}/src")
set(SOURCE_FILES
    "${SOURCE_DIR}/main.cpp"
    "${SOURCE_DIR}/PCH.cpp"
    "${SOURCE_DIR}/PCH.h"
    "${SOURCE_DIR}/Settings.h"
    "${SOURCE_DIR}/DefaultObjectHook.cpp"
    "${SOURCE_DIR}/DefaultObjectHook.h"
    "${SOURCE_DIR}/DefaultObjectLib.cpp"
    "${SOURCE_DIR}/DefaultObjectLib.h"
    "${SOURCE_DIR}/DialogLib.h"
    "${SOURCE_DIR}/DialogLib.cpp"
    "${SOURCE_DIR}/PatchLib.cpp"
    "${SOURCE_DIR}/PatchLib.h"
    "${SOURCE_DIR}/Fixes/BookMenuText.cpp"
    "${SOURCE_DIR}/Fixes/BookMenuText.h"
    "${SOURCE_DIR}/Fixes/DlcPreCulling.cpp"
    "${SOURCE_DIR}/Fixes/DlcPreCulling.h"
    "${SOURCE_DIR}/Fixes/Fixes.cpp"
    "${SOURCE_DIR}/Fixes/Fixes.h"
    "${SOURCE_DIR}/Fixes/GrenadeProjectile.cpp"
    "${SOURCE_DIR}/Fixes/GrenadeProjectile.h"
    "${SOURCE_DIR}/Fixes/NiExtraDataTyping.cpp"
    "${SOURCE_DIR}/Fixes/NiExtraDataTyping.h"
    "${SOURCE_DIR}/Fixes/RobotModMenu.cpp"
    "${SOURCE_DIR}/Fixes/RobotModMenu.h"
    "${SOURCE_DIR}/Fixes/WetnessMaterialSsr.cpp"
    "${SOURCE_DIR}/Fixes/WetnessMaterialSsr.h"
    "${SOURCE_DIR}/Fixes/ProjectileDownwardAiming.cpp"
    "${SOURCE_DIR}/Fixes/ProjectileDownwardAiming.h"
    "${SOURCE_DIR}/Patches/CreationClubLib.cpp"
    "${SOURCE_DIR}/Patches/CreationClubLib.h"
    "${SOURCE_DIR}/Patches/CreationClubSupport.cpp"
    "${SOURCE_DIR}/Patches/CreationClubSupport.h"
    "${SOURCE_DIR}/Patches/EslExtensionSupport.cpp"
    "${SOURCE_DIR}/Patches/EslExtensionSupport.h"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit.h"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/Archive2IndexEntryHook.cpp"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/Archive2IndexEntryHook.h"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/Archive2IndexHook.cpp"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/Archive2IndexHook.h"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/ExtendedArchiveDef.cpp"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/ExtendedArchiveDef.h"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/TextureIndexEntryHook.cpp"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/TextureIndexEntryHook.h"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/TextureIndexHook.cpp"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/TextureIndexHook.h"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/TextureRequestHook.cpp"
    "${SOURCE_DIR}/Patches/ExtendedArchiveLimit/TextureRequestHook.h"
    "${SOURCE_DIR}/Patches/ExtendedFormRange.cpp"
    "${SOURCE_DIR}/Patches/ExtendedFormRange.h"
    "${SOURCE_DIR}/Patches/FreeBuildSupport.cpp"
    "${SOURCE_DIR}/Patches/FreeBuildSupport.h"
    "${SOURCE_DIR}/Patches/Patches.cpp"
    "${SOURCE_DIR}/Patches/Patches.h"
    "${SOURCE_DIR}/Patches/PipBoyModSupport.cpp"
    "${SOURCE_DIR}/Patches/PipBoyModSupport.h"
    "${SOURCE_DIR}/Patches/PluginVersionPatcher.cpp"
    "${SOURCE_DIR}/Patches/PluginVersionPatcher.h"
    "${SOURCE_DIR}/Patches/RejectInvalidMaterial.cpp"
    "${SOURCE_DIR}/Patches/RejectInvalidMaterial.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/AdjustFormIDFileIndexHook.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/AdjustFormIDFileIndexHook.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/DataHandlerDef.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/DataHandlerDef.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/DataHandlerHook.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/DataHandlerHook.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/GetFileAtHook.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/GetFileAtHook.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/GetSmallFileHook.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/GetSmallFileHook.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/LoadTESInfoHook.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/LoadTESInfoHook.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SaveLoadGameDef.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SaveLoadGameDef.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SaveLoadGameHook.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SaveLoadGameHook.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SetCompileIndexHook.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SetCompileIndexHook.h"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SmallFileLib.cpp"
    "${SOURCE_DIR}/Patches/SmallFileLoader/SmallFileLib.h"
    "${SOURCE_DIR}/Warnings/ReferenceHandleCap.cpp"
    "${SOURCE_DIR}/Warnings/ReferenceHandleCap.h"
    "${SOURCE_DIR}/Warnings/Warnings.cpp"
    "${SOURCE_DIR}/Warnings/Warnings.h"
)

source_group(TREE "${ROOT_DIR}" FILES ${SOURCE_FILES})

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h")
configure_file(
    "${ROOT_DIR}/cmake/Plugin.h.in"
    "${VERSION_HEADER}"
    @ONLY
)

source_group("src" FILES "${VERSION_HEADER}")

configure_file(
    "${ROOT_DIR}/cmake/version.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
    @ONLY
)

add_library(
    "${PROJECT_NAME}"
    SHARED
    ${SOURCE_FILES}
    "${VERSION_HEADER}"
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
    "${ROOT_DIR}/.clang-format"
    "${ROOT_DIR}/.editorconfig"
)

target_compile_features(
    "${PROJECT_NAME}"
    PRIVATE
    cxx_std_23
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(
        "${PROJECT_NAME}"
        PRIVATE
        "/EHa" # Exception handling model
        "/sdl" # Enable Additional Security Checks
        "/utf-8" # Set Source and Executable character sets to UTF-8
        "/Zi" # Debug Information Format

        "/permissive-" # Standards conformance
        "/Zc:preprocessor" # Enable preprocessor conformance mode

        "/wd4324" # 'struct_name' : structure was padded due to __declspec(align())

        "$<$<CONFIG:DEBUG>:>"
        "$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
    )

    target_link_options(
        "${PROJECT_NAME}"
        PRIVATE
        "$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
        "$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
    )
endif()

target_include_directories(
    "${PROJECT_NAME}"
    PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/src"
    "${SOURCE_DIR}"
)

if(DEFINED CommonLibPath AND NOT ${CommonLibPath} STREQUAL "" AND IS_DIRECTORY ${CommonLibPath})
    add_subdirectory(${CommonLibPath} ${CommonLibName})
else()
    message(
        FATAL_ERROR
        "Variable ${CommonLibName}Path is not set or in external/."
    )
endif()

find_package(fmt REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(mmio REQUIRED CONFIG)
find_package(tomlplusplus REQUIRED CONFIG)
find_package(xbyak REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_compile_definitions(_CRT_SECURE_NO_WARNINGS) # silence warnings for mbstowcs
add_compile_definitions(_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING) # silence warning STL4043: stdext::checked_array_iterator, stdext::unchecked_array_iterator, and related factory functions are non-Standard extensions and will be removed in the future. std::span (since C++20) and gsl::span can be used instead. You can define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING or _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS to suppress this warning.
add_compile_definitions(_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

target_link_libraries(
    "${PROJECT_NAME}"
    PRIVATE
    CommonLibF4::CommonLibF4
    fmt::fmt
    mmio::mmio
    spdlog::spdlog
    tomlplusplus::tomlplusplus
    xbyak::xbyak
)

target_compile_definitions(
    "${PROJECT_NAME}"
    PRIVATE
    F4SE_SUPPORT_XBYAK
)

target_compile_definitions(
    CommonLibF4
    PUBLIC
    F4SE_SUPPORT_XBYAK
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
)

target_precompile_headers(
    "${PROJECT_NAME}"
    PRIVATE
    "${SOURCE_DIR}/PCH.h"
)

handle_data_files(
    FILES
    "$<TARGET_FILE:${PROJECT_NAME}>"
    "${ROOT_DIR}/data/Data/F4SE/Plugins/Daytripper4.toml"
    DESTINATION "F4SE/Plugins"
)

set(CONFIG_DIR "${ROOT_DIR}/data/Data/F4SE/Plugins/*")
file(GLOB_RECURSE CONFIG_FILES ${CONFIG_DIR})
message("Found for ${CONFIG_FILES} in ${CONFIG_DIR}")

# Automatic deployment to Mod Organizer 2 mod directory.
foreach(DEPLOY_TARGET $ENV{FalloutPluginTargets})
    message("Adding deployment target ${DEPLOY_TARGET}.")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET}/F4SE/Plugins/")
endforeach()

set(ZIP_DIR "${CMAKE_CURRENT_BINARY_DIR}/zip")
add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ZIP_DIR}"
    "${ZIP_DIR}/F4SE/Plugins/"
)

message("Copying mod into ${ZIP_DIR}.")
file(MAKE_DIRECTORY "${ZIP_DIR}/F4SE/Plugins/")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${ZIP_DIR}/F4SE/Plugins/"
)

if(CONFIG_FILES)
    message("Copying default config files ${CONFIG_FILES} into ${ZIP_DIR}.")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CONFIG_FILES} "${ZIP_DIR}/F4SE/Plugins/")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo OR ${CMAKE_BUILD_TYPE} STREQUAL Release)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> "${ZIP_DIR}/F4SE/Plugins/")
endif()

set(TARGET_ZIP "${PROJECT_NAME}-${PROJECT_VERSION}.7z")
message("Zipping ${ZIP_DIR} to ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ZIP}.")
ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ZIP} --format=7zip -- .
    WORKING_DIRECTORY ${ZIP_DIR}
)
