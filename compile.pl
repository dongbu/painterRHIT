#!/usr/bin/perl

my $file=$ARGV[0];

if (!$file) { print "Need arg of <filename>.cpp\n"; exit; }

my $cmd = "g++ $file.cpp -o $file  `pkg-config --cflags --libs opencv` -Wall";

print "running $cmd\n";
my $result = `$cmd`;
#print "R:$result\n";
