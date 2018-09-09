# gui.tcl	-	Robert Johns	-	April 25, 2014
# tcl script to create base for gui for tic-tac-toe client.
# communicates with c program ttt.c through pipes through a child process.
# beeps unless sound is silenced, kills window with exit if game ended.
# game ends through win, loss, draw or resignation by a client.

set silent 0
set turn 0
set done 0

#
# board canvas
#
canvas .c -width 300 -height 300 -bg white
pack   .c

# create horizontal lines
.c create line 0 100 300 100 -width 2
.c create line 0 200 300 200 -width 2

# create vertical lines
.c create line 100 0 100 300 -width 2
.c create line 200 0 200 300 -width 2

# create rectangles top left to bottom right
.c create rectangle 0   0   98  98  -fill white -outline white -tags mb1
.c create rectangle 101 0   198 98  -fill white -outline white -tags mb2
.c create rectangle 201 0   298 98  -fill white -outline white -tags mb3
.c create rectangle 0   101 98  198 -fill white -outline white -tags mb4
.c create rectangle 101 101 198 198 -fill white -outline white -tags mb5
.c create rectangle 201 101 298 198 -fill white -outline white -tags mb6
.c create rectangle 0   201 98  298 -fill white -outline white -tags mb7
.c create rectangle 101 201 198 298 -fill white -outline white -tags mb8
.c create rectangle 201 201 298 298 -fill white -outline white -tags mb9

#create text for displaying board
font create XO -family Helvetica -size 20 -weight bold
.c create text 49  49  -text " " -font XO -tags one
.c create text 149 49  -text " " -font XO -tags two
.c create text 249 49  -text " " -font XO -tags three
.c create text 49  149 -text " " -font XO -tags four
.c create text 149 149 -text " " -font XO -tags five
.c create text 249 149 -text " " -font XO -tags six
.c create text 49  249 -text " " -font XO -tags seven
.c create text 149 249 -text " " -font XO -tags eight
.c create text 249 249 -text " " -font XO -tags nine

#
# info canvas
#
canvas .i -width 300 -height 70 -bg white
pack   .i

.i create text 5 10 -text "status: " -anchor nw -tags status
.i create text 5 30 -text "you are: " -anchor nw -tags player
.i create text 5 50 -text "opponent: " -anchor nw -tags opp

#
# button frame
#
frame .f
pack .f -side bottom -anchor s -expand 1 -fill x

button .f.n -text "silence" -command {if {$silent == 0} {set silent 1; .f.n configure -text "sound"} else {set silent 0; .f.n configure -text "silence"}}
button .f.e -text "resign" -command {if {$done == 0} {set done 1; puts 0}}
button .f.r -text "exit" -command {if {$done == 1} {exit}}

pack .f.n .f.e .f.r -side left -expand 1 -fill x

#
# button info for board
#
.c bind mb1 <Button-1> {if {$turn == 1} {puts 1}}
.c bind mb2 <Button-1> {if {$turn == 1} {puts 2}}
.c bind mb3 <Button-1> {if {$turn == 1} {puts 3}}
.c bind mb4 <Button-1> {if {$turn == 1} {puts 4}}
.c bind mb5 <Button-1> {if {$turn == 1} {puts 5}}
.c bind mb6 <Button-1> {if {$turn == 1} {puts 6}}
.c bind mb7 <Button-1> {if {$turn == 1} {puts 7}}
.c bind mb8 <Button-1> {if {$turn == 1} {puts 8}}
.c bind mb9 <Button-1> {if {$turn == 1} {puts 9}}

#
# button info for frame
#
#silence
#resign
#exit
