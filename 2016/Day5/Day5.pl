use strict;
use warnings;

use Digest::MD5 qw(md5_hex);

# my $input = 'abc'; # Sample
my $input = 'uqwqemis'; # Actual input


sub FindInterestingCharacterPart1 {
  my ($doorId, $index) = @_;

  do {
    my $hashInput = $doorId . $index;
    ++$index;
    my $digest = md5_hex($hashInput);
    if (substr($digest, 0, 5) eq '00000') {
      return (substr($digest, 5, 1), $index);
    }
  } while (1);
}

sub DoPart1 {
  my $index = 0;
  my $password = '________';

  print "Door Code: $password\n";

  for (my $i = 0; $i < 8; ++$i) {
    my ($newChar, $newIndex) = FindInterestingCharacterPart1($input, $index);
    $index = $newIndex;
    substr($password, $i, 1) = $newChar;

    print "Door code: $password\n";
  }
}

sub FindInterestingCharacterPart2 {
  my ($doorId, $index) = @_;

  do {
    my $hashInput = $doorId . $index;
    ++$index;
    my $digest = md5_hex($hashInput);
    if (substr($digest, 0, 5) eq '00000') {
      my $newIndex = substr($digest, 5, 1);
      if ($newIndex =~ m/[0-7]/) {
        return ($newIndex, substr($digest, 6, 1), $index);
      }
    }
  } while (1);
}

sub DoPart2 {
  my $index = 0;
  my %password = ();
  my $password = '________'; # 8 _s

  print "Door Code: $password\n";

  do {
    my ($position, $newChar, $newIndex) = FindInterestingCharacterPart2($input, $index);
    $index = $newIndex;
    my $oldChar = substr($password, $position, 1);
    if ($oldChar eq '_') {
      substr($password, $position, 1) = $newChar;
      print "Door Code: $password\n";
    }
  } while (-1 != index($password, '_'));
}

my $part = shift;
$part = '1' if (!defined($part));

if ($part == '1') {
  DoPart1();
}
elsif ($part == '2') {
  DoPart2();
}
else {
  print "Which part should I do?\n";
}
