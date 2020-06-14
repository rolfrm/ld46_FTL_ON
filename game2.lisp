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

(define (load-model) (cons 'model (object-new)))
(define --empty-- (load-model))

(define (model . model-data)
    (let ((m (load-model)))
      (apply config-model m model-data)
      m))

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
(define update (lambda ()))

(define (face-model)
    (model
     (offset 0.5 1 0)
     (scale 0.5 0.5 1)

     (model
      (poly 0 0 0
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
      (color 1.0 0.3 0.0 1))
     (model
	       (poly 0 0 0
		     1 0 0
		     0 1 0
		     1 1 0)
	       (color 1.0 1.0 1.0 1)
	       (offset -2 0 0)
	       )
     (model
      (poly 0 0 0
	    1 0 0
	    0.5 -1 0)
      (color 1.0 1.0 1.0 1)
      (scale 2 1 1)
      (offset -1.5 -0.5 0)
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
(define mountain-color '(color 0.3 0.3 0.3 1))
(define sky-color '(color 0.8 0.8 1.0 1))
(define sea-color '(color 0.4 0.4 0.7 1))
(define sun-color '(color 1 1 0.9 1))
(define beach-color '(color 0.8 0.8 0.4 1))
(define grass-color '(color 0.2 0.5 0.2 1))
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
	     (offset 0 1.0 -6)
	     mountain-color)

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
	     (rect 5 1)
	     (offset 0 -1.0 -5)
	     beach-color)
	    
	    (model
	     (rect 5 1)
	     (offset 0 0.1 -4)
	     sea-color)
	    
	    (model
	     (offset -0.15 0.6 1)
	     (scale 0.5 0.5 1)
	     (face-model))
	    (model
	     unit-quad
	     (scale 0.25 0.5 1)
	     (color 0.5 0.5 0.5 1)
	     )
	    (model
	     (model
	      unit-quad
	      (offset 1 -1))
	     (scale 0.125 0.25 1)
	     (offset 0.25 0.3 0)
	     (rotate 0 0 0.1)
	     arm-color
	     )
	    (model
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
	     (model
	      (model
	       unit-quad
	       (offset -0.0 -1)
	       arm-color)
	      (scale 0.125 0.25 1)
	      )
	     (offset 0.2 -0.5 0)
	     (rotate 0 0 0.1)
	     )

	    
	    ))
	    

	    
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
