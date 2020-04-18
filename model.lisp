(define-model 'rolf-model)
(set-camera 0 0 5.5 0 0 0)

(define update
    (let ((t 3))
      (lambda ()
	(if (> t 5.0)
	    (set! t 0))
	(set-camera 0 0 (- 5.5 t) 0 0 0)
	(define-model 'a)
	(set-color (+ (* 0.15 (sin (* 10 t))) 0.5) 0.3 0.1 1.0)
					;(offset-model (* 0.1 (sin t)) -0.3)
	
	(set! t (+ t 0.00))
	)))

(display "FTLA\n")
(perspective 1.0 1.0 0.1 100)
;(orthographic -1.0 1.0 -1.0 1.0 -1 1)
(set-bg-color 0.1 0.1 0.1 1)

(define-model 'friend)
(set-color 0.9 0.9 0.4 1.0)
(load-poly 0 0 0
	   0.1 0 0
	   0.0 0.1 0
	   0.1 0.1 0)
(offset-model 0.2 0.2)
(unshow-model 'friend)

(define-model 'a)
(set-color 0.5 0.3 0.1 1.0)
(load-poly 0 0 -0
	   0.3 0 -0
	   0.1 0.6 -0
	   0.2 0.6 -0)
(offset-model -0.2 -0.3)
(show-model 'a)
(define-sub-object 'friend-attach 'a 'friend)
(set-sub-object-transform 'friend-attach 0.5 0.5 0.0 0.0 0.0 0.0)

(define-sub-object 'friend-attach2 'a 'friend)
(set-sub-object-transform 'friend-attach2 -0.55 0.5 0.0 0.0 0.0 0.0)

(define-sub-object 'friend-attach3 'a 'friend)
(set-sub-object-transform 'friend-attach3 -0.2 0.9 0.0 0.0 0.0 0.0)

(define-sub-object 'friend-attach4 'a 'friend)
(set-sub-object-transform 'friend-attach4 0.2 0.95 0.0 0.0 0.0 0.0)

(define-sub-object 'friend-attach5 'a 'friend)
(set-sub-object-transform 'friend-attach5 0.0 1.2 0.0 0.0 0.0 0.0)


(define-model 'b)
(set-color 0.0 1.0 1.5 0.5)
(load-poly 0 0 0
	   0.5 0 0
	   0 0.5 0
	   0.5 0.5 0
	   -0.5 0.5 0
	   0.5 0.65 0
	   -0.5 0.65 0)
(offset-model 0.2 0.2)
(unshow-model 'b)


(define-model 'c)
(set-color 0.1 0.4 0.1 1.0)
(load-poly 0.0 0 0
	   0.5 0 0
	   0.25 0.5 0)
(offset-model -0.3 -0.0)
(show-model 'c)

(define-model 'd)
(set-color 0.1 0.4 0.1 1.0)
(load-poly -0.1 0 0
	   0.6 0 0
	   0.25 0.5 0)
(offset-model -0.3 -0.20)
(show-model 'd)


(define-model 'star)
(set-color 1.0 1.0 1.0 1.0)
(load-poly -0.1 -0.1 0
	   0.1 -0.1 0
	   0.1 0.1 0
	   -0.1 0.1 0
	   0.2 -0.0 0
	   0 -0.1 0
	   )
(offset-model -0.8 0.8 3.5)
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
