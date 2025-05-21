20250405

------------------------------------------------------------------
Author: Chandana R, Rahul G Mohite
Version: 0.1 [Initial Draft]
------------------------------------------------------------------
This folder contains the codes reqired for the main project
The contents of the folder are as follows

+---Chandana_Motor.ino
+---Read Me_Motor
------------------------------------------------------------------
Chandana_Motor.ino us the program which is uded to move car in one direction with certain Speed
Note: Speed must be (0 to 255)

                    +----------------------+
                    |                      |
                    |----                  |
       Programmer---|    |                 |
                    |----                  |
                    |                      |
            M1------|                      |------M4
                    |     Motor Shield     |
            M2------|                      |------M3
                    |                      |
                    |   _ _                |
                    |  | | |               |
                    +---|-|----------------+
		        | |
	           	+ -    +----------------------------------+
                 	+9V    |_                                _|
		        | |----|_|OUT-                       In-|_|------------------|
			|      |  				  |                 -|
			|      |       Step-Down Converter        |                [12V]  Battery
			|      |_                                _|                 +|
			|------|_|OUT+                       In+|_|-------[Switch]---|
			       |                                  |
			       +----------------------------------+
								
------------------------------------------------------------------
Note: Code must be used in Arduino UNO.
