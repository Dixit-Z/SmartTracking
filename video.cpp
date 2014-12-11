#include "drone/AtCmd.hpp"
#include "video.hpp"

string winOutputVideo = "Projet ArDrone";
string winRepere	  = "Fenetre Repere";
string winDetected	  = "Image Noir et blanc";

int activate = 0;

void setActivate(int value) {
    activate = value;
}

void drawCross(Mat f, int x, int y, Scalar color) {
    //Dessine petite croix au centre
    line(f, Point(x - 10, y), Point(x + 10, y), color, 2);
    line(f, Point(x, y - 10), Point(x, y + 10), color, 2);
}

void* camera(void* arg) {
    Size fSize;
    VideoCapture cap(0); //capture video

    int vsplit = 55, hsplit = 55;

    if (!cap.isOpened()) {
        cout << "Impossible de lire le flux de la camera" << endl;
        return NULL;
    }
    Mat frame;
    cap >> frame;
    fSize.width = frame.cols;
    fSize.height = frame.rows;


    /*  Choix des valeurs basR et hautR pour correspondre au min et max celon une couleur
    Orange  0-22
    Yellow 22- 38
    Green 38-75
    Blue 75-130
    Violet 130-160
    Red 160-179
    */
    int basR = 0;
    int hautR = 22;

    int basS = 150;
    int hautS = 255;

    int basV = 60;
    int hautV = 255;


    // Initialise les fenetres //
    namedWindow(winDetected, 1);
    namedWindow(winOutputVideo, 1);
    namedWindow(winRepere, 1);
    // Bar pour les reperes
    createTrackbar("VSplit", winRepere, &vsplit, fSize.width / 2);
    createTrackbar("HSplit", winRepere, &hsplit, fSize.height / 2);
    //Bar pour choix de la couleur
    createTrackbar("LowH", winDetected, &basR, 179); //Hue (0 - 179)
    createTrackbar("HighH", winDetected, &hautR, 179);
    //Bar pour Saturation comparer au blanc
    createTrackbar("LowS", winDetected, &basS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", winDetected, &hautS, 255);
    //Bar pour la lumminosite comparer au noir
    createTrackbar("LowV", winDetected, &basV, 255);//Value (0 - 255)
    createTrackbar("HighV", winDetected, &hautV, 255);


    //Cree une image noir de taille de notre image tmp
    Mat imgLines = Mat::zeros(fSize.height, fSize.width, CV_8UC3);

    // YAW
    float roll;
    float pitch;
    float gaz;
    float yaw;
    /*AtCmd::sendMovement(3, 1, 1, 1, 1);
    AtCmd::sendMovement(3, 0, 0, 0, 0);
    AtCmd::sendMovement(3, -1, -1, -1, -1);*/


    while (true) {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // Nouvelle capture



        if (!bSuccess) {
            cout << "Impossible le flux video" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Passe de BGR a HSV

        Mat imgDetection;

        inRange(imgHSV, Scalar(basR, basS, basV), Scalar(hautR, hautS, hautV), imgDetection); //Met en noir les parties non comprit dans notre intervalle pour la balle

        //Retire les petits parasite en fond
        erode(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        dilate(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //Calcule l emplacement de l objet
        Moments position = moments(imgDetection);

        double y = position.m01; //y
        double x = position.m10; //x
        double dZone = position.m00; //z Meme si c est plus la proportion que prend la partie blanche en pixel


        // Regarde les actions a effectuer //
        // si proportion <= 10000 en pixel, l objet n est plus la
        int posX = x / dZone;
        int posY = y / dZone;
        /*
        cout << dZone << endl;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        RNG rng(12345);
        /// Find contours
        findContours(imgDetection, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        for (int i = 0; i< contours.size(); i++) {
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(imgOriginal, contours, i, color, 2, 8, hierarchy, 0, Point());
        }*/

        if (dZone > 10000) {
            roll = pitch = gaz = yaw = 0;

            string Action = "Mouvement a effectuer : ";
            if (posX > fSize.width / 2 + vsplit) {
                Action += "Gauche, ";   yaw = 1;
            } else if (posX < fSize.width / 2 - vsplit) {
                Action += "Droite, ";   yaw = -1;
            } if (posY > fSize.height / 2 + hsplit) {
                Action += "Descendre";  gaz = -1;
            } else if (posY < fSize.height / 2 - hsplit) {
                Action += "Monter";     gaz = 1;
            }
            cout << Action << endl;

            if(activate) {
                AtCmd::sendMovement(3, roll, pitch, gaz, yaw);
            }
        }

        // Genere la fenetre de repere //
        imgLines.setTo(Scalar(255, 255, 255));
        drawCross(imgLines, fSize.width / 2, fSize.height / 2, Scalar(0, 0, 255));
        drawCross(imgLines, posX, posY, Scalar(0, 255, 0));
        line(imgLines, Point(fSize.width / 2 - vsplit, 0), Point(fSize.width / 2 - vsplit, fSize.height), Scalar(0, 0, 255), 1); //VerticaleMin
        line(imgLines, Point(fSize.width / 2 + vsplit, 0), Point(fSize.width / 2 + vsplit, fSize.height), Scalar(0, 0, 255), 1); //VerticaleMax
        line(imgLines, Point(0, fSize.height / 2 - hsplit), Point(fSize.width, fSize.height / 2 - hsplit), Scalar(0, 0, 255), 1); //HorizontaleMin
        line(imgLines, Point(0, fSize.height / 2 + hsplit), Point(fSize.width, fSize.height / 2 + hsplit), Scalar(0, 0, 255), 1); //HorizontaleMax
        imgOriginal = imgOriginal & imgLines; // Croise les resultats à la fenetre de sortie //

        // Affichage des fenetre //
        imshow(winDetected, imgDetection);			//Pour montrer l image avec le masque
        imshow(winRepere, imgLines);				//Pour montrer la fenetre de repere
        imshow(winOutputVideo, imgOriginal);		//Image d origine

        if (waitKey(30) == 27) //Attends que esc soit presser pour quitter
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return NULL;
}

