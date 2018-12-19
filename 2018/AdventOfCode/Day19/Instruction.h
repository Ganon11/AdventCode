#pragma once

#include <iostream>
#include <string>
#include <vector>

enum Opcode
{
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
  EQRR,
  NONE
};

std::wstring opcode_to_string(const Opcode c);

Opcode string_to_opcode(const std::wstring& line);

class Instruction
{
public:
  Instruction(const std::wstring& line);

  Opcode get_code() const;
  int get_a() const;
  int get_b() const;
  int get_c() const;

  friend std::wostream& operator<<(std::wostream& out, const Instruction& i);

private:
  Opcode m_code;
  int m_a;
  int m_b;
  int m_c;
};

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

void do_command(std::vector<int>& registers, const Instruction& instruction);
