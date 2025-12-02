#include <algorithm>
#include <charconv>
#include <concurrencysal.h>
#include <cstdint>
#include <print>
#include <string>
#include <generator>
#include <cmath>
#include <string_view>
#include <ranges>
uint64_t to_uint(std::string_view view) {
    auto result = uint64_t{};
    std::ignore = std::from_chars(view.data(), view.data() + view.size(), result);
    return result;
}
uint64_t count_invalid_ids_n(uint64_t begin, uint64_t end, uint32_t n) {
    auto begin_str = std::to_string(begin);
    for(auto i : std::views::iota(1u, begin_str.size() / n)) {
        for(auto j : std::views::iota(0u, n)) {
            begin_str[i * n + j] = begin_str[j];
        }
    }
    auto val = to_uint(begin_str);
    if(val >= begin && val <= end) {
        std::println("{}", val);
    }
    return 0;
}
std::generator<uint32_t> factors_of(uint32_t n) {
    for(auto i : std::views::iota(1u, n)) {
        if(n % i == 0) co_yield i;
    }
}
uint64_t count_invalid_ids(uint64_t begin, uint64_t end) {
    // you could go over every value in the range but thats
    // not inteneded (probably) and slow as cheeks

    auto begin_str = std::to_string(begin);
    auto end_str = std::to_string(end);

    // n is every factor less than size because for a seq of digits to repeat, it must
    // repeat at least twice

    for(auto n : factors_of(begin_str.size())) {
        auto begin_first_n = std::string_view{begin_str.begin(), begin_str.begin() + n};
        auto end_first_n = std::string_view{end_str.begin(), end_str.begin() + n};

        auto bfn_as_int = to_uint(begin_first_n);
        auto efn_as_int = to_uint(end_first_n);

        if(bfn_as_int != efn_as_int) {
            // segment it then send
        } else {
            count_invalid_ids_n(to_uint(begin_str), to_uint(end_str), n);
        }
    }
    return 0;
}
int main() {
    count_invalid_ids(1188511880, 1188511890);
}