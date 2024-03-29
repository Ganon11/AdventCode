using System.Net;

namespace AdventOfCode.Services;

public static class AdventOfCodeService
{
   private static readonly HttpClientHandler Handler = new()
   {
      CookieContainer = GetCookieContainer(),
      UseCookies = true,
   };

   private static readonly HttpClient Client = new(Handler)
   {
      BaseAddress = new Uri("https://adventofcode.com/"),
   };

   public static async Task<string> FetchInput(int year, int day)
   {
      var currentEst = TimeZoneInfo.ConvertTime(DateTime.Now, TimeZoneInfo.Utc).AddHours(-5);
      if (currentEst < new DateTime(year, 12, day))
      {
         throw new InvalidOperationException("Too early to get puzzle input.");
      }

      var response = await Client.GetAsync($"{year}/day/{day}/input");
      return await response.EnsureSuccessStatusCode().Content.ReadAsStringAsync();
   }

   private static CookieContainer GetCookieContainer()
   {
      var container = new CookieContainer();
      container.Add(new Cookie
      {
         Name = "session",
         Domain = ".adventofcode.com",
         Value = ConfigurationService.GetCookie(),
      });

      return container;
   }
}
