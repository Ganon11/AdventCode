// See https://aka.ms/new-console-template for more information

// CA1852 Type 'Program' can be sealed because it has no subtypes in its containing assembly and is not externally visible
#pragma warning disable CA1852

await Tidy.AdventOfCode.Runner.CreateDefault().ExecuteAsync();

#pragma warning restore CA1852
