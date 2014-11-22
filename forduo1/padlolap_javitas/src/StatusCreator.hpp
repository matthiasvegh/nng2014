#ifndef STATUSCREATOR_H_
#define STATUSCREATOR_H_

#include <vector>
#include <string>
#include <memory>
#include "Status.hpp"

Status
createStatus(int width, int height, const std::vector<std::string>& lines);

Status loadStatusFromFile(const char *filename);


#endif /* STATUSCREATOR_H_ */
