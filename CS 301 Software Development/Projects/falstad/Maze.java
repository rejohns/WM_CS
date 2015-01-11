package falstad;

import java.awt.*;

/**
 * Class handles the user interaction for the maze. 
 * It implements a state-dependent behavior that controls the display and reacts to key board input from a user. 
 * After refactoring the original code from an applet into a panel, it is wrapped by a MazeApplication to be a java application 
 * and a MazeApp to be an applet for a web browser. At this point user keyboard input is first dealt with a key listener
 * and then handed over to a Maze object by way of the keyDown method.
 *
 * This code is refactored code from Maze.java by Paul Falstad, www.falstad.com, Copyright (C) 1998, all rights reserved
 * Paul Falstad granted permission to modify and use code for teaching purposes.
 * Refactored by Peter Kemper
 */
// MEMO: original code: public class Maze extends Applet {
public class Maze extends Panel {

	Font smallBannerFont, largeBannerFont;
	
	static final int view_width = 400;
	static final int view_height = 400;
	int zscale = view_height/2;

	static final int map_unit = 128;
	static final int view_offset = map_unit/8;
	static final int step_size = map_unit/4;

	private RangeSet rset;
	private Graphics gc;

	private Image buffer_img;
	
	private int state;			// keeps track of the current GUI state, one of STATE_TITLE,...,STATE_FINISH, mainly used in redraw()
	// user can navigate 
	// title -> generating -(escape) -> title
	// title -> generation -> play -(escape)-> title
	// title -> generation -> play -> finish -> title
	// STATE_PLAY is the main state where the user can navigate through the maze in a first person view
	static final int STATE_TITLE = 1;
	static final int STATE_GENERATING = 2;
	static final int STATE_PLAY = 3;
	static final int STATE_FINISH = 4;
	
	private int percentdone = 0; // describes progress during generation phase
	private boolean showMaze;		 	// toggle switch to show overall maze on screen
	private boolean showSolution;		// toggle switch to show solution in overall maze on screen
	private boolean solving;			// toggle switch 

	final int viewz = 50;    
	int viewx, viewy, ang;
	int dx, dy;  // current direction
	int px, py ; // current position on maze grid (x,y)
	int walk_step;
	int view_dx, view_dy; // current view direction


	// debug stuff
	boolean deepdebug = false;
	boolean all_visible = false;
	boolean new_game = false;

	int mazew; // width of maze
	int mazeh; // height of maze
	// grid for maze
	Cells mazecells ;
	int[][] mazedists;
	Cells seencells ;
	//BSPNode bsp_root;
	
	private boolean map_mode; // true: display map of maze, false: do not display map of maze
	// map_mode is toggled by user keyboard input, causes a call to draw_map during play mode
	//int map_scale; relocated to mapdrawer

	// MapDrawer to perform drawing of maps
	MapDrawer mapdrawer ;
	// Drawer to get the first person perspective
	FirstPersonDrawer firstpersondrawer ;
	// Mazebuilder is used to calculate a new maze together with a solution
	// The maze is computed in a separate thread. It is started in the local Build method.
	// The calculation communicates back by calling the local newMaze() method.
	public MazeBuilder mazebuilder;


	
	// The user picks a skill level between 0 - 9, a-f 
	// The following arrays transform this into corresponding dimensions for the result maze as well as the number of rooms and parts
	static int skill_x[] =     { 4, 12, 15, 20, 25, 25, 35, 35, 40, 60,
		70, 80, 90, 110, 150, 300 };
	static int skill_y[] =     { 4, 12, 15, 15, 20, 25, 25, 35, 40, 60,
		70, 75, 75,  90, 120, 240 };
	static int skill_rooms[] = { 0,  2,  2,  3,  4,  5, 10, 10, 20, 45,
		45, 50, 50,  60,  80, 160 };
	static int skill_partct[] = { 60,
		600, 900, 1200,
		2100, 2700, 3300,
		5000, 6000, 13500,
		19800, 25000, 29000,
		45000, 85000, 85000*4 };

