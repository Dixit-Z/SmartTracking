#include "video.hpp"

ListeObjCoord liste;
string winOutputVideo = "Projet ArDrone";
string winRepere	  = "Fenetre Repere";
string winDetected	  = "Image Noir et blanc";

bool drawing;
bool onDrawing;
Mat imgOriginal;
Mat imgDetection;
Rect rec;
Size fSize(640, 360);
int activate = 0;
int HH, HS, HV, LH, LS, LV;
int posX=0;
int posY=0;
pthread_mutex_t mutexVideo;
IplImage *img;
AVFormatContext *pFormatCtx = NULL;
AVCodecContext  *pCodecCtx;
AVFrame         *pFrame, *pFrameBGR;
uint8_t         *bufferBGR;
SwsContext      *pConvertCtx;
cible Ball(1);

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
	int hasFinished = 0;
	// Tant que des frames sont disponibles
	while (av_read_frame(pFormatCtx, &packet) >= 0 && !drawing) {
		// Decode progressivement l'image
		avcodec_decode_video2(pCodecCtx, pFrame, &hasFinished, &packet);
		// Lorsque l'image est prete //
		if (hasFinished) {
			// Convertie l'image au format BGR //
			pthread_mutex_lock(&mutexVideo);
			sws_scale(pConvertCtx, (const uint8_t *const *) pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameBGR->data, pFrameBGR->linesize);
			pthread_mutex_unlock(&mutexVideo);
			av_free_packet(&packet);
		}
		av_free_packet(&packet);
	}
	return NULL;
}

void* camera(void* arg) {
	//pFormatCtx=(AVFormatContext *)arg;
	char key;
	drawing=false;
	Ball.roll = Ball.pitch = Ball.gaz = Ball.yaw = 0;
	pthread_mutex_init(&mutexVideo, NULL);
	liste.suivant=NULL;
#if output_video == ov_remote_ffmpeg
	pthread_t ii;
	pthread_create(&ii, NULL, getimg, NULL);
#else	
	VideoCapture cap(0); //capture video webcam
#endif



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

	// Initialise les fenetres //
	namedWindow(winDetected, 1);
	namedWindow(winOutputVideo, 1);

	//Cree une image noir de taille de notre image tmp
	Mat imgLines = Mat::zeros(fSize.height, fSize.width, CV_8UC3);

	// POSITIONS DRONE
	/*float roll;
	float pitch;
	float gaz;
	float yaw;*/

	while (true) {

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
		pthread_t mtId,ocId;
		pthread_create(&mtId, NULL, &matchTemplate, NULL);
		pthread_create(&ocId, NULL, &opencv, NULL);
		
		pthread_join(mtId,NULL);
		pthread_join(ocId,NULL);
		Ball.setRealPos();
		//Ball.getRealPos();
		// Genere la fenetre de repere //
		imgLines.setTo(Scalar(255, 255, 255));
		drawCross(imgLines, fSize.width / 2, fSize.height / 2, Scalar(0, 0, 255));
		drawCross(imgLines, posX, posY, Scalar(0, 255, 0));

		imgOriginal = imgOriginal & imgLines; // Croise les resultats à la fenetre de sortie //

		// Affichage des fenetre //
		imshow(winDetected, imgDetection);			//Pour montrer l image avec le masque
		//imshow(winRepere, imgLines);				//Pour montrer la fenetre de repere
		imshow(winOutputVideo, imgOriginal);		//Image d origine
		//cout << "apres affichage fenetre" << endl;
		string Action = "Mouvement a effectuer : ";
		ObjCoord tmp = Ball.getRealPos();
		//cout << "x " << tmp.Xcoord << " y " << tmp.Ycoord << " z " << tmp.Zcoord << endl;
		if(tmp.Zcoord == -1){
			Action += "Recule, "; Ball.pitch = 0.1f;
		}else if(tmp.Zcoord == 1){
			Action += "Avance, "; Ball.pitch = -0.1f;
		}
		if (tmp.Xcoord <= 25 && tmp.Xcoord != 0) {
			Ball.yaw = 0.1f;
			Action += "Gauche ("+ to_string(Ball.yaw)+"%), ";
		} else if (tmp.Xcoord >= 75) {
			Ball.yaw = -0.1f;
			Action += "Droite ("+ to_string(Ball.yaw)+"%), ";
		} if (tmp.Ycoord >= 75) {
			Action += "Descendre";  Ball.gaz = -0.1f;
		} else if (tmp.Ycoord <= 25 && tmp.Ycoord != 0) {
			Action += "Monter";    Ball.gaz = 0.1f;
		}
		if(Ball.pitch != 0) {
			Ball.roll = Ball.yaw / 2;
			Ball.yaw = 0;
		}
		if(tmp.Xcoord == 0 && tmp.Ycoord == 0 && tmp.Zcoord == 0)
			Ball.roll = Ball.pitch = Ball.gaz = Ball.yaw = 0;
		cout << Action << endl;
		AtCmd::sendMovement(3, Ball.roll, Ball.pitch, Ball.gaz, Ball.yaw);
		key=waitKey(10);
		if(key == 10)
		{
			enVol=true;
			key=-1;
		}
		else if (key != -1) //Attends qu'une touche soit presser pour quitter
		{
			break;
		}
/* TRAITEMENT DE L'INFORMATION DES DEUX METHODES DE TRACKING */
/* FIN DU TRAITEMENT DES INFOS DE TRACKING */
	}
	stopTracking=true;
	destroyAllWindows();
	return NULL;
}
void ajoutListe(ListeObjCoord *liste, ObjCoord newNoeud)
{
	int i=0;
	ListeObjCoord *tamp=liste;
	liste->noeud=newNoeud;
	liste=liste->suivant;
	tamp=tamp->suivant;
	if(tamp!=NULL)
	{
		while(tamp->suivant!=NULL && i++<liste->nbNoeud)
		{
			tamp=tamp->suivant;
			liste->noeud=tamp->noeud;
			liste=liste->suivant;
		}
		if(i==liste->nbNoeud)
			liste->suivant=NULL;
	}
}

