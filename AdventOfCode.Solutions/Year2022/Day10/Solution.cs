namespace AdventOfCode.Solutions.Year2022.Day10;

using AdventOfCode.Position;

internal sealed class Solution : SolutionBase<IInstruction[]>
{
   public Solution() : base(10, 2022, "Cathode-Ray Tube") { }

   public override IInstruction[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line =>
      {
         if (line.Equals("noop", StringComparison.OrdinalIgnoreCase))
         {
            return new Noop();
         }
         else
         {
            var value = int.Parse(line.Split(' ').Last(), System.Globalization.CultureInfo.CurrentCulture);
            return new Add(value) as IInstruction;
         }
      }).ToArray();

   public override string SolvePartOne()
   {
      var total = 0;
      var program = new Program(p =>
      {
         if ((p.Cycle + 20) % 40 != 0)
         {
            return;
         }

         total += p.X * p.Cycle;
      });

      foreach (var instruction in this.ParsedInput)
      {
         instruction.Execute(program);
      }

      return total.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   private static string RenderCrtDisplay(Dictionary<Position, char> pixels, int rows, int cols)
   {
      var sb = new StringBuilder();
      _ = sb.AppendLine();
      for (var row = 0; row < rows; ++row)
      {
         for (var col = 0; col < cols; ++col)
         {
            var key = new Position(x: col, y: row);
            if (pixels.TryGetValue(key, out var value))
            {
               _ = sb.Append(value);
            }
         }
         _ = sb.AppendLine();
      }

      return sb.ToString();
   }

   public override string SolvePartTwo()
   {
      var crtDisplay = new Dictionary<Position, char>();
      var program = new Program(p =>
      {
         var validPositions = Enumerable.Range(p.X - 1, 3);
         var pixelPosition = (p.Cycle - 1) % 40;
         var screenPosition = new Position(x: pixelPosition, y: p.Cycle / 40);
         char newPixel;
         if (validPositions.Contains(pixelPosition))
         {
            newPixel = '#';
         }
         else
         {
            newPixel = ' ';
         }
         crtDisplay.Add(screenPosition, newPixel);
      });

      foreach (var instruction in this.ParsedInput)
      {
         instruction.Execute(program);
      }

      return RenderCrtDisplay(crtDisplay, 6, 40);
   }
}

internal sealed class Program
{
   public int Cycle { get; set; }
   public int X { get; set; } = 1;
   private readonly Action<Program> callback;

   public Program(Action<Program> callback) => this.callback = callback;

   public void AdvanceBy(int count)
   {
      for (var index = 0; index < count; ++index)
      {
         ++this.Cycle;
         this.callback(this);
      }
   }
}

internal interface IInstruction
{
   void Execute(Program p);
}

internal sealed class Noop : IInstruction
{
   public void Execute(Program p) => p.AdvanceBy(1);
}

internal sealed class Add : IInstruction
{
   public int Value { get; init; }

   public Add(int value) => this.Value = value;

   public void Execute(Program p)
   {
      p.AdvanceBy(2);
      p.X += this.Value;
   }
}
