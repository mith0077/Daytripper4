#include "PCH.h"

#include "DefaultObjectLib.h"

using namespace std;
using namespace RE;
using namespace REL;


vector<DefaultObject*> DefaultObject::instances_ = vector<DefaultObject*>();
mutex DefaultObject::instancesMtx_ = mutex();

DefaultObject::DefaultObject(const char* a_name, ENUM_FORM_ID a_formType, const char* a_description)
    : form(nullptr), type(ENUM_FORM_ID::kNONE), name_(a_name), formType_(a_formType), description_(a_description)
{
    lock_guard<mutex> lock(instancesMtx_);

    instances_.push_back(this);
}

void DefaultObject::InitializeOne()
{
    static const uintptr_t DefaultObjectCtor_addr = RelocationID(1144014, 2197003, 1144014).address();
    static const auto DefaultObjectCtor = reinterpret_cast<DefaultObject* (*)(DefaultObject*, const char*, ENUM_FORM_ID, const char*)>(DefaultObjectCtor_addr);

    logger::debug("Initializing DefaultObject: {}", name_);

    DefaultObjectCtor(this, name_, formType_, description_);
}

void DefaultObject::InitializeAll()
{
    lock_guard<mutex> lock(instancesMtx_);

    for (auto instance : DefaultObject::instances_) {
        assert(instance);

        instance->InitializeOne();
    }

    DefaultObject::instances_.clear();
}
