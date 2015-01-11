package edu.wm.cs.cs301.UI;

import falstad.BasicRobot;
import falstad.Gambler;
import falstad.MazeViewer;
import falstad.Cells;
import falstad.Maze;
import falstad.RobotDriver;
import falstad.UnsuitableRobotException;
import falstad.WallFollower;
import falstad.Wizard;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.widget.Button;

public class Play extends Activity implements OnKeyListener {
	public Maze maze;
	public Cells cells;
	Play play = this;
	private Button finish;
	
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    setContentView(R.layout.play);
	    
	    // TODO Auto-generated method stub
        Button back = (Button) findViewById(R.id.button4);
        back.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Title.class);
		        startActivityForResult(i, 0);
		        Log.v("Play: backButton", "returning to title sreen");
			}
		});
        
        MazeViewer mv = (MazeViewer) findViewById(R.id.MazeViewer);
        Log.v("Play, mazeViewer", ""+ mv);
        mv.setFocusable(true);
        mv.setFocusableInTouchMode(true);
        mv.requestFocus();
        Log.v("play, testing requestFocus()",""+mv.requestFocus());
        Log.v("play, testing if mv has foucs",""+mv.hasFocus());
        Log.v("play, testing if has foucus", ""+mv.findFocus());
        
        finish = (Button) findViewById(R.id.button10);
        finish.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Finish.class);
		        startActivityForResult(i, 0);
		        Log.v("Play: next button", "proceeding to finish sreen");
			}
		});
       
       Log.v("play, testing a sneaking suspicion", ""+finish.findFocus());
       Log.v("play, testing another suspicion",""+finish.hasFocus());
        
        maze = Generating.maze;
        cells = maze.mazecells;
        Log.v("Play: testing if the maze actually generated", cells.toString());
        Log.v("Play: testing if maze is of appropriate proportion","Width: "+ cells.height + " " + maze.mazeh);
        Log.v("Play: testing if maze is of appropriate proportion","Width: "+ cells.width + " " + maze.mazew);
        
        new Thread (new Runnable() {

			@Override
			public void run() {
		        if (Title.solverBot == "Wizard") {
		        	Log.v("Play: solving Wizard", "Hope this works!");
		        	BasicRobot r = new BasicRobot();
		        	r.setMaze(maze);
		        	RobotDriver solver = new Wizard();
		        	try {
						solver.setRobot(r);
					} catch (UnsuitableRobotException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					try {
						Log.v("Play: Wizard", "Solving now");
						solver.drive2Exit();
						//if (solver.getSolved()) {
				        Intent i = new Intent(play, Finish.class);
				        startActivityForResult(i, 0);
				        Log.v("Play: WallFollower won", "proceeding to finish sreen");
						//}
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		        }
		        else if (Title.solverBot == "Wall Follower") {
		        	Log.v("Play: solving WallFollower", "Hope this works!");
		        	BasicRobot r = new BasicRobot();
		        	r.setMaze(maze);
		        	RobotDriver solver = new WallFollower();
		        	try {
						solver.setRobot(r);
					} catch (UnsuitableRobotException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					try {
						Log.v("Play: WallFollower", "Solving now");
						solver.drive2Exit();
						//if (solver.getSolved()) {
				        Intent i = new Intent(play, Finish.class);
				        startActivityForResult(i, 0);
				        Log.v("Play: WallFollower won", "proceeding to finish sreen");
						//}
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		        }
		        else if (Title.solverBot == "Gambler") {
		        	Log.v("Play: solving Gambler", "Hope this works!");
		        	BasicRobot r = new BasicRobot();
		        	r.setMaze(maze);
		        	RobotDriver solver = new Gambler();
		        	try {
						solver.setRobot(r);
					} catch (UnsuitableRobotException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					try {
						Log.v("Play: Gambler", "Solving now");
						solver.drive2Exit();
						//if (solver.getSolved()) {
				        Intent i = new Intent(play, Finish.class);
				        startActivityForResult(i, 0);
				        Log.v("Play: Gambler won", "proceeding to finish sreen");
						//}
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		        }
		        else if (Title.solverBot == "Curious Gambler") {
		        	Log.v("Play: solving CuriousGambler", "Hope this works!");
		        	BasicRobot r = new BasicRobot();
		        	r.setMaze(maze);
		        	RobotDriver solver = new Gambler();
		        	try {
						solver.setRobot(r);
					} catch (UnsuitableRobotException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					try {
						Log.v("Play: CuriousGambler", "Solving now");
						solver.drive2Exit();
						//if (solver.getSolved()) {
				        Intent i = new Intent(play, Finish.class);
				        startActivityForResult(i, 0);
				        Log.v("Play: CuriousGambler won", "proceeding to finish sreen");
						//}
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
		        }
		        else {
		        	
		        }
			}
        	
        }).start();
	
	}


	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event) {
        Log.v("Play, onKeyListener","CALLED");
		if (event.getAction() == KeyEvent.ACTION_DOWN) {
            if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
            	Log.v("Play, onKeyListener", "CENTER");
                return true;
            }
            else if (keyCode == KeyEvent.KEYCODE_DPAD_UP) {
                	Log.v("Play, onKeyListener","UP");
                	maze.walk(1);
                	return true;
            }
            else if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
                	Log.v("Play, onKeyListener","DOWN");
                	maze.walk(-1);
                	return true;
            }
            else if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                	Log.v("Play, onKeyListener","LEFT");
                	maze.rotate(1);
                	return true;
            }
            else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                	Log.v("Play, onKeyListener","RIGHT");
                	maze.rotate(-1);
                	return true;
                }
            else {
                	Log.v("Play, onKeyListener","I got nothing.");
                }
        }
		return false;
	}
	

}