	// fixing a value matching the escape key
	final int ESCAPE = 27;


	/**
	 * Call back method for MazeBuilder to communicate newly generated maze as reaction to a call to build()
	 * @param root node for traversals, used for the first person perspective
	 * @param cells encodes the maze with its walls and border
	 * @param dists encodes the solution by providing distances to the exit for each position in the maze
	 * @param startx current position, x coordinate
	 * @param starty current position, y coordinate
	 */
	public void newMaze(BSPNode root, Cells c, int dists[][], int startx, int starty) {
		showMaze = showSolution = solving = false;
		mazecells = c ;
		mazedists = dists;
		seencells = new Cells(mazew+1,mazeh+1) ;
		//bsp_root = root; // delegated to firstpersondrawer
		//dx = 1; dy = 0;
		setCurrentDirection(1, 0) ;
		setCurrentPosition(startx,starty) ;
		walk_step = 0;
		view_dx = dx<<16; 
		view_dy = dy<<16;
		ang = 0;
		map_mode = false;
		// mazew and mazeh have been set in build() method before mazebuider was called to generate a new maze.
		// reset map_scale in mapdrawer to a value of 10
		mapdrawer = new MapDrawer(view_width,view_height,map_unit,step_size, mazecells, seencells, 10, mazedists, mazew, mazeh) ;
		
		firstpersondrawer = new FirstPersonDrawer(view_width,view_height,map_unit,step_size, mazecells, seencells, 10, mazedists, mazew, mazeh, root) ;
		// set the current state for the state-dependent behavior
		state = STATE_PLAY;
		// graphics update
		redraw();
		// the next line is needed because we're in a separate thread;
		// we can't seem to do a real paint in this thread, so tell
		// the main thread to do it.
		repaint();
	}

	
	private void setCurrentPosition(int x, int y)
	{
		px = x ;
		py = y ;
	}
	private void setCurrentDirection(int x, int y)
	{
		dx = x ;
		dy = y ;
	}
	
	
	void buildInterrupted() {
		state = STATE_TITLE;
		redraw();
		mazebuilder = null;
	}

	final double radify(int x) {
		return x*Math.PI/180;
	}



	/**
	 * Updates graphical output on screen, called from MazeBuilder
	 */
	public void redraw() {
		gc = buffer_img.getGraphics();

		switch (state) {
		case STATE_TITLE:
			redrawTitle(gc);
			break;
		case STATE_GENERATING:
			redrawGenerating(gc);
			break;
		case STATE_PLAY:
			redrawPlay(gc);
			break;
		case STATE_FINISH:
			redrawFinish(gc);
			break;
		}
		update(getGraphics());
	}

	private void centerString(Graphics g, FontMetrics fm, String str, int ypos) {
		g.drawString(str, (view_width-fm.stringWidth(str))/2, ypos);
	}

	/**
	 * Helper method for redraw to draw the title screen, screen is hardcoded
	 * @param gc graphics handler to manipulate screen
	 */
	private void redrawTitle(Graphics gc) {
		gc.setColor(Color.white);
		gc.fillRect(0, 0, view_width, view_height);
		gc.setFont(largeBannerFont);
		FontMetrics fm = gc.getFontMetrics();
		gc.setColor(Color.red);
		centerString(gc, fm, "MAZE", 100);
		gc.setColor(Color.blue);
		gc.setFont(smallBannerFont);
		fm = gc.getFontMetrics();
		centerString(gc, fm, "by Paul Falstad", 160);
		centerString(gc, fm, "www.falstad.com", 190);
		gc.setColor(Color.black);
		centerString(gc, fm, "To start, select a skill level.", 250);
		centerString(gc, fm, "(Press a number from 0 to 9,", 300);
		centerString(gc, fm, "or a letter from A to F)", 320);
		centerString(gc, fm, "v1.2", 350);
	}

