#pragma once

//    Grotopia Bot
//    Copyright (C) 2018  Growtopia Noobs
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "json.hpp"
#include <sstream>
#include <vector>
#include <limits>
#include <math.h>
#include <string>
#include <iostream>
#include <regex>
#include <iterator>
#include <curl/curl.h>
#include <algorithm>
#include "utilsfunc.h"
#include "corefunc.h"
#include "userfunc.h"
#include "enet/include/enet.h"

//#define WORLD_GO

using namespace std;



char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string hexStr(unsigned char data)
{
	std::string s(2, ' ');
	s[0] = hexmap[(data & 0xF0) >> 4];
	s[1] = hexmap[data & 0x0F];
	return s;
}

string generateMeta()
{
	string x;
	for (int i = 0; i < 9; i++)
	{
		x += hexStr(rand());
	}
	x += ".com";
	return x;
}

string generateMac()
{
	string x;
	for (int i = 0; i < 6; i++)
	{
		x += hexStr(rand());
		if (i != 5)
			x += ":";
	}
	return x;
}

string generateRid()
{
	string x;
	for (int i = 0; i < 16; i++)
	{
		x += hexStr(rand());
	}
	for (auto & c : x) c = toupper(c);
	return x;
}

string stripMessage(string msg) {
	regex e("\\x60[a-zA-Z0-9!@#$%^&*()_+\\-=\\[\\]\\{};':\"\\\\|,.<>\\/?]");
	string result = regex_replace(msg, e, "");
	result.erase(std::remove(result.begin(), result.end(), '`'), result.end());
	return result;
}

// Fungsi untuk menangkap data dari API response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Fungsi untuk mengirim permintaan GET ke API
std::string SendGetRequest(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Nonaktifkan SSL verification (opsional)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Ikuti redirect

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    return response;
}

// Fungsi untuk melakukan translasi menggunakan Google Translate API
std::string Translate(const std::string& text, const std::string& targetLang, const std::string& sourceLang = "auto") {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        // URL untuk LibreTranslate API
        std::string url = "https://libretranslate.com/translate";

        // Data JSON untuk POST
        std::string postData = "{"
            "\"q\": \"" + text + "\","
            "\"source\": \"" + sourceLang + "\","
            "\"target\": \"" + targetLang + "\","
            "\"format\": \"text\","
            "\"alternatives\": 3,"
            "\"api_key\": \"\""
        "}";

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Mengatur opsi cURL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Melakukan request
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return "Error: Failed to fetch translation.";
        }
    } else {
        return "Error: Failed to initialize cURL.";
    }

    return response;
}


void GrowtopiaBot::onLoginRequested()
{
    cout << "Logging on..." << endl;

    // Data login bot
    string ver = gameVersion;
    string hash = std::to_string((unsigned int)rand());
    string hash2 = std::to_string((unsigned int)rand());
    
    // Format paket login sesuai permintaan
string packet = 
"tankIDName|" + uname + "\n"
"tankIDPass|" + upass + "\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|5\n"
"GDPR|2\n"
"category|GROWMINES_3586\n"
"totalPlaytime|0\n"
"klv|90EE392D2F9D8BC997EEA41175FCD9DA\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_tXaWXoSUrGdcuETl\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|516902427\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";


string packet2 =
"tankIDName|BOBSQUISH3\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|5\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D18\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_kFvwRUxwkrlqhrQi\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|1201591342\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet3 =
"tankIDName|BOBSQUISH4\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet4 =
"tankIDName|BOBSQUISH5\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet5 =
"tankIDName|BOBSQUISH6\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";
	
string packet6 =
"tankIDName|BOBSQUISH7\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";	

string packet7 =
"tankIDName|BOBSQUISH8\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet8 =
"tankIDName|BOBSQUISH9\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet9 =
"tankIDName|BOBSQUISH10\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";	

string packet10 =
"tankIDName|BOBSQUISH11\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet11 =
"tankIDName|BOBSQUISH12\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet12 =
"tankIDName|BOBSQUISH13\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet13 =
"tankIDName|BOBSQUISH14\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet14 =
"tankIDName|BOBSQUISH15\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet15 =
"tankIDName|BOBSQUISH16\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet16 =
"tankIDName|BOBSQUISH17\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet17 =
"tankIDName|BOBSQUISH18\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet18 =
"tankIDName|BOBSQUISH19\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";

string packet19 =
"tankIDName|BOBSQUISH20\n"
"tankIDPass|12345678\n"
"requestedName|\n"
"f|1\n"
"protocol|212\n"
"game_version|5.02\n"
"cbits|0\n"
"player_age|6\n"
"GDPR|2\n"
"category|_-5100\n"
"totalPlaytime|0\n"
"klv|E75EE937333B83F25BC7A48FDBF86D19\n"
"gid|" + generateRid() + "\n"
"tr|4322\n"
"meta|InfinityPS_iuWsyLJhgeDoxTIA\n"
"fhash|-716928004\n"
"rid|" + generateRid() + "\n"
"platformID|4\n"
"deviceVersion|0\n"
"country|id\n"
"hash|1275125224\n"
"mac|" + generateMac() + "\n"
"wk|NONE0\n"
"lmode|1\n"
"user|-405210819\n"
"UUIDToken|-1\n"
"doorID|0\n"
"aat|2\n";



    // Cetak isi paket untuk debugging
    cout << packet;
//	sleep(3);
//	cout << packet2;
	///sleep(3);
	//cout << packet3;

    // Kirim paket login
// SendPacket(2, "protocol|212\nltoken|X3Rva2VuPWRHRnVhMGxFVG1GdFpYeENUMEpUVVZWSlUwZ0tkR0Z1YTBsRVVHRnpjM3d4TWpNME5UWTNPQXB5WlhGMVpYTjBaV1JPWVcxbGZBcG1mREVLY0hKdmRHOWpiMng4TWpFeUNtZGhiV1ZmZG1WeWMybHZibncxTGpBeUNtTmlhWFJ6ZkRFd01qUUtjR3hoZVdWeVgyRm5aWHcyQ2tkRVVGSjhNZ3BqWVhSbFoyOXllWHhIVWs5WFRVbE9SVk5mTXpVNE5ncDBiM1JoYkZCc1lYbDBhVzFsZkRBS2EyeDJmRGt3UlVVek9USkVNa1k1UkRoQ1F6azVOMFZGUVRReE1UYzFSa05FT1VSQkNtZHBaSHhoTTJObE5qZzNZeTFoTlRBMUxUUXhaamd0WWpOa09DMWhPVFEyTTJZek0yWXpPVEFLZEhKOE5ETXlNZ3B0WlhSaGZFbHVabWx1YVhSNVVGTmZTSHBwWm10NlJuVnJSbGx1ZG01YVNncG1hR0Z6YUh3dE56RTJPVEk0TURBMENuSnBaSHd3TWpFMU5UVXdRVGd4T0RZNE5UZEVNRGczTkRjME1UZzNRVVJEUkVSQlJncHdiR0YwWm05eWJVbEVmRFFLWkdWMmFXTmxWbVZ5YzJsdmJud3dDbU52ZFc1MGNubDhhV1FLYUdGemFId3hNamMxTVRJMU1qSTBDbTFoWTN3d01qb3dNRG93TURvd01Eb3dNRG93TUFwM2EzeE9UMDVGTUFvPSZncm93SWQ9Qk9CU1FVSVNIJnBhc3N3b3JkPTEyMzQ1Njc4\nplatformID|4", peer);
   // SendPacket(2, "protocol|212\nltoken|X3Rva2VuPWRHRnVhMGxFVG1GdFpYeENUMEpUVVZWSlUwZ3pDblJoYm10SlJGQmhjM044TVRJek5EVTJOemdLY21WeGRXVnpkR1ZrVG1GdFpYd0tabnd4Q25CeWIzUnZZMjlzZkRJeE1ncG5ZVzFsWDNabGNuTnBiMjU4TlM0d01ncGpZbWwwYzN3d0NuQnNZWGxsY2w5aFoyVjhOUXBIUkZCU2ZESUtZMkYwWldkdmNubDhYeTAxTVRBd0NuUnZkR0ZzVUd4aGVYUnBiV1Y4TUFwcmJIWjhSVGMxUlVVNU16Y3pNek5DT0ROR01qVkNRemRCTkRoR1JFSkdPRFpFTVRnS1oybGtmR0V6WTJVMk9EZGpMV0UxTURVdE5ERm1PQzFpTTJRNExXRTVORFl6WmpNelpqTTVNQXAwY253ME16SXlDbTFsZEdGOFNXNW1hVzVwZEhsUVUxOXJSblozVWxWNGQydHliSEZvY2xGcENtWm9ZWE5vZkMwM01UWTVNamd3TURRS2NtbGtmREF5TVVNNE56SkNRelZFTWtZNE5USXdPVGMxTVROQ1F6WXlSREpGTmpFNENuQnNZWFJtYjNKdFNVUjhOQXBrWlhacFkyVldaWEp6YVc5dWZEQUtZMjkxYm5SeWVYeHBaQXBvWVhOb2ZERXlOelV4TWpVeU1qUUtiV0ZqZkRBeU9qQXdPakF3T2pBd09qQXdPakF3Q25kcmZFNVBUa1V3Q2c9PSZncm93SWQ9Qk9CU1FVSVNIMyZwYXNzd29yZD0xMjM0NTY3OA==\nplatformID|4", peer);


//SendPacket(2, "protocol|212\nltoken|GUEST_ACCOUNT\nplatformID|4", currentPeer);
    SendPacket(2, packet, peer);
    SendPacket(2, packet2, peer2);
      SendPacket(2, packet3, peer3);
    /*  SendPacket(2, packet4, peer4);
    SendPacket(2, packet5, peer5);
      SendPacket(2, packet6, peer6);
      SendPacket(2, packet7, peer7);
    SendPacket(2, packet8, peer8);
      SendPacket(2, packet9, peer9);
      SendPacket(2, packet10, peer10);
      SendPacket(2, packet11, peer11);
      SendPacket(2, packet12, peer12);
      SendPacket(2, packet13, peer13);
      SendPacket(2, packet14, peer14);
          SendPacket(2, packet15, peer15);
    */
	currentWorld = "";
}


