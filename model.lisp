(define-model 'rolf-model)
(set-camera 0 0 3.5 0 0 0)

(define update
    (let ((x 4)
	  (y 0)
	  (z 6.5)
	  (ry 0.0)
	  (rx 0.0)
	  (thrust 0.0)
	  )
      (lambda ()
	(set-camera x y z rx ry 0)
	(define-model 'spaceship)
	(offset-model x y z)
	(rotate-model rx ry 0)
	(define-model 'sun)
	(rotate-model rx ry 0)

	(define-model 'planet)
	(rotate-model rx ry 0)
	(let ((dir (camera-direction))
	      (a 0.0))
	  (when (key-is-down *key-enter*)
	    
	    (set! thrust (if (key-is-down *key-shift*)
			     (+ thrust 0.001)
			     (- thrust 0.001)
			     )))
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
	)))

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

(define-sub-object 'spaceship-dasboard-tree 'dashboard 'a)
(set-sub-object-transform 'spaceship-dasboard-tree 0.9 0.2 -1.5 0.0 0.0 -0.1)

