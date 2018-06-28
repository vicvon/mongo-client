#include "bson_view.h"
#include <bson.h>
#include <boost/format.hpp>
#include <stdexcept>

#define BSONCPP_GET_KEY_ITER(keyname)   \
    bson_t b;                           \
    bson_iter_t iter;                   \
    do                                  \
    {                                   \                    
        bson_init_static(&b, data_, length_);   \
        bool ret = bson_iter_init_find(&iter, &b, keyname.c_str());    \
        if (!ret)       \
        {               \
            boost::format fmt = boost::format("key:%s can not found") % keyname;    \
            throw std::runtime_error(fmt.str());    \
        }                                   \
    }while(0)

namespace bsonCpp
{

view::view(const uint8_t * src, uint32_t length) : data_(src), length_(length)
{
}

const uint8_t * view::data() const
{
    return data_;
}

const uint32_t view::size() const
{
    return length_;
}

uint32_t view::count_keys()
{
    bson_t b;
    bson_init_static(&b, data_, length_);

    return bson_count_keys(&b);
}

bool view::find_key(const std::string & key)
{
    bson_t b;
    bson_iter_t iter;

    bson_init_static(&b, data_, length_);

    return bson_iter_init_find(&iter, &b, key.c_str());
}

std::string view::get_value_as_utf8(const std::string & key)
{
    BSONCPP_GET_KEY_ITER(key);

    if (BSON_TYPE_UTF8 != bson_iter_type(&iter))
    {
        boost::format fmt = boost::format("key:%s value error, type is not string") % key;
        throw std::runtime_error(fmt.str());
    }

    uint32_t len;
    const char * result = bson_iter_utf8(&iter, &len);
    return std::string(result, len);
}

int32_t view::get_value_as_int32(const std::string & key)
{
    BSONCPP_GET_KEY_ITER(key);

    if (BSON_TYPE_INT32 != bson_iter_type(&iter))
    {
        boost::format fmt = boost::format("key:%s value error, type is not integer32") % key;
        throw std::runtime_error(fmt.str());
    }
    return bson_iter_int32(&iter);;
}

int64_t view::get_value_as_int64(const std::string & key)
{
    BSONCPP_GET_KEY_ITER(key);

    if (BSON_TYPE_INT64 != bson_iter_type(&iter))
    {
        boost::format fmt = boost::format("key:%s value error, type is not integer64") % key;
        throw std::runtime_error(fmt.str());
    }
    return bson_iter_int64(&iter);
}

bool view::get_value_as_bool(const std::string & key)
{
    BSONCPP_GET_KEY_ITER(key);

    if (BSON_TYPE_BOOL != bson_iter_type(&iter))
    {
        boost::format fmt = boost::format("key:%s value error, type is not bool") % key;
        throw std::runtime_error(fmt.str());
    }
    return bson_iter_bool(&iter);
}

view view::get_value_as_doc(const std::string & key)
{
    BSONCPP_GET_KEY_ITER(key);

    if (BSON_TYPE_DOCUMENT != bson_iter_type(&iter))
    {
        boost::format fmt = boost::format("key:%s value error, type is not document") % key;
        throw std::runtime_error(fmt.str());
    }

    uint32_t len;
    const uint8_t * doc;
    bson_iter_document(&iter, &len, &doc);
    return view(doc, len);
}

view view::get_value_as_array(const std::string & key)
{
    BSONCPP_GET_KEY_ITER(key);

    if (BSON_TYPE_ARRAY != bson_iter_type(&iter))
    {
        boost::format fmt = boost::format("key:%s value error, type is not array") % key;
        throw std::runtime_error(fmt.str());
    }

    uint32_t len;
    const uint8_t * doc;
    bson_iter_array(&iter, &len, &doc);
    return view(doc, len);
}

}