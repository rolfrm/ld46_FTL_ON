(load "pre.lisp")
(define show-side-view #f)
(if show-side-view
    (begin
     (set-camera -13 0 8.5 0 0.8 0)
     (perspective 1.0 1.0 0.1 10000))
    (begin
     (set-camera 0 0 0 0 0 0)
     (orthographic -5.0 5.0 -5.0 5.0 -50 50)
     ))

(set-bg-color 0.05 0.05 0.05 1)
(define update
    (let ((time 0.0))
      (lambda ()


	
	(set! time (+ time 0.01))
	(let ((rotate-amount (+ 1 (* 0.5 (sin (* 5 time))))))
	  ;; todo:
	  ;;(config-model-part m1 'left-arm
	  ;;		     (rotate 0 0 rotate-amount))
	
	  ;;(config-model left-arm
	;;		(rotate 0 0 (+ 1 (* 0.5 (sin (* 5 time)))))))

	))))

(define (face-model)
    (model
     (offset 0.5 1 0)
     (scale 0.5 0.5 1)

     (model
      (poly
       0 0 0
	   1 0 0
	   0 1 0
	   1 1 0)

      (color 0.25 0.25 0.25 1)
      (offset -2.5 -2.5 -1)
      (scale 4 4 1)
      )

     (model ; sub model
      (poly 0 0 0
	    1 0 0
	    0.5 1 0)
      (color 1.0 1.0 1.0 1))
     (model
            (poly 0 0 0
	    1 0 0
	    0.5 1 0)

	       (color 1.0 1.0 1.0 1)
	       (offset -2 0 0)
	       )
     (model
      (poly 0 0 0
	    1 0 0
	    0.5 -1 0)
      (color 1.0 1.0 1.0 1)
      (scale 2 1 1)
      (offset -1.5 -0.9 0)
      )
     )
  )

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
(define arm-color '(color 0.2 0.2 0.2 1))  
(define mountain-color '(color 0.4 0.4 0.4 1))
(define sky-color '(color 0.8 0.8 1.0 1))
(define sea-color '(color 0.4 0.4 0.7 1))
(define sun-color '(color 1 1 0.9 1))
(define beach-color '(color 0.8 0.8 0.4 1))
(define grass-color '(color 0.2 0.5 0.2 1))
(define left-arm ())
(define m1 (model

	    (model
	     (poly -2 0 0
		   0 1 0
		   1 0 0

		   1 0.9 0
		   2 0 0
		   2 2 0
		   3 0 0
		   3 1.5 0
		   4 0 0
		   4 1.2 0
		   5 0 0
		   5 0.8 0
		   6 0 0
		   )
	     (scale 1 0.5 1)
	     (offset 0 1.0 -7)
	     (color 0.5 0.5 0.5 1.0))

	    (model
	     (text "Hello Beautiful!!")
	     ;unit-quad
	     (offset -0 4.5 2)
	     (color 0.2 0 0 1)
	     (scale 0.4 0.4 1)
	     )
	    (model
	     (rect 5 1)
	     (offset 0 0.2 -6)
	     mountain-color)
	    
	    (model
	     (rect 5 2)
	     (offset 0 3 -10)
	     sky-color)
	    
	    (model
	     (rect 0.5 0.5)
	     (offset -3 3.5 -8)
	     (rotate 0 0 0.77)
	     sun-color)	    
	    (model
	     (rect 0.5 0.5)
	     (offset -3 3.5 -8)
	     (rotate 0 0 0.0)
	     sun-color)	    


	    (model
	     (rect 5 2)
	     (offset 0 -4.0 -4)
	     grass-color)
	    
	    (model
	     (rect 1.5 1)
	     (offset -3 -2.0 -4)
	     (rotate 0 0 0.5)
	     grass-color)
	    (model
	     (rect 1 1)
	     (offset -3.5 -1.5 -4)
	     (rotate 0 0 0.4)
	     grass-color)
	    
	    (model
	     (rect 0.75 0.75)
	     (offset -1.5 -1.5 -4)
	     (rotate 0 0 0.2)
	     grass-color)
	    (model
	     (model (rect 1 1))
	     (offset -1.3 -1.5 -3)
	     (scale 0.15 0.15 1)
	     (rotate 0 0 0.1)
	     sun-color)

	    (model
	     (model unit-quad)
	     (offset -3.5 -1.2 -3)
	     (scale 0.135 0.125 1)
	     (rotate 0 0 1.0)
	     sun-color)


	    (model
	     (model unit-quad)
	     (offset -2.5 -1.0 -3)
	     (scale 0.125 0.135 1)
	     (rotate 0 0 2.3)
	     sun-color)


	    (model
	     (model unit-quad)
	     (offset -2.25 -2.0 -3)
	     (scale 0.105 0.105 1)
	     (rotate 0 0 2.9)
	     sun-color)
	    
	    (model
	     (rect 5 1)
	     (offset 0 -1.0 -5)
	     beach-color)
	    
	    (model
	     (rect 5 1)
	     (offset 0 0.1 -4)
	     sea-color)
	    (model
	     ;; the dude
	     
	     (model
	      (offset -0.15 0.6 1)
	      (scale 0.5 0.5 1)
	      (face-model))
	     (model
	      unit-quad
	      (scale 0.25 0.5 1)
	      (color 0.5 0.5 0.5 1)
	      )
	     (set! left-arm (model
	      (name 'left-arm)
	      (model
	       unit-quad
	       (offset 1 -1))
	      (scale 0.125 0.25 1)
	      (offset 0.25 0.3 0)
	      (rotate 0 0 0.1)
	      arm-color
	     ))
	     (model
	      (name 'right-arm)
	     (model
	      (model
	       unit-quad
	       (offset -1 -1)
	       arm-color)
	      (scale 0.125 0.25 1)
	      )
	     (offset -0.25 0.3 0)
	     (rotate 0 0 -0.25)
	     )

	    (model
	     (name 'right-leg)
	     (model
	      (model
	       unit-quad
	       (offset 0 -1)
	       arm-color)
	      (scale 0.125 0.25 1)
	      )
	     (offset -0.2 -0.5 0)
	     (rotate 0 0 -0.1)
	     )

	    (model
	     (name 'left-leg)
	     (model
	      (model
	       unit-quad
	       (offset -0.0 -1)
	       arm-color)
	      (scale 0.125 0.25 1)
	      )
	     (offset 0.2 -0.5 0)
	     (rotate 0 0 0.2)
	     )

	    
	    )))
	    

	    
(show-model m1)



"
<document>

  <g name='rects' opacity='0.8' transform='translate(-15, 0)'>
     <rect x='5' y='5' width='10' height='10' fill='red'/> 
     <rect x='15' y='5' width='10' height='10' fill='green'/>
  </g>
  
</document>
"


(display '(render-svg
	   (rect :x 1 :y 1 :width 100 :height 100 :color 'blue)))

'(model (poly 0 0 2 0 0 0 2 0)
  (color 1 0 0 1)
  (scale 1 1 1)
  (model
   (poly 0 0 0
    2 0 0
    0 2 0
    2 2 0)
   (color 1 1 0 1)
   (offset 10 0 0)))

(display '
 (model
  (poly 0 0 0 2 0 0 0 2 0)
  (color 1 0 0 1)

  (scale 1 1 1)
  (model
   (rect 0 0 2 2)
   (color 1 1 0 1)
   (offset 10 0 0)
   )
  (model
   (text "Hello world")
   (font "Dejavu Sans" :size 30 :style 'normal)
   (color 1 0 0 1)
  )))
