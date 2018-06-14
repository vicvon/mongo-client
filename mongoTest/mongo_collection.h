#ifndef _MONGO_COLLECTION_H_
#define _MONGO_COLLECTION_H_

#include <boost/shared_ptr.hpp>

namespace mongoCpp
{

class collection
{
public:
    collection(void * db, std::string name);
    ~collection();
private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};

}

#endif /* _MONGO_COLLECTION_H_ */