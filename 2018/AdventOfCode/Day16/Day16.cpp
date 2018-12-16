// Day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "../AoCHelpers/InputHandler.h"

std::vector<int> guess_addr(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_addi(std::vector<int> registers, int A, int B, int C);

std::vector<int> guess_mulr(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_muli(std::vector<int> registers, int A, int B, int C);

std::vector<int> guess_banr(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_bani(std::vector<int> registers, int A, int B, int C);

std::vector<int> guess_borr(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_bori(std::vector<int> registers, int A, int B, int C);

std::vector<int> guess_setr(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_seti(std::vector<int> registers, int A, int B, int C);

std::vector<int> guess_gtir(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_gtri(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_gtrr(std::vector<int> registers, int A, int B, int C);

std::vector<int> guess_eqir(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_eqri(std::vector<int> registers, int A, int B, int C);
std::vector<int> guess_eqrr(std::vector<int> registers, int A, int B, int C);

enum Opcode {
   ADDR,
   ADDI,
   MULR,
   MULI,
   BANR,
   BANI,
   BORR,
   BORI,
   SETR,
   SETI,
   GTIR,
   GTRI,
   GTRR,
   EQIR,
   EQRI,
   EQRR
};

std::wstring OpcodeToString(const Opcode c);

typedef std::set<Opcode> OpcodeSet;

typedef std::map<int, OpcodeSet> OpcodeGuesses;

OpcodeSet check_all_instructions(const std::vector<int>& before,
                                           const std::vector<int>& expected,
                                           const std::vector<int>& instruction);

typedef std::map<int, Opcode> OpcodeLookup;

void do_addr(std::vector<int>& registers, int A, int B, int C);
void do_addi(std::vector<int>& registers, int A, int B, int C);

void do_mulr(std::vector<int>& registers, int A, int B, int C);
void do_muli(std::vector<int>& registers, int A, int B, int C);

void do_banr(std::vector<int>& registers, int A, int B, int C);
void do_bani(std::vector<int>& registers, int A, int B, int C);

void do_borr(std::vector<int>& registers, int A, int B, int C);
void do_bori(std::vector<int>& registers, int A, int B, int C);

void do_setr(std::vector<int>& registers, int A, int B, int C);
void do_seti(std::vector<int>& registers, int A, int B, int C);

void do_gtir(std::vector<int>& registers, int A, int B, int C);
void do_gtri(std::vector<int>& registers, int A, int B, int C);
void do_gtrr(std::vector<int>& registers, int A, int B, int C);

void do_eqir(std::vector<int>& registers, int A, int B, int C);
void do_eqri(std::vector<int>& registers, int A, int B, int C);
void do_eqrr(std::vector<int>& registers, int A, int B, int C);

void do_command(std::vector<int>& registers, const std::vector<int>& instruction);

const OpcodeLookup opcodes{
   {0, EQIR},
   {1, BORR},
   {2, ADDR},
   {3, GTRI},
   {4, MULI},
   {5, GTIR},
   {6, MULR},
   {7, BANR},
   {8, BORI},
   {9, EQRI},
   {10, EQRR},
   {11, BANI},
   {12, SETR},
   {13, GTRR},
   {14, ADDI},
   {15, SETI}
};

int wmain(int argc, wchar_t* argv[]) {
   advent_of_code::InputHandler input{ argc, argv };
   std::vector<std::wstring> lines{ input.read_all_lines() };
   std::wregex register_regex{ L"\\[(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\]" };
   std::wregex command_regex{ L"(\\d+) (\\d+) (\\d+) (\\d+)" };
   std::wsmatch matches;

   std::vector<std::wstring>::iterator itr{ lines.begin() };
   OpcodeGuesses opcode_guesses;
   unsigned int multi_opcodes{ 0 };
   while (itr != lines.end()) {
      std::wstring before_line{ *itr++ };
      if (before_line.empty()) {
         std::wcout << L"End for part 1" << std::endl;
         break;
      }

      std::wstring instruction_line{ *itr++ };
      std::wstring after_line{ *itr++ };
      ++itr; // One blank line in between

      if (std::regex_search(before_line, matches, command_regex)) {
         std::wcout << L"End for part 1" << std::endl;
         break;
      }

      std::vector<int> before;
      std::regex_search(before_line, matches, register_regex);
      if (matches.size() != 5) { // Whole match, then four numbers
         std::wcerr << L"Malformed input" << std::endl;
         return -1;
      }

      before.push_back(_wtoi(matches[1].str().c_str()));
      before.push_back(_wtoi(matches[2].str().c_str()));
      before.push_back(_wtoi(matches[3].str().c_str()));
      before.push_back(_wtoi(matches[4].str().c_str()));

      std::vector<int> instruction;
      std::regex_search(instruction_line, matches, command_regex);
      if (matches.size() != 5) { // Whole match, then four numbers
         std::wcerr << L"Malformed input" << std::endl;
         return -1;
      }

      instruction.push_back(_wtoi(matches[1].str().c_str()));
      instruction.push_back(_wtoi(matches[2].str().c_str()));
      instruction.push_back(_wtoi(matches[3].str().c_str()));
      instruction.push_back(_wtoi(matches[4].str().c_str()));

      std::vector<int> expected;
      std::regex_search(after_line, matches, register_regex);
      if (matches.size() != 5) { // Whole match, then four numbers
         std::wcerr << L"Malformed input" << std::endl;
         return -1;
      }

      expected.push_back(_wtoi(matches[1].str().c_str()));
      expected.push_back(_wtoi(matches[2].str().c_str()));
      expected.push_back(_wtoi(matches[3].str().c_str()));
      expected.push_back(_wtoi(matches[4].str().c_str()));

      OpcodeSet possible{ check_all_instructions(before, expected, instruction) };
      if (possible.size() >= 3) {
         ++multi_opcodes;
      }

      auto guess{ opcode_guesses.find(instruction[0]) };
      if (guess == opcode_guesses.end()) {
         opcode_guesses[instruction[0]] = possible;
      }
      else {
         OpcodeSet old{ guess->second };
         std::vector<Opcode> intersection{ 16 };
         std::vector<Opcode>::iterator it{std::set_intersection(old.begin(),
            old.end(), possible.begin(), possible.end(), intersection.begin()) };
         intersection.resize(it - intersection.begin());
         opcode_guesses[instruction[0]] = OpcodeSet{ intersection.begin(), intersection.end() };
      }
   }

   std::wcout << multi_opcodes << L" samples acted like three or more opcodes." << std::endl;

   // Prep itr for program
   ++itr;
   std::vector<int> registers{ 0, 0, 0, 0 };
   while (itr != lines.end()) {
      std::wstring instruction_line{ *itr++ };
      std::vector<int> instruction;
      std::regex_search(instruction_line, matches, command_regex);
      if (matches.size() != 5) { // Whole match, then four numbers
         std::wcerr << L"Malformed input" << std::endl;
         return -1;
      }

      instruction.push_back(_wtoi(matches[1].str().c_str()));
      instruction.push_back(_wtoi(matches[2].str().c_str()));
      instruction.push_back(_wtoi(matches[3].str().c_str()));
      instruction.push_back(_wtoi(matches[4].str().c_str()));

      do_command(registers, instruction);
   }

   std::wcout << L"Program over - registers[0] is " << registers[0] << std::endl;

   return 0;
}

std::vector<int> guess_addr(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] + registers[B];
   return registers;
}

std::vector<int> guess_addi(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] + B;
   return registers;
}

std::vector<int> guess_mulr(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] * registers[B];
   return registers;
}

std::vector<int> guess_muli(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] * B;
   return registers;
}

std::vector<int> guess_banr(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] & registers[B];
   return registers;
}

std::vector<int> guess_bani(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] & B;
   return registers;
}

std::vector<int> guess_borr(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] | registers[B];
   return registers;
}

std::vector<int> guess_bori(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A] | B;
   return registers;
}

