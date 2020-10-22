#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <cmath>

using namespace std;


struct Data {
	int DEC = 0;
	vector<char> BIN;
	string UNI, UTF8;
};

void dec_to_bin(int dec, vector<char>& bin);
void bin_to_hex(vector<char> bin, string& hex);
void uni_to_utf8(vector<char> bin, int dec, string& utf);

void hex_to_bin(vector<char>& bin, string hex);
int bin_to_dec(vector<char> bin);
vector<char> hex_to_bytes(const string& hex);

int main()
{

	bool condition = true;
	while (condition)
	{
		system("CLS");
		int temp;
		cout << "1. change DEC -> UTF-8" << endl;
		cout << "2. convert 368intel.txt file from CP437 to UTF-8 and print in new file" << endl;
		cout << "3. exit" << endl;
		cin >> temp;

		if (temp == 1)
		{
			system("CLS");
			cout << "HTML-code -> UNICODE -> UTF-8\n\n";
			Data data;
			cin >> data.DEC;

			dec_to_bin(data.DEC, data.BIN);
			bin_to_hex(data.BIN, data.UNI);
			uni_to_utf8(data.BIN, data.DEC, data.UTF8);

			ofstream fr("convert_result.txt");

			cout << "UNICODE: U+"; fr << "UNICODE: U+";
			for (int i = data.UNI.size(); i < 4; i++) fr << "0";
			cout << data.UNI << endl; fr << data.UNI << endl;

			cout << "UTF-8: "; fr << "UTF-8: ";
			for (int i = 0; i != data.UTF8.size(); i++) {
				cout << data.UTF8[i]; fr << data.UTF8[i];
				if ((i + 1) % 2 == 0 && i + 1 != data.UTF8.size()) { cout << " "; fr << " "; }
			}

			cout << endl; fr << endl;
			cout << "CHAR: " << char(data.DEC) << endl; fr << "CHAR: " << char(data.DEC);
			cout << "Check Your convert_result.txt\n\n";
			fr.close();

			cout << "Do you want to continue?\n";
			cout << "1. Yes\n";
			cout << "2. No\n";
			cin >> temp;
			if (temp != 1) condition = false;
		}
		else if (temp == 2)
		{
			ifstream fd("386intel.txt");
			ofstream fr("output.txt");

			Data data;
			char currentSym;

			map<int, string> CP437;
			map<int, string>::iterator it;
			int cpDec; string cpHex;

			ifstream cp("CP437.txt");
			while (cp >> cpHex >> cpDec) CP437[cpDec] = cpHex;
			cp.close();

			cout << endl << "Wait a second...";
			while (fd.get(currentSym))
			{
				data.DEC = currentSym;
				if (data.DEC < 0)
				{
					data.DEC += 256;
					it = CP437.find(data.DEC);
					if (it != CP437.end()) data.UNI = it->second;

					hex_to_bin(data.BIN, data.UNI);
					data.DEC = bin_to_dec(data.BIN);
					uni_to_utf8(data.BIN, data.DEC, data.UTF8);
					
					vector<char> bytes = hex_to_bytes(data.UTF8);

					for (auto it : bytes) fr << it;
				}
				else fr << currentSym;
			}
			fd.close();
			fr.close();

			system("CLS");
			cout << "Check Your output.txt\n\n";

			cout << "Do you want to continue?\n";
			cout << "1. Yes\n";
			cout << "2. No\n";
			cin >> temp;
			if (temp != 1) condition = false;
		}
		else {
			condition = false;
		}
	}
	return 0;
}

void dec_to_bin(int dec, vector<char>& bin)
{
	for (int i = 0; 0 < dec; i++)
	{
		char temp = dec % 2 + '0';

		bin.push_back(temp);
		dec /= 2;
	}
	if (bin.size() % 4 != 0) {
		int temp = 4 - bin.size() % 4;
		for (int i = 0; i < temp; i++) bin.push_back('0');
	}

	reverse(bin.begin(), bin.end());

	//for (auto it : bin) cout << it;
}

int bin_to_dec(vector<char> bin)
{
	int dec = 0,j=0;

	for (int i = bin.size() - 1; i >= 0; i--)
	{
		int tempBin = bin[i] - '0', tempPow;
		tempPow = (int)pow(2, j);
		dec += (tempBin * tempPow);
		j++;
	}

	return dec;
}

