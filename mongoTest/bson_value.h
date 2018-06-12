#ifndef _BSON_VALUE_H_
#define _BSON_VALUE_H_

#include <cstdint>
#include <boost/noncopyable.hpp>

namespace bsonCpp
{

class raw_value : public boost::noncopyable
{
public:
    typedef void(* delete_fun)(void *);
    raw_value(uint8_t * src, uint32_t length, delete_fun dtor);
    ~raw_value();

    const uint8_t * data() const;
    const uint32_t size() const;

private:
    uint8_t * buff_;
    uint32_t buflen_;
    delete_fun delete_fun_;
};


}

#endif /* _BSON_VALUE_H_ */
