;;; If all three circles are external to eachother, radius of new circle
;;; is r = r1 r2 r3 / (r1 r2 + r1 r3 + r2 r3 + 2 sqrt(r1 r2 r3 (r1 + r2 + r3)))
;;; easy enough to find location using radius and two circles

;;; For point, looked at intersection of circles, radii (r1 + r), (r2 + r)

(define (sqr x)
  (* x x))

(define circle-fractal
  (let ((one .2)
	(two .4)
	(three .6)
	(four .8))
    (define (full-circle x y radius n)
      ;; Do circle to either side, recurse to top and bottom
      (lambda (t)
	(let ((pt (cond ((> n 10)
			 (let ((s (* 2 pi t)))
			   (cons (cos s) (sin s))))
			((< t one)
			 (let ((s (* 5 t)))
			   ((full-circle .5 0 .5 (+ n 1)) s)))
			((< t two)
			 (let ((s (* 5 (- t one))))
			   ((full-circle -.5 0 .5 (+ n 1)) s)))
			((< t three)
			 (let ((s (* 5 (- t two))))
			   ((inside-circle (list .5 0 .5) (list -.5 0 .5)
					   (list 0 0 1) (+ n 1)) s)))
			((< t four)
			 (let ((s (* 5 (- t three))))
			   ((inside-circle (list -.5 0 .5) (list .5 0 .5)
					   (list 0 0 1) (+ n 1)) s)))
			(else
			 (let ((s (* 5 (- t four))))
			   (cons (cos (* 2 pi s)) (sin (* 2 pi s))))))))
	  (cons (+ (* (car pt) radius) x)
		(+ (* (cdr pt) radius) y)))))
    (define (inside-circle out1 out2 in3 n)
      (let* ((r1 (get-r out1))
	     (r2 (get-r out2))
	     (r3 (get-r in3))
	     (e1 (/ r1))
	     (e2 (/ r2))
	     (e3 (- (/ r3)))
	     (radius
	      (/ (+ e1 e2 e3
		    (* +2 (sqrt (+ (* e1 e2) (* e1 e3) (* e2 e3)))))))
	     (location1
	      (circle-point out1 out2 radius))
	     (sqrdist1
	      (+ (sqr (- (car location1) (get-x in3)))
		 (sqr (- (cdr location1) (get-y in3)))))
	     (location2
	      (circle-point out2 out1 radius))
	     (sqrdist2
	      (+ (sqr (- (car location2) (get-x in3)))
		 (sqr (- (cdr location2) (get-y in3)))))
	     (location
	      (if (> sqrdist1 sqrdist2) location1 location2))
	     (new-circle
	      (list (car location) (cdr location) radius)))
	(lambda (t)
	  (cond ((> n 10)
		 (let ((s (* 2 pi t)))
		   (cons (+ (* (cos s) radius) (car location))
			 (+ (* (sin s) radius) (cdr location)))))
		((< t one)
		 (let ((s (* 5 t)))
		   ((full-circle (car location) (cdr location) radius (+ n 1)) s)))
		((< t two)
		 (let ((s (* 5 (- t one))))
		   ((inside-circle out1 new-circle in3 (+ n 1)) s)))
		((< t three)
		 (let ((s (* 5 (- t two))))
		   ((inside-circle out2 new-circle in3 (+ n 1)) s)))
		((< t four)
		 (let ((s (* 5 (- t three))))
		   ((outside-circle out1 out2 new-circle (+ n 1)) s)))
		(else
		 (let ((s (* 5 (- t four))))
		   (cons (+ (* (cos (* 2 pi s)) radius) (car location))
			 (+ (* (sin (* 2 pi s)) radius) (cdr location)))))))))
    (define (outside-circle out1 out2 out3 n)
      (let* ((r1 (get-r out1))
	     (r2 (get-r out2))
	     (r3 (get-r out3))
	     (radius
	      (/ (* r1 r2 r3)
		 (+ (* r1 r2) (* r1 r3) (* r2 r3)
		    (* 2 (sqrt (* r1 r2 r3 (+ r1 r2 r3)))))))
	     (location1
	      (circle-point out1 out2 radius))
	     (sqrdist1
	      (+ (sqr (- (car location1) (get-x out3)))
		 (sqr (- (cdr location1) (get-y out3)))))
	     (location2
	      (circle-point out2 out1 radius))
	     (sqrdist2
	      (+ (sqr (- (car location2) (get-x out3)))
		 (sqr (- (cdr location2) (get-y out3)))))
	     (location
	      (if (< sqrdist1 sqrdist2) location1 location2))
	     (new-circle
	      (list (car location) (cdr location) radius)))
	(lambda (t)
	  (cond ((> n 10)
		 (let ((s (* 2 pi t)))
		   (cons (+ (* (cos s) radius) (car location))
			 (+ (* (sin s) radius) (cdr location)))))
		((< t one)
		 (let ((s (* 5 t)))
		   ((full-circle (car location) (cdr location) radius (+ n 1)) s)))
		((< t two)
		 (let ((s (* 5 (- t one))))
		   ((outside-circle out1 out2 new-circle (+ n 1)) s)))
		((< t three)
		 (let ((s (* 5 (- t two))))
		   ((outside-circle out2 out3 new-circle (+ n 1)) s)))
		((< t four)
		 (let ((s (* 5 (- t three))))
		   ((outside-circle out3 out1 new-circle (+ n 1)) s)))
		(else
		 (let ((s (* 5 (- t four))))
		   (cons (+ (* (cos (* 2 pi s)) radius) (car location))
			 (+ (* (sin (* 2 pi s)) radius) (cdr location)))))))))

    (define (circle-point circ1 circ2 r)
      (let ((x1 (get-x circ1))
	    (y1 (get-y circ1))
	    (r1 (+ (get-r circ1) r))
	    (x2 (get-x circ2))
	    (y2 (get-y circ2))
	    (r2 (+ (get-r circ2) r)))
	(let* ((dist (sqrt (+ (sqr (- x1 x2)) (sqr (- y1 y2)))))
	       (to-between (/ (+ (sqr r1) (- (sqr r2)) (sqr dist)) (* 2 dist)))
	       (bet-to-perp (sqrt (- (sqr r1) (sqr to-between))))
	       (between-x (+ x1 (/ (* to-between (- x2 x1)) dist)))
	       (between-y (+ y1 (/ (* to-between (- y2 y1)) dist)))
	       (result-x (+ between-x (/ (* bet-to-perp (- y2 y1)) dist)))
	       (result-y (- between-y (/ (* bet-to-perp (- x2 x1)) dist))))
	  (cons result-x result-y))))
    full-circle))

(define (get-x circ)
  (first circ))

(define (get-y circ)
  (second circ))

(define (get-r circ)
  (third circ))

;(define win (make-graphics-device 'win32 500 500))
;;Value: win

;(define (draw-all p t0 t1 dt)
;  (if (< t0 t1)
;      (let ((point (p t0)))
;	(graphics-draw-point win (car point) (cdr point))
;	(draw-all p (+ t0 dt) t1 dt))))
;;Value: draw-all

;(draw-all (circle-fractal 0 0 1 5) 0 1 .00001)
;;Unspecified return value
