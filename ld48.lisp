(load "pre.lisp")


(define show-side-view #f)
(if show-side-view
    (begin
     (set-camera -20 0 8.5 0 0.8 0)
     (perspective 2.0 1.0 0.1 10000))
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
(define speed 0.5)
(display (eq? 1 1))
(display "???\n")
(define current-level ())
(define portals ())
(define levels ())
(define player ())
(define (load-level level)
  (when current-level
    (unshow-model current-level))
  (show-model level)
  (set! current-level level)
  (set! portals (get-assets level 'type 'portal))
  (set! player (get-assets level 'type 'player))
  (display "Loaded level ")
  (display level)
  (display " portals: ")
  (display portals)
  (display " player: ")
  (display player)
  (display "\n")
  )



(define update
  (let ((time 0.0)
	(x 0)
	(y -5)
	(portal #t)
	)
      (lambda ()
	(let ((events (pop-events)))
	  (for-each (lambda (x)
		      (when (pair? x)
			(when (= (cdr x) key-space) 
			  (display (get-model-tag sprite))
			  (display " <<< SPACE\n"))))
		    events)
	  )
	(set! time (+ time 0.01))
  	(when (key-is-down key-left)
	  (set! x (- x speed)))
	(when (key-is-down key-right)
	  (set! x (+ x speed)))
	(when (key-is-down key-up)
	  (set! y (+ y speed)))
	(when (key-is-down key-down)
	  (set! y (- y speed)))
	
	(config-model (car player)
		      (offset x y 0))
	(let ((collisions (detect-collisions player portals)))
	  (when (null? collisions)
	    (set! portal #f))
	  (when (and (!nil collisions) (not portal))
	    (for-each
	     (lambda (item)
	       (let* ((m1 (car item))
		      (m2 (cadr item))
		      (c1 (get-tag m2 'connect))
		      (new-level (find-model-by-name levels (car c1)))
		      (the-portal (get-asset new-level (cadr c1)))
		      (portal-loc (model-offset the-portal))
		      )
		 (println ">>>" levels c1 (model-offset the-portal) )
		 (when (!nil new-level)
		   (set! portal #t)
		   (load-level new-level)
		   (set! x (car portal-loc))
		   (set! y (cadr portal-loc))
		   )
		 ))
	     collisions)
		      ))
	;(detect-collisions (list (car portals)) (list (cadr portals)))
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
    (color 1 1 0 1))
   (model
    (poly
     0 0 0
     -1 0 0
     0 -1 0)
    (color 1 0 1 1))
   ))

(define sprite
  (model
   m1
   (tag 1)
   (distance-field (circle 0 0 0 1))
   ))

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
   (distance-field
    (circle 1 0 0 1.0))
   (color .5 .9 .5 1)
   (rotate 0 0 0.0)
   (scale 1 1)))
    

(define portal2
  (model
   (poly
    0 0 0
    0.8 1 0
    0.4 -0.9 0
    1.5 0.8 0
    2 -0.8 0
    2 -0.2 0
    )
   (color .5 .9 .5 1)
   (scale 1 1)
   (distance-field
    (circle 1 1 0 1.0))
   ))


(define level1
  (model
   (model
    sprite
    (tag '(type . player)))
   (model
    portal1
    (offset 2 8 0)
    (scale -1 1)
    (tag '(name . a)
	 '(type . portal)
	 '(connect level2 a)
	 )
    )
   (model
    portal2
    (offset -8 -8 0)
    
    (tag '(name . b)
	 '(type . portal)
	 '(connect level2 b)
	 )
    )
   (model
    blue-port
    (offset -15 0)
    (tag '(name . b))
    )
   (model
    red-port
    (offset -15 4))
   (model
    (model port
	   (color 0.5 1 0 1))
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
   (tag '(name . level1))
   ))

(define (blip x y z)
  (model
       (offset x y z)
       (poly -0.2 -0.2 0
	     .2 -0.2 0
	     -0.2 .2 0)
       (color 1 1 1 1)
       ))

(define level2
  (model
   (blip 0.0 -5.0 0.000000)
   (blip 0.0 0.0 0.000000)
   (blip 6.033523 -0.324323 0.000000)
   (blip 2.033860 -0.475112 0.000000)
   (blip 4.033687 -0.394712 0.000000)      
   (model
    sprite
    (offset -2 5 0)
    (tag '(type . player)))
   (model
    portal1
    (offset 5 -8)
    (scale 6 2)
    (tag '(name . b)
	 '(type . portal)
	 '(connect level1 a))
	 
    )
   (model
    portal2
    (offset 3 5)
    (scale 3 3)
    (tag '(name . a)
	 '(type . portal)
	 '(connect level1 b))
    )
   (model
    portal2
    (offset -15 5)
    (scale 3 5)
    (tag '(name . a)
	 '(type . portal)
	 '(connect level3 b))
    )
   
   (tag '(name . level2))
   ))

(define level3
  (model
   (tag '(name . level3))
   (model
    sprite
    (tag '(type . player)))
   (model
    portal1
    (tag '(name . a)
     '(type . portal)
     '(connect level1 a)))

   (model
    portal1
    (offset 10 0)
    (tag '(name . b)
     '(type . portal)
     '(connect level2 a)))

   ))


(set! levels (list level1 level2 level3))

(load-level level2)
(display (list (find-tag '((connect . level2)) 'connect) "---\n"))
(display (list (find-model-by-name levels 'level3) "\n"))
;(shutdown)
(display level1)
(display "\n")
(display (sub-models level2))
(for-each (lambda (model)
	    (display (get-model-tag model))
	    (display "\n")
	    )
	  (sub-models level2))


(display "||||")
(display (get-assets level2 'type 'portal))
(display "\n")
