package edu.wm.cs.cs301.UI;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class Finish extends Activity {

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    setContentView(R.layout.finish);
	    // TODO Auto-generated method stub
        Button back = (Button) findViewById(R.id.button5);
        back.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        Intent i = new Intent(v.getContext(), Title.class);
		        startActivityForResult(i, 0);
		        Log.v("Finish: backButton", "returning to title sreen");
			}
		});
	
	}

}
