#ifndef _MONGO_POOL_H_
#define _MONGO_POOL_H_

#include "mongo_uri.h"
#include "mongo_client.h"
#include <boost/shared_ptr.hpp>

namespace mongoCpp
{

class mongo_client_pool
{
public:
    mongo_client_pool(const uri & uri);
    ~mongo_client_pool();

    class pool_client_entry
    {
    public:
        pool_client_entry(mongo_client_pool * pool, void * cli);
        ~pool_client_entry();

        client & get();
    private:
        client client_;
        mongo_client_pool * pool_;
    };

    pool_client_entry get_client();
private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};

}

#endif /* _MONGO_POOL_H_ */