	/**
	 * Helper method for redraw to draw final screen, screen is hard coded
	 * @param gc graphics handler to manipulate screen
	 */
	private void redrawFinish(Graphics gc) {
		gc.setColor(Color.blue);
		gc.fillRect(0, 0, view_width, view_height);
		gc.setFont(largeBannerFont);
		FontMetrics fm = gc.getFontMetrics();
		gc.setColor(Color.yellow);
		centerString(gc, fm, "You won!", 100);
		gc.setColor(Color.orange);
		gc.setFont(smallBannerFont);
		fm = gc.getFontMetrics();
		centerString(gc, fm, "Congratulations!", 160);
		gc.setColor(Color.white);
		centerString(gc, fm, "Hit any key to restart", 300);
	}

	/**
	 * Helper method for redraw to draw screen during phase of maze generation, screen is hard coded
	 * only attribute percentdone is dynamic
	 * @param gc graphics handler to manipulate screen
	 */
	private void redrawGenerating(Graphics gc) {
		gc.setColor(Color.yellow);
		gc.fillRect(0, 0, view_width, view_height);
		gc.setFont(largeBannerFont);
		FontMetrics fm = gc.getFontMetrics();
		gc.setColor(Color.red);
		centerString(gc, fm, "Building maze", 150);
		gc.setFont(smallBannerFont);
		fm = gc.getFontMetrics();
		gc.setColor(Color.black);
		centerString(gc, fm, percentdone+"% completed", 200);
		centerString(gc, fm, "Hit escape to stop", 300);
	}

	/**
	 * Allows external increase to percentage in generating mode with subsequence graphics update
	 * @param pc gives the new percentage on a range [0,100]
	 * @return true if percentage was updated, false otherwise
	 */
	public boolean increasePercentage(int pc) {
		if (percentdone < pc && pc < 100) {
			percentdone = pc;
			if (state == STATE_GENERATING)
			{
				redraw();
				repaint();
			}
			else
				dbg("Warning: Receiving update request for percentage while not in generating state, skip redraw.") ;
			return true ;
		}
		return false ;
	}
	/**
	 * Helper method for redraw to draw screen during the game. If map_mode is true, i.e. the user wants to see the overall map,
	 * the map is drawn only on a small rectangle inside the maze area. The current position is located is centered such that 
	 * it may happen that only a part of the map fits the display and is thus visible.
	 * @param gc graphics handler to manipulate screen
	 */
	private void redrawPlay(Graphics gc) {
		firstpersondrawer.redrawPlay(gc, px, py, view_dx, view_dy, walk_step, view_offset, rset, ang) ;
		
		if (map_mode) {
			mapdrawer.draw_map(gc, px, py, walk_step, view_dx, view_dy, showMaze, showSolution) ;
			mapdrawer.draw_currentlocation(gc, view_dx, view_dy) ;
		}
	}
	

	/**
	 * overwrites Applet method, called in redraw()
	 */
	public void update(Graphics g) {
		paint(g) ;
		if (solving)
			solveStep();
	} 

	/**
	 * overwrites Applet method
	 */
	public void paint(Graphics g) {
		g.drawImage(buffer_img, 0, 0, this);
	}

	/////////////////////// Methods for debugging ////////////////////////////////
	private void dbg(String str) {
		System.out.println(str);
	}

	private void logPosition() {
		if (!deepdebug)
			return;
		dbg("x="+viewx/map_unit+" ("+
				viewx+") y="+viewy/map_unit+" ("+viewy+") ang="+
				ang+" dx="+dx+" dy="+dy+" "+view_dx+" "+view_dy);
	}
	///////////////////////////////////////////////////////////////////////////////

	/**
	 * Helper method for walk()
	 * @param dir
	 * @return true if there is no wall in this direction
	 */
	private boolean checkMove(int dir) {
		// obtain appropriate index for direction (CW_BOT, CW_TOP ...) 
		// for given direction parameter
		int a = ang/90;
		if (dir == -1)
			a = (a+2) & 3; // TODO: check why this works
		// check if cell has walls in this direction
		// returns true if there are no walls in this direction
		int[] masks = Cells.getMasks() ;
		return mazecells.hasMaskedBitsFalse(px, py, masks[a]) ;
	}



