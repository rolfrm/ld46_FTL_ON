(display "load pre.lisp?")
(load "pre.lisp")


(define show-side-view #f)
(if show-side-view
    (begin
     (set-camera -12 0 8.5 0 0.8 0)
     (perspective 1.0 1.0 0.1 10000))
    (begin
     (set-camera 0 0 0 0 0 0)
     (orthographic -25.0 25.0 -25.0 25.0 -50 50)
     ))
(define key-left 263);65) ;; a
(define key-right 262);68) ;; d
(define key-up 265) ;;87 s
(define key-down 264) ;; 83d
(define key-e 69)
(define key-space 32)
(set-bg-color 0.05 0.05 0.05 1)
(define speed 0.1)
(display (eq? 1 1))
(display "???\n")
(define update
  (let ((time 0.0)
	(x 0) (y 0))
      (lambda ()
	(let ((events (pop-events)))
	  (for-each (lambda (x)
		      (when (pair? x)
			(display x)
			(display "\n")
			(when (= (cdr x) key-space) 
			  ;(display (get-model-tag sprite))
			  (display " <<< SPACE\n"))))
		    events)
	  )
	(set! time (+ time 0.01))
	;(display time)
  	(when (key-is-down key-left)
	  (set! x (- x speed)))
	(when (key-is-down key-right)
	  (set! x (+ x speed)))
	(when (key-is-down key-up)
	  (set! y (+ y speed)))
	(when (key-is-down key-down)
	  (set! y (- y speed)))
	
	(config-model sprite
		      (offset x y 0))
	
	  
	)))

(define unit-quad
    (model
     (poly -1 -1 0
	   1 -1 0
	   -1 1 0
	   1 1 0)
     ))

(define (rect width height)
    (model
     unit-quad
     (scale width height 1)))
	    
(define m1
  (model
   (model
    (poly
     0 0 0
     1 0 0
     0 1 0)
    (color 1 1 1 1))
   (model
    (poly
     0 0 0
     -1 0 0
     0 -1 0)
    (color 1 1 1 1))
   ))

(define sprite
  (model
   m1))

(define port
  (model
   (poly
     0 0 0
     1 0 0
     0 1 0
     1 1 0)
   (scale 2 2)))

(define blue-port
  (model
   port
   (color 0 0 1 1)))


(define red-port
  (model
   port
   (color 1 0 0 1)))

(define portal1
  (model
   (poly
    0 0 0
    0.5 1 0
    0.5 -1 0
    1.5 0.8 0
    2 -0.8 0
    2 -0.2 0
    )
   (color .5 .9 .5 1)
   (scale 5 5)))
    

(define level1
  (model
   sprite
   (model
    portal1
    (offset 2 8 0))
   (model
    blue-port
    (offset -15 0)
    (tag '(portal a))
    )
   (model
    red-port
    (offset -15 4))
   (model
    (model port
	   (color 0 1 0 1))
    (offset -15 8))
   (model
    (poly
     0 0 0
     1 0 0
     0 1 0
     1 1 0)
    (scale 50 50 1)
    (offset -25 -25 0)
    (color 0.2 0.2 0.2 1.0))
   ))
	    
(show-model level1)
