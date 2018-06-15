#ifndef _BSON_VALUE_H_
#define _BSON_VALUE_H_

#include <cstdint>
#include <boost/noncopyable.hpp>

namespace bsonCpp
{

void deleter(uint8_t * p);
void bson_free_deleter(uint8_t *p);

class raw_value
{
public:
    typedef void(* delete_fun)(uint8_t *);
    raw_value();
    raw_value(uint8_t * src, uint32_t length, delete_fun dtor);
    virtual ~raw_value();
    raw_value(const raw_value& rhs);

    const uint8_t * data() const;
    const uint32_t size() const;
private:
    raw_value & operator=(const raw_value& rhs);
private:
    uint8_t * buff_;
    uint32_t buflen_;
    delete_fun delete_fun_;
};

class doc_value : public raw_value
{
public:
    doc_value(uint8_t * src, uint32_t length, delete_fun dtor) : raw_value(src, length, dtor)
    {
    }
    virtual ~doc_value()
    {
    }
};

class arr_value : public raw_value
{
public:
    arr_value(uint8_t * src, uint32_t length, delete_fun dtor) : raw_value(src, length, dtor)
    {
    }
    virtual ~arr_value()
    {
    }
};

}

#endif /* _BSON_VALUE_H_ */
