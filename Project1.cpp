//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Project1.res");
USEFORM("MainForm.cpp", Main);
USELIB("mbn.lib");
USE("C:\Documents and Settings\Eigenaar\Bureaublad\mbnv2\src\mbn.h", File);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->Title = "MambaNet Tester";
     Application->CreateForm(__classid(TMain), &Main);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------


