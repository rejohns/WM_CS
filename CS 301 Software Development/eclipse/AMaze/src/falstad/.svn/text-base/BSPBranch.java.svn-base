package falstad;

public class BSPBranch extends BSPNode {
    
	BSPNode lbranch, rbranch;
    int x, y, dx, dy;
    
    /**
     * Constructor
     * @param px
     * @param py
     * @param pdx
     * @param pdy
     * @param l
     * @param r
     */
    BSPBranch(int px, int py, int pdx, int pdy, BSPNode l, BSPNode r) {
	lbranch = l;
	rbranch = r;
	isleaf = false;
	x = px; y = py; dx = pdx; dy = pdy;
	xl = Math.min(l.xl, r.xl);
	xu = Math.max(l.xu, r.xu);
	yl = Math.min(l.yl, r.yl);
	yu = Math.max(l.yu, r.yu);
    }  
}
