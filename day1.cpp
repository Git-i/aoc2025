#include <charconv>
#include <cstdint>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <print>
#include <vector>
enum class Direction { L, R };
uint32_t rotate_dial(uint32_t from, std::pair<Direction, uint32_t> smth) {
    from += 100 * 100;
    if(smth.first == Direction::L) from -= smth.second;
    else from += smth.second;
    return from % 100;
}
std::pair<Direction, uint32_t> parse_rotation(std::string_view rot) {
    uint32_t res;
    std::ignore = std::from_chars(rot.data() + 1, rot.data() + rot.size(), res);
    return std::make_pair(
        rot[0] == 'L' ? Direction::L : Direction::R,
        res
    );
}
std::vector<std::string_view> split_lines(const std::string& str) {
    size_t off = 0;
    auto val = str.find_first_of('\n', off);
    std::vector<std::string_view> result;
    while(val != str.npos) {
        result.emplace_back(str.begin() + off, str.begin() + val);
        off = val + 1;
        val = str.find_first_of('\n', off);
    }
    result.emplace_back(str.begin() + off, str.end());
    return result;
}
int main() {
    std::ostringstream test_string_str;
    test_string_str << std::ifstream("puzzle_inputs/day1.txt").rdbuf();

    auto test_string = test_string_str.str(); 
    uint32_t dial_pos = 50;
    auto lines = split_lines(test_string);
    uint32_t count = 0;
    std::println("{}", lines.size());
    for(auto line : lines) {
        dial_pos = rotate_dial(dial_pos, parse_rotation(line));
        count += (dial_pos == 0) ? 1 : 0;
    }
    std::println("{}", count);
}