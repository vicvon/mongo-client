#ifndef _MONGO_CURSOR_H_
#define _MONGO_CURSOR_H_

#include "bson_value.h"
#include <boost/shared_ptr.hpp>

namespace mongoCpp
{

class cursor
{
public:
    cursor(void * curs);

    bool next();
private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};

}

#endif /* _MONGO_CURSOR_H_ */