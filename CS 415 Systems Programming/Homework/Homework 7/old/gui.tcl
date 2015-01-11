#TODO: make buttons do things - all 9 buttons
#TODO: make bottom buttons do things
#TODO: connect with ttt.c

#
# board canvas
#
canvas .c -width 300 -height 300 -bg white
pack   .c -padx  20  -pady   20

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

#
# info canvas
#
canvas .i -width 300 -height 80 -bg white
pack   .i -padx  20  -pady   20

.i create text 50 20 -text "you are:  " -tags player
.i create text 50 40 -text "opponent: " -tags opp
.i create text 50 60 -text "status: " -tags status

#
# button frame
#
frame .f
pack .f -side bottom -anchor s -expand 1 -fill x

button .f.i -text "silence" -command {exit}
button .f.e -text "exit" -command {exit}
button .f.r -text "resign" -command {exit}

pack .f.i .f.e .f.r -side left -expand 1 -fill x