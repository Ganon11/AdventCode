namespace AdventOfCode.Solutions.Year2021.Day04;

class Solution : SolutionBase<Tuple<int[], Solution.BingoBoard[]>>
{
  public Solution() : base(04, 2021, "Giant Squid") { }

  private static IEnumerable<int> TakeTurn(int number, ref BingoBoard[] boards, HashSet<int> ignored = null)
  {
    var winningIndices = new List<int>();
    for (var index = 0; index < boards.Length; ++index)
    {
      if (ignored != null && ignored.Contains(index))
      {
        continue;
      }

      boards[index].NumberCalled(number);
      if (boards[index].IsWinning())
      {
        winningIndices.Add(index);
      }
    }

    return winningIndices;
  }

  public override string SolvePartOne()
  {
    BingoBoard[] boards = new BingoBoard[ParsedInput.Item2.Length];
    Array.Copy(ParsedInput.Item2, boards, ParsedInput.Item2.Length);
    foreach (var number in ParsedInput.Item1)
    {
      var winners = TakeTurn(number, ref boards);
      if (winners.Any())
      {
        return boards[winners.First()]
          .GetScore(number)
          .ToString();
      }
    }

    return "";
  }

  public override string SolvePartTwo()
  {
    HashSet<int> ignored = new HashSet<int>();
    BingoBoard[] boards = new BingoBoard[ParsedInput.Item2.Length];
    List<BingoBoard> winningBoards = new List<BingoBoard>();
    Array.Copy(ParsedInput.Item2, boards, ParsedInput.Item2.Length);
    int calledNumber = -1;
    foreach (var number in ParsedInput.Item1)
    {
      calledNumber = number;
      var winners = TakeTurn(number, ref boards, ignored);
      if (winners.Any())
      {
        ignored.UnionWith(winners);
        winningBoards.AddRange(winners.Select(index => boards[index]));
        if (winningBoards.Count() == boards.Length)
        {
          break;
        }
      }
    }

    return winningBoards.Last().GetScore(calledNumber).ToString();
  }

  public override Tuple<int[], BingoBoard[]> ParseInput(string input)
  {
    var tokens = input.SplitByParagraph(shouldTrim: true);
    var numbers = tokens[0].Split(',')
      .Select(number => int.Parse(number, System.Globalization.CultureInfo.CurrentCulture))
      .ToArray();
    List<BingoBoard> boards = new List<BingoBoard>();
    foreach (var boardString in tokens.Skip(1))
    {
      var lines = boardString.SplitByNewline(shouldTrim: true);
      boards.Add(new BingoBoard(lines));
    }
    return new(numbers, boards.ToArray());
  }

  public record BingoSquare
  {
    public int Number { get; init; }
    public bool Picked { get; set; }

    public BingoSquare(int number)
    {
      this.Number = number;
      this.Picked = false;
    }
  }

  public class BingoBoard
  {
    public BingoSquare[][] Board { get; init; }

    public BingoBoard(string[] lines)
    {
      Board = lines.Select(line => System.Text.RegularExpressions.Regex.Split(line, @"\s+")
          .Select(num => new BingoSquare(int.Parse(num, System.Globalization.CultureInfo.CurrentCulture)))
          .ToArray()).ToArray();
    }

    public void NumberCalled(int number)
    {
      for (var row = 0; row < Board.Length; ++row)
      {
        for (var col = 0; col < Board[row].Length; ++col)
        {
          if (Board[row][col].Number == number)
          {
            Board[row][col].Picked = true;
            return;
          }
        }
      }
    }

    public bool IsWinning()
    {
      // Winning Row
      foreach (var row in this.Board)
      {
        if (row.All(s => s.Picked))
        {
          return true;
        }
      }

      // Winning Column
      for (var column = 0; column < 5; ++column)
      {
        bool allPicked = true;
        for (var row = 0; row < 5; ++row)
        {
          if (!this.Board[row][column].Picked)
          {
            allPicked = false;
            break;
          }
        }

        if (allPicked)
        {
          return true;
        }
      }

      return false;
    }

    public int GetScore(int number) => Board.Sum(row => row.Where(sq => !sq.Picked).Sum(sq => sq.Number)) * number;
  }
}
