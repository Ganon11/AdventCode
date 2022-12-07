namespace AdventOfCode.Solutions.Year2022.Day02;

class Solution : SolutionBase<string[][]>
{
   public Solution() : base(02, 2022, "Rock Paper Scissors") { }

   public override string[][] ParseInput(string input) => input.SplitByNewline(shouldTrim: true).Select(line => line.Split(" ")).ToArray();

   public override string SolvePartOne()
   {
      Dictionary<string, Dictionary<string, int>> P1 = new()
      {
         { "A", new() { { "X", 4 }, { "Y", 8 }, { "Z", 3 } } },
         { "B", new() { { "X", 1 }, { "Y", 5 }, { "Z", 9 } } },
         { "C", new() { { "X", 7 }, { "Y", 2 }, { "Z", 6 } } }
      };
      return ParsedInput
         .Select(line => P1[line[0]][line[1]])
         .Sum()
         .ToString();
   }

   public override string SolvePartTwo()
   {
      Dictionary<string, Dictionary<string, int>> P1 = new()
      {
         { "A", new() { { "X", 3 }, { "Y", 4 }, { "Z", 8 } } },
         { "B", new() { { "X", 1 }, { "Y", 5 }, { "Z", 9 } } },
         { "C", new() { { "X", 2 }, { "Y", 6 }, { "Z", 7 } } }
      };
      return ParsedInput
         .Select(line => P1[line[0]][line[1]])
         .Sum()
         .ToString();
   }
}
