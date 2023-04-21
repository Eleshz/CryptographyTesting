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
#include <functional>

namespace fs = std::filesystem;

int Num_ThreadsStarted = 0;

bool AllThreadsStartedBool = false;

std::string FinalFileLocation = "0";

std::mutex iostream_pause;
std::mutex intInput_pause;
std::mutex FolderInput_pause;
std::mutex FileInput_pause;
std::mutex AllScannersHaveStarted;

std::condition_variable WaitForThreads;

std::deque<std::filesystem::path> FolderList;
std::deque<std::filesystem::path> FolderDistribution;
std::vector<int> Thread_Available;
std::vector<std::thread> ThisThread;

std::string ScannerItem(int WhatThread);
int FileDistributor();

std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads){

	FolderList.resize(NumberOfThreads);
	FolderDistribution.resize(NumberOfThreads);
	Thread_Available.resize(NumberOfThreads);
	ThisThread.resize(NumberOfThreads);
	Num_ThreadsStarted = NumberOfThreads;

	for (int i = 0; i < FolderDistribution.size(); i++) {
		FolderDistribution[i] = "0";
	}

	for (int i = 0; i < ThisThread.size(); ++i) {
		std::thread ScanThread(ScannerItem, i);
		ThisThread[i] = std::move(ScanThread);
	}

	iostream_pause.lock();
	std::cout << "Made all threads!\n\n";
	iostream_pause.unlock();

	FolderList.push_front(StartSearchLocation);

	std::unique_lock<std::mutex> ThreadLock(AllScannersHaveStarted);
	while (!AllThreadsStartedBool) WaitForThreads.wait(ThreadLock);

	std::thread Dis_Thread(FileDistributor);

	std::cout << "Started FolderDistributor\n\n";

	int ThisThreadSize = ThisThread.size();
	for (int i = 0; i < ThisThread.size(); ++i){
		ThisThread[i].join();
	} Dis_Thread.join();

	return "GOOD";
}

std::string ScannerItem(int WhatThread) {
	//Waits for every thread to start before the file distributor starts
	intInput_pause.lock(); --Num_ThreadsStarted; intInput_pause.unlock();

	std::deque<std::filesystem::path> FileList;

	if (Num_ThreadsStarted == 0) {
		AllThreadsStartedBool = true;
		WaitForThreads.notify_all();
	} else;

	while (FinalFileLocation == "0") {

		if (FolderDistribution[WhatThread] == "0") {

			for (const auto& file : fs::directory_iterator(fs::path(FolderDistribution[WhatThread]))) {

				if (fs::is_directory(file)) {
					std::cout << file << " <--- Folder \n";
					FolderList.push_back(file);
				}
				else {
					std::cout << file << " <--- File \n";
					FileList.push_back(file.path());
				}

			}



		} else
		{
			continue;
		}

	}



	return "Ok";
}

int FileDistributor() {
	while (FinalFileLocation == "0") {
		fs::path folder = FolderList.front();
		if (folder.empty()) {
			FolderList.pop_front();
			continue;
		}
		for (auto i : Thread_Available) {
			if (Thread_Available[i] == 1) {
				FolderDistribution[i] = folder;
			}
		}
	}
	return 0;
}

