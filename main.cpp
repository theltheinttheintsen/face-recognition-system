// Include OpenCV library for camera and image processing
#include <opencv2/opencv.hpp>
// Include for input/output (cout, cin)
#include <iostream>
// Include for reading files (patient_data.txt)
#include <fstream>
// Include for using string variables
#include <string>

// Use standard namespace so we don't need to write std:: every time
using namespace std;
// Use OpenCV namespace so we don't need to write cv:: every time
using namespace cv;

// ============================================================
// FUNCTION: showPatientRecord
// PURPOSE : Search patient_data.txt by name and display record
// ============================================================
void showPatientRecord(string searchName) {

    // Open the patient data text file for reading
    ifstream file("C:/Users/thelt/OneDrive/Pictures/Desktop/360/patient_data.txt");

    // Check if file opened successfully
    if (!file.is_open()) {
        cout << "Error: Cannot open patient_data.txt!" << endl;
        return;
    }

    string line;
    bool found = false;
    bool printLines = false;


    cout << "       PHARMACEUTICAL RECORDS           " << endl;

    // Read file line by line
    while (getline(file, line)) {

        // Check if this line contains the patient name
        if (line.find(searchName) != string::npos) {
            found = true;
            printLines = true;
        }

        // Stop printing when we reach separator "---"
        if (line == "---" && printLines) {
            printLines = false;
            break;
        }

        // Print line if we are in correct patient section
        if (printLines) {
            cout << line << endl;
        }
    }

    // If patient not found in file
    if (!found) {
        cout << "Patient \"" << searchName << "\" not found!" << endl;
    }

    file.close();
}

// ============================================================
// MAIN FUNCTION
// PURPOSE : Run the secure pharmaceutical system
// FLOW    : 
//   1. Ask user to enter patient name
//   2. Open webcam
//   3. Wait for face to be detected
//   4. Once face confirmed, show patient record
// ============================================================
int main() {

    // --------------------------------------------------------
    // STEP 1: Ask for patient name before opening camera
    // This identifies WHICH patient record to retrieve
    // --------------------------------------------------------
    string patientName;
    cout << "========================================" << endl;
    cout << "   PHARMACEUTICAL SECURE SYSTEM         " << endl;
    cout << "========================================" << endl;
    cout << "Enter patient name: ";
    getline(cin, patientName);

    // --------------------------------------------------------
    // STEP 2: Load Haar Cascade face detector
    // Haar Cascade detects faces by looking for patterns
    // like eyes, nose and mouth in the image
    // --------------------------------------------------------
    CascadeClassifier faceCascade;
    faceCascade.load("C:/msys64/mingw64/share/opencv4/haarcascades/haarcascade_frontalface_default.xml");

    // Check if face detector loaded successfully
    if (faceCascade.empty()) {
        cout << "Error: Could not load face cascade!" << endl;
        cin.get();
        return -1;
    }

    // --------------------------------------------------------
    // STEP 3: Open webcam
    // Index 0 = default webcam
    // CAP_DSHOW = DirectShow backend (works better on Windows)
    // --------------------------------------------------------
    VideoCapture cap(0, CAP_DSHOW);

    // Check if webcam opened successfully
    if (!cap.isOpened()) {
        cout << "Error: Cannot open webcam!" << endl;
        cin.get();
        return -1;
    }

    cout << "\nPlease show your face to verify identity..." << endl;
    cout << "Press ESC to exit" << endl;

    Mat frame;           // Stores current camera frame (color)
    Mat gray;            // Stores grayscale version of frame
    vector<Rect> faces;  // Stores list of detected face positions
    bool accessGranted = false; // Tracks if access was given
    int faceCount = 0;   // Counts frames where face is detected

    // --------------------------------------------------------
    // STEP 4: Main camera loop
    // Runs continuously until face confirmed or ESC pressed
    // --------------------------------------------------------
    while (true) {

        // Capture one frame from webcam
        cap >> frame;

        // If frame is empty stop the loop
        if (frame.empty()) break;

        // Convert color frame to grayscale
        // Face detection works better on grayscale
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Detect faces in the grayscale frame
        // 1.1 = how much to shrink image each pass
        // 5   = minimum neighbors to confirm a face
        // Size(100,100) = minimum face size to detect
        faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, Size(100, 100));

        if (faces.size() > 0) {
            // Face detected! Count how many frames
            faceCount++;

            // Draw green rectangle around detected face
            for (Rect face : faces) {
                rectangle(frame, face, Scalar(0, 255, 0), 2);

                if (!accessGranted) {
                    // Show countdown message
                    string msg = "Verifying... " + to_string(10 - faceCount/3) + "s";
                    putText(frame, msg,
                            Point(face.x, face.y - 10),
                            FONT_HERSHEY_SIMPLEX, 0.7,
                            Scalar(0, 255, 255), 2);
                } else {
                    // Show access granted
                    putText(frame, "ACCESS GRANTED - " + patientName,
                            Point(face.x, face.y - 10),
                            FONT_HERSHEY_SIMPLEX, 0.6,
                            Scalar(0, 255, 0), 2);
                }
            }

            // After 30 consistent frames = face confirmed
            // This prevents false detections
            if (faceCount >= 30 && !accessGranted) {
                accessGranted = true;
                cout << "\nFace detected! Identity verified!" << endl;
                cout << "Welcome, " << patientName << "!" << endl;

                // Show patient record from file
                showPatientRecord(patientName);
            }

        } else {
            // No face detected
            faceCount = 0; // Reset counter

            if (!accessGranted) {
                // Show access denied message in red
                putText(frame, "NO FACE DETECTED - ACCESS DENIED",
                        Point(10, 30),
                        FONT_HERSHEY_SIMPLEX, 0.6,
                        Scalar(0, 0, 255), 2);
            }
        }

        // Display the camera frame on screen
        imshow("Pharmaceutical Secure System", frame);

        // Wait 30ms - if ESC pressed (key 27) exit loop
        if (waitKey(30) == 27) break;
    }

    // Release webcam and close all windows when done
    cap.release();
    destroyAllWindows();
    return 0;
}
