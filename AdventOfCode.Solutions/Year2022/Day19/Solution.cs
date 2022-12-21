namespace AdventOfCode.Solutions.Year2022.Day19;

using AdventOfCode.Solutions.Year2022.Day16;

internal sealed class Solution : SolutionBase<Blueprint[]>
{
   public Solution() : base(19, 2022, "Not Enough Minerals", true) { }

   public override Blueprint[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line => new Blueprint(line))
      .ToArray();

   private const int TIME_LIMIT = 24;

   private static long MaxGeodes(Blueprint blueprint)
   {
      var start = new GeodeState
      {
         OreCache = 0,
         ClayCache = 0,
         ObsidianCache = 0,
         GeodeCache = 0,
         OreRobots = 1,
         ClayRobots = 0,
         ObsidianRobots = 0,
         GeodeRobots = 0
      };

      var frontier = new PriorityQueue<GeodeState, long>(new IntMaxCompare());
      frontier.Enqueue(start, start.GeodeRobots);

      var cameFrom = new Dictionary<GeodeState, GeodeState?>
      {
         [start] = null
      };

      var costSoFar = new Dictionary<GeodeState, long>
      {
         [start] = 0
      };

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();
         if (costSoFar[current] >= TIME_LIMIT)
         {
            continue;
         }

         var neighbors = new List<GeodeState>();
         var newCost = costSoFar[current] + 1;

         // Build a geode robot?
         // We have the resources to build a geod robot
         if (current.OreCache >= blueprint.RobotCosts[Resource.Geode][Resource.Ore] && current.ObsidianCache >= blueprint.RobotCosts[Resource.Geode][Resource.Obsidian])
         {
            neighbors.Add(new GeodeState
            {
               OreCache = current.OreCache - blueprint.RobotCosts[Resource.Geode][Resource.Ore] + current.OreRobots,
               ClayCache = current.ClayCache + current.ClayRobots,
               ObsidianCache = current.ObsidianCache - blueprint.RobotCosts[Resource.Geode][Resource.Obsidian] + current.ObsidianRobots,
               GeodeCache = current.GeodeCache + current.GeodeRobots,
               OreRobots = current.OreRobots,
               ClayRobots = current.ClayRobots,
               ObsidianRobots = current.ObsidianRobots,
               GeodeRobots = current.GeodeRobots + 1
            });
         }
         else if (current.OreCache >= blueprint.RobotCosts[Resource.Obsidian][Resource.Ore] && current.ClayCache >= blueprint.RobotCosts[Resource.Obsidian][Resource.Clay])
         {
            neighbors.Add(new GeodeState
            {
               OreCache = current.OreCache - blueprint.RobotCosts[Resource.Obsidian][Resource.Ore] + current.OreRobots,
               ClayCache = current.ClayCache - blueprint.RobotCosts[Resource.Obsidian][Resource.Clay] + current.ClayRobots,
               ObsidianCache = current.ObsidianCache + current.ObsidianRobots,
               GeodeCache = current.GeodeCache + current.GeodeRobots,
               OreRobots = current.OreRobots,
               ClayRobots = current.ClayRobots,
               ObsidianRobots = current.ObsidianRobots + 1,
               GeodeRobots = current.GeodeRobots
            });
         }
         else if (current.OreCache >= blueprint.RobotCosts[Resource.Clay][Resource.Ore])
         {
            neighbors.Add(new GeodeState
            {
               OreCache = current.OreCache - blueprint.RobotCosts[Resource.Clay][Resource.Ore] + current.OreRobots,
               ClayCache = current.ClayCache + current.ClayRobots,
               ObsidianCache = current.ObsidianCache + current.ObsidianRobots,
               GeodeCache = current.GeodeCache + current.GeodeRobots,
               OreRobots = current.OreRobots,
               ClayRobots = current.ClayRobots + 1,
               ObsidianRobots = current.ObsidianRobots,
               GeodeRobots = current.GeodeRobots
            });
         }
         else if (current.OreCache >= blueprint.RobotCosts[Resource.Ore][Resource.Ore])
         {
            neighbors.Add(new GeodeState
            {
               OreCache = current.OreCache - blueprint.RobotCosts[Resource.Ore][Resource.Ore] + current.OreRobots,
               ClayCache = current.ClayCache + current.ClayRobots,
               ObsidianCache = current.ObsidianCache + current.ObsidianRobots,
               GeodeCache = current.GeodeCache + current.GeodeRobots,
               OreRobots = current.OreRobots + 1,
               ClayRobots = current.ClayRobots,
               ObsidianRobots = current.ObsidianRobots,
               GeodeRobots = current.GeodeRobots
            });
         }

         // If we can't build a robot, wait it out
         if (!neighbors.Any())
         {
            neighbors.Add(new GeodeState
            {
               OreCache = current.OreCache + current.OreRobots,
               ClayCache = current.ClayCache + current.ClayRobots,
               ObsidianCache = current.ObsidianCache + current.ObsidianRobots,
               GeodeCache = current.GeodeCache + current.GeodeRobots,
               OreRobots = current.OreRobots,
               ClayRobots = current.ClayRobots,
               ObsidianRobots = current.ObsidianRobots,
               GeodeRobots = current.GeodeRobots
            });
         }

         // Add new states
         foreach (var neighbor in neighbors)
         {
            if (!costSoFar.TryGetValue(neighbor, out var existingCost) || newCost < existingCost)
            {
               costSoFar[neighbor] = newCost;
               var priority = newCost + current.GeodeRobots;
               frontier.Enqueue(neighbor, priority);
               cameFrom[neighbor] = current;
            }
         }
      }

