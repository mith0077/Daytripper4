#pragma once

#include "PCH.h"

#include "Plugin.h"


namespace Settings
{
    template <class T>
    class Setting
    {
    public:
        using value_type = T;

        Setting(
            std::string_view a_group,
            std::string_view a_key,
            value_type a_default) noexcept :
            _group(a_group),
            _key(a_key),
            _value(a_default)
        {}

        [[nodiscard]] auto group() const noexcept -> std::string_view { return this->_group; }
        [[nodiscard]] auto key() const noexcept -> std::string_view { return this->_key; }

        template <class Self>
        [[nodiscard]] auto&& get(this Self&& a_self) noexcept
        {
            return std::forward<Self>(a_self)._value;
        }

        template <class Self>
        [[nodiscard]] auto&& operator*(this Self&& a_self) noexcept
        {
            return std::forward<Self>(a_self).get();
        }

    private:
        std::string_view _group;
        std::string_view _key;
        value_type _value;
    };

    using bSetting = Setting<bool>;
    using iSetting = Setting<std::int64_t>;
    using sSetting = Setting<std::string>;

#define MAKE_SETTING(a_type, a_group, a_key, a_default) \
    inline auto a_key = a_type(a_group, #a_key, a_default)

    MAKE_SETTING(bSetting, "Fixes", BookMenuText, true);
    MAKE_SETTING(bSetting, "Fixes", DlcPreCulling, true);
    MAKE_SETTING(bSetting, "Fixes", GrenadeProjectile, true);
    MAKE_SETTING(bSetting, "Fixes", NiExtraDataTyping, true);
    MAKE_SETTING(bSetting, "Fixes", ProjectileDownwardAiming, true);
    MAKE_SETTING(bSetting, "Fixes", RobotModMenu, true);
    MAKE_SETTING(bSetting, "Fixes", WetnessMaterialSsr, true);
    MAKE_SETTING(bSetting, "Patches", CreationClubSupport, true);
    MAKE_SETTING(bSetting, "Patches", EslExtensionSupport, true);
    MAKE_SETTING(bSetting, "Patches", ExtendedArchiveLimit, true);
    MAKE_SETTING(bSetting, "Patches", ExtendedFormRange, true);
    MAKE_SETTING(bSetting, "Patches", FreeBuildSupport, true);
    MAKE_SETTING(bSetting, "Patches", PipBoyModSupport, true);
    MAKE_SETTING(bSetting, "Patches", PluginVersionPatcher, true);
    MAKE_SETTING(bSetting, "Patches", RejectInvalidMaterial, true);
    MAKE_SETTING(bSetting, "Patches", SmallFileLoader, false);
    MAKE_SETTING(bSetting, "Warnings", ReferenceHandleCap, true);
    MAKE_SETTING(iSetting, "Warnings", ReferenceHandleLimit_LoadGame, 900000);
    MAKE_SETTING(iSetting, "Warnings", ReferenceHandleLimit_MainMenu, 800000);

#undef MAKE_SETTING

    inline void Load()
    {
        logger::debug("Entering Setting::load()");

        auto path = fmt::format("Data/F4SE/Plugins/{}.toml", Plugin::NAME);
        toml::parse_result config;

        try {
            config = toml::parse_file(path);
        }
        catch (...) {
            logger::error("Failed to parse config file '{}', using default values.", path);

            return;
        }

#define LOAD(a_setting)                                                              \
    if (const auto tweak = config[a_setting.group()][a_setting.key()]; tweak) {      \
        if (const auto value = tweak.as<decltype(a_setting)::value_type>(); value) { \
            *a_setting = value->get();                                               \
            logger::debug("Read setting from file: '{}:{}' -> {}.",                  \
                          a_setting.group(),                                         \
                          a_setting.key(),                                           \
                          value->get());                                             \
        }                                                                            \
        else {                                                                       \
            logger::error("Ignored wrong type of setting '{}:{}', expected '{}'.",   \
                          a_setting.group(),                                         \
                          a_setting.key(),                                           \
                          typeid(decltype(a_setting)::value_type).name());           \
        }                                                                            \
    }

        LOAD(BookMenuText);
        LOAD(DlcPreCulling);
        LOAD(GrenadeProjectile);
        LOAD(ProjectileDownwardAiming);
        LOAD(RobotModMenu);
        LOAD(WetnessMaterialSsr);
        LOAD(CreationClubSupport);
        LOAD(EslExtensionSupport);
        LOAD(ExtendedArchiveLimit);
        LOAD(ExtendedFormRange);
        LOAD(FreeBuildSupport);
        LOAD(PipBoyModSupport);
        LOAD(PluginVersionPatcher);
        LOAD(RejectInvalidMaterial);
        LOAD(SmallFileLoader);
        LOAD(ReferenceHandleCap);
        LOAD(ReferenceHandleLimit_LoadGame);
        LOAD(ReferenceHandleLimit_MainMenu);

#undef LOAD
    }
}
