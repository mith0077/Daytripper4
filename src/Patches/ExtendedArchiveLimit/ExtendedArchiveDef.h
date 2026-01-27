#pragma once

#include "PCH.h"


/* === SUMMARY OF ExtendedArchiveLimit DATA MODEL ===
**
** Follwing two data structures are modified to overcome hard limits of 256.
**
** > BSResource::Archive2::Index:
**   - dataFiles, asyncDataFiles and dataFileNameIDs have fixed capacity of 256.
**     Now, last element of each is repurposed as pointer to extended array,
**     that is an instance of BSTArray, holding 256th+ elements of dataFiles etc.
**
**     * Ex. dataFile260th = (BSTArray*)(dataFiles[255])->get(260 - 255)
**
** > BSResource::Archive2::Index::EntryHeader:
**   - Data width of 8-bit is insufficient for dataFileIndex.
**     Extra 8-bit is borrowed from unused upper byte of chunkOffsetOrType.
**
**     * Ex. dataFileIndexEx = dataFileIndex + (chunkOffsetOrType & 0xFF00)
*/

namespace ExtendedArchiveLimit
{
    void ConstructIndexEx(RE::BSResource::Archive2::Index* a_pIndex);
    void DestructIndexEx(RE::BSResource::Archive2::Index* a_pIndex);
    RE::BSTSmartPointer<RE::BSResource::Stream>& GetDataFileEx(RE::BSResource::Archive2::Index* a_pIndex, std::uint16_t a_index);
    RE::BSTSmartPointer<RE::BSResource::AsyncStream>& GetAsyncDataFileEx(RE::BSResource::Archive2::Index* a_pIndex, std::uint16_t a_index);
    RE::BSResource::ID& GetDataFileNameIdEx(RE::BSResource::Archive2::Index* a_pIndex, std::uint16_t a_index);
    std::uint16_t GetDataFileIndexEx(RE::BSResource::Archive2::Index::Entry* a_pEntry);
    void SetDataFileIndexEx(RE::BSResource::Archive2::Index::Entry* a_pEntry, std::uint16_t a_index);
}
