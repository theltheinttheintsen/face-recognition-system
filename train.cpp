#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace cv;
using namespace cv::face;

int main() {
    vector<Mat> images;
    vector<int> labels;

    cout << "=== TRAINING FACE RECOGNIZER ===" << endl;

    // Load photos for each patient
    for (int id = 1; id <= 2; id++) {
        for (int i = 0; i < 20; i++) {
            string path = "C:/Users/thelt/OneDrive/Pictures/Desktop/360/faces/" 
                         + to_string(id) + "/face_" + to_string(i) + ".jpg";
            Mat img = imread(path, IMREAD_GRAYSCALE);
            if (!img.empty()) {
                images.push_back(img);
                labels.push_back(id);
                cout << "Loaded: " << path << endl;
            }
        }
    }

    cout << "\nTotal images loaded: " << images.size() << endl;
    cout << "Training..." << endl;

    // Train LBPH recognizer
    Ptr<LBPHFaceRecognizer> recognizer = LBPHFaceRecognizer::create();
    recognizer->train(images, labels);

    // Save trained model
    recognizer->save("C:/Users/thelt/OneDrive/Pictures/Desktop/360/face_model.yml");
    cout << "Training complete! Model saved as face_model.yml" << endl;
    cin.get();
    return 0;
}
