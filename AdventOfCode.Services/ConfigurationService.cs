namespace AdventOfCode.Services;

using System.Text.Json;
using System.Text.Json.Serialization;
using AdventOfCode.Services.Models;

public static class ConfigurationService
{
   private static Config config = GetConfig();

   public static string GetCookie() => config.Cookie;

   public static int GetYear() => config.Year;

   public static int[] GetDays() => config.Days;

   private static Config GetConfig(string path = "config.json")
   {
      var options = new JsonSerializerOptions()
      {
         DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull,
         PropertyNameCaseInsensitive = true,
         WriteIndented = true
      };
      Config config;
      if (File.Exists(path))
      {
         config = JsonSerializer.Deserialize<Config>(File.ReadAllText(path), options);
         config.SetDefaults();
      }
      else
      {
         config = new Config();
         config.SetDefaults();
         File.WriteAllText(path, JsonSerializer.Serialize(config, options));
      }

      return config;
   }
}
