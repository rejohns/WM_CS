package falstad;

import android.graphics.Canvas;
import android.graphics.Color;

public class GraphicsWrapper {


Canvas gc = new Canvas();
	
	public void set(Object gc) {
		this.gc = (Canvas) gc;
	}
	
	public static int getColor(int val1, int i, int j) {
		Color col = new Color();
		return val1;
	}

	public void setColor (int x) {
		if (x == 0) {
			gc.drawColor(Color.BLACK);
		}
		else if (x == 1) {
			gc.drawColor(Color.DKGRAY);
		}
		else if (x == 2) {
			gc.drawColor(Color.WHITE);
		}
		else if (x == 3) {
			gc.drawColor(Color.GRAY);
		}
		else if (x == 4) {
			gc.drawColor(Color.YELLOW);
		}
		else if (x == 5) {
			gc.drawColor(Color.RED);
		}
	}
	
	//public void setColor(Object col) {
		//gc.drawColor((Color) col);	
	//}

	public void fillRect(int i, int j, int view_width, int k) {
		//gc.fillRect(i, j, view_width, k);	
	}

	public void fillPolygon(int[] xps, int[] yps, int i) {
		//gc.fillPolygon(xps, yps, i);	
	}

	public Object newPoint(int x1i, int x2) {
		return new Point(x1i, x2);
	}

	public void drawLine(int nx1, int ny1, int nx12, int ny2) {
		gc.drawLine(nx1, ny1, nx12, ny2, null);
		
	}

	public void fillOval(int i, int j, int cirsiz, int cirsiz2) {
		//gc.fillOval(i, j ,cirsiz, cirsiz2);
		
	}
	
}
