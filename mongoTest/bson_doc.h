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
    document & append(const std::string & k, T & v)
    {
        core_.setKey(k);
        core_.append(v);

        return *this;
    }

    document & append(const std::string & k);

    doc_value extract()
    {
        return core_.extract_document();
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
doc_value make_document(const boost::tuple<const std::string &, T0> & arg)
{
    document doc;
    doc.append(boost::get<0>(arg), boost::get<1>(arg));

    return doc.extract();
}

template <typename T0, typename T1>
doc_value make_document(const boost::tuple<const std::string &, T0> & arg1,
                        const boost::tuple<const std::string &, T1> & arg2)
{
    document doc;
    doc.append(boost::get<0>(arg1), boost::get<1>(arg1)).append(boost::get<0>(arg2), boost::get<1>(arg2));

    return doc.extract();
}

template <typename T0, typename T1, typename T2>
doc_value make_document(const boost::tuple<const std::string &, T0> & arg1,
                        const boost::tuple<const std::string &, T1> & arg2,
                        const boost::tuple<const std::string &, T2> & arg3)
{
    document doc;
    doc.append(boost::get<0>(arg1), boost::get<1>(arg1)).append(boost::get<0>(arg2), boost::get<1>(arg2)).
        append(boost::get<0>(arg3), boost::get<1>(arg3));

    return doc.extract();
}

doc_value make_document(const std::string & k);

}

#endif /* _BSON_DOC_H_ */