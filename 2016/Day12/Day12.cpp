// Day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum CommandType {
   COMMAND_COPY,
   COMMAND_INCREMENT,
   COMMAND_DECREMENT,
   COMMAND_JUMP_NOT_ZERO
};

struct Command {
   CommandType type;
   std::wstring value1;
   std::wstring value2;
};

std::vector<Command> GetCommands(const std::wstring& inputFile) {
   std::vector<Command> commands;
   std::wifstream input{ inputFile };
   if (!input.is_open()) {
      return commands;
   }

   std::wstring token;
   input >> token;
   while (input.good()) {
      Command cmd;
      if (token == L"cpy") {
         cmd.type = COMMAND_COPY;
         input >> cmd.value1;
         input >> cmd.value2;
      }
      else if (token == L"inc") {
         cmd.type = COMMAND_INCREMENT;
         input >> cmd.value1;
      }
      else if (token == L"dec") {
         cmd.type = COMMAND_DECREMENT;
         input >> cmd.value1;
      }
      else if (token == L"jnz") {
         cmd.type = COMMAND_JUMP_NOT_ZERO;
         input >> cmd.value1;
         input >> cmd.value2;
      }
      else {
         std::wcerr << L"Unrecognized command: " << token << std::endl;
         throw L"Unrecognized command";
      }

      commands.push_back(cmd);
      input >> token;
   }

   return commands;
}

int RunCommands(const std::vector<Command>& commands, int a, int b, int c, int d) {
   std::wcout << L"Initial state: a = " << a << L", b = " << b << L", c = " << c << L", d = " << d << std::endl;

   size_t instruction = 0;
   while (0 <= instruction && instruction < commands.size()) {
      Command currentCommand = commands[instruction];
      int val;
      switch (currentCommand.type) {
      case COMMAND_COPY:
         // value1 is source register or value, value2 is destination register
         if (currentCommand.value1 == L"a") {
            val = a;
         }
         else if (currentCommand.value1 == L"b") {
            val = b;
         }
         else if (currentCommand.value1 == L"c") {
            val = c;
         }
         else if (currentCommand.value1 == L"d") {
            val = d;
         }
         else {
            val = _wtoi(currentCommand.value1.c_str());
         }

         if (currentCommand.value2 == L"a") {
            a = val;
         }
         else if (currentCommand.value2 == L"b") {
            b = val;
         }
         else if (currentCommand.value2 == L"c") {
            c = val;
         }
         else {
            d = val;
         }

         ++instruction;
         break;
      case COMMAND_DECREMENT:
         // value1 is register, value2 is unused
         if (currentCommand.value1 == L"a") {
            --a;
         }
         else if (currentCommand.value1 == L"b") {
            --b;
         }
         else if (currentCommand.value1 == L"c") {
            --c;
         }
         else {
            --d;
         }

         ++instruction;
         break;
      case COMMAND_INCREMENT:
         // value1 is register, value2 is unused
         if (currentCommand.value1 == L"a") {
            ++a;
         }
         else if (currentCommand.value1 == L"b") {
            ++b;
         }
         else if (currentCommand.value1 == L"c") {
            ++c;
         }
         else {
            ++d;
         }

         ++instruction;
         break;
      case COMMAND_JUMP_NOT_ZERO:
         // value1 is register or value, value 2 is offset
         if (currentCommand.value1 == L"a") {
            val = a;
         }
         else if (currentCommand.value1 == L"b") {
            val = b;
         }
         else if (currentCommand.value1 == L"c") {
            val = c;
         }
         else if (currentCommand.value1 == L"d") {
            val = d;
         }
         else {
            val = _wtoi(currentCommand.value1.c_str());
         }

         if (val == 0) {
            // No offset
            ++instruction;
         }
         else {
            // Offset
            int offset = _wtoi(currentCommand.value2.c_str());
            instruction += offset;
         }
         break;
      }
   }

   std::wcout << L"Final state: a = " << a << L", b = " << b << L", c = " << c << L", d = " << d << std::endl;

   return a;
}

int wmain(int argc, wchar_t *argv[]) {
   std::wstring inputFile;
   if (argc == 2) {
      inputFile = argv[1];
   }
   else {
      inputFile = L"Sample.txt";
   }

   std::vector<Command> commands = GetCommands(inputFile);

   
   
   // Part 1
   int answer1{ RunCommands(commands, 0, 0, 0, 0) };

   // Part 2
   int answer2{ RunCommands(commands, 0, 0, 1, 0) };

   std::wcout << L"Answers: part 1 " << answer1 << L", part 2 " << answer2 << std::endl;

   return 0;
}
