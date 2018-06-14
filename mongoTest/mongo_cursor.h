#ifndef _MONGO_CURSOR_H_
#define _MONGO_CURSOR_H_

#include <boost/shared_ptr.hpp>

namespace mongoCpp
{

class cursor
{
public:
    cursor(void * curs);
private:
    struct Impl;
    boost::shared_ptr<Impl> impl_;
};

}

#endif /* _MONGO_CURSOR_H_ */