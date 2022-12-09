namespace AdventOfCode.Solutions.Year2021.Day02;

internal sealed class Solution : SolutionBase<Tuple<string, int>[]>
{
   public Solution() : base(02, 2021, "Dive!") { }

   public override Tuple<string, int>[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line =>
      {
         var tokens = line.Split(' ');
         return new Tuple<string, int>(tokens[0], int.Parse(tokens[1], System.Globalization.CultureInfo.CurrentCulture));
      })
      .ToArray();

   public override string SolvePartOne()
   {
      int horizontalPosition = 0;
      int depth = 0;
      foreach (var instruction in ParsedInput)
      {
         switch (instruction.Item1)
         {
            case "forward":
               horizontalPosition += instruction.Item2;
               break;
            case "down":
               depth += instruction.Item2;
               break;
            case "up":
               depth -= instruction.Item2;
               break;
            default:
               break;
         }
      }

      return (horizontalPosition * depth).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      int horizontalPosition = 0;
      int depth = 0;
      int aim = 0;
      foreach (var instruction in ParsedInput)
      {
         switch (instruction.Item1)
         {
            case "forward":
               horizontalPosition += instruction.Item2;
               depth += instruction.Item2 * aim;
               break;
            case "down":
               aim += instruction.Item2;
               break;
            case "up":
               aim -= instruction.Item2;
               break;
            default:
               break;
         }
      }

      return (horizontalPosition * depth).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