void GrowtopiaBot::packet_type3(string text)
{
	dbgPrint("Some text is here: " + text);
	if (text.find("LOGON ATTEMPTS") != string::npos)
	{
		cout << "Wrong username / password!. (LOGON ATTEMPTS)";
	}
	if (text.find("password is wrong") != string::npos)
	{
		cout << "Wrong password!";
	}
	if (text.find("action|logon_fail") != string::npos)
	{
		connectClient();
		objects.clear();
		currentWorld = "";
	}
}

void GrowtopiaBot::packet_type6(string text)
{
	//dbgPrint("Some text is here: " + text);
	SendPacket(2, "action|enter_game\n", peer);
	enet_host_flush(client);
}

void GrowtopiaBot::packet_unknown(ENetPacket* packet)
{
	dbgPrint("Got unknown packet type: " + std::to_string(GetMessageTypeFromPacket(packet)));
	dbgPrint("Packet size is " + std::to_string(packet->dataLength));
}



void GrowtopiaBot::OnSendToServer(string address, int port, int userId, string token)
{
    // Pisahkan token dengan pemisah "|"
    std::vector<std::string> parts;
    std::stringstream ss(token);
    std::string item;

    while (std::getline(ss, item, '|'))
    {
        parts.push_back(item);
    }

    // Jika ada setidaknya 2 bagian, ambil bagian awal dan akhir
    if (parts.size() >= 2)
    {
        login_token = parts[0] + "|" + parts.back(); // Simpan langsung ke login_token
    }

    // Simpan userId
    login_user = userId;
}

