#include "testApp.h"

#define DOWNSCALE 20

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    // we will have a dynamic number of images, based on the content of a directory:
    ofDirectory dir;
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.allowExt("jpeg");
    dir.listDir("images/");
	for(int i = 0; i < (int)dir.size(); i++){
        addImage(dir.getPath(i));
	}
    fillImages();
    
    camera.initGrabber(1920,1080);
    
    mosaic.allocate(960,540);
    mosaic.begin();
    ofClear(0);
    mosaic.end();
}

//--------------------------------------------------------------
void testApp::addImage(string file){
    ofPtr<ofImage> img = ofPtr<ofImage>( new ofImage(file) );
    img->setImageType(OF_IMAGE_GRAYSCALE);
    img->setAnchorPercent(0.5,0.5);
    
    //The other method
    ofxCvGrayscaleImage origImg;
    origImg.setFromPixels(img->getPixels(),img->getWidth(),img->getHeight());
    
    ofxCvGrayscaleImage smallImg;
    smallImg.allocate(DOWNSCALE,DOWNSCALE);
    //for interpolation you can pass in:
    //CV_INTER_NN - nearest-neigbor interpolation,
    //CV_INTER_LINEAR - bilinear interpolation (used by default)
    //CV_INTER_AREA - resampling using pixel area relation. It is preferred method
    //                for image decimation that gives moire-free results. In case of
    //                zooming it is similar to CV_INTER_NN method.
    //CV_INTER_CUBIC - bicubic interpolation.
    smallImg.scaleIntoMe(origImg,CV_INTER_AREA);
    
    
    int numPixels=smallImg.getWidth()*smallImg.getHeight();
    unsigned char * smallPix = smallImg.getPixels();
    long int accum=0;
    for(int i=0;i<numPixels;i++){
        accum+=smallPix[i];
    }
    accum/=numPixels;
    
    images[accum].push_back(img);
}

//--------------------------------------------------------------
void testApp::fillImages(){
    ofPtr<ofImage> img;
    for(int i=0;i<ALFA;i++){
        if(images[i].size()){
            img=images[i].at(0);
            break;
        }
    }
    for(int i=0;i<ALFA;i++){
        if(images[i].size()){
            img=images[i].at(0);
        }
        else{
            images[i].push_back(img);
        }
    }
}

//--------------------------------------------------------------
void testApp::update(){
    camera.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);
    camera.draw(0,0,960,540);
    mosaic.draw(960,0);
    float size=50;
    for(int i=0;i<ALFA;i++){
        for(int j=0;j<images[i].size();j++){
            images[i].at(j)->draw(i*ofGetWidth()/ALFA,540+size*j*0.5,size,size*images[i].at(j)->getHeight()/images[i].at(j)->getWidth());
        }
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofxCvColorImage origin;
    origin.allocate(camera.getWidth(),camera.getHeight());
    origin.setFromPixels(camera.getPixels(),camera.getWidth(),camera.getHeight());
    
    ofxCvColorImage originScaled;
    originScaled.allocate(mosaic.getWidth()/DOWNSCALE,mosaic.getHeight()/DOWNSCALE);
    //for interpolation you can pass in:
    //CV_INTER_NN - nearest-neigbor interpolation,
    //CV_INTER_LINEAR - bilinear interpolation (used by default)
    //CV_INTER_AREA - resampling using pixel area relation. It is preferred method
    //                for image decimation that gives moire-free results. In case of
    //                zooming it is similar to CV_INTER_NN method.
    //CV_INTER_CUBIC - bicubic interpolation.
    originScaled.scaleIntoMe(origin);
    
    ofxCvGrayscaleImage grayScaled;
    grayScaled.allocate(mosaic.getWidth()/DOWNSCALE,mosaic.getHeight()/DOWNSCALE);
    grayScaled=originScaled;
    
    unsigned char * colorPixels = originScaled.getPixels();
    unsigned char * grayPixels = grayScaled.getPixels();
    mosaic.begin();
    ofSetColor(255,255);
    for(int y=0;y<grayScaled.getHeight();y++){
        for(int x=0;x<grayScaled.getWidth();x++){
            int index=int(grayPixels[int(x+y*grayScaled.getWidth())]);
            ofPtr<ofImage> img = images[index].at(int(ofRandom(images[index].size())));
            ofSetColor(colorPixels[int(x+y*originScaled.getWidth())*3+0],colorPixels[int(x+y*originScaled.getWidth())*3+1],colorPixels[int(x+y*originScaled.getWidth())*3+2]);
            img->draw(x*DOWNSCALE,y*DOWNSCALE,DOWNSCALE*img->getWidth()/img->getHeight(),DOWNSCALE);
        }
    }
    //ofSetColor(255,200);
    //originScaled.draw(0,0,960,540);
    mosaic.end();
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