	private void rotateStep() {
		ang = (ang+1800) % 360;
		view_dx = (int) (Math.cos(radify(ang))*(1<<16));
		view_dy = (int) (Math.sin(radify(ang))*(1<<16));
		moveStep();
	}

	private void moveStep() {
		redraw();
		try {
			Thread.currentThread().sleep(25);
		} catch (Exception e) { }
	}

	private void rotateFinish() {
		//dx = (int) Math.cos(radify(ang));
		//dy = (int) Math.sin(radify(ang));
		setCurrentDirection((int) Math.cos(radify(ang)), (int) Math.sin(radify(ang))) ;
		logPosition();
	}

	private void walkFinish(int dir) {
		//px += dir*dx; py += dir*dy;
		setCurrentPosition(px + dir*dx, py + dir*dy) ;
		
		if (isEndPosition(px,py)) {
			state = STATE_FINISH;
			redraw();
		}
		walk_step = 0;
		logPosition();
	}

	/**
	 * checks if the given position is outside the maze
	 * @param x
	 * @param y
	 * @return true if position is outside, false otherwise
	 */
	private boolean isEndPosition(int x, int y) {
		return x < 0 || y < 0 || x >= mazew || y >= mazeh;
	}



	synchronized private void walk(int dir) {
		if (!checkMove(dir))
			return;
		for (int step = 0; step != 4; step++) {
			walk_step += dir;
			moveStep();
		}
		walkFinish(dir);
	}

	synchronized private void rotate(int dir) {
		int origang = ang;
		int steps = 4;

		for (int step = 0; step != steps; step++) {
			ang = origang + dir*(90*(step+1))/steps;
			rotateStep();
		}
		rotateFinish();
	}

	/**
	 * Helper method for solveStep
	 * @param n
	 */
	private void rotateTo(int n) {
		int a = ang/90;
		if (n == a)
			;
		else if (n == ((a+2) & 3)) {
			rotate(1);
			rotate(1);
		} else if (n == ((a+1) & 3)) {
			rotate(1);
		} else
			rotate(-1);
	}

	/**
	 * Method is only called in update() method (which serves redraw())
	 */
	synchronized private void solveStep() {
		solving = false;
		int d = mazedists[px][py];
		gc.setColor(Color.yellow);
		// case 1: we are not directly next to the final position
		if (d > 1) {
			int n = getDirectionIndexTowardsSolution(px,py,d);
			if (n == 4)
				dbg("HELP!");
			rotateTo(n);
			walk(1);
			repaint(25);
			solving = true;
			return;
		}
		// case 2: we are one step close to the final position
		int n;
		int[] masks = Cells.getMasks() ;
		for (n = 0; n < 4; n++) {
			// skip this direction if there is a wall or border
			if (mazecells.hasMaskedBitsGTZero(px, py, masks[n]))
				continue;
			// stop if position in this direction is end position
			if (isEndPosition(px+MazeBuilder.dirsx[n], py+MazeBuilder.dirsy[n]))
				break ;
		}
		rotateTo(n);
		walk(1);
	}


	private int getDirectionIndexTowardsSolution(int x, int y, int d) {
		int[] masks = Cells.getMasks() ;
		for (int n = 0; n < 4; n++) {
			if (mazecells.hasMaskedBitsTrue(x,y,masks[n]))
				continue;
				int dx = MazeBuilder.dirsx[n];
				int dy = MazeBuilder.dirsy[n];
				int dn = mazedists[x+dx][y+dy];
				if (dn < d)
					return n ;
		}
		return 4 ;
	}

