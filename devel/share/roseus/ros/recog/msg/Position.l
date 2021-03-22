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
  :slots ())

(defmethod recog::Position
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(setf (get recog::Position :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get recog::Position :datatype-) "recog/Position")
(setf (get recog::Position :definition-)
      "
")



(provide :recog/Position "d41d8cd98f00b204e9800998ecf8427e")


