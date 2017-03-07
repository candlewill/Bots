
use strict ;
use Tk ;
use Time::HiRes  qw/ usleep  gettimeofday /;

my ( 	$version,
		$judge ,
		@received_characters, 	#	Array containing log characters
		$file,
		$left,		#	Human/Program for left side 
		$right,		#		ditto for right side
		$hide,		#  0/1 show Human/Program or Other
		$speed ,	
) ;

my (	$mw,
		$scrheight ,
		$scrwidth ,
		$screensize,	
		%text_frames,		#	Frame to hold text widgets
		%text_window,		#	widget to hold ROText to display output					
		$buttons_frame,		#	Frame to hold buttons
		%button,			#	Buttons for judge, etc
		$scale,				#	Scale to adjust speed of playback
		%label,				#	Labels for Judges, Human, Program, Other
) ;

$version = '1.0.0' ;	#	initialize program version number
$speed = 5 ;			#	initialize speed of playback

#	Create and display main screen

$mw = MainWindow -> new(- title => "LoebnerPlayer $version") ;
$mw -> bind('<Destroy>' => sub{exit}) ;
$scrheight = $mw -> screenheight ;
$scrwidth = $mw -> screenwidth  ;
$scrwidth = $scrwidth;
$screensize = ($scrwidth).'x'.($scrheight-90). '+0+0' ;
$mw -> geometry($screensize) ;

$text_frames{left}{remote} = $mw -> Frame( 	-background => 'white',
											-borderwidth => 4, 
											-relief => 'groove' ) -> form(-t => '%0', -l => '%0', -b => '%45', -r => '%50') ; 
		 
$label{left}{remote} = $text_frames{left}{remote} -> Label(	-text =>"Other", 
									-font => ['Times',18,'bold'],
									-justify => 'center',
									-background => 'white' ) -> form(-t => '%0 ', -l => '%0 ',  -r => '%100') ;
									
$text_window{left}{remote} = $text_frames{left}{remote} -> Scrolled('ROText',	-font => ['Times',12,],    # changed to scrolled
									-borderwidth => 4, 
									-background => 'white',
									-relief => 'groove',
									-wrap => 'char',
									-takefocus => 0 ) -> form(-t => $label{left}{remote}, -l => '%0 ',  -r => '%100 ', -b => '%100') ;
									
$text_frames{left}{local} = $mw -> Frame( 	-background => 'white',
									-borderwidth => 4, 
									-relief => 'groove' ) -> form(-t => '%45', -l => '%0', -b => '%90', -r => '%50') ; 


$label{left}{local} = $text_frames{left}{local} -> Label(	-text =>"Judge", 
									-font => ['Times',18,'bold'],
									-justify => 'center',
									-background => 'white' ) -> form(-t => '%0 ', -l => '%0 ',  -r => '%100') ;

$text_window{left}{local} = $text_frames{left}{local} -> Scrolled('ROText',	-font => ['Times',12,], 
									-borderwidth => 4, 
									-background => 'white',
									-relief => 'groove',
									-wrap => 'char' ) -> form(-t => $label{left}{local}, -l => '%0 ',  -r => '%100 ', -b => '%100') ;
	
$text_frames{right}{remote} = $mw -> Frame( -background => 'white',
							-borderwidth => 4, 
							-relief => 'groove' ) -> form(-t => '%0', -l => '%50', -b => '%45', -r => '%100') ; 
						 
$label{right}{remote} = $text_frames{right}{remote} -> Label(	-text =>"Other", 
									-font => ['Times',18,'bold'],
									-justify => 'center',
									-background => 'white' ) -> form(-t => '%0 ', -l => '%0 ',  -r => '%100') ;
									
$text_window{right}{remote} = $text_frames{right}{remote} -> Scrolled('ROText',	-font => ['Times',12,],    # changed to scrolled
										-borderwidth => 4, 
										-background => 'white',
										-relief => 'groove',
										-wrap => 'char' ) -> form(-t => $label{right}{remote}, -l => '%0 ',  -r => '%100 ', -b => '%100') ;	
																	
$text_frames{right}{local} = $mw -> Frame( 	-background => 'white',
									-borderwidth => 4, 
									-relief => 'groove' ) -> form(-t => '%45', -l => '%50', -b => '%90', -r => '%100') ;
						
$label{right}{local} = $text_frames{right}{local} -> Label(	-text =>"Judge", 
										-font => ['Times',18,'bold'],
										-justify => 'center',
										-background => 'white',
										-borderwidth => 4, ) -> form(-t => '%0 ', -l => '%0 ',  -r => '%100 ') ;

$text_window{right}{local} = $text_frames{right}{local} -> Scrolled('ROText',	-font => ['Times',12,],    # changed to scrolled
									-borderwidth => 4, 
									-background => 'white',
									-relief => 'groove',
									-wrap => 'char',
									-relief => 'groove',
									-borderwidth => 6 ) -> form(-t => $label{right}{local}, -l => '%0 ',  -r => '%100 ', -b => '%100') ;
									
	
$buttons_frame = $mw -> Frame(-background => 'white') -> form (-t => '%90', -l => '%0', -r => '%100', -b => '%100') ;
	
