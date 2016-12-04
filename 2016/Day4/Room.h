#pragma once

#include <string>
#include <vector>

class Room {
public:
   Room(const std::wstring& input);
   
   bool IsValid() const;
   int GetSectorId() const;
   std::wstring GetEncryptedName() const;
   std::wstring GetDecryptedName() const;

private:
   std::wstring m_name;
   int m_sectorID;
   std::vector<wchar_t> m_checksum;
};
