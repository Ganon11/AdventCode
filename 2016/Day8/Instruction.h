#pragma once

#include <memory>
#include <string>
#include <vector>

enum InstructionType {
   INSTRUCTION_NONE = 0,
   INSTRUCTION_DRAWRECTANGLE = 1,
   INSTRUCTION_ROTATEROW = 2,
   INSTRUCTION_ROTATECOLUMN = 3
};

class Instruction abstract {
public:
   Instruction(const InstructionType type);
   virtual ~Instruction();

   InstructionType GetType() const;

private:
   InstructionType m_type;
};

class DrawRectangleInstruction : public Instruction {
public:
   DrawRectangleInstruction(const int width, const int height);
   virtual ~DrawRectangleInstruction();

   int GetWidth() const;
   int GetHeight() const;

private:
   int m_width;
   int m_height;
};

class RotateRowInstruction : public Instruction {
public:
   RotateRowInstruction(const int row, const int shift);
   virtual ~RotateRowInstruction();

   int GetRow() const;
   int GetShift() const;

private:
   int m_row;
   int m_shift;
};

class RotateColInstruction : public Instruction {
public:
   RotateColInstruction(const int col, const int shift);
   virtual ~RotateColInstruction();

   int GetCol() const;
   int GetShift() const;

private:
   int m_col;
   int m_shift;
};

std::shared_ptr<Instruction> MakeInstruction(const std::wstring& str);

typedef std::vector<std::shared_ptr<Instruction>> InstructionList;
