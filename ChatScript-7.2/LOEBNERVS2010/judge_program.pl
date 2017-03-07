use Tkx;
use Cwd ;
use Time::HiRes qw(  gettimeofday );	#used to create timestamp
use feature 'say' ;
use strict ;
use warnings;
my $cwd = getcwd();
# chdir  Tkx::tk___chooseDirectory(-title => "Choose Communications Directory") ;
#my $comm_dir =  Tkx::tk___chooseDirectory(-title => "Choose Communications Directory") ;
#chdir  "C:/Users/Public/Loebner_Prize_testing/communications";
my (%inputchar,@inputs,
	$judge_number,$round_number,
	@received_characters,#	array of sub_directory names, i.e. transmitted characters
	%special_chars,	 	#	hash of character names set by sub setup_chars()
	%folder_name,	 	#	holds names of human, program, and webcast folders
	%processed,		#	hash to track character
	$program,			#	program name, extracted for name of program comm folder
	%judge_human_side,  	#	see below
);
# 
# 	%judge_human_side
#
#	This hash is set in sub change_round to randomly assign sides
#    $j = (int(rand()*10))%2;   set $j to zero or one
#    if($j==0){
#		$judge_human_side{left}='human';
#		$judge_human_side{human}='left';
#		$judge_human_side{right}='program';
#		$judge_human_side{program}='right';
#    }
#    elsif($j==1){	not simple else, because there is an error check in change_round
#		$judge_human_side{left}='program';
#		$judge_human_side{program}='left';
#		$judge_human_side{right}='human';
#		$judge_human_side{human}='right';
#	}


#	Tks window widgets
my( %entry,
%output_window,
%scroll
);

my %logfile_name;
# chdir  $comm_dir;

#	Now extract program name for program's folder name
#	This assumes that the program comm folder will be named for program


my ($sec, $min, $hour, $day, $month, $year, $week_day, $day_of_year, $isdst, $hi, $lo, $local_time, $remote_time) = localtime() ;
$year  += 1900 ;
$month  ++ ;	#	increment so that Jan is 01 instead of 00
$sec = sprintf '%02d', $sec ;
$min = sprintf '%02d', $min ;
$hour = sprintf '%02d', $hour ;
$day = sprintf '%02d', $day ;
$month = sprintf '%02d', $month ;




my $title = "   23rd Annual Loebner Prize\nDerry ~ LondonDerry ~ LegenDerry UK 14 September 2013";

#	initialize special characters hash  eg $special_chars{braceleft} == '{',
setup_chars() ;

# ----------- set up Main Window
my $mw = Tkx::widget->new(".");
#	size main window
my $height	= $mw -> Tkx::winfo_screenheight ;
my $width	= $mw -> Tkx::winfo_screenwidth ;
$height -= 70;
$width -=  30;
my $size = $width .'x'. $height . '+0+5';
#$mw -> g_wm_geometry($size);

#	size output windows
my $textht = int($height / 40);
my $textw = int($width/20);

#	create a frame
my $fr = $mw->new_ttk__frame(-padding => 15);
$fr -> g_grid(-row =>0, -column=>0, -sticky => 'nsew');

#		now populate frame
#	Set up labels
$fr -> new_ttk__label(-text =>$title ,-font=>["times",18])-> g_grid(-column => 1, -row=>0,-columnspan =>7);
$fr -> new_ttk__label(-text => "Other\nEntity",-font=>["times",14])-> g_grid(-column => 0, -row=>2,-sticky => 'ew');
$fr -> new_ttk__label(-text => "",-font=>["times",14])-> g_grid(-column => 0, -row=>3,-sticky => 'ew');

$fr -> new_ttk__label(-text => "Judge Number",-font=>["times",14])-> g_grid(-column => 2, -row =>0 ,-sticky => 'e');
$entry{judge_num} = $fr->new_ttk__entry(-textvariable => \$judge_number, -width =>3) ;
$entry{judge_num} -> g_grid(-column => 3, -row =>0 ,-columnspan => 2, -sticky =>'w'); ;

$fr -> new_ttk__label(-text => "Round Number",-font=>["times",14])-> g_grid(-column => 5, -row =>0 ,-sticky => 'e');
$entry{round_num} = $fr->new_ttk__entry(-textvariable => \$round_number, -width =>3) ;
$entry{round_num} -> g_grid(-column => 6, -row =>0 ,-columnspan => 2, -sticky =>'w'); ;

