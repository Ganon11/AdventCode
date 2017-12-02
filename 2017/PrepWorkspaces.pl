use strict;
use warnings;

sub CreatePerlTemplateFile {
  my $dir = shift;
  my $filename = shift;
  my $perlFileName = "$dir/src/$filename";
  my $scriptTemplate = <<'END_SCRIPT_TEMPLATE';
use strict;
use warnings;

my $inputFile;
if (scalar(@ARGV) > 0) {
  $inputFile = shift @ARGV;
} else {
  $inputFile = '../input/Input.txt';
}

open(my $fh, "<", $inputFile) or die "Couldn't open $inputFile: $!";
while (<$fh>) {
  my $line = $_;
  chomp($line);
}
END_SCRIPT_TEMPLATE

  open(my $perlFileHandle, ">", $perlFileName) or die "Couldn't open $perlFileName: $!";
  print $perlFileHandle $scriptTemplate;
  close($perlFileHandle);
}

sub CreateSourceDirectory {
  my $dir = shift;
  mkdir("$dir/src");
  CreatePerlTemplateFile($dir, "$dir.pl");
}

sub CreateInputDirectory {
  my $dir = shift;
  mkdir("$dir/input");
  my @inputFiles = ('Input.txt', 'Sample1.txt', 'Sample2.txt', 'Sample3.txt');
  foreach my $inputFile (@inputFiles) {
    my $path = "$dir/input/$inputFile";
    open(my $fh, '>', $path) or die "Couldn't create $inputFile: $!";
    close($fh);
  }
}

sub CreateSublimeProjectFile {
  my $dir = shift;
  my $projectFileName = "$dir/$dir.sublime-project";
  my $projectTemplate = <<'END_PROJECT_TEMPLATE';
{
  "folders":
  [
    {
      "path": "src",
      "name": "Source"
    },
    {
      "path": "input",
      "name": "Input files"
    }
  ],
  "build_systems":
  [
    {
      "name": "Run with input",
      "cmd": ["perl", "$file", "../input/Input.txt"]
    },
    {
      "name": "Run with sample 1",
      "cmd": ["perl", "$file", "../input/Sample1.txt"]
    },
    {
      "name": "Run with sample 2",
      "cmd": ["perl", "$file", "../input/Sample2.txt"]
    },
    {
      "name": "Run with sample 3",
      "cmd": ["perl", "$file", "../input/Sample3.txt"]
    }
  ]
}

END_PROJECT_TEMPLATE

  open(my $projectFileHandle, ">", $projectFileName) or die "Couldn't open $projectFileName: $!";
  print $projectFileHandle $projectTemplate;
  close($projectFileHandle);
}

my @directories = grep { -d } glob '*';
foreach my $directory (@directories) {
  next if ($directory eq 'Day1');
  next if ($directory eq 'Day2');
  print "Prepping $directory\n";
  CreateSourceDirectory($directory);
  CreateInputDirectory($directory);
  CreateSublimeProjectFile($directory);
  print "Finished prepping $directory\n";
}
