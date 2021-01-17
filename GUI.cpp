#include "GUI.h"
#include "../Courses/Course.h"
#include "../StudyPlan/AcademicYear.h"
#include <sstream>
#include"..\Note.h"
#include <iostream>

using namespace std;


GUI::GUI()
{ 
	pWind = new window(WindWidth, WindHeight,wx,wy);
	pWind->ChangeTitle(WindTitle);
	ClearDrawingArea();
	ClearStatusBar();
	CreateMenu();
}


//Clears the status bar
void GUI::ClearDrawingArea() const
{
	pWind->SetBrush(BkGrndColor);
	pWind->SetPen(BkGrndColor);
	pWind->DrawRectangle(0, MenuBarHeight, WindWidth, WindHeight -StatusBarHeight);

}

void GUI::ClearStatusBar() const
{
	pWind->SetBrush(StatusBarColor);
	pWind->SetPen(StatusBarColor);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight, WindWidth, WindHeight);
}

void GUI::CreateMenu() const
{
	pWind->SetBrush(StatusBarColor);
	pWind->SetPen(StatusBarColor);
	pWind->DrawRectangle(0, 0, WindWidth, MenuBarHeight);

	//You can draw the menu icons any way you want.

	//First prepare List of images paths for menu item
	string MenuItemImages[ITM_CNT];
	MenuItemImages[ITM_ADD]     = "GUI\\Images\\Menu\\Menu_add_course.jpg";
	MenuItemImages[ITM_ADD_NOTE] =  "GUI\\Images\\Menu\\Add_notes.jpg";
	MenuItemImages[ITM_SHOW_NOTE] = "GUI\\Images\\Menu\\Show_Notes.jpg";
	MenuItemImages[ITM_LOAD]           = "GUI\\Images\\Menu\\Menu_add_course.jpg";                                                     
	MenuItemImages[ITM_SAVE]    = "GUI\\Images\\Menu\\Menu_Save.jpg";
	MenuItemImages[ITM_DISPLAY_INFO]    = "GUI\\Images\\Menu\\Menu_disp_Info.jpg";
	MenuItemImages[ITM_REDO]     = "GUI\\Images\\Menu\\Menu_redo.jpg";
	MenuItemImages[ITM_DEL]      =  "GUI\\Images\\Menu\\Delete_Course.jpg";
	MenuItemImages[ITM_CHANGE] = "GUI\\Images\\Menu\\ChangeCode.jpg";
	MenuItemImages[ITM_CHECK] = "GUI\\Images\\Menu\\CheckValidity.jpg";
	MenuItemImages[ITM_EXIT]     = "GUI\\Images\\Menu\\Menu_exit2.jpg";
	
	//TODO: Prepare image for each menu item and add it to the list

	//Draw menu items one image at a time
	for (int i = 0; i<ITM_CNT; i++)
		pWind->DrawImage(MenuItemImages[i], i*MenuItemWidth, 0, MenuItemWidth, MenuBarHeight);
}

////////////////////////    Output functions    ///////////////////

//Prints a message on the status bar
void GUI::PrintMsg(string msg) const
{
	ClearStatusBar();	//Clear Status bar to print message on it
						// Set the Message offset from the Status Bar
	int MsgX = 25;
	int MsgY = StatusBarHeight - 10;

	// Print the Message
	pWind->SetFont(20, BOLD , BY_NAME, "Arial");
	pWind->SetPen(MsgColor);
	pWind->DrawString(MsgX, WindHeight - MsgY, msg);
}

//////////////////////////////////////////////////////////////////////////
void GUI::UpdateInterface() const
{
	
	pWind->SetBuffering(true);
	//Redraw everything
	CreateMenu();
	ClearStatusBar();
	ClearDrawingArea();
	pWind->UpdateBuffer();
	pWind->SetBuffering(false);

}

