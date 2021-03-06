
//test

typedef struct _context context;

bool context_running(context * ctx);

void render_update(context * ctx, float dt);
context * context_init(gl_window * win);

void context_load_lisp(context * ctx, const char * file);
void context_load_lisp_string(context * ctx, const char * string, size_t length);
