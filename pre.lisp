(define (poly . elems)
    (cons 'poly elems))
(define (color . elems)
    (cons 'color elems))
(define (scale . elems)
    (cons 'scale elems))
(define (offset . elems)
    (cons 'offset elems))
(define (rotate . elems)
    (cons 'rotate elems))
(define (name . elems)
    (cons 'name elems))
(define (size w h)
    (cons 'size (cons w (cons h ()))))
(define (ortho . elems)
    (cons 'ortho elems))
(define (uv . elems)
    (cons 'uv elems))
(define (texture elems)
    (cons 'texture elems))
(define (text str)
    (cons 'text str))
(define (alpha . elems)
    (cons 'alpha elems))

(define (load-model) (cons 'model (object-new)))
(define --empty-- (load-model))

(define (model . model-data)
    (let ((m (load-model)))
      (apply config-model m model-data)
      m))

(define (load-view) (cons 'view (view-new)))

(define (view . view-data)
    (let ((m (load-view)))
      (apply config-model m view-data)
      m))

