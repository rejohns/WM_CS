package edu.wm.cs.cs301.UI;

import falstad.LosPreferences;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Spinner;
import android.widget.ToggleButton;

public class Title extends Activity {

	static int modProgress = 0;
	public int difficulty = 0;
	public boolean showWholeMaze = false;
	public boolean showSolution = false;
	public boolean showTheWalls = false;
	public static String mazeType = "Falstad";
	public static String solverBot = "Manual";
    
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        Spinner spinner1 = (Spinner) findViewById(R.id.spinner1);
        ArrayAdapter<CharSequence> adapter1 = ArrayAdapter.createFromResource(this, R.array.maze_array, android.R.layout.simple_spinner_item);
        adapter1.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner1.setAdapter(adapter1);
        spinner1.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
				if (arg2 == 0) {
					Log.v("Title: MazeSpinner", "Selected: falstad");
					mazeType = "Falstad";
				}
				else {
					Log.v("Title: MazeSpinner", "Selected: prim");
					mazeType = "Prim";
				}
				
			}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {}
        	
        });
        
        Spinner spinner2 = (Spinner) findViewById(R.id.spinner2);
        ArrayAdapter<CharSequence> adapter2 = ArrayAdapter.createFromResource(this, R.array.solver_array, android.R.layout.simple_spinner_item);
        adapter2.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner2.setAdapter(adapter2);
        spinner2.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> arg0, View arg1,
					int arg2, long arg3) {
				if (arg2 == 0) {
					Log.v("Title: SolverSpinner", "Selected: manual");
					solverBot = "Manual";
				}
				else if (arg2 == 1) {
					Log.v("Title: SolverSpinner", "Selected: gambler");
					solverBot = "Gambler";
				}
				else if (arg2 == 2) {
					Log.v("Title: SolverSpinner", "Selected: curious gambler");
					solverBot = "Curious Gambler";
				}
				else if (arg2 == 3) {
					Log.v("Title: SolverSpinner", "Selected: wall follower");
					solverBot = "Wall Follower";
				}
				else {
					Log.v("Title: SolverSpinner", "Selected: wizard");
					solverBot = "Wizard";
				}
			}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {}
        	
        	
        });
        
        Button next = (Button) findViewById(R.id.button1);
        next.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Generating.class);
		        startActivityForResult(i, 0);
		        Log.v("Title: nextButton", "Proceeding to generator");				
			}
		});
        
        Button reset = (Button) findViewById(R.id.button2);
        reset.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Title.class);
		        startActivityForResult(i, 0);
		        Log.v("Title: resetButton", "resetting the preferences sreen");
			}
		});
        
        ToggleButton showMaze = (ToggleButton) findViewById(R.id.ShowWholeMazeButton);
        showMaze.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if (isChecked) {
			        Log.v("Title: showMazeButton", "will show the maze");
			        showWholeMaze = true;
				}
				else {
					Log.v("Title: showMazeButton", "will not show maze");
					showWholeMaze = false;
				}
			}
        	
        });
        
        ToggleButton showPath = (ToggleButton) findViewById(R.id.SolutionToggleButton);
        showPath.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if (isChecked) {
			        Log.v("Title: showMazeButton", "will show the path");
			        showSolution = true;
				}
				else {
					Log.v("Title: showMazeButton", "will not show path");
					showSolution = false;
				}
			}
        	
        });
        
        ToggleButton showWalls = (ToggleButton) findViewById(R.id.VisibleWallsToggleButton);
        showWalls.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if (isChecked) {
			        Log.v("Title: showMazeButton", "will show the walls");
			        showTheWalls = true;
				}
				else {
					Log.v("Title: showMazeButton", "will not show walls");
					showTheWalls = false;
				}
			}
        	
        });
        
        SeekBar bar = (SeekBar) findViewById(R.id.DifficultySlider);
        bar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				Log.v("Title: difficulty seek bar", "difficulty now is: " + progress);
				modProgress = progress / 10;
				difficulty = modProgress;
				
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
				
			}
        	
        });
	}
	
}