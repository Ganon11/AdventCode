namespace AdventOfCode.Solutions.Year2022.Day22;

using AdventOfCode.Position;

internal sealed partial class Solution : SolutionBase<Tuple<Dictionary<Position, char>, string>>
{
   public Solution() : base(22, 2022, "Monkey Map", false) { }

   public override Tuple<Dictionary<Position, char>, string> ParseInput(string input)
   {
      var tokens = input.SplitByParagraph(shouldTrim: false);
      var map = new Dictionary<Position, char>();

      var row = 1;
      foreach (var line in tokens[0].SplitByNewline(shouldTrim: false))
      {
         var trimmedLine = line.Trim(new char[] { '\r', '\n' });
         for (var col = 1; col <= trimmedLine.Length; ++col)
         {
            if (trimmedLine[col - 1] != ' ')
            {
               map.Add(new Position(col, row), trimmedLine[col - 1]);
            }
         }

         ++row;
      }

      return new(map, tokens[1].Trim());
   }

   private static Dictionary<CubeState, CubeState> GetTeleports(bool debug)
   {
      var statePairs = new List<Tuple<CubeState, CubeState>>();
      if (debug)
      {
         // 1 to 2
         statePairs.Add(new(new CubeState(new(8, 1), Direction.North), new CubeState(new(4, 5), Direction.South)));
         statePairs.Add(new(new CubeState(new(9, 1), Direction.North), new CubeState(new(3, 5), Direction.South)));
         statePairs.Add(new(new CubeState(new(10, 1), Direction.North), new CubeState(new(2, 5), Direction.South)));
         statePairs.Add(new(new CubeState(new(11, 1), Direction.North), new CubeState(new(1, 5), Direction.South)));
         // 1 to 3
         statePairs.Add(new(new CubeState(new(8, 1), Direction.West), new CubeState(new(5, 5), Direction.South)));
         statePairs.Add(new(new CubeState(new(8, 2), Direction.West), new CubeState(new(6, 5), Direction.South)));
         statePairs.Add(new(new CubeState(new(8, 3), Direction.West), new CubeState(new(7, 5), Direction.South)));
         statePairs.Add(new(new CubeState(new(8, 4), Direction.West), new CubeState(new(8, 5), Direction.South)));
         // 1 to 6
         statePairs.Add(new(new CubeState(new(11, 1), Direction.East), new CubeState(new(12, 16), Direction.West)));
         statePairs.Add(new(new CubeState(new(11, 2), Direction.East), new CubeState(new(12, 15), Direction.West)));
         statePairs.Add(new(new CubeState(new(11, 3), Direction.East), new CubeState(new(12, 14), Direction.West)));
         statePairs.Add(new(new CubeState(new(11, 4), Direction.East), new CubeState(new(12, 13), Direction.West)));
      }
      else
      {

      }

      var teleports = new Dictionary<CubeState, CubeState>();
      foreach (var pair in statePairs)
      {
         teleports.Add(pair.Item1, pair.Item2);
         teleports.Add(pair.Item2, pair.Item1);
      }
      return teleports;
   }

   private static string PrintMap(Dictionary<Position, char> map)
   {
      var minRow = map.Keys.Select(p => p.Y).Min();
      var maxRow = map.Keys.Select(p => p.Y).Max();
      var minCol = map.Keys.Select(p => p.X).Min();
      var maxCol = map.Keys.Select(p => p.X).Max();
      var sb = new StringBuilder();

      for (var row = minRow; row <= maxRow; ++row)
      {
         for (var col = minCol; col <= maxCol; ++col)
         {
            if (map.TryGetValue(new Position(col, row), out var ch))
            {
               _ = sb.Append(ch);
            }
            else
            {
               _ = sb.Append(' ');
            }
         }
         _ = sb.AppendLine();
      }

      return sb.ToString();
   }

