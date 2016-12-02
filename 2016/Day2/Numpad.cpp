#include "stdafx.h"
#include "Numpad.h"

const NumPad SIMPLE_NUMPAD = {
   { L'1', L'2', L'3' },
   { L'4', L'5', L'6' },
   { L'7', L'8', L'9' }
};

const NumPad COMPLEX_NUMPAD = {
   { 0,    0,    L'1',    0,    0 },
   { 0,    L'2', L'3', L'4',    0 },
   { L'5', L'6', L'7', L'8', L'9' },
   { 0,    L'A', L'B', L'C',    0 },
   { 0,    0,    L'D',    0,    0 },
};

NumpadPosition::NumpadPosition(const NumpadType type) {
   switch (type) {
   case NUMPAD_SIMPLE:
      m_numpad = SIMPLE_NUMPAD;
      m_x = 1;
      m_y = 1;
      break;
   case NUMPAD_COMPLEX:
      m_numpad = COMPLEX_NUMPAD;
      m_x = 0;
      m_y = 2;
      break;
   }
}

NumpadPosition::~NumpadPosition() {
}

wchar_t NumpadPosition::GetCurrentKey() {
   return m_numpad[m_x][m_y];
}

void NumpadPosition::Move(const DirectionList& directions) {
   for (const Direction d : directions) {
      Move(d);
   }

   m_code.push_back(GetCurrentKey());
}

std::vector<wchar_t> NumpadPosition::GetCode() {
   return m_code;
}

std::wstring NumpadPosition::ToString() {
   return std::wstring{ m_code.data(), m_code.size() };
}

void NumpadPosition::Move(const Direction d) {
   switch (d) {
   case DIRECTION_UP:
      if (m_x > 0 && m_numpad[m_x - 1][m_y] != L'\0') --m_x;
      break;
   case DIRECTION_DOWN:
      if (m_x < m_numpad.size() - 1 && m_numpad[m_x + 1][m_y] != L'\0') ++m_x;
      break;
   case DIRECTION_LEFT:
      if (m_y > 0 && m_numpad[m_x][m_y - 1] != L'\0') --m_y;
      break;
   case DIRECTION_RIGHT:
      if (m_y < m_numpad[0].size() - 1 && m_numpad[m_x][m_y + 1] != L'\0') ++m_y;
      break;
   }
}
