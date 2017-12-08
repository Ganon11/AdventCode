use strict;
use warnings;

use Data::Dumper;
use Program;

sub trim {
  my $string = shift;
  $string =~ s/^\s+//;
  $string =~ s/\s+$//;
  return $string;
}

sub CalculateTotalWeight {
  my $programsRef = shift;
  my $program = shift;

  my %programs = %{$programsRef};
  my $localWeight = $programs{$program}->weight;
  my $totalWeight = $localWeight;

  my $childrenRef = $programs{$program}->children;
  if (!defined($childrenRef) or scalar(@$childrenRef) == 0) {
    $programs{$program}->totalweight($totalWeight);
    return $totalWeight;
  }

  my $comparison = undef;
  my $unbalanced = 0;
  my @children = @{ $programs{$program}->children };
  foreach my $child (@children) {
    my $childWeight = CalculateTotalWeight($programsRef, $child);
    if (!defined($comparison)) {
      $comparison = $childWeight;
    } else {
      if ($comparison != $childWeight) {
        $unbalanced = 1;
      }
    }
    $totalWeight += $childWeight;
  }

  $programs{$program}->totalweight($totalWeight);

  if ($unbalanced == 1) {
    print "$program is imbalanced:\n";
    print "\t$program has local weight $localWeight\n";
    foreach my $child (@children) {
      my $childWeight = $programs{$child}->totalweight;
      print "\tChild $child has weight $childWeight\n";
    }
  }

  return $totalWeight;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my %programs = ();
open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
  $line =~ m/(\w+) \((\d+)\)/i;
  my ($name, $weight) = ($1, int($2));
  my $arrowIndex = index($line, '->');
  $programs{$name} = Program->new(name => $name, weight => $weight);
  if ($arrowIndex != -1) {
    my @children = map { trim ($_) } split(/,/, substr($line, $arrowIndex + 2));
    $programs{$name}->children(\@children);
  }
}
close($fh);

foreach my $program (keys %programs) {
  my $childrenRef = $programs{$program}->children;
  next if (!defined($childrenRef) or scalar(@$childrenRef) == 0);
  foreach my $child (@$childrenRef) {
    $programs{$child}->parent($program);
  }
}

# The root program is the one without a parent
my $rootProgram;
foreach my $program (keys %programs) {
  my $parent = $programs{$program}->parent;
  if (!defined($parent)) {
    $rootProgram = $program;
    last;
  }
}

print "Root program: $rootProgram\n";
CalculateTotalWeight(\%programs, $rootProgram);
#my $result = IsBalanced(\%programs, $rootProgram);

# foreach my $program (keys %programs) {
#   if (0 == IsBalanced(\%programs, $program)) {
#     print "Following program is imbalanced:\n";
#     print Dumper($programs{$program});
#   }
# }

# print Dumper(\%programs);
