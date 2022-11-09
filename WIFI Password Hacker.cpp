#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <stdlib.h>


using namespace std;

string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw runtime_error("_popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

vector<string> getWifiList() {
	stringstream ss(exec("netsh wlan show profile").data());
	string input;
	vector<string> wifi;
	while(getline(ss,input))
		if (input.find("All User Profile") != string::npos)
			wifi.push_back(input.substr(27,input.length()));
	return wifi;
}

string getPassword(string ssid) {
	string command = "netsh wlan show profile \"" + ssid + "\" key=clear";
	stringstream ss(exec(command.data()).data());
	string input;
	while(getline(ss,input)){
		if (input.find("Key Content") != string::npos)
			return input.substr(29,input.length());
	}
	return "< NULL >";
}

int main(int c, char *argv[])
{
	// cout << "Getting list of known wifi networks..\n\n";
	vector<string> wifi = getWifiList();

	
	// Select path 
	ofstream ofs("Pass.txt");
	for (string ssid: wifi)
	{
		ofs << "SSID\t:\t" << ssid << "\n";
		ofs << "Key\t:\t" << getPassword(ssid) << "\n\n";
	}
	ofs.close();
	// cout << "\nOuput saved to `saved-wifi-passwords.txt`..\n";

	return 0;
}
