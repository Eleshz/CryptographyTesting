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



void ScannerItem(fs::path FileToScan);
void FileDistributor();

/// <summary>
/// Give this the place you want to start searching, the file i.e. "Test.txt" and the max number of threads, put 0 and it will assume based on your computer
/// </summary>
/// <param name="StartSearchLocation"></param>
/// <param name="FileToBeFound"></param>
/// <param name="NumberOfThreads"></param>
/// <returns></returns>
std::string QuickSearch(std::string StartSearchLocation, std::string FileToBeFound, int MaxAmountOfThreads){

}

/// <summary>
/// Scans a given folder
/// </summary>
/// <param name="WhatThread"></param>
void ScannerItem(fs::path FileToScan) {

	std::deque<std::filesystem::path> FileList;

}

/// <summary>
/// Makes threads to scan folders from the start of the queue
/// </summary>
void FileDistributor(){

}

