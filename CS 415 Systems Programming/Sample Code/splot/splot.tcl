# splot.tcl

# Called whenever we replot the points
proc replot val {
        puts stdout "p .c  [.sf.rxscroll get] \
                           [.sf.ryscroll get] \
                           [.sf.rzscroll get] \
                           [.sf.sscroll  get] 150"
        flush stdout
}

# Create canvas widget
canvas .c -width 300 -height 300 -bg white
pack .c -side top

# Frame to hold scrollbars
frame .sf
pack  .sf -expand 1 -fill x

# Scrollbars for rotating view.  Call replot whenever
# we move them.
scale .sf.rxscroll -label "X Rotate" -length 300 \
 -from 0 -to 360 -command "replot" -orient horiz

scale .sf.ryscroll -label "Y Rotate" -length 300 \
 -from 0 -to 360 -command "replot" -orient horiz

scale .sf.rzscroll -label "Z Rotate" -length 300 \
 -from 0 -to 360 -command "replot" -orient horiz

# Scrollbar for scaling view.
scale .sf.sscroll -label "Scale" -length 300 \
  -from 1 -to 1000 -command "replot" -orient horiz \
  -showvalue 0
  .sf.sscroll set 300

# Pack them into the frame
pack .sf.rxscroll .sf.ryscroll .sf.rzscroll \
     .sf.sscroll -side top

# Frame for holding buttons
frame .bf
pack  .bf -expand 1 -fill x

# Exit button
button .bf.exit -text "Exit" -command {exit}

# Reset button
button .bf.sreset -text "Reset" -command \
    {.sf.sscroll set 300; .sf.rxscroll set 0;
     .sf.ryscroll set 0; .sf.rzscroll set 0; replot 0}

# Dump PostScript
button  .bf.psout -text "Dump postscript" -command \
    {.c postscript -colormode gray -file "ps.out"}

# Pack buttons into frame
pack .bf.exit .bf.sreset .bf.psout -side left \
    -expand 1 -fill x

#Call replot
replot 0
