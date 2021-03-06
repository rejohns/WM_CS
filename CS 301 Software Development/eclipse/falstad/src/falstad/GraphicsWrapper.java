package falstad;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

public class GraphicsWrapper {

	Graphics gc;
	
	public void set(Object gc) {
		this.gc = (Graphics) gc;
	}
	
	public static Object getColor(int val1, int i, int j) {
		return new Color(val1, i, j);
	}

	public void setColor (int x) {
		if (x == 0) {
			gc.setColor(Color.black);
		}
		else if (x == 1) {
			gc.setColor(Color.darkGray);
		}
		else if (x == 2) {
			gc.setColor(Color.white);
		}
		else if (x == 3) {
			gc.setColor(Color.gray);
		}
		else if (x == 4) {
			gc.setColor(Color.yellow);
		}
		else if (x == 5) {
			gc.setColor(Color.red);
		}
	}
	
	public void setColor(Object col) {
		gc.setColor((Color) col);	
	}

	public void fillRect(int i, int j, int view_width, int k) {
		gc.fillRect(i, j, view_width, k);	
	}

	public void fillPolygon(int[] xps, int[] yps, int i) {
		gc.fillPolygon(xps, yps, i);	
	}

	public Object newPoint(int x1i, int x2) {
		return new Point(x1i, x2);
	}

	public void drawLine(int nx1, int ny1, int nx12, int ny2) {
		gc.drawLine(nx1, ny1, nx12, ny2);
		
	}

	public void fillOval(int i, int j, int cirsiz, int cirsiz2) {
		gc.fillOval(i, j ,cirsiz, cirsiz2);
		
	}

}
