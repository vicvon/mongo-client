#ifndef _MONGO_URI_H_
#define _MONGO_URI_H_

#include <boost/shared_ptr.hpp>

namespace mongoCpp
{
class uri
{
    struct Impl;
public:
    uri(std::string & uri_string);
    ~uri();

    const boost::shared_ptr<Impl> & get() const;
private:
    boost::shared_ptr<Impl> impl_;
};

}

#endif /* _MONGO_URI_H_ */