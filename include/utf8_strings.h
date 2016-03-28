#pragma once

namespace litehtml
{
	class utf8_to_wchar
	{
		const byte* m_utf8;
		twstring m_str;
	public:
        utf8_to_wchar(const char* val);

        void acquire_str(twstring& str)
        {
            m_str.swap(str);
            m_str.clear();
        }

	private:
		ucode_t getb()
		{
			if (!(*m_utf8)) return 0;
			return *m_utf8++;
		}
		ucode_t get_next_utf8(ucode_t val)
		{
			return (val & 0x3f);
		}
		ucode_t get_char();
	};

	class wchar_to_utf8
	{
		tstring m_str;
	public:
		wchar_to_utf8(const wchar_t* val);

        void acquire_str(tstring& str)
        {
            m_str.swap(str);
            m_str.clear();
        }
	};
}