std::vector<int> guess_setr(std::vector<int> registers, int A, int B, int C) {
   registers[C] = registers[A];
   return registers;
}

std::vector<int> guess_seti(std::vector<int> registers, int A, int B, int C) {
   registers[C] = A;
   return registers;
}

std::vector<int> guess_gtir(std::vector<int> registers, int A, int B, int C) {
   if (A > registers[B]) {
      registers[C] = 1;
   } else {
      registers[C] = 0;
   }
   return registers;
}

std::vector<int> guess_gtri(std::vector<int> registers, int A, int B, int C) {
   if (registers[A] > B) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
   return registers;
}

std::vector<int> guess_gtrr(std::vector<int> registers, int A, int B, int C) {
   if (registers[A]> registers[B]) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
   return registers;
}

std::vector<int> guess_eqir(std::vector<int> registers, int A, int B, int C) {
   if (A == registers[B]) {
      registers[C] = 1;
   } else {
      registers[C] = 0;
   }
   return registers;
}

std::vector<int> guess_eqri(std::vector<int> registers, int A, int B, int C) {
   if (registers[A] == B) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
   return registers;
}

std::vector<int> guess_eqrr(std::vector<int> registers, int A, int B, int C) {
   if (registers[A] == registers[B]) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
   return registers;
}

std::wstring OpcodeToString(const Opcode c) {
   switch (c) {
   case ADDR:
      return L"ADDR";
   case ADDI:
      return L"ADDI";
   case MULR:
      return L"MULR";
   case MULI:
      return L"MULI";
   case BANR:
      return L"BANR";
   case BANI:
      return L"BANI";
   case BORR:
      return L"BORR";
   case BORI:
      return L"BORI";
   case SETR:
      return L"SETR";
   case SETI:
      return L"SETI";
   case GTIR:
      return L"GTIR";
   case GTRI:
      return L"GTRI";
   case GTRR:
      return L"GTRR";
   case EQIR:
      return L"EQIR";
   case EQRI:
      return L"EQRI";
   case EQRR:
   default:
      return L"EQRR";
   }
}

