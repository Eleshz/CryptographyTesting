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


std::mutex NoYouDont;


bool ContinueSearch = true;

int ThreadsStarted = 0;

std::deque<std::filesystem::path> FolderList;

std::vector<std::string> FileList;

std::vector<int> ThreadSelection;

std::deque<std::string> FolderToPassToThread;


std::string ScannerItem(std::string GivenFolder, int WhatThread);

std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads) {

	ThreadSelection.resize(NumberOfThreads);

	

	std::vector<std::thread> ThisThread;
	ThisThread.resize(NumberOfThreads);

	std::cout << "Setup Complete! " << NumberOfThreads << " threads started!\n";
	std::cout << "Storing " << ThisThread.size() << " thread objects!\n\n";

	for (int i = 0; i < ThisThread.size(); ++i) {

		std::thread th(ScannerItem, "", i);

		ThisThread[i] = std::move(th);
	}
	
	for (auto& i : ThisThread) {
		i.join();
	}
	
	std::cout << "\n\nAll done!";
	

	return "NULL";
}

std::string ScannerItem(std::string GivenFolder, int WhatThread) {
	
	thread_local std::thread current_thread;
	

	NoYouDont.lock();

	std::cout << "Thread started: " << WhatThread+1 << "\n";

	NoYouDont.unlock();

	return "blah";
}
