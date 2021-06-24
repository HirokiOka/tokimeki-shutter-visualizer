#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetWindowTitle("TOKIMEKI Visualizer");
  ofSetFrameRate(30);
  ofBackground(188, 226, 232);

  //load image, sound, font
  logo_image.load(path_to_logo);
  kobe_image.load(path_to_kobe);
  boy_image.load(path_to_boy);
  button_sound.load(path_to_button_sound);
  tokimeki_sound.load(path_to_tokimeki_sound);
  cancel_sound.load(path_to_cancel_sound);
  text_font.load(path_to_font, 32);

  //start udp connection
  int init_port = 50001;
  for (int i = 0; i < 7; ++i) {
    int port_num = init_port + i;
    udpConnection[i].Create();
    udpConnection[i].SetEnableBroadcast(true);
    udpConnection[i].Bind(port_num);
    udpConnection[i].SetNonBlocking(true);
  }

  //vidGrabber.setVerbose(true);
  vidGrabber.setDeviceID(camera_device_id);
  vidGrabber.listDevices();
  vidGrabber.initGrabber(camera_width, camera_height);

  //button settings
  string button_labels[] = {"START", "START(NOCAM)", "GALLERY", "RETURN"};
  button_labels_size = sizeof(button_labels) / sizeof(button_labels[0]);
  for (int i = 0; i < button_labels_size; ++i) {
    string button_label = button_labels[i];
    myButton = new ofxDatGuiButton(button_label);
    myButton->onButtonEvent(this, &ofApp::onButtonEvent);
    myButton->setHeight(button_height);
    component = myButton;
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    if (button_label == "RETURN") component->setPosition(ofGetWidth()*2/3, ofGetHeight()- 100);
    else component->setPosition(ofGetWidth() / 2 - button_width / 2, ofGetHeight() / 2 - button_height / 2 + button_y_padding * i);
    component->setLabelColor(ofColor(0));
    component->setBackgroundColor(ofColor(0, 200, 0));
    component->setWidth(button_width, 0);
    components.push_back(component);
  }

  //plotter settings
  string sensor_labels[] = {"LFP_outside", "LFP_inside", "LFP_back", "RFP_outside", "RFP_inside", "RFP_back", "HORIZONTAL"};
  sensor_labels_size = sizeof(sensor_labels) / sizeof(sensor_labels[0]);
  for (int i = 0; i < sensor_labels_size; ++i) {
    string label = sensor_labels[i];
    plotters[i] = new ofxDatGuiValuePlotter(label, sensor_min_value, sensor_max_value);
    plotters[i]->setDrawMode(ofxDatGuiGraph::LINES);
    plotters[i]->setSpeed(3);
    component = plotters[i];
    component->setBackgroundColor(ofColor(0, 100, 0));
    float pos_x = 0;
    float pos_y = 0;
    if (i < 3) {
      //Left Foot Pressure
      pos_x = plotter_width;
      pos_y = ofGetHeight() - 220 + i * 70;
    } else if (i < 6){
      //Right Foot Pressue
      pos_x = 0;
      pos_y = ofGetHeight() - 220 + (i - 3) * 70;
    } else {
      //Head
      pos_x = 20;
      pos_y = 20;
    }
    component->setPosition(pos_x, pos_y);
    component->setWidth(plotter_width, plotter_label_width);
    plotter_components.push_back(component);
  }

  for (int i = 0; i < sensor_labels_size; ++i) {
    string label = sensor_labels[i];
    no_cam_plotters[i] = new ofxDatGuiValuePlotter(label, sensor_min_value, sensor_max_value);
    no_cam_plotters[i]->setDrawMode(ofxDatGuiGraph::LINES);
    no_cam_plotters[i]->setSpeed(3);
    component = no_cam_plotters[i];
    component->setBackgroundColor(ofColor(0, 100, 0));
    float pos_x = 0;
    float pos_y = 0;
    if (i < 3) {
      //Left Foot Pressure
      pos_x = plotter_width;
      pos_y = ofGetHeight() - 220 + i * 70;
    } else if (i < 6){
      //Right Foot Pressue
      pos_x = 0;
      pos_y = ofGetHeight() - 220 + (i - 3) * 70;
    } else {
      //Head
      pos_x = 20;
      pos_y = 20;
    }
    component->setPosition(pos_x, pos_y);
    component->setWidth(plotter_width, plotter_label_width);
    no_cam_plotter_components.push_back(component);
  }

}

