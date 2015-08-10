#include <complex>
#include "Sample.hpp"

using namespace std;

Sample::Sample(int index, int sample_rate) {
	this->index = index;
	this->sample_rate = sample_rate;
}



CRFSample::CRFSample(int index, int sample_rate,
                     complex<float> sample): Sample(index, sample_rate) {
	this->sample = complex<float>(sample);
}

CRFSample::CRFSample(int index, int sample_rate, float i,
                     float q): Sample(index, sample_rate) {
	this->sample = complex<float>(i, q);
}

complex<float> CRFSample::get_data() {
	return complex<float>(sample);
}

void CRFSample::set_data(complex<float> new_data) {
	this->sample = complex<float>(new_data);
}

void CRFSample::set_data(float i, float q) {
	this->sample = complex<float>(i, q);
}
