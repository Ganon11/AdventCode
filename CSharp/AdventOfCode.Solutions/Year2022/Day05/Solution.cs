using System.Text.RegularExpressions;

namespace AdventOfCode.Solutions.Year2022.Day05;

partial class Solution : SolutionBase
{
   public Solution() : base(05, 2022, "") { }

   protected override string SolvePartOne()
   {
      var parsedInput = ParseInput(Input);
      var stacks = parsedInput.Item1;

      foreach (var instruction in parsedInput.Item2)
      {
         for (var i = 0; i < instruction.Count; ++i)
         {
            stacks[instruction.Target].Push(stacks[instruction.Source].Pop());
         }
      }

      return GetTopCrateFromStacks(stacks);
   }

   protected override string SolvePartTwo()
   {
      var parsedInput = ParseInput(Input);
      var stacks = parsedInput.Item1;

      foreach (var instruction in parsedInput.Item2)
      {
         var temp = new Stack<char>();
         for (var i = 0; i < instruction.Count; ++i)
         {
            temp.Push(stacks[instruction.Source].Pop());
         }
         for (var i = 0; i < instruction.Count; ++i)
         {
            stacks[instruction.Target].Push(temp.Pop());
         }
      }

      return GetTopCrateFromStacks(stacks);
   }

   [GeneratedRegex(@"\s+")]
   private static partial Regex MultipleSpacesRegex();

   [GeneratedRegex(@"move (?<count>\d+) from (?<source>\d+) to (?<target>\d+)")]
   private static partial Regex InstructionRegex();

   public sealed record Instruction
   {
      public int Count { get; private set; }
      public int Source { get; private set; }
      public int Target { get; private set; }

      public Instruction(string line)
      {
         var match = InstructionRegex().Match(line);
         if (match.Success)
         {
            this.Count = int.Parse(match.Groups["count"].Value, System.Globalization.CultureInfo.CurrentCulture);
            this.Source = int.Parse(match.Groups["source"].Value, System.Globalization.CultureInfo.CurrentCulture) - 1;
            this.Target = int.Parse(match.Groups["target"].Value, System.Globalization.CultureInfo.CurrentCulture) - 1;
         }
         else
         {
            throw new ArgumentException($"Line was not in expected format", nameof(line));
         }
      }
   }
   private Tuple<Stack<char>[], Instruction[]> ParseInput(string rawInput)
   {
      rawInput = rawInput.Replace("\r", "");
      var tokens = rawInput.Split("\n\n");

      var reversedLines = tokens[0].Split("\n").Reverse();
      var numStacks = MultipleSpacesRegex().Split(reversedLines.First().Trim()).Length;
      var stacks = new Stack<char>[numStacks];
      for (var stackNum = 0; stackNum < numStacks; ++stackNum)
      {
         stacks[stackNum] = new Stack<char>();
      }

      foreach (var line in reversedLines.Skip(1))
      {
         for (var stackNum = 0; stackNum <= numStacks; ++stackNum)
         {
            var charIndex = (stackNum * 4) + 1;
            if (charIndex >= line.Length)
            {
               continue;
            }
            else if (char.IsUpper(line[charIndex]))
            {
               stacks[stackNum].Push(line[charIndex]);
            }
         }
      }

      var instructions = tokens[1].Split("\n")
         .Where(line => line.Any())
         .Select(line => new Instruction(line))
         .ToArray();

      return new(stacks, instructions);
   }

   private static string GetTopCrateFromStacks(Stack<char>[] stacks)
   {
      var answer = string.Empty;
      foreach (var stack in stacks)
      {
         answer += stack.Peek();
      }

      return answer;
   }
}
