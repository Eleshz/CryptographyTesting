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

std::atomic<int> Num_ThreadsStarted = 0;
std::atomic<int> Num_ThreadsENDed = 0;
std::atomic<int> Num_Threads;

bool AllThreadsStartedBool = false;
bool AllThreadsENDedBool = false;

std::string FinalFileLocation = "Yes";

std::mutex AllThreadsHaveStarted;
std::mutex AllThreadsHaveENDed;
std::mutex General_Pause;
std::mutex (File_DIS);

std::condition_variable WaitForThreads;
std::condition_variable WaitForThreadsEND;

std::vector<std::unique_ptr<std::condition_variable>> Thread_CV;

std::deque<std::filesystem::path> FolderList;
std::vector<std::filesystem::path> FolderDistribution;
std::vector<bool> Thread_Available;
std::vector<std::thread> ThisThread;

std::string FileToBeFound_Thread;

void ScannerItem(int WhatThread);
void FileDistributor();

/// <summary>
/// Give this the place you want to start searching, the file i.e. "Test.txt" and the number of threads, put 0 and it will assume based on your computer
/// </summary>
/// <param name="StartSearchLocation"></param>
/// <param name="FileToBeFound"></param>
/// <param name="NumberOfThreads"></param>
/// <returns></returns>
std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads){

	//Set sizes (Mostly for iteration)
	Num_Threads = NumberOfThreads;
	Thread_Available.resize(NumberOfThreads);
	ThisThread.resize(NumberOfThreads);
	Num_ThreadsENDed = NumberOfThreads+1;

	//Making condition variables and putting them into this super cool vector
	for (int i = 0; i < NumberOfThreads; ++i) {
		Thread_CV.push_back(std::make_unique<std::condition_variable>());
	}
	//Reversing the vector because I wanted to use the push back thing for some reason
	std::reverse(Thread_CV.begin(), Thread_CV.end());

	// Add root folder to the list of folders that need scanning
	FolderList.push_front(StartSearchLocation);

	//I make lots of scanning threads
	for (int i = 0; i < ThisThread.size(); ++i) {
		std::thread ScanThread(ScannerItem, i);
		ThisThread[i] = std::move(ScanThread);
	}
	//I make distributor thread
	std::thread Dis_Thread(FileDistributor);

	//Wait for everything to end
	std::unique_lock<std::mutex> ThreadLockEND(AllThreadsHaveENDed);
	while (!AllThreadsENDedBool) WaitForThreadsEND.wait(ThreadLockEND);

	// I join all the threads because I'm a good boy
	for (int i = 0; i < ThisThread.size(); ++i) {
		ThisThread[i].join();
	} Dis_Thread.join();
	FolderList.clear();

	//Output the directory
	return FinalFileLocation;
}

/// <summary>
/// Scans a given folder in the vector of it's thread number
/// </summary>
/// <param name="WhatThread"></param>
void ScannerItem(int WhatThread) {

	General_Pause.lock();
	std::cout << "Started thread " << WhatThread << "\n";
	General_Pause.unlock();

	//Hey look I have my own lock now
	auto& LOCK = Thread_CV[WhatThread];
	std::mutex Thread_Lock;

	//Wait for file distributor to make me start
	std::unique_lock<std::mutex> ThreadLockReady(Thread_Lock);
	while (!Thread_Available[WhatThread]) LOCK->wait(ThreadLockReady);

	General_Pause.lock();
	std::cout << "Ended thread " << WhatThread << "\n";
	General_Pause.unlock();


	//Shutdown check
	--Num_ThreadsENDed;
	if (Num_ThreadsENDed == 0) {
		AllThreadsENDedBool = true;
		WaitForThreadsEND.notify_all();
	}
}

/// <summary>
/// Distributes folders from the front of the queue to any empty threads found in the vector of threads
/// </summary>
void FileDistributor(){

	General_Pause.lock();
	std::cout << "File distributor started!\n";
	General_Pause.unlock();

	//Pause to simulate work
	std::this_thread::sleep_for(4s);



	//This is how you "unlock" a specific condition variable
	for (int i = 0; i < Num_Threads; ++i) {
		Thread_Available[i] = true;
		auto& LOCK = Thread_CV[i];
		LOCK->notify_all();
	}


	//Shutdown check
	--Num_ThreadsENDed;
	if (Num_ThreadsENDed == 0) {
		AllThreadsENDedBool = true;
		WaitForThreadsEND.notify_all();
	}
}

