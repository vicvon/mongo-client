#ifndef _BSON_CORE_H_
#define _BSON_CORE_H_

#include <string>
#include <cstdint>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace bsonCpp
{

class raw_value;
class document;

class core : public boost::noncopyable
{
public:
    class coreImpl;
    core();
    ~core();

    core& setKey(std::string & key);
    core& append(std::string & value);
    core& append(raw_value & value);
    raw_value extract_document();
private:
    boost::shared_ptr<coreImpl> impl_;
};


}

#endif /* _BSON_CORE_H_ */