$fr -> new_ttk__label(-text => "Left",-font=>["times",14])-> g_grid(-column => 2, -row =>1 ,-columnspan => 2);
$fr -> new_ttk__label(-text => "Right",-font=>["times",14])-> g_grid(-column => 5, -row =>1 ,-columnspan => 2);
$fr -> new_ttk__label(-text => "My Left Entry ->",-font=>["times",14], -background => 'yellow')-> g_grid(-column => 2, -row=>5, -sticky => 'e');
$fr -> new_ttk__label(-text => "My Right Entry ->",-font=>["times",14], -background => 'yellow')-> g_grid(-column => 5, -row=>5, -sticky => 'e');

#	create output windows
$output_window{left}{judge} = $fr->new_tk__text(-width=>$textw,-height=>$textht,-state=>'disabled',-background =>'seashell1');
$output_window{left}{judge}->g_grid(-row => 3, -column => 2, -columnspan => 2);
$scroll{left}{judge} = $fr ->new_ttk__scrollbar(-command => [$output_window{left}{judge}, "yview"], -orient => "vertical");
$output_window{left}{judge} ->configure(-yscrollcommand=>[$scroll{left}{judge},"set"]);
$scroll{left}{judge} -> g_grid(-column =>1, -row => 3, -sticky => 'ns');

$output_window{left}{other} = $fr->new_tk__text(-width=>$textw,-height=>$textht,-state=>'disabled',-background =>'seashell1');
$output_window{left}{other}->g_grid(-row => 2, -column=>2,-columnspan => 2);	
$scroll{left}{other} = $fr ->new_ttk__scrollbar(-command => [$output_window{left}{judge}, "yview"], -orient => "vertical");
$output_window{left}{other} ->configure(-yscrollcommand=>[$scroll{left}{other},"set"]);
$scroll{left}{other} -> g_grid(-column =>1, -row => 2, -sticky => 'ns');

$output_window{right}{judge} = $fr->new_tk__text(-width=>$textw,-height=>$textht,-state=>'disabled',-background =>'seashell1' );
$output_window{right}{judge}->g_grid(-row => 3, -column => 5, -columnspan => 2);
$scroll{right}{judge} = $fr ->new_ttk__scrollbar(-command => [$output_window{right}{judge}, "yview"], -orient => "vertical");
$output_window{right}{judge} ->configure(-yscrollcommand=>[$scroll{right}{judge},"set"]);
$scroll{right}{judge} -> g_grid(-column =>4, -row => 3, -sticky => 'ns');


$output_window{right}{other} = $fr->new_tk__text(-width=>$textw,-height=>$textht,-state=>'disabled',-background =>'seashell1');
$output_window{right}{other}->g_grid(-row => 2, -column=>5,-columnspan => 2);
$scroll{right}{other} = $fr ->new_ttk__scrollbar(-command => [$output_window{right}{judge}, "yview"], -orient => "vertical");
$output_window{right}{other} ->configure(-yscrollcommand=>[$scroll{right}{other},"set"]);
$scroll{right}{other} -> g_grid(-column =>4, -row => 2, -sticky => 'ns');

#	Create Judge's entry windows 
$entry{left} = $fr->new_ttk__entry(-textvariable => \$inputchar{left}, -width =>3) ;
$entry{right} = $fr->new_ttk__entry(-textvariable => \$inputchar{right}, -width =>3) ;

Tkx::ttk__style_configure("ShowEntry.TEntry",  -padding => 10, -relief=> 5);

$entry{left}->configure(-style => "ShowEntry.TEntry");
$entry{left} -> g_grid(-column => 3, -row => 5, -sticky => 'w');
$entry{right}->configure(-style => "ShowEntry.TEntry");
$entry{right} -> g_grid(-column => 6, -row => 5, -sticky => 'w');
$entry{left}->g_bind  ("<KeyPress>", [  sub {   my($x,$y) = @_;
											$x = insert_treat_char(char =>$x, side =>'left', who =>'judge');
											}, Tkx::Ev("%K", "%N")
						]
		);
$entry{right}->configure(-style => "ShowEntry.TEntry");
$entry{right} -> g_grid(-column => 6, -row => 5, -sticky => 'w');
$entry{right}->g_bind
("<KeyPress>",
   [  sub	{my($x,$y) = @_;
		 $x = insert_treat_char(char =>$x, side =>'right', who =>'judge');
		 },
	  Tkx::Ev("%K", "%N")
   ]
);


my $round_Button = $fr -> new_button(	-text => "New\nRound",
				-font => ['courier',14,'bold'],
				-background => 'green',
				-command => sub{ new_round() },
				-takefocus => 0,
				) 
