#ifndef slic3r_HeadlessShims_hpp_
#define slic3r_HeadlessShims_hpp_

// Headless CLI only (SLIC3R_HEADLESS). Stand-ins for a few small helpers that live in
// wxWidgets-heavy GUI files. Each copy names its source; keep it in sync on upstream merges.

#ifndef SLIC3R_HEADLESS
#error "HeadlessShims.hpp is only for the headless CLI build"
#endif

#include <array>
#include <cstring>
#include <string>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>

namespace Slic3r {
class DynamicPrintConfig;
namespace GUI {

// Defined in Plater.cpp (compiled headless as a carve-out, see the SLIC3R_HEADLESS guards there).
std::vector<int> get_min_flush_volumes(const DynamicPrintConfig &full_config, size_t nozzle_id);

// Copy of hex_digit_to_int() from GUI_Utils.hpp.
inline int headless_hex_digit_to_int(const char c)
{
    return
        (c >= '0' && c <= '9') ? int(c - '0') :
        (c >= 'A' && c <= 'F') ? int(c - 'A') + 10 :
        (c >= 'a' && c <= 'f') ? int(c - 'a') + 10 : -1;
}

// Copy of BitmapCache::parse_color4() from BitmapCache.cpp; keeps the call sites unchanged.
struct BitmapCache
{
    static bool parse_color4(const std::string& scolor, unsigned char* rgba_out)
    {
        rgba_out[0] = rgba_out[1] = rgba_out[2] = 0; rgba_out[3] = 255;
        if ((scolor.size() != 7 && scolor.size() != 9) || scolor.front() != '#')
            return false;
        const char* c = scolor.data() + 1;
        for (size_t i = 0; i < scolor.size() / 2; ++i) {
            int digit1 = headless_hex_digit_to_int(*c++);
            int digit2 = headless_hex_digit_to_int(*c++);
            if (digit1 == -1 || digit2 == -1)
                return false;
            rgba_out[i] = (unsigned char)(digit1 * 16 + digit2);
        }
        return true;
    }
};

// Copy of Plater::has_illegal_filename_characters(const std::string&) from Plater.cpp.
inline bool headless_has_illegal_filename_characters(const std::string& name)
{
    const char* illegal_characters = "<>:/\\|?*\"";
    for (size_t i = 0; i < std::strlen(illegal_characters); i++)
        if (name.find_first_of(illegal_characters[i]) != std::string::npos)
            return true;
    std::array<std::string, 5> escape_characters = {"&lt;", "&gt;", "&amp;", "&quot;", "&apos;"};
    for (auto escape : escape_characters) {
        if (boost::contains(name, escape)) {
            return true;
        }
    }
    return false;
}

} // namespace GUI
} // namespace Slic3r

#endif // slic3r_HeadlessShims_hpp_
