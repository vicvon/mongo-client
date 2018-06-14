#include "mongo_internal.h"

namespace mongoCpp
{
client::client(const uri & uri) :impl_(new Impl(uri))
{
}

client::client(void * cli):impl_(new Impl(static_cast<mongoc_client_t *>(cli)))
{
}

client::~client()
{
}

database client::get_database(std::string name)
{
    return database(impl_->client_, name);
}

}