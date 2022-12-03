namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

internal sealed class Day1 : Day<List<int>>
{
   public override List<int> ParseInput(string rawInput) => rawInput
      .Split("\n\n")
      .Select(g => g.Split("\n").Select(l => int.Parse(l, System.Globalization.CultureInfo.CurrentCulture)).Sum())
      .ToList();

   public override object ExecutePart1() => this.Input.Max();

   public override object ExecutePart2() => this.Input.OrderByDescending(x => x).Take(3).Sum();
}
