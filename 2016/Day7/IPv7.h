#pragma once

#include <string>
#include <vector>

class IPv7Address {
public:
   IPv7Address(const std::wstring& address);

   bool SupportsTLS() const;
   bool SupportsSSL() const;

   std::wstring GetAddress() const;

private:
   std::wstring m_address;
   std::vector<std::wstring> m_hypernets;
   std::vector<std::wstring> m_supernets;
};
