/***************************************************************************
 *            benchmark_command_line.hpp
 *
 *  Shared conventions with ariadne-cps/numeric/benchmarks:
 *  Ariadne threading/logging CLI, validated values and odd sample counts.
 *
 *  This file is part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/
#ifndef ARIADNE_PAVING_BENCHMARK_COMMAND_LINE_HPP
#define ARIADNE_PAVING_BENCHMARK_COMMAND_LINE_HPP

#include "utility/command_line_interface.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace Ariadne::PavingBenchmarkCommandLine {

// ThreadingCommandLineInterface initializes ThreadManager and attaches
// the real thread registry to Logger; do not install a separate registry.
// Follow numeric's parse_unsigned: the logging CLI owns error reporting.
template<class UInt>
UInt parse_unsigned(String const& value) {
    const std::string text(value);
    if (text.empty() or text.front()=='-') {
        throw std::invalid_argument("expected nonnegative integer");
    }
    std::size_t used=0u;
    unsigned long long parsed=std::stoull(text,&used);
    if (used!=text.size() or parsed>std::numeric_limits<UInt>::max()) {
        throw std::invalid_argument("integer out of range");
    }
    return static_cast<UInt>(parsed);
}

inline std::vector<unsigned> parse_unique_unsigned_list(
        String const& value,unsigned minimum) {
    const std::string text(value);
    if (text.empty() or text.front()==',' or text.back()==',') {
        throw std::invalid_argument("invalid integer list");
    }
    std::vector<unsigned> values;
    std::size_t start=0u;
    do {
        std::size_t end=text.find(',',start);
        std::string item=text.substr(
            start,end==std::string::npos?end:end-start);
        unsigned number=parse_unsigned<unsigned>(String(item));
        if (number<minimum or
                std::find(values.begin(),values.end(),number)!=values.end()) {
            throw std::invalid_argument("invalid or duplicate integer");
        }
        values.push_back(number);
        if (end==std::string::npos) { break; }
        start=end+1u;
    } while (true);
    return values;
}

inline bool requests_help(List<String> const& args) {
    return std::find(args.begin(),args.end(),String("--help"))!=args.end()
        or std::find(args.begin(),args.end(),String("-h"))!=args.end();
}

} // namespace Ariadne::PavingBenchmarkCommandLine

#endif
