;; Auto-generated. Do not edit!


(when (boundp 'recog::Position)
  (if (not (find-package "RECOG"))
    (make-package "RECOG"))
  (shadow 'Position (find-package "RECOG")))
(unless (find-package "RECOG::POSITION")
  (make-package "RECOG::POSITION"))

(in-package "ROS")
;;//! \htmlinclude Position.msg.html


(defclass recog::Position
  :super ros::object
  :slots (_theta _alpha _distance ))

(defmethod recog::Position
  (:init
   (&key
    ((:theta __theta) 0.0)
    ((:alpha __alpha) 0.0)
    ((:distance __distance) 0.0)
    )
   (send-super :init)
   (setq _theta (float __theta))
   (setq _alpha (float __alpha))
   (setq _distance (float __distance))
   self)
  (:theta
   (&optional __theta)
   (if __theta (setq _theta __theta)) _theta)
  (:alpha
   (&optional __alpha)
   (if __alpha (setq _alpha __alpha)) _alpha)
  (:distance
   (&optional __distance)
   (if __distance (setq _distance __distance)) _distance)
  (:serialization-length
   ()
   (+
    ;; float64 _theta
    8
    ;; float64 _alpha
    8
    ;; float64 _distance
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _theta
       (sys::poke _theta (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _alpha
       (sys::poke _alpha (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _distance
       (sys::poke _distance (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _theta
     (setq _theta (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _alpha
     (setq _alpha (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _distance
     (setq _distance (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get recog::Position :md5sum-) "81d9e56e69a7c53642d195f1f0bb7a27")
(setf (get recog::Position :datatype-) "recog/Position")
(setf (get recog::Position :definition-)
      "float64 theta
float64 alpha
float64 distance

")



(provide :recog/Position "81d9e56e69a7c53642d195f1f0bb7a27")


