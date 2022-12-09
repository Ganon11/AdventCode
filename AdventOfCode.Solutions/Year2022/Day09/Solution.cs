namespace AdventOfCode.Solutions.Year2022.Day09;

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

   public override string SolvePartOne()
   {
      var head = new Position.Position();
      var tail = new Position.Position();

      var tailPositions = new HashSet<Position.Position>
      {
         tail
      };

      foreach (var instruction in this.ParsedInput)
      {
         for (var count = 0; count < instruction.Item2; ++count)
         {
            switch (instruction.Item1)
            {
               case 'U':
                  head = head.North;
                  break;
               case 'D':
                  head = head.South;
                  break;
               case 'L':
                  head = head.West;
                  break;
               case 'R':
                  head = head.East;
                  break;
               default:
                  break;
            }

            if (head.X == tail.X && (head.Y - 2) == tail.Y)
            {
               tail = tail.North;
            }
            else if (head.X == tail.X && (head.Y + 2) == tail.Y)
            {
               tail = tail.South;
            }
            else if (head.Y == tail.Y && (head.X - 2) == tail.X)
            {
               tail = tail.East;
            }
            else if (head.Y == tail.Y && (head.X + 2) == tail.X)
            {
               tail = tail.West;
            }
            else if (!head.IsAdjacentTo(tail, includeDiagonals: true))
            {
               if (tail.X < head.X && tail.Y < head.Y)
               {
                  tail = tail.North.East;
               }
               else if (tail.X < head.X && tail.Y > head.Y)
               {
                  tail = tail.South.East;
               }
               else if (tail.X > head.X && tail.Y < head.Y)
               {
                  tail = tail.North.West;
               }
               else if (tail.X > head.X && tail.Y > head.Y)
               {
                  tail = tail.South.West;
               }
            }
            _ = tailPositions.Add(tail);
         }
      }

      return tailPositions.Count.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var positions = Enumerable.Repeat(new Position.Position(), 10).ToArray();

      var tailPositions = new HashSet<Position.Position>
      {
         positions.Last()
      };

      foreach (var instruction in this.ParsedInput)
      {
         for (var count = 0; count < instruction.Item2; ++count)
         {
            switch (instruction.Item1)
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

            _ = tailPositions.Add(positions.Last());
         }
      }

      return tailPositions.Count.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
