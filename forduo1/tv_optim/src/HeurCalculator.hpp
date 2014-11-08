#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

#include "Status.hpp"
#include <memory>
#include <boost/noncopyable.hpp>

class HeurCalculator: public boost::noncopyable {
public:
	typedef std::shared_ptr<const HeurCalculator> Ptr;

	HeurCalculator() {}

	int calculateStatus(const Status& status);
};

#endif /* HEURCALCULATOR_H_ */
