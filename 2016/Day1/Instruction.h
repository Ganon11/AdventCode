#pragma once

#include "Orientation.h"
#include <string>

class Instruction {
public:
   Instruction(wchar_t rotation, const std::wstring& blocks);
   Instruction(Rotation rotation, int blocks);

   Rotation getRotation() const;
   int getBlocks() const;

private:
   Rotation m_rotation;
   int m_blocks;
};
