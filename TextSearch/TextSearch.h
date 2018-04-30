#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <Shlwapi.h>
namespace fs = std::experimental::filesystem;

const char* Menu();

int Jump(const char*);

void Search();
void Entry();
void Settings();

void ListFiles(std::string, std::string, std::ofstream &);
bool ReadContent(std::string, std::string);
void WriteResult(std::string, std::ofstream &);