	/**
	 * Method incorporates all reactions to keyboard input in original code, 
	 * after refactoring, Java Applet and Java Application wrapper call this method to communicate input.
	 */
	public boolean keyDown(Event evt, int key) {
		switch (state) {
		// if screen shows title page, keys describe level of expertise
		// create a maze according to the user's selected level
		case STATE_TITLE:
			if (key >= '0' && key <= '9') {
				build(key - '0');
				break;
			}
			if (key >= 'a' && key <= 'f') {
				build(key - 'a' + 10);
				break;
			}
			break;
		// if we are currently generating a maze, recognize interrupt signal (ESCAPE key)
		// to stop generation of current maze
		case STATE_GENERATING:
			if (key == ESCAPE) {
				mazebuilder.Interrupt();
				buildInterrupted();
			}
			break;
		// if user explores maze, 
		// react to input for directions and interrupt signal (ESCAPE key)	
		// react to input for displaying a map of the current path or of the overall maze (on/off toggle switch)
		// react to input to display solution (on/off toggle switch)
		// react to input to increase/reduce map scale
		case STATE_PLAY:
			switch (key) {
			case Event.UP: case 'k': case '8':
				walk(1);
				break;
			case Event.LEFT: case 'h': case '4':
				rotate(1);
				break;
			case Event.RIGHT: case 'l': case '6':
				rotate(-1);
				break;
			case Event.DOWN: case 'j': case '2':
				walk(-1);
				break;
			case ESCAPE: case 65385:
				if (solving)
					solving = false;
				else
					state = STATE_TITLE;
				redraw();
				break;
			case ('w' & 0x1f): 
			{ 
				setCurrentPosition(px + dx, py + dy) ;
				redraw(); 
				break;
			}
			case '\t': case 'm':
				map_mode = !map_mode; redraw(); break;
			case 'z':
				showMaze = !showMaze; redraw(); break;
			case 's':
				showSolution = !showSolution; redraw(); break;
			case ('s' & 0x1f):
				if (solving)
					solving = false;
				else {
					solving = true;
					repaint(25);
				}
			break;
			case '+': case '=':
			{
				if (mapdrawer != null)
				{
					mapdrawer.incrementMapScale() ;
					redraw() ;
				}
				// else ignore
				break ;
			}
			case '-':
				if (mapdrawer != null)
				{
					mapdrawer.decrementMapScale() ;
					redraw() ;
				}
				// else ignore
				break ;
			}
			break;
		// if we are finished, return to initial state with title screen	
		case STATE_FINISH:
			state = STATE_TITLE;
			redraw();
			break;
		} 
		return true;
	}


	public void init() {
		state = STATE_TITLE;
		buffer_img = createImage(view_width, view_height);
		if (null == buffer_img)
		{
			System.out.println("Error: creation of buffered image failed, presumedly container not displayable");
		}
		rset = new RangeSet();
		largeBannerFont = new Font("TimesRoman", Font.BOLD, 48);
		smallBannerFont = new Font("TimesRoman", Font.BOLD, 16);
		// force MazeBuilder to load; if we load it later, it takes
		// FOREVER, using Netscape 2.0b5 on Windows NT
		MazeBuilder mbjunk = new MazeBuilder();
		FloatPair fpjunk = new FloatPair(0.0, 0.0);
		RangePair rpjunk = new RangePair(0, 0, 0, 0);
		Seg sgjunk = new Seg(0, 0, 0, 0, 0, 0);
		RangeSetElement rsejunk = new RangeSetElement(0, 0);
	}

	public void start() {
		redraw();
	}

	/**
	 * Method obtains a new Mazebuilder and has it compute new maze, 
	 * it is only used in keyDown()
	 * @param skill level determines the width, height and number of rooms for the new maze
	 */
	private void build(int skill) {
		state = STATE_GENERATING;
		percentdone = 0;
		redraw();
		mazebuilder = new MazeBuilder();
		mazew = skill_x[skill];
		mazeh = skill_y[skill];
		int roomcount = skill_rooms[skill];
		mazebuilder.build(this, mazew, mazeh, roomcount, skill_partct[skill]);
		// mazebuilder calls back by calling newMaze() to return newly generated maze
	}
	

}
