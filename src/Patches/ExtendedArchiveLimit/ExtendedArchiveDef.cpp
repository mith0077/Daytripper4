#include "PCH.h"

#include "ExtendedArchiveDef.h"

using namespace std;
using namespace RE;
using namespace REL;

using RE::BSResource::Stream;
using RE::BSResource::AsyncStream;
using Archive2Index = RE::BSResource::Archive2::Index;
using ResourceID = RE::BSResource::ID;


namespace ExtendedArchiveLimit
{
    constexpr std::uint8_t EXTENDED_ARRAY_POS = 255;

    void ConstructIndexEx(Archive2Index* a_pIndex)
    {
        logger::debug("Entering ConstructIndexEx()");

        assert(a_pIndex);

        auto ppExtendedArray_DataFile = reinterpret_cast<vector<BSTSmartPointer<Stream>>**>(a_pIndex->dataFiles + EXTENDED_ARRAY_POS);
        *ppExtendedArray_DataFile = new vector<BSTSmartPointer<Stream>>();

        auto ppExtendedArray_AsyncDataFile = reinterpret_cast<vector<BSTSmartPointer<AsyncStream>>**>(a_pIndex->asyncDataFiles + EXTENDED_ARRAY_POS);
        *ppExtendedArray_AsyncDataFile = new vector<BSTSmartPointer<AsyncStream>>();

        auto ppExtendedArray_DataFileNameId = reinterpret_cast<vector<ResourceID>**>(a_pIndex->dataFileNameIDs + EXTENDED_ARRAY_POS);
        *ppExtendedArray_DataFileNameId = new vector<ResourceID>();
    }

    void DestructIndexEx(Archive2Index* a_pIndex)
    {
        logger::debug("Entering DestructIndexEx()");

        assert(a_pIndex);

        auto ppExtendedArray_DataFile = reinterpret_cast<vector<BSTSmartPointer<Stream>>**>(a_pIndex->dataFiles + EXTENDED_ARRAY_POS);
        delete *ppExtendedArray_DataFile;
        *ppExtendedArray_DataFile = nullptr;

        auto ppExtendedArray_AsyncDataFile = reinterpret_cast<vector<BSTSmartPointer<AsyncStream>>**>(a_pIndex->asyncDataFiles + EXTENDED_ARRAY_POS);
        delete *ppExtendedArray_AsyncDataFile;
        *ppExtendedArray_AsyncDataFile = nullptr;

        auto ppExtendedArray_DataFileNameId = reinterpret_cast<vector<ResourceID>**>(a_pIndex->dataFileNameIDs + EXTENDED_ARRAY_POS);
        delete *ppExtendedArray_DataFileNameId;
        *ppExtendedArray_DataFileNameId = nullptr;
    }

    BSTSmartPointer<Stream>& GetDataFileEx(Archive2Index* a_pIndex, uint16_t a_index)
    {
        // logger::debug("Entering GetDataFileEx(), index = {}", a_index);

        assert(a_pIndex);

        if (EXTENDED_ARRAY_POS > a_index)
            return a_pIndex->dataFiles[a_index];

        auto pExtendedArray = *reinterpret_cast<vector<BSTSmartPointer<Stream>>**>(a_pIndex->dataFiles + EXTENDED_ARRAY_POS);
        uint16_t extendedIndex = a_index - EXTENDED_ARRAY_POS;

        assert(pExtendedArray);

        static mutex mtx;
        lock_guard<std::mutex> lock(mtx);

        if (pExtendedArray->size() < extendedIndex + 0x1)
            pExtendedArray->resize((extendedIndex & 0xFF00) + 0x100);

        return pExtendedArray->at(extendedIndex);
    }

    BSTSmartPointer<AsyncStream>& GetAsyncDataFileEx(Archive2Index* a_pIndex, uint16_t a_index)
    {
        // logger::debug("Entering GetAsyncDataFileEx(), index = {}", a_index);

        assert(a_pIndex);

        if (EXTENDED_ARRAY_POS > a_index)
            return a_pIndex->asyncDataFiles[a_index];

        auto pExtendedArray = *reinterpret_cast<vector<BSTSmartPointer<AsyncStream>>**>(a_pIndex->asyncDataFiles + EXTENDED_ARRAY_POS);
        uint16_t extendedIndex = a_index - EXTENDED_ARRAY_POS;

        assert(pExtendedArray);

        static mutex mtx;
        lock_guard<std::mutex> lock(mtx);

        if (pExtendedArray->size() < extendedIndex + 0x1)
            pExtendedArray->resize((extendedIndex & 0xFF00) + 0x100);

        return pExtendedArray->at(extendedIndex);
    }

    ResourceID& GetDataFileNameIdEx(Archive2Index* a_pIndex, uint16_t a_index)
    {
        // logger::debug("Entering GetDataFileNameIdEx(), index = {}", a_index);

        assert(a_pIndex);

        if (EXTENDED_ARRAY_POS > a_index)
            return a_pIndex->dataFileNameIDs[a_index];

        auto pExtendedArray = *reinterpret_cast<vector<ResourceID>**>(a_pIndex->dataFileNameIDs + EXTENDED_ARRAY_POS);
        uint16_t extendedIndex = a_index - EXTENDED_ARRAY_POS;

        assert(pExtendedArray);

        static mutex mtx;
        lock_guard<std::mutex> lock(mtx);

        if (pExtendedArray->size() < extendedIndex + 0x1)
            pExtendedArray->resize((extendedIndex & 0xFF00) + 0x100);

        return pExtendedArray->at(extendedIndex);
    }

    uint16_t GetDataFileIndexEx(Archive2Index::Entry* a_pEntry)
    {
        // logger::debug("Entering GetDataFileIndexEx()");

        assert(a_pEntry);

        uint16_t baseIndex = a_pEntry->dataFileIndex;
        uint16_t extendedIndex = a_pEntry->chunkOffsetOrType & 0xFF00;

        return baseIndex + extendedIndex;
    }

    void SetDataFileIndexEx(Archive2Index::Entry* a_pEntry, uint16_t a_index)
    {
        // logger::debug("Entering SetDataFileIndexEx()");

        assert(a_pEntry);

        uint16_t baseIndex = a_index & 0x00FF;
        uint16_t extendedIndex = a_index & 0xFF00;

        a_pEntry->dataFileIndex = static_cast<uint8_t>(baseIndex);
        a_pEntry->chunkOffsetOrType = extendedIndex + (a_pEntry->chunkOffsetOrType & 0x00FF);
    }
}
