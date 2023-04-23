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
int Num_ThreadsENDed = 0;

bool AllThreadsStartedBool = false;
bool AllThreadsENDedBool = false;

std::string FinalFileLocation = "0";

std::mutex iostream_pause;
std::mutex intInput_pause;
std::mutex FolderInput_pause;
std::mutex FileInput_pause;
std::mutex AllScannersHaveStarted;
std::mutex AllScannersHaveENDed;
std::mutex Thread_SP_Pause;

std::condition_variable WaitForThreads;
std::condition_variable WaitForThreadsEND;

std::deque<std::filesystem::path> FolderList;
std::deque<std::filesystem::path> FolderDistribution;
std::vector<bool> Thread_Available;
std::vector<std::thread> ThisThread;

std::string FileToBeFound_Thread;

void ScannerItem(int WhatThread);
void FileDistributor();

std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads){

	FolderDistribution.resize(NumberOfThreads);
	Thread_Available.resize(NumberOfThreads);
	ThisThread.resize(NumberOfThreads);
	FileToBeFound_Thread = FileToBeFound;

	Num_ThreadsStarted = NumberOfThreads;
	Num_ThreadsENDed = NumberOfThreads + 1;


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

	std::unique_lock<std::mutex> ThreadLockEND(AllScannersHaveENDed);
	while (!AllThreadsENDedBool) WaitForThreadsEND.wait(ThreadLock);

	for (int i = 0; i < ThisThread.size(); ++i){
		ThisThread[i].join();
	} Dis_Thread.join();

	FolderList.clear();

	return FinalFileLocation;
}

void ScannerItem(int WhatThread) {
	//Waits for every thread to start before the file distributor starts
	intInput_pause.lock(); --Num_ThreadsStarted; intInput_pause.unlock();

	std::deque<std::filesystem::path> FileList;

	if (Num_ThreadsStarted == 0) {
		AllThreadsStartedBool = true;
		WaitForThreads.notify_all();
	} else;

	Thread_Available[WhatThread] = true;

	Thread_SP_Pause.lock();
	while (FinalFileLocation == "0") {

		Thread_SP_Pause.unlock();

		if (FolderDistribution[WhatThread].string() != "0")
		{
				Thread_Available[WhatThread] = false;
				try {

					fs::directory_iterator iter(FolderDistribution[WhatThread]);

					for (const auto& file : iter) {
						if (fs::is_directory(file)) {
							intInput_pause.lock();
							FolderList.push_back(file);
							intInput_pause.unlock();
						}
						else {
							FileList.push_back(file.path());
						}

						for (auto file : FileList) {
							if (file.filename() == FileToBeFound_Thread) {
								Thread_SP_Pause.lock();
								FinalFileLocation = file.string();
								Thread_SP_Pause.unlock();
								goto END;
							}
						}
						FileList.clear();
					}
					Thread_SP_Pause.lock();
					FolderDistribution[WhatThread] = "0";
					Thread_Available[WhatThread] = true;
					Thread_SP_Pause.unlock();
				}
				catch (...) {
					Thread_SP_Pause.lock();
					std::cout << "EX,";
					FolderDistribution[WhatThread] = "0";
					Thread_Available[WhatThread] = true;
					Thread_SP_Pause.unlock();
				}
		}
		else {
			continue;
		}
	}

	END:

	--Num_ThreadsENDed;

	Thread_SP_Pause.lock();
	if (Num_ThreadsENDed == 0) {
		AllThreadsENDedBool = true;
		WaitForThreadsEND.notify_all();
	} else;
	Thread_SP_Pause.unlock();
}

void FileDistributor() {

	while (FinalFileLocation == "0") {

		if (FolderList.size() == 0)
			continue;

		fs::path folder = FolderList.front();

		if (folder.empty()) {
			Thread_SP_Pause.lock();
			FolderList.pop_front();
			Thread_SP_Pause.unlock();
			continue;
		}


		if (FolderList.size() == 0)
			continue;

		for (int i = 0; i < Thread_Available.size(); ++i) {

			if (FolderList.size() == 0)
				break;
			try
			{
				if (Thread_Available[i] == true) {
					Thread_SP_Pause.lock();
					Thread_Available[i] = false;
					FolderDistribution[i] = folder;
					FolderList.pop_front();
					Thread_SP_Pause.unlock();
				}
			}
			catch (...) {
				std::cout << "\n\nMAX SIZE OF DEQUE REACHED\n\n";
			}
		}
	}
	intInput_pause.lock(); --Num_ThreadsENDed;
	if (Num_ThreadsENDed == 0) {
		AllThreadsENDedBool = true;
		WaitForThreadsEND.notify_all();
	} else;
	intInput_pause.unlock();
}

