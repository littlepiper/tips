//time calculation
double t1 = getTickCount();
double t2 = getTickCount();
double t = (t2 - t1) * 1000 / getTickFrequency();
cout << "time:" << t << "ms" <<endl;

//read *.txt data
ifstream data("*.txt");
double d;
vector<double> v;
while(data >> d)
  v.push_back(d);
data.close();
 
//copy binary image
ifstream pic;
pic.open(path, std::ios::binary);
ofstream copy_pic;
copy_pic.open(path2, 'w');
copy_pic << pic.rdbuf();
copy_pic.close();
pic.close();

VS Resource file read example
HRSRC Rsrc = FindResource(hExe, MAKEINTRESOURCE(IDR_BIN1), TEXT("BIN"));
HGLOBAL hGlobal = LoadResource(NULL, Rsrc);
if (hGlobal == NULL)
{
	FreeResource(hGlobal);
	std::cout << "resource load error!" << std::endl;
	return -1;
}
LPVOID pBuffer = LockResource(hGlobal);
DWORD dwSize = SizeofResource(NULL, Rsrc);
char* pByte = new char[dwSize + 1];
memcpy(pByte, pBuffer, dwSize);
GlobalUnlock(hGlobal);
delete[] pByte;
pByte = NULL;

//get current path
string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; 
	string path = szFilePath;
	return path;
}
