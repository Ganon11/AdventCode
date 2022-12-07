namespace AdventOfCode.Solutions.Year2021.Day01;

class Solution : SolutionBase<int[]>
{
   public Solution() : base(01, 2021, "Sonar Sweep") { }

   public override int[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true).Select(line => int.Parse(line, System.Globalization.CultureInfo.CurrentCulture)).ToArray();

   private int NumTimesDescended(int[] depths, int window = 1)
   {
      if (depths.Length < window)
      {
         return 0;
      }

      int oldSum = depths.Take(window).Sum();
      int currentIndex = window;
      int total = 0;
      while (currentIndex < depths.Length)
      {
         int newSum = depths.Skip(currentIndex - window + 1).Take(window).Sum();
         if (newSum > oldSum)
         {
            ++total;
         }

         oldSum = newSum;
         ++currentIndex;
      }

      return total;
   }

   public override string SolvePartOne() => NumTimesDescended(ParsedInput).ToString();

   public override string SolvePartTwo() => NumTimesDescended(ParsedInput, 3).ToString();
}
