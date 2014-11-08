#ifndef STATUS_HPP
#define STATUS_HPP

#include "Program.hpp"
#include <memory>

struct Status {
	Program* program = nullptr;

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
	return lhs.program == rhs.program;
}
inline
bool operator!=(const Status& lhs, const Status& rhs)
{
	return !(lhs == rhs);
}

namespace std {
template<>
struct hash<Status> {
	size_t operator()(const Status& status) const
	{
		return hash<decltype(status.program)>{}(status.program);
	}
};

}

#endif /* STATUS_HPP */
