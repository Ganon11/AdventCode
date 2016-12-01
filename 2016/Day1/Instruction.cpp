#include "stdafx.h"

#include "Instruction.h"

Instruction::Instruction(wchar_t rotation, const std::wstring& blocks) : Instruction(GetRotation(rotation), _wtoi(blocks.c_str())) {}

Instruction::Instruction(Rotation rotation, int blocks) : m_rotation{ rotation }, m_blocks{ blocks } {}

Rotation Instruction::getRotation() const {
   return m_rotation;
}

int Instruction::getBlocks() const {
   return m_blocks;
}
