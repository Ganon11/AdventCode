namespace AdventOfCode.Solutions.Utils;

using System.Reflection;

#pragma warning disable CS8604 // Possible null reference argument.
#pragma warning disable CS8600 // Converting null literal or possible null value to non-nullable type.
#pragma warning disable CS8602 // Dereference of a possibly null reference.
#pragma warning disable CS8603 // Possible null reference return.
#pragma warning disable CS8601 // Possible null reference assignment.

public static class ObjectExtensions
{
   private static readonly MethodInfo CloneMethod = typeof(object).GetMethod("MemberwiseClone", BindingFlags.NonPublic | BindingFlags.Instance);

   public static bool IsPrimitive(this Type type)
   {
      if (type == typeof(string))
      {
         return true;
      }

      return type.IsValueType & type.IsPrimitive;
   }

   public static object? Copy(this object originalObject) => InternalCopy(originalObject, new Dictionary<object, object>(new ReferenceEqualityComparer()));

   private static object? InternalCopy(object originalObject, IDictionary<object, object> visited)
   {
      if (originalObject == null)
      {
         return null;
      }

      var typeToReflect = originalObject.GetType();
      if (IsPrimitive(typeToReflect))
      {
         return originalObject;
      }

      if (visited.TryGetValue(originalObject, out var value))
      {
         return value;
      }

      if (typeof(Delegate).IsAssignableFrom(typeToReflect))
      {
         return null;
      }

      var cloneObject = CloneMethod.Invoke(originalObject, null);
      if (typeToReflect.IsArray)
      {
         var arrayType = typeToReflect.GetElementType();
         if (IsPrimitive(arrayType) == false)
         {
            var clonedArray = (Array)cloneObject;
            clonedArray.ForEach((array, indices) => array.SetValue(InternalCopy(clonedArray.GetValue(indices), visited), indices));
         }

      }
      visited.Add(originalObject, cloneObject);
      CopyFields(originalObject, visited, cloneObject, typeToReflect);
      RecursiveCopyBaseTypePrivateFields(originalObject, visited, cloneObject, typeToReflect);
      return cloneObject;
   }

   private static void RecursiveCopyBaseTypePrivateFields(object originalObject, IDictionary<object, object> visited, object cloneObject, Type typeToReflect)
   {
      if (typeToReflect.BaseType != null)
      {
         RecursiveCopyBaseTypePrivateFields(originalObject, visited, cloneObject, typeToReflect.BaseType);
         CopyFields(originalObject, visited, cloneObject, typeToReflect.BaseType, BindingFlags.Instance | BindingFlags.NonPublic, info => info.IsPrivate);
      }
   }

   private static void CopyFields(object originalObject, IDictionary<object, object> visited,
      object cloneObject, Type typeToReflect,
      BindingFlags bindingFlags = BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.FlattenHierarchy,
      Func<FieldInfo, bool>? filter = null)
   {
      foreach (var fieldInfo in typeToReflect.GetFields(bindingFlags))
      {
         if (filter != null && filter(fieldInfo) == false)
         {
            continue;
         }

         if (IsPrimitive(fieldInfo.FieldType))
         {
            continue;
         }

         var originalFieldValue = fieldInfo.GetValue(originalObject);
         var clonedFieldValue = InternalCopy(originalFieldValue, visited);
         fieldInfo.SetValue(cloneObject, clonedFieldValue);
      }
   }

   public static T Copy<T>(this T original) => (T)Copy((object)original);
}

public class ReferenceEqualityComparer : EqualityComparer<object>
{
   public override bool Equals(object? x, object? y) => ReferenceEquals(x, y);

   public override int GetHashCode(object obj)
   {
      if (obj == null)
      {
         return 0;
      }

      return obj.GetHashCode();
   }
}

public static class ArrayExtensions
{
   public static void ForEach(this Array array, Action<Array, int[]> action)
   {
      if (array.LongLength == 0)
      {
         return;
      }

      var walker = new ArrayTraverse(array);
      do
      {
         action(array, walker.Position);
      }
      while (walker.Step());
   }
}

internal sealed class ArrayTraverse
{
   public int[] Position;
   private int[] maxLengths;

   public ArrayTraverse(Array array)
   {
      this.maxLengths = new int[array.Rank];
      for (var i = 0; i < array.Rank; ++i)
      {
         this.maxLengths[i] = array.GetLength(i) - 1;
      }
      this.Position = new int[array.Rank];
   }

   public bool Step()
   {
      for (var i = 0; i < this.Position.Length; ++i)
      {
         if (this.Position[i] < this.maxLengths[i])
         {
            this.Position[i]++;
            for (var j = 0; j < i; j++)
            {
               this.Position[j] = 0;
            }
            return true;
         }
      }
      return false;
   }
}

#pragma warning restore CS8601 // Possible null reference assignment.
#pragma warning restore CS8603 // Possible null reference return.
#pragma warning restore CS8602 // Dereference of a possibly null reference.
#pragma warning restore CS8600 // Converting null literal or possible null value to non-nullable type.
#pragma warning restore CS8604 // Possible null reference argument.
