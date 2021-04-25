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

(define (distance-field . elems)
    (cons 'distance-field elems))

(define (circle x y z r)
    (list 'circle x y z r))

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
	   (vector-set! objs id (cdr d))))
       model-data)
      m))

(define (tag . tag-data)
    (cons 'tag tag-data))

(define (load-view) (cons 'view (view-new)))

(define (view . view-data)
    (let ((m (load-view)))
      (apply config-model m view-data)
      m))

(define (!nil x)
  (not (null? x)))

(define (println . args)
    (for-each display args)
  (display "\n")
  (when (!nil args)
    (car args)))
  

(define (find-tag lst tag)
    (let loop ((lst lst))
      (when (!nil lst)
	(let ((first (car lst)))
	  (if (and (pair? first) (eq? (car first) tag))
	      (cdr first)
	      (when (pair? lst)
		(loop (cdr lst))))))))

(define (find-name list)
    (find-tag list 'name))

(define (get-asset model name)
  (let 
      iter ((items (sub-models model)))
      (when (pair? items)
	(let* ((item (car items))
	       (tags (get-model-tag item)))
	  (if (and (pair? tags) (eq? name (find-name tags)))
	      item
	      (when (pair? items)
		(iter (cdr items))))))))

(define (get-assets model tag value)
  (let 
      iter ((items (sub-models model)))
      (when (pair? items)
	(let* ((item (car items))
	       (tags (get-model-tag item)))
	  (if (and (pair? tags) (eq? value (find-tag tags tag)))
	      (cons item
		    (when (pair? items)
		      (iter (cdr items))))
	      (when (pair? items)
		(iter (cdr items)))	      
	      )))))

(define (get-tag model tag)
  (find-tag (get-model-tag model) tag))

  
(define (find-model-by-name _models name)
  (let iter ((models _models))
    (when (!nil models)
      (let* ((model (car models))
	     (tags (get-model-tag model))
	     (item-name (find-name tags)))
	(if (eq? item-name name)
	    model
	    (iter (cdr models))
	    )))))
