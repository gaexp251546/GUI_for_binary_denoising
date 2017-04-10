#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

#pragma once


namespace 測試視窗 {

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
	string imgName;
	string saveimgName;
	Mat original;
	Mat RedPic;
	Mat img;
	Mat protectRegion;
	//for eraser
	Mat RedPicCPY;
	Mat ProtectRegionCPY;
	Mat BinaryMapForUI;
	Mat BinaryMapForProtectMap;
	
	
	void mymouse(int event, int x, int y, int flag, void* param)
	{
		static int oldx, oldy, now_down = 0, eraserDown;
		
		
		//左鍵畫線部分
		if (event == CV_EVENT_LBUTTONDOWN){
			circle(RedPic, cvPoint(x, y), paint_size / 2, cvScalar(0, 0, 255));
			oldx = x; oldy = y;
			now_down = 1;

		}
		if (event == CV_EVENT_LBUTTONUP){
			now_down = 0;
		}
		if (event == CV_EVENT_MOUSEMOVE && now_down == 1){
			line(RedPic, cvPoint(x, y), cvPoint(oldx, oldy), cvScalar(0, 255, 0), paint_size, 8, 0);
			line(protectRegion, cvPoint(x * 4, y * 4), cvPoint(oldx * 4, oldy * 4), cvScalar(255), paint_size * 4, 8, 0);
			oldx = x; oldy = y;
		}

		//右鍵框選部分
		if (event == CV_EVENT_RBUTTONDOWN){
			circle(RedPic, cvPoint(x, y), 8, cvScalar(0, 0, 255));
			oldx = x; oldy = y;
		}
		if (event == CV_EVENT_RBUTTONUP){
			rectangle(RedPic, Rect(cv::Point(oldx, oldy), cvPoint(x, y)), Scalar(0, 255, 0), -1);
			rectangle(protectRegion, Rect(cv::Point(oldx * 4, oldy * 4), cvPoint(x * 4, y * 4)), Scalar(255), -1);
		}

		//中鍵橡皮擦部分
		if (event == CV_EVENT_MBUTTONDOWN){
			oldx = x; oldy = y;
			eraserDown = 1;
		}
		if (event == CV_EVENT_MBUTTONUP){
			eraserDown = 0;
			//把binaryMap歸零
			Mat TempForBinaryMap(RedPic.rows, RedPic.cols, CV_8UC1, Scalar(0));
			Mat TempForBinaryMap_protectVER(original.rows, original.cols, CV_8UC1, Scalar(0));
			BinaryMapForUI = TempForBinaryMap.clone();
			BinaryMapForProtectMap = TempForBinaryMap_protectVER.clone();
		}
		if (event == CV_EVENT_MOUSEMOVE && eraserDown == 1){
			line(BinaryMapForUI, cvPoint(x, y), cvPoint(oldx, oldy), cvScalar(1), paint_size, 8, 0);
			line(BinaryMapForProtectMap, cvPoint(x*4, y*4), cvPoint(oldx*4, oldy*4), cvScalar(1), paint_size*4, 8, 0);
	
			for (int i = 0; i < BinaryMapForUI.rows; i++)
			for (int j = 0; j < BinaryMapForUI.cols; j++){
				if ((int)BinaryMapForUI.at<uchar>(i, j) == 1)
					RedPic.at<Vec3b>(i, j) = RedPicCPY.at<Vec3b>(i, j);
			}


			for (int i = 0; i < BinaryMapForProtectMap.rows; i++)
			for (int j = 0; j < BinaryMapForProtectMap.cols; j++){
				if ((int)BinaryMapForProtectMap.at<uchar>(i, j) == 1)
					protectRegion.at<uchar>(i, j) = ProtectRegionCPY.at<uchar>(i, j);
			}

			oldx = x; oldy = y;
		}


		
	
	
		imshow("RedPic", RedPic);
	}
	//--
	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
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
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器
		/// 修改這個方法的內容。
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
					 MessageBox::Show("Click to start reading", "Message");
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
				// MessageBox::Show("aaa");
				 progressBar1->Value = 15;
				 img = ~img;//黑變白
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
				 //---------多加部分
				 for (int i = 0; i < img.rows; i++)
				 for (int j = 0; j < img.cols; j++){
					 if (original.at<uchar>(i, j) - img.at<uchar>(i, j) == -255){
						 RedPic.at<Vec3b>(i, j)[1] = 0;


					 }


					 if (img.at<uchar>(i, j) == 0)
						 RedPic.at<Vec3b>(i, j) = 0;
				 }//for
	
				 //變更顯示圖大小
				 resize(RedPic, RedPic, cv::Size(RedPic.cols / 4, RedPic.rows / 4), INTER_CUBIC);
				 //for eraser
				 Mat TempForBinaryMap(RedPic.rows, RedPic.cols, CV_8UC1, Scalar(0));
				 Mat TempForBinaryMap_protectVER(original.rows, original.cols, CV_8UC1, Scalar(0));
				 BinaryMapForUI = TempForBinaryMap.clone();
				 RedPicCPY = RedPic.clone();
				 BinaryMapForProtectMap = TempForBinaryMap_protectVER.clone();
				 ProtectRegionCPY = protectRegion.clone();

				 progressBar1->Value = 100;
				 namedWindow("RedPic", 0);//參數0代表以可包含全部圖片的視窗大小開啟
				 imshow("RedPic", RedPic);


				 cvSetMouseCallback("RedPic", mymouse);
				 cvWaitKey(0);

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
			 MessageBox::Show("左右件都有功能\nThe functions in this section perform various geometrical transformations of 2D images. They do not change the image content but deform the pixel grid and map this deformed grid to the destination image. In fact, to avoid sampling artifacts, the mapping is done in the reverse order, from destination to the source. That is, for each pixel (x, y) of the destination image, the functions compute coordinates of the corresponding “donor” pixel in the source image and copy the pixel value", "Manual");
}
};
}
