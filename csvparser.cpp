#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <vector>

using namespace std;

struct knnPoint 
{ 
	int val;	 // Group of point 
	double x, y, a, b, c, d;	 // Co-ordinate of point 
	double distance; // Distance from test point 
}; 

void csvreader(knnPoint arr[60]){
	fstream inputfile;

	inputfile.open("sampledata.csv",ios::in);
	
	
	vector<string> row;
	string line,word,temp;

	while (getline(inputfile,line)){
		stringstream s(line);
		double rawdata[7];
		int counter = 0;
    	while (getline(s,word,','))
		{			
			rawdata[counter] = stod(word);
			counter++;			
		}
		arr[int(rawdata[6])-1].x = rawdata[0]; 
		arr[int(rawdata[6])-1].y = rawdata[1]; 
		arr[int(rawdata[6])-1].a = rawdata[2]; 
		arr[int(rawdata[6])-1].b = rawdata[3];
		arr[int(rawdata[6])-1].c = rawdata[4]; 
		arr[int(rawdata[6])-1].d = rawdata[5];
	}
    inputfile.close();
}

int main(){
	knnPoint sampel[60];
	csvreader(sampel);
	for (int i = 0; i < 60; i++)
	{
		cout << sampel[i].x <<endl;
	}
	
	return 0;
}

