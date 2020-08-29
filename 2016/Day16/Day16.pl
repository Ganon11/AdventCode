use strict;
use warnings;

sub ExpandString {
  my ($data, $fillLength) = @_;

  while (length($data) < $fillLength) {
    $data = $data . '0' . reverse($data =~ tr/01/10/r) 
  }

  return substr($data, 0, $fillLength);
}

sub CalculateChecksum {
  my ($data, $fill_length) = @_;
  my $bin = sprintf('%b', $fill_length);
  $bin =~ /(10+)$/;
  my $chunk_size = oct("0b$1");
  my $checksum;
  $checksum .= tr/1// & 1 ^ 1 while ($_ = substr($data, 0, $chunk_size, ''));
  return $checksum;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'sample1.txt';
}

open(my $fh, '<', $inputfile) or die "Could not open $inputfile: $!";
my $fillLength = <$fh>;
my $data = <$fh>;
close($fh);

$fillLength =~ s/\R//g;
$data =~ s/\R//g;

my $dragonData = ExpandString($data, $fillLength);
my $checksum = CalculateChecksum($dragonData, $fillLength);

# print "Data: '$dragonData', checksum '$checksum'\n";
print "Checksum: '$checksum'\n";
