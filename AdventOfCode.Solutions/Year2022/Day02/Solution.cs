namespace AdventOfCode.Solutions.Year2022.Day02;

internal sealed class Solution : SolutionBase<char[][]>
{
   public Solution() : base(02, 2022, "Rock Paper Scissors") { }

   public override char[][] ParseInput(string input) => input.SplitByNewline()
      .Select(line => line.Split(" ")
         .Select(t => t[0])
         .ToArray())
      .ToArray();

   public override string SolvePartOne()
   {
      Dictionary<char, Dictionary<char, int>> p1 = new()
      {
         { 'A', new() { { 'X', 4 }, { 'Y', 8 }, { 'Z', 3 } } },
         { 'B', new() { { 'X', 1 }, { 'Y', 5 }, { 'Z', 9 } } },
         { 'C', new() { { 'X', 7 }, { 'Y', 2 }, { 'Z', 6 } } }
      };
      return this.ParsedInput
         .Select(line => p1[line[0]][line[1]])
         .Sum()
         .ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      Dictionary<char, Dictionary<char, int>> p2 = new()
      {
         { 'A', new() { { 'X', 3 }, { 'Y', 4 }, { 'Z', 8 } } },
         { 'B', new() { { 'X', 1 }, { 'Y', 5 }, { 'Z', 9 } } },
         { 'C', new() { { 'X', 2 }, { 'Y', 6 }, { 'Z', 7 } } }
      };
      return this.ParsedInput
         .Select(line => p2[line[0]][line[1]])
         .Sum()
         .ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
