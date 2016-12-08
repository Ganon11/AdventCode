#include "stdafx.h"
#include "Instruction.h"

#include <iostream>
#include <sstream>

Instruction::Instruction(const InstructionType type) : m_type{ type } {}

Instruction::~Instruction() {}

InstructionType Instruction::GetType() const {
   return m_type;
}

DrawRectangleInstruction::DrawRectangleInstruction(const int width, const int height)
   : Instruction{ InstructionType::RECT },
   m_width{ width }, m_height{ height }
{ }

DrawRectangleInstruction::~DrawRectangleInstruction() {}

int DrawRectangleInstruction::GetWidth() const {
   return m_width;
}

int DrawRectangleInstruction::GetHeight() const {
   return m_height;
}

RotateRowInstruction::RotateRowInstruction(const int row, const int shift)
   : Instruction{ InstructionType::ROW },
   m_row{ row }, m_shift{ shift }
{ }

RotateRowInstruction::~RotateRowInstruction() {}

int RotateRowInstruction::GetRow() const {
   return m_row;
}

int RotateRowInstruction::GetShift() const {
   return m_shift;
}

RotateColInstruction::RotateColInstruction(const int col, const int shift)
   : Instruction{ InstructionType::COLUMN },
   m_col{ col }, m_shift{ shift }
{ }

RotateColInstruction::~RotateColInstruction() {}

int RotateColInstruction::GetCol() const {
   return m_col;
}

int RotateColInstruction::GetShift() const {
   return m_shift;
}

std::shared_ptr<Instruction> MakeInstruction(const std::wstring& str) {
   std::wstringstream stream;
   std::wstring token;
   wchar_t c; // For throwaway characters
   stream << str;
   stream >> token;

   if (token == L"rect") {
      int width, height;
      stream >> width >> c >> height;
      return std::make_shared<DrawRectangleInstruction>(width, height);
   }
   else if (token == L"rotate") {
      int shift;
      stream >> token;
      if (token == L"row") {
         int row;
         stream >> c >> c >> row >> token >> shift;
         return std::make_shared<RotateRowInstruction>(row, shift);
      }
      else if (token == L"column") {
         int col;
         stream >> c >> c >> col >> token >> shift;
         return std::make_shared<RotateColInstruction>(col, shift);
      }
   }

   std::wcerr << L"Unrecognized command: " << str << std::endl;
   return nullptr;
}
