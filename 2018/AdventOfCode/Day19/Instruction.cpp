#include "pch.h"
#include "Instruction.h"

#include <regex>

std::wstring opcode_to_string(const Opcode c) {
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
    return L"EQRR";
  default:
    return L"NONE";
  }
}

Opcode string_to_opcode(const std::wstring& line) {
  if (0 == _wcsicmp(L"ADDR", line.c_str())) {
    return ADDR;
  }

  if (0 == _wcsicmp(L"ADDI", line.c_str())) {
    return ADDI;
  }

  if (0 == _wcsicmp(L"MULR", line.c_str())) {
    return MULR;
  }

  if (0 == _wcsicmp(L"MULI", line.c_str())) {
    return MULI;
  }

  if (0 == _wcsicmp(L"BANR", line.c_str())) {
    return BANR;
  }

  if (0 == _wcsicmp(L"BANI", line.c_str())) {
    return BANI;
  }

  if (0 == _wcsicmp(L"BORR", line.c_str())) {
    return BORR;
  }

  if (0 == _wcsicmp(L"BORI", line.c_str())) {
    return BORI;
  }

  if (0 == _wcsicmp(L"SETR", line.c_str())) {
    return SETR;
  }

  if (0 == _wcsicmp(L"SETI", line.c_str())) {
    return SETI;
  }

  if (0 == _wcsicmp(L"GTIR", line.c_str())) {
    return GTIR;
  }

  if (0 == _wcsicmp(L"GTRI", line.c_str())) {
    return GTRI;
  }

  if (0 == _wcsicmp(L"GTRR", line.c_str())) {
    return GTRR;
  }

  if (0 == _wcsicmp(L"EQIR", line.c_str())) {
    return EQIR;
  }

  if (0 == _wcsicmp(L"EQRI", line.c_str())) {
    return EQRI;
  }

  if (0 == _wcsicmp(L"EQRR", line.c_str())) {
    return EQRR;
  }

  return NONE;
}

Instruction::Instruction(const std::wstring& line) {
  std::wregex instruction_regex{ L"(\\w+) (-?\\d+) (-?\\d+) (-?\\d+)" };
  std::wsmatch matches;

  if (std::regex_search(line, matches, instruction_regex) && matches.size() == 5) {
    m_code = string_to_opcode(matches[1].str());
    m_a = _wtoi(matches[2].str().c_str());
    m_b = _wtoi(matches[3].str().c_str());
    m_c = _wtoi(matches[4].str().c_str());
  }
}

Opcode Instruction::get_code() const {
  return m_code;
}

int Instruction::get_a() const {
  return m_a;
}

int Instruction::get_b() const {
  return m_b;
}

int Instruction::get_c() const {
  return m_c;
}

std::wostream& operator<<(std::wostream& out, const Instruction& i) {
  out << opcode_to_string(i.m_code) << L' ' << i.m_a << L' ' << i.m_b << L' ' << i.m_c;
  return out;
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
  } else {
    registers[C] = 0;
  }
}

void do_gtri(std::vector<int>& registers, int A, int B, int C) {
  if (registers[A] > B) {
    registers[C] = 1;
  } else {
    registers[C] = 0;
  }
}

void do_gtrr(std::vector<int>& registers, int A, int B, int C) {
  if (registers[A] > registers[B]) {
    registers[C] = 1;
  } else {
    registers[C] = 0;
  }
}

void do_eqir(std::vector<int>& registers, int A, int B, int C) {
  if (A == registers[B]) {
    registers[C] = 1;
  } else {
    registers[C] = 0;
  }
}

void do_eqri(std::vector<int>& registers, int A, int B, int C) {
  if (registers[A] == B) {
    registers[C] = 1;
  } else {
    registers[C] = 0;
  }
}

void do_eqrr(std::vector<int>& registers, int A, int B, int C) {
  if (registers[A] == registers[B]) {
    registers[C] = 1;
  } else {
    registers[C] = 0;
  }
}

void do_command(std::vector<int>& registers, const Instruction& instruction) {
  int A{ instruction.get_a() };
  int B{ instruction.get_b() };
  int C{ instruction.get_c() };

  switch (instruction.get_code()) {
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

