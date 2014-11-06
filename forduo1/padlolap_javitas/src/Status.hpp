#ifndef STATUS_HPP
#define STATUS_HPP

#include "Array.hpp"
#include <memory>

using Value = int;
struct Status {
	Array<int> field;

	Status() = default;
	Status(const Status&) = default;
	Status& operator=(const Status&) = default;
	Status(Status&&) = default;
	Status& operator=(Status&&) = default;

	using Ptr = std::shared_ptr<Status>;
	using ConstPtr = std::shared_ptr<const Status>;
};

inline
bool operator==(const Status& lhs, const Status& rhs)
{
	return lhs.field == rhs.field;
}
inline
bool operator!=(const Status& lhs, const Status& rhs)
{
	return !(lhs == rhs);
}

inline Value getValue(const Status& status, Point p)
{
	return arrayAt(status.field, p, -1);
}

namespace std {
template<>
struct hash<Status> {
	size_t operator()(const Status& status) const
	{
		return hash<decltype(status.field)>{}(status.field);
	}
};

}

#endif /* STATUS_HPP */
