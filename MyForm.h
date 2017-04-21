#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

#pragma once


namespace ���յ��� {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	//--
	using namespace std;
	using namespace cv;
	double gRemoveSize, gBigObj;
	int gProtectSize = 51;
	int paint_size=100;
	int scollbarMax=350;
	int scollbarMin=20;
	int DUBBLECLICK = 0;//�ݬO�����Ҧ��٬O�@��Ҧ�
	int modeDC = 0;//�ݬO��j�Ҧ��٬O�@��j�p�Ҧ�

	string imgName;
	string saveimgName;
	Mat original;
	Mat RedPic;
	Mat img;
	Mat protectRegion;

	//�������
	Mat RedPicCPY;
	Mat ProtectRegionCPY;
	Mat BinaryMapForUI;
	Mat BinaryMapForProtectMap;
	Mat BinaryMapForUPsample;
	Mat RedPicCPY_forUPSAMPLEeraser;

	//��j��
	Mat RedPicCPY_forUPSAMPLE;
	Mat ROI;
	
	
	
	
	
	void mymouse(int event, int x, int y, int flag, void* param)
	{
		static int oldx, oldy, DownX, DownY, oldDownX, oldDownY, now_down = 0, eraserDown, ROIxBEGIN, ROIxEND, ROIyBEGIN, ROIyEND;
		cout << x << "," << y << endl;
		//����������j����
		if (event == CV_EVENT_LBUTTONDBLCLK && DUBBLECLICK == 1){

			modeDC = 1;
			ROIxBEGIN = x * 4 - RedPicCPY_forUPSAMPLE.cols / 4;
			ROIxEND = x * 4 + RedPicCPY_forUPSAMPLE.cols / 4;
			ROIyBEGIN = y * 4 - RedPicCPY_forUPSAMPLE.rows / 4;
			ROIyEND = y * 4 + RedPicCPY_forUPSAMPLE.rows / 4;

			if (ROIxBEGIN < 0) ROIxBEGIN = 0;
			else if (ROIxEND > RedPicCPY_forUPSAMPLE.cols)ROIxBEGIN = RedPicCPY_forUPSAMPLE.cols / 2 - 1;

			if (ROIyBEGIN< 0) ROIyBEGIN = 0;
			else if (ROIyEND > RedPicCPY_forUPSAMPLE.rows - 1)ROIyBEGIN = RedPicCPY_forUPSAMPLE.rows / 2 - 1;

			ROI = RedPicCPY_forUPSAMPLE(Rect(ROIxBEGIN, ROIyBEGIN, RedPicCPY_forUPSAMPLE.cols / 2 - 1, RedPicCPY_forUPSAMPLE.rows / 2 - 1));
			DUBBLECLICK = 0;
			imshow("RedPic", ROI);
		}
		//�k�������Y�p����
		else if (event == CV_EVENT_RBUTTONDBLCLK&&DUBBLECLICK == 1){
			modeDC = 0;
			DUBBLECLICK = 0;
		}

		//����e�u����
		else if (event == CV_EVENT_LBUTTONDOWN){
			//�Y�b�@��Ҧ��U
			if (modeDC == 0 && DUBBLECLICK == 0){
				circle(RedPic, cvPoint(x, y), paint_size / 2, cvScalar(0, 0, 255));
				circle(RedPicCPY_forUPSAMPLE, cvPoint(x * 4, y * 4), paint_size * 2, cvScalar(0, 0, 255));
				oldx = x; oldy = y;
				now_down = 1;
			}

			//�Y�b��j�Ҧ��U
			else if (modeDC == 1){
				DownX = ROIxBEGIN + x;
				DownY = ROIyBEGIN + y;
				circle(ROI, cvPoint(x, y), paint_size * 2, cvScalar(0, 0, 255));
				circle(RedPic, cvPoint(DownX / 4, DownY / 4), paint_size / 2, cvScalar(0, 0, 255));
				oldx = x; oldy = y;
				oldDownX = DownX; oldDownY = DownY;
				now_down = 1;
			}
		}
		else if (event == CV_EVENT_LBUTTONUP){
			now_down = 0;
		}
		else if (event == CV_EVENT_MOUSEMOVE && now_down == 1){
			//�Y�b�@��Ҧ��U
			if (modeDC == 0 && DUBBLECLICK == 0){
				line(RedPic, cvPoint(x, y), cvPoint(oldx, oldy), cvScalar(0, 255, 0), paint_size, 8, 0);
				line(protectRegion, cvPoint(x * 4, y * 4), cvPoint(oldx * 4, oldy * 4), cvScalar(255), paint_size * 4, 8, 0);
				line(RedPicCPY_forUPSAMPLE, cvPoint(x * 4, y * 4), cvPoint(oldx * 4, oldy * 4), cvScalar(0, 255, 0), paint_size * 4, 8, 0);
				oldx = x; oldy = y;

			}
			//�Y�b��j�Ҧ��U
			if (modeDC == 1){
				DownX = ROIxBEGIN + x;
				DownY = ROIyBEGIN + y;
				line(ROI, cvPoint(x, y), cvPoint(oldx, oldy), cvScalar(0, 255, 0), paint_size * 2, 8, 0);
				line(RedPic, cvPoint(DownX / 4, DownY / 4), cvPoint(oldDownX / 4, oldDownY / 4), cvScalar(0, 255, 0), paint_size / 2, 8, 0);
				line(protectRegion, cvPoint(DownX, DownY), cvPoint(oldDownX, oldDownY), cvScalar(255), paint_size * 2, 8, 0);

				oldx = x; oldy = y;
				oldDownX = DownX; oldDownY = DownY;

			}
		}

		//�k��ؿﳡ��
		else if (event == CV_EVENT_RBUTTONDOWN){
			//�Y�b�@��Ҧ��U
			if (DUBBLECLICK == 0 && modeDC == 0){
				oldx = x; oldy = y;
			}
			//�Y�b��j�Ҧ��U
			else if (modeDC == 1){
				DownX = ROIxBEGIN + x;
				DownY = ROIyBEGIN + y;
				oldDownX = DownX; oldDownY = DownY;
				oldx = x; oldy = y;
			}

		}
		else if (event == CV_EVENT_RBUTTONUP){
			//�Y�b�@��Ҧ��U
			if (DUBBLECLICK == 0 && modeDC == 0){
				rectangle(RedPic, Rect(cv::Point(oldx, oldy), cvPoint(x, y)), Scalar(0, 255, 0), -1);
				rectangle(protectRegion, Rect(cv::Point(oldx * 4, oldy * 4), cvPoint(x * 4, y * 4)), Scalar(255), -1);
				rectangle(RedPicCPY_forUPSAMPLE, Rect(cv::Point(oldx * 4, oldy * 4), cvPoint(x * 4, y * 4)), Scalar(0, 255, 0), -1);
			}
			//�Y�b��j�Ҧ��U
			else if (modeDC == 1){
				DownX = ROIxBEGIN + x;
				DownY = ROIyBEGIN + y;

				rectangle(ROI, Rect(cv::Point(oldx, oldy), cvPoint(x, y)), Scalar(0, 255, 0), -1);
				rectangle(RedPic, Rect(cv::Point(oldDownX / 4, oldDownY / 4), cvPoint(DownX / 4, DownY / 4)), Scalar(0, 255, 0), -1);
				rectangle(protectRegion, Rect(cv::Point(oldDownX, oldDownY), cvPoint(DownX, DownY)), Scalar(255), -1);

				oldDownX = DownX; oldDownY = DownY;
				oldx = x; oldy = y;
			}
		}

		//������������
		else if (event == CV_EVENT_MBUTTONDOWN){
			eraserDown = 1;
			//�Y�b�@��Ҧ��U
			if (DUBBLECLICK == 0 && modeDC == 0){
				oldx = x; oldy = y;

			}
			//�Y�b��j�Ҧ��U
			else if (modeDC == 1){
				DownX = ROIxBEGIN + x;
				DownY = ROIyBEGIN + y;
				oldDownX = DownX; oldDownY = DownY;
				oldx = x; oldy = y;
			}
		}
		else if (event == CV_EVENT_MBUTTONUP&&DUBBLECLICK == 0){
			eraserDown = 0;
			//��binaryMap�k�s
			Mat TempForBinaryMap(RedPic.rows, RedPic.cols, CV_8UC1, Scalar(0));
			Mat TempForBinaryMap_protectVER(original.rows, original.cols, CV_8UC1, Scalar(0));
			Mat TempForBinaryMap_Upsample(original.rows, original.cols, CV_8UC1, Scalar(0));
			BinaryMapForUI = TempForBinaryMap.clone();
			BinaryMapForProtectMap = TempForBinaryMap_protectVER.clone();
			BinaryMapForUPsample = TempForBinaryMap_Upsample.clone();
		}
		else if (event == CV_EVENT_MOUSEMOVE && eraserDown == 1){
			//�Y�b�@��Ҧ��U
			if (DUBBLECLICK == 0 && modeDC == 0){
				line(BinaryMapForUI, cvPoint(x, y), cvPoint(oldx, oldy), cvScalar(1), paint_size, 8, 0);
				line(BinaryMapForProtectMap, cvPoint(x * 4, y * 4), cvPoint(oldx * 4, oldy * 4), cvScalar(1), paint_size * 4, 8, 0);
				line(BinaryMapForUPsample, cvPoint(x * 4, y * 4), cvPoint(oldx * 4, oldy * 4), cvScalar(1), paint_size * 4, 8, 0);
				oldx = x; oldy = y;
			}

			//�Y�b��j�Ҧ��U
			if (modeDC == 1){
				DownX = ROIxBEGIN + x;
				DownY = ROIyBEGIN + y;

				line(BinaryMapForUPsample, cvPoint(DownX, DownY), cvPoint(oldDownX, oldDownY), cvScalar(1), paint_size * 2, 8, 0);
				line(BinaryMapForUI, cvPoint(DownX / 4, DownY / 4), cvPoint(oldDownX / 4, oldDownY / 4), cvScalar(1), paint_size / 2, 8, 0);
				line(BinaryMapForProtectMap, cvPoint(DownX, DownY), cvPoint(oldDownX, oldDownY), cvScalar(1), paint_size * 2, 8, 0);

				oldDownX = DownX; oldDownY = DownY;
				oldx = x; oldy = y;
			}


			for (int i = 0; i < BinaryMapForProtectMap.rows; i++)
			for (int j = 0; j < BinaryMapForProtectMap.cols; j++){
				if ((int)BinaryMapForProtectMap.at<uchar>(i, j) == 1)//�O�@�ϹϪ�������^�_
					protectRegion.at<uchar>(i, j) = ProtectRegionCPY.at<uchar>(i, j);

				if ((int)BinaryMapForUI.at<uchar>(i / 4, j / 4) == 1)//�����ϹϪ�������^�_
					RedPic.at<Vec3b>(i / 4, j / 4) = RedPicCPY.at<Vec3b>(i / 4, j / 4);

				if ((int)BinaryMapForUPsample.at<uchar>(i, j) == 1){//��j�ϹϪ�������^�_
					RedPicCPY_forUPSAMPLE.at<Vec3b>(i, j) = RedPicCPY_forUPSAMPLEeraser.at<Vec3b>(i, j);
				}
			}
		}

		if (modeDC == 1){
			DUBBLECLICK = 0;
			imshow("RedPic", ROI);
		}
		else{
			DUBBLECLICK = 0;
			imshow("RedPic", RedPic);
		}

	}
	//--
	/// <summary>
	/// MyForm ���K�n
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  �b���[�J�غc�禡�{���X
			//
		}

	protected:
		/// <summary>
		/// �M������ϥΤ����귽�C
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::HScrollBar^  hScrollBar1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Button^  button2;
	protected:

	public:
		/// <summary>
		/// �]�p�u��һݪ��ܼơC
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����]�p�u��䴩�һݪ���k - �ФŨϥε{���X�s�边
		/// �ק�o�Ӥ�k�����e�C
		/// </summary>
		
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Start";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker1_DoWork);
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->Location = System::Drawing::Point(200, 46);
			this->hScrollBar1->Maximum = 350;
			scollbarMax = this->hScrollBar1->Maximum;
			this->hScrollBar1->Minimum = 20;
			scollbarMin = this->hScrollBar1->Minimum;
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(192, 19);
			this->hScrollBar1->TabIndex = 1;
			this->hScrollBar1->Value = 100;
			this->hScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &MyForm::hScrollBar1_Scroll);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(118, 53);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(33, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"label1";
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(120, 12);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(386, 25);
			this->progressBar1->TabIndex = 3;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(13, 41);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Help";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(518, 85);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->hScrollBar1);
			this->Controls->Add(this->button1);
			this->Cursor = System::Windows::Forms::Cursors::Cross;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->Text = L" ";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				
				// openFileDialog1->Multiselect = true;
				 if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 try{
						 Image^ image = Image::FromFile(openFileDialog1->FileName);
					 }
					 catch (...)
					 {
						 MessageBox::Show("Unable to open this file!\nPlease choose another one!", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Error);
						 return;
					 }
					 using namespace Runtime::InteropServices;
					 const char* chars =
						 (const char*)(Marshal::StringToHGlobalAnsi(openFileDialog1->FileName)).ToPointer();
					 imgName = chars;
					 Marshal::FreeHGlobal(IntPtr((void*)chars));
					 //imgName = Convert::ToString( openFileDialog1->FileName);
					 backgroundWorker1->RunWorkerAsync();
				 }
				
	}
	private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 original = imread(imgName, 0);
				 //RedPic = new cv::Mat(original.rows, original.cols, CV_8UC3, Scalar(255, 255, 255));
				 RedPic = cv::Mat(original.rows, original.cols, CV_8UC3, Scalar(255, 255, 255));

				 img = imread(imgName, 0);
				 protectRegion = ~img.clone();
				 progressBar1->Value = 15;
				 img = ~img;//���ܥ�
				 gRemoveSize = ((double)80 / (6155 * 6435)) * (img.rows * img.cols);
				 gBigObj = ((double)500000 / (6155 * 6435)) * (img.rows * img.cols);


				 Mat labelImage, stats, centroids;
				 int nLabels = connectedComponentsWithStats(img, labelImage, stats, centroids, 8, CV_32S);
				 progressBar1->Value = 30;

				 //begin for copy
				 int label;
				 for (int h = 0; h < img.rows; h++)
				 for (int w = 0; w < img.cols; w++) {
					 label = labelImage.at<int>(h, w);

					 if (stats.at<int>(label, CC_STAT_AREA) < gRemoveSize || stats.at<int>(label, CC_STAT_AREA) > gBigObj)
						 protectRegion.at<uchar>(h, w) = 0;

				 } // for
				 progressBar1->Value = 40;


				 Mat kernel = getStructuringElement(MORPH_ELLIPSE, cv::Size(gProtectSize, gProtectSize));

				 dilate(protectRegion, protectRegion, kernel, cv::Point(-1, -1), 1);

				 progressBar1->Value = 70;
				 bool * protectLabel = new bool[nLabels];
				 for (int i = 0; i < nLabels; i++) protectLabel[i] = false;

				 //------
				 for (int h = 0; h < img.rows; h++)
				 for (int w = 0; w < img.cols; w++)
				 if (protectRegion.at<uchar>(h, w) != 0)
					 protectLabel[labelImage.at<int>(h, w)] = true;
				 progressBar1->Value = 80;

				 for (int h = 0; h < img.rows; h++)
				 for (int w = 0; w < img.cols; w++) {
					 label = labelImage.at<int>(h, w);

					 if (stats.at<int>(label, CC_STAT_AREA) < gRemoveSize && !protectLabel[label])
						 img.at<uchar>(h, w) = 0;
				 } // for
				 progressBar1->Value = 90;


				 img = ~img;
				 //---------�h�[����
				 for (int i = 0; i < img.rows; i++)
				 for (int j = 0; j < img.cols; j++){
					 if (original.at<uchar>(i, j) - img.at<uchar>(i, j) == -255){
						 RedPic.at<Vec3b>(i, j)[1] = 0;
					 }
					 if (img.at<uchar>(i, j) == 0)
						 RedPic.at<Vec3b>(i, j) = 0;
				 }//for

				 //��j�ΰѦҹ�
				 RedPicCPY_forUPSAMPLE = RedPic.clone();
				 RedPicCPY_forUPSAMPLEeraser = RedPic.clone();

				 //�ܧ���ܹϤj�p
				 resize(RedPic, RedPic, cv::Size(RedPic.cols / 4, RedPic.rows / 4), INTER_CUBIC);

				 //������Τ�����
				 Mat TempForBinaryMap(RedPic.rows, RedPic.cols, CV_8UC1, Scalar(0));
				 Mat TempForBinaryMap_protectVER(original.rows, original.cols, CV_8UC1, Scalar(0));
				 Mat TempForBinaryMap_UPsampleVER(original.rows, original.cols, CV_8UC1, Scalar(0));

				 BinaryMapForUI = TempForBinaryMap.clone();
				 RedPicCPY = RedPic.clone();
				 BinaryMapForProtectMap = TempForBinaryMap_protectVER.clone();
				 ProtectRegionCPY = protectRegion.clone();
				 BinaryMapForUPsample=TempForBinaryMap_UPsampleVER.clone();


				 progressBar1->Value = 100;
				 namedWindow("RedPic", 0);//�Ѽ�0�N��H�i�]�t�����Ϥ��������j�p�}��

				 cvSetMouseCallback("RedPic", mymouse);
				 

				 int key = 0;
				 while (1) {
					 if (key != 27){
						 key = waitKey(1);
						 if (key == '+'&&paint_size <= scollbarMax - 30)
							 paint_size += 30;

						 if (key == '-'&&paint_size >= scollbarMin + 30)
							 paint_size -= 30;
						 if (key == '3') 
							 DUBBLECLICK = 1;
					 }
					 else{
						 MessageBox::Show("Ready to store it.", "Message");
						 break;
					 }
					
				 }//
				 imshow("RedPic", RedPic);
				 waitKey();

				 //-----
				 for (int h = 0; h < img.rows; h++)
				 for (int w = 0; w < img.cols; w++)
				 if (protectRegion.at<uchar>(h, w) != 0)
					 protectLabel[labelImage.at<int>(h, w)] = true;

				 for (int h = 0; h < img.rows; h++)
				 for (int w = 0; w < img.cols; w++) {
					 label = labelImage.at<int>(h, w);

					 if (stats.at<int>(label, CC_STAT_AREA) < gRemoveSize && !protectLabel[label])
						 original.at<uchar>(h, w) = 255;
				 } // for
				 //----
				 
				 imwrite("Protect_Out.png", protectRegion);
				 imwrite("Final_Output.png", original);
				 MessageBox::Show("Image archives successful!!", "Message");
	}
private: System::Void hScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
				 paint_size = hScrollBar1->Value;
				 label1->Text = "Paint size : " + Convert::ToString(hScrollBar1->Value);
	}
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 hScrollBar1->Value = 100;
			 paint_size = 100;
			 label1->Text = "Paint size : " + Convert::ToString(hScrollBar1->Value);
			// hScrollBar1->Value = 250;
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 MessageBox::Show("Mouse event:\n1.Left click mouse ,drag and drop to draw modifying curve. \n2.Right click mouse ,drag and drop to draw modifying rectangle.\n3.Middle click mouse ,drag and drop to erase places you were modifying.\nKeyboard event:\n1.Press number 1 to shrink down the painting brush size.\n2.Press number 2 to enlarge the painting brush size.\nPress Esc to store this work", "manual");
}
};
}
