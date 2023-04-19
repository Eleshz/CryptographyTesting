#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <map>
#include <fstream>
#include <csignal>
#include <filesystem>
#include <deque>
#include <mutex>
#include <iostream>
#include <condition_variable>


std::mutex NoYouDont;

bool ContinueSearch = true;

std::deque<std::filesystem::path> FolderList;

std::vector<std::string> FileList;

std::vector<int> ThreadSelection;

std::deque<std::string> FolderToPassToThread;

int NumThreads;

std::condition_variable cv;
std::mutex cv_m;



std::string ScannerItem(std::string GivenFolder, int WhatThread, int NumberOfThreads);

std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads) {

	FolderList.resize(10);

	ThreadSelection.resize(NumberOfThreads);

	std::condition_variable WaitForMakingAllThreads;

	

	std::vector<std::thread> ThisThread;
	ThisThread.resize(NumberOfThreads);

	std::cout << "Setup Complete! " << NumberOfThreads << " threads started!\n";
	std::cout << "Storing " << ThisThread.size() << " thread objects!\n\n";

	for (int i = 0; i < ThisThread.size(); ++i) {

		std::thread th(ScannerItem, "", i, NumberOfThreads);

		ThisThread[i] = std::move(th);
	}
	
	NumThreads = NumberOfThreads;

	std::cout << "Made all threads!\n\n";

	FolderList.push_back("./");

	cv.notify_all();
	
	for (auto& i : ThisThread) {
		i.join();
	}
	std::cout << "\n\nAll done!";
	return "NULL";
}

std::string ScannerItem(std::string GivenFolder, int WhatThread, int NumberOfThreads) {
	
	std::unique_lock<std::mutex> lk(cv_m);
	cv.wait(lk, [&] {return NumThreads == NumberOfThreads; });

	std::cout << WhatThread+1 << " waited succeed...\n";

	return "blah";
}
