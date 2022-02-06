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

;;; set a test player
(defvar +test-juce-player+)
(setf +test-juce-player+ (juce::openAudioManager))

;;; get info about available devices
(juce::getDevicesTypeCount +test-juce-player+)
(juce::getDeviceTypeName +test-juce-player+ 0)
(juce::getInputDevicesCountForType +test-juce-player+ 0)
(juce::getOutputDevicesCountForType +test-juce-player+ 0)


(let ((n-types (juce::getDevicesTypeCount +test-juce-player+)))
  (loop for type from 0 to (1- n-types) append
        (let ((type-name (juce::getDeviceTypeName +test-juce-player+ type)))
          (loop for n from 0 to (1- (juce::getOutputDevicesCountForType +test-juce-player+ type))
                collect (list type-name (juce::getNthOutputDeviceName +test-juce-player+ type n)))
          )))

(let ((n-types (juce::getDevicesTypeCount +test-juce-player+)))
	  (loop for type from 0 to (1- n-types) append
	       (let ((type-name (juce::getDeviceTypeName +test-juce-player+ type)))
		 (loop for n from 0 to (1- (juce::getInputDevicesCountForType +test-juce-player+ type))
		    collect (list type-name (juce::getNthInputDeviceName +test-juce-player+ type n)))
		 )))


;;; setup a player
;; (juce::setdevicetype +test-juce-player+ "JACK")
;; (juce::setdevicetype +test-juce-player+ "ALSA")

(juce::audio-driver-output-devices
 +test-juce-player+
 (juce::getCurrentDeviceType +test-juce-player+))


(juce::getcurrentsamplerate +test-juce-player+)
(juce::getbuffersizes +test-juce-player+)
(juce::getavailablesampleratescount +test-juce-player+)
(juce::getsamplerates +test-juce-player+)

;; test play
;; (defparameter *soundfile* "/Users/bresson/_SHARED-FILES/IN-FILES/SOUNDFILES/Bassclarinet2.aif")
;; (defparameter *soundfile* "/home/andersvi/lyd/andersvi/Friday_10.wav")
;; (defparameter *soundfile* "C:/Users/Bresson/Desktop/SOUNDFILES/guitar-lick.aif")


;; (setq reader (juce::makeAudioFileReader *soundfile*))
;; (juce::startAudioSource +test-juce-player+ reader)
;; (juce::stopAudioSource +test-juce-player+ reader)
;; (juce::setAudioSourceGain reader 0.1)
;; (juce::setAudioSourcePos reader 0)
;; (juce::startAudioSource +test-juce-player+ reader)
;; (juce::freeAudioFileReader reader)
