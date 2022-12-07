namespace AdventOfCode.Solutions.Year2022.Day04;

using RangePair = Tuple<HashSet<int>, HashSet<int>>;

class Solution : SolutionBase
{
   public Solution() : base(04, 2022, "Camp Cleanup") { }

   private static HashSet<int> ConvertRangeStringToSet(string range)
   {
      var pair = range.Split('-')
         .Select(i => int.Parse(i, System.Globalization.CultureInfo.CurrentCulture))
         .ToArray();
      return new HashSet<int>(Enumerable.Range(pair[0], pair[1] - pair[0] + 1));
   }

   protected override string SolvePartOne()
   {
      return Input.SplitByNewline(shouldTrim: true)
         .Select(line =>
         {
            var ranges = line.Split(',');
            return new RangePair(ConvertRangeStringToSet(ranges[0]), ConvertRangeStringToSet(ranges[1]));
         })
         .Count(pair => pair.Item1.IsProperSubsetOf(pair.Item2) || pair.Item1.IsProperSupersetOf(pair.Item2) || pair.Item1.SetEquals(pair.Item2))
         .ToString();
   }

   protected override string SolvePartTwo()
   {
      return Input.SplitByNewline(shouldTrim: true)
         .Select(line =>
         {
            var ranges = line.Split(',');
            return new RangePair(ConvertRangeStringToSet(ranges[0]), ConvertRangeStringToSet(ranges[1]));
         })
         .Count(pair => pair.Item1.Intersect(pair.Item2).Any())
         .ToString();
   }
}
