#pragma once

#include <functional>
#include <iostream>
#include <map>
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

  void execute(std::vector<int>& registers) const;

  friend std::wostream& operator<<(std::wostream& out, const Instruction& i);

private:
  Opcode m_code;
  int m_a;
  int m_b;
  int m_c;

  typedef std::map<Opcode, std::function<void(std::vector<int>&, int, int, int)>> DynamicDispatchMap;
  static DynamicDispatchMap opcode_map;
};
