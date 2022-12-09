namespace AdventOfCode.Solutions.Year2022.Day09;

using AdventOfCode.Position;

internal class Solution : SolutionBase<Tuple<char, int>[]>
{
   public Solution() : base(09, 2022, "Rope Bridge") { }

   public override Tuple<char, int>[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line =>
      {
         var tokens = line.Split(' ');
         return new Tuple<char, int>(tokens[0][0], int.Parse(tokens[1], System.Globalization.CultureInfo.CurrentCulture));
      })
      .ToArray();

   private static void MoveRope(Position[] positions, char direction)
   {
      switch (direction)
      {
         case 'U':
            positions[0] = positions[0].North;
            break;
         case 'D':
            positions[0] = positions[0].South;
            break;
         case 'L':
            positions[0] = positions[0].West;
            break;
         case 'R':
            positions[0] = positions[0].East;
            break;
         default:
            break;
      }

      for (var index = 1; index < positions.Length; ++index)
      {
         if (positions[index - 1].X == positions[index].X && (positions[index - 1].Y - 2) == positions[index].Y)
         {
            positions[index] = positions[index].North;
         }
         else if (positions[index - 1].X == positions[index].X && (positions[index - 1].Y + 2) == positions[index].Y)
         {
            positions[index] = positions[index].South;
         }
         else if (positions[index - 1].Y == positions[index].Y && (positions[index - 1].X - 2) == positions[index].X)
         {
            positions[index] = positions[index].East;
         }
         else if (positions[index - 1].Y == positions[index].Y && (positions[index - 1].X + 2) == positions[index].X)
         {
            positions[index] = positions[index].West;
         }
         else if (!positions[index - 1].IsAdjacentTo(positions[index], includeDiagonals: true))
         {
            if (positions[index].X < positions[index - 1].X && positions[index].Y < positions[index - 1].Y)
            {
               positions[index] = positions[index].North.East;
            }
            else if (positions[index].X < positions[index - 1].X && positions[index].Y > positions[index - 1].Y)
            {
               positions[index] = positions[index].South.East;
            }
            else if (positions[index].X > positions[index - 1].X && positions[index].Y < positions[index - 1].Y)
            {
               positions[index] = positions[index].North.West;
            }
            else if (positions[index].X > positions[index - 1].X && positions[index].Y > positions[index - 1].Y)
            {
               positions[index] = positions[index].South.West;
            }
         }
      }
   }

   private static int MoveRopeWithKnots(int knotCount, Tuple<char, int>[] instructions)
   {
      var positions = Enumerable.Repeat(new Position(), knotCount).ToArray();

      var tailPositions = new HashSet<Position>
      {
         positions.Last()
      };

      foreach (var instruction in instructions)
      {
         for (var count = 0; count < instruction.Item2; ++count)
         {
            MoveRope(positions, instruction.Item1);
            _ = tailPositions.Add(positions.Last());
         }
      }

      return tailPositions.Count;
   }

   public override string SolvePartOne() => MoveRopeWithKnots(2, this.ParsedInput).ToString(System.Globalization.CultureInfo.CurrentCulture);

   public override string SolvePartTwo() => MoveRopeWithKnots(10, this.ParsedInput).ToString(System.Globalization.CultureInfo.CurrentCulture);
}
