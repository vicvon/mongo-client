#include "mongo_internal.h"

#include <mongoc.h>

namespace mongoCpp
{

database::database(void * cli, std::string name) : impl_(new Impl(static_cast<mongoc_client_t *>(cli), name))
{
}

database::~database()
{
}

collection database::get_collection(std::string name)
{
    return collection(impl_->database_, name);
}

}