void GrowtopiaBot::OnConsoleMessage(string message) {
    cout << "Found console message: " << endl;
    string strippedMessage = stripMessage(message);
    cout << strippedMessage << endl;

if (strippedMessage.find("Stats for this node:") != std::string::npos) {
    std::regex playerRegex(R"(([0-9]+) players)");
    std::regex pcRegex(R"(\(([0-9]+) PC)");
    std::regex androidRegex(R"(([0-9]+) Android)");
    std::regex iosRegex(R"(([0-9]+) iOS)");
    std::regex maxPlayersRegex(R"(Max-Players: ([0-9]+)/([0-9]+))");
  std::regex worldsActiveRegex(R"(([0-9]+) Worlds active)");
    std::regex serverLoadRegex(R"(Server Load: ([0-9]+))");

    std::smatch match;

    // Extract player data
    if (std::regex_search(strippedMessage, match, playerRegex)) {
        totalPlayers = std::stoi(match[1].str());
    }
    if (std::regex_search(strippedMessage, match, pcRegex)) {
        pcPlayers = std::stoi(match[1].str());
    }
    if (std::regex_search(strippedMessage, match, androidRegex)) {
        androidPlayers = std::stoi(match[1].str());
    }
    if (std::regex_search(strippedMessage, match, iosRegex)) {
        iosPlayers = std::stoi(match[1].str());
    }

    // Extract max players
        if (std::regex_search(strippedMessage, match, maxPlayersRegex)) {
        maxPlayersCurrent = std::stoi(match[1].str());
        maxPlayersLimit = std::stoi(match[2].str());
    }

    // Extract worlds active
    if (std::regex_search(strippedMessage, match, worldsActiveRegex)) {
        worldsActive = std::stoi(match[1].str());
    }

    // Extract server load
    if (std::regex_search(strippedMessage, match, serverLoadRegex)) {
        serverLoad = std::stoi(match[1].str());
    }

    // Create the updated stats message
    std::string onlineStats = "Online Players: " + std::to_string(totalPlayers) +
                              " (PC: " + std::to_string(pcPlayers) +
                              ", Android: " + std::to_string(androidPlayers) +
                              ", iOS: " + std::to_string(iosPlayers) + ")" +
                              " Max Players: " + std::to_string(maxPlayersCurrent) + "/" + std::to_string(maxPlayersLimit) +
                              ", Worlds Active: " + std::to_string(worldsActive) +
                              ", Server Load: " + std::to_string(serverLoad);

    // Send the data to the players
    SendPacket(2, "action|input\n|text|" + onlineStats, peer);
   
    /* Uncomment and repeat for additional peers
    SendPacket(2, "action|input\n|text|" + onlineStats, peer2);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer3);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer4);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer5);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer6);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer7);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer8);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer9);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer10);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer11);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer12);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer13);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer14);
    SendPacket(2, "action|input\n|text|" + onlineStats, peer15);
    */
}


    if (strippedMessage.find("MSG") != std::string::npos) {
        cout << "Found message!" << endl;
    }
    
  /*  if (message.find("`` left, `w") != string::npos)
	{
		string::size_type loc = message.find("`` left,", 0);
		SendPacket(2, "action|input\n|text|" + colorstr2(message.substr(3, loc) + " just left"), peer);
	}
	else if (message.find("`` entered, `w") != string::npos)
	{
		string::size_type loc = message.find("`` entered,", 0);
		SendPacket(2, "action|input\n|text|" + colorstr2(message.substr(3, loc) + " just joined"), peer);
	}
   */ 
    cout << "------------------------" << endl;
}



void GrowtopiaBot::OnPlayPositioned(string sound)
{

}

void GrowtopiaBot::OnSetFreezeState(int state)
{

}



void GrowtopiaBot::OnRemove(string data) // "netID|x\n"
{
	std::stringstream ss(data.c_str());
	std::string to;
	int netID = -1;
	while (std::getline(ss, to, '\n')) {
		string id = to.substr(0, to.find("|"));
		string act = to.substr(to.find("|") + 1, to.length() - to.find("|"));
		if (id == "netID")
		{
			netID = atoi(act.c_str());
		}
		else {
			dbgPrint(id + "!!!!!!!!!!!" + act);
		}
	}
	for (ObjectData& objectData : objects)
	{
		if (objectData.netId == netID)
		{
			objectData.isGone = true;
		}
	}
	//SendPacket(2, "action|input\n|text|Bye bye ::((", peer);
}

void GrowtopiaBot::OnSpawn(string data)
{
	std::stringstream ss(data.c_str());
	std::string to;
	cout << data;
	ObjectData objectData;
	bool actuallyOwner = false;
	bool nameProcessed = false; // Flag untuk melacak apakah 'name' telah diproses
    int bufferedNetID = -1; // Buffer untuk menyimpan netID sementara
 

	while (std::getline(ss, to, '\n')) {
		string id = to.substr(0, to.find("|"));
		string act = to.substr(to.find("|") + 1, to.length() - to.find("|") - 1);
		if (id == "country")
		{
			objectData.country = act;
		}
      else if (id == "name") {
    string strippedMessage = stripMessage(act);  
    if (strippedMessage == ownerUsername) {
        actuallyOwner = true;
        nameProcessed = true;  // Tandai bahwa 'name' telah diproses
        std::cout << "Owner detected: " << ownerUsername << " (" << strippedMessage << ")\n";
        
        objectData.name = strippedMessage;
        // Jika ada netID yang sudah terbuffer sebelumnya, proses sekarang
        if (bufferedNetID != -1) {
            std::cout << "Processing buffered netID: " << bufferedNetID << std::endl;
            owner = bufferedNetID;
            objectData.netId = bufferedNetID;
            std::cout << "SUCCESS: netID updated to " << objectData.netId << "\n";
            bufferedNetID = -1; // Reset bufferedNetID
        }
    } else {
        std::cout << "Owner not detected, strippedMessage: " << strippedMessage << std::endl;
    }
}

else if (id == "netID") {
    std::cout << "Received netID: " << act << std::endl;
    
    // Jika 'name' belum diproses, simpan netID terlebih dahulu
    if (!nameProcessed) {
        bufferedNetID = std::stoi(act); // Simpan netID untuk diproses nanti
        std::cout << "Buffered netID: " << bufferedNetID << std::endl;
    } else {
        // Jika 'name' sudah diproses, langsung proses netID
        int netID = std::stoi(act);       
        objectData.netId = netID;
        owner = netID;
        std::cout << "SUCCESS: netID updated to " << owner << "\n";
    }
}
		else if (id == "userID")
		{
			objectData.userId = atoi(act.c_str());
		}
		else if (id == "posXY")
		{
			int x = atoi(act.substr(0, to.find("|")).c_str());
			int y = atoi(act.substr(act.find("|") + 1, act.length() - act.find("|") - 1).c_str());
			objectData.x = x;
			objectData.y = y;
		}
		else if (id == "type")
		{
			if (act == "local")
				objectData.isLocal = true;
		}
		else if (act != "0" && (id == "invis" || id == "mstate" || id == "smstate"))
		{
			cout << "Some fishy boy is here: " << objectData.name << "; " << objectData.country << "; " << objectData.userId << "; " << objectData.netId << "; " << endl;
			objectData.isMod = true;
		}
	}

	if (actuallyOwner) cout << "Owner netID has been updated to " << objectData.netId << " username is " << ownerUsername << "Actuallyowner " << actuallyOwner;
	objects.push_back(objectData);
//	SendPacket(2, "action|input\n|text|`3Hello " + objectData.name + " `3with id " + objectData.netId + " from " + objectData.country + "!", peer);
}

void GrowtopiaBot::OnAction(string command)
{
	SendPacket(2, "action|input\n|text|Why do you "+command.substr(1, command.length())+"?", peer);
}

