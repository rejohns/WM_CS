package falstad;

import edu.wm.cs.cs301.UI.Finish;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;

public class MazeViewer extends View  {

	private Paint paint;
	private Maze maze;
	
	public MazeViewer(Context context, AttributeSet attributes) {
		super(context, attributes);
		paint = new Paint();
		paint.setColor(Color.GRAY);
		paint.setTextSize(25);
		paint.setAntiAlias(true);
		// TODO Auto-generated constructor stub
	}
	
	public void setMaze(Maze newMaze) {
		maze = newMaze;
	}
	
	//@Override
	public void onDraw(Canvas can){
		super.onDraw(can);
		Paint paint2 = new Paint();
		paint2.setColor(Color.CYAN);
		can.drawText("Hello, maze player!", 5, 30, paint);
		Rect rectangle = new Rect();
		rectangle.set(5, 40, 205, 240);
		can.drawRect(rectangle, paint2);
		can.drawText("This is totally", 10, 100, paint);
		can.drawText("a maze.", 10, 140, paint);
		Paint paint3 = new Paint();
		paint3.setColor(Color.GRAY);
		can.drawText("...nothing works.", 5, 255, paint3);
	}
}
