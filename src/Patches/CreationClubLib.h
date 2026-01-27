#pragma once

#include "PCH.h"


class CreationClubContents
{
    private:
        CreationClubContents();
        ~CreationClubContents() = default;

        CreationClubContents(const CreationClubContents&) = delete;
        CreationClubContents(CreationClubContents&&) = delete;
        CreationClubContents& operator=(const CreationClubContents&) = delete;
        CreationClubContents& operator=(CreationClubContents&&) = delete;

    public:
        static CreationClubContents *GetInstance();
        bool IsCreationClubFile(const char* a_filename);

    public:
        constexpr std::vector<std::string>::iterator begin() noexcept;
        constexpr std::vector<std::string>::iterator end() noexcept;
        constexpr std::vector<std::string>::size_type size() const noexcept;
        constexpr std::vector<std::string>::reference operator[](std::vector<std::string>::size_type n) noexcept;
        constexpr std::vector<std::string>::reference at(std::vector<std::string>::size_type n) noexcept;

    private:
        std::vector<std::string> _cccVec;
};
