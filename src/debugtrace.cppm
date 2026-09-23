/// debugtrace.cppm
/// (C) 2017 Masato Kokubo
module;
#if defined _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <cwchar>
    #ifndef __APPLE__
        #include <cuchar>
    #endif
#endif
#ifdef _MSC_VER
    #include <time.h>
#endif
#ifdef __GNUG__
    #include <climits>
    #include <cstdlib>
    #include <cstring>
    #include <cxxabi.h> // abi::__cxa_demangle()
#endif

#define TO_STRING1(v) #v
#define TO_STRING(v) TO_STRING1(v)
#if defined __GNUG__
    #define COMPILER_VERSION "g++ " __VERSION__ " (" TO_STRING(__cplusplus) ")"
#elif defined __clang__
    #define COMPILER_VERSION "clang " __VERSION__ " (" TO_STRING(__cplusplus) ")"
#elif defined _MSC_VER
    #define COMPILER_VERSION "Microsoft Visual C++ " TO_STRING(_MSC_FULL_VER) " (" TO_STRING(_MSVC_LANG) ")"
#endif

export module debugtrace;
import std;

export namespace debugtrace {
    char const* const _version = "3.0.0";
    const std::string _start_message = std::string("DebugTrace-cpp ") + _version + " compiled with " + COMPILER_VERSION;
    char const*       enter_string              = "Enter ";
    char const*       leave_string              = "Leave ";
    char const*       limit_string              = "...";
    int               maximum_indents           = 20;
    char const*       code_indent_string        = "| ";
    char const*       data_indent_string        = "  ";
    char const*       open_string               = "{";
    char const*       close_string              = "}";
    char const*       varname_value_separator   = " = ";
    char const*       pair_separator            = ": ";
    char const*       log_datetime_format       = "%Y-%m-%d %H:%M:%S%z";
    size_t            maximum_data_output_width = 80;
    size_t            collection_limit          = 256;
    bool              _initialized              = false;
    char const*       output_target             = "std::cerr";
    int               _code_nest_level          = 0;
    int               _before_code_nest_level   = 0;
    int               _data_nest_level          = 0;
    unsigned int      _code_page                = 65001; // UTF-8

    using std::to_string;
    template <typename T>
    concept has_to_string = requires(T const& value) {
        { to_string(value) } -> std::convertible_to<std::string>;
    };

    /// Returns a string for code indent.
    std::string _get_code_indent_string() {
        std::string indent_str;
        for (auto index = 0; index < _code_nest_level && index < maximum_indents; ++index)
            indent_str += code_indent_string;
        return indent_str;
    }

    /// Returns a string for data indent.
    std::string _get_data_indent_string() {
        std::string indent_str;
        for (auto index = 0; index < _data_nest_level && index < maximum_indents; ++index)
            indent_str += data_indent_string;
        return indent_str;
    }

    /// Outputs the date and time.
    std::string _get_log_datetime() {
        const auto now = std::time(nullptr);
    #ifdef _MSC_VER
        // Visual C++
        struct std::tm datetime;
        localtime_s(&datetime, &now);
    //    auto datetime_p = std::localtime(&now);
    //    auto datetime = *datetime_p;
    #else
        // Others
        const auto datetime = *std::localtime(&now);
    #endif // _MSC_VER
        char buff[64];
        std::strftime(buff, sizeof(buff), log_datetime_format, &datetime);
        return buff;
    }

    /// Sets Windwos Code Page
    /// @param codePage the code page (e.g.: CP_ACP, CP_UTF8)
    void set_code_page(unsigned int codePage) {
        _code_page = codePage;
    }