void GrowtopiaBot::SetHasGrowID(int state, string name, string password)
{

}

void GrowtopiaBot::SetHasAccountSecured(int state)
{

}



void GrowtopiaBot::OnTalkBubble(int netID, string bubbleText, int type, int number)
{
	std::cout << "Received netID: " << netID << std::endl;  // Cek nilai netID di sini
	if (netID != owner) {
   return;
        }
    
    if (bubbleText.find("!weather") != string::npos) {
        std::string location = bubbleText.substr(bubbleText.find("!weather") + 9); // Ambil lokasi setelah "!weather "
        if (location.empty()) {
            SendPacket(2, "action|input\n|text|Silakan masukkan nama lokasi. Contoh: !weather Jakarta", peer);
        } else {
            // URL API
            std::string apiKey = "060a6bcfa19809c2cd4d97a212b19273"; // API key Anda
            std::string apiUrl = "https://api.openweathermap.org/data/2.5/weather?q=" + location + "&units=metric&appid=" + apiKey + "&lang=id";

            // Mengirim permintaan ke API
            std::string response = SendGetRequest(apiUrl);

            if (response.find("\"cod\":200") != std::string::npos) { // Jika permintaan berhasil
                // Parsing data JSON manual
                size_t namePos = response.find("\"name\":\"");
                size_t weatherPos = response.find("\"main\":\"");
                size_t descPos = response.find("\"description\":\"");
                size_t tempPos = response.find("\"temp\":");
                size_t feelsLikePos = response.find("\"feels_like\":");
                size_t pressurePos = response.find("\"pressure\":");
                size_t humidityPos = response.find("\"humidity\":");
                size_t windSpeedPos = response.find("\"speed\":");
                size_t lonPos = response.find("\"lon\":");
                size_t latPos = response.find("\"lat\":");
                size_t countryPos = response.find("\"country\":\"");

                // Mengambil data dari JSON
                std::string cityName = response.substr(namePos + 8, response.find("\"", namePos + 8) - (namePos + 8));
                std::string weatherMain = response.substr(weatherPos + 8, response.find("\"", weatherPos + 8) - (weatherPos + 8));
                std::string weatherDesc = response.substr(descPos + 15, response.find("\"", descPos + 15) - (descPos + 15));
                float temp = std::stof(response.substr(tempPos + 7, response.find(",", tempPos) - (tempPos + 7)));
                float feelsLike = std::stof(response.substr(feelsLikePos + 14, response.find(",", feelsLikePos) - (feelsLikePos + 14)));
                int pressure = std::stoi(response.substr(pressurePos + 11, response.find(",", pressurePos) - (pressurePos + 11)));
                int humidity = std::stoi(response.substr(humidityPos + 11, response.find(",", humidityPos) - (humidityPos + 11)));
                float windSpeed = std::stof(response.substr(windSpeedPos + 8, response.find(",", windSpeedPos) - (windSpeedPos + 8)));
                float lon = std::stof(response.substr(lonPos + 6, response.find(",", lonPos) - (lonPos + 6)));
                float lat = std::stof(response.substr(latPos + 6, response.find(",", latPos) - (latPos + 6)));
                std::string country = response.substr(countryPos + 11, response.find("\"", countryPos + 11) - (countryPos + 11));

                // Mengirim pesan cuaca dalam bahasa Indonesia
                std::string weatherMessage = 
                 "*Cuaca:* " + weatherMain + ", " +
                "*Deskripsi:* " + weatherDesc + ", " +
                "*Suhu:* " + std::to_string(temp) + " °C, " +
                "*Terasa Seperti:* " + std::to_string(feelsLike) + " °C, " +
                "*Tekanan:* " + std::to_string(pressure) + " hPa, " +
                "*Kelembapan:* " + std::to_string(humidity) + "%, " +
               "*Negara:* " + country;

                // Mengirimkan hasil ke chat
                SendPacket(2, "action|input\n|text|" + weatherMessage, peer);
            } else {
                SendPacket(2, "action|input\n|text|Gagal mengambil data cuaca untuk lokasi " + location + ". Periksa kembali nama lokasi Anda.", peer);
            }
        }
    }
    
	cout << bubbleText << endl;
	if (bubbleText.find("!pos") != string::npos)
	{
		for (ObjectData x : objects)
		{
			if (owner == x.netId)
			{
				SendPacket(2, "action|input\n|text|Owner pos is " + 
           std::to_string(static_cast<int>(x.x)) + ":" + 
           std::to_string(static_cast<int>(x.y)) + ".", peer);
           SendPacket(2, "action|input\n|text|Owner pos is " + 
           std::to_string(static_cast<int>(x.x)) + ":" + 
           std::to_string(static_cast<int>(x.y)) + ".", peer2);
           SendPacket(2, "action|input\n|text|Owner pos is " + 
           std::to_string(static_cast<int>(x.x)) + ":" + 
           std::to_string(static_cast<int>(x.y)) + ".", peer3);
        }
    }
    }

    // Perintah untuk mencari lokasi dari IP
    if (bubbleText.find("!ipfind") != std::string::npos) {
        std::string ip = bubbleText.substr(bubbleText.find("!ipfind") + 8);
        
        if (ip.empty()) {
            SendPacket(2, "action|input\n|text|Silakan masukkan IP yang ingin dicari. Contoh: !ipfind 61.5.127.161", peer);
        } else {
            SendPacket(2, "action|input\n|text|Sedang mencari lokasi IP...", peer);
            sleep(3); // Delay 3 detik agar terlihat seperti sedang memproses

            std::string apiKey = "1191EDF97D2E991728D8E10E8C364730"; // API Key kamu
            std::string apiUrl = "https://api.ip2location.io/?key=" + apiKey + "&ip=" + ip;
            std::string response = SendGetRequest(apiUrl);

            if (response.find("\"country_name\"") != std::string::npos) {
                // Parsing manual JSON
                size_t countryPos = response.find("\"country_name\":\"");
                size_t regionPos = response.find("\"region_name\":\"");
                size_t cityPos = response.find("\"city_name\":\"");
                size_t latPos = response.find("\"latitude\":");
                size_t lonPos = response.find("\"longitude\":");

                std::string country = response.substr(countryPos + 15, response.find("\"", countryPos + 15) - (countryPos + 15));
                std::string region = response.substr(regionPos + 14, response.find("\"", regionPos + 14) - (regionPos + 14));
                std::string city = response.substr(cityPos + 12, response.find("\"", cityPos + 12) - (cityPos + 12));
                float lat = std::stof(response.substr(latPos + 10, response.find(",", latPos) - (latPos + 10)));
                float lon = std::stof(response.substr(lonPos + 11, response.find(",", lonPos) - (lonPos + 11)));

                // Pesan hasil pencarian
                std::string ipMessage =
                    " *IP:* " + ip + ", " +
                    " *Negara:* " + country + ", " +
                    "*Wilayah:* " + region + ", " +
                    "*Kota:* " + city + ", " +
                    "*Koordinat:* " + std::to_string(lat) + ", " + std::to_string(lon);

                SendPacket(2, "action|input\n|text|" + ipMessage, peer);
            } else {
                SendPacket(2, "action|input\n|text| Gagal menemukan lokasi untuk IP " + ip, peer);
            }
        }
    }

	if (bubbleText.find("!playercount") != string::npos)
	{
		int i=0;
		for (ObjectData x : objects)
		{
			if (!x.isGone)
			{
				i++;
			}
		}
		SendPacket(2, "action|input\n|text|There is " + std::to_string(i)+ " players.", peer);
	}


	if (bubbleText.find("!owner") != string::npos)
	{	
		for (ObjectData x : objects)
		{
			if (owner == x.netId)
			{
				SendPacket(2, "action|input\n|text|Owner is " + x.name + ".", peer);
				owner = netID;
			}
		}
	}
	if (bubbleText.find("!respawn") != string::npos)
	{
		respawn();
	}
	if (bubbleText.find("!serverinfo") != std::string::npos)
    {
        if (!login_token.empty())
        {
            // Kirim informasi server yang disimpan
            SendPacket(2, "action|input\n|text|`2Server Info IP And Port`0: " + login_token, peer);
        }
        else
        {
            // Jika belum ada informasi server, berikan pesan error
            SendPacket(2, "action|input\n|text|Server info not available.", peer);
        }
    }
	if (bubbleText.find("player_chat= ") != string::npos) {
    // Ambil string setelah "!packet "
		if (netID != owner) {
             return;
		}
    string packetr = bubbleText.substr(bubbleText.find("player_chat= ") + 13);

    // Kirim paket setelah penggantian
    SendPacket(2, "action|input\n|text|" + packetr, peer);
	}
	if (bubbleText.find("!packet ") != string::npos) {
    // Ambil string setelah "!packet "
    string packetr = bubbleText.substr(bubbleText.find("!packet ") + 8);

    // Ganti karakter '[' dengan '|'
    size_t pos = 0;
    while ((pos = packetr.find('[', pos)) != string::npos) {
        packetr.replace(pos, 1, "|");  // Ganti "[" dengan "|"
        pos++;  // Lanjutkan pencarian setelah posisi yang diganti
    }

    // Kirim paket setelah penggantian
    SendPacket(2, packetr, peer);
}
if (bubbleText.find("!online") != std::string::npos) {
        // Kirim perintah /stats ke server
        SendPacket(2, "action|input\n|text|/stats", peer);
    }

	if (bubbleText.find("!nfollow") != string::npos)
	{
		isFollowed = true;
		SendPacket(2, "action|input\n|text|Netid Follow .", peer);

	}
	if (bubbleText.find("!follow") != string::npos)
	{
		isFollowing = true;
	}
	
	if (bubbleText.find("!netid ") != string::npos) {
        string numberStr = bubbleText.substr(bubbleText.find("!netid ") + 7);
        try {
            number = std::stoi(numberStr); // Konversi string ke integer dan simpan ke 'number'
            std::cout << "Parsed netID: " << number << std::endl;

            // Jika ingin menyimpan dalam kelas, update variabel 'number' pada objek GrowtopiaBot
            this->number = number;  // Menyimpan hasil ke properti 'number' di dalam kelas

        } catch (const std::invalid_argument& e) {
            SendPacket(2, "Invalid input for number: " + numberStr, peer);
        } catch (const std::out_of_range& e) {
            SendPacket(2, "NUMBER OUT OF RANGE: " + numberStr, peer);
        }
    }
	if (bubbleText.find("!broadcast") != std::string::npos) 
	{
        cout << "Broadcasting message to all players!" << endl;

        // Pesan yang akan dibroadcast
        string message = "Hello, all players! This is a broadcast message.";

        // Buat paket yang berisi pesan
            ENetPacket* packet = enet_packet_create(NULL, 0, ENET_PACKET_FLAG_RELIABLE);     
       cout << "Broadcast message sent!" << endl;
	}
	if (bubbleText.find("!spam") != std::string::npos)
	{
        cout << "Spam ENetEvent RECEIVE triggered!" << endl;

        // Ambil jumlah spam dari bubbleText setelah "!spam"
        string spamCountStr = bubbleText.substr(bubbleText.find("!spam") + 6); // Ambil bagian setelah "!spam"
        int spamCount = 0;

        try {
            spamCount = std::stoi(spamCountStr); // Coba mengonversi ke integer
        } catch (std::invalid_argument& e) {
            cout << "Invalid spam count, using default 10." << endl;
            spamCount = 10; // Default spam count jika input tidak valid
	}

        // Mulai spam event
        for (int i = 0; i < spamCount; ++i) { // Loop untuk spam (berdasarkan jumlah yang diminta)
            ENetEvent fakeEvent;
            fakeEvent.type = ENET_EVENT_TYPE_RECEIVE; // Set tipe event ke RECEIVE
            fakeEvent.peer = this->peer; // Assign peer bot
            fakeEvent.packet = enet_packet_create("Fake Packet Data", strlen("Fake Packet Data") + 1, ENET_PACKET_FLAG_RELIABLE);

            // Handle event (proses di dalam fungsi eventLoop atau handler)
            enet_host_broadcast(this->client, 0, fakeEvent.packet);
            cout << "Spam event #" << i + 1 << " sent." << endl;

            // Tambahkan delay jika perlu agar tidak terlalu cepat
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        cout << "Spam selesai!" << endl;
	}
	if (bubbleText.find("!nstop") != string::npos)
	{
		isFollowed = false;
	}
	if (bubbleText.find("!stop") != string::npos)
	{
		isFollowing = false;
	}
	if (bubbleText.find("!up") != string::npos) {
        MoveBotRaw(0, -1); // Gerak ke atas
    } else if (bubbleText.find("!down") != string::npos) {
        MoveBotRaw(0, 1); // Gerak ke bawah
    } else if (bubbleText.find("!left") != string::npos) {
        MoveBotRaw(-1, 0); // Gerak ke kiri
    } else if (bubbleText.find("!right") != string::npos) {
        MoveBotRaw(1, 0); // Gerak ke kanan
    }
    if (bubbleText.find("!test") != string::npos) {
        ActivateInvisEffect();
   }
    if (bubbleText.find("!nazi") != string::npos) {
   InvisibleEffect(6);
    }
	
	
if (bubbleText.find("!go ") != string::npos)
	{
SendPacket(3, "action|quit_to_exit", peer);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer);
     SendPacket(3, "action|quit_to_exit", peer2);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer2);
    SendPacket(3, "action|quit_to_exit", peer3);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer3);
         SendPacket(3, "action|quit_to_exit", peer4);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer4);
     SendPacket(3, "action|quit_to_exit", peer5);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer5);
    SendPacket(3, "action|quit_to_exit", peer6);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer6);
         SendPacket(3, "action|quit_to_exit", peer7);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer7);
    SendPacket(3, "action|quit_to_exit", peer8);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer8);
    SendPacket(3, "action|quit_to_exit", peer9);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer9);
         SendPacket(3, "action|quit_to_exit", peer10);
		SendPacket(3, "action|join_request\nname|" + bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go ")), peer10);
         
  worldName = bubbleText.substr(bubbleText.find("!go ") + 4, bubbleText.length() - bubbleText.find("!go "));
	}
	if (bubbleText.find("!onmsg") != std::string::npos) {
        if (!spamMsgEnabled) {
            spamMsgEnabled = true;
            std::thread(&GrowtopiaBot::msgloop, this).detach(); // Jalankan msgloop() di thread baru
            SendPacket(2, "action|input\n|text|Spam message diaktifkan!", peer);
        } else {
            SendPacket(2, "action|input\n|text|Spam message sudah aktif.", peer);
        }
    }
    if (bubbleText.find("!set ") != std::string::npos) {
      string name = bubbleText.substr(bubbleText.find("!set ") + 4, bubbleText.length() - bubbleText.find("!set "));
    }

    // Perintah untuk menonaktifkan spam pesan otomatis
    if (bubbleText.find("!offmsg") != std::string::npos) {
        spamMsgEnabled = false;
        SendPacket(2, "action|input\n|text|Spam message dimatikan!", peer);
    }
    
	if (bubbleText.find("!dance") != string::npos)
	{
		SendPacket(2, "action|input\n|text|/dance", peer);
	}
	
	if (bubbleText.find("!translate") != std::string::npos) {
        std::cout << "Translate command received!" << std::endl;

        // Ambil bagian setelah "!translate" untuk mendapatkan perintah
        std::string command = bubbleText.substr(10); // Mengambil teks setelah "!translate "

        // Pisahkan perintah untuk mendapatkan bahasa target dan teks
        std::stringstream ss(command);
        std::string targetLang;
        std::getline(ss, targetLang, ' '); // Ambil bahasa target sebelum spasi

        // Ambil teks yang akan diterjemahkan setelah bahasa target
        std::string textToTranslate = command.substr(targetLang.length() + 1);

        if (targetLang.empty() || textToTranslate.empty()) {
            // Jika tidak ada bahasa target atau teks untuk diterjemahkan
            SendPacket(2, "action|input\n|text|Format salah! Gunakan format: !translate <target_language> <text>", peer);
        } else {
            // Menerjemahkan teks menggunakan API
            std::string translatedText = Translate(textToTranslate, targetLang);

            // Mengirim pesan terjemahan ke pemain yang melakukan perintah
            std::string translationMessage = "Terjemahan: " + translatedText;
            SendPacket(2, "action|input\n|text|" + translationMessage, peer);

            std::cout << "Translation message sent!" << std::endl;
        }
    }
        
	if (bubbleText.find("!spk ") != string::npos)
	{
		SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer2);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer3);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer4);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer5);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer6);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer7);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer8);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer9);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer10);
	SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer11);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer12);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer13);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer14);
