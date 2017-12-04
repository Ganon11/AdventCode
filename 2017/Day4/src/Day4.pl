use Array::Compare;
use strict;
use warnings;

sub PhraseIsValidPart1 {
  my $phrase = shift;

  my @allWords = split(/\s+/, $phrase);
  my %words = ();
  foreach my $word (@allWords) {
    return 0 if (exists($words{$word}));
    $words{$word} = 1;
  }

  return 1;
}

sub WordsAreAnagrams {
  my $word1 = shift;
  my $word2 = shift;
  my @letters1 = sort(split(//, $word1));
  my @letters2 = sort(split(//, $word2));
  my $comp = Array::Compare->new;
  return $comp->compare(\@letters1, \@letters2);
}

sub PhraseIsValidPart2 {
  my $phrase = shift;

  my @allWords = split(/\s+/, $phrase);
  for (my $i = 0; $i < scalar(@allWords) - 1; ++$i) {
    for (my $j = $i + 1; $j < scalar(@allWords); ++$j) {
      return 0 if WordsAreAnagrams($allWords[$i], $allWords[$j]);
    }
  }

  return 1;
}

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

my @validPhrasesPart1;
my @validPhrasesPart2;

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);

  if (PhraseIsValidPart1($line) == 1) {
    push(@validPhrasesPart1, $line);
  }

  if (PhraseIsValidPart2($line) == 1) {
    push(@validPhrasesPart2, $line);
  }
}
close($fh);

my $validPhraseCountPart1 = scalar(@validPhrasesPart1);
my $validPhraseCountPart2 = scalar(@validPhrasesPart2);
print "$validPhraseCountPart1 valid passphrases in part 1\n";
print "$validPhraseCountPart2 valid passphrases in part 2\n";
