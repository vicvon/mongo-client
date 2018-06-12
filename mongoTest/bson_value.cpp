#include "bson_value.h"

namespace bsonCpp
{
raw_value::raw_value(uint8_t * src, uint32_t length, delete_fun dtor) : buff_(src),
                                                                                 buflen_(length),
                                                                                 delete_fun_(dtor)
{
}

raw_value::~raw_value()
{
    delete_fun_(buff_);
}
}