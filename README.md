commontools
===========

##polar2garfield
#####Requirements:

1. Evironment viriable `MYBIN` is set. If you don't want this, you may delete `$(MYBIN)` at line 34 and line 42 in `Makefile`
2. ROOT is properly set, thus `$ROOTSYS` has a meaningful value.
   To do this, you may `source /path/to/root/bin/thisroot.sh`

####Installation:
`make`

####Usage:
1. Prepare an input file, say `mcrcellprt.in` (You'd better not change the format)  
   A simple example is:

		Global P_cel_1 = 13
		Global N_cel_1 = 224
		ROWS Polar
		s01 {P_cel_1}*1 {S_dia} 51.00 360.0*i/{N_cel_1} {V_max} {S_ten} {L} {S_den}
		
		&stop
2. Prepare a configuration file, say `config.txt`  
   You may simply copy parameter lines from `mcrcellprt.in` to `config.txt`
   A simple example is:

		Global P_cel_1 = 13
		Global N_cel_1 = 224
		
3. If you are in the directory holding `polar2garfield` and `mcrcellprt.in`, you may simply type  

		./polar2garfield -c config.txt -i mcrcellprt.in -o output.txt
		
   Then your result could be found in `output.txt` like
   
		s01   0     51         0
		s01   1     50.9799    1.43036
		s01   2     50.9198    2.85959
		s01   3     50.8195    4.28658
		s01   4     50.6793    5.71019
		s01   5     50.4992    7.12931
		s01   6     50.2794    8.54282
		s01   7     50.02      9.94961
		s01   8     49.7213    11.3486
		s01   9     49.3835    12.7386
		s01   10    49.0068    14.1186
		s01   11    48.5915    15.4875
		s01   12    48.138     16.8442

	 Where each column stands for `layer name, wire index, x, y` corespondingly