////////////////////////    Drawing functions    ///////////////////
void GUI::DrawCourse(const Course* pCrs)
{  

	if (pCrs->isSelected())
		pWind->SetPen(HiColor, 1);     

	else {
        if(pCrs->getCourseType()==UNIVE)
			pWind->SetPen(UNIVcolor, 2);
		if(pCrs->getCourseType() == MAJORE)
			pWind->SetPen(MAJORcolor, 2);
		if (pCrs->getCourseType() == TRACKE)
			pWind->SetPen(MINORcolor, 2);
		if (pCrs->getCourseType() == TRACKE)
			pWind->SetPen(CONCcolor, 2);
		if (pCrs->getCourseType() == TRACKE)
			pWind->SetPen(TRACKcolor, 2);
	}
	
	pWind->SetBrush(FillColor);
	graphicsInfo gInfo = pCrs->getGfxInfo();
	if (pCrs->getCourseType() == TRACKE) {
		pWind->DrawRectangle(gInfo.x, gInfo.y, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT,FILLED,2,2);
	}
	else {
		pWind->DrawRectangle(gInfo.x, gInfo.y, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT);
	}
	pWind->DrawLine(gInfo.x, gInfo.y + CRS_HEIGHT / 2, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT / 2);
	
	//Write the course code and credit hours.
	int Code_x = gInfo.x + CRS_WIDTH * 0.2;
	int Code_y = gInfo.y + CRS_HEIGHT * 0.07;
	pWind->SetFont(CRS_HEIGHT * 0.5, BOLD , BY_NAME, "Gramound");
	pWind->SetPen(MsgColor);

	ostringstream crd;
	//crd<< "crd:" << pCrs->getCredits();
	pWind->DrawString(Code_x, Code_y, pCrs->getCode());
	pWind->DrawString(Code_x, Code_y + CRS_HEIGHT/2, to_string(pCrs->getCredits()));

}
void GUI::DragCourse(const Course* pCrs)
{
	if (pCrs->isSelected()) {}
	else
		// Flush out the input queues before beginning
		pWind->FlushMouseQueue();
	pWind->FlushKeyQueue();

	bool bDragging = false;
	int iX = 0, iY = 0;
	int iXOld = 0;
	int iYOld = 0;
	graphicsInfo gInfo = pCrs->getGfxInfo();
	char cKeyData;
	// Loop until there escape is pressed
	while (pWind->GetKeyPress(cKeyData) != ESCAPE)
	{
		// Dragging voodoo
		if (bDragging == false) {
			if (pWind->GetButtonState(LEFT_BUTTON, iX, iY) == BUTTON_DOWN) {
				if (((iX > gInfo.x) && (iX < (gInfo.x + CRS_WIDTH))) && ((iY > gInfo.y) && (iY < (gInfo.y + CRS_HEIGHT)))) {
					bDragging = true;
					iXOld = iX; iYOld = iY;
					pWind->SetPen(BkGrndColor);
					pWind->SetBrush(BkGrndColor);
					pWind->DrawRectangle(gInfo.x, gInfo.y, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT);
					pWind->DrawLine(gInfo.x, gInfo.y + CRS_HEIGHT / 2, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT / 2);
				}
			}
		}
		else {
			if (pWind->GetButtonState(LEFT_BUTTON, iX, iY) == BUTTON_UP) {
				bDragging = false;
				pWind->SetPen(DrawColor, 2);
				pWind->SetBrush(FillColor);
				pWind->DrawRectangle(gInfo.x, gInfo.y, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT);
				pWind->DrawLine(gInfo.x, gInfo.y + CRS_HEIGHT / 2, gInfo.x + CRS_WIDTH, gInfo.y + CRS_HEIGHT / 2);

				//Write the course code and credit hours.
				int Code_x = gInfo.x + CRS_WIDTH * 0.15;
				int Code_y = gInfo.y + CRS_HEIGHT * 0.05;
				pWind->SetFont(CRS_HEIGHT * 0.4, BOLD, BY_NAME, "Gramound");
				pWind->SetPen(MsgColor);

				ostringstream crd;
				crd << "crd:" << pCrs->getCredits();
				pWind->DrawString(Code_x, Code_y, pCrs->getCode());
				pWind->DrawString(Code_x, Code_y + CRS_HEIGHT / 2, crd.str());
				//pCrs->setGfxInfo(gInfo);                                              
			}
			else {
				if (iX != iXOld) {
					gInfo.x = gInfo.x + (iX - iXOld);
					iXOld = iX;
				}
				if (iY != iYOld) {
					gInfo.y = gInfo.y + (iY - iYOld);
					iYOld = iY;
				}
			}

		}
	}
}
void GUI::DeleteCourse(int x,int y) {
	pWind->SetBrush(BkGrndColor);
	pWind->DrawRectangle(x, y, x + CRS_WIDTH, y + CRS_HEIGHT);

}
void GUI::DrawAcademicYear(const AcademicYear* pY)
{
	graphicsInfo gInfo = pY->getGfxInfo();


	pWind->SetBrush(BACKGROUND_RED);
	pWind->SetPen(BLACK, 2);

			pWind->SetPen(BLACK);
		
			////////// Draw Years;
			for(int i=1; i<=5;i++){
				//################################### Draw Academic Years######################33
				pWind->SetFont(16, 2, ROMAN);
				pWind->SetPen(BLACK);
				pWind->DrawString(4, WindHeight - StatusBarHeight - 6 - YearHeight * ((i-1)*3) - 50, "Year "+ to_string(i));
				pWind->DrawRectangle(0, WindHeight - StatusBarHeight  - YearHeight * ((i-1)*3) , 60, WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3)-90, FRAME, 7, 7);
				pWind->DrawLine(0, WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - 90, WindWidth, WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - 3* semHeight);
			
				//##########################################Draw Fall##################################
				pWind->SetFont(15, 1, ROMAN);
				pWind->DrawString(70, WindHeight - StatusBarHeight - 27 - YearHeight * (i-1)*3, "Fall");
				pWind->DrawRectangle(62, WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3), 120, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - semHeight), FRAME, 7, 7);
				pWind->SetPen(LIGHTBLUE);
				pWind->DrawLine(120, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - semHeight), WindWidth, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - semHeight));
				//###################################3Draw Spring########################
				pWind->SetPen(BLACK);
				pWind->DrawString(70, WindHeight - StatusBarHeight - 27 - YearHeight * (i - 1) * 3-25, "Spring");
				pWind->DrawRectangle(62, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - semHeight), 120, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - 2* semHeight), FRAME, 7, 7);
				
				//##########################################Draw Summer############################
				pWind->DrawString(65, WindHeight - StatusBarHeight - 27 - YearHeight * (i - 1) * 3 -25*2, "Summer");
				pWind->SetPen(LIGHTBLUE);
				pWind->DrawLine(120, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - 60), WindWidth, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - 60));
				pWind->SetPen(BLACK);
				pWind->DrawRectangle(62, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - 2* semHeight), 120, (WindHeight - StatusBarHeight - YearHeight * ((i - 1) * 3) - 3* semHeight), FRAME, 7, 7);
				
			}
}

	//Then each course should be drawn inside rect of its year/sem

	////////////////////////    Input functions    ///////////////////
	//This function reads the position where the user clicks to determine the desired action
	//If action is done by mouse, actData will be the filled by mouse position
