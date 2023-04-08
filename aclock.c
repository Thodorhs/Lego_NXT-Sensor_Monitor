#include "display.h"
#include "math.h"

void AclockDisplayFrame(UBYTE cx, UBYTE cy, UBYTE r) {
	UBYTE X,Y,Y2,X2;
	for( X=cx-r; X<cx+r; X++ ) {
		Y = sqrt(pow(r,2)-pow(X-cx,2)) + cy;
		Y2 = -sqrt(pow(r,2)-pow(X-cx,2)) + cy;
		DisplaySetPixel(X,Y);
		DisplaySetPixel(X,Y2);
	}
	for( Y=cy-r; Y<cy+r; Y++ ) {
		X = sqrt(pow(r,2)-pow(Y-cy,2)) + cx;
		X2 = -sqrt(pow(r,2)-pow(Y-cy,2)) + cx;
		DisplaySetPixel(X,Y);
		DisplaySetPixel(X2,Y);
	}
}

void 	  AclockDisplayHand(UBYTE cx, UBYTE cy, UBYTE r, UWORD hh, UBYTE clr) {
	ULONG _x0=(ULONG)cx;
	ULONG _y0=(ULONG)cy;
	ULONG x2,y2;
	ULONG tX,tY;
	float l;
	//change hh to rad
	float hh2 = (float)hh * ( (float)M_PI / (float) 180 );
	
	x2 = (ULONG)(r * cos(hh2)+_x0);
	y2 = (ULONG)(r * sin(hh2)+_y0);
	if(clr){
		if((x2-_x0)==0){
			if(_y0>y2)
				DisplayLineY(_x0,_y0,_y0+r,1);
			else
				DisplayLineY(_x0,_y0-(y2-_y0),_y0,1);
			return;
		}else if((y2-_y0)==0){
			if(x2<_x0)
				DisplayLineX(x2,_x0,_y0,1);
			else	
				DisplayLineX(_x0,x2,_y0,1);
			return;
		}
		l = (float)((y2-_y0)/(x2-_x0));		
			if(x2<_x0){
				for( tX = x2; tX < _x0; tX++ ) {
					tY =(ULONG)( l*(_x0-tX)+_y0);
					DisplayClrPixel(tX,tY);
				}
			}else{
				for( tX = _x0; tX < x2; tX++ ) {
					tY = l*(tX-_x0)+_y0;
					DisplayClrPixel(tX,tY);
				}
			}
	}else{
		if((x2-_x0)==0){
			if(_y0>y2)
				DisplayLineY(_x0,_y0,_y0+r,0);
			else
				DisplayLineY(_x0,_y0-(y2-_y0),_y0,0);
			return;
		}else if((y2-_y0)==0){
			if(x2<_x0)
				DisplayLineX(x2,_x0,_y0,0);
			else	
				DisplayLineX(_x0,x2,_y0,0);
			return;
		}
		
		l = (float)((y2-_y0)/(x2-_x0));
			if(x2<_x0)
				for( tX = x2; tX < _x0; tX++ ) {
					tY =(ULONG)( l*(_x0-tX)+_y0);
					DisplaySetPixel(tX,tY);
				}
			else	
				for( tX = _x0; tX < x2; tX++ ) {
					tY = l*(tX-_x0)+_y0;
					DisplaySetPixel(tX,tY);
				}
	}
	return;
}
void AclockDisplayFrameSymbol(UBYTE cx, UBYTE cy, UBYTE r, UWORD hh) {
	int x2,y2;
	//change hh to rad
	float hh2 = hh * ( (float)M_PI / (float) 180 );
	
	x2 = (r * cos(hh2)+cx);
	y2 = (r * sin(hh2)+cy);
	DisplaySetPixel(x2,y2);
	if(hh==90){
		DisplaySetPixel(x2,y2+1);
		DisplaySetPixel(x2,y2-1);
		DisplaySetPixel(x2,y2+2);
	}else if(hh==180){
		DisplaySetPixel(x2+1,y2);
		DisplaySetPixel(x2+2,y2);
		DisplaySetPixel(x2-1,y2);
	}else if(hh==270){
		DisplaySetPixel(x2,y2+1);
		DisplaySetPixel(x2,y2-2);
		DisplaySetPixel(x2,y2-1);
	}else{
		DisplaySetPixel(x2+1,y2);
		DisplaySetPixel(x2-1,y2);
		DisplaySetPixel(x2-2,y2);
	}
}
