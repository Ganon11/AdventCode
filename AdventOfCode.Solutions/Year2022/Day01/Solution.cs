namespace AdventOfCode.Solutions.Year2022.Day01;

internal sealed class Solution : SolutionBase<IEnumerable<int>>
{
   public Solution() : base(01, 2022, "Calorie Counting") { }

   public override IEnumerable<int> ParseInput(string input) => input
      .SplitByParagraph(shouldTrim: true)
      .Select(g => g.SplitByNewline(shouldTrim: true)
         .Select(l => int.Parse(l, System.Globalization.CultureInfo.CurrentCulture))
         .Sum()
      );

   public override string SolvePartOne() => this.ParsedInput
         .Max()
         .ToString(System.Globalization.CultureInfo.CurrentCulture);

   public override string SolvePartTwo() => this.ParsedInput
         .OrderByDescending(x => x)
         .Take(3)
         .Sum()
         .ToString(System.Globalization.CultureInfo.CurrentCulture);
}
