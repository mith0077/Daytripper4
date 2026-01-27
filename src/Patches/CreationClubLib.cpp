#include "PCH.h"

#include "Plugin.h"
#include "CreationClubLib.h"

using namespace std;

static const string cccFilePath = fmt::format("Data/F4SE/Plugins/{}.ccc", Plugin::NAME);


CreationClubContents::CreationClubContents()
    : _cccVec()
{
    ifstream ifs(cccFilePath, ios::in);

    if (!ifs) {
        logger::error("Failed to parse .ccc file '{}'.", cccFilePath);

        return;
    }

    string line;

    while (getline(ifs, line)) {
        if (0 == line.length())
            continue;

        _cccVec.push_back(line);
    }
}

CreationClubContents* CreationClubContents::GetInstance()
{
    static CreationClubContents _instance;

    return &_instance;
}

bool CreationClubContents::IsCreationClubFile(const char* a_filename)
{
    if (!a_filename)
        return false;

    for (auto sCcc : *this) {
        if (0 == _stricmp(a_filename, sCcc.c_str()))
            return true;
    }

    return false;
}

constexpr vector<string>::iterator CreationClubContents::begin() noexcept
{
    return _cccVec.begin();
}

constexpr vector<string>::iterator CreationClubContents::end() noexcept
{
    return _cccVec.end();
}

constexpr vector<string>::size_type CreationClubContents::size() const noexcept
{
    return _cccVec.size();
}

constexpr vector<string>::reference CreationClubContents::operator[](vector<string>::size_type n) noexcept
{
    return _cccVec[n];
}

constexpr vector<string>::reference CreationClubContents::at(std::vector<std::string>::size_type n) noexcept
{
    return _cccVec.at(n);
}
