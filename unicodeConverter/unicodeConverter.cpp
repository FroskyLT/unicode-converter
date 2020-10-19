#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>

using namespace std;

struct Data {
	int DEC = 0;
	vector<char> BIN;
	string UNI, UTF8;
};

void dec_to_bin(int dec, vector<char>& bin);
void bin_to_hex(vector<char> bin, string& hex);
void uni_to_utf8(vector<char> bin, int dec, string& utf);

int main()
{
	cout << "HTML-code -> UNICODE -> UTF-8\n\n";
	Data data;
	cin >> data.DEC;

	dec_to_bin(data.DEC, data.BIN);
	bin_to_hex(data.BIN, data.UNI);
	uni_to_utf8(data.BIN, data.DEC, data.UTF8);

	cout << "UNICODE: U+";
	for (int i = data.UNI.size(); i < 4; i++) cout << "0";
	cout << data.UNI << endl;
	cout << "UTF-8: ";
	for (int i = 0; i != data.UTF8.size(); i++) {
		cout << data.UTF8[i];
		if ((i + 1) % 2 == 0 && i + 1 != data.UTF8.size()) cout << " ";
	}
	/*for (int i = 0; i != BIN.size(); i++)
	{
		cout << BIN[i];
		if ((i + 1) % 4 == 0) cout << " ";
	}*/
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

void uni_to_utf8(vector<char> bin, int dec, string& utf)
{
	vector<char> utfbin;

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
			if		(i == 6) utfbin.push_back('0');
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
			if		(i == 6) utfbin.push_back('0');
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
			if		(i == 6) utfbin.push_back('0');
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
