# Button
button .b -text "Goodbye, world?" -command exit
pack .b

# Canvas
canvas .c -width 300 -height 300 -bg white
pack .c
set rect [.c create rectangle 100 100 200 200 -fill red -outline blue -width 5]
.c itemconfigure $rect -fill yellow
.c delete $rect
.c create rectangle 100 100 200 200 -fill red -outline blue -width 5 -tags rset
.c create rectangle 150 150 250 250 -fill blue -outline red -width 5 -tags rset
.c delete rset

# Frame
frame .sf
pack .sf -expand 1 -fill x
# Scale in frame
scale .sf.rxscroll -label "X Rotate" -length 500  -from 0 -to 360  -orient horiz
pack .sf.rxscroll
# Frame for a button
frame .bf
pack .bf -expand 1 -fill x
button .bf.exit -text "Exit" -command {exit}
pack .bf.exit

# Binding 1
canvas .c -height 300 -width 300
pack .c
.c create oval 95 105 105 95 -fill red -tag circle
.c bind circle <Button-1> {.c itemconfigure circle -fill blue}
.c bind circle <Double-1> +{exit}

# Binding 2

canvas .c -width 300 -height 300 -bg white
pack .c

.c create line 0 100 300 100 -width 4
.c create line 0 200 300 200 -fill red
.c create line 100 0 100 300
.c create line 200 0 200 300

.c create text 0 0 -anchor nw -text "Northwest Anchor"
.c create text 0 100 -anchor w -text "West Anchor" \
  -tags westtext
.c create text 150 150 -anchor c -text "Centered"
.c create text 150 300 -anchor s -text "South" -fill red \
  -font "-adobe-helvetica-medium-o-normal--18" \
  -tags southtext

.c bind westtext <Button-1> {\
	.c itemconfigure westtext -text "Foo"}

set scolor navyblue

.c bind southtext <Button-1> {\
	.c itemconfigure southtext -fill $scolor; \
	if {$scolor == "navyblue"} {set scolor red} else {set scolor navyblue}}

