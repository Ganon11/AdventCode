namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

internal sealed class Day2 : Day<string[][]>
{
   public override string[][] ParseInput(string rawInput) => rawInput.Split("\n").Select(line => line.Split(" ")).ToArray();

   private static Dictionary<string, Dictionary<string, int>> p1 = new()
   {
      { "A", new() { { "X", 4 }, { "Y", 8 }, { "Z", 3 } } },
      { "B", new() { { "X", 1 }, { "Y", 5 }, { "Z", 9 } } },
      { "C", new() { { "X", 7 }, { "Y", 2 }, { "Z", 6 } } }
   };

   public override object ExecutePart1() => this.Input.Select(line => p1[line[0]][line[1]]).Sum();

   private static Dictionary<string, Dictionary<string, int>> p2 = new()
   {
      { "A", new() { { "X", 3 }, { "Y", 4 }, { "Z", 8 } } },
      { "B", new() { { "X", 1 }, { "Y", 5 }, { "Z", 9 } } },
      { "C", new() { { "X", 2 }, { "Y", 6 }, { "Z", 7 } } }
   };

   public override object ExecutePart2() => this.Input.Select(line => p2[line[0]][line[1]]).Sum();
}
