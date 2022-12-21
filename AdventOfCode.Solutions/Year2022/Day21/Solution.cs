namespace AdventOfCode.Solutions.Year2022.Day21;
internal sealed class Solution : SolutionBase
{
   public Solution() : base(21, 2022, "Monkey Math", false) { }

   public override string SolvePartOne()
   {
      foreach (var line in this.Input.SplitByNewline(shouldTrim: true))
      {
         _ = Monkey.Create(line);
      }

      return Monkey.GetMonkeyValue("root").ToString(System.Globalization.CultureInfo.InvariantCulture);
   }

   public override string SolvePartTwo()
   {
      foreach (var line in this.Input.SplitByNewline(shouldTrim: true))
      {
         _ = Monkey.Create(line);
      }

      return Monkey.Print("root");
   }
}

internal abstract class Monkey
{
   public string Name { get; protected set; }

   protected Monkey(string name) => this.Name = name;

   public static Monkey Create(string line)
   {
      Monkey newMonkey;
      var tokens = line.Split(": ");
      var name = tokens[0];
      if (long.TryParse(tokens[1], out var value))
      {
         newMonkey = new SimpleMonkey(name, value);
      }
      else
      {
         var valueTokens = tokens[1].Split(' ');
         var monkey1 = valueTokens[0];
         var op = valueTokens[1].First();
         var monkey2 = valueTokens[2];
         newMonkey = new ComplexMonkey(name, monkey1, monkey2, op);
      }

      AllMonkeys[name] = newMonkey;
      return newMonkey;
   }

   public abstract long GetValue();

   private static readonly Dictionary<string, Monkey> AllMonkeys = new();

   public static long GetMonkeyValue(string name) => AllMonkeys[name].GetValue();

   public static string Print(string name)
   {
      if (AllMonkeys.TryGetValue(name, out var monkey) && monkey != null)
      {
         return monkey.ToString();
      }
      return string.Empty;
   }
}

internal class SimpleMonkey : Monkey
{
   private readonly long value;

   public SimpleMonkey(string name, long value) : base(name) => this.value = value;

   public override long GetValue() => this.value;

   public override string ToString()
   {
      if (this.Name.Equals("humn", StringComparison.OrdinalIgnoreCase))
      {
         return "x";
      }

      return this.value.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}

internal class ComplexMonkey : Monkey
{
   private readonly string monkey1;
   private readonly string monkey2;
   private readonly char op;

   public ComplexMonkey(string name, string monkey1, string monkey2, char op) : base(name)
   {
      this.monkey1 = monkey1;
      this.monkey2 = monkey2;
      this.op = op;
   }

   private long? value;
   public override long GetValue()
   {
      if (this.value == null)
      {
         this.value = this.op switch
         {
            '+' => GetMonkeyValue(this.monkey1) + GetMonkeyValue(this.monkey2),
            '-' => GetMonkeyValue(this.monkey1) - GetMonkeyValue(this.monkey2),
            '*' => GetMonkeyValue(this.monkey1) * GetMonkeyValue(this.monkey2),
            '/' => (long?)(GetMonkeyValue(this.monkey1) / GetMonkeyValue(this.monkey2)),
            _ => throw new InvalidOperationException($"Unknown operator {this.op}"),
         };
      }

      return this.value.Value;
   }

   public override string ToString()
   {
      var string1 = Print(this.monkey1);
      if (!string1.Contains('x'))
      {
         string1 = GetMonkeyValue(this.monkey1).ToString(System.Globalization.CultureInfo.InvariantCulture);
      }

      var string2 = Print(this.monkey2);
      if (!string2.Contains('x'))
      {
         string2 = GetMonkeyValue(this.monkey2).ToString(System.Globalization.CultureInfo.InvariantCulture);
      }

      if (this.Name.Equals("root", StringComparison.OrdinalIgnoreCase))
      {
         return $"{string1} = {string2}";
      }

      return $"({string1} {this.op} {string2})";
   }
}
