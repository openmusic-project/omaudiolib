(in-package :cl-user)

;;; load CFFI
(load (merge-pathnames "ffi/load-cffi" *load-pathname*))

;;; link the C library
(fli:register-module
   "omaudiolib"
   :real-name (namestring 
               (make-pathname 
                :directory (append (butlast (pathname-directory *load-pathname*))
                                   #+macosx (list "Builds" "MacOSX" "build" "Debug")
                                   #+linux (list "Builds" "Linux" "build")
                                   #+windows (list "Builds" "VisualStudio2015" "Win32" "Release" "Dynamic Library")
                                   )
                :name "omaudiolib"
                :type #+macosx "dylib" #+linux "so" #+windows "dll"))
   :connection-style :immediate)

;;; load the bindings
(load (merge-pathnames "omaudiolib.lisp" *load-pathname*))


(defun test-print (format-string &optional args)
  (apply 'format (append (list t (concatenate 'string "~%:omaudiolib-test: "
                                              (substitute #\~ #\$ format-string :test 'string-equal)))
                         args)))


(test-print (juce::versionstring))

;;; get info about available devices

(defun scan-devices ()
  (let* ((player (juce::openAudioManager))
         (n-types (juce::getDevicesTypeCount player)))
    (test-print "Number of devices types: $A" (list n-types))
    (dotimes (type n-types)
      (test-print "Device type $A: $A"(list type (juce::getDeviceTypeName player type)))
      (let ((n-out-devices (juce::getOutputDevicesCountForType player type))
            (n-in-devices (juce::getInputDevicesCountForType player type)))
        (test-print "  Number of output devices: $A" (list n-out-devices))
        (dotimes (out n-out-devices)
          (test-print "  - $A" (list (juce::getNthOutputDeviceName player type out))))
        (test-print "  Number of input devices: $A" (list n-in-devices))
        (dotimes (in n-in-devices)
          (test-print "  - $A" (list (juce::getNthInputDeviceName player type in))))
        ))
    (juce::closeAudioManager player)))

; (scan-devices)


(defun print-state (player)
  (test-print "Current device: $A" (list (juce::getCurrentDeviceName player)))
  (test-print "  Sample rates: $A" (list (juce::getsamplerates player)))
  (test-print "  - Current sample rate: $A" (list (juce::getcurrentsamplerate player)))
  (test-print "  Buffer sizes: $A" (list (juce::getbuffersizes player)))
  (test-print "  - Current buffer size: $A"(list (juce::getcurrentbuffersize player))))

(defparameter *DEVICE* 0)

(defun init-audio-device (n)
  (let ((player (juce::openAudioManager)))
    (assert (< n (juce::getDevicesTypeCount player)))
    (test-print "Setting device type: $A" (list (juce::getDeviceTypeName player n)))
    (juce::setdevicetype player (juce::getDeviceTypeName player n))
    (juce::setOutputDevice player 0)
    (print-state player)
    player))

; (progn (let ((p (init-audio-device 0))) (juce::closeAudioManager p)))


;; test play

(defun play (device soundfile)
  (let ((player (init-audio-device device))
        (source (juce::makeaudiosourcefromfile soundfile)))
   (juce::startAudioSource player source)
   (sleep 1)
   (juce::pauseAudioSource player source)
   (sleep 1)
   (juce::startAudioSource player source)
   (sleep 1)
   (juce::stopAudioSource player source)
   (sleep 1)
   (juce::startAudioSource player source)
   (sleep 1)
   (juce::stopAudioSource player source)

   (juce::freeaudiosource source)
   (juce::closeAudioManager player)))

;; (defparameter *soundfile* "/Users/bresson/_SHARED-FILES/IN-FILES/SOUNDFILES/Bassclarinet2.aif")
;; (defparameter *soundfile* "/home/andersvi/lyd/andersvi/Friday_10.wav")
;; (defparameter *soundfile* "C:/Users/Bresson/Desktop/SOUNDFILES/guitar-lick.aif")

; (play 0 *soundfile*)
