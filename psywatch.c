/***********************************/
/* Psychic Watch for OnHandPC      */
/* By M.Brent www.harmlesslion.com */
/***********************************/
/* Random number generator from */
/* the book Graphics Gems       */
/* Bin2Inc tool by Van Helsing  */
/********************************/

#include "iconlist.inc"
#include "numbers.inc"
#include "bytursi.inc"

/* definitions */
#define GVSET 0
#define GVXOR 1
#define GVOR  2
#define GVAND 3
#define GVPAINT 4

#define KEYFILER 0x01
#define KEYMENU  0x02
#define KEYEL    0x04
#define KEYENTER 0x08
#define KEYLEFT  0x10
#define KEYUP    0x20
#define KEYRIGHT 0x40
#define KEYDOWN  0x80

unsigned char *pDat;

/* Random function adapted from Graphics Gems */
static unsigned int nLastRand=1234;
#define MASK 0x6000; /* 15 bit */

void seedrand(unsigned int x) {
  nLastRand=x;
}

unsigned int myrand() {
  if (nLastRand&1) {
    nLastRand>>=1;
    nLastRand^=MASK;
  } else {
    nLastRand>>=1;
  }
  return(nLastRand);
}

/* wait for enter to be pressed and released */
void get_enter() {
  int key;

  key=0;
  while (!(key&KEYENTER)) {
    key=bi_getbtn();
  }

  while (key&KEYENTER) {
    key=bi_scanbtn();
  }
}

/* cls() doesn't seem to work reliably */
void clr() {
  gv_clear(0,0,319,239);
}

void drawicon(int x, int y, int s, unsigned char *pDat) {
  /* draw the icon big */
  int t1;

  pDat+=4;
 
  for (t1=0; t1<4; t1++) {
    if ((*pDat)&0x8) gv_square(x, y+s+s+s, x+s-1, y+s+s+s+s-1, GVPAINT, 0xffffffff);
    if ((*pDat)&0x4) gv_square(x, y+s+s  , x+s-1, y+s+s+s-1, GVPAINT, 0xffffffff);
    if ((*pDat)&0x2) gv_square(x, y+s    , x+s-1, y+s+s-1, GVPAINT, 0xffffffff);
    if ((*pDat)&0x1) gv_square(x, y      , x+s-1, y+s-1, GVPAINT, 0xffffffff);
    x+=s;
    pDat++;
  }
}

int main() {
  int t1,t2,t3,t4;
  int magic;

  clr();

  dos_gettime(&t1, &t2, &t3, &t4);  
  seedrand((t2<<8)|(t3)+t1+t4);

  while (bi_scanbtn()&KEYENTER);

  gv_kput(0, 0,  "* Psychic Watch *", 0, 0, GVSET);
  gv_kput(0, 12, "Think of a number", 0, 0, GVSET);
  gv_kput(0, 24, "10-99. Add the 2" , 0, 0, GVSET);
  gv_kput(0, 36, "digits then sub-" , 0, 0, GVSET);
  gv_kput(0, 48, "tract the result.", 0, 0, GVSET);
  get_enter();  

  clr();
  gv_kput(0, 0,  "For example, if", 0, 0, GVSET);
  gv_kput(0, 12, "you choose '23':"  , 0, 0, GVSET);
  gv_kput(0, 24, "2+3=5. 23-5=18" , 0, 0, GVSET);
  gv_kput(0, 36, "Remember this new", 0, 0, GVSET);
  gv_kput(0, 48, "result.", 0, 0, GVSET);
  get_enter();

  clr();
  gv_kput(0, 0,  "On the next page,", 0, 0, GVSET);
  gv_kput(0, 12, "find the result"  , 0, 0, GVSET);
  gv_kput(0, 24, "and memorize the" , 0, 0, GVSET);
  gv_kput(0, 36, "symbol.Then press", 0, 0, GVSET);
  gv_kput(0, 48, "enter&be amazed!", 0, 0, GVSET);
  get_enter();

loop:
  clr();
  gv_kput(0, 0, "Please wait...", 0, 0, GVSET);

  lcdfreeze(1);
  clr();
  gv_put(0, 0, NUMBERS, GVSET);
  magic=myrand()%16;
  t4=1;
  for (t1=20; t1<300; t1+=44) {
    for (t2=0; t2<190; t2+=16) {
      do {
        t3=myrand()%16;
      } while (t3==magic);
      if (t4%9 == 0) t3=magic;
      drawicon(t1, t2, 3, &ICON[t3][0]);
      t4++;
    }
  }
  lcdfreeze(0);
  
  /* permit scrolling around - no battery save on this screen */
  t1=0;
  t2=0;
  t3=0;

  while (!(t3&KEYENTER)) {
    t3=bi_scanbtn();
    if ((t3&KEYUP)&&(t2>1)) t2-=2;
    if ((t3&KEYDOWN)&&(t2<140)) t2+=2;
    if ((t3&KEYLEFT)&&(t1>2)) t1-=2;
    if ((t3&KEYRIGHT)&&(t1<216)) t1+=2;
    gv_place(t1,t2);
  }
  while (t3&KEYENTER) {
    t3=bi_scanbtn();
  }

  /* clear the buffer */
  t1=bi_clrbtn();
  
  clr();

  t3=19;
  t4=0;

  /* hide the screen */
  gv_place(102,0);

  /* draw icon */
  drawicon(t3, t4, 16, &ICON[magic][0]);

  /* fade it in */
  for (t1=0; t1<60; t1++) {
    for (t2=0; t2<60; t2++) {
      if (t2<=t1) {
        gv_place(0,0);
      } else {
        gv_place(102,0);
      }
    }
  }
  gv_place(0,0);
  get_enter();

  clr();
  gv_kput(0, 0,  "Press Enter to", 0, 0, GVSET);
  gv_kput(0, 12, "see it again."  , 0, 0, GVSET);
  gv_kput(0, 36, "Press Menu to", 0, 0, GVSET);
  gv_kput(0, 48, "exit to filer.", 0, 0, GVSET);
  
  t1=0;
  while ((!(t1&KEYENTER))&&(!(t1&KEYMENU))) {
    t1=bi_getbtn();
  }

  if (t1&KEYENTER) {
    while (t1&KEYENTER) {
      t1=bi_scanbtn();
    }
    goto loop;
  }

  /* say goodbye with the logo */
  clr();
  gv_place(0,0);

  gv_put(0, 1, TursLogo, GVSET);

  /* fade it out */
  for (t1=0; t1<60; t1++) {
    for (t2=0; t2<60; t2++) {
      if (t2>=t1) {
        gv_place(0,0);
      } else {
        gv_place(102,0);
      }
    }
  }
  clr();
  gv_place(0,0);

  return 0;
}  


