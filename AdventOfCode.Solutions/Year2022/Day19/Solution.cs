namespace AdventOfCode.Solutions.Year2022.Day19;
internal sealed class Solution : SolutionBase<Blueprint[]>
{
   public Solution() : base(19, 2022, "Not Enough Minerals", false) { }

   public override Blueprint[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line => new Blueprint(line))
      .ToArray();

   private static long MaxGeodes(Blueprint blueprint, int timeLimit)
   {
      long maxGeodes = 0;
      var queue = new PriorityQueue<ProductionState, long>();
      queue.Enqueue(new ProductionState(), 0);

      while (queue.Count > 0)
      {
         var state = queue.Dequeue();
         foreach (var next in state.CalculateNextStates(blueprint, timeLimit))
         {
            queue.Enqueue(next, next.Cache[Resource.Geode]);
         }

         maxGeodes = Math.Max(maxGeodes, state.Cache[Resource.Geode]);
      }

      return maxGeodes;
   }

   public override string SolvePartOne()
   {
      long sum = 0;
      foreach (var blueprint in this.ParsedInput)
      {
         var maxGeodes = MaxGeodes(blueprint, 24);
         sum += blueprint.Id * maxGeodes;
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

internal sealed class RobotBlueprint
{
   public Dictionary<Resource, long> Production { get; set; } = new Dictionary<Resource, long>();
   public Dictionary<Resource, long> Costs { get; set; } = new Dictionary<Resource, long>();

   private int TimeUntilBuild(ProductionState state)
   {
      var times = new List<int>();

      if (this.Costs[Resource.Ore] <= state.Cache[Resource.Ore])
      {
         times.Add(0);
      }
      else
      {
         times.Add((int)Math.Ceiling((this.Costs[Resource.Ore] - state.Cache[Resource.Ore]) / (double)state.Robots[Resource.Ore]));
      }

      if (this.Costs[Resource.Clay] <= state.Cache[Resource.Clay])
      {
         times.Add(0);
      }
      else
      {
         times.Add((int)Math.Ceiling((this.Costs[Resource.Clay] - state.Cache[Resource.Clay]) / (double)state.Robots[Resource.Clay]));
      }

      if (this.Costs[Resource.Obsidian] <= state.Cache[Resource.Obsidian])
      {
         times.Add(0);
      }
      else
      {
         times.Add((int)Math.Ceiling((this.Costs[Resource.Obsidian] - state.Cache[Resource.Obsidian]) / (double)state.Robots[Resource.Obsidian]));
      }

      return times.Max() + 1;
   }

   public ProductionState ScheduleBuild(ProductionState state)
   {
      var timeRequired = this.TimeUntilBuild(state);
      var newState = new ProductionState
      {
         Time = state.Time + timeRequired
      };

      foreach (var resource in Enum.GetValues(typeof(Resource)).Cast<Resource>())
      {
         newState.Cache[resource] = state.Cache[resource] - this.Costs[resource] + (timeRequired * state.Robots[resource]);
         newState.Robots[resource] = state.Robots[resource] + this.Production[resource];
      }

      return newState;
   }
}

internal sealed class Blueprint
{
   public int Id { get; init; }
   public Dictionary<Resource, RobotBlueprint> Blueprints { get; init; }
   public Dictionary<Resource, long> MaxCounts { get; internal set; }

   public Blueprint(string line)
   {
      var tokens = line.Split(' ');

      this.Id = int.Parse(tokens[1].Trim(':'), System.Globalization.CultureInfo.CurrentCulture);

      var oreRobotBlueprint = new RobotBlueprint
      {
         Production = new Dictionary<Resource, long>
         {
            { Resource.Ore, 1 },
            { Resource.Clay, 0 },
            { Resource.Obsidian, 0 },
            { Resource.Geode, 0 }
         },
         Costs = new Dictionary<Resource, long>
         {
            { Resource.Ore, long.Parse(tokens[6], System.Globalization.CultureInfo.CurrentCulture) },
            { Resource.Clay, 0 },
            { Resource.Obsidian, 0 },
            { Resource.Geode, 0 }
         }
      };

      var clayRobotBlueprint = new RobotBlueprint
      {
         Production = new Dictionary<Resource, long>
         {
            { Resource.Ore, 0 },
            { Resource.Clay, 1 },
            { Resource.Obsidian, 0 },
            { Resource.Geode, 0 }
         },
         Costs = new Dictionary<Resource, long>
         {
            { Resource.Ore, long.Parse(tokens[12], System.Globalization.CultureInfo.CurrentCulture) },
            { Resource.Clay, 0 },
            { Resource.Obsidian, 0 },
            { Resource.Geode, 0 }
         }
      };

      var obsidianRobotBlueprint = new RobotBlueprint
      {
         Production = new Dictionary<Resource, long>
         {
            { Resource.Ore, 0 },
            { Resource.Clay, 0 },
            { Resource.Obsidian, 1 },
            { Resource.Geode, 0 }
         },
         Costs = new Dictionary<Resource, long>
         {
            { Resource.Ore, long.Parse(tokens[18], System.Globalization.CultureInfo.CurrentCulture) },
            { Resource.Clay, long.Parse(tokens[21], System.Globalization.CultureInfo.CurrentCulture) },
            { Resource.Obsidian, 0 },
            { Resource.Geode, 0 }
         }
      };

      var geodeRobotBlueprint = new RobotBlueprint
      {
         Production = new Dictionary<Resource, long>
         {
            { Resource.Ore, 0 },
            { Resource.Clay, 0 },
            { Resource.Obsidian, 0 },
            { Resource.Geode, 1 }
         },
         Costs = new Dictionary<Resource, long>
         {
            { Resource.Ore, long.Parse(tokens[27], System.Globalization.CultureInfo.CurrentCulture) },
            { Resource.Clay, 0 },
            { Resource.Obsidian, long.Parse(tokens[30], System.Globalization.CultureInfo.CurrentCulture) },
            { Resource.Geode, 0 }
         }
      };

      this.Blueprints = new Dictionary<Resource, RobotBlueprint>
      {
         { Resource.Ore, oreRobotBlueprint },
         { Resource.Clay, clayRobotBlueprint },
         { Resource.Obsidian, obsidianRobotBlueprint },
         { Resource.Geode, geodeRobotBlueprint }
      };

      this.MaxCounts = new Dictionary<Resource, long>
      {
         { Resource.Ore, this.Blueprints.Select(kvp => kvp.Value.Costs[Resource.Ore]).Max() },
         { Resource.Clay, this.Blueprints.Select(kvp => kvp.Value.Costs[Resource.Clay]).Max() },
         { Resource.Obsidian, this.Blueprints.Select(kvp => kvp.Value.Costs[Resource.Obsidian]).Max() }
      };
   }
}

internal sealed class ProductionState : IComparable<ProductionState>
{
   public int Time { get; set; } = 1;
   public Dictionary<Resource, long> Cache { get; init; }
   public Dictionary<Resource, long> Robots { get; init; }

   public ProductionState()
   {
      this.Cache = new Dictionary<Resource, long>
      {
         { Resource.Ore, 1 },
         { Resource.Clay, 0 },
         { Resource.Obsidian, 0 },
         { Resource.Geode, 0 }
      };

      this.Robots = new Dictionary<Resource, long>
      {
         { Resource.Ore, 1 },
         { Resource.Clay, 0 },
         { Resource.Obsidian, 0 },
         { Resource.Geode, 0 }
      };
   }

   public IEnumerable<ProductionState> CalculateNextStates(Blueprint blueprint, int timeBudget)
   {
      var result = new List<ProductionState>();
      if (this.Time < timeBudget)
      {
         if (blueprint.MaxCounts[Resource.Ore] > this.Robots[Resource.Ore] && this.Cache[Resource.Ore] > 0)
         {
            result.Add(blueprint.Blueprints[Resource.Ore].ScheduleBuild(this));
         }

         if (blueprint.MaxCounts[Resource.Clay] > this.Robots[Resource.Clay] && this.Cache[Resource.Ore] > 0)
         {
            result.Add(blueprint.Blueprints[Resource.Clay].ScheduleBuild(this));
         }

         if (blueprint.MaxCounts[Resource.Obsidian] > this.Robots[Resource.Obsidian] && this.Cache[Resource.Ore] > 0 && this.Cache[Resource.Clay] > 0)
         {
            result.Add(blueprint.Blueprints[Resource.Obsidian].ScheduleBuild(this));
         }

         if (this.Cache[Resource.Ore] > 0 && this.Cache[Resource.Obsidian] > 0)
         {
            result.Add(blueprint.Blueprints[Resource.Geode].ScheduleBuild(this));
         }
      }

      return result.Where(state => state.Time <= timeBudget);
   }

   public int CompareTo(ProductionState? other) => other.Cache[Resource.Geode].CompareTo(this.Cache[Resource.Geode]);
}
