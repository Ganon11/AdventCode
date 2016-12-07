#include "stdafx.h"
#include "IPv7.h"
#include <algorithm>
#include <functional>

IPv7Address::IPv7Address(const std::wstring& address) : m_address{ address } {
   std::wstring progress{ L"" };

   for (const wchar_t c : address) {
      switch (c) {
      case L'[':
         m_supernets.push_back(progress);
         progress.clear();
         break;
      case L']':
         m_hypernets.push_back(progress);
         progress.clear();
         break;
      default:
         progress.push_back(c);
      }
   }

   if (!progress.empty()) {
      m_supernets.push_back(progress);
   }
}

namespace {
   bool ContainsABBA(const std::wstring& string) {
      size_t len{ string.length() };

      if (len < 4) {
         return false;
      }

      for (size_t i = 0; i < len - 3; ++i) {
         wchar_t a{ string[i] }, b{ string[i + 1] }, c{ string[i + 2] }, d{ string[i + 3] };
         if (a == d && b == c && a != b) {
            return true;
         }
      }

      return false;
   }
}

bool IPv7Address::SupportsTLS() const {
   std::function<bool(const std::wstring&)> fContainsAbba = [](const auto& s) { return ContainsABBA(s); };
   if (std::end(m_hypernets) != std::find_if(std::begin(m_hypernets), std::end(m_hypernets), fContainsAbba)) {
      return false;
   }

   if (std::end(m_supernets) != std::find_if(std::begin(m_supernets), std::end(m_supernets), fContainsAbba)) {
      return true;
   }

   return false;
}

namespace {
   std::vector<std::wstring> GetABAs(const std::wstring& string) {
      std::vector<std::wstring> abas;
      size_t len = string.length();

      if (len < 3) {
         return abas;
      }

      for (size_t i = 0; i < len - 2; ++i) {
         wchar_t a{ string[i] }, b{ string[i + 1] }, c{ string[i + 2] };
         if (a == c && a != b) {
            abas.push_back(std::wstring{ a, b, c });
         }
      }

      return abas;
   }

   bool ContainsBAB(const std::wstring& string, const wchar_t a, const wchar_t b) {
      return std::wstring::npos != string.find(std::wstring{ b, a, b });
   }
}

bool IPv7Address::SupportsSSL() const {
   for (const std::wstring& supernet : m_supernets) {
      std::vector<std::wstring> abas{ GetABAs(supernet) };
      for (const std::wstring& aba : abas) {
         if (std::end(m_hypernets) != std::find_if(std::begin(m_hypernets), std::end(m_hypernets), [&aba](const std::wstring& s) { return ContainsBAB(s, aba[0], aba[1]); })) {
            return true;
         }
      }
   }
   return false;
}

std::wstring IPv7Address::GetAddress() const {
   return m_address;
}
