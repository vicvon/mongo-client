#include "bson_core.h"

#include <bcon.h>

#include "bson_value.h"
#include <iostream>
namespace bsonCpp
{
void bson_free_deleter(uint8_t *p)
{
    bson_free(p);
}

class raii_bson_t : public boost::noncopyable
{
public:
    raii_bson_t()
    {
        bson_init(&bson_);
    }
    ~raii_bson_t()
    {
        bson_destroy(&bson_);
    }
    bson_t * get()
    {
        return &bson_;
    }
private:
    bson_t bson_;
};

class core::coreImpl
{
public:
    coreImpl(bool is_array) : index_(0), isArray_(is_array)
    {
    }

    ~coreImpl()
    {
    }

    bson_t * get()
    {
        return bson_.get();
    }
    void setKey(const std::string & key)
    {
        key_ = key;
    }
    const std::string getKey()
    {
        if (isArray_)
        {
            char str[16] = {0};
            const char * key;
            bson_uint32_to_string(index_++, &key, str, sizeof(str));

            return std::string(key);
        }
        return key_;
    }

    doc_value extractor_document()
    {
        uint32_t buflen;
        uint8_t * buff = bson_destroy_with_steal(bson_.get(), true, &buflen);
        bson_init(bson_.get());

        return doc_value(buff, buflen, bson_free_deleter);
    }

    arr_value extractor_array()
    {
        uint32_t buflen;
        uint8_t * buff = bson_destroy_with_steal(bson_.get(), true, &buflen);
        bson_init(bson_.get());

        return arr_value(buff, buflen, bson_free_deleter);
    }
private:
    std::string key_;
    raii_bson_t bson_;
    uint32_t index_;
    bool isArray_;
};

core::core(bool is_array) : impl_(new coreImpl(is_array))
{
}

core::~core()
{
}

core & core::setKey(const std::string & key)
{
    impl_->setKey(key);

    return *this;
}

core & core::append(std::string & value)
{
    std::string key = impl_->getKey();
    if (!bson_append_utf8(impl_->get(),
                        key.c_str(), 
                        static_cast<int>(key.length()),
                        value.c_str(),
                        static_cast<int>(value.length())))
    {
    }
    return *this;
}

core & core::append(doc_value & value)
{
    bson_t bson;
    bson_init_static(&bson, value.data(), value.size());
    
    std::string key = impl_->getKey();
    if (!bson_append_document(impl_->get(),
                              key.c_str(),
                              static_cast<int>(key.length()),
                              &bson))
    {
    }

    return *this;
}

core & core::append(arr_value & value)
{
    bson_t bson;
    bson_init_static(&bson, value.data(), value.size());

    std::string key = impl_->getKey();
    if (!bson_append_array(impl_->get(),
                           key.c_str(),
                           static_cast<int>(key.length()),
                           &bson))
    {
    }

    return *this;
}

core & core::append(int32_t value)
{
    std::string key = impl_->getKey();
    if (!bson_append_int32(impl_->get(),
                           key.c_str(),
                           static_cast<int>(key.length()),
                           value))
    {
    }
    return *this;
}

core & core::append()
{
    std::string key = impl_->getKey();
    if (!bson_append_null(impl_->get(), 
                          key.c_str(), 
                          static_cast<int>(key.length())))
    {
    }
    return *this;
}

doc_value core::extract_document()
{
    return impl_->extractor_document();
}

arr_value core::extractor_array()
{
    return impl_->extractor_array();
}

}