SendPacket(2, "action|input\n|text|" + bubbleText.substr(bubbleText.find("!spk ") + 5, bubbleText.length() - bubbleText.find("!spk ")), peer15);

}

	
	if (bubbleText.find("!about") != string::npos || bubbleText.find("!help") != string::npos)
	{
		SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer2);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer3);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer4);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer5);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer6);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer7);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer8);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer9);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer10);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer11);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer12);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer13);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer14);
SendPacket(2, "action|input\n|text|This is bot from Growtopia Noobs. Modified BY BOBSQUISH", peer15);
	}
}



void GrowtopiaBot::SetRespawnPos(int respawnPos)
{
	respawnX = respawnPos % 100; // hacky!!! TODO: get from world data (100)
	respawnY = respawnPos / 100; // hacky!!! TODO: get from world data (100)
}

void GrowtopiaBot::OnEmoticonDataChanged(int val1, string emoticons)
{

}

void GrowtopiaBot::OnSetPos(float x, float y)
{

}

void GrowtopiaBot::OnAddNotification(string image, string message, string audio, int val1) // You will see that when banned :(
{

}

/*

void GrowtopiaBot::OnFailedToEnterWorld()
{

}

void GrowtopiaBot::OnSuperMainStartAcceptLogonFB211131ddf(int val1, string domain, string folder, string deniedApps, string settings)
{

}

void GrowtopiaBot::OnSetBux(int val1, int val2, int val3) // missing params
{

}

void GrowtopiaBot::OnZoomCamera(float val1, int val2)
{

}

void GrowtopiaBot::OnGuildDataChanged(int val1, int val2, int val3, int val4)
{

}

void GrowtopiaBot::OnFactionDataChanged(int val1, int val2, int val3, string data)
{

}

void GrowtopiaBot::OnSetClothing(int hair, int shirt, int pants, int feet, int face, int hand, int back, int mask, int necklace, int val1, int val2, int val3, int val4)
{

}*/

