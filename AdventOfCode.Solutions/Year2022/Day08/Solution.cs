namespace AdventOfCode.Solutions.Year2022.Day08;

internal sealed class Solution : SolutionBase<char[][]>
{
   public Solution() : base(08, 2022, "Treetop Tree House") { }

   public override char[][] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line => line.Trim().ToCharArray())
      .ToArray();

   private static int CountVisible(char[][] trees)
   {
      var visibleTrees = new HashSet<Position.Position>();
      var height = trees.Length;
      var width = trees.First().Length;

      // Check from top edge going down
      for (var col = 0; col < width; ++col)
      {
         var max = -1;
         for (var row = 0; row < height; ++row)
         {
            if (trees[row][col] > max)
            {
               _ = visibleTrees.Add(new Position.Position(col - '0', row - '0'));
               max = trees[row][col];
            }
         }
      }

      // Check from bottom edge going up
      for (var col = 0; col < width; ++col)
      {
         var max = -1;
         for (var row = height - 1; row >= 0; --row)
         {
            if (trees[row][col] > max)
            {
               _ = visibleTrees.Add(new Position.Position(col - '0', row - '0'));
               max = trees[row][col];
            }
         }
      }

      // Check from left edge going right
      for (var row = 0; row < height; ++row)
      {
         var max = -1;
         for (var col = 0; col < width; ++col)
         {
            if (trees[row][col] > max)
            {
               _ = visibleTrees.Add(new Position.Position(col - '0', row - '0'));
               max = trees[row][col];
            }
         }
      }

      // Check from right edge going left
      for (var row = 0; row < height; ++row)
      {
         var max = -1;
         for (var col = width - 1; col >= 0; --col)
         {
            if (trees[row][col] > max)
            {
               _ = visibleTrees.Add(new Position.Position(col - '0', row - '0'));
               max = trees[row][col];
            }
         }
      }

      return visibleTrees.Count;
   }

   public override string SolvePartOne() => CountVisible(this.ParsedInput).ToString(System.Globalization.CultureInfo.CurrentCulture);

   private static int CalculateScenicScore(char[][] trees, int row, int col)
   {
      static int CalculateScore(char[][] trees, int row, int col, int deltaRow, int deltaCol)
      {
         var score = 0;
         var r = row + deltaRow;
         var c = col + deltaCol;
         while (r >= 0 && r < trees.Length && c >= 0 && c < trees[r].Length)
         {
            ++score;
            if (trees[r][c] >= trees[row][col])
            {
               break;
            }

            r += deltaRow;
            c += deltaCol;
         }

         return score;
      }

      var upScore = CalculateScore(trees, row, col, -1, 0);
      var downScore = CalculateScore(trees, row, col, 1, 0);
      var leftScore = CalculateScore(trees, row, col, 0, -1);
      var rightScore = CalculateScore(trees, row, col, 0, 1);

      return upScore * downScore * leftScore * rightScore;
   }

   public override string SolvePartTwo()
   {
      var height = this.ParsedInput.Length;
      var width = this.ParsedInput.First().Length;
      var bestScenicScore = -1;

      for (var row = 1; row < (height - 1); ++row)
      {
         for (var col = 1; col < (width - 1); ++col)
         {
            var scenicScore = CalculateScenicScore(this.ParsedInput, row, col);
            if (scenicScore > bestScenicScore)
            {
               bestScenicScore = scenicScore;
            }
         }
      }

      return bestScenicScore.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }
}
