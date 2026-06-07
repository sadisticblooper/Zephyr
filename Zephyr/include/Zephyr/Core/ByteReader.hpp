#pragma once
#include <cstdint>
#include <cstddef>
#include <string>

namespace zephyr::core
{
	class ByteReader
	{
	public:
		ByteReader(const uint8_t* data, size_t size) : data_(data), size_(size) {}

		uint8_t ReadU8();
		uint32_t ReadU32();
		uint64_t ReadU64();
		int32_t ReadI32();
		int64_t ReadI64();

		void ReadBytes(void* dst, size_t n);

		std::string ReadFString();

		void Seek(size_t pos);
		size_t Tell() const { return pos_; }
		size_t Remaining() const { return ok_ && pos_ <= size_ ? size_ - pos_ : 0; }
		bool Ok() const { return ok_; }
		const uint8_t* cur() const { return data_ + pos_; }

	private:
		bool CanRead(size_t n) const { return ok_ && pos_ + n <= size_; }
		void Fail() { ok_ = false; }

		const uint8_t* data_ = nullptr;
		size_t size_ = 0;
		size_t pos_ = 0;
		bool ok_ = true;
	};
}