void GrowtopiaBot::AtApplyTileDamage(int x, int y, int damge)
{
	cout << "Damage " + std::to_string(damge) + " applied at X:" + std::to_string(x) + " Y: " + std::to_string(y) << endl;
}

void GrowtopiaBot::AtApplyLock(int x, int y, int itemId) // ApplyLockFromGamePacket TODO: *(int*)(data + 4)
{
	SendPacket(2, "action|input\n|text|Lock " + std::to_string(itemId) + " applied at X:" + std::to_string(x) + " Y: " + std::to_string(y), peer);
}

void GrowtopiaBot::AtPlayerMoving(PlayerMoving* data)
{
    int object = -1;
    
    // Loop through all objects to find the corresponding object by netID
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects.at(i).netId == data->netID)
        {
            object = i;
        }
    }

    if (object != -1)
    {
        // Update object coordinates
        objects[object].x = data->x;
        objects[object].y = data->y;
            objects[object].punchX = data->punchX;
            objects[object].punchY = data->punchY;
        
    }

    // Check if we are following the player (owner) and additional conditions
    if (isFollowing && data->netID == owner && data->plantingTree == 0)
    {
        // Handle backward walking and character state changes
        if (backwardWalk)
            data->characterState ^= 0x10;

        // Check for respawn conditions
        if ((data->characterState & 0x800) && (data->characterState & 0x100)) {
            SendPacket(2, "action|respawn", peer);
SendPacket(2, "action|respawn", peer2);
SendPacket(2, "action|respawn", peer3);
SendPacket(2, "action|respawn", peer4);
SendPacket(2, "action|respawn", peer5);
SendPacket(2, "action|respawn", peer6);
SendPacket(2, "action|respawn", peer7);
SendPacket(2, "action|respawn", peer8);
SendPacket(2, "action|respawn", peer9);
SendPacket(2, "action|respawn", peer10);
SendPacket(2, "action|respawn", peer11);
SendPacket(2, "action|respawn", peer12);
SendPacket(2, "action|respawn", peer13);
SendPacket(2, "action|respawn", peer14);
SendPacket(2, "action|respawn", peer15);

        }

        // Update local objects position
        for (int i = 0; i < objects.size(); i++)
        {
            if (objects.at(i).isLocal)
            {
                objects.at(i).x = data->x;
                objects.at(i).y = data->y;
    
                    objects.at(i).punchX = data->punchX;
                    objects.at(i).punchY = data->punchY;
                
            }
     }

        // Send movement data, including punch position if available
        SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer2, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer3, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer4, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer5, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer6, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer7, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer8, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer9, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer10, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer11, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer12, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer13, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer14, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer15, ENET_PACKET_FLAG_RELIABLE);
}
	if (isFollowed && data->netID == number && data->punchX == -1 && data->punchY == -1 && data->plantingTree == 0) // <--- bypass - can get banned from character state!!!, replacing isnt enought
	{
		if (backwardWalk)
			data->characterState ^= 0x10;
		if ((data->characterState & 0x800) && (data->characterState & 0x100)) {
			SendPacket(2, "action|respawn", peer);
SendPacket(2, "action|respawn", peer2);
SendPacket(2, "action|respawn", peer3);
SendPacket(2, "action|respawn", peer4);
SendPacket(2, "action|respawn", peer5);
SendPacket(2, "action|respawn", peer6);
SendPacket(2, "action|respawn", peer7);
SendPacket(2, "action|respawn", peer8);
SendPacket(2, "action|respawn", peer9);
SendPacket(2, "action|respawn", peer10);
SendPacket(2, "action|respawn", peer11);
SendPacket(2, "action|respawn", peer12);
SendPacket(2, "action|respawn", peer13);
SendPacket(2, "action|respawn", peer14);
SendPacket(2, "action|respawn", peer15);
	}
		for (int i = 0; i < objects.size(); i++)
			if (objects.at(i).isLocal) {
				objects.at(i).x = data->x;
				objects.at(i).y = data->y;
			}
		SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer2, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer3, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer4, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer5, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer6, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer7, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer8, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer9, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer10, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer11, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer12, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer13, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer14, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer15, ENET_PACKET_FLAG_RELIABLE);
	}
}




