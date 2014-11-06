#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "Status.hpp"
#include <unordered_map>

template <typename Ptr, typename Functor>
class Dereferencer1 {
	Functor functor;
public:
	Dereferencer1(Functor functor = Functor{}):
		functor(functor)
	{}
	auto operator()(const Ptr& arg) const -> decltype(functor(*arg))
	{
		return functor(*arg);
	}
};

template <typename Ptr, typename Functor>
class Dereferencer2 {
	Functor functor;
public:
	Dereferencer2(Functor functor = Functor{}):
		functor(functor)
	{}
	auto operator()(const Ptr& arg1, const Ptr& arg2) const ->
		decltype(functor(*arg1, *arg2))
	{
		return functor(*arg1, *arg2);
	}
};

class VisitedStates {
	typedef std::unordered_map<Status::ConstPtr, int,
			Dereferencer1<Status::ConstPtr, std::hash<Status>>,
			Dereferencer2<Status::ConstPtr, std::equal_to<Status>>
		> VisitedStateSet;
	VisitedStateSet visitedStates;
public:
	VisitedStates() = default;
	bool checkAndPush(const Status::ConstPtr &elem, int heur);
	std::size_t size() const
	{
		return visitedStates.size();
	}
};

#endif /* VISITEDSTATES_H_ */
