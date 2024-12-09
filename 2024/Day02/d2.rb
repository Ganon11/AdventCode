require 'optparse'
require 'sorbet-runtime'

# typed: true
extend T::Sig

sig {params(levels: T::Array[Integer]).returns(T::Boolean)}
def levels_are_safe?(levels)
  increasing = T.let(false, T::Boolean)
  decreasing = T.let(false, T::Boolean)
  levels.each_with_index do |_, index|
    next if index == 0
    diff = T.must(levels[index]) - T.must(levels[index - 1])
    if diff > 0
      increasing = true
    elsif diff < 0
      decreasing = true
    else
      return false # differs by less than 1
    end

    return false if diff.abs > 3
  end

  return increasing ^ decreasing
end

sig {params(report: String, dampener: T::Boolean).returns(T::Boolean)}
def report_is_safe?(report, dampener)
  levels = report.split.map(&:to_i)
  if dampener
    (0..levels.length()).each do |index|
      if levels_are_safe?(levels.reject.with_index{|_, i| i == index})
        return true
      end
    end

    return false
  else
    return levels_are_safe?(levels)
  end
end

sig {params(reports: T::Array[String], dampener: T::Boolean).returns(Integer)}
def count_safe_reports(reports, dampener)
  sum = 0
  reports.each do |report|
    if report_is_safe?(report, dampener)
      sum += 1
    end
  end
  sum
end

options = {
  :part => 1,
  :filename => 'input.txt'
}

OptionParser.new do |opts|
  opts.banner = 'Usage: d2.rb [options]'
  opts.on('-p PART', '--part=PART', Integer, 'Part 1 or 2?')
  opts.on('-f FILENAME', '--filename=FILENAME', String, 'Which file to use?')
end.parse!(into: options)

reports = T.let(IO.readlines(options[:filename]).map(&:strip), T::Array[String])
puts count_safe_reports(reports, options[:part] == 2)