-> g_grid (-row => 2, -column=>7 );
	   
my $reset_Button = $fr -> new_button(-text => "Reset!",
				-font => ['courier',14,'bold'],
				-background => 'red',
				-command => sub{ reset_round() },
				-takefocus => 0,
				) 
-> g_grid (-row => 3, -column=>7 ) ;






sub endless_loop{
while(1){ 	
	get_remote_char();
	Tkx::update();		# Otherwise hangs up
};
}
#---------------------------------------------------------------------------------------------------------

sub insert_treat_char{

	#
	#	this subroutine accepts characters from entry via bind or
	#	subroutine get_remote_char() via call
	#	It inserts character onto screen, creates subdirecties and logfile entry
	#
	my $sub_dir_name;
	my %args = @_ ;	# get call arguments via hash - std. Perl subroutine calls
	my $what_to_insert  = $args{char} ;	#	get character to enter
	my $who = $args{who} ;  	# 'judge' or 'other'
	my $side = $args{side};	# 'left' or 'right'
	$inputchar{$side} ="";	#  clear entry window	
	$remote_time = $args{remote_time} ;	# from get_remote_char, otherwise blank
	#	setup time stamp
	($lo, $hi)= gettimeofday ;
	$hi = sprintf '%07u',  $hi ; # left fill with zeros
	$lo = sprintf '%011u', $lo ; #    ditto
	$local_time = $lo.$hi ;
	
	my $len = length $what_to_insert ;  # simple letter or special character ?
	if($len == 1){	#	simple character
		$output_window{$side}{$who}-> configure(-state => 'normal');
		$output_window{$side}{$who} -> insert("end","$what_to_insert");
		$output_window{$side}{$who} -> see('end');
		$output_window{$side}{$who} ->configure(-state=>'disabled');
	}
	if ($len > 1){	#	special character - now convert
		if( exists $special_chars{$what_to_insert } ){
			unless($what_to_insert eq 'BackSpace'){
				$what_to_insert = $special_chars{$what_to_insert } ;
				$output_window{$side}{$who}-> configure(-state => 'normal');
				$output_window{$side}{$who} -> insert("end","$what_to_insert");
				$output_window{$side}{$who} -> see('end');
				$output_window{$side}{$who} ->configure(-state=>'disabled');
			}
			else{	#	backspace - eliminate prev. character
			   $output_window{$side}{$who}-> configure(-state => 'normal');
			   $output_window{$side}{$who} -> delete( "end -2 chars", "end -1 chars"  ) ;	#	Yes, delete last character					
			   $output_window{$side}{$who} -> see( "end"  ) ;
			   $output_window{$side}{$who} ->configure(-state=>'disabled');		
			}          
		}
		else{return};
	}	#	end special character routine
	#	Now create webcast information time, side, judge/human, character
	#	restore character "space" to create name
	if($what_to_insert eq ' '){$what_to_insert = 'space'};
	if($who eq 'judge'){  # send info to human or program comm folder
		chdir "$folder_name{ $judge_human_side{$side}}" or die "Can't chdir  $folder_name{ $judge_human_side{$side}} $!";
		$sub_dir_name = "$local_time.$args{char}.judge";
		mkdir $sub_dir_name ;
		if($side eq 'left'){
			chdir $folder_name{ webcast_left};
			$sub_dir_name = "$local_time.$side.judge.$args{char}.webcast";
			mkdir $sub_dir_name ;
			print LOGFILE  "J$judge_number $local_time judge $side $args{char}\n" ;
		}
		elsif($side eq 'right'){
			chdir $folder_name{ webcast_right};
			$sub_dir_name = "$local_time.$side.judge.$args{char}.webcast";
			mkdir $sub_dir_name ;
			print LOGFILE  "J$judge_number $local_time judge $side $args{char}\n" ;
		}
	}
	else{  # other's character
		if($side eq 'left'){
			chdir $folder_name{ webcast_left};
			if($judge_human_side{$side} eq 'program'){
				$sub_dir_name = "$local_time.$side.$program.$args{char}.webcast";
				mkdir $sub_dir_name ;
				print LOGFILE  "J$judge_number $local_time $program $side $args{char}\n" ;
	
			}
			else{
				$sub_dir_name = "$local_time.$side.human.$args{char}.webcast";
				mkdir $sub_dir_name ;
				print LOGFILE  "J$judge_number $local_time human $side $args{char}\n" ;
	
			}
		}
		else{
			chdir $folder_name{ webcast_right};
			if($judge_human_side{$side} eq 'program'){
				$sub_dir_name = "$local_time.$side.$program.$args{char}.webcast";
				mkdir $sub_dir_name ;
				print LOGFILE  "J$judge_number $local_time $program $side $args{char}\n" ;
	
			}
			else{
				$sub_dir_name = "$local_time.$side.human.$args{char}.webcast";
				mkdir $sub_dir_name ;
				print LOGFILE  "J$judge_number $local_time human $side $args{char}\n" ;
	
			}

		}
	}
}	# end insert_treat_char routine
	
	sub get_remote_char{
	#	This subroutine receives characters from program and human folders
	my ($remote_time, $insert_letter, $num_rcvd_chars);
	
	#	First check human folder, then program
	foreach  my $which_other ('human','program'){	   
	   chdir $folder_name{$which_other} ;	# e.g.  $folder_name{human} = name of human's folder	   
	   my $side = $judge_human_side{$which_other} ;  #  $side will be 'left' or 'right'
	   # fill array with names of sub_directories
	   @received_characters = glob("*.other")  ;	# get all subdirectories
	   $num_rcvd_chars = scalar @received_characters ;	# how many?
	   if( $num_rcvd_chars ){	#	if greater than zero
		  foreach my $sub_dir_name ( @received_characters ){	# get sub_directory names
	
			rmdir $sub_dir_name ;	#	First, remove the directory
			
			
			 ( $remote_time, $insert_letter, undef )= split '\.', $sub_dir_name ;	#	Extract info
			 unless(exists $processed{ $remote_time }{$side}  ){
				#	hash element does not exits, therefore this character has not yet been processed
				#	add element to hash indexed by time and side
				$processed{ $remote_time }{$side} = $insert_letter ;
				#	send to subroutine to show in screen, and print to log
				insert_treat_char(char => $insert_letter, who => 'other', side => $side, remote_time => $remote_time) ;
			 }
		  } ;
		   @received_characters = () ;
	   } ;
	}
}


