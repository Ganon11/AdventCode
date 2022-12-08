namespace AdventOfCode.PositionTests;

[TestClass]
public class GetPointsInLineTests
{
   [TestMethod]
   public void Test_PointsInLineEmptyForInvalidLines()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(2, 4, 9);

      var points = Position.Position.GetPointsInLine(p1, p2);
      Assert.IsFalse(points.Any());

      p1 = new(1, 2, 3);
      p2 = new(1, 4, 9);

      points = Position.Position.GetPointsInLine(p1, p2);
      Assert.IsFalse(points.Any());

      p1 = new(1, 2, 3);
      p2 = new(2, 2, 9);

      points = Position.Position.GetPointsInLine(p1, p2);
      Assert.IsFalse(points.Any());

      p1 = new(1, 2, 3);
      p2 = new(2, 4, 3);

      points = Position.Position.GetPointsInLine(p1, p2);
      Assert.IsFalse(points.Any());
   }

   [TestMethod]
   public void Test_PointsInHorizontalLine()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(4, 2, 3);

      var subpositions = new List<Position.Position>() {
          p1,
          new Position.Position(2, 2, 3),
          new Position.Position(3, 2, 3),
          p2
       };
      var line = Position.Position.GetPointsInLine(p1, p2).ToList();
      CollectionAssert.AreEqual(subpositions, line);
   }

   [TestMethod]
   public void Test_PointsInVerticalLine()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(1, 5, 3);

      var subpositions = new List<Position.Position>() {
          p1,
          new Position.Position(1, 3, 3),
          new Position.Position(1, 4, 3),
          p2
       };
      var line = Position.Position.GetPointsInLine(p1, p2).ToList();
      CollectionAssert.AreEqual(subpositions, line);
   }

   [TestMethod]
   public void Test_PointsInStackedLine()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(1, 2, 6);

      var subpositions = new List<Position.Position>() {
          p1,
          new Position.Position(1, 2, 4),
          new Position.Position(1, 2, 5),
          p2
       };
      var line = Position.Position.GetPointsInLine(p1, p2).ToList();
      CollectionAssert.AreEqual(subpositions, line);
   }

   [TestMethod]
   public void Test_PointsInXYDiagonalLine()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(4, 5, 3);

      var subpositions = new List<Position.Position>() {
          p1,
          new Position.Position(2, 3, 3),
          new Position.Position(3, 4, 3),
          p2
       };
      var line = Position.Position.GetPointsInLine(p1, p2).ToList();
      CollectionAssert.AreEqual(subpositions, line);
   }

   [TestMethod]
   public void Test_PointsInXZDiagonalLine()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(4, 2, 6);

      var subpositions = new List<Position.Position>() {
          p1,
          new Position.Position(2, 2, 4),
          new Position.Position(3, 2, 5),
          p2
       };
      var line = Position.Position.GetPointsInLine(p1, p2).ToList();
      CollectionAssert.AreEqual(subpositions, line);
   }

   [TestMethod]
   public void Test_PointsInYZDiagonalLine()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(1, 5, 6);

      var subpositions = new List<Position.Position>() {
          p1,
          new Position.Position(1, 3, 4),
          new Position.Position(1, 4, 5),
          p2
       };
      var line = Position.Position.GetPointsInLine(p1, p2).ToList();
      CollectionAssert.AreEqual(subpositions, line);
   }

   [TestMethod]
   public void Test_PointsInXYZDiagonalLine()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(4, 5, 6);

      var subpositions = new List<Position.Position>() {
          p1,
          new Position.Position(2, 3, 4),
          new Position.Position(3, 4, 5),
          p2
       };
      var line = Position.Position.GetPointsInLine(p1, p2).ToList();
      CollectionAssert.AreEqual(subpositions, line);
   }

   [TestMethod]
   public void Test_PointsInLine_OrderDoesNotMatter()
   {
      Position.Position p1 = new(1, 2, 3);
      Position.Position p2 = new(1, 2, 6);
      var line1 = Position.Position.GetPointsInLine(p1, p2);
      var line2 = Position.Position.GetPointsInLine(p2, p1);
      Assert.IsTrue(line1.All(p => line2.Contains(p)));
   }

   [TestMethod]
   public void Test_PointsInLine_OnePoint()
   {
      Position.Position p = new(1, 2, 3);
      var line = Position.Position.GetPointsInLine(p, p);
      Assert.IsTrue(line.Any());
      Assert.AreEqual(1, line.Count());
      Assert.IsTrue(line.Contains(p));
   }
}
