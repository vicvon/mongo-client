#ifndef _BSON_CPP_H_
#define _BSON_CPP_H_

#include "bson_core.h"

#include <string>

namespace bsonCpp
{
class document
{
public:
    document();
    ~document();

    template <typename T>
    document & append(std::string & k, T & v)
    {
        core_.setKey(k);
        core_.append(v);

        return *this;
    }
private:
    core core_;
};

template <typename T>
document make_document(std::string & k, T & v)
{
    document doc;
    doc.append(k, v);

    return doc;
}

}

#endif /* _BSON_CPP_H_ */