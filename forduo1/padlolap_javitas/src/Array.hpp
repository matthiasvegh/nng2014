#ifndef ARRAY_H_
#define ARRAY_H_

#include "Point.hpp"
#include "PointRange.hpp"
#include "Hash.hpp"
#include <assert.h>
#include <vector>
#include <boost/range/algorithm.hpp>
#include <type_traits>

//#define NDEBUG
//
//#define assert assert_
//
//inline void assert_(bool expr) {
//	if (!expr) {
//		std::cerr << "bing" << std::endl;
//		exit(1);
//	}
//}

template<typename T>
class Array {
	std::size_t width_, height_;
	std::vector<T> data_;
public:
	typedef T valueType;
	typedef typename std::vector<T>::reference reference;
	typedef typename std::vector<T>::const_reference const_reference;

	Array(): width_(0), height_(0) {}
	Array(std::size_t width, std::size_t height, const T& defValue = T()):
		width_(width), height_(height), data_(width * height, defValue)
	{}
	Array(const Array& ) = default;
	Array(Array&& ) = default;
	Array& operator=(const Array& ) = default;
	Array& operator=(Array&& ) = default;

	reference operator[](std::size_t pos) {
		return data_[pos];
	}
	const_reference operator[](std::size_t pos) const {
		assert(pos >= 0 && pos < size());
		return data_[pos];
	}
	reference operator[](Point p) {
		assert(p.x >= 0 && p.y >= 0
				&& p.x < static_cast<int>(width_) &&
				p.y < static_cast<int>(height_));
		return data_[p.y*width_ + p.x];
	}
	const_reference operator[](Point p) const {
		assert(p.x >= 0 && p.y >= 0
				&& p.x < static_cast<int>(width_) &&
				p.y < static_cast<int>(height_));
		return data_[p.y*width_ + p.x];
	}
	std::size_t size() const { return data_.size(); }
	std::size_t width() const { return width_; }
	std::size_t height() const { return height_; }
	void reset(std::size_t newWidth, std::size_t newHeight, const T& defValue = T())
	{
		width_ = newWidth;
		height_ = newHeight;
		data_.resize(width_ * height_);
		fill(defValue);
	}
	void fill(const T &value)
	{
		boost::fill(data_, value);
	}
	void clear()
	{
		data_.clear();
		width_ = 0;
		height_ = 0;
	}

	bool operator==(const Array<T>& other) const
	{
		return data_ == other.data_;
	}
};

template<typename T>
inline const typename Array<T>::const_reference arrayAt(const Array<T> &arr, Point p, const T& def)
{
	if (p.x >= 0 && p.y >= 0 && p.x < static_cast<int>(arr.width()) && p.y < static_cast<int>(arr.height()))
			return arr[p];
		return def;
}

template <typename Array>
inline PointRange arrayRange(const Array& array)
{
	return PointRange(Point(0,0), Point(array.width(), array.height()));
}

template<typename T>
inline bool operator!=(const Array<T>& lhs, const Array<T>& rhs)
{
	return !(lhs == rhs);
}

namespace std {
template<typename T>
struct hash<Array<T>> {
	size_t operator()(const Array<T>& arr) const
	{
		size_t seed = 0;
		for (size_t i = 0; i < arr.size(); ++i) {
			hash_combine(seed, arr[i]);
		}
		return seed;
	}
};

}

#endif /* ARRAY_H_ */