sub new_round{
	my (%side, $j);
	#chdir  $comm_dir;
	$folder_name{webcast_left}	= Tkx::tk___chooseDirectory(-title => "Choose left webcast Folder") ;
	$folder_name{webcast_right}	= Tkx::tk___chooseDirectory(-title => "Choose right webcast Folder") ;
	$folder_name{human}		= Tkx::tk___chooseDirectory(-title => "Choose Human Folder") ;
	$folder_name{program}	= Tkx::tk___chooseDirectory(-title => 'Choose Program Folder' ) ;
	$program = $folder_name{program};
	$program =~ /.*\/(.*$)/ ;  	# capture everything from last "/" (excluding slash) as $1
	$program = $1 ;			# this will be the name of the program
	$logfile_name{log} =  "logfile.$program.$year-$month-$day--$hour-$min-$sec.log";
	$logfile_name{restart} =  "restart.log";
	open (LOGFILE, ">>$logfile_name{log}") ;
	print LOGFILE "Loebner Prize Sept 14 2013 Derry ~ LondonDerry ~ LegenDerry\n" ;
	print LOGFILE "These transcripts are in the public domain\n" ;
	print LOGFILE "Transcripts of judge $judge_number program $program round $round_number\n" ;
#	open (PROGRAM_STATUS,'>restart.log') or die "Can't open restart log $!";   
		#	randomly choose left/right to be judge/human
	$j = (int(rand()*10))%2;
	if($j==0){
		$judge_human_side{left}='human';
		$judge_human_side{human}='left';
		$judge_human_side{right}='program';
		$judge_human_side{program}='right';
	}
	elsif($j==1){
		$judge_human_side{left}='program';
		$judge_human_side{program}='left';
		$judge_human_side{right}='human';
		$judge_human_side{human}='right';
	}
	else{	#  Oh oh, big error
		Tkx::tk___messageBox(-type => "ok",
			-message => "Error  Error  \$j = $j",
			-icon => "info", -title => "Choice Made"); 	
	}
	Tkx::tk___messageBox(-type => "ok",
		-message => "left = $judge_human_side{left} \n right = $judge_human_side{right}\nJudge Number $judge_number ",
		-icon => "info", -title => "Choice Made");
	print LOGFILE "Left =$judge_human_side{left} \n" ;

	#	clear output screens
	foreach my $side('left', 'right'){
	   foreach my $who('judge','other'){
		$output_window{$side}{$who}-> configure(-state => 'normal');
		$output_window{$side}{$who} -> delete('0.0', 'end') ;
		$output_window{$side}{$who} ->configure(-state=>'disabled');
	   }		
	}
	#	setup status file if restart necessary
#	print PROGRAM_STATUS "$judge_human_side{left}\n" ;
#	print PROGRAM_STATUS "$judge_number\n" ;
#	print PROGRAM_STATUS "$round_number\n" ;


	#print PROGRAM_STATUS "$judge_human_side{right}\n" ;
	#print PROGRAM_STATUS "$judge_human_side{human}\n" ;
	#print PROGRAM_STATUS "$judge_human_side{program}\n" ;
	#print PROGRAM_STATUS "$folder_name{webcast}\n" ;
	#print PROGRAM_STATUS "$folder_name{human}\n" ;
	#print PROGRAM_STATUS "$folder_name{program}\n" ;

#	close PROGRAM_STATUS;
	endless_loop() ;
}

