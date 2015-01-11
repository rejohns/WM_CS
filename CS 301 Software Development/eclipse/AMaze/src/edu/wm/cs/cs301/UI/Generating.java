package edu.wm.cs.cs301.UI;

import falstad.Maze;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class Generating extends Activity {

	static Maze maze = new Maze();
	Generating gen = this;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    setContentView(R.layout.generating);
	    
        Button back = (Button) findViewById(R.id.button3);
        back.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Title.class);
		        startActivityForResult(i, 0);
				Log.v("Generating: backButton", "Returning to title screen");		
			}
		});
        
	}
	
	@Override
	public void onStart() {
		super.onStart();
		Context context = getApplicationContext();
		CharSequence text = "";
		new Thread(new Runnable() {

			@Override
			public void run() {
				Log.v("Generating: spinning wheel", "starting generating process");
				maze.build(Title.modProgress);
				Intent myIntent = new Intent(gen, Play.class);
		        Log.v("Generating: all done", "Proceeding to play screen");
		        startActivity(myIntent);
			}
			
		}).start();
	}
}
