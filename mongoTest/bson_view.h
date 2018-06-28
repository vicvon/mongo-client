#ifndef _BSON_VIEW_H_
#define _BSON_VIEW_H_

#include <cstdint>
#include <string>

namespace bsonCpp
{

class view
{
public:
    view(const uint8_t * src, uint32_t length);

    const uint8_t * data() const;
    const uint32_t size() const;

    uint32_t count_keys();
    bool find_key(const std::string & key);
    std::string get_value_as_utf8(const std::string & key);
    int32_t get_value_as_int32(const std::string & key);
    int64_t get_value_as_int64(const std::string & key);
    bool get_value_as_bool(const std::string & key);
    view get_value_as_doc(const std::string & key);
    view get_value_as_array(const std::string & key);
private:
    const uint8_t * data_;
    uint32_t length_;
};

}

#endif