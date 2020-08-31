use strict;
use warnings;

use List::Util qw(any);

sub HasABBA {
  my $string = shift;

  if ($string =~ m/([a-z])([a-z])\2\1/) {
    return 1 if $1 ne $2;
  }

  return 0;
}

sub GetABAs {
  my $ip = shift;

  my $numCharacters = length($ip);
  my @abas = ();
  for (my $i = 0; $i < ($numCharacters - 2); ++$i) {
    my ($c1, $c2, $c3) = (substr($ip, $i, 1), substr($ip, $i + 1, 1), substr($ip, $i + 2, 1));
    if ($c1 eq $c3 and $c1 ne $c2) {
      push(@abas, "$c1$c2$c3");
    }
  }

  return @abas;
}

sub HasBAB {
  my ($string, $a, $b) = @_;
  return 1 if (-1 != index($string, "$b$a$b"));
  return 0;
}

sub GetHypernets {
  my $ip = shift;
  return $ip =~ m/\[([a-z]+)\]/g;
}

sub GetSupernets {
  my $ip = shift;
  return split(/\[[a-z]+\]/, $ip);
}

sub IPSupportsTLS {
  my $ip = shift;
  my @hypernets = GetHypernets($ip);
  my @supernets = GetSupernets($ip);

  return 0 if (any { HasABBA($_) } @hypernets);
  return 1 if (any { HasABBA($_) } @supernets);
  return 0;
}

sub IPSupportsSSL {
  my $ip = shift;
  my @hypernets = GetHypernets($ip);
  my @supernets = GetSupernets($ip);

  foreach my $supernet (@supernets) {
    my @abas = GetABAs($supernet);
    foreach my $aba (@abas) {
      my ($a, $b) = (substr($aba, 0, 1), substr($aba, 1, 1));
      if (any { HasBAB($_, $a, $b) } @hypernets) {
        return 1;
      }
    }
  }

  return 0;
}

my $inputfile = shift;
if (!defined($inputfile)) {
  $inputfile = 'sample.txt';
}

open(my $fh, '<', $inputfile) or die "Couldn't open $inputfile: $!";
my @lines = <$fh>;
close($fh);

my $tlsTotal = 0;
my $sslTotal = 0;
foreach my $line (@lines) {
  $line =~ s/\R//g; # Trim line breaks.

  if (1 == IPSupportsTLS($line)) {
    $tlsTotal = $tlsTotal + 1;
  }

  if (1 == IPSupportsSSL($line)) {
    $sslTotal = $sslTotal + 1;
  }
}

print "TLS Total: $tlsTotal\n";
print "SSL Total: $sslTotal\n";