   private static Position Move(Position start, int count, Direction d, Dictionary<Position, char> map)
   {
      var p = start.Copy();
      for (var index = 0; index < count; ++index)
      {
         Position newPosition;
         switch (d)
         {
            case Direction.East:
               newPosition = p.East;
               if (!map.ContainsKey(newPosition))
               {
                  newPosition = map.Keys.Where(m => m.Y == start.Y).OrderBy(m => m.X).First();
               }

               if (map[newPosition] == '#')
               {
                  return p;
               }

               p = newPosition;
               break;
            case Direction.South:
               newPosition = p.North;
               if (!map.ContainsKey(newPosition))
               {
                  newPosition = map.Keys.Where(m => m.X == start.X).OrderBy(m => m.Y).First();
               }

               if (map[newPosition] == '#')
               {
                  return p;
               }

               p = newPosition;
               break;
            case Direction.West:
               newPosition = p.West;
               if (!map.ContainsKey(newPosition))
               {
                  newPosition = map.Keys.Where(m => m.Y == start.Y).OrderByDescending(m => m.X).First();
               }

               if (map[newPosition] == '#')
               {
                  return p;
               }

               p = newPosition;
               break;
            case Direction.North:
               newPosition = p.South;
               if (!map.ContainsKey(newPosition))
               {
                  newPosition = map.Keys.Where(m => m.X == start.X).OrderByDescending(m => m.Y).First();
               }

               if (map[newPosition] == '#')
               {
                  return p;
               }

               p = newPosition;
               break;
            default:
               throw new ArgumentException($"What direction is this?", nameof(d));
         }
      }

      return p;
   }

   private static Direction Turn(Direction d, char rotation) => rotation switch
   {
      'L' => d switch
      {
         Direction.East => Direction.North,
         Direction.South => Direction.East,
         Direction.West => Direction.South,
         Direction.North => Direction.West,
         _ => throw new ArgumentException($"What direction is this?", nameof(d)),
      },
      'R' => d switch
      {
         Direction.East => Direction.South,
         Direction.South => Direction.West,
         Direction.West => Direction.North,
         Direction.North => Direction.East,
         _ => throw new ArgumentException($"What direction is this?", nameof(d)),
      },
      _ => throw new ArgumentException($"What rotation is this?", nameof(rotation)),
   };

   private static long GetPassword(Position p, Direction d) => (1000 * p.Y) + (4 * p.X) + (long)d;

   public override string SolvePartOne()
   {
      var numbers = NonDigitsRegex().Split(this.ParsedInput.Item2).Select(s => int.Parse(s, System.Globalization.CultureInfo.CurrentCulture)).ToList();
      var turns = this.ParsedInput.Item2.Where(c => !char.IsDigit(c)).ToList();

      var startRow = this.ParsedInput.Item1.Keys.Select(p => p.Y).Min();
      var startCol = this.ParsedInput.Item1.Keys.Where(p => p.Y == startRow).Select(p => p.X).Min();
      var p = new Position(startCol, startRow);
      var d = Direction.East;

      var numberIndex = 0;
      var turnIndex = 0;
      while (numberIndex < numbers.Count && turnIndex < turns.Count)
      {
         p = Move(p, numbers[numberIndex++], d, this.ParsedInput.Item1);
         d = Turn(d, turns[turnIndex++]);
      }
      p = Move(p, numbers[numberIndex], d, this.ParsedInput.Item1);

      return GetPassword(p, d).ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      return "";
   }

   [System.Text.RegularExpressions.GeneratedRegex("\\D+")]
   private static partial System.Text.RegularExpressions.Regex NonDigitsRegex();
}

internal enum Direction
{
   East = 0,
   South = 1,
   West = 2,
   North = 3
}

internal sealed record CubeState
{
   public Position Position { get; set; }
   public Direction Direction { get; set; }

   public CubeState(Position position, Direction direction)
   {
      this.Position = position;
      this.Direction = direction;
   }
}
