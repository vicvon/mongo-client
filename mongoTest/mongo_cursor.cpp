#include "mongo_cursor.h"
#include <mongoc.h>
#include <stdexcept>
#include <boost/format.hpp>

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
        boost::format fmt = boost::format("mongoc cursor next error, error message: %s") % err.message;
        throw std::runtime_error(fmt.str());
    }

    return false;
}

bsonCpp::view cursor::getResult()
{
    return bsonCpp::view(bson_get_data(impl_->out), impl_->out->len);
}

}