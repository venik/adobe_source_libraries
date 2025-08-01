/*
    Copyright 2008 Adobe Systems Incorporated
    Distributed under the Boost Software License - Version 1.0 (see the accompanying file LICENSE
    or a copy at https://stlab.github.io/adobe_source_libraries/licenses.html)
*/

/**************************************************************************************************/

/*!
\author Marshall Clow
\date    January 2008
*/

#ifndef ADOBE_ALGORITHM_GATHER_HPP
#define ADOBE_ALGORITHM_GATHER_HPP

#include <algorithm> // for std::table_partition
#include <functional>

#include <boost/range/begin.hpp> // for boost::begin(range)
#include <boost/range/end.hpp>   // for boost::end(range)


/**************************************************************************************************/
/*!
    \defgroup gather gather
    \ingroup mutating_algorithm

    \c gather() takes a collection of elements defined by a pair of iterators and moves
    the ones satisfying a predicate to them to a position (called the pivot) within
    the sequence. The algorithm is stable. The result is a pair of iterators that
    contains the items that satisfy the predicate.

    Given an sequence containing:
    <pre>
    0 1 2 3 4 5 6 7 8 9
    </pre>

    a call to gather ( arr, arr + 10, arr + 4, IsEven ()) will result in:

    <pre>
    1 3 0 2 4 6 8 5 7 9
        |---|-----|
      first |  second
          pivot
    </pre>


    The problem is broken down into two basic steps, namely, moving the items before the pivot
    and then moving the items from the pivot to the end. These "moves" are done with calls to
    stable_partition.

    \par Storage Requirements:

    The algorithm uses stable_partition, which will attempt to allocate temporary memory,
    but will work in-situ if there is none available.

    \par Time Complexity:

    If there is sufficient memory available, the run time is linear in <code>N</code>.
    If there is not any memory available, then the run time is <code>O(N log N)</code>.
*/

/**************************************************************************************************/

namespace adobe {

/**************************************************************************************************/

/*!
    \ingroup gather
    \brief iterator-based gather implementation
*/

template <typename Iter, // Iter models BidirectionalIterator
          typename Pred>
// Pred models UnaryPredicate
std::pair<Iter, Iter> gather(Iter first, Iter last, Iter pivot, Pred pred) {
    //	The first call partitions everything up to (but not including) the pivot element,
    //	while the second call partitions the rest of the sequence.
    return std::make_pair(
        std::stable_partition(
            first, pivot,
            std::bind(std::logical_not<bool>(), std::bind(pred, std::placeholders::_1))),
        std::stable_partition(pivot, last, std::bind(pred, std::placeholders::_1)));
}

/**************************************************************************************************/

/*!
    \ingroup gather
    \brief range-based gather implementation
*/

template <typename BidirectionalRange, //
          typename Pred>               // Pred models UnaryPredicate
std::pair<typename boost::range_iterator<BidirectionalRange>::type,
          typename boost::range_iterator<BidirectionalRange>::type>
gather(BidirectionalRange& range, typename boost::range_iterator<BidirectionalRange>::type pivot,
       Pred pred) {
    return adobe::gather(boost::begin(range), boost::end(range), pivot, pred);
}

/**************************************************************************************************/

} // namespace adobe

/**************************************************************************************************/

#endif
