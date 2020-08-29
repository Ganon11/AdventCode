// Day9.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::wstring GetCompressedString(const std::wstring& inputFile) {
   std::wstring str;
   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return str;
   }

   do {
      std::wstring line;
      std::getline(input, line);
      str.append(line);
   } while (input.good());

   return str;
}

template <int> size_t Decompress(const std::wstring& input);

template <> size_t Decompress<1>(const std::wstring& input) {
   size_t pos{ input.find(L'(') };
   if (pos == std::wstring::npos) {
      return input.length();
   }
   else {
      std::wregex re{ L"\\((\\d+)x(\\d+)\\)" };
      std::wsmatch m;

      if (!std::regex_search(input, m, re)) {
         return 0;
      }

      std::wstring bracket{ m[0] };
      size_t size{ static_cast<size_t>(_wtoi(m[1].str().c_str())) }, repeat{ static_cast<size_t>(_wtoi(m[2].str().c_str())) };
      size_t index{ pos + bracket.length() };
      return pos + (size * repeat) + Decompress<1>(input.substr(index + size));
   }
}

template <> size_t Decompress<2>(const std::wstring& input) {
   size_t pos{ input.find(L'(') };
   if (pos == std::wstring::npos) {
      return input.length();
   }
   else {
      std::wregex re{ L"\\((\\d+)x(\\d+)\\)" };
      std::wsmatch m;

      if (!std::regex_search(input, m, re)) {
         return 0;
      }

      std::wstring bracket{ m[0] };
      size_t size{ static_cast<size_t>(_wtoi(m[1].str().c_str())) }, repeat{ static_cast<size_t>(_wtoi(m[2].str().c_str())) };
      size_t index{ pos + bracket.length() };
      return pos + (Decompress<2>(input.substr(index, size)) * repeat) + Decompress<2>(input.substr(index + size));
   }
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"sample2.txt";
   }

   std::wstring compressedText{ GetCompressedString(inputFile) };

   std::wcout << L"First: " << Decompress<1>(compressedText) << std::endl;
   std::wcout << L"Second: " << Decompress<2>(compressedText) << std::endl;

   return 0;
}
