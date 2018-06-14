#include "mongo_internal.h"

namespace mongoCpp
{

uri::uri(std::string & uri_string) : impl_(new Impl(uri_string))
{
}

uri::~uri()
{
}

const boost::shared_ptr<uri::Impl>& uri::get() const 
{
    return impl_;
}

}