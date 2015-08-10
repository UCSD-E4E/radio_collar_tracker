#include <queue>
#include "FileLoader.hpp"
#include <string>
#include "Sample.hpp"
#include <iostream>
#include <cstdint>
#include <sys/stat.h>
using namespace std;

#ifndef QUEUE_SIZE_MAX
#define QUEUE_SIZE_MAX 1024000
#endif // QUEUE_SIZE_MAX

void RFFileLoader::run() {
	// Make variables
	string file_to_load;
	ifstream file_stream;
	uint8_t buffer[2];	// read buffer
	int index = 0;	// index of sample

	// thread body
	while (run_state) {
		// Lock queue
		file_queue_mutex.lock();
		// check if empty
		if (file_queue.empty()) {
			// Get file
			file_to_load = file_queue.front();
			file_queue.pop();
			// Unlock
			file_queue_mutex.unlock();

			// Make stream
			file_stream.open(file_to_load, ios::in | ios::binary);
			// If fail
			if (file_stream.fail()) {
				cerr << "Failed to open file!" << endl;
			}

			// while read 2 into the buffer
			while (file_stream.read((char*)buffer, 2)) {
				// Load into queue if space
				signal_queue_mutex.lock();
				while(signal_queue.size() >= QUEUE_SIZE_MAX){
					signal_queue_mutex.unlock();
					// wait
					// TODO force using posix wait
					signal_queue_mutex.lock();
				}

				signal_queue.emplace(index, sample_rate, buffer[0] / 128.0 - 0.5,
						buffer[1] / 128.0 - 0.5);
				signal_queue_mutex.unlock();

				// close stream
				file_stream.close();
			}
		} else {
			// queue empty

			// unlock queue
			file_queue_mutex.unlock();

			// wait
			// TODO use conditional variable.  For now, just bash
		}
	}
}

RFFileLoader::RFFileLoader(int sample_rate) : sample_rate(sample_rate), run_state(true) {
	// Start worker thread associated with this class
	thread class_thread(&RFFileLoader::run, this);
}

void RFFileLoader::addFile(string filename){
	// Check file is good
	struct stat buffer;
	stat(filename.c_str(), &buffer);
	if(!S_ISREG(buffer.st_mode)){
		cerr << "Failed to open file!" << endl;
	}
	// Add to queue
	file_queue_mutex.lock();
	file_queue.push(filename);
	file_queue_mutex.unlock();
	// clean up
}

RFFileLoader::~RFFileLoader(){
	run_state = false;
	class_thread.join();
}

CRFSample RFFileLoader::getNextSample(){
	signal_queue_mutex.lock();
	CRFSample retval = signal_queue.front();
	signal_queue.pop();
	signal_queue_mutex.unlock();
	return retval;
}
