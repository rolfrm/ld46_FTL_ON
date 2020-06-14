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

;(set-camera 0 0 3.5 0 0 0)
;(perspective 1.0 1.0 0.1 10000)
(orthographic -5.0 5.0 -5.0 5.0 -5 5)
(set-bg-color 0.05 0.05 0.05 1)
(define update (lambda ()))

(define m1 (model
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
	      (offset 0.5 1 0)
	      (scale 2 2 2)
	      )
)

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
