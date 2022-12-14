namespace AdventOfCode.Solutions.Year2022.Day06;

internal sealed class Solution : SolutionBase
{
   public Solution() : base(06, 2022, "Tuning Trouble") { }
   private static int FindMarker(string input, int targetLength)
   {
      if (input.Length < targetLength)
      {
         return -1;
      }

      var marker = 0;
      var answer = 0;
      while (answer < targetLength)
      {
         var currentChar = input[answer++];
         marker ^= 1 << currentChar;
      }

      while (answer < input.Length)
      {
         if (Convert.ToString(marker, 2).Count(c => c == '1') == targetLength)
         {
            return answer;
         }

         var oldChar = input[answer - targetLength];
         marker ^= 1 << oldChar;
         var currentChar = input[answer++];
         marker ^= 1 << currentChar;
      }

      return -1;
   }

   public override string SolvePartOne() => FindMarker(this.Input, 4).ToString(System.Globalization.CultureInfo.CurrentCulture);

   public override string SolvePartTwo() => FindMarker(this.Input, 14).ToString(System.Globalization.CultureInfo.CurrentCulture);
}
