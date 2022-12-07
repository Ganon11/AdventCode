namespace AdventOfCode.Solutions.Year2022.Day01;

class Solution : SolutionBase<List<int>>
{
   public Solution() : base(01, 2022, "Calorie Counting") { }

   public override List<int> ParseInput(string input) => input
      .SplitByParagraph(shouldTrim: true)
      .Select(g => g.SplitByNewline(shouldTrim: true).Select(l => int.Parse(l, System.Globalization.CultureInfo.CurrentCulture)).Sum())
      .ToList();

   public override string SolvePartOne()
   {
      return ParsedInput
         .Max()
         .ToString();
   }

   public override string SolvePartTwo()
   {
      return ParsedInput
         .OrderByDescending(x => x)
         .Take(3)
         .Sum()
         .ToString();
   }
}
