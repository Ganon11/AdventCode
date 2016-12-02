#pragma once

#include "Direction.h"

typedef std::vector<std::vector<wchar_t>> NumPad;

enum NumpadType {
   NUMPAD_NONE = 0,
   NUMPAD_SIMPLE = 1,
   NUMPAD_COMPLEX = 2,
};

class NumpadPosition {
public:
   NumpadPosition(const NumpadType type);
   ~NumpadPosition();

   wchar_t GetCurrentKey();
   void Move(const DirectionList& directions);
   std::vector<wchar_t> GetCode();
   std::wstring ToString();

private:
   void Move(const Direction d);

   NumPad m_numpad;
   size_t m_x;
   size_t m_y;
   std::vector<wchar_t> m_code;
};
