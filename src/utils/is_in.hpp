#include <algorithm>
#include <initializer_list>

// source of this sourcery:
// https://stackoverflow.com/questions/15181579/most-efficient-way-to-compare-a-variable-to-multiple-values

template <typename T, typename item_type>
bool isIn(const T& v, std::initializer_list<item_type> lst)
{
    return std::find(std::begin(lst), std::end(lst), v) != std::end(lst);
}
