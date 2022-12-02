using Day2;

string filepath;
if (args.Length == 0)
{
   filepath = "input/input.txt";
}
else
{
   filepath = args[0];
}

var lines = File.ReadAllLines(filepath);
int total = 0;
int intendedTotal = 0;
foreach (var line in lines)
{
   RPSRound round = new RPSRound(line);
   total += round.Score;
   intendedTotal += round.IntendedScore;
}

Console.WriteLine($"Your score: {total}");
Console.WriteLine($"Your intended score: {intendedTotal}");

namespace Day2
{
   public enum RPSChoice
   {
      Rock = 1,
      Paper = 2,
      Scissors = 3
   }

   public enum RPSResult
   {
      Loss = 0,
      Draw = 3,
      Won = 6
   }

   public class RPSRound
   {
      public RPSChoice Theirs { get; private set; }

      public RPSChoice Yours { get; private set; }
      public RPSResult Result { get; private set; }
      public int Score => (int)this.Yours + (int)this.Result;

      public RPSResult IntendedResult { get; private set; }
      public RPSChoice IntendedChoice { get; private set; }
      public int IntendedScore => (int)this.IntendedChoice + (int)this.IntendedResult;

      public RPSRound(string line)
      {
         var tokens = line.Split(' ');
         switch (tokens[0])
         {
            case "A":
               this.Theirs = RPSChoice.Rock;
               break;
            case "B":
               this.Theirs = RPSChoice.Paper;
               break;
            case "C":
               this.Theirs = RPSChoice.Scissors;
               break;
            default:
               break;
         }

         switch (tokens[1])
         {
            case "X":
               this.Yours = RPSChoice.Rock;
               this.IntendedResult = RPSResult.Loss;
               break;
            case "Y":
               this.Yours = RPSChoice.Paper;
               this.IntendedResult = RPSResult.Draw;
               break;
            case "Z":
               this.Yours = RPSChoice.Scissors;
               this.IntendedResult = RPSResult.Won;
               break;
            default:
               break;
         }

         switch (this.Theirs)
         {
            case RPSChoice.Rock:
               switch (this.Yours)
               {
                  case RPSChoice.Rock:
                     this.Result = RPSResult.Draw;
                     break;
                  case RPSChoice.Paper:
                     this.Result = RPSResult.Won;
                     break;
                  case RPSChoice.Scissors:
                     this.Result = RPSResult.Loss;
                     break;
                  default:
                     break;
               }

               switch (this.IntendedResult)
               {
                  case RPSResult.Loss:
                     this.IntendedChoice = RPSChoice.Scissors;
                     break;
                  case RPSResult.Draw:
                     this.IntendedChoice = RPSChoice.Rock;
                     break;
                  case RPSResult.Won:
                     this.IntendedChoice = RPSChoice.Paper;
                     break;
                  default:
                     break;
               }

               break;
            case RPSChoice.Paper:
               switch (this.Yours)
               {
                  case RPSChoice.Rock:
                     this.Result = RPSResult.Loss;
                     break;
                  case RPSChoice.Paper:
                     this.Result = RPSResult.Draw;
                     break;
                  case RPSChoice.Scissors:
                     this.Result = RPSResult.Won;
                     break;
                  default:
                     break;
               }

               switch (this.IntendedResult)
               {
                  case RPSResult.Loss:
                     this.IntendedChoice = RPSChoice.Rock;
                     break;
                  case RPSResult.Draw:
                     this.IntendedChoice = RPSChoice.Paper;
                     break;
                  case RPSResult.Won:
                     this.IntendedChoice = RPSChoice.Scissors;
                     break;
                  default:
                     break;
               }
               break;
            case RPSChoice.Scissors:
               switch (this.Yours)
               {
                  case RPSChoice.Rock:
                     this.Result = RPSResult.Won;
                     break;
                  case RPSChoice.Paper:
                     this.Result = RPSResult.Loss;
                     break;
                  case RPSChoice.Scissors:
                     this.Result = RPSResult.Draw;
                     break;
                  default:
                     break;
               }

               switch (this.IntendedResult)
               {
                  case RPSResult.Loss:
                     this.IntendedChoice = RPSChoice.Paper;
                     break;
                  case RPSResult.Draw:
                     this.IntendedChoice = RPSChoice.Scissors;
                     break;
                  case RPSResult.Won:
                     this.IntendedChoice = RPSChoice.Rock;
                     break;
                  default:
                     break;
               }
               break;
            default:
               this.Result = RPSResult.Loss;
               this.IntendedChoice = RPSChoice.Scissors;
               break;
         }
      }
   }
}
