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
    isOwner_ = false;
}

raw_value::raw_value(uint8_t * src, uint32_t length,
                     boost::function<void(uint8_t *)> dtor /* = NULL */) : buff_(src),
                                                                           buflen_(length), delete_fun_(dtor)
{
    if (delete_fun_ == NULL)
        isOwner_ = false;
    else
        isOwner_ = true;
}

raw_value::~raw_value()
{
    if (delete_fun_ != NULL)
    {
        delete_fun_(buff_);
    }
}

raw_value::raw_value(const raw_value & rhs)
{
    if (rhs.buflen_ > 0)
    {
        if (rhs.isOwner_)
        {
            buff_ = new uint8_t[rhs.size()];
            buflen_ = rhs.size();
            delete_fun_ = deleter;
            std::copy(rhs.buff_, rhs.buff_ + rhs.buflen_, buff_);
        }
        else
        {
            buff_ = rhs.buff_;
            buflen_ = rhs.buflen_;
            delete_fun_ = NULL;
            isOwner_ = false;
        }
    }
    else
    {
        buff_ = NULL;
        buflen_ = 0;
        delete_fun_ = NULL;
        isOwner_ = false;
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

raw_value & raw_value::operator=(const raw_value & rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    if (buff_ && delete_fun_)
    {
        delete_fun_(buff_);
    }

    if (rhs.buflen_ > 0)
    {
        if (rhs.isOwner_)
        {
            buff_ = new uint8_t[rhs.buflen_];
            std::copy(rhs.buff_, rhs.buff_ + rhs.buflen_, buff_);
            buflen_ = rhs.buflen_;
            delete_fun_ = deleter;
        }
        else
        {
            buff_ = rhs.buff_;
            buflen_ = rhs.buflen_;
            delete_fun_ = NULL;
            isOwner_ = false;
        }

        return *this;
    }

    buff_ = NULL;
    buflen_ = 0;
    delete_fun_ = NULL;
    isOwner_ = false;
    return *this;
}

}