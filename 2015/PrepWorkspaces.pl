use strict;
use warnings;

sub CreatePythonTemplateFile {
  my $dir = shift;
  my $filename = shift;
  my $perlFileName = "$dir/src/$filename";
  my $scriptTemplate = <<'END_SCRIPT_TEMPLATE';
import sys

def main():
  filename = 'sample.txt'
  if len(sys.argv) > 1:
    filename = sys.argv[1]

if __name__ == "__main__":
  main()
END_SCRIPT_TEMPLATE

  open(my $perlFileHandle, ">", $perlFileName) or die "Couldn't open $perlFileName: $!";
  print $perlFileHandle $scriptTemplate;
  close($perlFileHandle);
}

sub CreateSourceDirectory {
  my $dir = shift;
  mkdir("$dir/src");
  CreatePythonTemplateFile($dir, "$dir.py");
}

sub CreateInputDirectory {
  my $dir = shift;
  mkdir("$dir/input");
  my @inputFiles = ('input.txt', 'sample1.txt', 'sample2.txt', 'sample3.txt');
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
      "cmd": ["python", "$file", "../input/input.txt"]
    },
    {
      "name": "Run with sample 1",
      "cmd": ["python", "$file", "../input/sample1.txt"]
    },
    {
      "name": "Run with sample 2",
      "cmd": ["python", "$file", "../input/sample2.txt"]
    },
    {
      "name": "Run with sample 3",
      "cmd": ["python", "$file", "../input/sample3.txt"]
    }
  ]
}

END_PROJECT_TEMPLATE

  open(my $projectFileHandle, ">", $projectFileName) or die "Couldn't open $projectFileName: $!";
  print $projectFileHandle $projectTemplate;
  close($projectFileHandle);
}

#foreach my $directory (grep { -d } glob '*') {
foreach my $num (13..25) {
  my $directory = "Day$num";
  next if ($directory eq 'Day12');
  print "Prepping $directory\n";
  mkdir($directory);
  CreateSourceDirectory($directory);
  CreateInputDirectory($directory);
  CreateSublimeProjectFile($directory);
  print "Finished prepping $directory\n";
}
