#! /usr/bin/env perl

# roff2* - transform roff files into other formats

# Source file position: <groff-source>/contrib/groffer/perl/roff2.pl
# Installed position: <prefix>/bin/roff2*

# Copyright (C) 2006, 2008, 2009 Free Software Foundation, Inc.
# Written by Bernd Warken.

# Last update: 5 Jan 2009

# This file is part of `groffer', which is part of `groff'.

# `groff' is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# `groff' is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

########################################################################

require v5.6;

use strict;
use warnings;
use File::Spec;

# temporary dir and files
use File::Temp qw/ tempfile /;

my $Dev_Null = File::Spec->devnull();
my $Mode;
my $Name;
{
  my ($v, $d);
  ($v, $d, $Name) = File::Spec->splitpath($0);
  die "wrong program name: $Name;"
    if $Name !~ /^roff2[a-z]/;
}
$Mode = $Name;
$Mode =~ s/^roff2//;

my $Groff_Version = `groff --version 2>$Dev_Null`;
die "$Name error: groff does not work;" if $?;
my $Groffer_Version = `groffer --version 2>$Dev_Null`;
my $Has_Groffer = ! $?;

if ($Has_Groffer) {
  foreach (@ARGV) {
    last if $_ eq '--';
    next if $_ eq '-';
    if ($_ eq '-v' || '--version' =~ m|^$_|) {
      print $Name, ' in ', $Groffer_Version;
      exit 0;
    }
    if ($_ eq '-h' || '--help' =~ m|^$_|) {
      &usage_with_groffer();
      exit 0;
    }
  }
  system('groffer', '--to-stdout', "--$Mode", @ARGV);
  exit $?;
} else {
  my @filespec;
  my $double_minus;
  foreach (@ARGV) {
    if ($double_minus) {
      push @filespec, $_;
      next;
    }
    if ($_ eq '--') {
      $double_minus = 1;
      next;
    }
    if ($_ eq '-') {
      push @filespec, '-';
      next;
    }
    if ($_ eq '-v' || '--version' =~ m|^$_|) {
      print $Name, ' in ', $Groff_Version;
      exit 0;
    }
    if ($_ eq '-h' || '--help' =~ m|^$_|) {
      &usage_without_groffer();
      exit 0;
    }
    if ($_ =~ /^-/) {
      &error_no_groffer();
      &error_no_options();
      next;
    }
    if (-f && -r) {
      push @filespec, $_;
    } else {
      &error_no_groffer();
      print STDERR "$_ is not an existing, readable file.\n";
    }
  }

  @filespec = ('-') unless @filespec;
  my $has_stdin;
  foreach (@filespec) {
    if ($_ eq '-') {
      $has_stdin =1;
      last;
    }
  }

  if ($has_stdin) {
    my $tempdir;
    foreach ($ENV{'GROFF_TMPDIR'}, $ENV{'TMPDIR'}, $ENV{'TMP'},
	     $ENV{'TEMP'}, $ENV{'TEMPDIR'},
	     File::Spec->catfile($ENV{'HOME'}, 'tmp')) {
      if ($_ && -d $_ && -w $_) {
	$tempdir = $_;
	last;
      }
    }
    my $template = $Name . '_XXXX';
    my ($fh, $stdin);
    if ($tempdir) {
      ($fh, $stdin) = tempfile($template, UNLINK => 1, DIR => $tempdir) ||
	die "$Name: could not create temporary file;";
    } else {
      ($fh, $stdin) = tempfile($template, UNLINK => 1) ||
	die "$Name: could not create temporary file;";
    }
    open $fh, ">$stdin";
    print $fh $_ foreach <STDIN>;

    foreach (@filespec) {
      $_ = $stdin if $_ eq '-';
    }
  }				# if $has_stdin

  my $grog;
  my $groff_options = "-T$Mode";
  $groff_options = '-TX75-12 -Z' if $Mode eq 'x';
  $groff_options = '-Tlatin1' if $Mode eq 'text';
  if ($Mode eq 'pdf') {
    my $ps2pdf;
    my @path = File::Spec->path();
    foreach (@path) {
      my $file = File::Spec->catfile($_, 'ps2pdf');
      if (-f $file && -x $file) {
	$ps2pdf = "$file -";
	last;
      }
    }
    unless ($ps2pdf) {
      foreach (@path) {
	my $file = File::Spec->catfile($_, 'gs');
	if (-f $file && -x $file) {
	  $ps2pdf = $file . ' -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite ' .
	    '-sOutputFile=- -c save pop -f -';
	  last;
	}
      }
    }
    $grog = `grog -Tps @filespec`;
    chomp $grog;
    system("$grog | $ps2pdf");
    exit $?;
  } else {
    $grog = `grog $groff_options @filespec`;
    chomp $grog;
    system($grog);
    exit $?;
  }
}

my $error_no_groffer;
sub error_no_groffer {
  return 1 if $error_no_groffer;
  $error_no_groffer = 1;
  print STDERR "$Name: groffer is not available.\n";
}

my $error_no_options;
sub error_no_options {
  return 1 if $error_no_options;
  $error_no_groffer = 1;
  print STDERR "$Name: groffer options are not allowed.\n";
}

sub usage_with_groffer {
  print <<EOF;
usage: $Name [option]... [--] [filespec]...

-h | --help     print usage information
-v | --version  print version information

All other options are arbitrary options of `groffer'; the options
override the behavior of this program.

`filespec's are the same as in `groffer': either the names of
existing, readable files or `-' for standard input or a search pattern
for man pages.  No `filespec' assumes standard input automatically.
EOF
}

sub usage_without_groffer {
  print <<EOF;
usage: $Name [option]... [--] [filespec]...

-h | --help     print usage information
-v | --version  print version information

No other options are allowed because `groffer' is not available.

The only `filespec's allowed are the names of existing, readable files
or `-' for standard input.  No `filespec' assumes standard input
automatically.
EOF
}
