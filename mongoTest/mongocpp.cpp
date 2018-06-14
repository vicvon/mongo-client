#include "mongocpp.h"
#include <mongoc.h>

namespace mongoCpp
{
mongo_init::mongo_init()
{
    mongoc_init();
}

mongo_init::~mongo_init()
{
    mongoc_cleanup();
}

}