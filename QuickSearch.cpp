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

std::mutex NoYouDont;

std::deque<std::filesystem::path> FolderList;

std::deque<std::filesystem::path> FileList;

std::vector<int> ThreadSelection;

std::deque<std::filesystem::path> FolderToPassToThread;

std::vector<std::thread> ThisThread;

std::string ScannerItem(std::string GivenFolder, int WhatThread);

int FileDistributor(std::deque<std::filesystem::path> &FolderListPass);

std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads) {

	namespace fs = std::filesystem;

	ThreadSelection.resize(NumberOfThreads);

	ThisThread.resize(NumberOfThreads);

	for (int i = 0; i < ThisThread.size(); ++i) {
		std::thread ScanThread(ScannerItem, "", i);
		ThisThread[i] = std::move(ScanThread);
	}


	NoYouDont.lock();
	std::cout << "Made all threads!\n\n";
	NoYouDont.unlock();

	FolderList.push_front(StartSearchLocation);

	std::thread Dis_Thread(FileDistributor, std::ref(FolderList));

	std::cout << "Started FolderDistributor\n\n";

	int ThisThreadSize = ThisThread.size();

	for (int i = 0; i < ThisThread.size(); ++i) {
		ThisThread[i].join();
	}

	Dis_Thread.join();

	std::cout << "\n\nAll done! Found file at: ";

	return "GOOD";
}

std::string ScannerItem(std::string GivenFolder, int WhatThread) {



	return "blah";
}

int FileDistributor(std::deque<std::filesystem::path>& FolderListPass) {



	std::cout << "File distributor woke up!\n";

	return 0;
}

