namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

using RangePair = Tuple<HashSet<int>, HashSet<int>>;

internal sealed class Day4 : Day<IEnumerable<RangePair>>
{
   private static HashSet<int> ConvertRangeStringToSet(string range)
   {
      var pair = range.Split('-')
         .Select(i => int.Parse(i, System.Globalization.CultureInfo.CurrentCulture))
         .ToArray();
      return new HashSet<int>(Enumerable.Range(pair[0], pair[1] - pair[0] + 1));
   }

   public override IEnumerable<RangePair> ParseInput(string rawInput) => rawInput.Split('\n')
      .Select(line =>
      {
         var ranges = line.Split(',');
         return new RangePair(
            ConvertRangeStringToSet(ranges[0]),
            ConvertRangeStringToSet(ranges[1])
         );
      });

   public override object ExecutePart1() => this.Input
      .Count(pair => pair.Item1.IsProperSubsetOf(pair.Item2)
         || pair.Item2.IsProperSubsetOf(pair.Item1)
         || pair.Item1.SetEquals(pair.Item2));

   public override object ExecutePart2() => this.Input
      .Count(pair => pair.Item1.Intersect(pair.Item2).Any());
}
