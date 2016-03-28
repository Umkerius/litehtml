#pragma once

#include <cassert>
#include <numeric>
#include <string>

namespace litehtml
{

// Non-owning string class
template <typename CharT>
class string_view
{
public:
    using size_type = typename std::basic_string<CharT>::size_type;
    using const_iterator = const CharT*;
    static const size_type npos = -1;

    string_view() = default;
    string_view(const CharT* str, size_t size) : m_str(str), m_size(size) {}
    string_view(const CharT* str);

    template <typename Alloc>
    string_view(const std::basic_string<CharT, std::char_traits<CharT>, Alloc>& str) 
        : string_view(str.c_str(), str.size()) {}

    const_iterator begin() const { return m_str; }
    const_iterator end() const { return begin() + m_size; }

    CharT front() const { return c_str()[0]; }
    CharT back() const { return c_str()[size() - 1]; }

    const CharT* data() const { return begin(); }
    std::basic_string<CharT> to_string() const { return std::basic_string<CharT>(c_str(), size()); }

    size_type size() const { return m_size; }
    size_type length() const { return size(); }
    bool empty() const { return size() == 0; }
    void clear();

    string_view<CharT> substr(size_type offset, size_type len = npos) const;
    void remove_prefix(size_type n);
    void remove_suffix(size_type n);

    size_type find(string_view<CharT> str, size_type pos = 0) const;
    size_type find(const CharT* str, size_type pos = 0) const;
    size_type find(const CharT* str, size_type pos, size_type len) const;
    size_type find(CharT ch, size_type pos = 0) const;
    size_type find_first_of(string_view<CharT> str, size_type pos = 0) const;
    size_type find_first_of(const CharT* str, size_type pos = 0) const;
    size_type find_first_of(const CharT* str, size_type pos, size_type len) const;
    size_type find_first_of(CharT ch, size_type pos = 0) const;
    size_type find_last_of(string_view<CharT> str, size_type pos = npos) const;
    size_type find_last_of(const CharT* str, size_type pos = npos) const;
    size_type find_last_of(const CharT* str, size_type pos, size_type len) const;
    size_type find_last_of(CharT ch, size_type pos = npos) const;
    size_type find_first_not_of(string_view<CharT> str, size_type pos = 0) const;
    size_type find_first_not_of(const CharT* str, size_type pos = 0) const;
    size_type find_first_not_of(const CharT* str, size_type pos, size_type len) const;
    size_type find_first_not_of(CharT ch, size_type pos = 0) const;
    size_type find_last_not_of(string_view<CharT> str, size_type pos = npos) const;
    size_type find_last_not_of(const CharT* str, size_type pos = npos) const;
    size_type find_last_not_of(const CharT* str, size_type pos, size_type len) const;
    size_type find_last_not_of(CharT ch, size_type pos = npos) const;

    CharT operator[](size_type pos) const { return c_str()[pos]; }
    CharT at(size_type pos) const;

    int compare(string_view<CharT> another);

private:
    //c_str is not recommended for use
    //it usually used for call C functions which expects null-terminated string
    //you cant be sure that string_view::c_str returns pointer to null-terminated string
    //if you need an access to internal string, you should use data or begin
    //they do the same as c_str, but if you use it consciously, you are already warned
    const CharT* c_str() const { return begin(); }

