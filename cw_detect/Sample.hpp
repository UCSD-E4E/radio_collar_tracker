#ifndef __SAMPLE_H_
#define __SAMPLE_H_

#include <complex>

using namespace std;
/**
 * This class represents a sample of data in a stream of data.
 */
class Sample {
	private:
		/**
		 * Defines the sampling rate of this sample in samples per second
		 */
		int sample_rate;
		/**
		 * Defines the index of this sample, zero based
		 */
		int index;
		/**
		 * Flag that indicates whether or not this Sample is a terminating
		 * sample.
		 */
		bool terminating = false;
	public:
		/**
		 * Creates a Sample object with a specified index and sampling rate
		 */
		Sample(int index, int sample_rate);
		/**
		 * Returns the index of this sample.
		 */
		int getIndex();
		/**
		 * Returns the sampling rate of the associated signal
		 */
		int getSampleRate();
		/**
		 * Sets the terminating sample flag for this Sample.
		 */
		void setTerminating();
		/**
		 * Unset the terminating sample flag for this Sample.
		 */
		void setNotTerminating();
		/**
		 * Checks if this Sample is the terminating sample or not.
		 *
		 * @return true if this sample is a terminating sample, false otherwise
		 */
		 bool isTerminating();
};

/**
 * This class represetns a single complex RF data sample.
 */
class CRFSample: public Sample {
	private:
		complex<float> sample;
	public:
		/**
		 * Creates a new RFSample, with its own copy of the complex sample.
		 *
		 * @param index	The index of this sample in the stream, starting at 0
		 * @param sample_rate	The sampling rate of this stream.
		 * @param sample	The sample value this object should reflect
		 */
		CRFSample(int index, int sample_rate, complex<float> sample);

		/**
		 * Creates a new RFSample, with the complex sample set to i + j * q, where j
		 * is the imaginary unit.
		 *
		 * @param index	The index of this sample in the stream, starting at 0
		 * @param sample_rate	The sampling rate of this stream.
		 * @param i	The in-phase or real part of the complex value
		 * @param q	The quadrature or imaginary part of the complex value
		 */
		CRFSample(int index, int sample_rate, float i, float q);

		/**
		 * Returns a copy of the complex data stored internally.
		 *
		 * @return	The complex value represented by this sample
		 */
		complex<float> getData();

		/**
		 * Sets the internal data to reflect the value of the new data provided.
		 * Specifically, this sample will represent the same time, but with sample
		 * value set to be equal to new_data.
		 *
		 * @param new_data	The new data this object should reflect.
		 */
		void setData(complex<float> new_data);

		/**
		 * Sets the interal data to reflect the value of the new data provided.
		 * Specifically, this sample will represent the same time, but with sample
		 * value i + j * q, where j is the imaginary unit.
		 *
		 * @param i The in-phase or real part of the complex value.
		 * @param q The quadrature or imaginary part of the complex value.
		 */
		void setData(float i, float q);
};

/**
 * Interface for all output classes.
 *
 * Specifies a single callback function that permits another object to retrieve
 * in order the samples generated by this factory.
 *
 * @author NATHAN HUI <ntlhui@gmail.com>
 */
class SampleFactory{
	public:
		/**
		 * Virtual destructor.  Must stop any associated threads.
		 */
		virtual ~SampleFactory(){};
		/**
		 * Callback function that returns the next available sample from this
		 * factory object.
		 *
		 * @return	Pointer to a CRFSample object representing the next
		 * 			available sample from this factory object.
		 */
		virtual CRFSample* getNextSample(){
			return NULL;
		};
};
#endif // __SAMPLE_H_