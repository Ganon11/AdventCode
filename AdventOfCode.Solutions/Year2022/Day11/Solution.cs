namespace AdventOfCode.Solutions.Year2022.Day11;

using System.Text.RegularExpressions;

internal sealed class Solution : SolutionBase<Monkey[]>
{
   public Solution() : base(11, 2022, "Monkey in the Middle", true) { }

   public override Monkey[] ParseInput(string input) => input.SplitByParagraph(shouldTrim: true)
      .Select(paragraph => new Monkey(paragraph))
      .ToArray();

   public override string SolvePartOne()
   {
      Monkey[] allMonkeys = (Monkey[])this.ParsedInput.Clone();
      for (var round = 0; round < 20; ++round)
      {
         foreach (var monkey in allMonkeys)
         {
            monkey.TakeTurn(allMonkeys);
         }
      }

      return allMonkeys.Select(m => m.InspectionCount)
         .OrderByDescending(x => x)
         .Take(2)
         .Aggregate((x, y) => x * y)
         .ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      Monkey[] allMonkeys = (Monkey[])this.ParsedInput.Clone();
      for (var round = 0; round < 10000; ++round)
      {
         foreach (var monkey in allMonkeys)
         {
            monkey.TakeTurn(allMonkeys);
         }
      }

      return allMonkeys.Select(m => m.InspectionCount)
         .OrderByDescending(x => x)
         .Take(2)
         .Aggregate((x, y) => x * y)
         .ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal sealed partial class Monkey
{
   public int Id { get; init; }
   public List<long> Items { get; set; } = new List<long>();
   public Func<long, long> Operation { get; init; }
   public Func<long, bool> Test { get; init; }
   public Tuple<int, int> Destinations { get; init; }

   public long InspectionCount { get; private set; }

   public Monkey(string paragraph)
   {
      var lines = paragraph.SplitByNewline(shouldTrim: true);
      Match m;

      m = MonkeyIdRegex().Match(lines[0]);
      if (!m.Success)
      {
         throw new ArgumentException($"Error parsing Monkey Id from line \"{lines[0]}\"", nameof(paragraph));
      }
      this.Id = int.Parse(m.Groups["id"].Value, System.Globalization.CultureInfo.CurrentCulture);

      m = StartingItemsRegex().Match(lines[1]);
      if (!m.Success)
      {
         throw new ArgumentException($"Error parsing Items from line \"{lines[1]}\"", nameof(paragraph));
      }
      this.Items = m.Groups["items"].Value
         .Trim()
         .Split(", ")
         .Select(token => long.Parse(token, System.Globalization.CultureInfo.CurrentCulture))
         .ToList();

      m = OperationRegex().Match(lines[2]);
      if (!m.Success)
      {
         throw new ArgumentException($"Error parsing Operation from line \"{lines[2]}\"", nameof(paragraph));
      }
      var operationValue = m.Groups["value"].Value;
      if (operationValue.Equals("old", StringComparison.OrdinalIgnoreCase))
      {
         this.Operation = m.Groups["operation"].Value switch
         {
            "+" => (x) => x + x,
            "*" => (x) => x * x,
            _ => throw new ArgumentException($"Unexpected format of Operation line \"{lines[2]}\"", nameof(paragraph)),
         };
      }
      else
      {
         long longValue = long.Parse(operationValue, System.Globalization.CultureInfo.CurrentCulture);
         this.Operation = m.Groups["operation"].Value switch
         {
            "+" => (x) => x + longValue,
            "*" => (x) => x * longValue,
            _ => throw new ArgumentException($"Unexpected format of Operation line \"{lines[2]}\"", nameof(paragraph)),
         };
      }

      m = TestRegex().Match(lines[3]);
      if (!m.Success)
      {
         throw new ArgumentException($"Error parsing Test from line \"{lines[3]}\"", nameof(paragraph));
      }
      var testValue = long.Parse(m.Groups["value"].Value, System.Globalization.CultureInfo.CurrentCulture);
      this.Test = (x) => x % testValue == 0;

      m = TrueMonkeyRegex().Match(lines[4]);
      if (!m.Success)
      {
         throw new ArgumentException($"Error parsing True Monkey Id from line \"{lines[4]}\"", nameof(paragraph));
      }
      var trueMonkey = int.Parse(m.Groups["monkey"].Value, System.Globalization.CultureInfo.CurrentCulture);

      m = FalseMonkeyRegex().Match(lines[5]);
      if (!m.Success)
      {
         throw new ArgumentException($"Error parsing False Monkey Id from line \"{lines[5]}\"", nameof(paragraph));
      }
      var falseMonkey = int.Parse(m.Groups["monkey"].Value, System.Globalization.CultureInfo.CurrentCulture);

      this.Destinations = new(trueMonkey, falseMonkey);
   }

   public void TakeTurn(Monkey[] allMonkeys, bool amWorried = false, bool debugOutput = false)
   {
      if (debugOutput)
      {
         Console.WriteLine($"Monkey {this.Id}:");
      }
      foreach (var item in this.Items)
      {
         if (debugOutput)
         {
            Console.WriteLine($"\tMonkey inspects an item with a worry level of {item}");
         }
         // Inspect
         long newItem = this.Operation(item);
         if (debugOutput)
         {
            Console.WriteLine($"\t\tWorry level changed to {newItem}");
         }

         // Worry Level Decrease
         if (!amWorried)
         {
            newItem /= 3;
            if (debugOutput)
            {
               Console.WriteLine($"\t\tMonkey gets bored with item. Worry level is divided by 3 to {newItem}");
            }
         }

         // Test
         if (this.Test(newItem))
         {
            allMonkeys[this.Destinations.Item1].Items.Add(newItem);
            if (debugOutput)
            {
               Console.WriteLine($"\t\tCurrent worry level IS divisible.");
               Console.WriteLine($"\t\tItem with worry level {newItem} is thrown to monkey {this.Destinations.Item1}.");
            }
         }
         else
         {
            allMonkeys[this.Destinations.Item2].Items.Add(newItem);

            if (debugOutput)
            {
               Console.WriteLine($"\t\tCurrent worry level is not divisible.");
               Console.WriteLine($"\t\tItem with worry level {newItem} is thrown to monkey {this.Destinations.Item2}.");
            }
         }

         ++this.InspectionCount;
      }

      this.Items.Clear();
   }

   [GeneratedRegex("Monkey (?<id>\\d+)")]
   private static partial Regex MonkeyIdRegex();

   [GeneratedRegex("Starting items: (?<items>.+)")]
   private static partial Regex StartingItemsRegex();

   [GeneratedRegex("Operation: new = old (?<operation>\\*|\\+) (?<value>.+)")]
   private static partial Regex OperationRegex();

   [GeneratedRegex("Test: divisible by (?<value>\\d+)")]
   private static partial Regex TestRegex();

   [GeneratedRegex("If true: throw to monkey (?<monkey>\\d+)")]
   private static partial Regex TrueMonkeyRegex();

   [GeneratedRegex("If false: throw to monkey (?<monkey>\\d+)")]
   private static partial Regex FalseMonkeyRegex();
}
