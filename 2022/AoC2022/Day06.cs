namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

internal sealed class Day6 : Day
{
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

   public override object ExecutePart1() => FindMarker(this.Input, 4);

   public override object ExecutePart2() => FindMarker(this.Input, 14);
}
