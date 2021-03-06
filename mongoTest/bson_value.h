#ifndef _BSON_VALUE_H_
#define _BSON_VALUE_H_

#include <cstdint>
#include <boost/function.hpp>

namespace bsonCpp
{

void deleter(uint8_t * p);
void bson_free_deleter(uint8_t *p);

class raw_value
{
public:
    raw_value();
    raw_value(uint8_t * src, uint32_t length, boost::function<void(uint8_t *)> dtor = NULL);
    virtual ~raw_value();
    raw_value(const raw_value& rhs);

    const uint8_t * data() const;
    const uint32_t size() const;

    raw_value & operator=(const raw_value& rhs);
private:
    uint8_t * buff_;
    uint32_t buflen_;
    boost::function<void(uint8_t *)> delete_fun_;
    bool isOwner_;     /* 是否拥有buff_所指向地址的所有权,即是否需要自动释放 */
};

class doc_value : public raw_value
{
public:
    doc_value() : raw_value()
    {
    }

    doc_value(uint8_t * src, uint32_t length, boost::function<void(uint8_t *)> dtor = NULL) : raw_value(src, length, dtor)
    {
    }

    virtual ~doc_value()
    {
    }
};

class arr_value : public raw_value
{
public:
    arr_value() : raw_value()
    {
    }

    arr_value(uint8_t * src, uint32_t length, boost::function<void(uint8_t *)> dtor = NULL) : raw_value(src, length, dtor)
    {
    }

    virtual ~arr_value()
    {
    }
};

}

#endif /* _BSON_VALUE_H_ */