ActionData GUI::GetUserAction(string msg) const
	{
		keytype ktInput;
		clicktype ctInput;
		char cKeyData;


		// Flush out the input queues before beginning
		pWind->FlushMouseQueue();
		pWind->FlushKeyQueue();

		PrintMsg(msg);

		while (true)
		{
			int x, y;
			ctInput = pWind->GetMouseClick(x, y);	//Get the coordinates of the user click
			ktInput = pWind->GetKeyPress(cKeyData);

			if (ktInput == ESCAPE)	//if ESC is pressed,return CANCEL action
			{
				return ActionData{ CANCEL };
			}


			if (ctInput == LEFT_CLICK)	//mouse left click
			{
				//[1] If user clicks on the Menu bar
				if (y >= 0 && y < MenuBarHeight)
				{
					//Check whick Menu item was clicked
					//==> This assumes that menu items are lined up horizontally <==
					int ClickedItemOrder = (x / MenuItemWidth);
					//Divide x coord of the point clicked by the menu item width (int division)
					//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on

					switch (ClickedItemOrder)
					{
					case ITM_ADD: return ActionData{ ADD_CRS };//Add course
					case ITM_ADD_NOTE: return ActionData{ ADD_NOTE };
					case ITM_SHOW_NOTE:return ActionData{ SHOW_NOTE };
					//case ITM_Drag: return ActionData{ Drag_Drop };
					case ITM_LOAD: return ActionData{ LOAD };
					//case ITM_LOAD_RULES: return ActionData{ LOAD_RULES };
					case ITM_DISPLAY_INFO: return ActionData{ DISPLAY_INFO };   //display course Information
					case ITM_DEL:return ActionData{ DEL_CRS };
					case ITM_SAVE:return ActionData{ SAVE };
					case ITM_CHANGE: return ActionData{ CHANGE_CODE };
					case ITM_CHECK: return ActionData{ CHECK_VALIDITY };
					case ITM_EXIT: return ActionData{ EXIT };		//Exit
					break;

					default: return ActionData{ MENU_BAR };	//A click on empty place in menu bar
					}
				}

				//[2] User clicks on the drawing area
				if (y >= MenuBarHeight && y < WindHeight - StatusBarHeight)
				{
					return ActionData{ DRAW_AREA,x,y };	//user want clicks inside drawing area
				}

				//[3] User clicks on the status bar
				return ActionData{ STATUS_BAR };
			}
		}//end while
	}
