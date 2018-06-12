#include "bson_core.h"

#include <bcon.h>

#include "bson_value.h"

namespace bsonCpp
{
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
    bson_t * get()
    {
        return bson_.get();
    }
    void setKey(std::string & key)
    {
        key_ = key;
    }
    const std::string& getKey() const
    {
        return key_;
    }
private:
    std::string key_;
    raii_bson_t bson_;
};

core::core():impl_(new coreImpl)
{
}

core::~core()
{
}

core & core::setKey(std::string & key)
{
    impl_->setKey(key);

    return *this;
}

core & core::append(std::string & value)
{
    if (!bson_append_utf8(impl_->get(),
                        impl_->getKey().c_str(), 
                        static_cast<int>(impl_->getKey().length()),
                        value.c_str(),
                        static_cast<int>(value.length())))
    {
    }
    return *this;
}

raw_value& core::extract_document()
{
    
}

}