    /// Returns a string representation of the type of the value.
    /// @param value the value to output
    template <typename T>
    std::string _get_type_string(T const& value, size_t size = -1) {
        auto type_string = std::string("(");
    #ifdef _MSC_VER
        // Visual C++
        type_string += typeid(value).name();
    #else
        // gcc, clang
        int status;
        char* type_name = abi::__cxa_demangle(typeid(value).name(), nullptr, nullptr, &status);
        type_string += (type_name == nullptr ? "?" : type_name);
        if (type_name != nullptr)
            std::free(type_name);
    #endif // _MSC_VER
        if ((int)size != -1) {
            type_string += " size:";
            type_string += std::to_string(size);
        }
        type_string += ')';
        return type_string;
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(bool const& value) {
        return std::vector<std::string>({value ? "true" : "false"});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(char const& value) {
        return std::vector<std::string>({"(char)" + std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(signed char const& value) {
        return std::vector<std::string>({"(signed char)" + std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(unsigned char const& value) {
        return std::vector<std::string>({"(unsigned char)" + std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(short const& value) {
        return std::vector<std::string>({"(short)" + std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(unsigned short const& value) {
        return std::vector<std::string>({"(unsigned short)" + std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(int const& value) {
        return std::vector<std::string>({std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(unsigned int const& value) {
        return std::vector<std::string>({std::to_string(value) + 'u'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(long const& value) {
        return std::vector<std::string>({std::to_string(value) + 'l'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(unsigned long const& value) {
        return std::vector<std::string>({std::to_string(value) + "ul"});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(long long const& value) {
        return std::vector<std::string>({std::to_string(value) + "ll"});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(unsigned long long const& value) {
        return std::vector<std::string>({std::to_string(value) + "ull"});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(float const& value) {
        return std::vector<std::string>({std::to_string(value) + 'f'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(double const& value) {
        return std::vector<std::string>({std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(long double const& value) {
        return std::vector<std::string>({std::to_string(value) + 'l'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(wchar_t const& value) {
        return std::vector<std::string>({"(wchar_t)" + std::to_string(value)});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(char* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(char*)nullptr"
            : "(char*)\"" + std::string(value) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(char const* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(const char*)nullptr"
            : "(const char*)\"" + std::string(value) + '"'
        });
    }


    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(signed char* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(signed char*)nullptr"
            : "(signed char*)\"" + std::string((char*)value) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(signed char const* const& value) {
        return std::vector<std::string>({value == nullptr
            ?"(const signed char*)nullptr"
            :"(const signed char*)\"" + std::string((const char*)value) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(unsigned char* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(unsigned char*)nullptr"
            : "(unsigned char*)\"" + std::string((char *)value) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(unsigned char const* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(const unsigned char*)nullptr"
            : "(const unsigned char*)\"" + std::string((const char*)value) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const std::string& value) {
        return std::vector<std::string>({"(std::string)\"" + value + '"'});
    }

    /// Convert std::wstring to std::string.
    /// @param wstring the std::wstring
    /// @return a converted std::string
    std::string _to_string(const std::wstring& wstring) {
    #ifdef _WIN32
        const auto wstringLen = (int)wstring.length();
        const auto stringLen = WideCharToMultiByte(_code_page, 0, wstring.c_str(), wstringLen, nullptr, 0, 0, 0);
        std::string string(stringLen, '\0');
        WideCharToMultiByte(_code_page, 0, wstring.c_str(), wstringLen, &string[0], stringLen, 0, 0);
        return string;
    #else
        mbstate_t mbstate{};
        std::string result;
        result.reserve(wstring.length() * MB_CUR_MAX);
        char buf[MB_LEN_MAX];

        for (auto const& wchar : wstring) {
            const auto converted_count = wcrtomb(buf, wchar, &mbstate);
            if (converted_count == (size_t)-1)
                return "<Cannot convert the wstring to string>";
            result.append(buf, converted_count);
        }
        return result;
    #endif // _WIN32
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const std::wstring& value) {
        return std::vector<std::string>({"(std::wstring)\"" + _to_string(value) + '"'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(wchar_t* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(wchar_t*)nullptr"
            : "(wchar_t*)\"" + _to_string(std::wstring(value)) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(wchar_t const* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(const wchar_t*)nullptr"
            : "(const wchar_t*)\"" + _to_string(std::wstring(value)) + '"'
        });
    }

    /// Convert std::u16string to std::string.
    /// @param u16string the std::u16string
    std::string _to_string(const std::u16string& u16string) {
    #ifdef _WIN32
        const auto u16stringLen = (int)u16string.length();
        const auto stringLen = WideCharToMultiByte(_code_page, 0, (LPCWCH)u16string.c_str(), u16stringLen, nullptr, 0, 0, 0);
        std::string string(stringLen, '\0');
        WideCharToMultiByte(_code_page, 0, (LPCWCH)u16string.c_str(), u16stringLen, &string[0], stringLen, 0, 0);
        return string;
    #elif defined __APPLE__
        // Darwin has 32-bit wchar_t, but does not provide c16rtomb.
        std::wstring wstring;
        wstring.reserve(u16string.length());
        for (std::size_t i = 0; i < u16string.length(); ++i) {
            char32_t code_point = u16string[i];
            if (code_point >= 0xD800 && code_point <= 0xDBFF) {
                if (++i == u16string.length())
                    return "<Cannot convert the u16string to string>";
                const char32_t low = u16string[i];
                if (low < 0xDC00 || low > 0xDFFF)
                    return "<Cannot convert the u16string to string>";
                code_point = 0x10000 + ((code_point - 0xD800) << 10) + (low - 0xDC00);
            } else if (code_point >= 0xDC00 && code_point <= 0xDFFF) {
                return "<Cannot convert the u16string to string>";
            }
            wstring.push_back(static_cast<wchar_t>(code_point));
        }
        const auto result = _to_string(wstring);
        return result == "<Cannot convert the wstring to string>"
            ? "<Cannot convert the u16string to string>" : result;
    #else
        mbstate_t mbstate{};
        std::string result;
        result.reserve(u16string.length() * MB_CUR_MAX);
        char buf[MB_LEN_MAX];

        for (auto const& char16 : u16string) {
            const auto converted_count = c16rtomb(buf, char16, &mbstate);
            if (converted_count == (size_t)-1)
                return "<Cannot convert the u16string to string>";
            result.append(buf, converted_count);
        }
        return result;
    #endif // _WIN32
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const std::u16string& value) {
        return std::vector<std::string>({"(std::u16string)\"" + _to_string(value) + '"'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(char16_t* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(char16_t*)nullptr"
            : "(char16_t*)\"" + _to_string(std::u16string(value)) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const char16_t* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(const char16_t*)nullptr"
            : "(const char16_t*)\"" + _to_string(std::u16string(value)) + '"'
        });
    }

    /// Convert std::u32string to std::string.
    /// @param u32string the std::u32string
    std::string _to_string(const std::u32string& u32string) {
    #ifdef _WIN32
        return "<Unimplemented(string <- u32string)>";
    #elif defined __APPLE__
        // Darwin has 32-bit wchar_t, but does not provide c32rtomb.
        std::wstring wstring;
        wstring.reserve(u32string.length());
        for (const char32_t code_point : u32string) {
            if (code_point > 0x10FFFF || (code_point >= 0xD800 && code_point <= 0xDFFF))
                return "<Cannot convert the u32string to string>";
            wstring.push_back(static_cast<wchar_t>(code_point));
        }
        const auto result = _to_string(wstring);
        return result == "<Cannot convert the wstring to string>"
            ? "<Cannot convert the u32string to string>" : result;
    #else
        mbstate_t mbstate{};
        std::string result;
        result.reserve(u32string.length() * MB_CUR_MAX);
        char buf[MB_LEN_MAX];

        for (auto const& char32 : u32string) {
            const auto converted_count = c32rtomb(buf, char32, &mbstate);
            if (converted_count == (size_t)-1)
                return "<Cannot convert the u32string to string>";
            result.append(buf, converted_count);
        }
        return result;
    #endif // _WIN32
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const std::u32string& value) {
        return std::vector<std::string>({"(std::u32string)\"" + _to_string(value) + '"'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(char32_t* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(char32_t*)nullptr"
            : "(char32_t*)\"" + _to_string(std::u32string(value)) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const char32_t* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(const char32_t*)nullptr"
            : "(const char32_t*)\"" + _to_string(std::u32string(value)) + '"'
        });
    }

    #ifdef __cpp_char8_t
    /// Convert std::u8string to std::string.
    /// @param u8string the std::u8string
    std::string _to_string(const std::u8string& u8string) {
    #ifdef _WIN32
        if (_code_page == CP_UTF8)
            return std::string((const char *)u8string.c_str());

        const auto u8stringLen = (int)u8string.length();
        const auto wstringLen = MultiByteToWideChar(CP_UTF8, 0, (const char *)u8string.c_str(), u8stringLen, nullptr, 0);
        std::wstring wstring(wstringLen, '\0');
        MultiByteToWideChar(CP_UTF8, 0, (const char *)u8string.c_str(), u8stringLen, &wstring[0], wstringLen);

        const auto stringLen = WideCharToMultiByte(_code_page, 0, wstring.c_str(), wstringLen, nullptr, 0, 0, 0);
        std::string string(stringLen, '\0');
        WideCharToMultiByte(_code_page, 0, wstring.c_str(), wstringLen, &string[0], stringLen, 0, 0);
        return string;
    #else
        return std::string((const char *)u8string.c_str());
    #endif // _WIN32
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const std::u8string& value) {
        return std::vector<std::string>({"(std::u8string)\"" + _to_string(value) + '"'});
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(char8_t* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(char8_t*)nullptr"
            : "(char8_t*)\"" + _to_string((std::u8string(value))) + '"'
        });
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    std::vector<std::string> to_strings(const char8_t* const& value) {
        return std::vector<std::string>({value == nullptr
            ? "(const char8_t*)nullptr"
            : "(const char8_t*)\"" + _to_string((std::u8string(value))) + '"'
        });
    }
    #endif // __cpp_char8_t


    template <typename T>
    std::vector<std::string> to_strings(T const& value);

    template <typename T>
    std::vector<std::string> to_strings(T const* pointer);

    template <typename T1, typename T2>
    std::vector<std::string> to_strings(const std::pair<T1, T2>& value);

    template <typename T, size_t N>
    std::vector<std::string> to_strings(const std::array<T, N>& container);

    template <typename T, class Allocator = std::allocator<T>>
    std::vector<std::string> to_strings(const std::deque<T, Allocator>& container);

    template <typename T, class Allocator = std::allocator<T>>
    std::vector<std::string> to_strings(const std::list<T, Allocator>& container);

    template <
        typename Key,
        typename T,
        class Compare = std::less<Key>,
        class Allocator = std::allocator<std::pair<Key const, T>>
    >
    std::vector<std::string> to_strings(const std::map<Key, T, Compare, Allocator>& container);

    template <
        typename Key,
        typename T,
        class Compare = std::less<Key>,
        class Allocator = std::allocator<std::pair<Key const, T>>
    >
    std::vector<std::string> to_strings(const std::multimap<Key, T, Compare, Allocator>& container);

    template <
        typename Key,
        typename T,
        class Hash = std::hash<Key>,
        class Pred = std::equal_to<Key>,
        class Allocator = std::allocator<std::pair<Key const, T>>
    >
    std::vector<std::string> to_strings(const std::unordered_map<Key, T, Hash, Pred, Allocator>& container);

    template <
        typename Key,
        typename T,
        class Hash = std::hash<Key>,
        class Pred = std::equal_to<Key>,
        class Allocator = std::allocator<std::pair<Key const, T>>
    >
    std::vector<std::string> to_strings(const std::unordered_multimap<Key, T, Hash, Pred, Allocator>& container);

    template <
        typename Key,
        class Compare = std::less<Key>,
        class Allocator = std::allocator<Key>
    >
    std::vector<std::string> to_strings(const std::set<Key, Compare, Allocator>& container);

    template <
        typename Key,
        class Compare = std::less<Key>,
        class Allocator = std::allocator<Key>
    >
    std::vector<std::string> to_strings(const std::multiset<Key, Compare, Allocator>& container);

    template <
        typename Key,
        class Hash = std::hash<Key>,
        class Pred = std::equal_to<Key>,
        class Allocator = std::allocator<Key>
    >
    std::vector<std::string> to_strings(const std::unordered_set<Key, Hash, Pred, Allocator>& container);

    template <
        typename Key,
        class Hash = std::hash<Key>,
        class Pred = std::equal_to<Key>,
        class Allocator = std::allocator<Key>
    >
    std::vector<std::string> to_strings(const std::unordered_multiset<Key, Hash, Pred, Allocator>& container);

    template <typename T, class Allocator = std::allocator<T>>
    std::vector<std::string> to_strings(const std::vector<T, Allocator>& container);

    template <class C>
    std::vector<std::string> _to_strings_container(C const& container);


    /// Returns a string representation of the value.
    /// @param value the value to output
    template <typename T>
    std::vector<std::string> to_strings(T const& value)
            requires has_to_string<T> {
        std::string str;
        str += _get_type_string(value);
        using std::to_string;
        str += to_string(value);
        return { str };
    }

    /// Returns a string representation of the value.
    /// @param pointer the pointer of the value to output
    template <typename T>
    std::vector<std::string> to_strings(T* pointer)
            requires has_to_string<T> {
        std::string str;
        str += _get_type_string(pointer);
        if (pointer == nullptr)
            str += "nullptr";
        else {
            str += "&";
            using std::to_string;
            str += to_string(*pointer);
        }
        return { str };
    }

    /// Returns a string representation of the value.
    /// @param value the value to output
    template <typename T1, typename T2>
    std::vector<std::string> to_strings(const std::pair<T1, T2>& value) {
        auto first_strings = to_strings(value.first);
        auto second_strings = to_strings(value.second);

        first_strings.back() += pair_separator;

        auto index = 0;
        for (auto const& second_string : second_strings) {
            if (index == 0) {
                if (first_strings.back().size() + second_string.size() <= maximum_data_output_width)
                    // one line
                    first_strings.back() += second_string;
                else
                    first_strings.push_back(_get_data_indent_string() + second_string);
            } else {
                first_strings.push_back(second_string);
            }
            ++index;
        }

        return first_strings;
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename T, size_t N>
    std::vector<std::string> to_strings(const std::array<T, N>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename T, class Allocator>
    std::vector<std::string> to_strings(const std::deque<T, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename T, class Allocator>
    std::vector<std::string> to_strings(const std::list<T, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, typename T, class Compare, class Allocator>
    std::vector<std::string> to_strings(const std::map<Key, T, Compare, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, typename T, class Compare, class Allocator>
    std::vector<std::string> to_strings(const std::multimap<Key, T, Compare, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, typename T, class Hash, class Pred, class Allocator>
    std::vector<std::string> to_strings(const std::unordered_map<Key, T, Hash, Pred, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, typename T, class Hash, class Pred, class Allocator>
    std::vector<std::string> to_strings(const std::unordered_multimap<Key, T, Hash, Pred, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, class Compare, class Allocator>
    std::vector<std::string> to_strings(const std::set<Key, Compare, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, class Compare, class Allocator>
    std::vector<std::string> to_strings(const std::multiset<Key, Compare, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, class Hash, class Pred, class Allocator>
    std::vector<std::string> to_strings(const std::unordered_set<Key, Hash, Pred, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename Key, class Hash, class Pred, class Allocator>
    std::vector<std::string> to_strings(const std::unordered_multiset<Key, Hash, Pred, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <typename T, class Allocator>
    std::vector<std::string> to_strings(const std::vector<T, Allocator>& container) {
        return _to_strings_container(container);
    }

    /// Returns a string representation of the container object.
    /// @param container the container object to output
    template <class C>
    std::vector<std::string> _to_strings_container(C const& container) {
        std::vector<std::string> strings;

        auto one_line = true;

        const auto type_str = _get_type_string(container, container.size());
        auto string = type_str;
        string += open_string;
        auto delimiter = "";
        auto count = (size_t)1;
        for (auto const& value : container) {
            string += delimiter;
            if (count > collection_limit) {
                string += limit_string;
                break;
            }

            const auto value_strings = to_strings(value);
            if (value_strings.size() > 1) {
                // the value string is not one line
                one_line = false;
                break;
            }
            string += value_strings.front();

            if (string.size() > maximum_data_output_width) {
                // multi lines
                one_line = false;
                break;
            }

            delimiter = ", ";
            count += 1;
        }

        if (one_line) {
            // strings is one line
            string += close_string;
            strings.push_back(string);

        } else {
            // strings is not one line
            strings.push_back(type_str + open_string);
            _data_nest_level += 1;
            const auto indent_string = _get_data_indent_string();
            count = 1;
            for (auto const& value : container) {
                if (count > collection_limit) {
                    strings.push_back(indent_string + limit_string);
                    break;
                }

                const auto value_strings = to_strings(value);
                auto is_first = true;
                for (auto const& value_string : value_strings) {
                    if (is_first)
                        strings.push_back(indent_string + value_string);
                    else
                        strings.push_back(value_string);
                    is_first = false;
                }

                strings.back() += ',';
                count += 1;
            }

            _data_nest_level -= 1;
            strings.push_back(_get_data_indent_string() + close_string);
        }

        return strings;
    }

    void print_message(const std::string message, char const* file_name, std::uint_least32_t line = 0) noexcept {
        auto log_str = _get_log_datetime();
        log_str += ' ';
        log_str += _get_code_indent_string();
        log_str += message;

        if (file_name[0] != '\0') {
            const char* file_name2 = std::strrchr(file_name, '/');
            if (file_name2 == nullptr) {
                file_name2 = std::strrchr(file_name, '\\');
                if (file_name2 == nullptr)
                    file_name2 = file_name;
                else
                    ++file_name2;
            } else
                ++file_name2;

            log_str += " (";
            log_str += file_name2;
            if (line > 0) {
                log_str += pair_separator;
                log_str += std::to_string(line);
            }
            log_str += ')';
        }

        if (std::strcmp(output_target, "std::cout") == 0) {
            std::cout << log_str << std::endl;
        } else if (std::strcmp(output_target, "std::cerr") == 0) {
            std::cerr << log_str << std::endl;
        } else {
            auto file_path = output_target;
            auto openmode = std::ios_base::app;
            if (std::strncmp(file_path, "+", 1) == 0) {
                ++file_path;
            } else if (!_initialized) {
                openmode = std::ios_base::trunc;
            }
            auto output_stream = std::ofstream(file_path, openmode);
            if (output_stream) {
                output_stream << log_str << std::endl;
                output_stream.close();
            } else {
                auto error_message = std::string("Can't open the debugtrace::output_target (") + output_target + ").";
                output_target = "std::cerr";
                print_message(log_str, "", 0);
                print_message(error_message, "", 0);
            }
        }
    }

    inline void print_message(const std::string message, const std::source_location location = std::source_location::current()) noexcept {
        print_message(message, location.file_name(), location.line());
    }

    template <typename T>
    void print(char const* name, T const& value, char const* file_name, std::uint_least32_t line) noexcept {
        _data_nest_level = 0;
        const auto value_strings = to_strings(value);

        auto index = 0;
        for (auto const& value_string : value_strings) {
            print_message(index == 0
                ? std::string(name) + varname_value_separator + value_string
                : value_string, file_name, line);
            ++index;
        }
    }

    template <typename T>
    inline void print(char const* name, T const& value, const std::source_location location = std::source_location::current()) noexcept {
        print(name, value, location.file_name(), location.line());
    }

    void _initialize() noexcept {
        if (!_initialized) {
            print_message(_start_message, "", 0);
            _initialized = true;
            print_message(std::string("Output: ") + output_target, "", 0);
            print_message("", "", 0);
        }
    }

    /// A utility class for debugging.
    /// Call DebugTrace.enter and DebugTrace.leave methods when enter and leave your methods,
    /// then outputs execution trace of the program.
    class Enter {
        std::source_location _location;

    public:
        Enter() = delete;
        Enter(Enter const&) = delete;
        Enter& operator =(const Enter&) = delete;

        /// Outputs a message when entering the function.
        /// @param func_name the function name
        /// @param file_name the source file name of the code that called this constructor
        /// @param line the line number of the code that called this function
        Enter(const std::source_location location = std::source_location::current()) noexcept {
            _initialize();
            _location = location;

            if (_before_code_nest_level > _code_nest_level)
                print_message("", "", 0);

            print_message(std::string(enter_string) + _location.function_name(),
                _location.file_name(), _location.line());

            _before_code_nest_level = _code_nest_level;
            ++_code_nest_level;
        }

        /// Outputs a message when leaving the function
        ~Enter() noexcept {
            _before_code_nest_level = _code_nest_level;
            --_code_nest_level;

            print_message(std::string(leave_string) + _location.function_name(),
                _location.file_name());
        }
    };

} // namespace debugtrace
