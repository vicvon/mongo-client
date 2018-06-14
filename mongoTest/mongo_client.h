#ifndef _MONGO_CLIENT_H_
#define _MONGO_CLIENT_H_

#include "mongo_uri.h"
#include "mongo_database.h"
#include <boost/shared_ptr.hpp>

namespace mongoCpp
{

class client
{
public:
    client(const uri & uri);
    ~client();

    database get_database(std::string name);
private:
    friend class mongo_client_pool;
    client(void * cli);
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};
}

#endif /* _MONGO_CLIENT_H_ */