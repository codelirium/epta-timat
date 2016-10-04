#!/usr/bin/perl -w

#  pr0ber v1.0-hakin9 -- user guessing utility 
#  Dep: timat
#  
#  Copyright (C) 2006 Stavros Lekkas <xfactor @ linuxmail.org>
#                     Thanos Theodorides <attheodo @ inf.uth.gr>
#  
#  This program has been designed for the purposes of a hakin9 article.
#    
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

sub usage
{
        print "Usage: $0 <user> <host>\n";
        exit(1);
}

sub check_user
{
        my $user = shift;
        my $host = shift;
        my $avg;
        my @results;

        $| = 1;
        open TimAt, "./timat $host $user 10|" or die "$!";
        while(<TimAt>){
                 $result .= <TimAt>;
        }
        close TimAt;
        @results = split /:/, $result;
        $avg = $results[1];
        $avg =~ s/\s//g;
        $result = 0;
        return $avg;

}

$valid_user = "root";
$check_user = shift or usage();
$host = shift or usage();

$ValidUser_Avg = check_user($valid_user,$host);
$GuessUser_Avg = check_user($check_user,$host);
$Factor = $ValidUser_Avg/$GuessUser_Avg;

if($Factor > 1.2)
{
 print "[+] User ", $check_user," does not exist!\n";
} 
else 
{
 print "[+] User ", $check_user," exists!\n";
}

