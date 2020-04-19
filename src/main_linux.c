#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iron/full.h>
#include <iron/gl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "scheme.h"
#include "main.h"

#include <ctype.h>

void * alloc0(size_t s){
  return calloc(1, s);
}

void * alloc(size_t s){
  return malloc(s);
}

void * ralloc(void * s, size_t news){
  return realloc(s, news);
}

void dealloc(void * s){
  return free(s);
}

char * vfmtstr(const char * fmt, va_list args){
  va_list args2;
  va_copy(args2, args);
  size_t size = vsnprintf (NULL, 0, fmt, args2) + 1;
  va_end(args2);
  char * out = alloc(size);
  vsprintf (out, fmt, args);
  va_end(args);
  return out;
}

char * fmtstr(const char * fmt, ...){
  va_list args;
  va_start (args, fmt);
  return vfmtstr(fmt, args);
}
void * iron_clone(const void * src, size_t s){
  void * out = alloc(s);
  memcpy(out, src, s);
  return out;
}


void log_print(log_level level, const char * fmt, ...){
  UNUSED(level);
  va_list args;
  va_start (args, fmt);
  vprintf(fmt, args);
  va_end(args);
}




int prev_width = 0, prev_height = 0;

bool firstTime = true;


int logd_enable = 1;

int main(){
  int fps = 0;
  int simulate_infinite_loop = 1;

  gl_window * win = gl_window_open(800,800);
  
  var ctx = context_init(win);
  context_load_lisp(ctx, "init.scm");
  context_load_lisp(ctx, "init.lisp");
  context_load_lisp(ctx, "model.lisp");
  u64 lastmod = 0;
  printf("Edit? %i\n", file_modification_date("model.lisp"));
  var last = 0.0; timestampf();
  while(context_running(ctx)){
    var nowmod = file_modification_date("model.lisp");
    if(nowmod != lastmod){
      lastmod = nowmod;

      //iron_sleep(1);
      context_load_lisp(ctx, "model.lisp");
      printf("Edit!\n");
    }
    var now = timestampf();
    if(last < 0.001)
      last = now;
    render_update(ctx, now - last);
    last = now;
    gl_window_swap(win);
    gl_window_poll_events();
      
  }
  return 0;
}