//--------------------------------------------------------------
void ofApp::update(){
  //Menu
  if (app_state == 0) {
    for (int i = 0; i < components.size() - 1; ++i) components[i]->update();

  } else if (app_state == 1) {
    //Visualizer

    for (int i = 0; i < 7; ++i) {
      char udpMessage[255];
      udpConnection[i].Receive(udpMessage, 255);
      string message=udpMessage;
      int row_data = ofBinaryToInt(ofToBinary(message));
      int fixed_value = 0;
      if (i == 6) {
        fixed_value = ofMap(row_data, -90, 90, 0, 250);
      } else {
        fixed_value = ofMap(row_data, 250, 0, 0, 250);
      }
      if (row_data) plotters[i]->setValue(fixed_value);
      plotter_components[i]->update();
    }

    /*
    for (int i = 0; i < plotter_components.size(); ++i) {
      //float current_value = ofRandom(0, 250);
      plotters[i]->setValue(current_value);
      plotter_components[i]->update();
    } 
    */
    vidGrabber.update();
    components[3]->update();

  } else if (app_state == 2) {
    //GALLERY
      
    if (!is_read_dir) {
      ofImage current_image;
      ofDirectory dir(photo_dir_path);
      dir.allowExt("jpg");
      dir.allowExt("jpeg");
      dir.allowExt("png");
      dir.listDir();
      for (int i = 0; i < dir.size(); ++i) {
        current_image.load(dir.getPath(i));
        current_image.resize(photo_width, photo_height);
        photos.push_back(current_image);
      }
      is_read_dir = true;
    }
   components[3]->update();

  } else if (app_state == 3) {
    //NOCAM
    /*
    for (int i = 0; i < plotter_components.size(); ++i) {
      float current_value = ofRandom(0, 1000);
      plotters[i]->setValue(current_value);
      plotter_components[i]->update();
    } 
    */
    for (int i = 0; i < 7; ++i) {
      char udpMessage[255];
      udpConnection[i].Receive(udpMessage, 255);
      string message=udpMessage;
      int row_data = ofBinaryToInt(ofToBinary(message));
      int fixed_value = 0;
      if (i == 6) {
        fixed_value = ofMap(row_data, -90, 90, 0, 250);
      } else {
        fixed_value = ofMap(row_data, 250, 0, 0, 250);
      }
      if (row_data) no_cam_plotters[i]->setValue(fixed_value);
      no_cam_plotter_components[i]->update();
    }
  }
  components[3]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  int window_width = ofGetWidth();
  int window_height = ofGetHeight();

  //Menu
  if (app_state == 0) {
    ofSetColor(255);
    kobe_image.draw(0, 0, window_width, window_height);
    logo_image.draw(window_width / 2 - x_offset, window_height / 4 - y_offset, logo_width, logo_height);
    for (int i = 0; i < components.size() - 1; ++i) components[i]->draw();
    
  //Visualizer
  } else if(app_state == 1) {
    ofBackground(188, 226, 232);
    /*
    if (is_tokimeki) {
      drawTokimeki();
      if (ofGetElapsedTimeMillis() > 3000) is_tokimeki = false;
    }
    */
    ofSetColor(0);
    text_font.drawString("TOKIMEKI VISION", window_width/2 - 200, window_height / 6);

    ofSetColor(255);

    boy_image.draw(window_width / 4 - boy_image.getWidth() / 2, window_height / 2 - boy_image.getHeight() / 4, boy_image.getWidth()/2, boy_image.getHeight()/2);

    float boy_head_x = window_width / 4 - boy_image.getWidth() / 3;
    float boy_head_y = window_height / 2 - boy_image.getHeight() / 4;
    float boy_rfoot_x = boy_head_x - 20;
    float boy_lfoot_x = boy_head_x + 60;
    float boy_foot_y = window_height / 2 + boy_image.getHeight() / 6;

    vidGrabber.draw(window_width/3, window_height/5);

    ofSetColor(100, 0, 0);
    ofDrawLine(20, 20, boy_head_x, boy_head_y);
    ofDrawLine(0, ofGetHeight(), boy_rfoot_x, boy_foot_y);
    ofDrawLine(400, ofGetHeight(), boy_lfoot_x, boy_foot_y);

    for (int i = 0; i < plotter_components.size(); ++i) {
      plotter_components[i]->draw();
    }
    components[3]->draw();

 //GALLERY
  } else if (app_state == 2) {
    ofSetColor(0);
    text_font.drawString("GALLERY MODE", 60, 60);
    ofSetColor(255);
    for (int i = 0; i < photos.size(); ++i) {
      int col_num = window_width / photo_width; 
      if (i < col_num) {
        photos[i].draw(i * (photo_width + 20) + 20, 80);
      } else {
        photos[i].draw((i % col_num) * (photo_width + 20) + 20, photo_height + 100);
      }
    }
    components[3]->draw();

  } else if (app_state == 3) {
    //NOCAM
    ofBackground(188, 226, 232);
    ofSetColor(255);

    boy_image.draw(window_width / 2 - boy_image.getWidth() / 2, window_height / 2 - boy_image.getHeight() / 2);

    float boy_head_x = window_width / 2;
    float boy_head_y = window_height / 2 - boy_image.getHeight() / 2;
    float boy_rfoot_x = boy_head_x - 20;
    float boy_lfoot_x = boy_head_x + 60;
    float boy_foot_y = window_height / 2 + boy_image.getHeight() / 6;

    ofSetColor(100, 0, 0);
    ofDrawLine(20, 20, boy_head_x, boy_head_y);

    for (int i = 0; i < plotter_components.size(); ++i) {
      plotter_components[i]->draw();
    }
    components[3]->draw();
  }
}

