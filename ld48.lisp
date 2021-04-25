(load "pre.lisp")


(define show-side-view #t)
(if show-side-view
    (begin
     (set-camera -11 0 8.5 0 0.3 0)
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
(define ports ())
(define blocked-portals ())
(define (load-level level)
  (when current-level
    (unshow-model current-level))
  (show-model level)
  (set! current-level level)
  (set! portals (get-assets level 'type 'portal))
  (set! player (get-assets level 'type 'player))
  (set! ports (get-assets level 'type 'port))
  (println "Loaded level " level " portals: " portals " player: "
	   player " ports: " ports )
  )


(define update
  (let ((time 0.0)
	(x 0)
	(y -5)
	(portal #t)
	(current-port ())
	(level-to-load ())
	(carrying ())
	
	)
    (lambda ()
      (when #t
      (let ((collisions (detect-collisions player ports)))
	(set! current-port
	      (when (pair? collisions)
		(car collisions))))
      (let ((events (pop-events)))
	(when (!nil events)
	  (for-each (lambda (x)
		      (when (pair? x)
			(when (= (cdr x) key-space) 
			  (println player '-click' current-port)
			  (set! carrying (cadr current-port))
			  )))
		    events))
	)

      (when (!nil carrying)

	;(println (get-tag carrying 'name))
	(config-model (get-asset super-level 'carry)
		      (color 0 1 0 1)
		      (offset (- x 1) y 1))
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

	(let ((collisions
	       (detect-collisions player
				  (filter portals
					  (lambda (portal)
					    (not (!nil (find blocked-portals portal))))))))
	  (when (null? collisions)
	    (set! portal #f))
	  (when (and (!nil collisions) (not portal))
	    (for-each
	     (lambda (item)
	       (let* ((m1 (println (car item)))
		      (m2 (cadr item))
		      (c1 (get-tag m2 'connect))
		      (new-level (find-model-by-name levels (car c1)))
		      (the-portal (get-asset new-level (cadr c1)))
		      (portal-loc (model-offset the-portal))
		      )
		 (when (!nil new-level)

		   (set! portal #t)
		   (when (!nil carrying)
		     (block-portal the-portal)
		     (block-portal m2)
		     )
		   (load-level new-level)
		   (set! x (car portal-loc))
		   (set! y (cadr portal-loc))
		   )
		 ))
	     collisions)
	  )
	)
	))))

(define (block-portal model)
  (set! blocked-portals (cons model blocked-portals ))
  (config-model model
		(color 0.3 0.3 0.6 1.0)))

(define hidden '(hidden))

(define unit-quad
    (model
     (poly -1 -1 0
	   1 -1 0
	   -1 1 0
	   1 1 0)
     ))

(define blue (color 0 0 1 1))
(define red (color 1 0 0 1))
(define green (color 0 1 0 1))

(define quad2
  (model
   (poly 0 0 0
	 0 1 0
	 1 0 0
	 1 1 0)))

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
   (model
    hidden
    quad2
    blue
    (tag '(name blue-carry))
    (offset 1 0 0)
    )
   (model
    hidden
    quad2
    green
    (tag '(name green-carry))
    (offset 0 2 0)
    )
   (model
    quad2
    hidden
    red
    (tag '(name red-carry))
    (offset 1 1  0)
    )
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
   (scale 2 2)
   (distance-field
    (rectangle 0.5 0.5 0.5 1.0 1.0 1.0))

   ))

(define blue-port
  (model
   port
   (color 0 0 1 1)))

(define red-port
  (model
   port
   (color 1 0 0 1)))

(define hexagon
  (poly
   0 0 0
   1 1 0
   1 -1 0
   2 1 0
   2 -1 0
   3 0 0))
(define pi 3.14)
(define (ngon-calculator n)
  (let ((angle (/ (* 2.0 pi) n))
	(result ()))
    
    (for 0 n (lambda (i)
	       (let* ((i2 (ceiling (if (= (modulo i 2) 1) (/ i 2) (- (/ i 2)))))
		      (p (* angle i2)))
		 (set! result (cons (cons (+ (cos p)) (+ (sin p)) ) result)))))
    (reverse result)
    ))

(define (npoly-calculator n)
  (let ((result '(poly)))
    (for-each 
     (lambda (x) (set! result (cons 0.25  (cons (cdr x) (cons (car x) result)))))
     (ngon-calculator n))
    (reverse result)))
		  

(println (ngon-calculator 5) (npoly-calculator 5))

(define pentagon
  (npoly-calculator 5))

(define hexagon
  (npoly-calculator 6))

(define megagon
  (npoly-calculator 11))


;(shutdown pentagon)


(define portal1
  (model
   pentagon
   (distance-field
    (circle 1 0 0 1.0))
   (color .5 .9 .5 1)
   (rotate 0 0 0.5)
   (scale 1 1)))
    

(define portal2
  (model
   hexagon
   (color .5 .9 .5 1)
   (scale 6 6)
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
    (scale -2 1.5)
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
    (tag '(name . blue)
	 '(type . port))
    )
   (model
    red-port
    (offset -15 4)
    (tag
     '(name . red)
     '(type . port))
    )
   (model
    port
    (color 0.5 1 0 1)
    (tag '(type . port)
	 '(name . green))
    (offset -15 8))
   (model
    (poly
     0 0 0
     1 0 0
     0 1 0
     1 1 0)
    (scale 50 50 1)
    (offset -25 -25 -0.1)
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

(display (list (find-tag '((connect . level2)) 'connect) "---\n"))
(display (list (find-model-by-name levels 'level3) "\n"))

(display "\n")
(display (sub-models level2))
(for-each (lambda (model)
	    (display (get-model-tag model))
	    (display "\n")
	    )
	  (sub-models level2))


(println "|||" (get-assets level1 'type 'portal)) 
(println (get-assets level1 'type 'port) "<-----") 


(define super-level
  (model
   (model
    (poly
     0 0 0
     1 0 0
     0 1 0)
    (offset 0 0 1)
    (color 1 0 0 1)
    (tag '(name . carry))
    )))
(show-model super-level)


(load-level level1)
(println "----> " (find (list 1 '(1 2) 3 4 5) '(1 2)))
(println "----> " (filter '(9 8 1 2 3 4 5) (lambda (x) (> x 3))))