void GrowtopiaBot::InvisibleEffect(int count) {
    // Daftar offset acak untuk menciptakan efek di sekitar bot
    std::vector<int> randomOffsets{ 32, 64, -32, -64, 0, 0 };

    // Cari objek bot lokal
    for (int i = 0; i < objects.size(); i++) {
        if (objects.at(i).isLocal) {
            PlayerMoving data;
                 for (int j = 0; j < count; j++) {
                // Tentukan posisi acak di sekitar bot
                data.x = objects.at(i).x + 16 + randomOffsets[rand() % randomOffsets.size()];
                data.y = objects.at(i).y + 16 + randomOffsets[rand() % randomOffsets.size()];

                // Kirim paket mentah untuk efek
                SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer2, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer3, ENET_PACKET_FLAG_RELIABLE);
					SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer4, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer5, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer6, ENET_PACKET_FLAG_RELIABLE);
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer7, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer8, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer9, ENET_PACKET_FLAG_RELIABLE);
		SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer10, ENET_PACKET_FLAG_RELIABLE);
					   // Debugging
                std::cout << "Effect spawned at X: " << data.x / 32 << ", Y: " << data.y / 32 << std::endl;
            }
            break;
        }
    }
}

void GrowtopiaBot::MoveBotRandom(int repeatCount, int offsetX, int offsetY) {
    // Ambil objek bot lokal
    for (int i = 0; i < objects.size(); i++) {
        if (objects.at(i).isLocal) {
            // Dapatkan posisi awal bot
            int startX = objects.at(i).x;
            int startY = objects.at(i).y;

            // Loop untuk mengirimkan gerakan raw sebanyak repeatCount
            for (int j = 0; j < repeatCount; j++) {
                // Buat pergerakan acak dengan offset
                int randomX = startX + offsetX + (rand() % 32);
                int randomY = startY + offsetY + (rand() % 32);

                // Buat paket `PLAYER_MOVING`
                PlayerMoving data;
                data.packetType = 17;
                data.characterState = 0; // Sesuaikan jika diperlukan
                data.x = randomX;
                data.y = randomY;
                data.punchX = -1;
                data.punchY = -1;

                // Kirim paket mentah
                SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer2, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer3, ENET_PACKET_FLAG_RELIABLE);
					SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer4, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer5, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer6, ENET_PACKET_FLAG_RELIABLE);
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer7, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer8, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer9, ENET_PACKET_FLAG_RELIABLE);
		SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer10, ENET_PACKET_FLAG_RELIABLE);
					
                // Debugging
                std::cout << "Sent raw movement to X: " << data.x / 32 << ", Y: " << data.y / 32 << " (" << j + 1 << "/" << repeatCount << ")" << std::endl;
            }

            break; // Bot lokal sudah ditemukan, hentikan loop
        }
    }
}

