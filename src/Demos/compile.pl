#!/usr/bin/perl

my $file=$ARGV[0];

if (!$file) { print "Need arg of <filename>.cpp\n"; exit; }

$file =~ s/(.*)\.cpp/$1/;

my $cmd = "g++ $file.cpp -o bin/$file  `pkg-config --cflags --libs opencv` -Wall -I../Libraries -I../../include/Libraries";


# gcc /System/Library/Frameworks/GLUT.framework/GLUT /System/Library/Frameworks/OpenGL.framework/OpenGL opengldemo2.cpp -o myGlutApp

print "running $cmd\n";
my $result = `$cmd`;
#print "R:$result\n";
