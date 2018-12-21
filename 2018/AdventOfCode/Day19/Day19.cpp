// Day19.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"
#include "Instruction.h"

unsigned int get_instruction_pointer(const std::wstring& line);
void print_registers(const std::vector<int>& registers);
bool execute_instruction(std::vector<int>& registers, const std::vector<Instruction>& program,
    unsigned int& instruction_pointer, const unsigned int ip_register);

int wmain(int argc, wchar_t* argv[]) {
  advent_of_code::InputHandler input{ argc, argv };
  std::vector<std::wstring> lines{ input.read_all_lines() };
  std::vector<Instruction> program;
  program.reserve(lines.size() - 1); // First line is instruction pointer binding
  unsigned int ip_register{ get_instruction_pointer(lines[0]) };

  for (size_t index = 1; index < lines.size(); ++index) {
    program.push_back(lines[index]);
  }

  std::vector<int> registers = { 13813247, 0, 0, 0, 0, 0 };
  unsigned int instruction_pointer{ 0 };
  while (execute_instruction(registers, program, instruction_pointer, ip_register)) {
    ++instruction_pointer;
  }

  print_registers(registers);
  std::wcout << std::endl << L"Program has halted - registers[0] = " << registers[0] << std::endl;

  /*registers = { 1, 0, 0, 0, 0, 0 };
  instruction_pointer = 0;
  while (execute_instruction(registers, program, instruction_pointer, ip_register)) {
    ++instruction_pointer;
  }

  std::wcout << L"Program has halted - registers[0] = " << registers[0] << std::endl;*/

  return 0;
}

unsigned int get_instruction_pointer(const std::wstring& line) {
  std::wregex ip_regex{ L"#ip (\\d)" };
  std::wsmatch matches;

  if (!std::regex_match(line, matches, ip_regex) || matches.size() != 2) {
    return std::numeric_limits<unsigned int>::max();
  }

  return static_cast<unsigned int>(_wtoi(matches[1].str().c_str()));
}

void print_registers(const std::vector<int>& registers) {
  std::wcout << L'[';

  for (size_t index = 0; index < 6; ++index) {
    if (index > 0) {
      std::wcout << L", ";
    }
    std::wcout << registers[index];
  }

  std::wcout << L']';
}

std::set<unsigned long long> register_5_values_seen;

bool execute_instruction(std::vector<int>& registers, const std::vector<Instruction>& program,
    unsigned int& instruction_pointer, const unsigned int ip_register) {

  if (instruction_pointer < 0 || instruction_pointer >= program.size()) {
    return false;
  }

  registers[ip_register] = instruction_pointer;
  if (28 == instruction_pointer) {
    auto insertion_result{ register_5_values_seen.insert(registers[5]) };
    if (insertion_result.second) {
      std::wcout << L"Register 5 == " << registers[5] << std::endl;
    }
  }

  program[instruction_pointer].execute(registers);

  instruction_pointer = registers[ip_register];

  return true;
}
