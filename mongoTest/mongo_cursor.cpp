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
};

cursor::cursor(void * curs):impl_(new Impl(static_cast<mongoc_cursor_t *>(curs)))
{
}

}