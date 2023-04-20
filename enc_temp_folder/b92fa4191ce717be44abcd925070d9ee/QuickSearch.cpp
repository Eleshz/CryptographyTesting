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
#include <utility>


std::mutex NoYouDont;

bool ContinueSearch = true;

int StartSearch1or0 = 0;

std::deque<std::filesystem::path> FolderList;

std::deque<std::filesystem::path> FileList;

std::vector<int> ThreadSelection;

std::deque<std::filesystem::path> FolderToPassToThread;

int NumThreads;

std::condition_variable ControlV;
std::mutex CV_m;



std::string ScannerItem(std::string GivenFolder, int WhatThread, int NumberOfThreads);

int FileDistributor(std::deque<std::filesystem::path> &FolderListPass, int Start);

int FileDistributor(std::deque<std::filesystem::path> &FolderListPass, int Start) {



	std::cout << "File distributor woke up!\n";

	return 0;
}

std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads) {

	namespace fs = std::filesystem;

	FolderList.resize(10);

	ThreadSelection.resize(NumberOfThreads);

	std::condition_variable WaitForMakingAllThreads;



	std::vector<std::thread> ThisThread;
	ThisThread.resize(NumberOfThreads);

	for (int i = 0; i < ThisThread.size(); ++i) {
		std::thread ScanThread(ScannerItem, "", i, NumberOfThreads);
		ThisThread[i] = std::move(ScanThread);
	}


	NoYouDont.lock();
	std::cout << "Made all threads!\n\n";
	NoYouDont.unlock();

	FolderList.push_front(StartSearchLocation);

	std::thread Dis_Thread(FileDistributor, std::ref(FolderList), StartSearch1or0);

	std::cout << "Started FolderDistributor\n\n";

	NumThreads = NumberOfThreads;

	std::cout << ThisThread.size() << "\n\n";

	//for (int i = 0; i < ThisThread.size(); ++i) {

	//	std::cout << "HI\n";

	//	if (ThisThread[i].joinable()) { ThisThread[i].join();}
	//	else
	//	{
	//		std::cout << "Thread " << i << "skipped";
	//		continue;
	//	}
	//}

	std::cout << "\n\nAll done! Found file at: ";
	return "GOOD";
}

std::string ScannerItem(std::string GivenFolder, int WhatThread, int NumberOfThreads) {
	NoYouDont.lock();
	std::cout << WhatThread+1 << " waited succeed...\n";
	NoYouDont.unlock();

	return "blah";
}

