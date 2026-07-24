/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#define BOOST_TEST_MAIN

// File being tested is included first
#include <adobe/closed_hash.hpp>

#include <cassert>
#include <iterator>

#include <boost/test/unit_test.hpp> // NOLINT(misc-include-cleaner)

#include <adobe/test/check_regular.hpp>

/**************************************************************************************************/

namespace adobe {

/*
    Precondition: x != T(), remote_address(T) is defined.
*/

template <typename T>
void test_movable(const T& x) {
    assert(x != T());
    // assert(is_movable<T>::value);

    adobe::check_regular(x);

    T y = x;
    // move construction (and RVO)
    const void* addr = remote_address(y);
    T z = std::move(y);
#ifndef __clang_analyzer__
    // Use after move
    BOOST_CHECK(y == T());
#endif
    BOOST_CHECK(z == x);
    BOOST_CHECK(remote_address(z) == addr);
    // move assignment
    y = std::move(z);
#ifndef __clang_analyzer__
    // Use after move
    BOOST_CHECK(z == T());
#endif
    BOOST_CHECK(y == x);
    BOOST_CHECK(remote_address(y) == addr);
}

template <typename T>
const void* remote_address(const std::vector<T>& x) {
    assert(!x.empty());
    return &*x.begin();
}

template <typename T>
const void* remote_address(const closed_hash_set<T>& x) {
    assert(!x.empty());
    return &*x.begin();
}

template <typename Key, typename Value>
const void* remote_address(const closed_hash_map<Key, Value>& x) {
    assert(!x.empty());
    return &*x.begin();
}

} // namespace adobe

BOOST_AUTO_TEST_CASE(closed_hash) {
    using namespace adobe;

    typedef adobe::closed_hash_set<int> hash_set_t;
    typedef adobe::closed_hash_map<int, double> hash_map_t;

    typedef std::vector<int> vector_t;
    typedef adobe::closed_hash_map<int, vector_t> hash_map_vector_t;

    {
        int a[] = {0, 1, 2, 3, 4, 5};
        adobe::test_movable(hash_set_t(std::begin(a), std::end(a)));
    }
    {
        std::pair<int, double> a[] = {std::make_pair(0, 0.0), std::make_pair(1, 1.1),
                                      std::make_pair(2, 2.2)};
        adobe::test_movable(hash_map_t(std::begin(a), std::end(a)));
    }

    { // empty properties
        hash_set_t x;
        BOOST_CHECK(x.begin() == x.end());
        BOOST_CHECK(x.empty());
        BOOST_CHECK(x.capacity() == 0);
        BOOST_CHECK(x.size() == 0);
    }

    { // empty properties
        hash_map_t x;
        BOOST_CHECK(x.begin() == x.end());
        BOOST_CHECK(x.empty());
        BOOST_CHECK(x.capacity() == 0);
        BOOST_CHECK(x.size() == 0);
    }

    { // non-empty properties
        int a[] = {0, 1, 2};
        hash_set_t x(std::begin(a), std::end(a));
        BOOST_CHECK(x.begin() != x.end());
        BOOST_CHECK(!x.empty());
        BOOST_CHECK(std::next(x.begin(), 3) == x.end());
        BOOST_CHECK(x.capacity() != 0);
        BOOST_CHECK(x.size() == 3);
    }

    { // non-empty properties
        std::pair<int, double> a[] = {std::make_pair(0, 0.0), std::make_pair(1, 1.1),
                                      std::make_pair(2, 2.2)};
        hash_map_t x(std::begin(a), std::end(a));
        BOOST_CHECK(x.begin() != x.end());
        BOOST_CHECK(!x.empty());
        BOOST_CHECK(std::next(x.begin(), 3) == x.end());
        BOOST_CHECK(x.capacity() != 0);
        BOOST_CHECK(x.size() == 3);
    }

    { // iterators & indexing
        int a[] = {0, 1, 2};
        const hash_set_t x(std::begin(a), std::end(a));
        hash_set_t y = x;
        BOOST_CHECK(std::next(x.begin(), 3) == x.end());
        BOOST_CHECK(std::next(x.rbegin(), 3) == x.rend());
        BOOST_CHECK(std::next(y.rbegin(), 3) == y.rend());
    }

    { // iterators & indexing
        std::pair<int, double> a[] = {std::make_pair(0, 0.0), std::make_pair(1, 1.1),
                                      std::make_pair(2, 2.2)};
        const hash_map_t x(std::begin(a), std::end(a));
        hash_map_t y = x;
        BOOST_CHECK(std::next(x.begin(), 3) == x.end());
        BOOST_CHECK(std::next(x.rbegin(), 3) == x.rend());
        BOOST_CHECK(std::next(y.rbegin(), 3) == y.rend());
        BOOST_CHECK(y[1] == 1.1);
        y[1] = 5.5;
        BOOST_CHECK(y[1] == 5.5);
    }

    { // reserve
        std::pair<int, vector_t> a[] = {std::make_pair(1, vector_t(1, 1)),
                                        std::make_pair(2, vector_t(2, 2)),
                                        std::make_pair(3, vector_t(3, 3))};
        hash_map_vector_t x(std::begin(a), std::end(a));
        // items may still relocate even if capacity is reserved.
        // const void* addr = remote_address(x.find(2)->second);

        std::size_t c = x.capacity();
        x.reserve(2 * x.capacity());
        BOOST_CHECK(x.capacity() > c);
        BOOST_CHECK(x.size() == 3);
        // BOOST_CHECK(addr == remote_address(x.find(2)->second));
    }
}
