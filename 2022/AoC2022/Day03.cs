namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

internal sealed class Day3 : Day<string[]>
{
   public override string[] ParseInput(string rawInput) => rawInput.Split('\n');

   private static int GetPriority(char input)
   {
      if (input is >= 'a' and <= 'z')
      {
         return input - 'a' + 1;
      }
      else
      {
         return input - 'A' + 27;
      }
   }

   public override object ExecutePart1()
   {
      var sum = 0;
      foreach (var rucksack in this.Input)
      {
         var comp1 = new HashSet<char>();
         var comp2 = new HashSet<char>();

         var index = 0;
         for (; index < (rucksack.Length / 2); ++index)
         {
            _ = comp1.Add(rucksack[index]);
         }

         for (; index < rucksack.Length; ++index)
         {
            _ = comp2.Add(rucksack[index]);
         }

         var intersection = comp1.Intersect(comp2).First();
         sum += GetPriority(intersection);
      }

      return sum;
   }

   public override object ExecutePart2()
   {
      var sum = 0;
      var numGroups = this.Input.Length / 3;
      for (var group = 0; group < numGroups; ++group)
      {
         var rucksack1 = new HashSet<char>(this.Input[group * 3].ToCharArray());
         var rucksack2 = new HashSet<char>(this.Input[(group * 3) + 1].ToCharArray());
         var rucksack3 = new HashSet<char>(this.Input[(group * 3) + 2].ToCharArray());

         var intersection = rucksack1.Intersect(rucksack2).Intersect(rucksack3).First();
         sum += GetPriority(intersection);
      }

      return sum;
   }
}
