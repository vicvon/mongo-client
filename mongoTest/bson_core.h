#ifndef _BSON_CORE_H_
#define _BSON_CORE_H_

#include <string>
#include <cstdint>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace bsonCpp
{

class doc_value;
class arr_value;
class document;

class core : public boost::noncopyable
{
public:
    core(bool is_array);
    ~core();

    core& setKey(const std::string & key);
    core& append(const std::string & value);
    core& append(const char * value);
    core& append(const doc_value & value);
    core& append(const arr_value & value);
    core& append(const int32_t value);
    core& append(const bool value);
    core& append();
    doc_value extract_document();
    arr_value extractor_array();
private:
    class coreImpl;
    boost::shared_ptr<coreImpl> impl_;
};


}

#endif /* _BSON_CORE_H_ */