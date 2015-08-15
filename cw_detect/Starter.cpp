/**
 * This program is designed to integrate the modules in the Radio Collar
 * Tracker post-processing code.
 *
 * @author NATHAN HUI <ntlhui@gmail.com>
 */
#include "FileLoader.hpp"
#include "FileWriter.hpp"
#include "Sample.hpp"
#include "Decimator.hpp"
#include <iostream>

/**
 * This is the main routine for this program.  This function initializes and
 * sets up the various module associated with this program.
 *
 * @param argc	Number of command line arguments
 * @param argv	Array of C strings representing arguments
 */
int main(int argc, char** argv){
	cout << "Starter: Loading classes" << endl;
	RFFileLoader* file_loader = new RFFileLoader(2048000);
	Decimator* decimator = new Decimator(2048000, 1000, file_loader);
	FileWriter* file_writer = new FileWriter(2048000, decimator, 1);
	file_writer->setMetaSuffix(".test");
	file_writer->setDataSuffix(".test");
	file_writer->setDataDir("./test/");
	file_writer->start();
	cout << "Starter: Adding file" << endl;
	file_loader->addFile("test.raw");
	cout << "Starter: Signaling Termination" << endl;
	file_loader->sendTerminating();
	cout << "Starter: Waiting for pipeline to finish" << endl;
	while(!file_writer->hasTerminating()){
	}
	cout << "Starter: Deleting classes" << endl;
	delete file_writer;
	delete decimator;
	delete file_loader;
	return 0;
}