      return costSoFar.Keys.Max(state => state.GeodeCache);
   }

   public override string SolvePartOne()
   {
      long sum = 0;
      foreach (var blueprint in this.ParsedInput)
      {
         long maxGeodes = MaxGeodes(blueprint);
         long qualityLevel = blueprint.Id * maxGeodes;
         sum += qualityLevel;
      }

      return sum.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      return "";
   }
}

internal enum Resource
{
   Ore,
   Clay,
   Obsidian,
   Geode
}

internal class Blueprint
{
   public int Id { get; init; }
   public Dictionary<Resource, Dictionary<Resource, long>> RobotCosts { get; init; }

   public Blueprint(string line)
   {
      var tokens = line.Split(' ');

      this.Id = int.Parse(tokens[1].Trim(':'), System.Globalization.CultureInfo.CurrentCulture);

      var oreRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[6], System.Globalization.CultureInfo.CurrentCulture) },
      };
      var clayRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[12], System.Globalization.CultureInfo.CurrentCulture) },
      };
      var obsidianRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[18], System.Globalization.CultureInfo.CurrentCulture) },
         { Resource.Clay, long.Parse(tokens[21], System.Globalization.CultureInfo.CurrentCulture) }
      };
      var geodeRobotCost = new Dictionary<Resource, long>
      {
         { Resource.Ore, long.Parse(tokens[27], System.Globalization.CultureInfo.CurrentCulture) },
         { Resource.Obsidian, long.Parse(tokens[30], System.Globalization.CultureInfo.CurrentCulture) }
      };

      this.RobotCosts = new Dictionary<Resource, Dictionary<Resource, long>>
      {
         { Resource.Ore, oreRobotCost },
         { Resource.Clay, clayRobotCost },
         { Resource.Obsidian, obsidianRobotCost },
         { Resource.Geode, geodeRobotCost }
      };
   }
}

internal class GeodeState : IEquatable<GeodeState>
{
   public long OreCache { get; init; }
   public long ClayCache { get; init; }
   public long ObsidianCache { get; init; }
   public long GeodeCache { get; init; }

   public long OreRobots { get; init; }
   public long ClayRobots { get; init; }
   public long ObsidianRobots { get; init;}
   public long GeodeRobots { get; init; }

   public bool Equals(GeodeState? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.OreRobots == other.OreRobots &&
         this.ClayRobots == other.ClayRobots &&
         this.ObsidianRobots == other.ObsidianRobots &&
         this.GeodeRobots == other.GeodeRobots &&
         this.OreCache == other.OreCache &&
         this.ClayCache == other.ClayCache &&
         this.ObsidianCache == other.ObsidianCache &&
         this.GeodeCache == other.GeodeCache;
   }

   public override bool Equals(object? obj) => this.Equals(obj as GeodeState);

   public override int GetHashCode() => new Tuple<long, long, long, long, long, long, Tuple<long, long>>(
      this.OreRobots,
      this.ClayRobots,
      this.ObsidianRobots,
      this.GeodeRobots,
      this.OreCache,
      this.ClayCache,
      new(this.ObsidianCache, this.GeodeCache)
      ).GetHashCode();
}