/////////////////////////////////drawNotes/////////////
void GUI::DrawNotes(vector <string> totalnotes) {
     pNoteWind = new window(300, 200, 900, 100);
	 pNoteWind->ChangeTitle("Notes Window");
	 pNoteWind->SetPen(LIGHTSALMON);
	 pNoteWind->SetBrush(WHITE);

	pNoteWind->SetFont(14, PLAIN, BY_NAME, "Arial");
	
	pNoteWind->SetPen(MsgColor);
	int j = 0;	
	for (int i = 0; i < totalnotes.size(); i++) {
		pNoteWind->DrawString(0, j, totalnotes[i]);
		j = j + 20;
		}
	int x, y;
	pNoteWind->WaitMouseClick(x, y);
	delete pNoteWind;
}

void GUI::OpenCheckWindow() {
	pValidityWindow = new window(400, 400, 500, 100);
	pValidityWindow->ChangeTitle("Check Report");
	pValidityWindow->SetBrush(LIGHTBLUE);
}
void GUI::DrawStringModerate(string s, int x, int y) {
	
	pWind->SetFont(20, 2, ROMAN);
	pValidityWindow->SetPen(YELLOW, 3);
	pValidityWindow->DrawString(x, y, "Moderate Issue:");
	pValidityWindow->SetPen(BLACK, 2);
	pValidityWindow->DrawString(x+10 , y, s);
	
}


void GUI::DrawStringCritical(string s, int x, int y) {

	pWind->SetFont(20, 2, ROMAN);
	pValidityWindow->SetPen(RED, 3);
	pValidityWindow->DrawString(x, y, "Critical Issue:");
	pValidityWindow->SetPen(BLACK, 2);
	pValidityWindow->DrawString(x + 50, y, s);

}


void GUI::CLoseCheckReport() {
	int n, m;
	pValidityWindow->WaitMouseClick(n, m);
	delete pValidityWindow;
}

void GUI::Info_Window(Course* P_course)
{

	int msg_x = 0;
	int msg_y = 0;
	p_Infowind = new window(W, L, Start, End);
	p_Infowind->ChangeTitle(InfowindTitle);
	p_Infowind->SetFont(12, BOLD, BY_NAME, "Arial");
	p_Infowind->SetPen(infoColor);

	if (P_course != nullptr)
	{
		p_Infowind->DrawString(msg_x,  msg_y,"Course Code: "+ P_course->getCode());
		msg_y += 15;
		p_Infowind->DrawString(msg_x,  msg_y,"Course Title: " +P_course->getTitle());
		msg_y += 15;
		//p_Infowind->DrawString(msg_x, L - msg_y, P_course->getType());
		p_Infowind->DrawString(msg_x,  msg_y, "Course Credits: " + to_string(P_course->getCredits()));
		msg_y += 15;
		list<Course_Code> prereq = P_course->getPreReq();
		list<Course_Code> coreq = P_course->getCoReq();
		p_Infowind->SetPen(DARKMAGENTA);
		p_Infowind->DrawString(msg_x, msg_y, "Corequisite list:");
		msg_y += 15;
		p_Infowind->SetPen(infoColor);
		if (coreq.empty()) {
			p_Infowind->DrawString(0, msg_y, "There are no Corequisite Courses for this course");
			msg_y += 15;
		}
		for (auto corequisit = coreq.begin(); corequisit != coreq.end(); ++corequisit)
		{
			p_Infowind->DrawString(0, msg_y, *corequisit);
			msg_y += 15;

		}
		p_Infowind->SetPen(DARKMAGENTA);
		p_Infowind->DrawString(msg_x, msg_y, "Prerequisite list:");
		msg_y += 15;
		p_Infowind->SetPen(infoColor);
		for (auto prerequisit = prereq.begin(); prerequisit != prereq.end(); ++prerequisit)
		{
			p_Infowind->DrawString(0, msg_y, *prerequisit);
			msg_y += 15;
		}
	}
	int x, y;
	p_Infowind->WaitMouseClick(x, y);
	delete p_Infowind;
}


string GUI::GetSrting() const
{
	//Reads a complete string from the user until the user presses "ENTER".
	//If the user presses "ESCAPE". This function should return an empty string.
	//"BACKSPACE" is also supported
	//User should see what he is typing at the status bar

	

	string userInput;
	char Key;
	while (1)
	{
		pWind->WaitKeyPress(Key);

		switch (Key)
		{
		case 27: //ESCAPE key is pressed
			PrintMsg("");
			return ""; //returns nothing as user has cancelled the input

		case 13:		//ENTER key is pressed
			return userInput;

		case 8:		//BackSpace is pressed
			if (userInput.size() > 0)
				userInput.resize(userInput.size() - 1);
			break;

		default:
			userInput += Key;
		};

		PrintMsg(userInput);
	}

}








GUI::~GUI()
{
	delete pWind;
	delete pNoteWind;
	delete pValidityWindow;
}
