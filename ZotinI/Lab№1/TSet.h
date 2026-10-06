#pragma once

#include <iostream>
#include "Tbitfield.h"

class TSet {
private:
	int maxpower;
	TBitField bitfield;
public:
	TSet(int mp) : maxpower(mp), bitfield(mp) {}
	TSet(const TSet& s) : maxpower(s.maxpower), bitfield(s.bitfield) {}
	TSet(const TBitField& bf) : maxpower(bf.GetLength()), bitfield(bf) {}

	operator TBitField() {
		return bitfield;
	}

	int GetMaxPower() const {
		return maxpower;
	}

	void InsElem(const int Elem) {
		if (Elem >= 0 && Elem < maxpower) {
			bitfield.SetBit(Elem);
		}
	}

	void DelElem(const int Elem) {
		if (Elem >= 0 && Elem < maxpower) {
			bitfield.ClrBit(Elem);
		}
	}
	int IsMember(const int Elem) const {
		if (Elem >= 0 && Elem < maxpower) {
			return bitfield.GetBit(Elem);
		}
		return 0;
	}

	int operator== (const TSet& s) const {
		return bitfield == s.bitfield;
	}

	int operator!= (const TSet& s) const {
		return bitfield != s.bitfield;
	}
	TSet& operator=(const TSet& s) {
		if (this != &s) {
			maxpower = s.maxpower;
			bitfield = s.bitfield;
		}
		return *this;
	}

	TSet operator+ (const int Elem) {
		TSet s(*this);
		s.InsElem(Elem);
		return s;
	}

	TSet operator- (const int Elem) {
		TSet s(*this);
		s.DelElem(Elem);
		return s;
	}
	TSet operator+ (const TSet& s) {
		return TSet(bitfield | s.bitfield);
	}
	TSet operator* (const TSet& s) {
		return TSet(bitfield & s.bitfield);
	}
	TSet operator~ () {
		return TSet(~bitfield);
	}

	friend std::istream& operator>>(std::istream& istr, TSet& bf);
	friend std::ostream& operator<<(std::ostream& ostr, const TSet& bf);
};



inline std::ostream& operator<<(std::ostream& stream, const TSet& s) {
	for (int i = 0; i < s.GetMaxPower(); i++) {
		if (s.IsMember(i)) {
			stream << i << ' ';
		}
	}
	stream << '\n';
	return stream;
}

inline std::istream& operator>>(std::istream& stream, TSet& s) {
	int elem;

	for (int i = 0; i < s.GetMaxPower(); i++) {
		s.DelElem(i);
	}

	while (true) {
		stream >> elem;
		if (stream.fail()) {
			std::cout << "Input error\n";
			stream.clear();
			stream.ignore(10000, '\n');
			continue;
		}

		if (elem < 0) {
			break;
		}
		s.InsElem(elem);
	}

	return stream;
}