#include "mongo_internal.h"

namespace mongoCpp
{
mongo_client_pool::mongo_client_pool(const uri & uri) : impl_(new Impl(uri))
{
}

mongo_client_pool::~mongo_client_pool()
{
}

mongo_client_pool::pool_client_entry mongo_client_pool::get_client()
{
    mongoc_client_t * client = mongoc_client_pool_pop(impl_->pool_);
    return pool_client_entry(this, static_cast<void *>(client));
}

mongo_client_pool::pool_client_entry::pool_client_entry(mongo_client_pool * pool, void * cli): client_(cli), pool_(pool)
{
}

mongo_client_pool::pool_client_entry::~pool_client_entry()
{
    mongoc_client_pool_push(pool_->impl_->pool_, client_.impl_->client_);
}

client & mongo_client_pool::pool_client_entry::get()
{
    return client_;
}

}