#pragma once

#include <iostream>

typedef unsigned int TELEM;

class TBitField
{
private:
	int  bitlen;
	TELEM* pmem; 
	int  memlen; 

	int   GetMemIndex(const int n) const {
		return n >> 5;
	}

	TELEM GetMemMask(const int n) const {
		return 1 << (n % 32);
	}

public:
	TBitField(int len) {
		if (len <= 0) {
			bitlen = 0;
			pmem = nullptr;
			memlen = 0;
		}
		else {
			bitlen = len;
			memlen = (len + 31) / 32;
			pmem = new TELEM[memlen];
			for (int i = 0; i < memlen; ++i) {
				pmem[i] = 0;
			}
		}
	}

	TBitField(const TBitField& bf) {
		bitlen = bf.bitlen;
		memlen = bf.memlen;

		if (memlen > 0) {
			pmem = new TELEM[memlen];
			for (int i = 0; i < memlen; ++i) {
				pmem[i] = bf.pmem[i];
			}
		}
		else {
			pmem = nullptr;
		}
	}

	~TBitField() {
		delete[] pmem;
	}

	int GetLength() const {
		return bitlen;
	}
	void SetBit(const int n) {
		if (n >= 0 && n < bitlen) {
			int index = GetMemIndex(n);
			TELEM mask = GetMemMask(n);
			pmem[index] = pmem[index] | mask;
		}
	}
	void ClrBit(const int n) {
		if (n >= 0 && n < bitlen) {
			int index = GetMemIndex(n);
			TELEM mask = GetMemMask(n);
			
			if ((pmem[index] & mask) != 0) {
				pmem[index] = pmem[index] - mask;
			}
		}
	}
	int  GetBit(const int n) const {
		if (n >= 0 && n < bitlen) {
			int index = GetMemIndex(n);
			TELEM mask = GetMemMask(n);
			return (pmem[index] & mask) != 0;
		}
		return 0;
	}

	int operator==(const TBitField& bf) const {
		if (bitlen != bf.bitlen) {
			return 0;
		}

		for (int i = 0; i < memlen; i++) {
			if (pmem[i] != bf.pmem[i]) {
				return 0;
			}
		}

		return 1;
	}

	int operator!=(const TBitField& bf) const {
		return 1 - (*this == bf);
	}

	TBitField& operator=(const TBitField& bf) {
		if (this != &bf) {
			if (memlen != bf.memlen) {
				if (pmem != nullptr) {
					delete[] pmem;
				}
				memlen = bf.memlen;
				if (memlen > 0) {
					pmem = new TELEM[memlen];
				}
				else {
					pmem = nullptr;
				}
			}

			bitlen = bf.bitlen;
			for (int i = 0; i < memlen; i++) {
				pmem[i] = bf.pmem[i];
			}
		}
		return *this;
	}

	TBitField  operator|(const TBitField& bf) {
		int maxlen;
		if (bitlen > bf.bitlen) {
			maxlen = bitlen;
		}
		else {
			maxlen = bf.bitlen;
		}

		TBitField res(maxlen);

		int minmemlen;
		if (memlen > bf.memlen) {
			minmemlen = bf.memlen;
		}
		else {
			minmemlen = memlen;
		}

		for (int i = 0; i < minmemlen; i++) {
			res.pmem[i] = pmem[i] | bf.pmem[i];
		}

		if (memlen > bf.memlen) {
			for (int i = minmemlen; i < memlen; i++) {
				res.pmem[i] = pmem[i];
			}
		}
		else {
			for (int i = minmemlen; i < bf.memlen; i++) {
				res.pmem[i] = bf.pmem[i];
			}
		}
		return res;
	}

	TBitField  operator&(const TBitField& bf) {
		int maxlen;
		if (bitlen > bf.bitlen) {
			maxlen = bitlen;
		}
		else {
			maxlen = bf.bitlen;
		}

		TBitField res(maxlen);

		int minmemlen;
		if (memlen > bf.memlen) {
			minmemlen = bf.memlen;
		}
		else {
			minmemlen = memlen;
		}

		for (int i = 0; i < minmemlen; i++) {
			res.pmem[i] = pmem[i] & bf.pmem[i];
		}
		return res;
	}

	TBitField  operator~() {
		TBitField res(bitlen);
		for (int i = 0; i < bitlen; i++) {
			if (GetBit(i) == 0) {
				res.SetBit(i);
			}
		}
		return res;
	}

	friend std::istream& operator>>(std::istream& istr, TBitField& bf);
	friend std::ostream& operator<<(std::ostream& ostr, const TBitField& bf);
};


inline std::ostream& operator<<(std::ostream& stream, const TBitField& bf) {
	int len = bf.GetLength();

	for (int i = 0; i < len; i++) {
		if (bf.GetBit(i)) {
			stream << '1';
		}
		else {
			stream << '0';
		}
	}
	return stream;
}

inline std::istream& operator>>(std::istream& stream, TBitField& bf) {
	char elem;
	int len = bf.GetLength();
	for (int i = 0; i < len; i++) {
		bf.ClrBit(i);
	}

	for (int i = 0; i < len; i++) {
		stream >> elem;
		if (elem == '1') {
			bf.SetBit(i);
		}
		else if (elem == '0') {
			bf.ClrBit(i);
		}
		else {
			std::cout << "Input error\n";
			break;
		}
	}
	return stream;
}


