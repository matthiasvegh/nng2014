#include <vector>
#include <algorithm>

#include "bloodbank_api.h"

void runtests(BloodBank& bank)
{
	Batch* first= bank.createBatch();

	const size_t stride = 2;
	std::vector<size_t> indices;
	indices.reserve(bank.getNumberOfSamples());
	for(size_t i = 0; i< bank.getNumberOfSamples(); ++i) {
		indices.push_back(i);
	}

	for(std::vector<size_t>::iterator it = indices.begin(); it != indices.end(); it+=stride) {
		size_t testId = first->addTest();
		for(size_t j = 0; j < stride; ++j) {
			first->addSample(testId, *(it+j));
		}
	}
	if (bank.getNumberOfSamples() % stride) {
		size_t remainderId = first->addTest();
		for(std::vector<size_t>::iterator it = indices.begin() + ((indices.size() / stride) * stride -1); it != indices.begin(); ++it) {
			first->addSample(remainderId, *it);
		}
	}

	bank.evaluateBatch(first);

	Batch* second= bank.createBatch();
	std::vector<size_t> samples;
	for(size_t i= 0; i < first->getNumberOfTests(); ++i)
	{
		first->getSamples(i, samples);
		if (first->getTestResult(i))
		{
			// BAD Result, retest
			for(std::vector<size_t>::iterator s= samples.begin(); s != samples.end(); ++s)
			{
				size_t testID= second->addTest();
				second->addSample(testID, *s);
			}
		} else
		{
			for(std::vector<size_t>::iterator s= samples.begin(); s != samples.end(); ++s)
			{
				bank.markSafe(*s);
			}
		}
	}
	bank.evaluateBatch(second);

	for(size_t i= 0; i < second->getNumberOfTests(); ++i)
	{
		if (! second->getTestResult(i))
		{
			// Negative test, keep
			second->getSamples(i, samples);
			bank.markSafe(samples[0]);
		}
	}
}