sub reset_round{
	# chdir  $comm_dir;
	open (PROGRAM_STATUS,'<restart.log')or die "Can't open $!";
	my (%side, $j);
	#chdir  $comm_dir;
	$folder_name{webcast}   = Tkx::tk___chooseDirectory(-title => "Choose webcast Folder") ;
	$folder_name{human}     = Tkx::tk___chooseDirectory(-title => "Choose Human Folder") ;
	$folder_name{program}   = Tkx::tk___chooseDirectory(-title => 'Choose Program Folder' ) ;
	$program = $folder_name{program};
	$program =~ /.*\/(.*$)/ ;       # capture everything from last "/" (excluding slash) as $1
	$program = $1 ;                 # this will be the name of the program
	$logfile_name{log} =  "logfile.$program.$year-$month-$day--$hour-$min-$sec.restart.log";
	$logfile_name{restart} =  "restart.log";
	#	input info on previous run
	my $info =<PROGRAM_STATUS>;
	$judge_number =<PROGRAM_STATUS>;
	chomp $judge_number ;
	$round_number =<PROGRAM_STATUS>;
	chomp $round_number;
	open (LOGFILE, ">>$logfile_name{log}") ;
	print LOGFILE "Loebner Prize 14 Sept 2013 Derry ~ LondonDerry ~ LegenDerry UK\n" ;
	print LOGFILE "These transcripts are in the public domain\n" ;
	print LOGFILE "Transcripts of *restarted* program $program\n" ;
	print LOGFILE "Transcripts of judge $judge_number program $program round $round_number\n" ;

	if($info =~ /human/){
		$judge_human_side{left}='human';
		$judge_human_side{human}='left';
		$judge_human_side{right}='program';
		$judge_human_side{program}='right';
	}
	elsif($info =~ /program/){
		$judge_human_side{left}='program';
		$judge_human_side{program}='left';
		$judge_human_side{right}='human';
		$judge_human_side{human}='right';
	}
	else{   #  Oh oh, big error
	Tkx::tk___messageBox(-type => "ok",
	-message => "Error  Error  \$j = $j",
	-icon => "info", -title => "Choice Made");      
	}
	Tkx::tk___messageBox(-type => "ok",
	-message => "left=$judge_human_side{left}\nright=$judge_human_side{right}\nround $round_number\njudge=$judge_number ",
	-icon => "info", -title => "Choice Made");
	
	print LOGFILE "Left=$judge_human_side{left} \n" ;

	
	#       clear output screens
	foreach my $side('left', 'right'){
		foreach my $who('judge','other'){
			$output_window{$side}{$who}-> configure(-state => 'normal');
			$output_window{$side}{$who} -> delete('0.0', 'end') ;
			$output_window{$side}{$who} ->configure(-state=>'disabled');
		}            
	}
	endless_loop() ;
}

Tkx::MainLoop();


sub setup_chars{
%special_chars =
 ( 	braceleft => '{',
	braceright => '}',
	bracketleft => '[',
	bracketright => ']',
	parenleft => '(',
	parenright => ')',
	space => ' ',
	comma => ',',
	period => '.',
	greater => '>',
	less => '<',
	slash => '/',
	backslash => '\\',
	bar => '|',
	quotedbl => '"',
	quoteright => "'",
	Tab => "\t",
	equal => '=',
	underscore => '_',
	plus => '+',
	minus => '-',
	exclam => '!',
	at => '@',
	numbersign => '#',
	dollar => '$',
	percent => '%',
	asterisk => '*',
	asciicircum => '^',
	asciitilde => '~',
	quoteleft => '`',
	ampersand => '&', 
	Return => "\n",
	colon => ":",
	semicolon => ";",
	question => "?",
	BackSpace => "BackSpace",
) ;
}