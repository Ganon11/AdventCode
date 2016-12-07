// Day7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IPv7.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<IPv7Address> GetAddresses(const std::wstring& inputFile) {
   std::vector<IPv7Address> addresses;

   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return addresses;
   }

   do {
      std::wstring line;
      input >> line;
      addresses.push_back(IPv7Address(line));
   } while (input.good());

   return addresses;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Sample.txt";
   }

   auto addresses = GetAddresses(inputFile);
   auto tlsTotal = std::count_if(std::begin(addresses), std::end(addresses), [](const IPv7Address& a) { return a.SupportsTLS(); });
   auto sslTotal = std::count_if(std::begin(addresses), std::end(addresses), [](const IPv7Address& a) { return a.SupportsSSL(); });

   std::wcout << tlsTotal << L" addresses support TLS, and " << sslTotal << L" addresses support SSL." << std::endl;
   return 0;
}
