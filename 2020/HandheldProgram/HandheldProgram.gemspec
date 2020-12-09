require_relative 'lib/HandheldProgram/version'

Gem::Specification.new do |s|
  s.name = "HandheldProgram"
  s.version = HandheldProgram::VERSION
  s.date = "2020-12-08"
  s.summary = "An instruction set for handheld programs"
  s.description = "See Advent of Code"
  s.authors = ["Michael Stark"]
  s.email = "stark3@gmail.com"
  s.files = Dir["{lib}/**/*.rb"]
  s.homepage = "https://github.com/Ganon11/AdventCode"
  s.license = "MIT"
end