OpcodeSet check_all_instructions(const std::vector<int>& before,
                                           const std::vector<int>& expected,
                                           const std::vector<int>& instruction) {
   auto matches_after{ [&expected](const std::vector<int>& actual){
      return std::equal(expected.begin(), expected.end(), actual.begin());
   } };

   OpcodeSet possible_opcodes;
   if (matches_after(guess_addr(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(ADDR);
   }
   if (matches_after(guess_addi(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(ADDI);
   }

   if (matches_after(guess_mulr(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(MULR);
   }
   if (matches_after(guess_muli(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(MULI);
   }

   if (matches_after(guess_banr(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(BANR);
   }
   if (matches_after(guess_bani(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(BANI);
   }

   if (matches_after(guess_borr(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(BORR);
   }
   if (matches_after(guess_bori(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(BORI);
   }

   if (matches_after(guess_setr(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(SETR);
   }
   if (matches_after(guess_seti(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(SETI);
   }

   if (matches_after(guess_gtir(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(GTIR);
   }
   if (matches_after(guess_gtri(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(GTRI);
   }
   if (matches_after(guess_gtrr(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(GTRR);
   }

   if (matches_after(guess_eqir(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(EQIR);
   }
   if (matches_after(guess_eqri(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(EQRI);
   }
   if (matches_after(guess_eqrr(before, instruction[1], instruction[2], instruction[3]))) {
      possible_opcodes.insert(EQRR);
   }

   return possible_opcodes;
}

void do_addr(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] + registers[B];
}

void do_addi(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] + B;
}

void do_mulr(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] * registers[B];
}

void do_muli(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] * B;
}

void do_banr(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] & registers[B];
}

void do_bani(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] & B;
}

void do_borr(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] | registers[B];
}

void do_bori(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A] | B;
}

void do_setr(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = registers[A];
}

void do_seti(std::vector<int>& registers, int A, int B, int C) {
   registers[C] = A;
}

void do_gtir(std::vector<int>& registers, int A, int B, int C) {
   if (A > registers[B]) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
}

void do_gtri(std::vector<int>& registers, int A, int B, int C) {
   if (registers[A] > B) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
}

void do_gtrr(std::vector<int>& registers, int A, int B, int C) {
   if (registers[A] > registers[B]) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
}

void do_eqir(std::vector<int>& registers, int A, int B, int C) {
   if (A == registers[B]) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
}

void do_eqri(std::vector<int>& registers, int A, int B, int C) {
   if (registers[A] == B) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
}

void do_eqrr(std::vector<int>& registers, int A, int B, int C) {
   if (registers[A] == registers[B]) {
      registers[C] = 1;
   }
   else {
      registers[C] = 0;
   }
}

void do_command(std::vector<int>& registers, const std::vector<int>& instruction) {
   int code{ instruction[0] };
   int A{ instruction[1] };
   int B{ instruction[2] };
   int C{ instruction[3] };

   switch (opcodes.at(code)) {
   case ADDR:
      do_addr(registers, A, B, C);
      break;
   case ADDI:
      do_addi(registers, A, B, C);
      break;
   case MULR:
      do_mulr(registers, A, B, C);
      break;
   case MULI:
      do_muli(registers, A, B, C);
      break;
   case BANR:
      do_banr(registers, A, B, C);
      break;
   case BANI:
      do_bani(registers, A, B, C);
      break;
   case BORR:
      do_borr(registers, A, B, C);
      break;
   case BORI:
      do_bori(registers, A, B, C);
      break;
   case SETR:
      do_setr(registers, A, B, C);
      break;
   case SETI:
      do_seti(registers, A, B, C);
      break;
   case GTIR:
      do_gtir(registers, A, B, C);
      break;
   case GTRI:
      do_gtri(registers, A, B, C);
      break;
   case GTRR:
      do_gtrr(registers, A, B, C);
      break;
   case EQIR:
      do_eqir(registers, A, B, C);
      break;
   case EQRI:
      do_eqri(registers, A, B, C);
      break;
   case EQRR:
   default:
      do_eqrr(registers, A, B, C);
      break;
   }

   return;
}
