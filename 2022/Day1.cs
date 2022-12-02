namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

internal class Day1 : Day
{
   private static List<int> GetElves(string input)
   {
      var elves = new List<int>();
      var groups = input.Split("\n\n");
      foreach (var group in groups)
      {
         var lines = group.Split('\n');
         var total = 0;
         foreach (var line in lines)
         {
            total += int.Parse(line, System.Globalization.CultureInfo.CurrentCulture);
         }

         elves.Add(total);
      }

      return elves;
   }

   public override object ExecutePart1() => GetElves(this.Input).Max();

   public override object ExecutePart2() => GetElves(this.Input).OrderByDescending(x => x).Take(3).Sum();
}
