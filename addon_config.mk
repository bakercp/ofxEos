meta:
	ADDON_NAME = ofxEOS
	ADDON_DESCRIPTION = An openFrameworks wrapper for EOS.
	ADDON_AUTHOR = Christopher Baker
	ADDON_TAGS = "machine learning" "computer vision" "face model" "faces" "face tracking"
	ADDON_URL = http://github.com/bakercp/ofxEOS

common:
	ADDON_DEPENDENCIES = ofxOpenCv ofxEigen ofxCereal ofxTOML ofxSpatialHash
	ADDON_DEFINES =
	ADDON_CFLAGS = -O3 -Wno-strict-aliasing
