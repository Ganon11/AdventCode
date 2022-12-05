namespace Ganon11.AdventOfCode.Year2022;
using Tidy.AdventOfCode;

internal sealed partial class Day5 : Day<Tuple<Stack<char>[], string[]>>
{
   public override Tuple<Stack<char>[], string[]> ParseInput(string rawInput)
   {
      rawInput = rawInput.Replace("\r", "");
      var tokens = rawInput.Split("\n\n");

      var reversedLines = tokens[0].Split("\n").Reverse();
      var numStacks = MultipleSpacesRegex().Split(reversedLines.First().Trim()).Length;
      var stacks = new Stack<char>[numStacks];
      for (var stackNum = 1; stackNum <= numStacks; ++stackNum)
      {
         stacks[stackNum - 1] = new Stack<char>();
      }

      foreach (var line in reversedLines.Skip(1))
      {
         for (var stackNum = 1; stackNum <= numStacks; ++stackNum)
         {
            var charIndex = ((stackNum - 1) * 4) + 1;
            if (charIndex >= line.Length)
            {
               continue;
            }
            else if (char.IsUpper(line[charIndex]))
            {
               stacks[stackNum - 1].Push(line[charIndex]);
            }
         }
      }

      var instructions = tokens[1].Split("\n");

      return new(stacks, instructions);
   }

   public override object ExecutePart1()
   {
      var stacks = this.Input.Item1;

      foreach (var line in this.Input.Item2)
      {
         var match = InstructionRegex().Match(line);
         var count = int.Parse(match.Groups["count"].Value, System.Globalization.CultureInfo.CurrentCulture);
         var source = int.Parse(match.Groups["source"].Value, System.Globalization.CultureInfo.CurrentCulture) - 1;
         var target = int.Parse(match.Groups["target"].Value, System.Globalization.CultureInfo.CurrentCulture) - 1;

         for (var i = 0; i < count; ++i)
         {
            stacks[target].Push(stacks[source].Pop());
         }
      }

      string answer = string.Empty;
      foreach (var stack in stacks)
      {
         answer += stack.Peek();
      }

      return answer;
   }

   public override object ExecutePart2()
   {
      var stacks = this.Input.Item1;

      foreach (var line in this.Input.Item2)
      {
         var match = InstructionRegex().Match(line);
         var count = int.Parse(match.Groups["count"].Value, System.Globalization.CultureInfo.CurrentCulture);
         var source = int.Parse(match.Groups["source"].Value, System.Globalization.CultureInfo.CurrentCulture) - 1;
         var target = int.Parse(match.Groups["target"].Value, System.Globalization.CultureInfo.CurrentCulture) - 1;

         var temp = new Stack<char>();
         for (var i = 0; i < count; ++i)
         {
            temp.Push(stacks[source].Pop());
         }
         for (var i = 0; i < count; ++i)
         {
            stacks[target].Push(temp.Pop());
         }
      }

      string answer = string.Empty;
      foreach (var stack in stacks)
      {
         answer += stack.Peek();
      }

      return answer;
   }

   [System.Text.RegularExpressions.GeneratedRegex(@"\s+")]
   private static partial System.Text.RegularExpressions.Regex MultipleSpacesRegex();

   [System.Text.RegularExpressions.GeneratedRegex(@"move (?<count>\d+) from (?<source>\d+) to (?<target>\d+)")]
   private static partial System.Text.RegularExpressions.Regex InstructionRegex();
}
