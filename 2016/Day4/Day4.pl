use strict;
use warnings;

sub ParseLine {
  my $line = shift;
  if ($line =~ m/([a-z\-]+)\-(\d+)\[([a-z]+)\]/) {
    return ($1, $2, $3); # name, sector id, checksum
  }

  return undef;
}

sub IsValidRoom {
  my ($name, $checksum) = @_;

  # Get counts of letters in name
  my %counts = ();
  foreach my $char (split //, $name) {
    if ($char ne '-') {
      if (exists($counts{$char})) {
        $counts{$char} = $counts{$char} + 1;
      } else {
        $counts{$char} = 1;
      }
    }
  }

  # Check counts against checksum
  my $index = 0;
  foreach my $char (sort { $counts{$b} <=> $counts{$a} or $a cmp $b } keys %counts) {
    my $sumchar = substr($checksum, $index, 1);

    if ($char ne $sumchar) {
      return 0;
    }

    $index = $index + 1;
    last if ($index > 4);
  }

  return 1;
}

my @letters = split(//, 'abcdefghijklmnopqrstuvwxyz');

sub DecryptName {
  my ($encrypted, $sector) = @_;
  $sector = $sector % 26;
  my $decrypted = '';
  foreach my $char (split(//, $encrypted)) {
    if ($char eq '-') {
      $decrypted = $decrypted . ' ';
    } else {
      my $oldindex = index('abcdefghijklmnopqrstuvwxyz', $char);
      my $newindex = ($oldindex + $sector) % 26;
      my $newchar = $letters[$newindex];
      $decrypted = $decrypted . $newchar;
    }
  }

  return $decrypted;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = "Sample.txt";
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
chomp(my @lines = <$fh>);
close($fh);

my $total = 0;

for my $line (@lines) {
  my ($name, $sector, $checksum) = ParseLine($line);
  if (!defined($name) or !defined($name) or !defined($name)) {
    warn "Failed to parse line \"$line\"\n";
    continue;
  }

  if (1 == IsValidRoom($name, $checksum)) {
    $total = $total + $sector;
    my $decryptedname = DecryptName($name, $sector);
    if ($decryptedname eq 'northpole object storage') {
      print "$decryptedname, $sector\n";
    }
  }
}

print "Sector ID Sum: $total\n";
