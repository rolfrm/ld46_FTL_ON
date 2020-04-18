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
#include "scheme-private.h"
#include "main.h"

vec4 bg_color;

void render_update(context * ctx){

  glClearColor(bg_color.x,bg_color.y,bg_color.z,bg_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  

}

int list_len(scheme * sc, pointer data){
  int cnt = 0;
  while(data != sc->NIL){
    cnt += 1;
    data = pair_cdr(data);
  }
  return cnt;
}

float * pointer_to_floats(scheme * sc, pointer data, size_t * cnt){
  var orig_data = data;
  int l = list_len(sc, data);
  f32 * array = alloc0(l * sizeof(data[0]));
  int offset = 0;
  while(data != sc->NIL){
    
    var val = pair_car(data);
    if(is_integer(val)){
      array[offset] = (float)ivalue(val);
    }else if(is_number(val)){
      array[offset] = (float)rvalue(val);
    }
    data = pair_cdr(data);
    offset += 1;
  }
  *cnt = l;
  return array;
}

pointer load_poly(scheme * sc, pointer data){
  size_t c = 0;
  f32 * fs = pointer_to_floats(sc, data, &c);
  for(size_t i = 0; i < c; i++){
    printf(" %f", fs[i]);
  }
  printf("\n");
  return data;
}

pointer set_color(scheme * sc, pointer data){
  size_t count;
  f32 * colors = pointer_to_floats(sc, data, &count);
  if(count == 4)
    bg_color = vec4_new(colors[0] ,colors[1], colors[2], colors[3]);
  free(colors);
  return data;
}


pointer print_result(scheme * sc, pointer data){
  printf("\nhej?? %i %i \n", pair_cdr(data) == sc->NIL, 0);
  return data;
}



void context_init(context * ctx){
   scheme_registerable reg[] = {
    {.f = print_result, .name = "print2"},
    {.f = load_poly, .name = "load-poly"},
    {.f = set_color, .name = "set-bg-color"}

  };
   var sc = scheme_init_new();
   ctx->sc = sc;
   scheme_set_output_port_file(sc, stdout);
   scheme_register_foreign_func_list(sc, reg, array_count(reg));
}


void context_load_lisp(context * ctx, const char * file){
  var fin =fopen(file, "r");
  scheme_load_named_file(ctx->sc, fin, file);
  fclose(fin); 
}
