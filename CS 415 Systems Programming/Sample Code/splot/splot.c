/* splot.c */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "child.h"

#define Z_DIST 400.0
#define SCALE_FACTOR 100.0
/* Factor for degrees to radians */
#define DEG2RAD 0.0174532

typedef struct _point_list {
    float x, y, z;
    int xd, yd;
    int type; /* Color */

    struct _point_list *next;
} point_list;

static char *colornames[] = {"red", "blue", "slateblue", "lightblue",
        "yellow", "orange", "gray90"
};

void matrixy(float *a, float *b, float sinr, float cosr) {
        float tma;
        tma = *a;
        *a = (tma * cosr) - (*b * sinr);
        *b = (tma * sinr) + (*b * cosr);
}

void plot_points(FILE *readfrom, FILE *write_to,
        point_list *list, char *canvas_name,
        float xr, float yr, float zr,
        float s, int half) {

	point_list *node;
	float cx, sx, cy, sy, cz, sz, mz;
        float x,y,z;

	xr *= DEG2RAD; yr *= DEG2RAD; zr *= DEG2RAD;
        s /= SCALE_FACTOR;

	cx = cos(xr); sx = sin(xr);
	cy = cos(yr); sy = sin(yr);
	cz = cos(zr); sz = sin(zr);

	for (node = list; node != NULL; node = node->next) {
	  /* Simple 3D transform with perspective */
          x = (node->x * s); y = (node->y * s);
          z = (node->z * s);
          matrixy(&x,&y,sz,cz); matrixy(&x,&z,sy,cy); matrixy(&y,&z,sx,cx);
          mz = Z_DIST - z; if (mz < 3.4e-3) mz = 3.4e-3;
          x /= (mz * (1.0/Z_DIST));
          y /= (mz * (1.0/Z_DIST));
          node->xd = x+half; node->yd = y+half;
        }

	/* Erase points */
	fprintf(write_to,"%s delete dots\n",canvas_name);

	for (node = list; node != NULL; node = node->next) {
    /* Send canvas command to wish... create
     * an oval on the canvas for each point. */
    fprintf(write_to,
        "%s create oval %d %d %d %d " \
        "-fill %s -outline %s -tags dots\n",
        canvas_name,(node->xd)-3,(node->yd)-3,
        (node->xd)+3,(node->yd)+3,
        colornames[node->type],
        colornames[node->type]);
	}
}

/* Create dataset list given filename to read */
point_list *load_points(char *fname) {

  FILE *fp;
  point_list *thelist = NULL, *node;

  assert (fp = fopen(fname,"r"));
  while (!feof(fp)) {
     assert (node = (point_list *)malloc(sizeof(point_list)));
     if (fscanf (fp, "%f %f %f %d",
             &(node->x),&(node->y),&(node->z),
		 &(node->type)) == 4) {
       node->next = thelist;
       thelist = node;
     }
  }
  fclose(fp);
  return thelist;
}

int main(int argc,char **argv) {
    FILE *read_from, *write_to;
    char result[80], canvas_name[5];
    float xr,yr,zr,s;
    int childpid, half;
    point_list *thelist;
    
    assert(argc == 2);
    thelist = load_points(argv[1]);
    childpid = start_child("wish",&read_from,&write_to);


  /* Tell wish to read the init script */
  fprintf (write_to, "source splot.tcl\n");

  while(1) {
    /* Blocks on read from wish */
    if (fgets (result, 80, read_from) <= 0) exit(0); /* Exit if wish dies */

    /* Scan the string from wish */
    if ((sscanf (result, "p %s %f %f %f %f %d", canvas_name,&xr,&yr,&zr, &s,&half)) == 6)
      plot_points(read_from,write_to,thelist, canvas_name,xr,yr,zr,s,half);
    else
      fprintf(stderr,"Bad command: %s\n",result);
  }
  return(0);
}