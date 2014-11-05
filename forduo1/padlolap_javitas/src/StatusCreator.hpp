#ifndef STATUSCREATOR_H_
#define STATUSCREATOR_H_

#include <vector>
#include <string>
#include <memory>

class Status;
class Table;

Status
createStatus(int width, int height, const std::vector<std::string>& lines);

Status loadStatusFromFile(const char *filename);


#endif /* STATUSCREATOR_H_ */
