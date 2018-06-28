#ifndef _MONGO_CURSOR_H_
#define _MONGO_CURSOR_H_

#include "bson_view.h"
#include <boost/shared_ptr.hpp>

namespace mongoCpp
{

class cursor
{
public:
    cursor(void * curs);
    bool next();
    bsonCpp::view getResult();
private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};

}

#endif /* _MONGO_CURSOR_H_ */