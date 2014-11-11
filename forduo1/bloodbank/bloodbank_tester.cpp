#include <vector>
#include <algorithm>
#include <ctime>
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

	std::vector<size_t> indices;
	indices.reserve(bank.getNumberOfSamples());
	for(size_t i = 0; i< bank.getNumberOfSamples(); ++i) {
		indices.push_back(i);
	}

	std::srand(std::time(0));

	std::random_shuffle(indices.begin(), indices.end());

	const std::size_t totalSamples = bank.getNumberOfSamples();
	HealthyRegister reg(bank);
	UnhealthyRegister ureg;
	StatisticsRecorder rec;

	std::vector<std::size_t> failedFirstRound =
		runRound(bank, indices, stride1, reg, ureg, rec);
	std::cout<<"healthies found round 1: "<<reg.numberPassed<<std::endl;

	HANDLEROUND(reg, totalSamples);
	std::random_shuffle(failedFirstRound.begin(), failedFirstRound.end());

	std::size_t unHealthyRemaining = float(totalSamples)*0.1 - ureg.numberFailed;
	float p = unHealthyRemaining/float(failedFirstRound.size());
	std::size_t stride2 = 1.0/p -2;

	std::vector<std::size_t> failedSecondRound =
		runRound(bank, failedFirstRound, stride2, reg, ureg, rec);
	std::cout<<"healthies found round 2: "<<reg.numberPassed<<std::endl;

	HANDLEROUND(reg, totalSamples);
	std::random_shuffle(failedSecondRound.begin(), failedSecondRound.end());

	const std::size_t badRemaining = std::size_t(indices.size()/10)+1 - ureg.numberFailed;
	const std::size_t minimumToPass = (std::size_t(indices.size()*0.8)+1) - reg.numberPassed;

	failedSecondRound.resize(minimumToPass+badRemaining);

	std::vector<std::size_t> failedThirdRound =
		runRound(bank, failedSecondRound, 1, reg, ureg, rec);

}
