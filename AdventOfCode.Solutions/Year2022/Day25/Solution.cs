namespace AdventOfCode.Solutions.Year2022.Day25;

internal sealed class Solution : SolutionBase<IEnumerable<string>>
{
   public Solution() : base(25, 2022, "Full of Hot Air", false) { }

   public override IEnumerable<string> ParseInput(string input) => input.SplitByNewline(shouldTrim: true);

   private static long DecodeSnafu(string snafu)
   {
      long sum = 0;
      long product = 1;
      foreach (var c in snafu.Reverse())
      {
         switch (c)
         {
            case '2':
               sum += 2 * product;
               break;
            case '1':
               sum += product;
               break;
            case '-':
               sum -= product;
               break;
            case '=':
               sum -= 2 * product;
               break;
            default:
               break;
         }

         product *= 5;
      }

      return sum;
   }

   private static readonly char[] SnafuDigits = new[] { '=', '-', '0', '1', '2' };

   private static string EncodeSnafu(long number)
   {
      if (number == 0)
      {
         return string.Empty;
      }

      long quotient = (number + 2) / 5;
      long remainder = (number + 2) % 5;
      return EncodeSnafu(quotient) + SnafuDigits[remainder];
   }

   public override string SolvePartOne()
   {
      var sum = this.ParsedInput
         .Select(DecodeSnafu)
         .Sum();
      var answer = EncodeSnafu(sum);
      var check = DecodeSnafu(answer);

      return answer.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo() => "Merry Christmas!";
}
