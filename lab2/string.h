#ifndef STRING_H
#define STRING_H

#include <algorithm>
#include <cstdint>
#include <cstring>

const uint8_t STORAGE_INCREASE_SIZE = 2;
const uint16_t STR_BUF_SIZE = 300;

class TString {
public:
	using value_type = char;
	using iterator = value_type *;
	using const_iterator = const value_type *;

	TString(): Size_(0), Capacity_(0), Storage_(nullptr)
	{
	}

	TString(const value_type* str) {
		Size_ = strlen(str);
		Capacity_ = Size_ * STORAGE_INCREASE_SIZE;
		Storage_ = new value_type[Capacity_];
		std::copy(str, str + Size_, Storage_);
		Storage_[Size_] = '\0';
	}

	TString(const TString& str) {
		Size_ = str.Size_;
		Capacity_ = Size_ * STORAGE_INCREASE_SIZE;
		Storage_ = new value_type[Capacity_];
		std::copy(str.Storage_, str.Storage_ + str.Size_, Storage_);
		Storage_[Size_] = '\0';
	}

	TString(TString&& str) noexcept {
		Size_ = str.Size_;
		Capacity_ = str.Capacity_;
		Storage_ = str.Storage_;
		str.Storage_ = nullptr;
	}

	TString& operator= (const TString& str) {
		delete[] Storage_;
		Size_ = str.Size_;
		Capacity_ = str.Capacity_;
		Storage_ = new value_type[Capacity_];
		std::copy(str.Storage_, str.Storage_ + Size_ + 1, Storage_);
		return *this;
	}

	TString& operator= (TString&& str) noexcept {
		delete[] Storage_;
		Size_ = str.Size_;
		Capacity_ = str.Capacity_;
		Storage_ = str.Storage_;
		str.Storage_ = nullptr;
		return *this;
	}

	TString& operator= (const value_type* str) {
		delete[] Storage_;
		Size_ = strlen(str);
		Capacity_ = Size_ * STORAGE_INCREASE_SIZE;
		Storage_ = new value_type[Capacity_];
		std::copy(str, str + Size_ + 1, Storage_);
		return *this;
	}

	void CStrMove(value_type* str) {
		delete[] Storage_;
		Storage_ = str;
		Size_ = strlen(str);
		Capacity_ = Size_ + 1;
	}

	void Swap(TString& str) {
		std::swap(Size_, str.Size_);
		std::swap(Capacity_, str.Capacity_);
		std::swap(Storage_, str.Storage_);
	}

	void PushBack(const value_type& ch) {
		if (Size_ + 1 == Capacity_) {
			int newCapacity = Capacity_ ? Capacity_ * STORAGE_INCREASE_SIZE : 1;

			value_type* newStorage = new value_type[newCapacity];
			std::copy(Storage_, Storage_ + Capacity_, newStorage);
			delete[] Storage_;
			Storage_ = newStorage;
			Capacity_ = newCapacity;
		}

		Storage_[Size_] = ch;
		++Size_;
		Storage_[Size_] = '\0';
	}

	~TString() {
		delete[] Storage_;
		Size_ = 0;
		Capacity_ = 0;
		Storage_ = nullptr;
	}

	iterator begin() noexcept {
		return Storage_;
	}

	const_iterator begin() const noexcept {
		return Storage_;
	}

	iterator end() noexcept {
		if (Storage_)
			return Storage_ + Size_;
		return nullptr;
	}

	const_iterator end() const noexcept {
		if (Storage_)
			return Storage_ + Size_;
		return nullptr;
	}

	const value_type * CStr() const {
		return Storage_;
	}

	short Size() const noexcept {
		return Size_;
	}

	TString& operator+= (const value_type& ch) {
		PushBack(ch);
		return *this;
	}

	const value_type& At(int index) const {
		if (index < 0 || index > Size_) {
			throw std::out_of_range("You are doing this wrong!");
		}

		return Storage_[index];
	}

	value_type& At(int index) {
		const value_type& elem = const_cast<const TString*>(this)->At(index);
		return const_cast<value_type&>(elem);
	}

	const value_type& operator[](int index) const {
		return At(index);
	}

	value_type& operator[](int index) {
		return At(index);
	}

	friend std::ostream& operator<< (std::ostream& os, const TString& str);
	friend std::istream& operator>> (std::istream& is, TString& str);

private:
	short Size_;
	short Capacity_;
	value_type *Storage_;
};

std::ostream& operator<< (std::ostream& os, const TString& str) {
	for (auto ch : str)
		os << ch;
	return os;
}

std::istream& operator>> (std::istream& is, TString& str) {
	char buf[STR_BUF_SIZE];
	if (is >> buf)
		str = buf;
	return is;
}

bool operator< (const TString& lhs, const TString& rhs) {
	int minSize = std::min(lhs.Size(), rhs.Size());
	for (int i = 0; i < minSize; ++i) {
		if (lhs[i] != rhs[i]) {
			return lhs[i] < rhs[i];
		}
	}
	return lhs.Size() < rhs.Size();
}

bool operator> (const TString& lhs, const TString& rhs) {
	return rhs < lhs;
}

bool operator<= (const TString& lhs, const TString& rhs) {
	return !(lhs > rhs);
}

bool operator>= (const TString& lhs, const TString& rhs) {
	return !(lhs < rhs);
}

bool operator== (const TString& lhs, const TString& rhs) {
	return !(lhs > rhs) && !(lhs < rhs);
}

bool operator!= (const TString& lhs, const TString& rhs) {
	return !(lhs == rhs);
}

#endif