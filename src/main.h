
//test

typedef struct{
  gl_window * win;
  int running;
  scheme * sc;
}context;

void render_update(context * ctx);
void context_init(context * ctx);

void context_load_lisp(context * ctx, const char * file);
