use 5.010;
use strict;
use warnings;

my %d=(clique=>1,size=>2,length=>3,shortest=>4,list=>5,people=>6,graph=>7,importance=>8,important=>8,more=>9);
print "Type \"exit\" to get out of here\n";
while(1){
	print "Enter Query\n";
	my $he=<STDIN>;
	chomp $he;
	$he =~ s/\?/ /ig;
	my @q=split(' ',$he,length($he));
	my $arr='';
	
	my $elm;
	if ($he eq 'exit'){
		last;
	}
	for my $mm (@q)
	{
		$elm=$d{$mm};
		if (defined $elm){
			$arr.=$elm;
		}
	}
	my $quer=0;
	my $entry1='nulli';
	my $entry2='nulli';
	if (defined $arr){
		#Query 1
		if ($arr=='12' || $arr=='21'){
			#Take out the entry number
			$quer=1;
			for my $mf (@q){
				if ($mf=~ /[12][0-9][0-9][0-9]/){
					$entry1=$mf;
				}
			}
		}
		#Query 2
		if ($arr=='43' || $arr=='34'){
			#Take out the entry number
			$quer=2;
			my $fff=1;
			for my $mf (@q){
				if ($mf=~ /[12][0-9][0-9][0-9]/){
					if ($fff){
						$entry1=$mf;
						$fff=0;
					}
					else{
						$entry2=$mf;
					}
				}
			}
		}
		#Query 3
		if ($arr=='54' || $arr=='45'){
			#Take out the entry number
			$quer=3;
			my $fff=1;
			for my $mf (@q){
				if ($mf=~ /[12][0-9][0-9][0-9]/){
					if ($fff){
						$entry1=$mf;
						$fff=0;
					}
					else{
						$entry2=$mf;
					}
				}
			}
		}
		#Query 4
		if ($arr=='47' || $arr=='74' ||$arr=='4'){
			$quer=4;
		}
		#Query 5
		if ($arr=='8'){
			$quer=5;
			for my $mf (@q){
				if ($mf=~ /[12][0-9][0-9][0-9]/){
					$entry1=$mf;
				}
			}
		}
		#Query 6
		if ($arr=='89'||$arr=='98'){
			$quer=6;
			for my $mf (@q){
				if ($mf=~ /[12][0-9][0-9][0-9]/){
					$entry1=$mf;
				}
			}
		}
		print "Query:".$quer."\n";
	}
	else{
		print "No Query Match\n";
	}
	my @args=("./analyzer",$quer,$entry1,$entry2);
	system(@args)==0
		or die "system failed";

	print "Do you want to continue?\n";
	$he=<STDIN>;
	chomp $he;
	if ($he eq "no" || $he eq "No" || $he eq "NO"){
		print "Exiting....";
		last;
	}
	elsif ($he eq "yes" || $he eq "Yes" || $he eq "YES"){
		
	}
	else{
		print "Invalid Choice...Exiting....";
		last;
	}

}
