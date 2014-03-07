#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
    window.setGlutDisplayString("rgb alpha double samples acc depth>=24");
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new testApp());
}
