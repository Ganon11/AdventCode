using System.Numerics;

namespace AdventOfCode.Solutions.Year2021.Day03;

class Solution : SolutionBase<string[]>
{
   public Solution() : base(03, 2021, "Binary Diagnostic") { }

   public override string[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true);

   private static int[] GetBitCounts(string[] diagnostics)
   {
      var bitLength = diagnostics.First().Length;
      var bitcounts = Enumerable.Repeat(0, bitLength).ToArray();
      foreach (var diagnostic in diagnostics)
      {
         for (var index = 0; index < bitLength; ++index)
         {
            if (diagnostic[index] == '1')
            {
               bitcounts[index]++;
            }
         }
      }

      return bitcounts;
   }

   private static int GetSimpleRate(string[] diagnostics, Func<int, float, bool> condition)
   {
      var bitLength = diagnostics.First().Length;
      var bitcounts = GetBitCounts(diagnostics);
      var bits = new string('0', bitLength).ToCharArray();
      for (var index = 0; index < bitLength; ++index)
      {
         if (condition(bitcounts[index], diagnostics.Length / 2.0f))
         {
            bits[index] = '1';
         }
      }

      return Convert.ToInt32(string.Join("", bits), 2);
   }

   private static int GetComplexRate(string[] diagnostics, Func<int, float, bool> condition)
   {
      var bitLength = diagnostics.First().Length;
      string[] candidates = new string[diagnostics.Length];
      Array.Copy(diagnostics, candidates, diagnostics.Length);

      for (var index = 0; index < bitLength; ++index)
      {
         var count = candidates.Length;
         if (count == 1)
         {
            break;
         }

         var bitcounts = GetBitCounts(candidates);
         var new_bit = condition(bitcounts[index], count / 2.0f) ? '0' : '1';
         candidates = candidates.Where(diagnostic => diagnostic[index] == new_bit).ToArray();
      }

      return Convert.ToInt32(candidates.First(), 2);
   }

   public override string SolvePartOne()
   {
      int gamma = GetSimpleRate(ParsedInput, (a, b) => a >= b);
      int epsilon = GetSimpleRate(ParsedInput, (a, b) => a <= b);
      return (gamma * epsilon).ToString();
   }

   public override string SolvePartTwo()
   {
      int o2 = GetComplexRate(ParsedInput, (a, b) => a < b);
      int co2 = GetComplexRate(ParsedInput, (a, b) => a >= b);
      return (co2 * o2).ToString();
   }
}
