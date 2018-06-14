#ifndef _MONGO_COLLECTION_H_
#define _MONGO_COLLECTION_H_

#include "bson_value.h"
#include "mongo_cursor.h"
#include <boost/shared_ptr.hpp>

namespace mongoCpp
{

class collection
{
public:
    collection(void * db, std::string name);
    ~collection();

    cursor aggregate(bsonCpp::doc_value & pipeline, bsonCpp::doc_value & options);
    bool index();
    bool update_one(bsonCpp::doc_value & selector, bsonCpp::doc_value & updater, bsonCpp::doc_value & options);
private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};

}

#endif /* _MONGO_COLLECTION_H_ */