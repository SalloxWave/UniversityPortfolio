#!/usr/bin/env perl
use strict;
use warnings;

local $/ = "\n\n"; 
my $count = 1; 

while ( my $chunk = <> ) {
    open ( my $output, '>', sprintf( "level_%02d.sokoban", $count++ ) ) or die $!;
    print {$output} $chunk;
    close ( $output ); 
}