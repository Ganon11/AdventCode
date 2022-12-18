namespace AdventOfCode.Solutions.Year2022.Day04;

using RangePair = Tuple<HashSet<int>, HashSet<int>>;

internal sealed class Solution : SolutionBase<IEnumerable<RangePair>>
{
   public Solution() : base(04, 2022, "Camp Cleanup") { }

   private static HashSet<int> ConvertRangeStringToSet(string range)
   {
      var pair = range.Split('-')
         .Select(i => int.Parse(i, System.Globalization.CultureInfo.CurrentCulture))
         .ToArray();
      return new HashSet<int>(Enumerable.Range(pair[0], pair[1] - pair[0] + 1));
   }

   public override IEnumerable<RangePair> ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
         .Select(line =>
         {
            var ranges = line.Split(',');
            return new RangePair(ConvertRangeStringToSet(ranges[0]), ConvertRangeStringToSet(ranges[1]));
         });

   public override string SolvePartOne() => this.ParsedInput
         .Count(pair => pair.Item1.IsProperSubsetOf(pair.Item2) || pair.Item1.IsProperSupersetOf(pair.Item2) || pair.Item1.SetEquals(pair.Item2))
         .ToString(System.Globalization.CultureInfo.CurrentCulture);

   public override string SolvePartTwo() => this.ParsedInput
         .Count(pair => pair.Item1.Intersect(pair.Item2).Any())
         .ToString(System.Globalization.CultureInfo.CurrentCulture);
}
