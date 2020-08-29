use strict;
use warnings;

use Data::Dumper;

sub ReadFile {
  my $inputfile = shift;
  open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";

  my @counts = ();
  while (my $line = <$fh>) {
    $line =~ s/\R//g; # Trim line breaks.
    my @chars = split(//, $line);
    my $charsize = scalar(@chars);
    for (my $i = 0; $i < $charsize; $i = $i + 1) {
      my $result = $counts[$i]{$chars[$i]};
      if (defined($result)) {
        $counts[$i]{$chars[$i]} = $result + 1;
      } else {
        $counts[$i]{$chars[$i]} = 1;
      }
    }
  }

  close($fh);

  return @counts;
}

sub RestoreStringHighestFrequency {
  my $aref = shift;
  my @counts = @{$aref};
  my $string = '';

  foreach my $hashref (@counts) {
    my @sortedChars = sort { $hashref->{$b} <=> $hashref->{$a} } keys %$hashref;
    $string = $string . $sortedChars[0];
  }

  return $string;
}

sub RestoreStringLowestFrequency {
  my $aref = shift;
  my @counts = @{$aref};
  my $string = '';

  foreach my $hashref (@counts) {
    my @sortedChars = sort { $hashref->{$a} <=> $hashref->{$b} } keys %$hashref;
    $string = $string . $sortedChars[0];
  }

  return $string;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'sample.txt'
}

my @counts = ReadFile($inputfile);
print RestoreStringHighestFrequency(\@counts), "\n";
print RestoreStringLowestFrequency(\@counts), "\n";
