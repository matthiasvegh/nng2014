#include "fastSolve.hpp"
#include <iostream>

class InternalFastSolver {
	Status status;
	Status targetStatus;
	Array<std::vector<Point>> badPositions{3, 3};
public:
	std::vector<SimpleNode> result;
	InternalFastSolver(Status status, Status targetStatus):
		status(std::move(status)),
		targetStatus(std::move(targetStatus))
	{}

	void swapBadPositions(int color1, int color2)
	{
		auto& bad1 = badPositions[Point{color1, color2}];
		auto& bad2 = badPositions[Point{color2, color1}];
		while (!bad1.empty() && !bad2.empty()) {
			std::swap(status[bad1.back()], status[bad2.back()]);
			result.emplace_back(status, MoveDescriptor{bad1.back(), bad2.back()});
			bad1.pop_back();
			bad2.pop_back();
		}
	}

	bool isOk(Point p)
	{
		return status[p] == targetStatus[p];
	}

	bool swapOneBadPosition()
	{
		auto range = arrayRange(status);
		for (auto it = range.begin(); it != range.end(); ++it) {
			if (!isOk(*it)) {
				for (auto it2 = std::next(it); it2 != range.end(); ++it2) {
					if (!isOk(*it2) && status[*it2] == targetStatus[*it]) {
						std::swap(status[*it], status[*it2]);
						result.emplace_back(status,
								MoveDescriptor{*it, *it2});
						return true;
					}
				}
			}
		}
		return false;
	}

	void solve()
	{
		while (status != targetStatus) {
			badPositions.reset(3, 3);
			for (Point p: arrayRange(status)) {
				badPositions[Point{status[p], targetStatus[p]}].push_back(p);
			}

			swapBadPositions(0, 1);
			swapBadPositions(0, 2);
			swapBadPositions(1, 2);

			swapOneBadPosition();
		}
	}
};

std::vector<SimpleNode> fastSolve(Status status, Status targetStatus)
{
	InternalFastSolver solver(std::move(status), std::move(targetStatus));
	solver.solve();
	return solver.result;
}


