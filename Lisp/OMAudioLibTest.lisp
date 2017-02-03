(in-package :cl-user)

;;; load CFFI
(load (merge-pathnames "ffi/load-cffi" *load-pathname*))

;;; link the C library
(fli:register-module 
   "OMJuceAudioLib" 
   ;;:real-name "/home/andersvi/site/OM/OM-DEVELOPMENT_WINTER.2017/efficace/OMJuceAudioLib/Builds/LinuxMakefile/build/libOMJuceAudioLib.so"
   :real-name (namestring (make-pathname :directory (append (butlast (pathname-directory *load-pathname*))
                                                            #+macosx (list "Builds" "MacOSX" "build" "Debug")
    							    #+linux (list "Builds" "Linux" "build")
    							    )
    					 :name "OMAudioLib" 
                                         :type #+macosx "dylib" #+linux "so"))
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
(juce::getOutputDevicesCount +test-juce-player+)

(let ((n-types (juce::getDevicesTypeCount +test-juce-player+)))
  (loop for type from 0 to (1- n-types) append
        (let ((type-name (juce::getDeviceTypeName +test-juce-player+ type)))
          (loop for n from 0 to (1- (juce::getOutputDevicesCountForType +test-juce-player+ type)) 
                collect (list type-name (juce::getNthOutputDeviceName +test-juce-player+ type n)))
          )))

(pprint (let ((n-types (juce::getDevicesTypeCount +test-juce-player+)))
	  (loop for type from 0 to (1- n-types) append
	       (let ((type-name (juce::getDeviceTypeName +test-juce-player+ type)))
		 (loop for n from 0 to (1- (juce::getInputDevicesCountForType +test-juce-player+ type)) 
		    collect (list type-name (juce::getNthInputDeviceName +test-juce-player+ type n)))
		 ))))


(juce::setdevices +test-juce-player+ 
                  "" 0 ; no input 
                  "Built-in Output" 2 ; change with correct device
                  44100 512)

;;; setup a player
;; (juce::setdevicetype +test-juce-player+ "JACK")
;; (juce::setdevicetype +test-juce-player+ "ALSA")

(juce::audio-driver-output-devices
 +test-juce-player+
 (juce::getCurrentDeviceType +test-juce-player+))

(juce::setdevices +test-juce-player+ 
                  "" 0 ; no input 
                  ;; "system" 2 
                  "Default ALSA Output (currently PulseAudio Sound Server)" 2 ; change with correct device
                  44100 128)


(juce::getcurrentsamplerate +test-juce-player+)
(juce::getbuffersizes +test-juce-player+)
(juce::getavailablesampleratescount +test-juce-player+)
(juce::getsamplerates +test-juce-player+)

;; test play 
#+macosx (defparameter *soundfile* "/Users/bresson/_SHARED-FILES/IN-FILES/SOUNDFILES/Bassclarinet2.aif")
#+linux (defparameter *soundfile* "/home/andersvi/lyd/andersvi/Friday_10.wav")

(juce::makefilereader *soundfile*)

(let ((reader (juce::makefilereader *soundfile*)))
  (unwind-protect 
      (progn
        (juce::startreader +test-juce-player+ reader)
        (sleep 1)
        (juce::pausereader +test-juce-player+ reader)
        (sleep 1)
        
        (juce::setgainreader reader 0.1) 
        (juce::setposreader reader 0) 
        (juce::startreader +test-juce-player+ reader)
        (sleep 1)
        (juce::stopreader +test-juce-player+ reader)
	'done)
    (juce::freereader reader)
    ))
  

