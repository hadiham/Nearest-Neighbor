/* Hadi Hamid
Lab 036
no partner :(
Date Submitted:
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

//struct for the locations.txt data
struct Location{ 
    int row;
    int column;
    char symbol;
    int ID;
    bool visited;
};

//Loads planets to "Location" struct and determines which planets are out of range 
void LoadPlanetLocation(vector<Location> &group, istream &is, vector<int> &points){
    
    //reading the first 3 lines
    int totalRow;
    int totalCol;
    int startRow;
    int startCol;
    int endRow;
    int endCol;

    //pushing back the data into a vector of ints called points
    is >> totalRow;
    points.push_back(totalRow);
    is >> totalCol;
    points.push_back(totalCol);
    is >> startRow;
    points.push_back(startRow);
    is >> startCol;
    points.push_back(startCol);
    is >> endRow;
    points.push_back(endRow);
    is >> endCol;
    points.push_back(endCol);

   
   //reading in data and pushing it back into vector
   Location location;
    while(is >> location.row >> location.column >> location.symbol >> location.ID){
        
        location.visited = false;
        
        group.push_back(location);
        
        }
    
    //determining which planets are out of range
    for(int i = 0; i < group.size(); ++i){
        if(group.at(i).row <= points.at(0) && group.at(i).row > 0 && group.at(i).column <= points.at(1) && group.at(i).column > 0 ){
    
        }
        else {
           
            cout << group.at(i).ID << " out of range - ignoring" << endl;
            group.erase(group.begin() + (i));
            
            --i;
        }
    }
}

//function to change the mistakes in names.txt 
void changeLetters(vector<string> &planetName){
    for(int i = 0; i < planetName.size(); ++i){

        while(planetName.at(i).find("XX") != planetName.at(i).npos) { 
            int index = planetName[i].find("XX");
            planetName.at(i) = planetName.at(i).erase(index, 2);
        }

        while(planetName.at(i).find("_") != planetName.at(i).npos){
            int index2 = planetName.at(i).find("_");
            planetName.at(i).replace(index2, 1, " ");
        }
    }
}

//function used to create the map
void createGridMap(vector<vector<string> > &gridVecs, const vector<int> &points, vector<Location> &group){
    int numRows = points.at(0);
    int numCols = points.at(1);
    for (int i = 0; i < numRows; ++i){
        vector<string> gridRow;
    
        for (int k = 0; k < numCols;++k){
            gridRow.push_back(".");
     
        }
   gridVecs.push_back(gridRow); 
    } 

 
    gridVecs[points.at(2) - 1][points.at(3) - 1] = "S";
    gridVecs[points.at(4) - 1][points.at(5) - 1] = "E";

    for(size_t x = 0; x < group.size(); ++x){
        gridVecs[group[x].row -1][group[x].column -1] = group[x].symbol;
   }
}

//creating a distance formula to use to find the minimum distance between planets
double distanceFormula(int &x_1, int &y_1, int &x_2, int &y_2 ){
    int stepOne = pow((x_1 - x_2), 2) + pow((y_1 - y_2), 2);
    double stepTwo = sqrt(stepOne);

    return stepTwo; 
}


int main(){

    string locFile; // file user inputs when asked for locations file
    string nameFile; // file user inputs when asked for names file

    Location location; //struct "Location" being put into a variable location
    vector<int> points; //vector of integers used in importantData to get first 3 lines of code
    vector<Location> group; //vector of struct for necessary data in locations.txt
    vector<vector<string> > gridVecs; //vector of vector of strings to make grid

    

    //making necessary vectors for names.txt
    vector<int> nameID;
    vector<string> planetName;
    int ID;
    string pName;
    
    //asking the user to enter location and name file
    cout << "Enter Locations Filename: "; 
    cin >> locFile;
    cout << "Enter Names Filename: ";
    cin >> nameFile;

    //reading in the file the user entered
    ifstream loc(locFile);
    ifstream name(nameFile);
    
    //reading in data from names.txt and pushing it back into vectors
    while(name >> ID >> pName){
        nameID.push_back(ID);
        planetName.push_back(pName);
    }

    //if one of the files does not open, print an error message
    if(!loc.is_open()) {
        cout << "Input file could not be opened" << endl;
        return 1;
    }
    if(!name.is_open()){
        cout << "Input file could not be opened" << endl;
        return 1;
    }
  
  //loading planet data into vectors
    LoadPlanetLocation(group, loc, points);
   
   //changing "XX" and "_" in names.txt
    changeLetters(planetName);
    
    //making grid
   createGridMap(gridVecs, points, group);
   
    //creating journey.txt file
   ofstream fout("journey.txt");

    //print grid to file
    for (int i = 0; i < gridVecs.size(); ++i){ 

        for (int k = 0; k < gridVecs.at(i).size(); ++k){
            fout << gridVecs.at(i).at(k);
        }
        fout << '\n';
    }
  

    fout << "Start at "<<points[2]<<" "<< points[3] <<endl; //starting locations in file

    //create the journey
    int startingLocRow = points[0];
    int startingLocCol = points[1];
    string planet;
    int planetIndex;
    int currentID;
    
    //finding minimum distance between planets using nested loops
    for(size_t i =0; i < group.size(); ++i){
        double minDistance = 200000000.0;
        
            for(size_t j = 0; j < group.size(); ++j){ //loop through all the planets that haven't been visited 
                if(group[j].visited == false){
                    double distance = distanceFormula(startingLocRow, startingLocCol, group.at(j).row, group.at(j).column); // calculating the distances 
                    if(distance < minDistance){
                        minDistance = distance;
                        planetIndex = j;
                        currentID = group[j].ID;

                    }

                    if(distance == minDistance){ //choosing the panet with the smaller ID if the distances are equal 
                        if(group[j].ID < currentID){
                            minDistance = distance;
                            planetIndex = j;
                        }
                    }
                }


            }   
        

        startingLocRow = group[planetIndex].row;
        startingLocCol = group[planetIndex].column;
    
        //making sure one planet isnt visited twice
        group[planetIndex].visited = true;

        for(size_t r = 0; r < nameID.size(); ++r){
            if(group[planetIndex].ID == nameID[r]){
                planet = planetName[r];
            }        
        }
        
            
                    
        fout << "Go to " << planet << " at " << group[planetIndex].row <<" "<< group[planetIndex].column << endl;

                
            
                
    }

    fout << "End at " << points[4] << " "<< points[5] <<endl; // show the ending locations 
    
    fout.close();

   
 
}



