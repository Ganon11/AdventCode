namespace AdventOfCode.PositionTests;

[TestClass]
public class AdjacencyTests
{
   [TestMethod]
   public void Test_North()
   {
      Position.Position origin = new();
      var p = origin.North;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(1, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_South()
   {
      Position.Position origin = new();
      var p = origin.South;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(-1, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_East()
   {
      Position.Position origin = new();
      var p = origin.East;
      Assert.AreEqual(1, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_West()
   {
      Position.Position origin = new();
      var p = origin.West;
      Assert.AreEqual(-1, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_Up()
   {
      Position.Position origin = new();
      var p = origin.Up;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(1, p.Z);
   }

   [TestMethod]
   public void Test_Down()
   {
      Position.Position origin = new();
      var p = origin.Down;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(-1, p.Z);
   }

   [TestMethod]
   public void Test_NorthEast()
   {
      Position.Position origin = new();
      var p = origin.NorthEast;
      Assert.AreEqual(1, p.X);
      Assert.AreEqual(1, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_NorthWest()
   {
      Position.Position origin = new();
      var p = origin.NorthWest;
      Assert.AreEqual(-1, p.X);
      Assert.AreEqual(1, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_NorthUp()
   {
      Position.Position origin = new();
      var p = origin.NorthUp;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(1, p.Y);
      Assert.AreEqual(1, p.Z);
   }

   [TestMethod]
   public void Test_NorthDown()
   {
      Position.Position origin = new();
      var p = origin.NorthDown;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(1, p.Y);
      Assert.AreEqual(-1, p.Z);
   }

   [TestMethod]
   public void Test_SouthEast()
   {
      Position.Position origin = new();
      var p = origin.SouthEast;
      Assert.AreEqual(1, p.X);
      Assert.AreEqual(-1, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_SouthWest()
   {
      Position.Position origin = new();
      var p = origin.SouthWest;
      Assert.AreEqual(-1, p.X);
      Assert.AreEqual(-1, p.Y);
      Assert.AreEqual(0, p.Z);
   }

   [TestMethod]
   public void Test_SouthUp()
   {
      Position.Position origin = new();
      var p = origin.SouthUp;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(-1, p.Y);
      Assert.AreEqual(1, p.Z);
   }

   [TestMethod]
   public void Test_SouthDown()
   {
      Position.Position origin = new();
      var p = origin.SouthDown;
      Assert.AreEqual(0, p.X);
      Assert.AreEqual(-1, p.Y);
      Assert.AreEqual(-1, p.Z);
   }

   [TestMethod]
   public void Test_EastUp()
   {
      Position.Position origin = new();
      var p = origin.EastUp;
      Assert.AreEqual(1, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(1, p.Z);
   }

   [TestMethod]
   public void Test_EastDown()
   {
      Position.Position origin = new();
      var p = origin.EastDown;
      Assert.AreEqual(1, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(-1, p.Z);
   }

   [TestMethod]
   public void Test_WestUp()
   {
      Position.Position origin = new();
      var p = origin.WestUp;
      Assert.AreEqual(-1, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(1, p.Z);
   }

   [TestMethod]
   public void Test_WestDown()
   {
      Position.Position origin = new();
      var p = origin.WestDown;
      Assert.AreEqual(-1, p.X);
      Assert.AreEqual(0, p.Y);
      Assert.AreEqual(-1, p.Z);
   }

   [TestMethod]
   public void Test_Self_Adjacency()
   {
      Position.Position origin = new();
      Assert.IsFalse(origin.IsAdjacentTo(origin));
   }

   [TestMethod]
   public void Test_Simple_Adjacency()
   {
      Position.Position origin = new();
      var n = origin.North;
      var s = origin.South;
      var e = origin.East;
      var w = origin.West;
      var u = origin.Up;
      var d = origin.Down;

      Assert.IsTrue(origin.IsAdjacentTo(n));
      Assert.IsTrue(n.IsAdjacentTo(origin));
      Assert.IsTrue(origin.IsAdjacentTo(s));
      Assert.IsTrue(s.IsAdjacentTo(origin));
      Assert.IsTrue(origin.IsAdjacentTo(e));
      Assert.IsTrue(e.IsAdjacentTo(origin));
      Assert.IsTrue(origin.IsAdjacentTo(w));
      Assert.IsTrue(w.IsAdjacentTo(origin));
      Assert.IsTrue(origin.IsAdjacentTo(u));
      Assert.IsTrue(u.IsAdjacentTo(origin));
      Assert.IsTrue(origin.IsAdjacentTo(d));
      Assert.IsTrue(d.IsAdjacentTo(origin));

      Assert.IsFalse(n.IsAdjacentTo(s));
      Assert.IsFalse(n.IsAdjacentTo(e));
      Assert.IsFalse(n.IsAdjacentTo(w));
      Assert.IsFalse(n.IsAdjacentTo(u));
      Assert.IsFalse(n.IsAdjacentTo(d));

      Assert.IsFalse(s.IsAdjacentTo(n));
      Assert.IsFalse(s.IsAdjacentTo(e));
      Assert.IsFalse(s.IsAdjacentTo(w));
      Assert.IsFalse(s.IsAdjacentTo(u));
      Assert.IsFalse(s.IsAdjacentTo(d));

      Assert.IsFalse(e.IsAdjacentTo(n));
      Assert.IsFalse(e.IsAdjacentTo(s));
      Assert.IsFalse(e.IsAdjacentTo(w));
      Assert.IsFalse(e.IsAdjacentTo(u));
      Assert.IsFalse(e.IsAdjacentTo(d));

      Assert.IsFalse(w.IsAdjacentTo(n));
      Assert.IsFalse(w.IsAdjacentTo(s));
      Assert.IsFalse(w.IsAdjacentTo(e));
      Assert.IsFalse(w.IsAdjacentTo(u));
      Assert.IsFalse(w.IsAdjacentTo(d));

      Assert.IsFalse(u.IsAdjacentTo(n));
      Assert.IsFalse(u.IsAdjacentTo(s));
      Assert.IsFalse(u.IsAdjacentTo(e));
      Assert.IsFalse(u.IsAdjacentTo(w));
      Assert.IsFalse(u.IsAdjacentTo(d));

      Assert.IsFalse(d.IsAdjacentTo(n));
      Assert.IsFalse(d.IsAdjacentTo(s));
      Assert.IsFalse(d.IsAdjacentTo(e));
      Assert.IsFalse(d.IsAdjacentTo(w));
      Assert.IsFalse(d.IsAdjacentTo(u));

      var ne = origin.NorthEast;
      var nw = origin.NorthWest;
      var nu = origin.NorthUp;
      var nd = origin.NorthDown;
      var se = origin.SouthEast;
      var sw = origin.SouthWest;
      var su = origin.SouthUp;
      var sd = origin.SouthDown;

      Assert.IsFalse(origin.IsAdjacentTo(ne));
      Assert.IsFalse(origin.IsAdjacentTo(nw));
      Assert.IsFalse(origin.IsAdjacentTo(nu));
      Assert.IsFalse(origin.IsAdjacentTo(nd));
      Assert.IsFalse(origin.IsAdjacentTo(se));
      Assert.IsFalse(origin.IsAdjacentTo(sw));
      Assert.IsFalse(origin.IsAdjacentTo(su));
      Assert.IsFalse(origin.IsAdjacentTo(sd));
   }

   [TestMethod]
   public void Test_Diagonal_Adjacency()
   {
      Position.Position origin = new();

      var unw = origin.NorthWestUp;
      var un = origin.NorthUp;
      var une = origin.NorthEastUp;
      var uw = origin.WestUp;
      var u = origin.Up;
      var ue = origin.EastUp;
      var usw = origin.SouthWestUp;
      var us = origin.SouthUp;
      var use = origin.SouthEastUp;

      var nw = origin.NorthWest;
      var n = origin.North;
      var ne = origin.NorthEast;
      var w = origin.West;
      var e = origin.East;
      var sw = origin.SouthWest;
      var s = origin.South;
      var se = origin.SouthEast;

      var dnw = origin.NorthWestDown;
      var dn = origin.NorthDown;
      var dne = origin.NorthEastDown;
      var dw = origin.WestDown;
      var d = origin.Down;
      var de = origin.EastDown;
      var dsw = origin.SouthWestDown;
      var ds = origin.SouthDown;
      var dse = origin.SouthEastDown;

      Assert.IsTrue(origin.IsAdjacentTo(unw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(un, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(une, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(uw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(u, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(ue, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(usw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(us, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(use, includeDiagonals: true));

      Assert.IsTrue(origin.IsAdjacentTo(nw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(n, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(ne, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(w, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(e, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(sw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(s, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(se, includeDiagonals: true));

      Assert.IsTrue(origin.IsAdjacentTo(dnw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(dn, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(dne, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(dw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(d, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(de, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(dsw, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(ds, includeDiagonals: true));
      Assert.IsTrue(origin.IsAdjacentTo(dse, includeDiagonals: true));

      // A few non-adjacent positions
      Assert.IsFalse(origin.IsAdjacentTo(origin.Up.Up, includeDiagonals: true));
      Assert.IsFalse(origin.IsAdjacentTo(origin.North.North, includeDiagonals: true));
      Assert.IsFalse(origin.IsAdjacentTo(origin.West.West, includeDiagonals: true));
      Assert.IsFalse(origin.IsAdjacentTo(origin.East.East, includeDiagonals: true));
      Assert.IsFalse(origin.IsAdjacentTo(origin.South.South, includeDiagonals: true));
      Assert.IsFalse(origin.IsAdjacentTo(origin.Down.Down, includeDiagonals: true));
   }

   [TestMethod]
   public void Test_Simple_Adjacent_Positions()
   {
      Position.Position origin = new();
      var n = origin.North;
      var s = origin.South;
      var e = origin.East;
      var w = origin.West;
      var u = origin.Up;
      var d = origin.Down;

      var adjacentPositions = origin.GetAdjacentPositions();
      Assert.AreEqual(6, adjacentPositions.Count());
      Assert.IsTrue(adjacentPositions.Contains(n));
      Assert.IsTrue(adjacentPositions.Contains(s));
      Assert.IsTrue(adjacentPositions.Contains(e));
      Assert.IsTrue(adjacentPositions.Contains(w));
      Assert.IsTrue(adjacentPositions.Contains(u));
      Assert.IsTrue(adjacentPositions.Contains(d));
      Assert.IsFalse(adjacentPositions.Contains(origin));
   }

   [TestMethod]
   public void Test_Diagonal_Adjacent_Positions()
   {
      Position.Position origin = new();

      var unw = origin.NorthWestUp;
      var un = origin.NorthUp;
      var une = origin.NorthEastUp;
      var uw = origin.WestUp;
      var u = origin.Up;
      var ue = origin.EastUp;
      var usw = origin.SouthWestUp;
      var us = origin.SouthUp;
      var use = origin.SouthEastUp;

      var nw = origin.NorthWest;
      var n = origin.North;
      var ne = origin.NorthEast;
      var w = origin.West;
      var e = origin.East;
      var sw = origin.SouthWest;
      var s = origin.South;
      var se = origin.SouthEast;

      var dnw = origin.NorthWestDown;
      var dn = origin.NorthDown;
      var dne = origin.NorthEastDown;
      var dw = origin.WestDown;
      var d = origin.Down;
      var de = origin.EastDown;
      var dsw = origin.SouthWestDown;
      var ds = origin.SouthDown;
      var dse = origin.SouthEastDown;

      var adjacentPositions = origin.GetAdjacentPositions(includeDiagonals: true);
      Assert.AreEqual(26, adjacentPositions.Count());
      Assert.IsTrue(adjacentPositions.Contains(unw));
      Assert.IsTrue(adjacentPositions.Contains(un));
      Assert.IsTrue(adjacentPositions.Contains(une));
      Assert.IsTrue(adjacentPositions.Contains(uw));
      Assert.IsTrue(adjacentPositions.Contains(u));
      Assert.IsTrue(adjacentPositions.Contains(ue));
      Assert.IsTrue(adjacentPositions.Contains(usw));
      Assert.IsTrue(adjacentPositions.Contains(us));
      Assert.IsTrue(adjacentPositions.Contains(use));

      Assert.IsTrue(adjacentPositions.Contains(nw));
      Assert.IsTrue(adjacentPositions.Contains(n));
      Assert.IsTrue(adjacentPositions.Contains(ne));
      Assert.IsTrue(adjacentPositions.Contains(w));
      Assert.IsTrue(adjacentPositions.Contains(e));
      Assert.IsTrue(adjacentPositions.Contains(sw));
      Assert.IsTrue(adjacentPositions.Contains(s));
      Assert.IsTrue(adjacentPositions.Contains(se));

      Assert.IsTrue(adjacentPositions.Contains(dnw));
      Assert.IsTrue(adjacentPositions.Contains(dn));
      Assert.IsTrue(adjacentPositions.Contains(dne));
      Assert.IsTrue(adjacentPositions.Contains(dw));
      Assert.IsTrue(adjacentPositions.Contains(d));
      Assert.IsTrue(adjacentPositions.Contains(de));
      Assert.IsTrue(adjacentPositions.Contains(dsw));
      Assert.IsTrue(adjacentPositions.Contains(ds));
      Assert.IsTrue(adjacentPositions.Contains(dse));
      Assert.IsFalse(adjacentPositions.Contains(origin));
   }
}
