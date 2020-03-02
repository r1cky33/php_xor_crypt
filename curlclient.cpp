#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <Windows.h>

#define CURL_STATICLIB
#pragma warning(disable:4996)

#include "../curl/curl.h"

#ifdef _DEBUG
#pragma comment (lib, "../curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "../curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")

#include "../curl/curl.h"

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

std::string XOR(std::string value, std::string key)
{
	std::string retval(value);
	long unsigned int klen = key.length();
	long unsigned int vlen = value.length();
	unsigned long int k = 0;
	unsigned long int v = 0;
	for (; v < vlen; v++) {
		retval[v] = value[v] ^ key[k];
		k = (++k < klen ? k : 0);
	}
	return retval;
}

#include <cassert>
std::string hex2bin(std::string const& s) {
	assert(s.length() % 2 == 0);

	std::string sOut;
	sOut.reserve(s.length() / 2);

	std::string extract;
	for (std::string::const_iterator pos = s.begin(); pos < s.end(); pos += 2)
	{
		extract.assign(pos, pos + 2);
		sOut.push_back(std::stoi(extract, nullptr, 16));
	}
	return sOut;
}

int main(void)
{
	auto curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/api/api.php?id=r1cky33&pw=123&type=1");
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
		curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

		std::string response_string;
		std::string header_string;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

		char* url;
		long response_code;
		double elapsed;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl = NULL;

		//"decryption"
		std::string bin = hex2bin(response_string);
		std::string decrypted = XOR(bin, "mysecretkey");

		char* data = (char*)response_string.c_str();

		int j;

		std::cout << data;
		
		std::cin >> j;
	}
}
