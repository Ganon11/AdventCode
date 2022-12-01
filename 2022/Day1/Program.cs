using Day1;

string filepath;
if (args.Length == 0)
{
   filepath = "input/input.txt";
}
else
{
   filepath = args[0];
}

string[] lines = File.ReadAllLines(filepath);
List<Elf> allElves = new List<Elf>();
Elf currentElf = new Elf();
foreach (string line in lines)
{
   if (line.Any())
   {
      currentElf.Items.Add(int.Parse(line, System.Globalization.CultureInfo.CurrentCulture));
   }
   else
   {
      allElves.Add(currentElf);
      currentElf = new Elf();
   }
}

if (currentElf.Items.Any())
{
   allElves.Add(currentElf);
}

IOrderedEnumerable<Elf> sortedElves = allElves.OrderByDescending(e => e.Calories);

Elf maxElf = sortedElves.Skip(0).Take(1).First();
Elf secElf = sortedElves.Skip(1).Take(1).First();
Elf thiElf = sortedElves.Skip(2).Take(1).First();

Console.WriteLine($"Max Elf carrying {maxElf.Calories}");
Console.WriteLine($"Second Elf carrying {secElf.Calories}");
Console.WriteLine($"Third Elf carrying {thiElf.Calories}");
Console.WriteLine($"Total: {maxElf.Calories + secElf.Calories + thiElf.Calories}");

namespace Day1
{
   public class Elf
   {
      private int? calories;
      public int Calories
      {
         get
         {
            if (!this.calories.HasValue)
            {
               this.calories = 0;
               foreach (var item in this.Items)
               {
                  this.calories += item;
               }
            }

            return this.calories.Value;
         }
      }

      private HashSet<int> items = new();
      public HashSet<int> Items
      {
         get => this.items;
         set
         {
            this.calories = null;
            this.items = value;
         }
      }
   }
}
