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

bool AllThreadsStartedBool = false;
bool AllThreadsENDedBool = false;

std::string FinalFileLocation = "0";

std::mutex AllThreadsHaveStarted;
std::mutex AllThreadsHaveENDed;

std::condition_variable WaitForThreads;
std::condition_variable WaitForThreadsEND;

std::deque<std::filesystem::path> FolderList;
std::vector<std::filesystem::path> FolderDistribution;
std::vector<bool> Thread_Available;
std::vector<std::thread> ThisThread;

std::string FileToBeFound_Thread;

void ScannerItem(int WhatThread);
void FileDistributor();

std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int NumberOfThreads){

}

void ScannerItem(int WhatThread) {

}

void FileDistributor() {

}