void setAVFormatContext(AVFormatContext * AVFormat)
{
	pFormatCtx=AVFormat;
}
void *drawingAndParam(void * arg)
{
	string winParametrage = "Thresholded";
	string winDetected = "Parametrages";
	char key;
	drawing = false;
	onDrawing = true;
	pthread_mutex_init(&mutexVideo, NULL);
#if output_video == ov_remote_ffmpeg
	int errorcode = avformat_open_input(&pFormatCtx, "tcp://192.168.1.1:5555", NULL, NULL);
	if (errorcode < 0) {
		cout << "ERREUR CAMERA DRONE!!!" << errorcode;
		return 0;
	}
	avformat_find_stream_info(pFormatCtx, NULL);
	av_dump_format(pFormatCtx, 0, "tcp://192.168.1.1:5555", 0);
	pCodecCtx = pFormatCtx->streams[0]->codec;
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		cout << "ERREUR avcodec_find_decoder!!!";
		return 0;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		cout << "ERREUR avcodec_open2!!!";
		return 0;
	}
	//pFrame = av_frame_alloc();
	//pFrameBGR = av_frame_alloc();
	pFrame = avcodec_alloc_frame();
	pFrameBGR = avcodec_alloc_frame();
	bufferBGR = (uint8_t*)av_mallocz(avpicture_get_size(PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height) * sizeof(uint8_t));
	avpicture_fill((AVPicture*)pFrameBGR, bufferBGR, PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);
	pConvertCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_BGR24, SWS_SPLINE, NULL, NULL, NULL);
	img = cvCreateImage(cvSize(pCodecCtx->width, (pCodecCtx->height == 368) ? 360 : pCodecCtx->height), IPL_DEPTH_8U, 3);
	if (!img) {
		cout << "ERREUR PAS D'IMAGE!!!";
		return 0;
	}

	pthread_t ii;
	pthread_create(&ii, NULL, getimg, NULL);

#else	
	VideoCapture cap(0); //capture video webcam

