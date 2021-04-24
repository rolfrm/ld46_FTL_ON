(define (for a b f)
  (unless (= a b)
    (f a)
    (for (+ 1 a) b f)))

(define (vector-grow vec size)
  (let ((out (make-vector size)))
    (for 0 (min (vector-length vec) size)
	 (lambda (i)
	   (vector-set! out i (vector-ref vec i))))
    out))


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

(define objs (make-vector 100))

(define (get-model-tag m)
    (vector-ref objs (cdr m)))

(define (model . model-data)
    (let* ((m (load-model))
	   (id (cdr m))
	   )
      (apply config-model m model-data)
      (for-each
       (lambda (d)
	 (when (and (pair? d) (eq? (car d) 'tag))
	   

	   (display (cons id (cdr d))))) model-data)
      (display "\n")
      m))

(define (tag . tag-data)
    (cons 'tag tag-data))

(define (load-view) (cons 'view (view-new)))

(define (view . view-data)
    (let ((m (load-view)))
      (apply config-model m view-data)
      m))

