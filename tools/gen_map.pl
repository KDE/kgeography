#!/usr/bin/perl
#
#
# Copyright (C) 2004 by Yann Verley
# yann.verley@free.fr
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
#
#
# Last modification : 12 May 2004

use strict;

sub print_colors
{
	my ($red, $green, $blue) = @_;

	print "\t\t<color>\n";
	print "\t\t\t<red>$red</red>\n";
	print "\t\t\t<green>$green</green>\n";
	print "\t\t\t<blue>$blue</blue>\n";
	print "\t\t</color>\n";
}

sub print_header
{
	my ($map_file, $map_name) = @_;

	print "<map>\n";

	print "\t<mapFile>$map_file</mapFile>\n";
	print "\t<name>$map_name</name>\n";
}

sub print_new_division
{
	my ($name, $capital, $flag, $ignore, $red, $green, $blue) = @_;

	print "\t<division>\n";
	
	print "\t\t<name>$name</name>\n";
	
	if ($ignore eq "yes")
	{
		print "\t\t<ignore>yes</ignore>\n";
	}

	if ($flag ne "")
	{
		print "\t\t<flag>$flag</flag>\n";
	}

	if ($capital ne "")
	{
		print "\t\t<capital>$capital</capital>\n";
	}

	print_colors($red, $green, $blue);

	print "\t</division>\n";
}

sub print_end_of_file
{
	print "</map>\n";
}

sub usage
{
	print "Usage : gen_map.pl division_list.txt [ignored_list.txt]\n\n";

	print "This script produces a kgm file to be used with KGeography.\n";
	print "It produces too a file, gimp_rgb.txt. This file can be used to help you\n";
	print "to fill the divisions in your map with an image processing program.\n\n";

	print "Example :\n";
	print "gen_map.pl spain.txt ignored.txt >spain.kgm\n\n";
	
	print "Example 1 of division_list.txt :\n";
	print "#division name;capital\n";
	print "Andalucia;Seville\n";
	print "Aragon;Zaragoza\n\n";
	
	print "Example 2 of division_list.txt :\n";
	print "#division name;capital;flag file\n";
	print "Germany;Berlin;germany.png\n";
	print "France;Paris;france.png\n\n";
	
	print "Example of ignored_list.txt :\n";
	print "#division name;red value;green value;blue value\n";
	print "Frontier;0;0;0\n";
	print "Water;50;50;255\n";
	exit;
}

#####################
#BEGINNING OF PROGRAM
#####################

my @div_name_list;
my @div_capital_list;
my @div_flag_list;
my @ign_name_list;
my @ign_red_list;
my @ign_green_list;
my @ign_blue_list;


if (($ARGV[0] eq "") or ($ARGV[0] eq "-h"))
{
	usage();
}

open(DIVFILE, "<".$ARGV[0]) or die "Can't open file $ARGV[0] : $!";

while (<DIVFILE>)
{
	next if (/^#/);
	chomp;

	my @spl = split /;/;

	push @div_name_list, $spl[0];
	push @div_capital_list, $spl[1];
	push @div_flag_list, $spl[2];
}

close(DIVFILE);

if (open(IGNFILE, "<".$ARGV[1]))
{
	while (<IGNFILE>)
	{
		next if (/^#/);
		chomp;

		my @spl = split /;/;

		push @ign_name_list, $spl[0];
		push @ign_red_list, $spl[1];
		push @ign_green_list, $spl[2];
		push @ign_blue_list, $spl[3];
	}

	close(IGNFILE);
}

my $basename = $ARGV[0];
$basename =~ s/\.[[:alnum:]]*$//;

print_header($basename.".png", ucfirst($basename));

for (my $i = 0; $i <= $#ign_name_list ; $i++)
{
	print_new_division($ign_name_list[$i],
			   "",
			   "",
			   "yes",
			   $ign_red_list[$i],
			   $ign_green_list[$i],
			   $ign_blue_list[$i]);
}

my $min_red = 130;
my $min_green = 130;
my $min_blue = 130;
my $max_red = 250;
my $max_green = 250;
my $max_blue = 250;

my @red_list;
my @green_list;
my @blue_list;

my $red;
my $green;
my $blue;

my $step;

if ($#div_name_list < 25)
{
	$step = 60;
}
elsif ($#div_name_list < 60)
{
	$step = 40;
}
elsif ($#div_name_list < 120)
{
	$step = 30;
}
elsif ($#div_name_list < 210)
{
	$step = 24;
}
elsif ($#div_name_list < 340)
{
	$step = 20;
}
else
{
	# > 700 divisions allowed
	$step = 15;
}

#now we build list of colors which will be used to colorize the map
for ($red = $min_red ; $red <= $max_red ; $red += $step)
{
	for ($green = $min_green ; $green <= $max_green ; $green += $step)
	{
		for ($blue = $min_blue ; $blue <= $max_blue ; $blue += $step)
		{
			push @red_list, $red;
			push @green_list, $green;
			push @blue_list, $blue;
		}
	}
}

open(RGB_FILE, ">gimp_rgb.txt") or die "Cannot open file : $!";

for (my $i = 0 ; $i <= $#div_name_list ; $i++)
{
	print_new_division($div_name_list[$i],
			$div_capital_list[$i],
			$div_flag_list[$i],
			"no",
			$red_list[$i],
			$green_list[$i],
			$blue_list[$i]);

	printf RGB_FILE "%s : %X%X%X\n", $div_name_list[$i],
			$red_list[$i], $green_list[$i], $blue_list[$i];
}

close(RGB_FILE);

print_end_of_file();

