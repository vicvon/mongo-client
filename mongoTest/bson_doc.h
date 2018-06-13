#ifndef _BSON_CPP_H_
#define _BSON_CPP_H_

#include "bson_core.h"
#include "bson_value.h"
#include <string>

namespace bsonCpp
{
class document
{
public:
    document() : core_(false)
    {
    }
    ~document()
    {
    }

    template <typename T>
    document & append(const std::string & k, T & v)
    {
        core_.setKey(k);
        core_.append(v);

        return *this;
    }

    document & append(const std::string & k)
    {
        core_.setKey(k);
        core_.append();

        return *this;
    }

    doc_value extract()
    {
        return core_.extract_document();
    }
private:
    core core_;
};

template <typename T>
doc_value make_document(const std::string & k, T v)
{
    document doc;
    doc.append(k, v);

    return doc.extract();
}

doc_value make_document(const std::string & k)
{
    document doc;
    doc.append(k);

    return doc.extract();
}

}

#endif /* _BSON_CPP_H_ */