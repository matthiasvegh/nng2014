#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

#include <memory>
#include <boost/noncopyable.hpp>

class Status;

class HeurCalculator: public boost::noncopyable {
public:
	typedef std::shared_ptr<const HeurCalculator> Ptr;
	int calculateStatus(const Status& status);
};

#endif /* HEURCALCULATOR_H_ */
