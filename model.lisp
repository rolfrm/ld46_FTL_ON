(define-model 'rolf-model)
(set-camera 0 0 3.5 0 0 0)

(define (vec-dist a b)
    (let ((x1 (vector-ref a 0))
	  (y1 (vector-ref a 1))
	  (z1 (vector-ref a 2))
	  (x2 (vector-ref b 0))
	  (y2 (vector-ref b 1))
	  (z2 (vector-ref b 2)))
      (let ((x (- x1 x2))
	    (y (- y1 y2))
	    (z (- z1 z2)))
	  
	(sqrt (+ (* x x) (* y y) (* z z))))))

(define blueorb1-loc #(1.3 0 0))
(define blueorb2-loc #(2 0 0))
(define blueorb3-loc #(3 0 0))

(define update
    (let ((x 4)
	  (y 0)
	  (z 6.5)
	  (ry 0.0)
	  (rx 0.0)
	  (thrust 0.0)
	  (points 1)
	  (blink 0.0)
	  (fuel-cell 1.0)
	  )
      (let (
	    (add-point
	     (lambda ()
	       (set! points (+ 1 points))
	       
	       (when (= points 1)
		 (show-sub-object 'spaceship-dashboard-btn2))
	       (when (= points 2)
		 (show-sub-object 'spaceship-dashboard-btn3))
	       (when (= points 3)
		 (show-sub-object 'spaceship-dashboard-btn4))
	       
	       ))
	    
	(update-fuel-display (lambda ()
			       	       (define-model 'friend)
	       (if (= points 0)
		   (begin
		    
		    (set-color 1.0 0.5 0.5 (+ 0.5 (* 0.5 (sin blink))))
		    (show-sub-object 'spaceship-dasboard-message)
	     )
		   (begin
		    (set-color 0.9 0.9 0.4 1.0 )
		    (hide-sub-object 'spaceship-dasboard-message)
		    ))))
	    )
	(begin
				

	 (lambda ()
	   (update-fuel-display)
	(set! fuel-cell (- fuel-cell 0.001))
	(when (< fuel-cell 0)
	  (set! fuel-cell 1)
	  (set! points (- points 1)))
	(set! blink (+ blink 0.1))
	(display points)
	(display #\newline)
	(define-model 'friend)
	(if (= points 0)
	    (set-color 1.0 0.5 0.5 (+ 0.5 (* 0.5 (sin blink))))
	    (set-color 0.9 0.9 0.4 fuel-cell )
	    )
	(set-camera x y z rx ry 0)
	(define-model 'spaceship)
	(offset-model x y z)
	(rotate-model rx ry 0)
	(define-model 'sun)
	(rotate-model rx ry 0)

	(define-model 'planet)
	(rotate-model rx ry 0) 

	(define-model 'planetgrass)
	(rotate-model rx ry 0)

	(define-model 'blueorb)
	(rotate-model rx ry 0)
	(define-model 'blueorb2)
	(rotate-model rx ry 0)
	(define-model 'blueorb3)
	(rotate-model rx ry 0)
	(when (< (vec-dist (vector x y z) blueorb1-loc) 0.5)
	  (set! blueorb1-loc #(0 0 0))
	  (add-point)

	  (unshow-model 'blueorb))
	(when (< (vec-dist (vector x y z) blueorb2-loc) 0.5)
	  (set! blueorb2-loc #(0 0 0))
	  (add-point)	  
	  (unshow-model 'blueorb2))
	(when (< (vec-dist (vector x y z) blueorb3-loc) 0.5)
	  (set! blueorb3-loc #(0 0 0))
	  (add-point)
	  (unshow-model 'blueorb3))

	
	;(display )
	;(display #\newline)
	
	
	(let ((dir (camera-direction))
	      (a 0.0))
	  (when (key-is-down *key-enter*)

	    (when (> points -1)
	    (set! thrust (if (key-is-down *key-shift*)
			     (+ thrust 0.001)
			     (- thrust 0.001)
			     ))))
	  (set! thrust (* 0.99 thrust))
	  
	  (set! x (+ x (* (vector-ref dir 0) thrust)))
	  (set! y (+ y (* (vector-ref dir 1) thrust)))
	  (set! z (+ z (* (vector-ref dir 2) thrust))))
	
	
	(when (key-is-down *key-left*)
	  (set! ry (- ry 0.01)))
	(when (key-is-down *key-right*)
	  (set! ry (+ ry 0.01)))
	(when (key-is-down *key-up*)
	  (set! rx (- rx 0.01)))
	(when (key-is-down *key-down*)
	  (set! rx (+ rx 0.01)))
	)))))

(perspective 1.0 1.0 0.1 100)
;(orthographic -10.0 10.0 -10.0 10.0 -10 10)
(set-bg-color 0.1 0.1 0.1 1)

(define-model 'friend)
(set-color 0.9 0.9 0.4 1.0)
(load-poly -0.1 -0.1 0
	   0.1 -0.1 0
	   -0.1 0.1 0
	   0.1 0.1 0)
(offset-model 0.0 0.0 0.4)
(rotate-model 0 0.0 0.1)
(scale-model 0.1 0.1 0.1)
(unshow-model 'friend)

(define-model 'a)
(set-color 0.4 0.2 0.1 1.0)
(load-poly 0 0 -0
	   0.3 0 -0
	   0.1 0.6 -0
	   0.2 0.6 -0)
(offset-model -0.3 -0.3)
(scale-model 0.2 0.2)
(unshow-model 'a)

;; (define-sub-object 'friend-attach 'a 'friend)
;; (set-sub-object-transform 'friend-attach 0.5 0.5 0.0 0.0 0.0 0.0)

;; (define-sub-object 'friend-attach2 'a 'friend)
;; (set-sub-object-transform 'friend-attach2 -0.55 0.5 0.0 0.0 0.0 0.0)

;; (define-sub-object 'friend-attach3 'a 'friend)
;; (set-sub-object-transform 'friend-attach3 -0.2 0.9 0.0 0.0 0.0 0.0)

;; (define-sub-object 'friend-attach4 'a 'friend)
;; (set-sub-object-transform 'friend-attach4 0.2 0.95 0.0 0.0 0.0 0.0)

;; (define-sub-object 'friend-attach5 'a 'friend)
;; (set-sub-object-transform 'friend-attach5 0.0 1.2 0.0 0.0 0.0 0.0)


(define-model 'b)
(set-color 0.1 1.0 1.5 0.5)
(load-poly 0 0 0
	   0.5 0 0
	   0 0.5 0
	   0.5 0.5 0
	   -0.5 0.5 0
	   0.5 0.65 0
	   -0.5 0.65 0)
(offset-model 0.2 0.2)
(unshow-model 'b)


(define-model 'tree-top)
(set-color 0.2 0.4 0.1 1.0)
(load-poly 0.0 0 0
	   0.5 0 0
	   0.25 0.5 0)
(offset-model -0.0 -0.0)
(rotate-model -0.0 -0.5)
(unshow-model 'tree-top)
(define-sub-object 'tree-tree-top 'a 'tree-top)
(set-sub-object-transform 'tree-tree-top -0.05 0.5 0.0 0.0 0.0 0.0)


(define-model 'd)
(set-color 0.1 0.4 0.1 1.0)
(load-poly -0.3 0 0
	   0.3 0 0
	   0.0 0.5 0)
(offset-model -0.0 -0.0)
(display "hej")
(display #\newline)
(define-sub-object 'tree-tree-top2 'a 'd)
(set-sub-object-transform 'tree-tree-top2
			  0.15 0.1 0.0
			  0.0 0.0 0.05)

(define-sub-object 'tree-tree-top3 'a 'd)
(set-sub-object-transform 'tree-tree-top3
			  0.15 0.3 0.0;
			  0.0 0.0 -0.05)
;(unshow-model 'd)


(define-model 'star)
(set-color 1.0 1.0 1.0 1.0)
(load-poly -0.1 -0.1 0
	   0.1 -0.1 0
	   0.1 0.1 0
	   -0.1 0.1 0
	   0.2 -0.0 0
	   0 -0.1 0
	   )
(offset-model -0.8 0.8 1.8)
(show-model 'star)

(define-model 'star2)
(set-color 1.0 1.0 1.0 1.0)
(load-poly -0.1 -0.1 0
	   0.1 -0.1 0
	   0.1 0.1 0
	   -0.1 0.1 0
	   0.2 -0.0 0
	   0 -0.1 0
	   )
(offset-model 0.8 0.8 1.5)
(show-model 'star2)

(display (camera-direction))
(display (camera-right))



(define pi 3.14)
(define ( gen-circ pts)
    (let ((lst ()))
      (do ((i 0 (+ i 1)))
	  ((> i pts))
	(begin
	 (set! lst (cons (sin (/ (* i pi) pts)) lst))
	 (set! lst (cons (cos (/ (* i pi) pts)) lst))
	 (set! lst (cons 0 lst))
	 (set! lst (cons (- 0 (sin (/ (* i pi) pts))) lst))
	 (set! lst (cons (cos (/ (* i pi) pts)) lst))
	 (set! lst (cons 0 lst))
	 ))
      (reverse lst)))
(display (gen-circ 12))

(define-model 'sun)
(set-color 1 1 0.5 1)
(apply load-poly (gen-circ 12))
(show-model 'sun)

(define-model 'planet)
(set-color 0.5 0.5 0.5 1)
(apply load-poly (gen-circ 12))
(offset-model 4 0 0)
(scale-model 0.1 0.1 0.1)
(show-model 'planet)

(define-model 'planetgrass)
(set-color 0.4 0.5 0.4 1)
(apply load-poly (gen-circ 12))
(offset-model 4.3 0 0)
(scale-model 0.05 0.05 0.1)
(show-model 'planetgrass)


(define-model 'blueorb)
(set-color 0.5 0.5 1.0 1)
(apply load-poly (gen-circ 12))
(offset-model 1.3 0 0)
(scale-model 0.05 0.05 0.1)
(show-model 'blueorb)

(define-model 'blueorb2)
(set-color 0.5 0.5 1.0 1)
(apply load-poly (gen-circ 12))
(offset-model 2.0 0 0)
(scale-model 0.05 0.05 0.1)
(show-model 'blueorb2)

(define-model 'blueorb3)
(set-color 0.5 0.5 1.0 1)
(apply load-poly (gen-circ 12))
(offset-model 3.0 0 0)
(scale-model 0.05 0.05 0.1)
(show-model 'blueorb3)


(define-model 'spaceship)
(show-model 'spaceship)


(define-model 'dashboard)
(set-color 0 0 0 1)
(load-poly -3 0  0
	   -3 -1 0
	   -0.5 0  -2
	   -1.5 -1 0
	    0.5 0  -2
	    1.5 -1 0
	    3 0  0
	    3 -1 0
	    )
(offset-model 0 -0.5 0)

(define-sub-object 'spaceship-dashboard 'spaceship 'dashboard)

(define-sub-object 'spaceship-dashboard-btn 'dashboard 'friend)
(set-sub-object-transform 'spaceship-dashboard-btn -0.2 0.05 -1.5 0.0 0.0 -0.1)

(define-sub-object 'spaceship-dashboard-btn2 'dashboard 'friend)
(set-sub-object-transform 'spaceship-dashboard-btn2 0.2 0.05 -1.5 0.0 0.0 -0.1)

(define-sub-object 'spaceship-dashboard-btn3 'dashboard 'friend)
(set-sub-object-transform 'spaceship-dashboard-btn3 0.2 0.12 -1.5 0.0 0.0 -0.1)

(define-sub-object 'spaceship-dashboard-btn4 'dashboard 'friend)
(set-sub-object-transform 'spaceship-dashboard-btn4 0.2 0.085 -1.5 0.0 0.0 -0.1)

(hide-sub-object 'spaceship-dashboard-btn2)
(hide-sub-object 'spaceship-dashboard-btn3)
(hide-sub-object 'spaceship-dashboard-btn4)


(define-sub-object 'spaceship-dasboard-tree 'dashboard 'a)
(set-sub-object-transform 'spaceship-dasboard-tree 0.9 0.2 -1.5 0.0 0.0 -0.1)


(define-model 'low-message)
(scale-model 0.2 0.2 0.2)
(define-sub-object 'spaceship-dasboard-message 'dashboard 'low-message)
(set-sub-object-transform 'spaceship-dasboard-message -0.8 -1.5 -5 0.0 0.0 0)
(hide-sub-object 'spaceship-dasboard-message)


(define-model 'char-l)
(set-color 1 0 0 1)
(load-poly 0 0 0
	   0.25 0 0
	   0 -0.6 0
	   0.25 -0.4 0
	   0.5 -0.6 0
	   0.5 -0.4 0
	   )
(scale-model 1 1 1)
(define-sub-object 'spaceship-dasboard-message1 'low-message 'char-l)
;(set-sub-object-transform 'spaceship-dasboard-message1 0.0 0.0 -5 0.0 0.0 0)

(define-model 'char-o)
(set-color 1 0 0 1)
(load-poly 0 0 0
	   0.25 0 0
	   0 -0.6 0
	   0.25 -0.4 0
	   0.5 -0.6 0
	   0.5 -0.4 0
	   0.8 -0.6 0
	   0.6 0.0 0
	   0.8 0.3 0
	   0.0 0.0 0
	   0.0 0.2  0
	   )
(scale-model 0.7 0.8 1)
(offset-model 0 -0.1 0)
(define-sub-object 'message-o 'low-message 'char-o)
(set-sub-object-transform 'message-o 0.7 -0.05 0 0.0 0.0 0)

(define-model 'char-w)
(set-color 1 0 0 1)
(load-poly 0 0 0
	   0.25 0 0
	   0 -0.6 0
	   0.25 -0.4 0
	   0.5 -0.6 0
	   0.5 -0.4 0
	   0.8 -0.6 0
	   0.6 0.0 0
	   0.8 0 0
	   0.8 -0.6 0
	   0.8 -0.4 0
	   1.2 -0.6 0
	   1.2 -0.4 0
	   1.2 0.0 0
	   1.4 0.0 0
	   1.4 -0.6 0
	   )
(scale-model 0.7 0.8 1)
(offset-model 0 -0.1 0)
(define-sub-object 'message-w 'low-message 'char-w)
(set-sub-object-transform 'message-w 1.5 -0.05 0 0.0 0.0 0)


;(unshow-model 'low-message)
    

;(hide-sub-object 'spaceship-dasboard-tree)

