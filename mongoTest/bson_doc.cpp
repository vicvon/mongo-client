#include "bson_doc.h"

namespace bsonCpp
{

doc_value make_document(const std::string & k)
{
    document doc;
    doc.append(k);

    return doc.extract();
}

document & document::append(doc_value & v)
{
    core_.concat(v);
    return *this;
}

document & document::append(const std::string & k)
{
    core_.setKey(k);
    core_.append();

    return *this;
}

}