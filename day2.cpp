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
#include <sstream>

// I don't know how to architecture this sum some functions were supposed were supposed
// to return uint64s and the addition would get done somewhere but I lost track, so I used a global

uint64_t to_uint(std::string_view view) {
    auto result = uint64_t{};
    std::ignore = std::from_chars(view.data(), view.data() + view.size(), result);
    return result;
}
uint64_t count_invalid_ids_n(uint64_t begin, uint64_t end, uint32_t n) {
    // make the string repeat the first half over and see if its range
    auto begin_str = std::to_string(begin);
    for (auto i : std::views::iota(1u, begin_str.size() / n)) {
        for (auto j : std::views::iota(0u, n)) {
            begin_str[i * n + j] = begin_str[j];
        }
    }
    auto val = to_uint(begin_str);
    if (val >= begin && val <= end) {
        std::println("{}", val);
        return val;
    }
    return 0;
}
uint64_t count_invalid_ids(uint64_t begin, uint64_t end) {
    // you could go over every value in the range but thats
    // not inteneded (probably) and slow as cheeks
    auto begin_str = std::to_string(begin);
    auto end_str = std::to_string(end);
    if (begin_str.size() != end_str.size())
    {
        uint32_t smallest_number_with_same_digits_as_end_str = 1;
        for (auto i : std::views::iota(0u, end_str.size() - 1)) smallest_number_with_same_digits_as_end_str *= 10;
        return count_invalid_ids(begin, smallest_number_with_same_digits_as_end_str - 1) +
        count_invalid_ids(smallest_number_with_same_digits_as_end_str, end);
    }

    if (begin_str.size() % 2 != 0) return 0;
    auto n = begin_str.size() / 2;
    auto begin_first_n = std::string_view{ begin_str.begin(), begin_str.begin() + n };
    auto end_first_n = std::string_view{ end_str.begin(), end_str.begin() + n };

    auto bfn_as_int = to_uint(begin_first_n);
    auto efn_as_int = to_uint(end_first_n);

    if (bfn_as_int != efn_as_int) {
        // the first half has to be the same so we split the range
        // into subranges where the first subhalf is the same
        // eg 123087 - 125924 -> [123087 - 123999, 12400 - 125924] (recursion handles the second part)
        std::ranges::fill(begin_str.begin() + n, begin_str.end(), '9');
        // this portion might be really slow
        auto new_begin = to_uint(begin_str);
        return count_invalid_ids(begin, new_begin) +
        count_invalid_ids(new_begin + 1, end);
    }
    else {
        // I don't know why this function is named this way, it only returns the invalid id
        // not a count
        return count_invalid_ids_n(to_uint(begin_str), to_uint(end_str), n);
    }
}
int main() {
    std::stringstream input;
    input <<
        "52-75,71615244-71792700,89451761-89562523,"
        "594077-672686,31503-39016,733-976,1-20,400309-479672,"
        "458-635,836793365-836858811,3395595155-3395672258,290-391,"
        "5168-7482,4545413413-4545538932,65590172-65702074,25-42,"
        "221412-256187,873499-1078482,118-154,68597355-68768392,"
        "102907-146478,4251706-4487069,64895-87330,8664371543-8664413195,"
        "4091-5065,537300-565631,77-115,83892238-83982935,6631446-6694349,"
        "1112-1649,7725-9776,1453397-1493799,10240-12328,15873-20410,1925-2744,"
        "4362535948-4362554186,3078725-3256936,710512-853550,279817-346202,45515-60928,3240-3952";
    std::string string;
    uint64_t sum = 0;
    while (std::getline(input, string, ',')) {
        auto dash = string.find_first_of('-');
        sum += count_invalid_ids(
            to_uint(std::string_view{ string.begin(), string.begin() + dash }),
            to_uint(std::string_view{ string.begin() + dash + 1, string.end() })
        );
    }
    std::println("sum = {}", sum);
}