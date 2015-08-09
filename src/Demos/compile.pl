#!/usr/bin/perl

my $file=$ARGV[0];

if (!$file) { print "Need arg of <filename>.cpp\n"; exit; }

$file =~ s/(.*)\.cpp/$1/;

my $cmd = "g++ $file.cpp -o bin/$file  `pkg-config --cflags --libs opencv` -Wall -I../Libraries -I../../include/Libraries";

print "running $cmd\n";
my $result = `$cmd`;
#print "R:$result\n";
