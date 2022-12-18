namespace AdventOfCode.Solutions.Year2022.Day11;

using System.Text.RegularExpressions;

internal sealed class Solution : SolutionBase<Monkey[]>
{
   public Solution() : base(11, 2022, "Monkey in the Middle", false) { }

   public override Monkey[] ParseInput(string input) => input.SplitByParagraph(shouldTrim: true)
      .Select(paragraph => new Monkey(paragraph))
      .ToArray();

   public override string SolvePartOne()
   {
      var allMonkeys = this.ParsedInput.Copy();
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
      var allMonkeys = this.ParsedInput.Copy();

      long commonFactor = 1;
      foreach (var monkey in allMonkeys)
      {
         commonFactor *= monkey.TestValue;
      }

      for (var round = 1; round <= 10000; ++round)
      {
         foreach (var monkey in allMonkeys)
         {
            monkey.TakeTurn(allMonkeys, commonFactor: commonFactor, amWorried: true);
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
   public char Operation { get; init; }
   public string OperationValue { get; init; }
   public long TestValue { get; init; }
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
      this.OperationValue = m.Groups["value"].Value;
      this.Operation = m.Groups["operation"].Value[0];

      m = TestRegex().Match(lines[3]);
      if (!m.Success)
      {
         throw new ArgumentException($"Error parsing Test from line \"{lines[3]}\"", nameof(paragraph));
      }
      this.TestValue = long.Parse(m.Groups["value"].Value, System.Globalization.CultureInfo.CurrentCulture);

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

   public void TakeTurn(Monkey[] allMonkeys, long commonFactor = 0, bool amWorried = false, bool debugOutput = false)
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

         var operand = this.OperationValue.Equals("old", StringComparison.OrdinalIgnoreCase) ? item : int.Parse(this.OperationValue, System.Globalization.CultureInfo.CurrentCulture);
         var newItem = this.Operation switch
         {
            '*' => item * operand,
            '+' => item + operand,
            _ => -1,
         };
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
         else if (commonFactor != 0)
         {
            newItem %= commonFactor;
            if (debugOutput)
            {
               Console.WriteLine($"\t\tMonkey gets bored with item. Worry level is modulo'd by {commonFactor} to {newItem}");
            }
         }

         // Test
         if (newItem % this.TestValue == 0)
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
