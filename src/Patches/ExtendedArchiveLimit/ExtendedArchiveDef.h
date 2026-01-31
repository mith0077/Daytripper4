#pragma once

#include "PCH.h"


/* === SUMMARY OF ExtendedArchiveLimit DATA MODEL ===
**
** Follwing four data structures are modified to overcome hard limits of 256.
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
**
** > TextureIndex::Index (No CommonLib definition):
**   - dataFiles is already variable array. dataFileNameIDs have fixed capacity of 256.
**     Now, last element of each is repurposed as pointer to extended array,
**     that is an instance of BSTArray, holding 256th+ elements of dataFiles etc.
**
**     * Ex. dataFileNameID260th = (BSTArray*)(dataFileNameIDs[255])->get(260 - 255)
**
** > BSTextureStreamer::NativeDesc:
**   - Data width of 8-bit is insufficient for dataFileIndex.
**     Extra 8-bit is borrowed from unused chunks[0].padding.
**
**     * Ex. dataFileIndexEx = dataFileIndex + chunks[0].padding
*/

namespace RE::TextureIndex {
    struct Index
    {
        using Entry = BSTextureStreamer::NativeDesc<BSGraphics::TextureHeader>;
        using ID = BSResource::ID;

        class Pager;
        struct NameIDAccess;

        class CursorWithEntry :
            public BSBTreeFile::BPTree<Pager, BSBTreeFile::PGFSEDL<Entry, ID, 4096>, 16>::Cursor  // 000
        {
        public:
            // members
            Entry e;  // 148
        };

        // members
        BSTSmallIndexScatterTable<ID, NameIDAccess>                           nameTable;             // 0000
        BSTArray<BSTSmartPointer<BSResource::Stream>>                         dataFiles;             // 0020
        ID                                                                    dataFileNameIDs[256];  // 0038
        BSBTreeFile::BPTree<Pager, BSBTreeFile::PGFSEDL<Entry, ID, 4096>, 16> tr;                    // 0C38
        BSReadWriteLock                                                       lock;                  // 0C58
    };
}

namespace ExtendedArchiveLimit
{
    // Archive2::Index manipulating functions
    void ConstructIndexEx(RE::BSResource::Archive2::Index* a_pIndex);
    void DestructIndexEx(RE::BSResource::Archive2::Index* a_pIndex);
    RE::BSTSmartPointer<RE::BSResource::Stream>& GetDataFileEx(RE::BSResource::Archive2::Index* a_pIndex, std::uint16_t a_index);
    RE::BSTSmartPointer<RE::BSResource::AsyncStream>& GetAsyncDataFileEx(RE::BSResource::Archive2::Index* a_pIndex, std::uint16_t a_index);
    RE::BSResource::ID& GetDataFileNameIdEx(RE::BSResource::Archive2::Index* a_pIndex, std::uint16_t a_index);

    // Archive2::Index::Entry manipulating functions
    std::uint16_t GetDataFileIndexEx(RE::BSResource::Archive2::Index::Entry* a_pEntry);
    void SetDataFileIndexEx(RE::BSResource::Archive2::Index::Entry* a_pEntry, std::uint16_t a_index);

    // TextureIndex::Index manipulating functions
    void ConstructTextureIndexEx(RE::TextureIndex::Index* a_pTexIndex);
    void DestructTextureIndexEx(RE::TextureIndex::Index* a_pTexIndex);
    RE::BSResource::ID& GetTextureDataFileNameIdEx(RE::TextureIndex::Index* a_pTexIndex, std::uint16_t a_index);

    // Archive2::Index::Entry manipulating functions
    std::uint16_t GetTextureDataFileIndexEx(RE::TextureIndex::Index::Index::Entry* a_pTexEntry);
    void SetTextureDataFileIndexEx(RE::TextureIndex::Index::Entry* a_pTexEntry, std::uint16_t a_index);
    void CopyTextureDataFileIndexEx(RE::TextureIndex::Index::Entry* a_pDstEntry, RE::TextureIndex::Index::Entry* a_pSrcEntry);
}
