#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofNoFill();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);

	for (int i = 0; i < 3; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 5;

	while (this->log_list.size() < 500) {

		for (auto noise_seed : this->noise_seed_list) {

			vector<glm::vec2> log;
			log.push_back(glm::vec2(ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.003), 0, 1, 0, ofGetWidth()), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.003), 0, 1, 0, ofGetHeight())));
			this->log_list.push_back(log);
			this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
			this->life_list.push_back(ofRandom(120, 300));
		}
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec3(this->log_list[i].back() * 0.003, ofGetFrameNum() * 0.003)), 0, 1, -360, 360);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 90) {

			ofSetLineWidth(1.5);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 90, 0, 1.5));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}