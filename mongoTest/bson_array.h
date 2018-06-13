#ifndef _BSON_ARRAY_H_
#define _BSON_ARRAY_H_

#include "bson_core.h"
#include "bson_value.h"

#include <boost/tuple/tuple.hpp>

namespace bsonCpp
{
class array
{
public:
    array() : core_(true)
    {
    }

    ~array()
    {
    }

    template <typename T>
    array & append(T & v)
    {
        core_.append(v);

        return *this;
    }

    arr_value extract()
    {
        return core_.extractor_array();
    }
private:
    core core_;
};

template <typename T0>
arr_value make_array(T0 v0)
{
    array arr;
    arr.append(v0);

    return arr.extract();
}

template <typename T0, typename T1>
arr_value make_array(T0 v0, T1 v1)
{
    array arr;
    arr.append(v0).append(v1);

    return arr.extract();
}

template <typename T0, typename T1, typename T2>
arr_value make_array(T0 v0, T1 v1, T2 v2)
{
    array arr;
    arr.append(v0).append(v1).append(v2);

    return arr.extract();
}

template <typename T0, typename T1, typename T2, typename T3>
arr_value make_array(T0 v0, T1 v1, T2 v2, T3 v3)
{
    array arr;
    arr.append(v0).append(v1).append(v2).append(v3);

    return arr.extract();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4>
arr_value make_array(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4)
{
    array arr;
    arr.append(v0).append(v1).append(v2).append(v3).append(v4);

    return arr.extract();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
arr_value make_array(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5)
{
    array arr;
    arr.append(v0).append(v1).append(v2).append(v3).append(v4).append(v5);

    return arr.extract();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
arr_value make_array(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6)
{
    array arr;
    arr.append(v0).append(v1).append(v2).append(v3).append(v4).append(v5).append(v6);

    return arr.extract();
}

template <typename T0, typename T1, typename T2, typename T3, 
          typename T4, typename T5, typename T6, typename T7>
arr_value make_array(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7)
{
    array arr;
    arr.append(v0).append(v1).append(v2).append(v3).append(v4).append(v5).append(v6).append(v7);

    return arr.extract();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
arr_value make_array(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8)
{
    array arr;
    arr.append(v0).append(v1).append(v2).append(v3).append(v4)
       .append(v5).append(v6).append(v7).append(v8);

    return arr.extract();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9>
arr_value make_array(T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9)
{
    array arr;
    arr.append(v0).append(v1).append(v2).append(v3).append(v4)
       .append(v5).append(v6).append(v7).append(v8).append(v9);

    return arr.extract();
}

}

#endif /* _BSON_ARRAY_H_ */