void ofApp::drawStar(int x, int y, int r) {
  int vertex_num = 10;
  ofSetColor(255, 217, 0);
  ofPushMatrix();
  ofTranslate(x, y);
  //ofRotateRad(60);
  ofRotateRad(ofGetFrameNum() * 0.04);
  ofBeginShape();
  for (int i = 0; i < vertex_num; ++i) {
    int radius = (i % 2 == 0) ? r : r / 2;
    ofVertex(radius * cos(ofDegToRad(360 * i / vertex_num)), radius * sin(ofDegToRad(360 * i / vertex_num)));
  }
  ofEndShape(true);
  ofPopMatrix();
}

void ofApp::drawTokimeki() {
  ofBackground(234, 145, 152);
  drawStar(100, 243,30);
  drawStar(220, 200, 60);
  drawStar(400, 20, 20);
  drawStar(500, 500, 30);
  drawStar(400, 200, 60);
  drawStar(500, 30, 40);
  drawStar(800, 50, 60);
  drawStar(800, 600, 60);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  /*
  if (app_state == 1) {
    tokimeki_sound.play();
    is_tokimeki = !is_tokimeki;
    ofResetElapsedTimeCounter();
  }
  */
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

  for (int i = 0; i < button_labels_size; ++i){
    components[i]->setPosition(ofGetWidth() / 2 - button_width / 2, ofGetHeight() / 2 - button_height / 2 + button_y_padding * i);
    if (i == button_labels_size-1) {
      components[i]->setPosition(ofGetWidth()*2/3, ofGetHeight()- 100);
    }
  }
    for (int i = 0; i < plotter_components.size(); ++i) {
      float pos_x = 0;
      float pos_y = 0;
      if (i < 3) {
        //Left Foot Pressure
        pos_x = plotter_width;
        pos_y = ofGetHeight() - 220 + i * 70;
      } else if (i < 6){
        //Right Foot Pressue
        pos_x = 0;
        pos_y = ofGetHeight() - 220 + (i - 3) * 70;

      } else {
        //Head
        pos_x = 20;
        pos_y = 20;
      }
      plotter_components[i]->setPosition(pos_x, pos_y);
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
  string label = e.target->getLabel();
  if (label == "START") {
    button_sound.play();
    app_state = 1;
  } else if(label == "GALLERY") {
    button_sound.play();
    is_read_dir = false;
    app_state = 2;
  } else if (label == "RETURN") {
    cancel_sound.play();
    app_state = 0;
    photos.clear();
  } else if (label == "START(NOCAM)") {
    app_state = 3;
    button_sound.play();
  }
}
