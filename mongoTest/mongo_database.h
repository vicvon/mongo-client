#ifndef _MONGO_DATABASE_H_
#define _MONGO_DATABASE_H_

#include "mongo_client.h"
#include "mongo_collection.h"
#include <boost/shared_ptr.hpp>

namespace mongoCpp
{
class database
{
public:
    database(void * cli, std::string name);
    ~database();

    collection get_collection(std::string name);
private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};
}

#endif /* _MONGO_DATABASE_H_ */