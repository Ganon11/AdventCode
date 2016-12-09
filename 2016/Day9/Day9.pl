use strict;
use warnings;

sub DecompressV1 {
  my $compressedtext = shift;

  my $pos = index($compressedtext, '(');
  if ($pos == -1) {
    return length($compressedtext);
  } elsif ($compressedtext =~ m/(\((\d+)x(\d+)\))/) {
    my ($bracket, $size, $repeat) = ($1, $2, $3);
    my $i = $pos + length($bracket);
    my $leadingStrLen = length(substr($compressedtext, 0, $pos));
    my $textToRepeat = substr($compressedtext, $i, $size);
    my $remainingText = substr($compressedtext, $i + $size);
    return $leadingStrLen + (length($textToRepeat) * $repeat) + DecompressV1($remainingText);
  } else {
    return 0;
  }
}

sub DecompressV2 {
  my $compressedtext = shift;

  my $pos = index($compressedtext, '(');
  if ($pos == -1) {
    return length($compressedtext);
  } elsif ($compressedtext =~ m/(\((\d+)x(\d+)\))/) {
    my ($bracket, $size, $repeat) = ($1, $2, $3);
    my $i = $pos + length($bracket);
    my $leadingStrLen = length(substr($compressedtext, 0, $pos));
    my $textToRepeat = substr($compressedtext, $i, $size);
    my $remainingText = substr($compressedtext, $i + $size);
    return $leadingStrLen + (DecompressV2($textToRepeat) * $repeat) + DecompressV2($remainingText);
  } else {
    return 0;
  }
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'Sample1.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
chomp(my @lines = <$fh>);
close($fh);

my $compressedtext = '';
foreach my $line (@lines) {
  $line =~ s/(?:\R|\s)//g; # Trim newlines and whitespace
  $compressedtext = $compressedtext . $line;
}

my $len = DecompressV1($compressedtext);
print "V1 Length: $len\n";

$len = DecompressV2($compressedtext);
print "V2 Length: $len\n";
