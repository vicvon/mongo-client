#include "bson_value.h"
#include <bson.h>
#include <algorithm>

namespace bsonCpp
{

void deleter(uint8_t * p)
{
    delete [] p;
}

void bson_free_deleter(uint8_t *p)
{
    bson_free(p);
}

raw_value::raw_value():buff_(NULL), buflen_(0), delete_fun_(NULL)
{
}

raw_value::raw_value(uint8_t * src, uint32_t length, delete_fun dtor) : buff_(src),
                                                                        buflen_(length),
                                                                        delete_fun_(dtor)
{
}

raw_value::~raw_value()
{
    if (delete_fun_ != NULL)
        delete_fun_(buff_);
}

raw_value::raw_value(const raw_value & rhs)
{
    if (rhs.size() > 0)
    {
        buff_ = new uint8_t[rhs.size()];
        buflen_ = rhs.size();
        delete_fun_ = deleter;
        std::copy(rhs.data(), rhs.data() + rhs.size(), buff_);
    }
    else
    {
        buff_ = NULL;
        buflen_ = 0;
        delete_fun_ = NULL;
    }
}

const uint8_t * raw_value::data() const
{
    return buff_;
}

const uint32_t raw_value::size() const
{
    return buflen_;
}

}