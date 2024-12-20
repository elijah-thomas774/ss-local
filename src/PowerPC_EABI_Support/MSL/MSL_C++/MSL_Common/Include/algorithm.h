#ifndef MSL_ALGORITHM_H_
#define MSL_ALGORITHM_H_

namespace std {
template <class ForwardIterator, class T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T &val);

template <class ForwardIterator, class T>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T &val);

template <class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p);

/*
template<class OutputIt, class Size, int A2>
struct __fill_n {
    OutputIt fill_n(OutputIt first, Size count, const unsigned long& value);
};

template<>
unsigned long* __fill_n<unsigned long, long, 0>::fill_n(unsigned long* first, long count, const unsigned long& value) {
    for (; count > 0; count--) {
        *first++ = value;
    }
    return first;
}

template<class OutputIt, class Size, class T>
OutputIt fill_n(OutputIt first, Size count, const T& value) {
    return __fill_n::fill_n(first, count, value);
}


template<class ForwardIt, class T>
void __fill(ForwardIt first, ForwardIt last, const T& value, std::random_access_iterator_tag param_3) {
    fill_n(first, last - first, value);
}
*/

template <class ForwardIt, class T>
void fill(ForwardIt first, ForwardIt last, const T &value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

template <class InputIt, class OutputIt>
inline OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    for (; first < last; ++first, ++d_first) {
        *d_first = *first;
    }
    return d_first;
}

template <class BidirIt1, class BidirIt2>
inline BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
    while (first != last) {
        *(--d_last) = *(--last);
    }
    return d_last;
}

} // namespace std

#endif