$scale = $buttons_frame -> Scale(	-orient=> 'horizontal',
										-background => 'white',
										-activebackground => 'SlateBlue1',
										-from => 1, 
										-to => 100,
										-label => 'Playback Speed',
										-variable => \$speed,
										-tickinterval => 5,) -> form( -t => '%5', -l => '%5',  -r => '%45') ;
										
$button{get_log} = $buttons_frame -> Button(-text => 'Which Log?', 
											-font=>['courier',14],
											-background => 'SteelBlue1',
											-activebackground => 'SlateBlue1',
											-command=> sub{ picklog() } ) 		-> form( -t => '%15', -l => [$scale,100]) ;
										
$button{J1} = $buttons_frame -> Button(	-text => 'J1', 
										-font=>['courier',14,'bold'],
										-background => 'SteelBlue1',
										-activebackground => 'SlateBlue1', 
										-command=> sub{ pickjudge( 'J1') } ) 	-> form( -t => '%15', -l => [$button{get_log},100]) ;
										
$button{J2} = $buttons_frame -> Button(	-text => 'J2', 
										-font=>['courier',14,'bold'],
										-background => 'SteelBlue1',
										-activebackground => 'SlateBlue1',
										-command=> sub{ pickjudge( 'J2') } ) 	-> form( -t => '%15', -l => [$button{J1},50]) ;
										
$button{J3} = $buttons_frame -> Button(	-text => 'J3', 
										-font=>['courier',14,'bold'],
										-background => 'SteelBlue1',
										-activebackground => 'SlateBlue1', 
										-command=> sub{ pickjudge( 'J3') } ) 	-> form( -t => '%15', -l => [$button{J2},50]) ;
										
$button{J4} = $buttons_frame -> Button(	-text => 'J4', 
										-font=>['courier',14,'bold'],
										-background => 'SteelBlue1',
										-activebackground => 'SlateBlue1', 
										-command=> sub{ pickjudge( 'J4') } ) 	-> form( -t => '%15', -l => [$button{J3},50]) ;
										
$button{hideside} =  $buttons_frame -> Checkbutton( 	-text => "Hide\nSides",
														-background => 'white',
														-activebackground => 'SlateBlue1',
														-variable => \$hide,
														-font=>['courier',14,'bold'] )-> form( -t => '%15', -l => [$button{J4},50]) ;		
MainLoop ;

sub pickjudge{
	$judge = shift ;
	unless( $file){
		$mw -> messageBox(-type => 'ok', -message => "Please pick a log file") ;
		$button{get_log} -> flash ;
		$button{get_log} -> flash ;
		return ;
	}	
	foreach my $left_right_index('left', 'right'){		# clear windows
		foreach my $local_remote_index('local','remote'){
			$text_window{$left_right_index}{$local_remote_index} -> delete('0.0', 'end') ;
		}		
	} ;
	unless($hide){
		findsides() ;	#	Configure labels for left and right windows1
	}
	else{
		$label{left}{remote} -> configure(-text => "Other") ; 
		$label{right}{remote} -> configure(-text => "Other") ;  		
	}
		sub findsides{
			my $jindex = 0 ;
			foreach my $line(@received_characters){
				chomp $line ;
				my ($ijudge, $time, undef, $location,$side,$char) = split " ",$line ;
				if($ijudge eq $judge){
					my $templine = $received_characters[ $jindex - 1] ;
					$templine =~ /^.*left\s*=\s*(\w*)/ ;
					$left = $1 ;
					$templine =~ /^.*right\s*=\s*(\w*)/ ;
					$right = $1  ;
					$label{left}{remote} -> configure(-text => "\u$left") ; 
					$label{right}{remote} -> configure(-text => "\u$right") ;  
					return ;
					#$templine =~ /               /
				}
				$jindex ++ ;
				}
		}	
	insert_characters() ;	
}

sub picklog{
	@received_characters = () ;	
	$file = $mw->getOpenFile() ;
	open(INFILE, "<$file") ;
	@received_characters = <INFILE> ;
	$button{J1} -> flash ;
 	$button{J2} -> flash ;
 	$button{J3} -> flash ;
 	$button{J4} -> flash ;
 	pickjudge() ;
}


sub insert_characters{
	unless(Exists $mw){exit} ;
	my ($lo, $hi)	;
	my $hold_time = 0;
	my $diff_time = 0 ;
	my $sleep_time ;
	my $index = 0 ;
	foreach my $line(@received_characters){
		my ($ijudge, $time, undef, $location,$side,$char) = split " ",$line ;
		if($ijudge eq $judge){
			$index ++ ;
			chomp $line ;
			$time =~ /(\d\d\d\d\d\d\d\d\d\d\d)(\d\d\d\d\d\d\d)$/ ;
			$lo = $1;
			$hi = $2;
			if($hold_time == 0){
				$hold_time = $hi ;
			}
			if($hi < $hold_time){$hi +=1000000} ;  
			$diff_time = ($hi - $hold_time)/$speed ;
			Time::HiRes::usleep($diff_time) ;
			if($char eq 'CR'){$char = "\n"} ;
			if($char eq 'space'){$char = ' '} ;
			if($char eq 'BackSpace'){
				$text_window{$side}{$location} -> delete('end - 2 chars','end - 1 chars') ;
			}			
			else{			
				$text_window{$side}{$location} -> insert('end',$char) ;
				$text_window{$side}{$location} -> see('end',) ;	
				$text_window{$side}{$location} -> update ;	
			}
		}	
	}
}