    const CharT* m_str = nullptr;
    size_type m_size = 0;
};

//--------------------------------------------------------------------------------------------------
// Free functions 
//--------------------------------------------------------------------------------------------------
template <typename CharT>
basic_lite_string<CharT> to_lite_string(string_view<CharT> str)
{
    return basic_lite_string<CharT>(str.data(), str.size());
}

//--------------------------------------------------------------------------------------------------
// string_view implementation 
//--------------------------------------------------------------------------------------------------
template <typename CharT>
string_view<CharT>::string_view(const CharT* str) 
    : string_view(str, str != nullptr ? std::char_traits<CharT>::length(str) : 0) {}

template <typename CharT>
void string_view<CharT>::clear()
{
    m_str = nullptr;
    m_size = 0;
}

template <typename CharT>
string_view<CharT> string_view<CharT>::substr(size_type offset, size_type len) const
{
    size_type newLen = std::min(size() - offset, len);

    assert(offset + newLen <= size());
    return string_view<CharT>(c_str() + offset, newLen);
}

template <typename CharT>
void string_view<CharT>::remove_prefix(size_type n)
{
    m_str += n;
    m_size -= n;
}

template <typename CharT>
void string_view<CharT>::remove_suffix(size_type n)
{
    m_size -= n;
}

template <typename CharT>
typename string_view<CharT>::size_type 
string_view<CharT>::find(string_view<CharT> str, size_type pos) const
{
    return find(str.c_str(), pos, str.size());
}

template <typename CharT>
typename string_view<CharT>::size_type 
string_view<CharT>::find(const CharT* str, size_type pos) const
{
    size_type str_size = std::char_traits<CharT>::length(str);
    return find(str, pos, str_size);
}

template <typename CharT>
typename string_view<CharT>::size_type 
string_view<CharT>::find(const CharT* str, size_type pos, size_type len) const
{
    using traits = std::char_traits<CharT>;

    if (len == 0 && pos <= size())
        return pos;	// null string always matches (if inside string)

    size_type nm;
    if (pos < size() && len <= (nm = size() - pos))
    {	// room for match, look for it
        const CharT *u_ptr, *v_ptr;
        for (nm -= len - 1, v_ptr = c_str() + pos;
            (u_ptr = traits::find(v_ptr, nm, *str)) != 0;
            nm -= u_ptr - v_ptr + 1, v_ptr = u_ptr + 1)
        {
            if (traits::compare(u_ptr, str, len) == 0)
            {
                return (u_ptr - c_str());	// found a match
            }
        }
    }

    return npos; // no match
}

template <typename CharT>
typename string_view<CharT>::size_type 
string_view<CharT>::find(CharT ch, size_type pos) const
{
    return find(&ch, pos, 1);
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_first_of(string_view<CharT> str, size_type pos) const
{
    return find_first_of(str.c_str(), pos, str.size());
}

template <typename CharT>
typename string_view<CharT>::size_type 
string_view<CharT>::find_first_of(const CharT* str, size_type pos) const
{
    size_type str_size = std::char_traits<CharT>::length(str);
    return find_first_of(str, pos, str_size);
}

template <typename CharT>
typename string_view<CharT>::size_type 
string_view<CharT>::find_first_of(const CharT* str, size_type pos, size_type len) const
{
    using traits = std::char_traits<CharT>;

    if (0 < len && pos < size())
    {	// room for match, look for it
        const CharT *const v_ptr = c_str() + size();
        for (const CharT *u_ptr = c_str() + pos; u_ptr < v_ptr; ++u_ptr)
        {
            if (traits::find(str, len, *u_ptr) != 0)
            {
                return (u_ptr - c_str());	// found a match
            }
        }
    }

    return npos; // no match
}

template <typename CharT>
typename string_view<CharT>::size_type 
string_view<CharT>::find_first_of(CharT ch, size_type pos) const
{
    return find_first_of(&ch, pos, 1);
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_of(string_view<CharT> str, size_type pos) const
{
    return find_last_of(str.c_str(), pos, str.size());
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_of(const CharT* str, size_type pos) const
{
    size_type str_size = std::char_traits<CharT>::length(str);
    return find_last_of(str, pos, str_size);
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_of(const CharT* str, size_type pos, size_type len) const
{
    using traits = std::char_traits<CharT>;

    if (0 < len && 0 < size())
    {	// worth searching, do it
        const CharT *u_ptr = c_str() + (pos < size() ? pos : size() - 1);
        for (;; --u_ptr)
        {
            if (traits::find(str, len, *u_ptr) != 0)
            {
                return (u_ptr - c_str());	// found a match
            }
            else if (u_ptr == c_str())
            {
                break;	// at beginning, no more chance for match
            }
        }
    }

    return npos;	// no match
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_of(CharT ch, size_type pos) const
{
    return find_last_of(&ch, pos, 1);
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_first_not_of(string_view<CharT> str, size_type pos) const
{
    return find_first_not_of(str.c_str(), pos, str.size());
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_first_not_of(const CharT* str, size_type pos) const
{
    size_type str_size = std::char_traits<CharT>::length(str);
    return find_first_not_of(str, pos, str_size);
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_first_not_of(const CharT* str, size_type pos, size_type len) const
{
    using traits = std::char_traits<CharT>;

    if (pos < size())
    {	// room for match, look for it
        const CharT* const v_ptr = c_str() + size();
        for (const CharT* u_ptr = c_str() + pos; u_ptr < v_ptr; ++u_ptr)
        {
            if (traits::find(str, len, *u_ptr) == 0)
            {
                return (u_ptr - c_str());
            }
        }
    }
    return npos;
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_first_not_of(CharT ch, size_type pos) const
{
    return find_first_not_of(&ch, pos, 1);
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_not_of(string_view<CharT> str, size_type pos) const
{
    return find_last_not_of(str.c_str(), pos, str.size());
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_not_of(const CharT* str, size_type pos) const
{
    size_type str_size = std::char_traits<CharT>::length(str);
    return find_last_not_of(str, pos, str_size);
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_not_of(const CharT* str, size_type pos, size_type len) const
{
    using traits = std::char_traits<CharT>;

    if (0 < size())
    {	// worth searching, do it
        const CharT *u_ptr = c_str() + (pos < size() ? pos : size() - 1);
        for (;; --u_ptr)
        {
            if (traits::find(str, len, *u_ptr) == 0)
            {
                return (u_ptr - c_str());
            }
            else if (u_ptr == c_str())
            {
                break;
            }
        }
    }
    return npos;
}

template <typename CharT>
typename string_view<CharT>::size_type
string_view<CharT>::find_last_not_of(CharT ch, size_type pos) const
{
    return find_last_not_of(&ch, pos, 1);
}

template <typename CharT>
CharT string_view<CharT>::at(size_type pos) const
{
    assert(pos < size()); //performance is important, so just assert
    return c_str()[pos];
}

template <typename CharT>
int string_view<CharT>::compare(string_view<CharT> another)
{
    //check if this string is shorter or longer
    if (size() < another.size())
        return -1;
    else if (size() > another.size())
        return 1;

    //check if pointers are equal (sizes already equal)
    if (c_str() == another.c_str())
        return 0;

    // strings have the same size
    return std::char_traits<CharT>::compare(data(), another.data(), size());
}

//--------------------------------------------------------------------------------------------------
// Operators 
//--------------------------------------------------------------------------------------------------

// string_view equality
template <typename CharT>
bool operator==(string_view<CharT> lhs, string_view<CharT> rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename CharT>
bool operator==(std::basic_string<CharT> lhs, string_view<CharT> rhs)
{
    return string_view<CharT>(lhs) == rhs;
}

template <typename CharT>
bool operator==(string_view<CharT> lhs, std::basic_string<CharT> rhs)
{
    return lhs == string_view<CharT>(rhs);
}

template <typename CharT>
bool operator==(const CharT* lhs, string_view<CharT> rhs)
{
    return string_view<CharT>(lhs) == rhs;
}

template <typename CharT>
bool operator==(string_view<CharT> lhs, const CharT* rhs)
{
    return lhs == string_view<CharT>(rhs);
}

// string_view inequality
template <typename CharT>
bool operator!=(string_view<CharT> lhs, string_view<CharT> rhs)
{
    return !(lhs == rhs);
}

template <typename CharT>
bool operator!=(std::basic_string<CharT> lhs, string_view<CharT> rhs)
{
    return !(lhs == rhs);
}

template <typename CharT>
bool operator!=(string_view<CharT> lhs, std::basic_string<CharT> rhs)
{
    return !(lhs == rhs);
}

template <typename CharT>
bool operator!=(const CharT* lhs, string_view<CharT> rhs)
{
    return !(lhs == rhs);
}

template <typename CharT>
bool operator!=(string_view<CharT> lhs, const CharT* rhs)
{
    return !(lhs == rhs);
}

// string_view comparison
template <typename CharT>
bool operator<(string_view<CharT> lhs, string_view<CharT> rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename CharT>
bool operator>(string_view<CharT> lhs, string_view<CharT> rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename CharT>
std::basic_string<CharT> operator+(string_view<CharT> lhs, string_view<CharT> rhs)
{
    std::basic_string<CharT> result;
    result.reserve(lhs.size() + rhs.size());

    result.append(lhs.data(), lhs.size());
    result.append(rhs.data(), rhs.size());

    return result;
}

template <typename CharT>
std::basic_string<CharT> operator+(std::basic_string<CharT> lhs, string_view<CharT> rhs)
{
    return string_view<CharT>(lhs) + rhs;
}

template <typename CharT>
std::basic_string<CharT> operator+(string_view<CharT> lhs, const CharT* rhs)
{
    return lhs + string_view<CharT>(rhs);
}

} // namespace litehtml