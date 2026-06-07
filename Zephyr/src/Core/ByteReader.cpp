#include "../../include/Zephyr/Core/ByteReader.hpp"
#include <cstring>

namespace zephyr::core
{
	uint8_t ByteReader::ReadU8() 
	{
		if (!CanRead(1))
		{
			Fail();
			return 0;
		}
		return data_[pos_++];
	}

	uint32_t ByteReader::ReadU32()
	{
		if (!CanRead(4))
		{
			Fail();
			return 0;
		}

		uint32_t v;
		std::memcpy(&v, data_ + pos_, 8);
		pos_ += 8;
		return v;
	}

	uint64_t ByteReader::ReadU64() 
	{
		if (!CanRead(8)) 
		{ 
			Fail(); 
			return 0;
		}

		uint64_t v;
		std::memcpy(&v, data_ + pos_, 8);
		pos_ += 8;
		return v;
	}

	int32_t ByteReader::ReadI32() 
	{
		uint32_t u = ReadU32();
		int32_t v;
		std::memcpy(&v, &u, 4);
		return v;
	}

	int64_t ByteReader::ReadI64() 
	{
		uint64_t u = ReadU64();
		int64_t v;
		std::memcpy(&v, &u, 8);
		return v;
	}

	void ByteReader::ReadBytes(void* dst, size_t n) 
	{
		if (!CanRead(n)) 
		{ 
			Fail(); 
			return; 
		}

		std::memcpy(dst, data_ + pos_, n);
		pos_ += n;
	}

	std::string ByteReader::ReadFString()
	{
		int32_t len = ReadI32();
		if (!ok_)
		{
			return {};
		}

		if (len == 0)
		{
			return {};
		}

		if (len > 0)
		{
			if (!CanRead(static_cast<size_t>(len)))
			{
				Fail();
				return {};
			}

			const char* p = reinterpret_cast<const char*>(data_ + pos_);
			size_t chars = static_cast<size_t>(len);

			if (chars > 0 && p[chars - 1] == '\0')
				chars--;
			std::string s(p, chars);
			pos_ += static_cast<size_t>(len);

			return s;
		}

		int64_t units = -static_cast<int64_t>(len);
		size_t bytes = static_cast<size_t>(units) * 2;
		
		if (!CanRead(bytes))
		{
			Fail();
			return {};
		}

		std::string out;
		out.reserve(static_cast<size_t>(units));

		const uint8_t* p = data_ + pos_;
		for (int64_t i = 0; i < units; i++)
		{
			uint32_t cp = static_cast<uint32_t>(p[i * 2] | (p[i * 2 + 1] << 8));
			
			if (cp == 0)
				break;

			if (cp >= 0xD800 && cp <= 0xDBFF && i + 1 < units)
			{
				uint32_t lo = static_cast<uint32_t>(p[(i + 1) * 2] | (p[(i + 1) * 2 + 1] << 8));
				if (lo >= 0xDC00 && lo <= 0xDFFF) {
					cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
					++i;
				}
			}

			if (cp < 0x80) {
				out.push_back(static_cast<char>(cp));
			}
			else if (cp < 0x800) {
				out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
				out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			}
			else if (cp < 0x10000) {
				out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
				out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			}
			else {
				out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
				out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			}
		}
		pos_ += bytes;
		return out;
	}

	void ByteReader::Seek(size_t pos) 
	{
		if (pos > size_) 
		{ 
			Fail(); 
			return; 
		}
		pos_ = pos;
	}
}
