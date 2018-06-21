#include "mongo_cursor.h"
#include <mongoc.h>

namespace mongoCpp
{

struct cursor::Impl
{
    Impl(mongoc_cursor_t * curs):curs_(curs)
    {
    }
    ~Impl()
    {
        mongoc_cursor_destroy(curs_);
    }
    mongoc_cursor_t * curs_;
    const bson_t * out;
};

cursor::cursor(void * curs):impl_(new Impl(static_cast<mongoc_cursor_t *>(curs)))
{
}

bool cursor::next()
{
    bson_error_t err;
    if (mongoc_cursor_next(impl_->curs_, &impl_->out))
    {
        return true;
    }
    else if (mongoc_cursor_error(impl_->curs_, &err))
    {
    }

    return false;
}

}