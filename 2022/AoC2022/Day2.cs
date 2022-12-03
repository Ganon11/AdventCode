namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

internal sealed class Day2 : Day
{
   private static int CalculateScore(string input, Dictionary<char, Dictionary<char, int>> lookup)
   {
      var score = 0;
      foreach (var line in input.Split('\n'))
      {
         var tokens = line.Split(' ');
         score += lookup[tokens[0][0]][tokens[1][0]];
      }

      return score;
   }

   public override object ExecutePart1()
   {
      Dictionary<char, Dictionary<char, int>> lookup = new()
      {
         { 'A', new() { { 'X', 4 }, { 'Y', 8 }, { 'Z', 3 } } },
         { 'B', new() { { 'X', 1 }, { 'Y', 5 }, { 'Z', 9 } } },
         { 'C', new() { { 'X', 7 }, { 'Y', 2 }, { 'Z', 6 } } }
      };

      return CalculateScore(this.Input, lookup);
   }

   public override object ExecutePart2()
   {
      Dictionary<char, Dictionary<char, int>> lookup = new()
      {
         { 'A', new() { { 'X', 3 }, { 'Y', 4 }, { 'Z', 8 } } },
         { 'B', new() { { 'X', 1 }, { 'Y', 5 }, { 'Z', 9 } } },
         { 'C', new() { { 'X', 2 }, { 'Y', 6 }, { 'Z', 7 } } }
      };

      return CalculateScore(this.Input, lookup);
   }
}
