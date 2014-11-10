#include <vector>
#include <algorithm>
#include <iostream>
#include "bloodbank_api.h"

std::vector<std::size_t> runRound(BloodBank& bank, std::vector<size_t> indices, std::size_t stride) {
	Batch* batch = bank.createBatch();

	std::size_t i;
	for(i=0; i<indices.size()-stride; i+=stride) {
		std::size_t testId = batch->addTest();
		for(std::size_t j=0; j<stride; ++j) {
			batch->addSample(testId, indices[i+j]);
		}
	}


	if(i<indices.size()) {
		std::size_t testId = batch->addTest();
		for(; i<indices.size(); ++i) {
			batch->addSample(testId, indices[i]);
		}
	}

	bank.evaluateBatch(batch);
	std::vector<size_t> unknownValues;

	for(std::size_t test=0; test<batch->getNumberOfTests(); ++test) {
		std::vector<std::size_t> samples;
		batch->getSamples(test, samples);

		bool healthy = !batch->getTestResult(test);

		for(std::vector<size_t>::iterator it=samples.begin(); it!=samples.end(); ++it) {
			if(healthy) {
				bank.markSafe(*it);
			} else {
				// TODO: what if it was alone, then it is certainly bad
				unknownValues.push_back(*it);
			}
		}

	}
	return unknownValues;
}

void runtests(BloodBank& bank)
{

	const std::size_t stride1 = 9;
	const std::size_t stride2 = 3;
	const std::size_t stride3 = 1;

	std::vector<size_t> indices;
	indices.reserve(bank.getNumberOfSamples());
	for(size_t i = 0; i< bank.getNumberOfSamples(); ++i) {
		indices.push_back(i);
	}

	std::vector<std::size_t> failedFirstRound =
		runRound(bank, indices, stride1);

	std::vector<std::size_t> failedSecondRound =
		runRound(bank, failedFirstRound, stride2);

	std::vector<std::size_t> failedThirdRound =
		runRound(bank, failedSecondRound, stride3);

}
