#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iomanip>
#include<string>
#include<vector>
#include <windows.h>
#include<winuser.h>
#include<stdio.h>
#include<stdlib.h>
#define WINVER 0x0500
#pragma comment(lib, "winmm")


using namespace cv;
using namespace std;

Mat src_gray;
int thresh = 65;
RNG rng(12345);
vector<int> rects;
vector<int> gestures;
string prevcomm = "";
string lastcomm = "";
string sspath = "C:/Users/garvi/OneDrive/Desktop/gestures/";
int sscount=0;
int nums = 10;
int hLow = 0;
int hHigh = 255;
int sLow = 0;
int sHigh = 255;
int vLow = 0;
int vHigh = 255;
int s = 0;
Mat imgThresh;

void checkgest() {
    Mat img(500, 600, CV_8UC3, Scalar(255, 255, 255));
    map<int, int> ch;
    int c = 0;
    string comm = "";
    int prev = gestures[0];
    for (int i = 1; i < gestures.size(); i++) {
        if (gestures[i] == prev) {c++; }
        else c = 0;
        if (c > 4) {comm.push_back('0' + gestures[i]); c = 0;}
        prev = gestures[i];
    }

    cout << comm << endl;
    if (comm == "50" || comm == "500" || comm == "5000") {
        putText(img, "Application has recieved a shutdown command, press ESC in the next 10 seconds to cancel.", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
        if (waitKey(10000) == 27) {
            putText(img, "Shutdown cancelled", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
            namedWindow("command");
            imshow("command", img);
            return;
        }
        putText(img, "Shutting down. See you later!", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
        system("c:\\windows\\system32\\shutdown /s");
    }
    else if (comm == "51" || comm=="511" || comm == "5111") {
        putText(img, "Application has recieved a restart command, press ESC in the next 10 seconds to cancel.", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
        if (waitKey(10000) == 27) {
            putText(img, "Restart cancelled", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
            namedWindow("command");
            imshow("command", img);
            return;
        }
        putText(img, "Restarting. See you in a while!", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
        system("c:\\windows\\system32\\shutdown /r");
    }
    else if (comm == "52" || comm == "522" || comm == "5222") {
        putText(img, "Application has recieved a sleep command, press ESC in the next 10 seconds to cancel.", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
        if (waitKey(10000) == 27) {
            putText(img, "Log off cancelled", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
            namedWindow("command");
            imshow("command", img);
            return;
        }
        putText(img, "Logging off. See you later!", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
        system("c:\\windows\\system32\\shutdown /l");
    }
    else if (comm == "1" || comm == "11" || comm=="111") {
        
        INPUT ip[1];
        ip[0].type = 1;
        ip[0].ki.wScan = 0;
        ip[0].ki.time = 0;
        ip[0].ki.dwExtraInfo = 0;
        ip[0].ki.wVk = 0xAF; 
        ip[0].ki.dwFlags = 0x0001; 
        SendInput(1, ip, sizeof(INPUT));
        
        ip[0].ki.dwFlags = 0x0002;
        SendInput(1, ip, sizeof(INPUT));
        putText(img, "increasing volume", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
        
    }
    else if (comm=="2" ||comm == "22" || comm == "222") {
        
        INPUT ip[1];
        ip[0].type = 1;
        ip[0].ki.wScan = 0;
        ip[0].ki.time = 0;
        ip[0].ki.dwExtraInfo = 0;
        ip[0].ki.wVk = 0xAE;
        ip[0].ki.dwFlags = 0x0001;
        SendInput(1, ip, sizeof(INPUT));

        ip[0].ki.dwFlags = 0x0002;
        putText(img, "decreasing volume", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
    }
    else if (comm == "3" || comm == "33" || comm == "333") {
        
        HWND hwnd = GetDesktopWindow();
        HDC hwindowDC, hwindowCompatibleDC;

        int height, width, srcheight, srcwidth;
        HBITMAP hbwindow;
        Mat src;
        BITMAPINFOHEADER  bi;

        hwindowDC = GetDC(hwnd);
        hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
        SetStretchBltMode(hwindowCompatibleDC, 3);

        RECT windowsize;    
        GetClientRect(hwnd, &windowsize);
        srcheight = windowsize.bottom+250;
        srcwidth = windowsize.right+400;
        height = windowsize.bottom+250;
        width = windowsize.right+400;

        src.create(height, width, CV_8UC3);     

        int iBits = GetDeviceCaps(hwindowDC, 12) * GetDeviceCaps(hwindowDC, 14);
        WORD wBitCount;
        if (iBits <= 1)
            wBitCount = 1;
        else if (iBits <= 4)
            wBitCount = 4;
        else if (iBits <= 8)
            wBitCount = 8;
        else
            wBitCount = 24;
        hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
        bi.biSize = sizeof(BITMAPINFOHEADER);    
        bi.biWidth = width;
        bi.biHeight = -height;  
        bi.biPlanes = 1;
        bi.biBitCount = wBitCount;
        bi.biCompression = 0L;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 256;            
        bi.biClrImportant = 0;

        SelectObject(hwindowCompatibleDC, hbwindow);
        StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, (DWORD)0x00CC0020); 
        GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, 0);  


        DeleteObject(hbwindow);
        DeleteDC(hwindowCompatibleDC);
        ReleaseDC(hwnd, hwindowDC);
        
        
        string savepath = sspath;
        savepath += "newScreenShot";
        savepath += to_string(sscount%nums);
        sscount++;
        if (sscount > nums) sscount = 0;
        savepath += ".jpg";
        imwrite(savepath, src);
        putText(img, "Screenshot saved in destination folder", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
    }
    else {
        putText(img, "COMMAND NOT FOUND", Point(100, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 0));
        namedWindow("command");
        imshow("command", img);
    }
    lastcomm = prevcomm;
    prevcomm = comm;
    comm="";
}

void thresh_callback(int, void*,Mat &img)
{
    Mat canny_output;
    Mat imgBlur;
    Mat imgdil;
    Canny(src_gray, canny_output, thresh, thresh * 2, 3);
    //GaussianBlur(canny_output, imgBlur, Size(3, 3), 3, 0);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(canny_output, imgBlur, kernel);
    vector<vector<Point> > contours;
    findContours(imgBlur, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<Moments> mu(contours.size());
    int mxar = 0;
    int mxi=0;
    for (size_t i = 0; i < contours.size(); i++)
    {
        mu[i] = moments(contours[i]);
        if (mu[i].m00 > mxar) mxi = i;
    }
    
    Mat drawing = Mat::zeros(imgBlur.size(), CV_8UC3);
    bool handDetected = false;
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(255, 255, 255);
        Rect boundRect=boundingRect(contours[mxi]);
        rectangle(img,boundRect.tl(),boundRect.br(),Scalar(255,31,213),2); 
        handDetected = true;
        if (arcLength(contours[i], true) >= 1200 && arcLength(contours[i], true) <= 1500) {
            string ar = to_string(boundRect.area());
            //checkgest(img);
            gestures.push_back(0);
            if (gestures.size() == 21) gestures.erase(gestures.begin());
            putText(img, "Hand detected", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "0 fingers open", Point(300, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "area: "+ar, Point(300, 200), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
        }
        else if (arcLength(contours[i], true) > 1500 && arcLength(contours[i], true) <= 1800) {
            string ar = to_string(boundRect.area());
            //checkgest(img);
            gestures.push_back(1);
            if (gestures.size() == 21) gestures.erase(gestures.begin());
            putText(img, "Hand detected", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "1 finger open", Point(300, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "area: " + ar, Point(300, 200), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
        }
        else if (arcLength(contours[i], true) >= 1900 && arcLength(contours[i], true) <= 2200) {
            string ar = to_string(boundRect.area());
            //checkgest(img);
            gestures.push_back(2);
            if (gestures.size() == 21) gestures.erase(gestures.begin());
            putText(img, "Hand detected", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "2 fingers open", Point(300, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "area: " + ar, Point(300, 200), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
        }
        else if (arcLength(contours[i], true) >= 2300 && arcLength(contours[i], true) <= 2700) {
            string ar = to_string(boundRect.area());
            //checkgest(img);
            gestures.push_back(3);
            if (gestures.size() == 21) gestures.erase(gestures.begin());
            putText(img, "Hand detected", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "3 fingers open", Point(300, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "area: " + ar, Point(300, 200), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
        }
        else if (arcLength(contours[i], true) >= 2700 && arcLength(contours[i], true) <= 3000) {
            string ar = to_string(boundRect.area());
            
            //checkgest(img);
            gestures.push_back(4);
            if (gestures.size() == 21) gestures.erase(gestures.begin());
            putText(img, "Hand detected", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "4 fingers open", Point(300, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "area: " + ar, Point(300, 200), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
        }
        else if (arcLength(contours[i], true) >= 3000 && arcLength(contours[i], true) <= 3600) {
            string ar = to_string(boundRect.area());
            gestures.push_back(5);
            if (gestures.size() == 21) gestures.erase(gestures.begin());
            putText(img, "Hand detected", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "5 fingers open", Point(300, 150), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
            putText(img, "area: " + ar, Point(300, 200), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
        }
        drawContours(drawing, contours, (int)i, color, 2);
    }
    if (!handDetected) {
        gestures.clear();
        putText(img, "Hand out of screen", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
    }
    imshow("Contours", drawing);
}

void setColour(Mat &imgOriginal,Mat &forshow) {
    namedWindow("Control");
    createTrackbar("LowH", "Control", &hLow, 255); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &hHigh, 255);
    createTrackbar("LowS", "Control", &sLow, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &sHigh, 255);
    createTrackbar("LowV", "Control", &vLow, 255); //Value (0 - 255)
    createTrackbar("HighV", "Control", &vHigh, 255);

    
    inRange(imgOriginal, Scalar(hLow, sLow, vLow), Scalar(hHigh, sHigh, vHigh), imgThresh);
    erode(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    dilate(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    imshow("Calibrated", imgThresh);
    imshow("Source", forshow);
}

int landingScreen() {
    system("CLS");
    cout << "Welcome to the Gesture app" << endl;
    cout << "Please choose one of the folowing and enter your choice number" << endl;
    cout << "1: Run the gesture app" << endl;
    cout << "2: Settings" << endl;
    cout << "3: How to use" << endl;
    cout << "4: exit" << endl;
    cout << "Your choice: " << endl;
    int x;
    cin >> x;
    return x;
}

int main(int argc, char** argv)
{
    ifstream fin;
    fin.open("config.txt");
    string temp;
    getline(fin, temp);
    sspath = temp;    
    getline(fin, temp);
    sscount = stoi(temp);
    getline(fin, temp);
    nums = stoi(temp);
    getline(fin, temp);
    hLow = stoi(temp);
    getline(fin, temp);
    hHigh = stoi(temp);
    getline(fin, temp);
    sLow = stoi(temp);
    getline(fin, temp);
    sHigh = stoi(temp);
    getline(fin, temp);
    vLow = stoi(temp);
    getline(fin, temp);
    vHigh = stoi(temp);
    fin.close();

    int choice = landingScreen();
    Mat imgAlt;
    Mat imgOriginal;
    while (choice != 4) {
        if (choice == 1) {
            VideoCapture cap(0); 
            if (!cap.isOpened())  
            {
                cout << "Cannot open the web cam" << endl;
                return -1;
            }
            
            while (true)
            {
                system("CLS");
                cout << "Sit back and relax. You can now use gestures while you work. Press ESC to exit" << endl;
                cout << "See 'how to use' in main menu for assistance" << endl;
                bool aSuccess = cap.read(imgOriginal);
                bool bSuccess = cap.read(imgAlt);
                if (!aSuccess || !bSuccess)
                {
                    cout << "Cannot read a frame from video stream" << endl;
                    break;
                }

                rectangle(imgOriginal, Point(240, 0), Point(640, 600), Scalar(0, 0, 0), -1);

               
                inRange(imgOriginal, Scalar(hLow, sLow, vLow), Scalar(hHigh, sHigh, vHigh), imgThresh);
                erode(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
                dilate(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

                dilate(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
                erode(imgThresh, imgThresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
                
                blur(imgThresh, imgThresh, Size(3, 3));

                src_gray = imgThresh;
                const int max_thresh = 255;
                if (waitKey(50) == 27)
                {
                    cout << "esc key is pressed by user" << endl;
                    destroyAllWindows();
                    choice = landingScreen();
                    break;
                }
                thresh_callback(0, 0, imgAlt);
                if (gestures.size() > 10) { checkgest(); }

                string source_window = "Source";
                namedWindow(source_window);
                imshow(source_window, imgAlt);
            }
        }
        else if (choice == 2) {
            system("CLS");
            cout << "Please choose one of the following:" << endl;
            cout << "1: Change screenshot settings"<<endl;
            cout << "2: Callibrate gesture sensing"<<endl;
            cout << "3: Go to main menu" << endl;
            cout << "Choice: ";
            int c2;
            cin >> c2;
            if (c2 == 1) {
                system("CLS");
                cout << "Please choose one of the following:" << endl;
                cout << "1: Change screenshot save path" << endl;
                cout << "2: Change number of screenshots stored in destination folder" << endl;
                cout << "3: Go to main menu" << endl;
                cout << "Choice: ";
                int c3;
                cin >> c3;
                if (c3 == 1) {
                    system("CLS");
                    cout << "Current destination: " << sspath << endl;
                    cout << "Enter path of detination folder: ";
                    cin >> sspath;
                    cout << "Destination path changed successfully" << endl;
                    cout << "Press any key to go to main menu";
                    getchar(); getchar();
                    destroyAllWindows();
                    choice = landingScreen();
                    if (choice == 4) break;
                    else continue;
                }
                else if (c3 == 2) {
                    system("CLS");
                    cout << "Current limit: " << nums << endl;
                    cout << "Enter the number of screenshots you want to save in the destination folder: ";
                    cin >> nums;
                    cout << "Limit changed successfully" << endl;
                    cout << "Press any key to go to main menu";
                    getchar(); getchar();
                    destroyAllWindows();
                    choice = landingScreen();
                    if (choice == 4) break;
                    else continue;
                }
                else {
                    destroyAllWindows();
                    choice = landingScreen();
                    if (choice == 4) break;
                    else continue;
                }
            }
            else if (c2 == 2) {
                VideoCapture cap(0);
                system("CLS");
                cout << "Three windows will open now. One will show your image as captured from the webcam, One will show" << endl;
                cout << "image as seen by the system. One will be control window. Please move the sliders in control window " << endl;
                cout << "until you can only see your right hand in white colour in the 'Calibrated' window. Press s when you're done" << endl;
                waitKey(10000);
                bool calib = false;
                Mat forshow;
                while (!calib) {
                    
                    bool aSuccess = cap.read(imgOriginal);
                    bool bSuccess = cap.read(forshow);
                    rectangle(imgOriginal, Point(240, 0), Point(640, 600), Scalar(0, 0, 0), -1);
                    setColour(imgOriginal,forshow);
                    if (waitKey(30) == 's')
                    {
                        calib = true;
                        destroyAllWindows();
                    }
                    else continue;
                }
                cout << endl << "Calibration successfull" << endl;
                cout << "press any key to go to main menu" << endl;
                getchar();
                destroyAllWindows();
                choice = landingScreen();
                if (choice == 4) break;
                else continue;
            }
            else if (c2 == 3) {
                destroyAllWindows();
                choice = landingScreen();
                if (choice == 4) break;
            }
        }
        else if (choice == 3) {
        system("CLS");
        cout << "This app supports 5 different gestures" << endl;
        cout << "Show one finger: Increase volume" << endl;
        cout << "Show two fingers: Decrease volume" << endl;
        cout << "Show three fingers: Screenshot is captured and saved in the destination folder" << endl;
        cout << "Show 5 fingers for one sec and immediatly change to closed fist: System recieves shutdown command" << endl;
        cout << "Show 5 fingers for one sec and immediatly change to 1 finger: System recieves restart command" << endl;
        cout << "Show 5 fingers for one sec and immediatly change to 2 fingers: System recieves sleep command" << endl;
        cout << "Enjoy!"<<endl;
        cout << "press any key to go to main menu ";
        getchar();
        getchar();
        destroyAllWindows();
        choice = landingScreen();
        if (choice == 4) break;
        else continue;
        }
    }

    ofstream fout;
    fout.open("config.txt",ios::trunc);
    fout << sspath << endl;
    fout << to_string(sscount) << endl;
    fout << to_string(nums) << endl;
    fout << to_string(hLow) << endl;       
    fout << to_string(hHigh) << endl;    
    fout << to_string(sLow) << endl;    
    fout << to_string(sHigh) << endl;    
    fout << to_string(vLow) << endl;    
    fout << to_string(vHigh) << endl;
    fout.close();

    return 0;
}