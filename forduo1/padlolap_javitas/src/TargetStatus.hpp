#ifndef SRC_TARGETSTATUS_HPP
#define SRC_TARGETSTATUS_HPP

#include "Status.hpp"
#include <boost/optional.hpp>

Status findTargetStatus(Status status, boost::optional<unsigned> seed);

#endif /* SRC_TARGETSTATUS_HPP */
