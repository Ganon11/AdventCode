use strict;
use warnings;
use Digest::MD5 qw(md5_hex);

use Data::Dumper;

my $PART = 2;

sub GetHash {
  my ($salt, $index, $part) = @_;

  my $input = "$salt$index";
  if (1 == $part) {
    return md5_hex($input);
  }
  else {
    my $digest = md5_hex($input);
    for (my $i = 0; $i < 2016; ++$i) {
      $digest = md5_hex($digest);
    }

    return $digest;
  }
}

sub GetTriple {
  my $digest = shift;
  if ($digest =~ m/([0-9a-f])\1\1/) {
    return $1;
  }

  return undef;
}

sub GetQuintuples {
  my $digest = shift;
  my @quintuples = $digest =~ m/([0-9a-f])\1\1\1\1/g;
  return @quintuples;
}

# my $salt = 'abc'; # Sample
my $salt = 'ahsbgdzn'; # Input
my $index = 0;
my $totalKeys = 0;

my @infos = (undef);

while ($totalKeys < 64) {
  my $ref = shift(@infos);
  ++$index;
  my $cacheSize = scalar(@infos);

  if (!defined($ref)) {
    my $digest = GetHash($salt, $index, $PART);
    $ref = [GetTriple($digest), GetQuintuples($digest)];
  }

  my $tripleChar = @$ref[0];
  if (defined($tripleChar)) {
    #Found a triple - check up to 1000 next hashes for quintuples
    for (my $j = 0; $j <= 999; ++$j) {
      # If we've already computed this hash
      if ($j < $cacheSize) {
        my $otherRef = $infos[$j];

        # Did this hash have quintuples?
        if (defined(@$otherRef[1])) {
          my %quintuples = map { $_ => 1 } @$otherRef[1];
          if (exists($quintuples{$tripleChar})) {
            ++$totalKeys;
            last;
          }
        }
      }
      else {
        # Still need to compute this hash.
        my $digest = GetHash($salt, $index + $j + 1, $PART);
        my $newRef = [GetTriple($digest), GetQuintuples($digest)];
        push(@infos, $newRef);

        # Did this hash have quintuples?
        if (defined(@$newRef[1])) {
          my %quintuples = map { $_ => 1 } @$newRef[1];
          if (exists($quintuples{$tripleChar})) {
            ++$totalKeys;
            last;
          }
        }
      }
    }
  }
}

print "Found 64 keys with index $index\n";
