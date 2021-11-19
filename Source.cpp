#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include <string>
#include <atlbase.h>
#pragma warning(disable : 4996)
///////////////disply the "dd" to "theDay" & "mm" to "theMonth" & "yy" to "theYear"/////////////////////////////
struct Date {
	int theDay;
	int theMonth;
	int theYear;
};
struct Date date;
struct Remainder {
	int theDay;
	int theMonth;
	char note[50];
};
struct Remainder R;
////////////////////////change "xy" to "coordinate"
COORD coordinate = { 0, 0 };
///////////////////////////CHANGE FUN NAME "gotoxy" to "goToxy"/////////////////////////////////
void goToxy(int x, int y)
{
	coordinate.X = x; coordinate.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

///////////////////////////////change fun name "SetColor" to "setColor"//////////////////////////
void setColor(int forgC)
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		wColor = (csbi.wAttributes & 0xF0) + (forgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}
////////////////////////change the fun name "ClearConsoleToColors" to "clearConsoleToColors"/////////////////////////
////////////////////change variable ForgC/BackC to forgC/backC ///////////////////////// 
void clearConsoleToColors(int forgC, int backC)
{
	WORD wColor = ((backC & 0x0F) << 4) + (forgC & 0x0F);
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SetConsoleTextAttribute(hStdOut, wColor);
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		FillConsoleOutputCharacter(hStdOut, (TCHAR)32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
		FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
		SetConsoleCursorPosition(hStdOut, coord);
	}
	return;
}

void setColorAndBackground(int forgC, int backC)
{
	WORD wColor = ((backC & 0x0F) << 4) + (forgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}

///////////////////change fun name "check_leapYear" to "checkLeapYear"///////////////////////////////////
int checkLeapYear(int year) { 
	if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
		return 1;
	return 0;
}
////////////////////change fun name "increase_month" to "increaseMonth"/////////////////////////////
//////////////////the magic number ///////////////
int numMonth = 12;
void increaseMonth(int* theMonth, int* theYear) { 
	++* theMonth;
	if (*theMonth > numMonth) {
		++* theYear;
		*theMonth = *theMonth - numMonth;
	}
}

////////////////////////change fun name "decrease_month" to "decreaseMonth"
//////////////////////////use magic number //////////////////////
int minYear = 1600;
void decreaseMonth(int* theMonth, int* theYear) { 
	--* theMonth;
	if (*theMonth < 1) {
		--* theYear;
		if (*theYear < minYear) {
			printf("No record available");
			return;
		}
		*theMonth = *theMonth + numMonth;
	}
}
/////////////////////////just delete the retern which repeated in each line & make the code smart ///////////////////////////////////////////////////////////////
int getNumberOfDays(int month, int year) {
	switch (month) {
	case 1: case 3:case 5:case 7: case 8:case 10:case 12:
		return(31);
	case 4: case 6: case 9: case 11:
		return(30);
	case 2: if (checkLeapYear(year) == 1)
		return(29);
		/////////////////////////////delete "else" ////////////////////////////
		return(28);
		////////////////////////////delete "defult" ///////////////////////////
		return(-1);
	}
}

const char* getName(int day) { 
	switch (day) {
	case 0:return("Sunday");
	case 1:return("Monday");
	case 2:return("Tuesday");
	case 3:return("Wednesday");
	case 4:return("Thursday");
	case 5:return("Friday");
	case 6:return("Saturday");
		///////////////////DELETE defult//////////////////////////
		return("Error in getName() module.Invalid argument passed");
	}
}

////////////////////////make the code smart by merging some lines with each other/////////////////////////////////////////////
/////////////////////////change fun name "DayNumber" to "dayNumber"///////////////////////////////////
int numDay = 7;
int dayNumber(int day, int mon, int year) {
	int res = 0, t1, t2;
	year = year - minYear;
	while (year >= 100) {
		res = res + 5;
		year = year - 100;
	}
	res = (res % numDay);
	t1 = ((year - 1) / 4);
	t2 = (year - 1) - t1;
	t1 = ((t1 * 2) + t2) % numDay;
	res = (res + t1) % numDay;
	t2 = 0;
	for (t1 = 1; t1 < mon; t1++) {
		t2 += getNumberOfDays(t1, year);
	}
	t2 = (t2 + day) % numDay;
	res = (res + t2) % numDay;
	if (year > 2000)
		res = res + 1;
	res = res % numDay;
	return res;
}

const char* getDay(int theDay, int theMonth, int theYear) {
	int day;
	if (!(theMonth >= 1 && theMonth <= numMonth)) {
		return("Invalid month value");
	}
	if (!(theDay >= 1 && theDay <= getNumberOfDays(theMonth, theYear))) {
		return("Invalid date");
	}
	if (theYear >= minYear) {
		day = dayNumber(theDay, theMonth, theYear);
		day = day % numDay;
		return(getName(day));
	}
	////////////////////////////delete else////////////////////////////////
	return("Please give year more than 1600");
}
///////////////////////////put "FILE* filePrint;" out of the functions/////////////////////////////////////
FILE* filePrint;
int checkNote(int theDay, int theMonth) {
	filePrint = fopen("note.dat", "rb");
	if (filePrint == NULL) {
		printf("Error in Opening the file");
	}
	while (fread(&R, sizeof(R), 1, filePrint) == 1) {
		if (R.theDay == theDay && R.theMonth == theMonth) {
			fclose(filePrint);
			return 1;
		}
	}
	fclose(filePrint);
	return 0;
}
//////////////create function to print the structure of callender ////////////////
void printCal(int mon, int year, int x, int y) {
	goToxy(25, 6);
	switch (mon) {
	case 1: printf("January"); break;
	case 2: printf("February"); break;
	case 3: printf("March"); break;
	case 4: printf("April"); break;
	case 5: printf("May"); break;
	case 6: printf("June"); break;
	case 7: printf("July"); break;
	case 8: printf("August"); break;
	case 9: printf("September"); break;
	case 10: printf("October"); break;
	case 11: printf("November"); break;
	case 12: printf("December"); break;
	}
	printf(" , %d", year);
	goToxy(20, 7);
	printf("---------------------------");

	y += 3;
	goToxy(x, y);
	printf("S   M   T   W   T   F   S   ");
	y++;
}
void printDay() {
	int calNote, day;
	day = dayNumber(theDay, mon, year);
	switch (day) {
	case 0:
		x = x;
		calNote = 1;
		break;
	case 1:
		x = x + 4;
		calNote = 2;
		break;
	case 2:
		x = x + 8;
		calNote = 3;
		break;
	case 3:
		x = x + 12;
		calNote = 4;
		break;
	case 4:
		x = x + 16;
		calNote = 5;
		break;
	case 5:
		x = x + 20;
		calNote = 6;
		break;
	case 6:
		x = x + 24;
		calNote = 7;
		break;
		////////////////////////delete default/////////////////////////
		printf("INVALID DATA FROM THE getOddNumber()MODULE");
		return;
	}
}
void printMonth(int mon, int year, int x, int y) { 
	////////////rename "nod" to "numOfDay" AND "calNote" to "calNote" AND "d" to "theDay" 
	int numOfDay, calNote, theDay = 1, x1 = x,  isNote = 0;
	if (!(mon >= 1 && mon <= numMonth)) {
		printf("INVALID MONTH");
		_getch();
		return;
	}
	if (!(year >= minYear)) {
		printf("INVALID YEAR");
		_getch();
		return;
	}
	goToxy(20, y);
	printf("---------------------------\n");
	
	printCal();

	numOfDay = getNumberOfDays(mon, year);

	printDay();

	goToxy(x, y);
	if (calNote == 1) {
		setColor(12);
	}
	if (checkNote(theDay, mon) == 1) {
		setColorAndBackground(15, 12);
	}
	printf("%02d", theDay);
	setColorAndBackground(15, 1);
	for (theDay = 2; theDay <= numOfDay; theDay++) {
		if (calNote % numDay == 0) {
			y++;
			calNote = 0;
			x = x1 - 4;
		}
		x = x + 4;
		calNote++;
		goToxy(x, y);
		if (calNote == 1) {
			setColor(12);
		}
			setColor(15);
		if (checkNote(theDay, mon) == 1) {
			setColorAndBackground(15, 12);
		}
		printf("%02d", theDay);
		setColorAndBackground(15, 1);
	}
	goToxy(8, y + 2);
	setColor(14);
	printf("Press 'n'  to Next, Press 'p' to Previous and 'q' to Quit");
	goToxy(8, y + 3);
	printf("Red Background indicates the NOTE, Press 's' to see note: ");
	setColor(15);
}

void AddNote() {
	filePrint = fopen("note.dat", "ab+");
	system("cls");
	goToxy(5, 7);
	printf("Enter the date(theDay/theMonth): ");
	scanf_s("%d%d", &R.theDay, &R.theMonth);
	goToxy(5, 8);
	printf("Enter the Note(50 character max): ");
	fflush(stdin);
	scanf(" %[^\n]", R.note);
	if (fwrite(&R, sizeof(R), 1, filePrint)) {
		goToxy(5, 12);
		puts("Note is saved sucessfully");
		fclose(filePrint);
	}
	else {
		goToxy(5, 12);
		setColor(12);
		puts("\aFail to save!!\a");
		setColor(15);
	}
	goToxy(5, 15);
	printf("Press any key............");
	_getch();
	fclose(filePrint);
}

void showNote(int theMonth) {
	int i = 0, isFound = 0;
	system("cls");
	filePrint = fopen("note.dat", "rb");
	if (filePrint == NULL) {
		printf("Error in opening the file");
	}
	while (fread(&R, sizeof(R), 1, filePrint) == 1) {
		if (R.theMonth == theMonth) {
			goToxy(10, 5 + i);
			printf("Note %d Day = %d: %s", i + 1, R.theDay, R.note);
			isFound = 1;
			i++;
		}
	}
	if (isFound == 0) {
		goToxy(10, 5);
		printf("This Month contains no note");
	}
	goToxy(10, 7 + i);
	printf("Press any key to back.......");
	_getch();

}

int main() {
	clearConsoleToColors(15, 1);
	std::string s = "Calender Project - Programming-technique.blogspot.com";
	LPCWSTR wide_string;
	wide_string = CA2T(s.c_str());
	SetConsoleTitle(wide_string);
	int choice;
	char ch = 'a';
	while (1) {
		system("cls");
		////////////////////////just reduce the number of lines by put the lines of print in one print////////////////////////
		printf("1. Find Out the Day\n2. Print all the day of month\n3. Add Note\n4.EXIT\nENTER YOUR CHOICE : ");
		scanf_s("%d", &choice);
		system("cls");
		switch (choice) {
		case 1:
			printf("Enter date (theDay theMonth theYearYY) : ");
			scanf_s("%d %d %d", &date.theDay, &date.theMonth, &date.theYear);
			printf("Day is : %s", getDay(date.theDay, date.theMonth, date.theYear));
			printf("\nPress any key to continue......");
			_getch();
			break;
		case 2:
			printf("Enter month and year (theMonth YYYY) : ");
			scanf_s("%d %d", &date.theMonth, &date.theYear);
			system("cls");
			while (ch != 'q') {
				printMonth(date.theMonth, date.theYear, 20, 5);
				ch = _getch();
				if (ch == 'n') {
					increaseMonth(&date.theMonth, &date.theYear);
					system("cls");
					printMonth(date.theMonth, date.theYear, 20, 5);
				}
				if (ch == 'p') {
					decreaseMonth(&date.theMonth, &date.theYear);
					system("cls");
					printMonth(date.theMonth, date.theYear, 20, 5);
				}
				if (ch == 's') {
					showNote(date.theMonth);
					system("cls");
				}
			}
			break;
		case 3:
			AddNote();
			break;
		case 4:
			exit(0);
		}
	}
	return 0;
}

