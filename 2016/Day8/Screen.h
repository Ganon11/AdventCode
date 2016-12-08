#pragma once

#include "Instruction.h"

#include <vector>

class Screen {
public:
   Screen(const size_t rows, const size_t cols);

   void Print() const;
   size_t PixelCount() const;
   void FollowInstruction(const std::shared_ptr<Instruction> instruction);
   void FollowInstructionList(const InstructionList& list);

private:
   void DrawRectangle(const int width, const int height);
   void RotateRow(const int row, const int shift);
   void RotateColumn(const int column, const int shift);

   size_t m_rows;
   size_t m_cols;
   typedef std::vector<bool> row;
   std::vector<row> m_screen;
};
