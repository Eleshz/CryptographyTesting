#include <memory>
#include <string>
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
#include <atomic>

namespace fs = std::filesystem;
using namespace std::literals::chrono_literals;

// A global lock to be used by everything
std::mutex GlobalPause;
// List of folder that needs to be scanned, scanners put in, distributor take out and sends back to scanner threads
std::deque<fs::path> FolderList;
// Passes the folder to a specific thread, lock with "ThreadPersonalMutex" before using
std::vector<fs::path> FolderDistribution;
// Lets the file distributor know if a folder is done
std::vector<bool> ThreadDone;
// Lets the file scanner know if a folder is ready and the thread can start work again
std::vector<bool> ThreadCanStart;
// Each thread gets one, the scanner will not restart unless it is woken by the distributor
std::vector <std::unique_ptr<std::condition_variable>> ThreadBlocker;
// Each thread gets a lock also, can be locked for the file distributor input
std::vector <std::unique_ptr<std::mutex>> ThreadPersonalMutex;
// Holds the threads, mostly for joining
std::vector<std::thread> ThreadHolder;
// Set this when you have found the final path
std::string FinalPath = "0";
// Just holds the name of the file that needs to be found, but it's here so it's global
fs::path FileToFind = "0";

void ScannerItem(int WhatThread);
void FileDistributor();

fs::path QuickSearch(const fs::path StartSearchLocation, const fs::path FileToBeFound, int ThreadsToStart){

	FileToFind = FileToBeFound;

	if (ThreadsToStart < 1) { ThreadsToStart = std::ceil((std::thread::hardware_concurrency())*0.75);}
	ThreadHolder.resize(ThreadsToStart);
	ThreadCanStart.resize(ThreadsToStart);
	ThreadDone.resize(ThreadsToStart);
	FolderDistribution.resize(ThreadsToStart);

	for (int i = 0; i < ThreadsToStart; ++i) {
		ThreadDone[i] = true;
		ThreadCanStart[i] = false;
		ThreadPersonalMutex.emplace_back(std::make_unique<std::mutex>());
		ThreadBlocker.emplace_back(std::make_unique<std::condition_variable>());
		std::thread Thread(ScannerItem, i);
		ThreadHolder[i] = std::move(Thread);
		FolderDistribution[i] = "NeedFolder";
	}

	FolderList.push_front(StartSearchLocation);

	FileDistributor();

	for (int i = 0; i < ThreadsToStart; ++i) {
		ThreadCanStart[i] = true;
		ThreadBlocker[i]->notify_all();
		ThreadHolder[i].join();
	}

	return FinalPath;
}

void ScannerItem(int WhatThread){

	while (true) {

		if (FinalPath != "0")
			break;

		// Make local file and folder lists

		std::vector<std::filesystem::path> FileListLocal;
		std::vector<std::filesystem::path> FolderListLocal;

		// Wait with a CV until a folder exists to scan

		std::mutex ThreadMutexForStarting;

		std::unique_lock<std::mutex> ThreadLockStart(ThreadMutexForStarting);
		while (!ThreadCanStart[WhatThread]) ThreadBlocker[WhatThread]->wait(ThreadLockStart);

		ThreadDone[WhatThread] = false;
		ThreadCanStart[WhatThread] = false;

		ThreadPersonalMutex[WhatThread]->lock();
		fs::path WorkingPath = FolderDistribution[WhatThread];
		ThreadPersonalMutex[WhatThread]->unlock();

		if (WorkingPath == "0")
			break;

		fs::directory_iterator iter(WorkingPath);

		for (const auto& file : iter) {
			try {
				if (fs::is_directory(file)) {
					FolderListLocal.push_back(file);
				}
				else {
					FileListLocal.push_back(file.path());
				}

			}
			catch (...) {
				std::cout << "EX,";
			}
		}

		for (auto file : FileListLocal) {
			if (file.filename() == FileToFind) {

				std::unique_lock<std::mutex> TempLock(GlobalPause);
				FinalPath = file.string();
				break;
			}
		}

		std::unique_lock<std::mutex> TempLock(GlobalPause);
		std::copy(FolderListLocal.begin(), FolderListLocal.end(), std::back_inserter(FolderList));
		TempLock.unlock();

		FileListLocal.clear();
		FolderListLocal.clear();

		ThreadPersonalMutex[WhatThread]->lock();
		FolderDistribution[WhatThread] = "0";
		ThreadDone[WhatThread] = true;
		ThreadPersonalMutex[WhatThread]->unlock();
	}

	FolderDistribution[WhatThread] = "";
	ThreadDone[WhatThread] = false;
	ThreadCanStart[WhatThread] = false;
	return;
}

void FileDistributor(){
	while (FinalPath == "0") {

		fs::path TempPath;

		if (FolderList.empty()) {
			continue;
		}

		for (int i = 0; i < ThreadHolder.size(); ++i) {

			if (FolderList.empty()) {
				break;
			}

			fs::path TempPath = FolderList[0];

			try {
				if (fs::is_empty(TempPath) || fs::is_symlink(TempPath)) {
					FolderList.pop_front();
					continue;
				}
			}
			catch (...) {
				FolderList.pop_front();
				continue;
			}

			if (ThreadDone[i] == true) {

				ThreadPersonalMutex[i]->lock();
				FolderDistribution[i] = FolderList[0];
				FolderList.pop_front();
				ThreadCanStart[i] = true;
				ThreadPersonalMutex[i]->unlock();
				ThreadBlocker[i]->notify_all();
			}
		}

	}
}

