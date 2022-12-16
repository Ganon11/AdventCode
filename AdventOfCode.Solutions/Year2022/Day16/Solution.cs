namespace AdventOfCode.Solutions.Year2022.Day16;

using System.Text.RegularExpressions;

internal sealed class Solution : SolutionBase<Dictionary<string, Valve>>
{
   public Solution() : base(16, 2022, "Proboscidea Volcanium", false) { }

   public override Dictionary<string, Valve> ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
         .Select(line => new Valve(line))
         .ToDictionary(valve => valve.Name);

   private static readonly Dictionary<(string, string), int> PrecomputedCosts = new();
   private static int CostToActivate(string source, string destination, Dictionary<string, Valve> valves)
   {
      if (PrecomputedCosts.TryGetValue((source, destination), out var value))
      {
         return value;
      }

      var frontier = new Queue<string>();
      frontier.Enqueue(source);
      var cameFrom = new Dictionary<string, string>
      {
         { source, string.Empty }
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
      while (!currentUnwind.Equals(source, StringComparison.OrdinalIgnoreCase))
      {
         ++cost;
         currentUnwind = cameFrom[currentUnwind];
      }

      PrecomputedCosts.Add((source, destination), cost);
      PrecomputedCosts.Add((destination, source), cost);
      return cost;
   }

   private static long Score(IEnumerable<string> valveOrder, Dictionary<string, Valve> valves)
   {
      var score = 0;
      var timeLeft = 30;
      for (var index = 1; index < valveOrder.Count(); ++index)
      {
         timeLeft -= CostToActivate(valveOrder.ElementAt(index - 1), valveOrder.ElementAt(index), valves);
         if (timeLeft < 0)
         {
            return score;
         }

         score += valves[valveOrder.ElementAt(index)].FlowRate * timeLeft;
      }

      return score;
   }

   public override string SolvePartOne()
   {
      var maxScore = long.MinValue;
      var usefulValves = this.ParsedInput
         .Where(kvp => kvp.Value.FlowRate > 0)
         .Select(kvp => kvp.Key)
         .ToList();
      var permutations = usefulValves.Permutations().ToList();
      foreach (var permutation in permutations)
      {
         var order = permutation.Prepend("AA");
         var score = Score(order, this.ParsedInput);
         if (score > maxScore)
         {
            maxScore = score;
         }
      }

      return maxScore.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      return "";
   }
}

internal sealed partial record Valve : IEquatable<Valve>
{
   public string Name { get; init; }
   public int FlowRate { get; init; }
   public HashSet<string> AdjacentValves { get; init; }

   public Valve(string line)
   {
      var m = ValveRegex().Match(line);
      if (m.Success)
      {
         this.Name = m.Groups["name"].Value;
         this.FlowRate = int.Parse(m.Groups["flowRate"].Value, System.Globalization.CultureInfo.CurrentCulture);
         this.AdjacentValves = m.Groups["valves"].Value.Split(", ").ToHashSet();
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
}