void bin_to_hex(vector<char> bin, string& hex)
{
	ifstream fd("bin-hex.txt");
	map<string, char> bin_hex;
	map<string, char>::iterator it;
	string b; char h;
	while (fd >> b >> h) bin_hex[b] = h;
	fd.close();

	string group;
	for (int i = 0; i != bin.size(); i++)
	{
		group.push_back(bin[i]);

		if ((i + 1) % 4 == 0) {
			it = bin_hex.find(group);
			if (it != bin_hex.end())
				hex.push_back(it->second);
			group = "";
		}
	}

	/*if (hex.size() % 4 != 0) {
		int temp = 4 - hex.size() % 4;
		for (int i = 0; i < temp; i++) hex.insert(hex.begin(), '0');
	}*/

	//for (auto it : bin_hex) cout << it.first << " " << it.second << endl;
	//for (auto it : hex) cout << it;
}

void hex_to_bin(vector<char>& bin, string hex)
{
	ifstream fd("bin-hex.txt");
	map<char, string> bin_hex;
	map<char, string>::iterator it;
	string b; char h;
	while (fd >> b >> h) bin_hex[h] = b;
	fd.close();

	bin = {};
	for (int i = 0; i != hex.size(); i++)
	{
		it = bin_hex.find(hex[i]);
		if (it != bin_hex.end())
			for (int i = 0; i != it->second.size(); i++)
				bin.push_back(it->second[i]);
	}

	//for (auto it : bin) cout << it;
}

void uni_to_utf8(vector<char> bin, int dec, string& utf)
{
	vector<char> utfbin;
	utf = {};
	if (dec < 128) { //1 byte 
		for (int i = 0; i < 8; i++)
		{
			if (i == 7) utfbin.push_back('0');
			else {
				if (bin.size() != 0) {
					utfbin.push_back(bin.back());
					bin.pop_back();
				}
				else utfbin.push_back('0');
			}
		}
	}
	else if (dec < 2048) { //2 bytes 
		for (int i = 0; i < 16; i++)
		{
			if (i == 6) utfbin.push_back('0');
			else if (i == 7) utfbin.push_back('1');
			else if (i == 13) utfbin.push_back('0');
			else if (i == 14) utfbin.push_back('1');
			else if (i == 15) utfbin.push_back('1');
			else {
				if (bin.size() != 0) {
					utfbin.push_back(bin.back());
					bin.pop_back();
				}
				else utfbin.push_back('0');
			}
		}
	}
	else if (dec < 65536) { //3 bytes 
		for (int i = 0; i < 24; i++)
		{
			if (i == 6) utfbin.push_back('0');
			else if (i == 7) utfbin.push_back('1');
			else if (i == 14) utfbin.push_back('0');
			else if (i == 15) utfbin.push_back('1');
			else if (i == 20) utfbin.push_back('0');
			else if (i == 21) utfbin.push_back('1');
			else if (i == 22) utfbin.push_back('1');
			else if (i == 23) utfbin.push_back('1');

			else {
				if (bin.size() != 0) {
					utfbin.push_back(bin.back());
					bin.pop_back();
				}
				else utfbin.push_back('0');
			}
		}
	}
	else if (dec < 1114112) { //4 bytes
		for (int i = 0; i < 32; i++)
		{
			if (i == 6) utfbin.push_back('0');
			else if (i == 7) utfbin.push_back('1');
			else if (i == 14) utfbin.push_back('0');
			else if (i == 15) utfbin.push_back('1');
			else if (i == 22) utfbin.push_back('0');
			else if (i == 23) utfbin.push_back('1');
			else if (i == 27) utfbin.push_back('0');
			else if (i == 28) utfbin.push_back('1');
			else if (i == 29) utfbin.push_back('1');
			else if (i == 30) utfbin.push_back('1');
			else if (i == 31) utfbin.push_back('1');

			else {
				if (bin.size() != 0) {
					utfbin.push_back(bin.back());
					bin.pop_back();
				}
				else utfbin.push_back('0');
			}
		}
	}

	reverse(utfbin.begin(), utfbin.end());
	//for (auto it : utfbin) cout << it;

	bin_to_hex(utfbin, utf);
}

vector<char> hex_to_bytes(const string& hex) {
	vector<char> bytes;

	for (unsigned int i = 0; i < hex.length(); i += 2) {
		string byteString = hex.substr(i, 2);
		char byte = (char)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}

	return bytes;
}