void GrowtopiaBot::ActivateInvisEffect() {
    // Vektor untuk nilai acak
    std::vector<int> random_ = {32, 64, -32, -64, 0, 0};

    // Ambil objek bot lokal
    for (int i = 0; i < objects.size(); i++) {
        if (objects.at(i).isLocal) {
            for (int j = 0; j < 6; j++) {
                // Generate koordinat acak
                int randomX = random_[rand() % random_.size()];
                int randomY = random_[rand() % random_.size()];

                // Buat paket `PLAYER_MOVING`
                PlayerMoving data;
                data.characterState = 0; // State karakter, bisa diatur sesuai
                data.x = objects.at(i).x + 16 + randomX;
                data.y = objects.at(i).y + 16 + randomY;
                data.punchX = 50;
                data.punchY = 40;

                // Paket mentah
                
                // Kirim paket mentah
                SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer2, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer3, ENET_PACKET_FLAG_RELIABLE);
					SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer4, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer5, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer6, ENET_PACKET_FLAG_RELIABLE);
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer7, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer8, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer9, ENET_PACKET_FLAG_RELIABLE);
		SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer10, ENET_PACKET_FLAG_RELIABLE);
						
                // Debugging
                std::cout << "Invis effect at X: " << data.x / 32 << ", Y: " << data.y / 32 << std::endl;
            }
            break;
        }
    }
}




void GrowtopiaBot::MoveBotRaw(int deltaX, int deltaY) {
    // Ambil objek bot lokal
    for (int i = 0; i < objects.size(); i++) {
        if (objects.at(i).isLocal) {
            // Perbarui posisi X dan Y
            objects.at(i).x += deltaX * 32; // 32 adalah skala posisi di server Growtopia
            objects.at(i).y += deltaY * 32;

            // Buat paket `PLAYER_MOVING`
            PlayerMoving data;
            
            data.characterState = 0; // Sesuaikan jika diperlukan
            data.x = objects.at(i).x;
            data.y = objects.at(i).y;
            data.punchX = -1;
            data.punchY = -1;
            // Kirim paket mentah
            SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer2, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer3, ENET_PACKET_FLAG_RELIABLE);
					SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer4, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer5, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer6, ENET_PACKET_FLAG_RELIABLE);
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer7, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer8, ENET_PACKET_FLAG_RELIABLE);
SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer9, ENET_PACKET_FLAG_RELIABLE);
		SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer10, ENET_PACKET_FLAG_RELIABLE);
					
            // Debugging
            std::cout << "Bot moved to X: " << data.x / 32 << ", Y: " << data.y / 32 << std::endl;
            break;
        }
    }
}


void GrowtopiaBot::AtAvatarSetIconState(int netID, int state) // if player goes BRB, etc...
{

}

void GrowtopiaBot::WhenConnected()
{
	cout << "Connected to server!" << endl;
}

void GrowtopiaBot::WhenDisconnected()
{
	cout << "Disconnected from server..." << endl;
	connectClient();
}

int counter = 0; // 10ms per step

vector<string> explode(const string &delimiter, const string &str)
{
	vector<string> arr;

	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return arr;//no change

	int i = 0;
	int k = 0;
	while (i < strleng)
	{
		int j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}

void GrowtopiaBot::userLoop()
{
	bool succes = false;
    if (timeFromWorldEnter > 200 && currentWorld != worldName)
    {
        if (worldName == "" || worldName == "-")
        {
            timeFromWorldEnter = 0;
            
        }
        else
        {
        	
            // Mengirim permintaan bergabung ke semua peer
            SendPacket(3, "action|join_request\nname|" + worldName, peer); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer2); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer3); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer4); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer5); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer6); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer7); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer8); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer9); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer10); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer11); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer12); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer13); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer14); // MARRKS
SendPacket(3, "action|join_request\nname|" + worldName, peer15); // MARRKS
cout << currentWorld << "; " << worldName << endl;
            objects.clear();
        }
        timeFromWorldEnter=0;
    }
    timeFromWorldEnter++;
	counter++;
	if ((counter % 1800) == 0)
	{
		string name = "";
		float distance = std::numeric_limits<float>::infinity();
		float ownerX;
		float ownerY;
		for (ObjectData x : objects)
		{
			if (x.netId == owner)
			{
				ownerX=x.x;
				ownerY=x.y;
			}
		}
		if (owner != -1)
		{
            for (ObjectData x : objects)
			{
				if (((x.x - ownerX)*(x.x - ownerX)) + ((x.y - ownerY)*(x.y - ownerY)) < distance && x.netId != owner && !x.isGone)
				{
					distance = ((x.x - ownerX)*(x.x - ownerX)) + ((x.y - ownerY)*(x.y - ownerY)); // just dont calculate squere root = faster
               name = x.name;
				}
                if(x.netId==owner && x.isGone)
                    goto NO_OWNER_MESSAGE;
			}
			if (distance == std::numeric_limits<float>::infinity())
			{
				SendPacket(2, "action|input\n|text|There are no other players:(", peer);
			}
			else {
				SendPacket(2, "action|input\n|text|Closest player is " + name + " with distance " + std::to_string(sqrt(distance)), peer);
            }
		}
	}
    NO_OWNER_MESSAGE:
    return;
}



void GrowtopiaBot::msgloop() {
    while (spamMsgEnabled) {  
        string msg = "Message From Bot Only tested";

        // Pastikan peer valid sebelum mengirim pesan
        if (peer) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer);
        if (peer2) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer2);
        if (peer3) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer3);
        if (peer4) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer4);
        if (peer5) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer5);
        if (peer6) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer6);
        if (peer7) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer7);
        if (peer8) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer8);
        if (peer9) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer9);
        if (peer10) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer10);
        if (peer11) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer11);
        if (peer12) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer12);
        if (peer13) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer13);
        if (peer14) SendPacket(2, "action|input\n|text|/msg " + name + " " + colorstr2(msg), peer14);

        // Gunakan delay lebih lama agar tidak dianggap spam
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay 1 detik
    }
}


void GrowtopiaBot::userInit() {
	connectClient();
	
	cout << flush;
}

void GrowtopiaBot::respawn()
{
	PlayerMoving data;
	data.characterState = 0x924; // animation
	SendPacket(2, "action|respawn", peer); // respawn request
SendPacket(2, "action|respawn", peer2);
    SendPacket(2, "action|respawn", peer3); // respawn request
 
	for (int i = 0; i < objects.size(); i++)
		if (objects.at(i).isLocal)
		{
			data.x = objects.at(i).x;
			data.y = objects.at(i).y;
			data.netID = objects.at(i).netId;
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer2, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer3, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer4, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer5, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer6, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer7, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer8, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer9, ENET_PACKET_FLAG_RELIABLE);
  SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer10, ENET_PACKET_FLAG_RELIABLE);
  
	cout << "Send" << endl;
			break;
		}
}
