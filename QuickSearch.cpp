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

std::mutex Pause;

int MaxThreads;

std::atomic<int> NumberOfLivingThreads;

std::deque<fs::path> FolderList;

std::string FinalPath = "0";
std::string FileToFind = "0";

void ScannerItem(std::string FileToScan);
void FileDistributor();

/// <summary>
/// Give this the place you want to start searching, the file i.e. "Test.txt" and the max number of threads, put 0 and it will assume based on your computer
/// </summary>
/// <param name="StartSearchLocation"></param>
/// <param name="FileToBeFound"></param>
/// <param name="NumberOfThreads"></param>
/// <returns></returns>
std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int MaxAmountOfThreads){

	//Don't look at this lol
	if (MaxAmountOfThreads < 1) {MaxThreads = std::ceil((std::thread::hardware_concurrency())*0.75)+1;}
	else {MaxThreads = MaxAmountOfThreads;}

	FileToFind = FileToBeFound;

	FolderList.push_front(StartSearchLocation);

	FileDistributor();

	return FinalPath;
}

/// <summary>
/// Scans a given folder
/// </summary>
/// <param name="WhatThread"></param>
void ScannerItem(std::string FileToScan) {

	std::deque<std::filesystem::path> FileList;
	std::deque<std::filesystem::path> PersonalFolderList;

	NumberOfLivingThreads++;

	std::unique_lock<std::mutex> LocalPause(Pause);
	LocalPause.unlock();

	fs::directory_iterator iter(FileToScan);

	for (const auto& file : iter) {
		try {
			if (fs::is_directory(file)) {
				PersonalFolderList.push_back(file);
			}
			else {
				FileList.push_back(file.path());
			}

		}
		catch (...) {
				std::cout << "EX,";
		}
	}

	for (auto file : FileList) {
		if (file.filename() == FileToFind) {

			LocalPause.lock();
			FinalPath = file.string();
			LocalPause.unlock();
			goto END;
		}
	}
	FileList.clear();
	LocalPause.lock();
	std::copy(PersonalFolderList.begin(), PersonalFolderList.end(), std::back_inserter(FolderList));
	LocalPause.unlock();

	END:
	NumberOfLivingThreads--;
	FileList.clear();
	return;

}

/// <summary>
/// Makes threads to scan folders from the start of the queue
/// </summary>
void FileDistributor(){

	while (true) {

		std::unique_lock<std::mutex> LocalPause (Pause);
		if (FinalPath != "0")
			break;
		LocalPause.unlock();
		LocalPause.lock();
		if (FolderList.empty())
			continue;
		try {
			if (FolderList.front().empty() || fs::is_empty(FolderList.front()) || fs::is_symlink(FolderList.front()) || !fs::exists(FolderList.front())) {
				FolderList.pop_front();
				LocalPause.unlock();
				continue;
			}
		}
		catch (...) {
			FolderList.pop_front();
			LocalPause.unlock();
			continue;
		}
		LocalPause.unlock();
		LocalPause.lock();
		if (NumberOfLivingThreads == MaxThreads) {
			LocalPause.unlock();
			continue;
		}
		else {
			std::string PathAsStringTemp = FolderList.front().string();
			std::jthread Worker(ScannerItem, PathAsStringTemp);
			Worker.detach();
			FolderList.pop_front();
		}
	}
	return;
}

