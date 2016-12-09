#include "stdafx.h"
#include "Screen.h"

#include <iostream>

Screen::Screen(const size_t rows, const size_t cols) : m_rows{ rows }, m_cols{ cols } {
   row r;
   r.resize(cols, false);
   m_screen.resize(rows, r);
}

void Screen::Print() const {
   system("cls");
   for (size_t x = 0; x < m_rows; ++x) {
      for (size_t y = 0; y < m_cols; ++y) {
         if (m_screen[x][y]) {
            std::wcout << L"#";
         }
         else {
            std::wcout << L" ";
         }
      }
      std::wcout << std::endl;
   }
}

size_t Screen::PixelCount() const {
   size_t count = 0;

   for (const auto& row : m_screen) {
      count += std::count(std::begin(row), std::end(row), true);
   }

   return count;
}

void Screen::FollowInstruction(const std::shared_ptr<Instruction> instruction) {
   switch (instruction->GetType()) {
   case InstructionType::INSTRUCTION_DRAWRECTANGLE: {
      std::shared_ptr<DrawRectangleInstruction> rectangleInstruction{ std::static_pointer_cast<DrawRectangleInstruction>(instruction) };
      DrawRectangle(rectangleInstruction->GetWidth(), rectangleInstruction->GetHeight());
      break;
   }
   case InstructionType::INSTRUCTION_ROTATEROW: {
      std::shared_ptr<RotateRowInstruction> rowInstruction{ std::static_pointer_cast<RotateRowInstruction>(instruction) };
      RotateRow(rowInstruction->GetRow(), rowInstruction->GetShift());
      break;
   }
   case InstructionType::INSTRUCTION_ROTATECOLUMN: {
      std::shared_ptr<RotateColInstruction> colInstruction{ std::static_pointer_cast<RotateColInstruction>(instruction) };
      RotateColumn(colInstruction->GetCol(), colInstruction->GetShift());
      break;
   }
   case InstructionType::INSTRUCTION_NONE:
   default:
      std::wcerr << L"Unknown instruction." << std::endl;
   }
}

void Screen::FollowInstructionList(const InstructionList& list) {
   for (const auto& instruction : list) {
      FollowInstruction(instruction);
      Print();
      Sleep(50);
   }
}

void Screen::DrawRectangle(const int width, const int height) {
   for (size_t x = 0; x < static_cast<size_t>(height); ++x) {
      for (size_t y = 0; y < static_cast<size_t>(width); ++y) {
         m_screen[x][y] = true;
      }
   }
}

void Screen::RotateRow(const int row, const int shift) {
   for (size_t a = 0; a < static_cast<size_t>(shift); ++a) {
      auto& r = m_screen[row];
      bool tmp = *(r.rbegin());
      for (size_t y = m_cols - 1; y > 0; --y) {
         r[y] = r[y - 1];
      }
      r[0] = tmp;
   }
}

void Screen::RotateColumn(const int column, const int shift) {
   for (size_t a = 0; a < static_cast<size_t>(shift); ++a) {
      bool tmp = (*m_screen.rbegin())[column];
      for (size_t x = m_rows - 1; x > 0; --x) {
         m_screen[x][column] = m_screen[x - 1][column];
      }
      m_screen[0][column] = tmp;
   }
}
