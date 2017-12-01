use strict;
use warnings;

my $inputFile;
if (@ARGV) {
  $inputFile = shift;
} else {
  $inputFile = 'Sample1.txt';
}

open(my $fh, '<', $inputFile) or die "Couldn't open $inputFile: $!";
my $captcha = <$fh>;
close($fh);
chomp($captcha);

my @digits = map { int($_) } split(//, $captcha);
print "Input: @digits\n";

my $sum1 = 0;
my $sum2 = 0;
my $captchaLength = length($captcha);
my $halfway = $captchaLength / 2;
for (my $i = 0; $i < $captchaLength; ++$i) {
  my $firstDigit = $digits[$i];
  my $nextDigitIndex = ($i + 1) % $captchaLength;

  my $nextDigit = $digits[$nextDigitIndex];
  if ($firstDigit == $nextDigit) {
    $sum1 = $sum1 + $firstDigit;
  }

  $nextDigitIndex = ($i + $halfway) % $captchaLength;

  $nextDigit = $digits[$nextDigitIndex];
  if ($firstDigit == $nextDigit) {
    $sum2 = $sum2 + $firstDigit;
  }
}

print "First Captcha Sum: $sum1\n";
print "Second Captcha Sum: $sum2\n";
