namespace AdventOfCode.Solutions.Year2022.Day10;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<string[]>
{
   public Solution() : base(10, 2022, "Cathode-Ray Tube") { }

   public override string[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true);

   public override string SolvePartOne()
   {
      var x = 1;
      var isAdding = false;
      var eip = 0;
      var indices = new HashSet<int>()
      {
         20,
         60,
         100,
         140,
         180,
         220
      };
      var total = 0;
      for (var clock = 1; clock <= 220; ++clock)
      {
         if (indices.Contains(clock))
         {
            total += clock * x;
         }

         if (this.ParsedInput.Length <= eip)
         {
            Console.WriteLine($"X is {x}, clock is {clock}");
            Console.WriteLine($"eip is {eip}, past last instruction");
            break;
         }
         if (isAdding)
         {
            x += int.Parse(this.ParsedInput[eip++].Split(' ').Last(), System.Globalization.CultureInfo.CurrentCulture);
            isAdding = false;
         }
         else if (this.ParsedInput[eip].Equals("noop", StringComparison.Ordinal))
         {
            ++eip;
         }
         else
         {
            isAdding = true;
         }
      }

      return total.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   private static string RenderCrtDisplay(HashSet<Position> litPixels, int rows, int cols)
   {
      var sb = new StringBuilder();
      sb.AppendLine();
      for (var row = 0; row < rows; ++row)
      {
         for (var col = 1; col <= cols; ++col)
         {
            if (litPixels.Contains(new Position(x: col, y: row)))
            {
               sb.Append('#');
            }
            else
            {
               sb.Append('.');
            }
         }
         sb.AppendLine();
      }

      return sb.ToString();
   }

   public override string SolvePartTwo()
   {
      var isAdding = false;
      var eip = 0;
      var crtDisplay = new HashSet<Position>();
      var sprite = new Position(x: 1, y: -1);
      for (var clock = 1; clock <= 240; ++clock)
      {
         var pixelPosition = (clock % 40) - 1;
         if (pixelPosition == 0)
         {
            sprite.Y += 1;
         }

         // sprite.X + 1 <= pixelPosition <= sprite.X - 1
         if (pixelPosition >= sprite.X - 1 && pixelPosition <= sprite.X + 1)
         {
            crtDisplay.Add(new Position(x: pixelPosition, y: sprite.Y));
         }

         if (isAdding)
         {
            var deltaX = int.Parse(this.ParsedInput[eip++].Split(' ').Last(), System.Globalization.CultureInfo.CurrentCulture);
            sprite.X += deltaX;
            isAdding = false;
         }
         else if (this.ParsedInput[eip].Equals("noop", StringComparison.Ordinal))
         {
            ++eip;
         }
         else
         {
            isAdding = true;
         }
      }

      return RenderCrtDisplay(crtDisplay, 6, 40);
   }
}
