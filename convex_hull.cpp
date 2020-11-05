// You need to complete this program for your second project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <stack>
#include <vector>
#include <fstream>
#include<bits/stdc++.h> 

using namespace std;

struct Point 
{ 
    int x, y; 
}; 

Point p0;
#define  iPair std::pair<int, int>
std::set<iPair> Qconvex;
std::vector<Point> convex;

Point nextToTop(stack<Point> &S) 
{ 
    Point p = S.top(); 
    S.pop(); 
    Point res = S.top(); 
    S.push(p); 
    return res; 
} 
  
void swap(Point &p1, Point &p2) 
{ 
    Point temp = p1; 
    p1 = p2; 
    p2 = temp; 
} 

int distance(Point p1, Point p2) 
{ 
    return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y); 
} 

int orientation(Point p, Point q, Point r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0){ return 0; }  
    else if(val > 0){return 1;}
    else{return 2;} 
} 

int compare(const void *vp1, const void *vp2) 
{ 
   Point *p1 = (Point *)vp1; 
   Point *p2 = (Point *)vp2; 
  
   // Find orientation 
   int ot = orientation(p0, *p1, *p2); 
   if (ot == 0) 
     if(distance(p0, *p2) >= distance(p0, *p1)){return -1;}
     else{return 1;} 
  
   if(ot == 2){return -1;}
   else{return 1;} 
} 
   
void Graham(Point points[], int n) 
{ 
   int minY = points[0].y, min = 0; 
   for (int i = 1; i < n; i++) 
   { 
     int y = points[i].y; 

      if ((y < minY) || (minY == y && points[i].x < points[min].x)) 
      {  
         minY = points[i].y;
         min = i; 
      } 
  }
   swap(points[0], points[min]); 

   p0 = points[0]; 
   qsort(&points[1], n-1, sizeof(Point), compare); 
   int m = 1; 
   for (int i=1; i<n; i++) 
   { 

      while (i < n-1 && orientation(p0, points[i], points[i+1]) == 0){
         i++; 
         points[m] = points[i]; 
         m++; 
      }
   } 
   if (m < 3) return; 
  
   stack<Point> S; 
   S.push(points[0]); 
   S.push(points[1]); 
   S.push(points[2]); 
  
   for (int i = 3; i < m; i++) 
   { 
      while (orientation(nextToTop(S), S.top(), points[i]) != 2) 
         S.pop(); 
         S.push(points[i]); 
   } 
  
   while (!S.empty()) 
   { 
       Point p = S.top(); 
       std::cout << "(" << p.x << ", " << p.y <<")" << std::endl; 
       S.pop(); 
   } 
} 

void jarvis(Point points[], int n){
   if(n < 3){
      std::cout<< "At leat 3 points are needed\n";
      return; 
   } 

   int left = 0;
   for(int i = 1; i < n; i++){
      if(points[i].x < points[left].x)
         left = i;
   }

   int p = left;
   int q;
   do{
      convex.push_back(points[p]);
      q = (p + 1)& n;
      for(int i = 0; i < n; i++){
         if(orientation(points[p], points[i], points[q]) == 2){
            q = i;
         }
      }
      p = q;
   }while(p != left);

   for(int i = 0; i < convex.size(); i++){
      std::cout << "(" << convex[i].x <<", "<< convex[i].y << ")\n";
   }
   
}

int findSide(iPair p1, iPair p2, iPair p) 
{ 
   int val = (p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first); 
  
    if (val > 0) 
        return 1; 
    if (val < 0) 
        return -1; 
    return 0; 
} 

int lineDist(iPair p1, iPair p2, iPair p) 
{ 
    return abs ((p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first));} 

void Quickhull(iPair a[], int n, iPair p1, iPair p2, int side) 
{ 
    int index = -1; 
    int max = 0; 
 
    for (int i = 0; i < n; i++) 
    { 
        int tmp = lineDist(p1, p2, a[i]); 
        if (findSide(p1, p2, a[i]) == side && tmp > max) 
        { 
            index = i; 
            max = tmp; 
        } 
    } 

    if (index == -1) 
    { 
        Qconvex.insert(p1); 
        Qconvex.insert(p2); 
        return; 
    } 

    Quickhull(a, n, a[index], p1, -findSide(a[index], p1, p2)); 
    Quickhull(a, n, a[index], p2, -findSide(a[index], p2, p1)); 
} 

void printHull(iPair a[], int n){
   if(n >3){
      std::cout<< "Quickhull not possible \n";
      return;
   }
   int minX = 0, maxX = 0;
   for(int i = 1; i < n; i++){
      if(a[i].first < a[minX].first)
         minX = i;
      if(a[i].first > a[maxX].first)
         maxX = i;
   }

   Quickhull(a, n, a[minX], a[maxX], -1);
   while(!Qconvex.empty()){
      std::cout << "(" <<( *Qconvex.begin()).first << ", "<< (*Qconvex.begin()).second << ") "; 
        Qconvex.erase(Qconvex.begin());
   }
}

int main(int argc, char *argv[])
{   
   if (argc < 3) 
      std::cout << "wrong format! should be \"a.exe algType dataFile\"";
   else {
      std::string algType = argv[1];
      std::string dataFilename = argv[2];

      std::string outputFile = "";
      //read your data points from dataFile (see class example for the format)
      int arr;
      std::ifstream file;
      file.open(dataFilename);
      int i = 0;
      
      std::vector<Point> ps;      
      Point points[ps.size()];
      int n = sizeof(points)/sizeof(points[0]);
      while(!file.eof()){
           int x, y;
           file >> x >> y;
           points[i].x = x;
           points[i].y = y;
           i++;
       }
       file.close();
      std::vector<Point> hull;
      iPair quickP[ps.size()];

      if (algType[0]=='G') {
         //call your Graham Scan algorithm to solve the problem
         Graham(points, n);
         outputFile = "hull_G.txt";
      } 
      else if (algType[0]=='J') {
         //call your Javis March algorithm to solve the problem
         jarvis(points, n);
         outputFile = "hull_J.txt";
      }
      else { //default 
         //call your Quickhull algorithm to solve the problem
         printHull(quickP, n);
         outputFile = "hull_Q.txt";
      }

      std::ofstream outFile;
      outFile.open(outputFile);

      // Write convex hull to the outputFile 
      for (int i = 0; i < hull.size(); ++i)
      {
         // Put x of the point in file
         outFile << "(" << hull[i].x;

         // Put a space in between the numbers
         outFile << ", ";

         // Put y of the point in file
         outFile << hull[i].y << ")";
         
         outFile << "\n";
      }
      //write your convex hull to the outputFile (see class example for the format)
      //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.
	}
	return 0;
}