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

      var marker = new Queue<char>();
      var answer = 0;
      while (answer < targetLength)
      {
         marker.Enqueue(input[answer++]);
      }

      while (answer < input.Length)
      {
         if (marker.SequenceEqual(marker.Distinct()))
         {
            return answer;
         }

         _ = marker.Dequeue();
         marker.Enqueue(input[answer]);
         ++answer;
      }

      return -1;
   }

   public override string SolvePartOne()
   {
      return FindMarker(Input, 4).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      return FindMarker(Input, 14).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
