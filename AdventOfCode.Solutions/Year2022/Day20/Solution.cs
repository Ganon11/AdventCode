namespace AdventOfCode.Solutions.Year2022.Day20;

internal sealed class Solution : SolutionBase<int[]>
{
   public Solution() : base(20, 2022, "Grove Positioning System", false) { }

   public override int[] ParseInput(string input) => input.SplitByNewline(shouldTrim: true)
      .Select(line => int.Parse(line, System.Globalization.CultureInfo.CurrentCulture))
      .ToArray();

   public override string SolvePartOne()
   {
      var head = MyLinkedListNode.Create(this.ParsedInput);
      foreach (var number in this.ParsedInput)
      {
         if (number == 0)
         {
            continue;
         }

         var current = head;
         while (current.Data != number)
         {
            current = current.Next;
         }

         var actualNumber = number % this.ParsedInput.Length;

         var newPrev = current;
         if (number < 0)
         {
            for (var index = 0; index <= Math.Abs(actualNumber); ++index)
            {
               newPrev = newPrev.Previous;
            }
         }
         else if (number > 0)
         {
            for (var index = 0; index < actualNumber; ++index)
            {
               newPrev = newPrev.Next;
            }
         }
         // Remove from current spot
         var oldPrev = current.Previous;
         var oldNext = current.Next;
         oldPrev.Next = oldNext;
         oldNext.Previous = oldPrev;

         // Insert at new spot
         var newNext = newPrev.Next;
         newPrev.Next = current;
         current.Previous = newPrev;
         current.Next = newNext;
         newNext.Previous = current;
      }

      // Find 0 node
      var zeroNode = head;
      while (zeroNode.Data != 0)
      {
         zeroNode = zeroNode.Next;
      }

      var sum = 0;
      var counts = new int[]
      {
         1000 % this.ParsedInput.Length,
         2000 % this.ParsedInput.Length,
         3000 % this.ParsedInput.Length
      };

      for (var count = 0; count <= counts.Max(); ++count)
      {
         if (counts.Contains(count))
         {
            sum += zeroNode.Data;
         }

         zeroNode = zeroNode.Next;
      }

      return sum.ToString(System.Globalization.CultureInfo.CurrentCulture);
   }

   public override string SolvePartTwo()
   {
      return "";
   }
}

internal class MyLinkedListNode
{
   public int Data { get; init; }
   public MyLinkedListNode? Previous { get; set; }
   public MyLinkedListNode? Next { get; set; }

   public MyLinkedListNode(int data)
   {
      this.Data = data;
      this.Previous = null;
      this.Next = null;
   }

   public static MyLinkedListNode? Create(int[] data)
   {
      MyLinkedListNode? head = null;
      MyLinkedListNode? current = null;
      foreach (var number in data)
      {
         var newNode = new MyLinkedListNode(number);
         head ??= newNode;
         if (current != null)
         {
            current.Next = newNode;
            newNode.Previous = current;
         }

         current = newNode;
      }

      current.Next = head;
      head.Previous = current;
      return head;
   }
}
