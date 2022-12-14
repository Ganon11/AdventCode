namespace AdventOfCode.Solutions;

public static class SolutionCollector
{
   public static IEnumerable<ISolution> FetchSolutions(int year, IEnumerable<int> days)
   {
      if (days.Sum() == 0)
      {
         days = Enumerable.Range(1, 25).ToArray();
      }

      foreach (var day in days)
      {
         var type = Type.GetType($"AdventOfCode.Solutions.Year{year}.Day{day:D2}.Solution");
         if (type != null)
         {
            if (type.GetConstructors().Length != 1)
            {
               throw new InvalidOperationException($"Solution for {year} day {day:D2} does not have one constructor");
            }

            var constructor = type.GetConstructors()[0];
            if (constructor == null)
            {
               throw new InvalidOperationException($"Solution for {year} day {day:D2} does not have one constructor");
            }

            if (constructor.Invoke(constructor.GetParameters().ToArray()) is ISolution solution)
            {
               yield return solution;
            }
         }
      }
   }
}
