#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <iron/full.h>
#include <iron/gl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>


#include "scheme.h"
#include <ctype.h>
#include "main.h"
//#include "lisp.c"

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



u32 current_color = 0xFFFFFFFF;

EMSCRIPTEN_KEEPALIVE
void setColor(const char * color){
  if(color[0] !='#')
    ERROR("This is not a valid color %s\n", color);
  
  sscanf(color + 1, "%x", &current_color);
  if(strlen(color + 1) == 6){
    current_color = (current_color) | (0xFF << 24); 
  }
  printf("Setting color to: %s=%i\n", color, current_color);
}


EM_JS(const char*, encodeAsString2, (const char * data, int length), {

  // 'jsString.length' would return the length of the string as UTF-16
  // units, but Emscripten C strings operate as UTF-8.

  var lengthBytes = length;
  var stringOnWasmHeap = _malloc(lengthBytes);
  stringToUTF8(data, stringOnWasmHeap, lengthBytes);
  return stringOnWasmHeap;
});

void decodeAsString(void * data, int length);
char * vfmtstr(const char * fmt, va_list args);
bool started = false;

/*
EMSCRIPTEN_KEEPALIVE
const char * encodeAsString(size_t * s){
  infinipaint_save save;
  infinipaint_save_data(current_context, &save);
  var r = encode_as_string(&save);
  decodeAsString(r);
  return r;
  }*/

EMSCRIPTEN_KEEPALIVE
void decodeAsString(void * data, int length){
  //infinipaint_save save = {0};
  //decode_from_data(&save, data, length);
  //infinipaint_load_data(current_context, &save);  
}

EMSCRIPTEN_KEEPALIVE
void decodeAsString2(int data, int length){
  //decodeAsString((void *) data, length);

}

EMSCRIPTEN_KEEPALIVE
void infiniPaintZoom(float amount){
  //infinipaint_zoom(current_context, amount);
}


EMSCRIPTEN_KEEPALIVE
void infiniPaintNoEvents(int ok){
  //infinipaint_block_events(current_context, ok);
}


EMSCRIPTEN_KEEPALIVE
void brushConf(int size, int resolution){
  //infinipaint_configure_brush(current_context, size, resolution);
}
EMSCRIPTEN_KEEPALIVE
void setTouchPaintEnable(int toggle){
  //infinipaint_set_touch_paint_enable(current_context, toggle);
}



int convert_touch_id(int x){
  static int touch_ids[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1} ;
  int lowest = 0;
  for(u32 i = 0; i < array_count(touch_ids); i++){
    if(touch_ids[i] < touch_ids[lowest]){
      lowest = i;
    }
    if(touch_ids[i] == x){
      return i;
    }else if(touch_ids[i] == -1){
      touch_ids[i] = x;
      return i;
    }
  }
  touch_ids[lowest] = x;
  return lowest;
}

int touchnr = 0;
EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
  return 0;
}

EM_JS(int, canvas_get_width, (), {
  return canvas.getBoundingClientRect().width;
});

EM_JS(int, canvas_get_height, (), {
  return canvas.getBoundingClientRect().height;
});

EM_JS(void, js_set_color, (const char * x), {
    brushcolor.value = x;
  });

EM_JS(void, infinipaint_load_from_url, (), {
    Module.loadInitFile();
  });


int prev_width = 0, prev_height = 0;
gl_window * win;
bool firstTime = true;
double last_meas = 0.0;
void do_mainloop(context * ctx){
  if(last_meas < 0.01)
    last_meas = emscripten_date_now();
  int new_width = canvas_get_width();
  int new_height = canvas_get_height();
  if(new_width != prev_width || new_height != prev_height){
    //infinipaint_req_size(ctx, new_width, new_height);
    prev_width = new_width;
    prev_height = new_height;
  }
  if(firstTime){
    firstTime = false;

  }else{
    //return;
  }
  //current_context = ctx;
  var now = emscripten_date_now();
  render_update(ctx, MAX(1.0 / 60.0, (last_meas - now) * 1000.0));
  
  
}

int logd_enable = 1;

int main(){
  int fps = 0;
  int simulate_infinite_loop = 1;

  win = gl_window_open(800,800);
  
  //emscripten_set_touchstart_callback(0, 0, 0, touch_callback);
  //emscripten_set_touchend_callback(0, 0, 0, touch_callback);
  //emscripten_set_touchmove_callback(0, 0, 0, touch_callback);
  //emscripten_set_touchcancel_callback(0, 0, 0, touch_callback);
  var sc = scheme_init_new();
  scheme_set_output_port_file(sc, stdout);

  scheme_load_string(sc, "(display (cons \"Hello world!\" (+ 1 2)))");
  printf("\n", sc);
  var ctx = context_init(win);

  context_load_lisp(ctx, "init.scm");
  context_load_lisp(ctx, "init.lisp");
  context_load_lisp(ctx, "ld48.lisp");
  //context_load_lisp_string(ctx, game2_lisp, game2_lisp_len);
  printf("Done loading MODEL\n");
  emscripten_set_main_loop_arg((void *) &do_mainloop, ctx, fps, simulate_infinite_loop);
  return 0;
}

void _error(const char * file, int line, const char * message, ...){


  va_list args;
  char * sbuf;
  va_start (args, message);
  vasprintf(&sbuf, message, args);
  va_end (args);
  printf("%s", sbuf);
  emscripten_log(EM_LOG_ERROR, "FAILED AT: %s :%i %s", file, line ,sbuf);
  
  emscripten_force_exit(1);
  
}
