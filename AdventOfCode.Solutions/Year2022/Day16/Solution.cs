namespace AdventOfCode.Solutions.Year2022.Day16;

using System.Text.RegularExpressions;

internal sealed class Solution : SolutionBase<Dictionary<long, Valve>>
{
   public Solution() : base(16, 2022, "Proboscidea Volcanium", true) { }

   public override Dictionary<long, Valve> ParseInput(string input)
   {
      var lines = input.SplitByNewline(shouldTrim: true);
      var valves = lines.Select(l => new Valve(l));
      var valveNames = valves.Select(v => v.Name).ToList();
      valveNames.Sort();
      foreach (var valve in valves)
      {
         valve.Id = (long)1 << valveNames.IndexOf(valve.Name);
         foreach (var adjacent in valve.AdjacentValveNames)
         {
            _ = valve.AdjacentValves.Add((long)1 << valveNames.IndexOf(adjacent));
         }
      }

      return valves.ToDictionary(v => v.Id);
   }

   private static readonly Dictionary<(long, long), int> PrecomputedCosts = new();
   private static int CostToActivate(long source, long destination, Dictionary<long, Valve> valves)
   {
      if (PrecomputedCosts.TryGetValue((source, destination), out var value))
      {
         return value;
      }

      var frontier = new Queue<long>();
      frontier.Enqueue(source);
      var cameFrom = new Dictionary<long, long>
      {
         { source, -1 }
      };

      while (frontier.Any())
      {
         var current = frontier.Dequeue();
         foreach (var next in valves[current].AdjacentValves)
         {
            if (!cameFrom.ContainsKey(next))
            {
               frontier.Enqueue(next);
               cameFrom[next] = current;
            }
         }
      }

      var cost = 1;
      var currentUnwind = destination;
      while (!currentUnwind.Equals(source))
      {
         ++cost;
         currentUnwind = cameFrom[currentUnwind];
      }

      PrecomputedCosts.Add((source, destination), cost);
      PrecomputedCosts.Add((destination, source), cost);
      return cost;
   }

   private static IEnumerable<SearchState> CalculateScores(Dictionary<long, Valve> valves, int timeLimit)
   {
      var maxScore = long.MinValue;
      var usefulValves = valves.Where(kvp => kvp.Value.FlowRate > 0)
         .Select(kvp => kvp.Key)
         .ToHashSet();

      var start = new SearchState(1, 0, 0);

      var frontier = new PriorityQueue<SearchState, long>(new IntMaxCompare());
      frontier.Enqueue(start, 0);

      var cameFrom = new Dictionary<SearchState, SearchState?>
      {
         { start, null }
      };

      var costSoFar = new Dictionary<SearchState, int>
      {
         { start, 0 }
      };

      while (frontier.Count > 0)
      {
         var current = frontier.Dequeue();

         if (current.TotalPressure > maxScore)
         {
            maxScore = current.TotalPressure;
         }

         foreach (var usefulValve in usefulValves)
         {
            if ((usefulValve & current.OpenValves) > 0)
            {
               continue;
            }

            var newCost = costSoFar[current] + CostToActivate(current.CurrentValve, usefulValve, valves);
            if (newCost > timeLimit)
            {
               continue;
            }

            long remainingTime = timeLimit - newCost;

            //var newOpenValves = new HashSet<string>(current.OpenValves);
            //_ = newOpenValves.Add(usefulValve);
            var newOpenValves = current.OpenValves | usefulValve;

            var newScore = current.TotalPressure + (valves[usefulValve].FlowRate * remainingTime);

            var newState = new SearchState(usefulValve, newOpenValves, newScore);
            if (!costSoFar.TryGetValue(newState, out var existingCost) || newCost < existingCost)
            {
               costSoFar[newState] = newCost;
               var newPriority = newCost;
               frontier.Enqueue(newState, newPriority);
               cameFrom[newState] = current;
            }
         }
      }

      return cameFrom.Keys.OrderByDescending(state => state.TotalPressure).ToArray();
   }

   public override string SolvePartOne()
   {
      var scores = CalculateScores(this.ParsedInput, 30);
      return scores.First().TotalPressure.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      var scores = CalculateScores(this.ParsedInput, 26);
      var maxScore = long.MinValue;
      // Search top % of scores
      var scoreFactor = this.Debug ? 1.0 : 0.1;
      var maxScoreIndex = scoreFactor * scores.Count();
      for (var index = 0; index < maxScoreIndex; ++index)
      {
         for (var index2 = index + 1; index2 < maxScoreIndex; ++index2)
         {
            var state1 = scores.ElementAt(index);
            var state2 = scores.ElementAt(index2);
            if ((state1.OpenValves & state2.OpenValves) > 0)
            {
               continue;
            }

            if (state1.TotalPressure + state2.TotalPressure > maxScore)
            {
               maxScore = state1.TotalPressure + state2.TotalPressure;
            }
         }
      }

      return maxScore.ToString(System.Globalization.CultureInfo.CurrentCulture);
      //return "";
   }
}

internal sealed class IntMaxCompare : IComparer<long>
{
   public int Compare(long x, long y) => y.CompareTo(x);
}

internal sealed partial class Valve : IEquatable<Valve>
{
   public long Id { get; set; }
   public string Name { get; init; }
   public int FlowRate { get; init; }
   public HashSet<string> AdjacentValveNames { get; init; }
   public HashSet<long> AdjacentValves { get; set; }

   public Valve(string line)
   {
      var m = ValveRegex().Match(line);
      if (m.Success)
      {
         this.Name = m.Groups["name"].Value;
         this.FlowRate = int.Parse(m.Groups["flowRate"].Value, System.Globalization.CultureInfo.CurrentCulture);
         this.AdjacentValveNames = m.Groups["valves"].Value.Split(", ").ToHashSet();
         this.AdjacentValves = new HashSet<long>();
      }
      else
      {
         throw new ArgumentException($"Line {line} improperly formatted.", nameof(line));
      }
   }

   bool IEquatable<Valve>.Equals(Valve? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.Name.Equals(other.Name, StringComparison.OrdinalIgnoreCase);
   }

   [GeneratedRegex("Valve (?<name>..) has flow rate=(?<flowRate>\\d+); tunnels? leads? to valves? (?<valves>.+)")]
   private static partial Regex ValveRegex();

   public override bool Equals(object obj) => ((IEquatable<Valve>)this).Equals(obj as Valve);
}

internal sealed record SearchState : IEquatable<SearchState>
{
   public long CurrentValve { get; init; }
   public long OpenValves { get; init; }
   public long TotalPressure { get; init; }

   public SearchState(long current, long valves, long pressure)
   {
      this.CurrentValve = current;
      this.OpenValves = valves;
      this.TotalPressure = pressure;
   }

   bool IEquatable<SearchState>.Equals(SearchState? other)
   {
      if (other == null)
      {
         return false;
      }

      return this.CurrentValve == other.CurrentValve && this.OpenValves == other.OpenValves;
   }
}
