package edu.wm.cs.cs301.UI;

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

public class Preferences extends Activity {

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.title);
		
		Button prefs = (Button) findViewById(R.id.prefsButton);
        prefs.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Preferences.class);
		        startActivityForResult(i, 0);
		        Log.v("Title: prefsButton", "Proceeding to preferences");				
			}
				
		});
        
        Button start = (Button) findViewById(R.id.startButton);
        start.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Generating.class);
		        startActivityForResult(i, 0);
		        Log.v("Title: startButton", "Proceeding to generator");				
			}
				
		});
		
		Spinner spinner1 = (Spinner) findViewById(R.id.spinner1);
        ArrayAdapter<CharSequence> adapter1 = ArrayAdapter.createFromResource(this, R.array.maze_array, android.R.layout.simple_spinner_item);
        adapter1.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner1.setAdapter(adapter1);
        spinner1.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
				if (arg2 == 0) {
					Log.v("Preferences: MazeSpinner", "Selected: falstad");
				}
				else {
					Log.v("Preferences: MazeSpinner", "Selected: prim");
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
					Log.v("Preferences: SolverSpinner", "Selected: manual");
				}
				else if (arg2 == 1) {
					Log.v("Preferences: SolverSpinner", "Selected: gambler");
				}
				else if (arg2 == 2) {
					Log.v("Preferences: SolverSpinner", "Selected: curious gambler");
				}
				else if (arg2 == 3) {
					Log.v("Preferences: SolverSpinner", "Selected: wall follower");
				}
				else {
					Log.v("Preferences: SolverSpinner", "Selected: wizard");
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
		        Log.v("Preferences: nextButton", "Proceeding to generator");				
			}
		});
        
        Button reset = (Button) findViewById(R.id.button2);
        reset.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Preferences.class);
		        startActivityForResult(i, 0);
		        Log.v("Preferences: resetButton", "resetting the preferences sreen");
			}
		});
        
        ToggleButton showMaze = (ToggleButton) findViewById(R.id.ShowWholeMazeButton);
        showMaze.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if (isChecked) {
			        Log.v("Preferences: showMazeButton", "will show the maze");
				}
				else {
					Log.v("Preferences: showMazeButton", "will not show maze");
				}
			}
        	
        });
        
        ToggleButton showPath = (ToggleButton) findViewById(R.id.SolutionToggleButton);
        showPath.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if (isChecked) {
			        Log.v("Preferences: showMazeButton", "will show the path");
				}
				else {
					Log.v("Preferences: showMazeButton", "will not show path");
				}
			}
        	
        });
        
        ToggleButton showWalls = (ToggleButton) findViewById(R.id.VisibleWallsToggleButton);
        showWalls.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if (isChecked) {
			        Log.v("Preferences: showMazeButton", "will show the walls");
				}
				else {
					Log.v("Preferences: showMazeButton", "will not show walls");
				}
			}
        	
        });
        
        SeekBar bar = (SeekBar) findViewById(R.id.DifficultySlider);
        bar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				Log.v("Preferences: difficulty seek bar", "difficulty now is: " + progress);
				
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
