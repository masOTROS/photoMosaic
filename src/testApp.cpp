#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // we will have a dynamic number of images, based on the content of a directory:
    ofDirectory dir;
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.allowExt("jpeg");
    dir.listDir("images/");
	for(int i = 0; i < (int)dir.size(); i++){
        addImage(dir.getPath(i));
	}

}

//--------------------------------------------------------------
void testApp::addImage(string file){
    ofPtr<ofImage> img = ofPtr<ofImage>( new ofImage(file) );
    img->setImageType(OF_IMAGE_GRAYSCALE);
    long int accum=0;
    int numPixels=img->getWidth()*img->getHeight();
    unsigned char * imgPixels = img->getPixels();
    for(int i=0;i<numPixels;i++){
        accum+=imgPixels[i];
    }
    accum/=numPixels;
    images[accum].push_back(img);
    
    ofxCvGrayscaleImage origImg;
    origImg.setFromPixels(img->getPixels(),img->getWidth(),img->getHeight());
    ofxCvGrayscaleImage smallImg;
    smallImg.allocate(1,1);
    smallImg.scaleIntoMe(origImg);
    unsigned char * smPix = smallImg.getPixels();
    cout<<accum<<","<<smPix[0]<<endl;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);
    float size=200;
    for(int i=0;i<ALFA;i++){
        for(int j=0;j<images[i].size();j++){
            images[i].at(j)->draw(i*ofGetWidth()/ALFA,size*j,size,size*images[i].at(j)->getHeight()/images[i].at(j)->getWidth());
        }
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}