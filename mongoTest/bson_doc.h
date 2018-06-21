#ifndef _BSON_DOC_H_
#define _BSON_DOC_H_

#include "bson_core.h"
#include "bson_value.h"
#include <string>
#include <boost/tuple/tuple.hpp>

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
    document & append(const boost::tuple<const std::string &, T> & arg)
    {
        return append(boost::get<0>(arg), boost::get<1>(arg));
    }

    document & append(doc_value & v);
    document & append(const std::string & k);

    doc_value extract()
    {
        return core_.extract_document();
    }
private:
    template <typename T>
    document & append(const std::string & k, T & v)
    {
        core_.setKey(k);
        core_.append(v);

        return *this;
    }

private:
    core core_;
};

template <typename T>
boost::tuple<const std::string &, T> kvp(const std::string & k, T v)
{
    return boost::tuple<const std::string &, T>(k, v);
}

template <typename T0>
doc_value make_document(T0 v0)
{
    document doc;
    doc.append(v0);

    return doc.extract();
}

template <typename T0, typename T1>
doc_value make_document(T0 v0, T1 v1)
{
    document doc;
    doc.append(v0).append(v1);

    return doc.extract();
}

template <typename T0, typename T1, typename T2>
doc_value make_document(T0 v0, T1 v1, T2 v2)
{
    document doc;
    doc.append(v0).append(v1).append(v2);

    return doc.extract();
}

template <typename T0, typename T1, typename T2, typename T3>
doc_value make_document(T0 v0, T1 v1, T2 v2, T3 v3)
{
    document doc;
    doc.append(v0).append(v1).append(v2).append(v3);

    return doc.extract();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4>
doc_value make_document(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4)
{
    document doc;
    doc.append(v0).append(v1).append(v2).append(v3).append(v4);

    return doc.extract();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
doc_value make_document(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5)
{
    document doc;
    doc.append(v0).append(v1).append(v2).append(v3).append(v4).append(v5);

    return doc.extract();
}

doc_value make_document(const std::string & k);

}

#endif /* _BSON_DOC_H_ */