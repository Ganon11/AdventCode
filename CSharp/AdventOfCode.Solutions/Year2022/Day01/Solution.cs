namespace AdventOfCode.Solutions.Year2022.Day01;

class Solution : SolutionBase
{
   public Solution() : base(01, 2022, "") { }

   protected override string SolvePartOne()
   {
      return Input.SplitByParagraph(shouldTrim: true)
         .Select(g => g.SplitByNewline(shouldTrim: true).Select(l => int.Parse(l, System.Globalization.CultureInfo.CurrentCulture)).Sum())
         .ToList()
         .Max()
         .ToString();
   }

   protected override string SolvePartTwo()
   {
      return Input.SplitByParagraph(shouldTrim: true)
         .Select(g => g.SplitByNewline(shouldTrim: true).Select(l => int.Parse(l, System.Globalization.CultureInfo.CurrentCulture)).Sum())
         .ToList()
         .OrderByDescending(x => x)
         .Take(3)
         .Sum()
         .ToString();
   }
}