#endif
	HH=179;LS=1;HS=255;LV=1;HV=255;LH=1;
	//Mat frame;
	namedWindow(winDetected, CV_WINDOW_NORMAL);
	Mat frame;
	setMouseCallback(winDetected, MouseCallBack, NULL);
	while(true)
	{	
		if(onDrawing) //Tant que l'utilisateur ne commence pas la sélection!
		{
			#if output_video != ov_remote_ffmpeg
				bool bSuccess = cap.read(frame); // Nouvelle capture
			if (!bSuccess) {
				cout << "Impossible de lire le flux video" << endl;
				break;
			}
			#else
				pthread_mutex_lock(&mutexVideo);
				memcpy(img->imageData, pFrameBGR->data[0], pCodecCtx->width * ((pCodecCtx->height == 368) ? 360 : pCodecCtx->height) * sizeof(uint8_t) * 3);
				pthread_mutex_unlock(&mutexVideo);
				frame = cv::cvarrToMat(img, true);
			#endif
		imshow(winDetected, frame);
		}
		if(!onDrawing && !drawing) //On affiche en direct la sélection de l'utilisateur
		{
			Mat tmpFrame=frame.clone();
			rectangle(tmpFrame, rec, CV_RGB(51,156,204),1,8,0);
			imshow(winDetected, tmpFrame);
		}
		if(drawing) //L'utilisateur a fini de sélectionner
		{
			//cible Ball(1);
			namedWindow(winParametrage, CV_WINDOW_NORMAL);
			setMouseCallback(winDetected, NULL, NULL);	
			rectangle(frame, rec, CV_RGB(51,156,204),2,8,0);
			imshow(winDetected, frame);
			Mat selection = frame(rec);
			Ball.setPicture(selection);
			while(key != 'q')
			{
				//Bar pour choix de la couleur
				createTrackbar("LowH", winParametrage, &LH, 179); //Hue (0 - 179)
				createTrackbar("HighH", winParametrage, &HH, 179);
				//Bar pour Saturation comparer au blanc
				createTrackbar("LowS", winParametrage, &LS, 255); //Saturation (0 - 255)
				createTrackbar("HighS", winParametrage, &HS, 255);
				//Bar pour la lumminosite comparer au noir
				createTrackbar("LowV", winParametrage, &LV, 255);//Value (0 - 255)
				createTrackbar("HighV", winParametrage, &HV, 255);
				Mat imgHSV;

				cvtColor(selection, imgHSV, COLOR_BGR2HSV); //Passe de BGR a HSV

				Mat imgDetection;

				inRange(imgHSV, Scalar(LH, LS, LV), Scalar(HH, HS, HV), imgDetection); //Met en noir les parties non comprit dans notre intervalle pour la balle

				//Retire les petits parasite en fond
				erode(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				dilate(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				dilate(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				erode(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				imshow(winParametrage, imgDetection);
				Moments position;
				position = moments(imgDetection);
				Ball.lastdZone = position.m00;
				key = waitKey(10);
			}
				
			Mat graySelect;
			int minHessian = 800;
			cvtColor(selection, graySelect, COLOR_BGR2GRAY);
			Ptr<SURF> detector = SURF::create(minHessian);
			vector<KeyPoint> KP;
			detector->detect(graySelect, KP);
			Mat KPimg;
			drawKeypoints(graySelect, KP, KPimg, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
			Mat desc;
			Ptr<SURF> extractor = SURF::create();
			extractor->compute(graySelect, KP, desc);
			Ball.setimgGray(graySelect);
			Ball.setKP(KP);
			Ball.setDesc(desc);
			
			/*while(key!='z')
			{
				imshow("PTS INTERETS", KPimg);
				key = waitKey(10);
			}*/

			break;
		}
		key = waitKey(10);
	}

	destroyAllWindows();
#if output_video != ov_remote_ffmpeg
	cap.release();
#endif
}
void MouseCallBack(int event, int x , int y , int flags, void* userdata)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		rec = Rect(x, y, 0, 0);
		onDrawing = false;
	}
	else if (event == CV_EVENT_LBUTTONUP) 	
	{
			rec = Rect(rec.x, rec.y, x-rec.x, y-rec.y);
			drawing = true;
	}
	else if(event == CV_EVENT_MOUSEMOVE && !onDrawing)
	{
		rec = Rect(rec.x, rec.y, x-rec.x, y-rec.y);
	}
}
void *matchTemplate(void * args)
{
	/*
	   DEBUT TRAITEMENT MATCHTEMPLATE
	 */
	Mat result(imgOriginal.cols - Ball.getPicture().cols + 1, imgOriginal.rows - Ball.getPicture().rows + 1, CV_32FC1);
	matchTemplate(imgOriginal, Ball.getPicture(), result, CV_TM_SQDIFF_NORMED);
	//normalize(result, result, -1, 1, NORM_MINMAX, -1, Mat());
	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	//Rect target = Rect(maxLoc.x, maxLoc.y, Ball.getPicture().cols, Ball.getPicture().rows);
	Rect target = Rect(minLoc.x, minLoc.y, Ball.getPicture().cols, Ball.getPicture().rows);
	//if(maxVal>0.8)
	//
	/*if(minVal<0.076)
		rectangle(imgOriginal, target, Scalar::all(-1), 2, 8, 0);*/
	/*
	   FIN TRAITEMENT MATCHTEMPLATE
	 */


	/*
		DEBUT TRAITEMENT KEYPOINTS
	 */

	Mat imgGray1;
	int minHessian = 400;
	cvtColor(imgOriginal(target), imgGray1, COLOR_BGR2GRAY);
	Ptr<SURF> detector = SURF::create(minHessian);
	vector<KeyPoint> keypoints_f1;
	detector->detect(imgGray1, keypoints_f1);
	Mat img_kp1;
	//drawKeypoints(imgGray1, keypoints_f1, img_kp1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	Ptr<SURF> extractor = SURF::create();
	Mat desc1;	
	extractor->compute(imgGray1, keypoints_f1, desc1);
	//BFMatcher matcher(NORM_L2);
	//vector<DMatch> matches;
	//matcher.match(desc1, Ball.getDesc(),matches);
	//Mat img_matches;
	//drawMatches(imgGray1, keypoints_f1, Ball.getimgGray(), Ball.getKP(), matches, img_matches);
	//imshow("test", img_matches);
	/*
	   FIN TRAITEMENT TLD
	 */
	if(minVal<0.15 && keypoints_f1.size()>=Ball.getKP().size())
	{
		rectangle(imgOriginal, target, Scalar::all(-1), 2, 8, 0);
		Ball.setCurrentTLD((float)(target.x+(target.width/2))/fSize.width*100, (float)(target.y+(target.height/2))/fSize.height*100, 0);
	}
	else
	{
		Ball.setFoundTLD(false);
	}
	return NULL;
}
void *opencv(void * args)
{
	/*
	   DEBUT TRAITEMENT OPENCV
	 */
	
	Mat imgHSV;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Passe de BGR a HSV

	inRange(imgHSV, Scalar(LH, LS, LV), Scalar(HH, HS, HV), imgDetection); //Met en noir les parties non comprit dans notre intervalle pour la balle

	//Retire les petits parasite en fond
	erode(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	dilate(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgDetection, imgDetection, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	int i, nlabels;
	Rect box;
	int maxArea=0;
	Mat labels;
	Mat centroids;
	Mat stats;
	nlabels=connectedComponentsWithStats(imgDetection, labels, stats, centroids, 4, CV_32S);

	for(i=1; i<(int)nlabels;i++)
	{
		int *row = (int *) &stats.at<int>(i,0);
		//printf("i : %d, mon area %d vs %d max \n", i, row[CC_STAT_AREA], maxArea);
		if(row[CC_STAT_AREA]>maxArea)
		{
			box = Rect(row[CC_STAT_LEFT], row[CC_STAT_TOP], row[CC_STAT_WIDTH], row[CC_STAT_HEIGHT]);
			maxArea=row[CC_STAT_AREA];
		}
	}
	Moments position;
	//Si la composante connexe n'est pas assez grande ce n'est pas l'objet
	if(maxArea>400)
	{
		Ball.setFoundCV(true);
		rectangle(imgOriginal, box, Scalar(0,255,0), 4, 8, 0);

		//Calcule l emplacement de l objet
		position = moments(imgDetection(box));

		double y = position.m01; //y
		double x = position.m10; //x
		double dZone = position.m00; //z
		cout << "dZone " << dZone << endl;

		// Regarde les actions a effectuer //
		posX = x / dZone;
		posY = y / dZone;
		posX+=box.x;//imgOriginal.cols - box.x;
		posY+=box.y;//imgOriginal.rows - box.y;
		

		int posZ;
		if(dZone>Ball.lastdZone)
		{
			posZ=-1;
		}
		else if(dZone > Ball.lastdZone-800 || dZone < Ball.lastdZone+800)
		{
			 posZ=0;
		}
		else
		{
			posZ=1;
		}
		 Ball.setCurrentCV((float)posX/fSize.width*100,(float)posY/fSize.height*100, posZ);
	}
	else
	{
		if(activate) {//On passe ici quand la zone détectée est trop petite ou que l'on en détecte pas.
			//AtCmd::sendMovement(0, 0, 0, 0, 0); // CHANGE
		}
		Ball.setFoundCV(false);
	}

	/*
	   FIN TRAITEMENT OPENCV
	 */
	return NULL;
}

