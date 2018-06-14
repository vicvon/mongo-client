#include "mongo_collection.h"
#include "mongo_internal.h"

namespace mongoCpp
{

struct collection::Impl
{
    Impl(mongoc_database_t *db, std::string name)
    {
        collection_ = mongoc_database_get_collection(db, name.c_str());
    }
    ~Impl()
    {
        mongoc_collection_destroy(collection_);
    }

    mongoc_collection_t * collection_;
};

collection::collection(void * db, std::string name) : impl_(new Impl(static_cast<mongoc_database_t *>(db), name))
{
}

collection::~collection()
{
}

}