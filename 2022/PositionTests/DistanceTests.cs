﻿namespace PositionTests
{
   [TestClass]
   public class DistanceTests
   {
      [TestMethod]
      public void Test_Manhattan_Distance_From_Origin()
      {
         Position.Position p = new(3, 5);
         Assert.AreEqual(8, p.ManhattanDistance());

         p = new(3, -5);
         Assert.AreEqual(8, p.ManhattanDistance());

         p = new(-3, -5);
         Assert.AreEqual(8, p.ManhattanDistance());

         p = new(-3, 5);
         Assert.AreEqual(8, p.ManhattanDistance());
      }

      [TestMethod]
      public void Test_Manhattan_Distance_From_Origin_With_Z()
      {
         Position.Position p = new(3, 5, 7);
         Assert.AreEqual(15, p.ManhattanDistance());

         p = new(3, -5, 7);
         Assert.AreEqual(15, p.ManhattanDistance());

         p = new(-3, -5, 7);
         Assert.AreEqual(15, p.ManhattanDistance());

         p = new(-3, 5, 7);
         Assert.AreEqual(15, p.ManhattanDistance());

         p = new(3, 5, -7);
         Assert.AreEqual(15, p.ManhattanDistance());

         p = new(3, -5, -7);
         Assert.AreEqual(15, p.ManhattanDistance());

         p = new(-3, -5, -7);
         Assert.AreEqual(15, p.ManhattanDistance());

         p = new(-3, 5, -7);
         Assert.AreEqual(15, p.ManhattanDistance());
      }

      [TestMethod]
      public void Test_Manhattan_Distance()
      {
         Position.Position p1 = new(3, 5);
         Position.Position p2 = new(-3, -5);
         Assert.AreEqual(p1.ManhattanDistance(p2), p2.ManhattanDistance(p1));
         Assert.AreEqual(16, p1.ManhattanDistance(p2));
      }

      [TestMethod]
      public void Test_Manhattan_Distance_Wiith_Z()
      {
         Position.Position p1 = new(1, 2, 3);
         Position.Position p2 = new(-1, 2, -3);
         Assert.AreEqual(p1.ManhattanDistance(p2), p2.ManhattanDistance(p1));
         Assert.AreEqual(8, p1.ManhattanDistance(p2));
      }
   }
}