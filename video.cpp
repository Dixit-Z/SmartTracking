#include "drone/AtCmd.hpp"
#include "video.hpp"

#define ov_local 1
#define ov_remote_opencv 2
#define ov_remote_ffmpeg 4
#define output_video ov_remote_ffmpeg

// FFmpeg
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

string winOutputVideo = "Projet ArDrone";
string winRepere	  = "Fenetre Repere";
string winDetected	  = "Image Noir et blanc";

int activate = 0;

pthread_mutex_t mutexVideo;
// Camera image
IplImage *img;
AVFormatContext *pFormatCtx;
AVCodecContext  *pCodecCtx;
AVFrame         *pFrame, *pFrameBGR;
uint8_t         *bufferBGR;
SwsContext      *pConvertCtx;

void setActivate(int value) {
    activate = value;
}

void drawCross(Mat f, int x, int y, Scalar color) {
    //Dessine petite croix au centre
    line(f, Point(x - 10, y), Point(x + 10, y), color, 2);
    line(f, Point(x, y - 10), Point(x, y + 10), color, 2);
}

void* getimg(void* arg) {
    AVPacket packet;
    int frameFinished = 0;

    // Read all frames
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Decode the frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        // Decoded all frames
        if (frameFinished) {
            // Convert to BGR
            pthread_mutex_lock(&mutexVideo);
            sws_scale(pConvertCtx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameBGR->data, pFrameBGR->linesize);
            pthread_mutex_unlock(&mutexVideo);

            // Free the packet and break immidiately
            av_free_packet(&packet);
            //break;
        }

        // Free the packet
        av_free_packet(&packet);
    }
}

void* camera(void* arg) {
    pthread_mutex_init(&mutexVideo, NULL);
#if output_video == ov_remote_ffmpeg
    if (avformat_open_input(&pFormatCtx, "tcp://192.168.1.1:5555", NULL, NULL) < 0) {
        cout << "ERREUR !!!";
        return 0;
    }
    // Retrive and dump stream information
    avformat_find_stream_info(pFormatCtx, NULL);
    av_dump_format(pFormatCtx, 0, "tcp://192.168.1.1:5555", 0);

    // Find the decoder for the video stream
    pCodecCtx = pFormatCtx->streams[0]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        cout << "ERREUR !!!";
        return 0;
    }
    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        cout << "ERREUR !!!";
        return 0;
    }
    // Allocate video frames and a buffer
#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(55,28,1)
        pFrame = av_frame_alloc();
        pFrameBGR = av_frame_alloc();
#else
    pFrame = avcodec_alloc_frame();
    pFrameBGR = avcodec_alloc_frame();
#endif
    bufferBGR = (uint8_t*)av_mallocz(avpicture_get_size(PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height) * sizeof(uint8_t));
    // Assign appropriate parts of buffer to image planes in pFrameBGR
    avpicture_fill((AVPicture*)pFrameBGR, bufferBGR, PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);
    // Convert it to BGR
    pConvertCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_BGR24, SWS_SPLINE, NULL, NULL, NULL);
    // Allocate an IplImage
    img = cvCreateImage(cvSize(pCodecCtx->width, (pCodecCtx->height == 368) ? 360 : pCodecCtx->height), IPL_DEPTH_8U, 3);
    if (!img) {
        cout << "ERREUR !!!";
        return 0;
    }

    pthread_t ii;
    pthread_create(&ii, NULL, getimg, NULL);

#elif output_video == ov_local
    //VideoCapture cap("tcp://192.168.1.1:5555"); //capture video
    VideoCapture cap(0); //capture video
#else
    VideoCapture cap("tcp://192.168.1.1:5555"); //capture video
#endif

    Size fSize(640, 360);

    int vsplit = 55, hsplit = 55;


#if output_video != ov_remote_ffmpeg
    if (!cap.isOpened()) {
        cout << "Impossible de lire le flux de la camera" << endl;
        return NULL;
    }
    Mat frame;
    cap >> frame;
    fSize.width = frame.cols;
    fSize.height = frame.rows;
#endif

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

    while (true) {
        Mat imgOriginal;

#if output_video != ov_remote_ffmpeg
        bool bSuccess = cap.read(imgOriginal); // Nouvelle capture
        if (!bSuccess) {
            cout << "Impossible de lire le flux video" << endl;
            break;
        }
#else
        pthread_mutex_lock(&mutexVideo);
        memcpy(img->imageData, pFrameBGR->data[0], pCodecCtx->width * ((pCodecCtx->height == 368) ? 360 : pCodecCtx->height) * sizeof(uint8_t) * 3);
        pthread_mutex_unlock(&mutexVideo);
        imgOriginal = cv::cvarrToMat(img, true);
#endif

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

        if (dZone > 1000) {
            roll = pitch = gaz = yaw = 0;

            string Action = "Mouvement a effectuer : ";
	    if(dZone > 30000){
	    	Action += "Recule, ";
	    }else{
	    	Action += "Avance, ";
	    
	    }
            if (posX > fSize.width / 2 + vsplit) {
                Action += "Gauche, ";   yaw = 0.25f;
            } else if (posX < fSize.width / 2 - vsplit) {
                Action += "Droite, ";   yaw = -0.25f;
            } if (posY > fSize.height / 2 + hsplit) {
                Action += "Descendre";  gaz = -0.25f;
            } else if (posY < fSize.height / 2 - hsplit) {
                Action += "Monter";     gaz = 0.25f;
            }
            cout << Action << endl;

            if(activate) {
                AtCmd::sendMovement(0, roll, pitch, gaz, yaw);
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

