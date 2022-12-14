namespace AdventOfCode.Services.Models;

using System.Text.Json;
using System.Text.Json.Serialization;

internal struct Config
{
   public string Cookie { get; set; }

   public int Year { get; set; }

   [JsonConverter(typeof(DaysConverter))]
   public int[] Days { get; set; }

   public void SetDefaults()
   {
      //Make sure we're looking at EST, or it might break for most of the US
      var currentEst = TimeZoneInfo.ConvertTime(DateTime.Now, TimeZoneInfo.Utc).AddHours(-5);
      if (this.Cookie == default)
      {
         this.Cookie = "";
      }

      if (this.Year == default)
      {
         this.Year = currentEst.Year;
      }

      if (this.Days == default(int[]))
      {
         this.Days = (currentEst.Month == 12 && currentEst.Day <= 25) ? new int[] { currentEst.Day } : new int[] { 0 };
      }
   }
}

internal sealed class DaysConverter : JsonConverter<int[]>
{
   public override int[] Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
   {
      IEnumerable<string> tokens;

      switch (reader.TokenType)
      {
         case JsonTokenType.Number:
            return new int[] { reader.GetInt16() };

         case JsonTokenType.String:
            tokens = new string[] { reader.GetString() ?? "" };
            break;
         default:
            var obj = JsonSerializer
                .Deserialize<object[]>(ref reader);

            tokens = obj != null
                ? obj.Select(o => o.ToString() ?? "")
                : Array.Empty<string>();
            break;
      }

      var days = tokens.SelectMany(this.ParseString);
      if (days.Contains(0))
      {
         return new[] { 0 };
      }

      return days.Where(v => v is < 26 and > 0).OrderBy(day => day).ToArray();
   }

   private IEnumerable<int> ParseString(string str) => str.Split(",").SelectMany(str =>
      {
         if (str.Contains(".."))
         {
            var split = str.Split("..");
            var start = int.Parse(split[0], System.Globalization.CultureInfo.CurrentCulture);
            var stop = int.Parse(split[1], System.Globalization.CultureInfo.CurrentCulture);
            return Enumerable.Range(start, stop - start + 1);
         }
         else if (int.TryParse(str, out var day))
         {
            return new int[] { day };
         }

         return Array.Empty<int>();
      });

   public override void Write(Utf8JsonWriter writer, int[] value, JsonSerializerOptions options)
   {
      writer.WriteStartArray();
      foreach (var val in value)
      {
         writer.WriteNumberValue(val);
      }

      writer.WriteEndArray();
   }
}
