/*
	dGame README maker
	Written by Scott Atkins
	6-15-2008
*/

#include <fstream>
#include <windows.h>
using namespace std;

int main(int argc, char **argv)
{
	ofstream file;
	file.open("README.txt");

	//write README file
	file << "dGame README\n";
	file << "Written by Scott Atkins\n";

	SYSTEMTIME st;
	GetLocalTime(&st);

	file << "Archive created " << st.wMonth << "-" << st.wDay << "-" << st.wYear << " " << st.wHour << ":" << st.wMinute << endl;

	file.close();
	return 0;
}