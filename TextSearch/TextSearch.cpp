// TextSearch.cpp : Defines the entry point for the console application.
//

#include "TextSearch.h"
#include "INIReader.h"

std::string dir;
std::string resultPath;

void(*funcs[])() = { Search, Entry, Settings, nullptr };

int main()
{
	Settings();
	while (Jump(Menu()));
    return 0;
}

int Jump(const char* rs)
{
	char code = rs[0];
	if (code == 'Q' || code == 'q') return 0;

	int funcLenght = 0;
	while (funcs[funcLenght] != NULL) funcLenght++;

	//ASCII numeral to int
	int i = (int)code - '0';
	//Zero-based list
	i--;
	if (i < 0 || i >= funcLenght) {
		printf("Invalid choise\n");
		return 1;
	}
	else
	{
		funcs[i]();
		return 1;
	}
}

void Search() 
{
	std::ofstream rf(resultPath);
	std::string searchTerm = "";

	rf.clear();
	printf("Search for: ");
	fflush(stdout);
	std::getline(std::cin, searchTerm);
	ListFiles(dir, searchTerm, rf);
	rf.close();
	printf("Result for: %s\n", searchTerm.c_str());

	//String to wchar
	wchar_t pathW[100];
	std::copy(resultPath.c_str(), resultPath.c_str() + lstrlenA(resultPath.c_str()) + 1, pathW);

	ShellExecute(0, 0, pathW, 0, 0, SW_SHOW);
}

void Entry()
{
	std::string fileName = "";
	std::string filePath = "";
	
	printf("New file: ");
	fflush(stdout);
	std::getline(std::cin, fileName);

	filePath = dir + "\\" + fileName + ".txt";

	//Wchar out of filename and the path
	wchar_t pathW[100];
	std::copy(filePath.c_str(), filePath.c_str() + lstrlenA(filePath.c_str()) + 1, pathW);
	//Make the file
	std::ofstream nF(dir + "\\" + fileName + ".txt");
	nF.close();

	ShellExecute(0, 0, pathW, 0, 0, SW_SHOW);
}

void Settings()
{
	INIReader reader("..//config.ini");
	if (reader.ParseError() < 0) {
		printf("Cannot load the config-file\n");
		return;
	}
	dir = reader.Get("Settings", "directory", "UNKNOWN");
	resultPath = dir + "\\result.txt";
	printf("Settings refreshed\n");
}

const char* Menu()
{
	printf("Choose an option:\n");
	printf("1. Search\n");
	printf("2. New entry\n");
	printf("3. Settings\n");
	printf("Q. Quit\n");
	printf(">> ");

	fflush(stdout);
	const int buffersize = 16;
	static char response[buffersize];
	fgets(response, buffersize, stdin);

	return response;
}

void ListFiles(std::string directory, std::string searchterm, std::ofstream &resultFile)
{
	std::stringstream buffer;
	for (auto &p : fs::directory_iterator(directory))
	{
		//Path
		buffer << p;
		printf("%s\n", buffer.str().c_str());
		if (buffer.str() == resultPath) { buffer.str(""); continue; }
		if (ReadContent(buffer.str(), searchterm)) { WriteResult(buffer.str(), resultFile); }
		buffer.str("");
	}
}

bool ReadContent(std::string path, std::string searchterm)
{
	std::transform(searchterm.begin(), searchterm.end(), searchterm.begin(), ::toupper);
	std::ifstream file;
	std::string s;
	int matchesMax = searchterm.length();
	int matches = 0;

	//Check the path
	for (char &c : path)
	{
		//to uppercase
		std::transform(path.begin(), path.end(), path.begin(), ::toupper);

		if (c == searchterm.c_str()[matches]) { matches++; }
		else { matches = 0; }
		if (matches == matchesMax)
		{
			file.close();
			return true;
		}
	}

	matches = 0;

	//Check the content
	file.open(path);
	if (!file)
	{
		std::cerr << "Unable to open file " << path << std::endl;
		return false;
	}

	while (std::getline(file, s)) 
	{
		//to uppercase
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);

		for (char &c : s)
		{
			if (c == searchterm.c_str()[matches]) { matches++; }
			else { matches = 0; }
			//printf("%c", c);
			if (matches == matchesMax)
			{
				file.close();
				//printf("%s\n", path.c_str());
				//printf("%s\n", s.c_str());
				return true;
			}
		}
	}
	file.close();
	return false;
}

void WriteResult(std::string path, std::ofstream &resultFile) 
{
	std::ifstream file;
	std::string s;

	resultFile << path << std::endl;

	file.open(path);
	if (!file)
	{
		std::cerr << "Unable to open file " << path << std::endl;
		return;
	}

	while (std::getline(file, s))
	{
		resultFile << s << std::endl;
	}
	resultFile << "--------------------------------------------------------------" << std::endl;
	file.close();
}