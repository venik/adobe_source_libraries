/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <adobe/algorithm/select.hpp>

#include <functional>

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp> // NOLINT(misc-include-cleaner)

BOOST_AUTO_TEST_CASE(select_test) {
    adobe::select_0_2(10, 5, std::less<>());
    /* PLACEHOLDER - Currently select.hpp is well tested by median and clamp. */
}
