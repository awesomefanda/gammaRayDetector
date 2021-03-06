#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

namespace patch
{
template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}
}

#define NUM 50005


struct Point
{
    double x;
    double y;
    double z;
} ;

int compareDouble(const void * a, const void * b)
{
    return ( *(double*)a - *(double*)b );
}
double distance(vector<Point> &points, int i, int j)
{
    return sqrt((points[i].x - points[j].x)*(points[i].x - points[j].x) + (points[i].y - points[j].y)*(points[i].y - points[j].y));
}


bool compareFunction(const Point &a, const Point &b)
{
    return a.x < b.x;
}
double radius[] = {0.0005, 0.001, 0.005, 0.05, 0.1} ;

// Algo2 is the improved solution which sorts the list of points on x axis and do not consider the points farther than required distance in x axis.
void algo2(vector<Point> &points, int count, double neighborDist, ofstream& myfile)
{

    // Sorting the points based on x axis.
    std::sort(points.begin(), points.end(), compareFunction);

    int maxNumNeighbor = 0;
    int nodeWithMaxNumNeighbor[10005];
    int neighborCount=0;
    int i,j;

    for(i = 0; i < count; i++)
    {
        int numNeighbor = 0;

        for(j = i + 1; j < count; j++)
        {
            if (std::abs(points[j].x - points[i].x) > neighborDist)
            {
                break;
            }
            if (distance(points, i, j) <= neighborDist)
            {
                numNeighbor++;
            }
        }


        for(j = i - 1; j >= 0; j--)
        {
            if (std::abs(points[j].x - points[i].x) > neighborDist)
            {
                break;
            }
            if (distance(points, i, j) <= neighborDist)
            {
                numNeighbor++;
            }
        }

        if (numNeighbor > maxNumNeighbor)
        {
            maxNumNeighbor = numNeighbor;
            nodeWithMaxNumNeighbor[0] = i;
            neighborCount = 1;
        }
        else if (numNeighbor == maxNumNeighbor)
        {
            nodeWithMaxNumNeighbor[neighborCount++] = i;
        }
    }

    myfile << maxNumNeighbor;
    myfile << ",";

}




void calc_Max_Num(vector<Point> &coordinates, int count, int j)
{
    ofstream myfile;
    int i;
    int z_val=(int)coordinates[count -2].z;
    string filename = ("662Kev_New_Sand" + patch::to_string(z_val) + ".csv").c_str();
    cout << z_val << endl;
    myfile.open (filename,ios::app);
    myfile << j;
    myfile << ",";

    vector<double> rad(radius, radius + sizeof(radius) / sizeof(double));
    for (int i=0; i < rad.size(); i++)
    {
        algo2(coordinates, count, rad[i], myfile);
    }
    myfile<<endl;

    myfile.close();
}


// Read file and put the coordinates in the provided array.
void readFile(vector<Point> &coordinates, int* count, const char * fileName, int j)
{
    FILE *fp;
    char str[100];

    int size=0;
    *count = 0;


    fp = fopen(fileName , "r");
    if(fp == NULL)
    {   cout << "Error opening file %d"<< " Points_" << j<< endl;

        return;
    }

    // Parsing the file.
    while (fgets(str, 100, fp) != NULL)
    {
        int flag = 1;
        char* p = NULL;
        p = strtok (str, ",");
        double x, y, z;
        while (p != NULL)
        {

            double num = atof(p);
            if(flag==1)
            {
                x = num;
            }
            else if(flag==2)
            {
                y = num;

            }
            else
            {
                z = num;
                *count = *count + 1;
                flag=0;
                Point temp_point = {x, y, z};
                coordinates.push_back(temp_point);
                x=0;
                y=0;
                z=0;
            }
            p = strtok (NULL,",");
            ++flag;





        }

        if(*count >1 && coordinates[*count -1 ].z != coordinates[*count -2].z )
        {

            calc_Max_Num(coordinates, *count, j);
            *count=0;
            coordinates.clear();
            coordinates.resize(0);
            coordinates.shrink_to_fit();


        }


    }
    calc_Max_Num(coordinates, *count, j);





    fclose(fp);
}


int main()
{

    vector<Point> coordinates;
    int count, i;

    int j;
    for(j=0; j<10; j++){

         readFile(coordinates, &count, ("Points" + patch::to_string(j) + ".txt").c_str(), j);
         count=0;
         coordinates.clear();
         coordinates.resize(0);
         coordinates.shrink_to_fit();
    }


    return 0;
}
