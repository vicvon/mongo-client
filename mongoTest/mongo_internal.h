#ifndef _URI_INTERNAL_H_
#define _URI_INTERNAL_H_

#include "mongo_uri.h"
#include "mongo_pool.h"
#include "mongo_client.h"
#include <mongoc.h>

namespace mongoCpp
{
struct uri::Impl
{
    Impl(std::string uri_string)
    {
        uri_ = mongoc_uri_new(uri_string.c_str());
    }
    ~Impl()
    {
        mongoc_uri_destroy(uri_);
    }

    mongoc_uri_t *uri_;
};

struct mongo_client_pool::Impl
{
    Impl(const uri & uri)
    {
        pool_ = mongoc_client_pool_new(uri.get()->uri_);
    }
    ~Impl()
    {
        mongoc_client_pool_destroy(pool_);
    }

    mongoc_client_pool_t * pool_;
};

struct client::Impl
{
    Impl(const uri & uri):isPool_(false)
    {
        client_ = mongoc_client_new_from_uri(uri.get()->uri_);
    }
    Impl(mongoc_client_t * client):isPool_(true)
    {
        client_ = client;
    }
    ~Impl()
    {
        if (!isPool_)
        {
            mongoc_client_destroy(client_);
        }
    }
    mongoc_client_t * client_;
private:
    bool isPool_;
};

}

#endif /* _MONGO_INTERNAL_H_ */