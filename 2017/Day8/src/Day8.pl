use strict;
use warnings;
use Instruction;
use Data::Dumper;

sub GetValue {
  my $registersRef = shift;
  my $register = shift;
  my %registers = %{$registersRef};

  if (!exists($registers{$register})) {
    $registers{$register} = 0;
  }

  return $registers{$register};
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @instructions = ();
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  if ($line =~ m/(\w+) (inc|dec) (-?\d+) if (\w+) (<|>|==|<=|>=|!=) (-?\d+)/i) {
    my ($target, $operator, $value, $comparison, $comparator, $comparisonvalue) = ($1, $2, $3, $4, $5, $6);
    push(@instructions, Instruction->new(
      target => $target,
      operator => $operator,
      value => int($value),
      comparison => $comparison,
      comparator => $comparator,
      comparisonvalue => int($comparisonvalue)
    ));
  }
}
close($fh);

my $temporaryMaxValue = undef;
my %registers = ();
foreach my $instruction (@instructions) {
  my (
    $target,
    $operator,
    $value,
    $comparison,
    $comparator,
    $comparisonvalue
  ) = (
    $instruction->target,
    $instruction->operator,
    $instruction->value,
    $instruction->comparison,
    $instruction->comparator,
    $instruction->comparisonvalue
  );

  #print "Checking $comparison $comparator $comparisonvalue\n";

  my $lhs = GetValue(\%registers, $comparison);
  $registers{$comparison} = $lhs;

  if ($comparator eq '>') {
    next if ($lhs <= $comparisonvalue);
  } elsif ($comparator eq '<') {
    next if ($lhs >= $comparisonvalue);
  } elsif ($comparator eq '==') {
    next if ($lhs != $comparisonvalue);
  } elsif ($comparator eq '!=') {
    next if ($lhs == $comparisonvalue);
  } elsif ($comparator eq '>=') {
    next if ($lhs < $comparisonvalue);
  } elsif ($comparator eq '<=') {
    next if ($lhs > $comparisonvalue);
  } else {
    print "Unknown comparator $comparator\n";
    next;
  }

  #print "Performing $target $operator $value\n";
  my $newValue = GetValue(\%registers, $target);
  if ($operator =~ m/inc/) {
    $newValue = $newValue + $value;
  } else {
    $newValue = $newValue - $value;
  }
  $registers{$target} = $newValue;
  if (!defined($temporaryMaxValue) or $newValue > $temporaryMaxValue) {
    $temporaryMaxValue = $newValue;
  }
}

my $maxValue = undef;
foreach my $r (keys(%registers)) {
  if (!defined($maxValue) or $registers{$r} > $maxValue) {
    $maxValue = $registers{$r};
  }
}

print "Max value: $maxValue\n";
print "Temporary max value: $temporaryMaxValue\n";

#print Dumper(\@instructions);
#print Dumper(\%registers);
