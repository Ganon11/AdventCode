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

var lines = File.ReadAllLines(filepath);
var allElves = new List<Elf>();
var currentElf = new Elf();
foreach (var line in lines)
{
   if (line.Any())
   {
      _ = currentElf.Items.Add(int.Parse(line, System.Globalization.CultureInfo.CurrentCulture));
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

var sortedElves = allElves.OrderByDescending(e => e.Calories);

var maxElf = sortedElves.Skip(0).Take(1).First();
var secElf = sortedElves.Skip(1).Take(1).First();
var thiElf = sortedElves.Skip(2).Take(1).First();

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
