#include "MyForm.h"


using namespace System;
using namespace ���յ���;



[STAThread]
int main(array<System::String ^> ^args)
{
	//�إߥ��󱱨���e�A���ҥ�windows����ı�ƮĪG
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//�إߥD�����ð���
	Application::Run(gcnew MyForm());

	Console::WriteLine(L"Hello World");

	return 0;
}
