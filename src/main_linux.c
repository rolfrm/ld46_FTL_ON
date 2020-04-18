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
void do_mainloop(context * ctx){
  gl_window_make_current(ctx->win);
  glClear(GL_COLOR_BUFFER_BIT);
  
}


int logd_enable = 1;

int main(){
  int fps = 0;
  int simulate_infinite_loop = 1;

  gl_window * win = gl_window_open(800,800);

  var ctx = (context *)alloc0(sizeof(context));//infinipaint_context_new(t, win);
  ctx->win = win;
  ctx->running = 1;
  context_init(ctx);
  context_load_lisp(ctx, "model.lisp");

  u64 lastmod = 0;
  printf("Edit? %i\n", file_modification_date("model.lisp"));
  while(ctx->running){
    var nowmod = file_modification_date("model.lisp");
    if(nowmod != lastmod){
      lastmod = nowmod;

      //iron_sleep(1);
      context_load_lisp(ctx, "model.lisp");
      printf("Edit!\n");
    }
    render_update(ctx);
    gl_window_swap(ctx->win);
  }
  return 0;
}

