use strict;
use warnings;

sub ExpandString {
  my $a = shift;
  my $desiredLength = shift;
  my $currentLength = length($a);

  if ($currentLength >= $desiredLength) {
    return substr($a, 0, $desiredLength);
  }

  print "current: $currentLength\n";
  my $b = join('', reverse(split(//, $a)));
  $b =~ tr/01/10/;

  return ExpandString($a . '0' . $b, $desiredLength);
}

sub CalculateChecksum {
  my $data = shift;

  my @pairs = ($data =~ m/([01]{2})/g);
  my $checksum = '';

  foreach my $pair (@pairs) {
    my ($c1, $c2) = split(//, $pair);
    if ($c1 eq $c2) {
      $checksum = $checksum . '1';
    }
    else {
      $checksum = $checksum . '0';
    }
  }

  my $len = length($checksum);
  if (1 == $len % 2) {
    return $checksum;
  }
  else {
    return CalculateChecksum($checksum);
  }
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample1.txt';
}

open(my $fh, '<', $inputfile) or die "Could not open $inputfile: $!";
my $fillLength = <$fh>;
my $data = <$fh>;
close($fh);

$fillLength =~ s/\R//g;
$data =~ s/\R//g;

my $dragonData = ExpandString($data, $fillLength);
my $checksum = CalculateChecksum($dragonData);

# print "Data: '$dragonData', checksum '$checksum'\n";
print "Checksum: '$checksum'\n";
