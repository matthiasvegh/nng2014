#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include "bloodbank_api.h"

struct StatisticsRecorder {
	std::vector<std::vector<std::size_t> > failedSampleses;
	std::vector<std::size_t> healthySamples;

	void registerHealthy(std::size_t v) {
		healthySamples.push_back(v);
	}
	void registerFail(const std::vector<std::size_t>& vec) {
		failedSampleses.push_back(std::vector<std::size_t>());
		std::copy(vec.begin(), vec.end(), std::back_inserter(failedSampleses.back()));
	}

};

template<typename HealthyHandler, typename UnhealthyHandler>
std::vector<std::size_t> runRound(
		BloodBank& bank,
		std::vector<size_t> indices,
		std::size_t stride,
		HealthyHandler& h,
		UnhealthyHandler& uh,
		StatisticsRecorder& recorder)
{
	if(indices.size() < stride) {
		return indices;
	}
	Batch* batch = bank.createBatch();

	std::size_t i;
	for(i=0; i<indices.size()-stride; i+=stride) {
		std::size_t testId = batch->addTest();
		for(std::size_t j=0; j<stride; ++j) {
			batch->addSample(testId, indices[i+j]);
		}
	}

	std::vector<size_t> unknownValues;

	if(i<indices.size()) {
		std::copy(indices.begin()+i, indices.end(), std::back_inserter(unknownValues));
	}

	bank.evaluateBatch(batch);

	for(std::size_t test=0; test<batch->getNumberOfTests(); ++test) {
		std::vector<std::size_t> samples;
		batch->getSamples(test, samples);

		bool healthy = !batch->getTestResult(test);

		if(healthy) {
			for(std::vector<size_t>::iterator it=samples.begin(); it!=samples.end(); ++it) {
				recorder.registerHealthy(*it);
				h(*it);
			}
		} else {
			recorder.registerFail(samples);
			if(samples.size() == 1) {
				uh(samples[0]);
			} else {
				std::copy(samples.begin(), samples.end(), std::back_inserter(unknownValues));
			}
		}

	}
	return unknownValues;
}

struct UnhealthyRegister {
	std::size_t numberFailed;
	UnhealthyRegister():numberFailed(0) { }
	void operator()(std::size_t) {
		++numberFailed;
	}
};

struct HealthyRegister {
	BloodBank& bank;
	std::size_t numberPassed;
	HealthyRegister(BloodBank& bank): bank(bank), numberPassed(0) { }
	void operator()(std::size_t sample) {
		bank.markSafe(sample);
		++numberPassed;
	}
};

bool doWeContinue(std::size_t numberPassed, std::size_t numberOfSamples) {
	return float(numberPassed) < 0.8f * float(numberOfSamples);
}

#define HANDLEROUND(h, s) \
	if(not doWeContinue((h).numberPassed, (s))) { \
		return; \
	}\


void runtests(BloodBank& bank)
{

	const std::size_t stride1 = 6;
	const std::size_t stride2 = 3;

	std::vector<size_t> indices;
	indices.reserve(bank.getNumberOfSamples());
	for(size_t i = 0; i< bank.getNumberOfSamples(); ++i) {
		indices.push_back(i);
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(indices.begin(), indices.end(), g);

	const std::size_t totalSamples = bank.getNumberOfSamples();
	HealthyRegister reg(bank);
	UnhealthyRegister ureg;
	StatisticsRecorder rec;

	std::vector<std::size_t> failedFirstRound =
		runRound(bank, indices, stride1, reg, ureg, rec);

	HANDLEROUND(reg, totalSamples);
	std::shuffle(failedFirstRound.begin(), failedFirstRound.end(), g);

		std::vector<std::size_t> failedSecondRound =
		runRound(bank, failedFirstRound, stride2, reg, ureg, rec);

	HANDLEROUND(reg, totalSamples);
	std::shuffle(failedSecondRound.begin(), failedSecondRound.end(), g);

	const std::size_t badRemaining = std::size_t(indices.size()/10)+1 - ureg.numberFailed;
	const std::size_t minimumToPass = (std::size_t(indices.size()*0.8)+1) - reg.numberPassed;

	failedSecondRound.resize(minimumToPass+badRemaining);

	std::vector<std::size_t> failedThirdRound =
		runRound(bank, failedSecondRound, 1, reg, ureg, rec);

}
