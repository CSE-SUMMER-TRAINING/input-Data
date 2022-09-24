#include<bits/stdc++.h>
using namespace std;

const int BRN = 1, BN = 15, FN = 30;

vector<pair<string, int>>BRANCH[BRN][BN][FN];
int FLOOR[BRN][BN], BUILD[BRN];

class Hall
{
    public:
            // atrributes
            int branchNum, buildNum, floorNum, volume;
            string name;
            // methods
            Hall(const int, const int, const int, const int, const string);
};

Hall::Hall(const int brn, const int bn, const int fn, const int v, const string s)
{
    branchNum = brn;
    buildNum = bn;
    floorNum = fn;
    volume = v;
    name = s;
}

class Floor 
{
    public:
            // atrributes
            int branchNum, buildNum, floorNum, numOfHalls, volume;
            vector<Hall>hallsInFloor;
            // methods
            Floor(const int, const int, const int, const int, const int = 0);
            void getHalls();
};

Floor::Floor(const int brn, const int bn, const int fn, const int noh, const int v)
{
    branchNum = brn;
    buildNum = bn;
    floorNum = fn;
    numOfHalls = noh;
    volume = v;
    getHalls();
}

void Floor::getHalls()
{
    for (pair<string, int> hall : BRANCH[branchNum][buildNum][floorNum])
    {
        hallsInFloor.push_back(Hall(branchNum, buildNum, floorNum, hall.second, hall.first));
    }
}

class Build
{
    public: 
            // atrributes
            int branchNum, buildNum, numOfFloors, volume;
            vector<Floor>floorsInBuild;
            // methods
            Build(const int, const int, const int, const int = 0);
            void getFloors();      
};

Build::Build(const int brn, const int bn, const int nof, const int v)
{
    branchNum = brn;
    buildNum = bn;
    numOfFloors = nof;
    volume = v;
    getFloors();
}

void Build::getFloors()
{
    for (int i = 0; i < numOfFloors; i++)
    {
        floorsInBuild.push_back(Floor(branchNum, buildNum, i, BRANCH[branchNum][buildNum][i].size()));
    }
}

class Branch
{
    public:
            // attributes
            int branchNum, numOfBuilds, volume;
            string name;
            vector<Build>buildsInBranch;
            // methods
            Branch(const int, const int, const int = 0);
            void getBuilds();
};

Branch::Branch(const int brn, const int nob, const int v)
{
    branchNum = brn;
    volume = v;
    numOfBuilds = nob;
    getBuilds();
}

void Branch::getBuilds()
{
    for (int i = 0; i < numOfBuilds; i++)
    {
        buildsInBranch.push_back(Build(branchNum, i, FLOOR[branchNum][i] + 1));
    }
}

vector<Branch>college;

void getInputData(int brn)
{
    ifstream File;
    File.open("hallsData.csv");
    if (File.fail())
    {
        cerr << "Error Opening file" << endl;
        exit(1);
    }
    while (File.good())
    {
        string name, volumeStr, buildStr, floorStr, str;
        int volume, build, floor;
        getline(File, name, ',');
        if (name == "end")return;
        getline(File, volumeStr, ',');
        getline(File, buildStr, ',');
        getline(File, floorStr);        
        stringstream geek(volumeStr);
        stringstream geek2(buildStr);
        stringstream geek3(floorStr);
        geek >> volume;
        geek2 >> build;
        geek3 >> floor;
        BRANCH[brn][build][floor].push_back({name, volume});
        FLOOR[brn][build] = max(FLOOR[brn][build], floor);
        BUILD[brn] = max(BUILD[brn], build);
        // cout << name << ' ' << volume << ' ' << build << ' ' << floor << '\n';
    }
    File.close();
    return;
}

void ClacluateVolume()
{
    for (int i = 0; i < BRN; i++)
    {
        int volumeOfBranch = 0;
        vector<Build> & thisBranch = college[i].buildsInBranch;
        for (int j = 0; j < college[i].numOfBuilds; j++)
        {
            int volumeOfBuild = 0;
            Build & thisBuild = thisBranch[j];
            for (int k = 0; k < thisBuild.numOfFloors; k++)
            {
                int volumeOfFloor = 0;
                Floor & thisFloor = thisBuild.floorsInBuild[k];
                for (int z = 0; z < thisFloor.numOfHalls; z++)
                {
                    volumeOfFloor += thisFloor.hallsInFloor[z].volume;
                }
                thisFloor.volume = volumeOfFloor;
                volumeOfBuild += volumeOfFloor;
            }
            thisBuild.volume = volumeOfBuild;
            volumeOfBranch += volumeOfBuild;
        }
        college[i].volume = volumeOfBranch;
    }
}

void printCollege()
{
    for (int i = 0; i < BRN; i++)
    {
        cout << "\t\t>> Branch Num " << i << " <<\n\n";
        vector<Build>thisBranch = college[i].buildsInBranch;
        for (int j = 0; j < college[i].numOfBuilds; j++)
        {
            cout << "BUild Num " << j << '\n';
            Build thisBuild = thisBranch[j];
            for (int k = 0; k < thisBuild.numOfFloors; k++)
            {
                cout << "Floor Num " << k << " : \t";
                Floor thisFloor = thisBuild.floorsInBuild[k];
                for (int z = 0; z < thisFloor.numOfHalls; z++)
                {
                    cout << thisFloor.hallsInFloor[z].name << ' ';
                }
                cout << '\n';
            }
            cout << "\n\n";
        }
    }
}   


int main()
{   
    for (int i = 0; i < BRN; i++)
    {
        getInputData(i);
    }
    for (int i = 0; i < BRN; i++)
    {
        college.push_back(Branch(i, BUILD[i] + 1));
    }
    ClacluateVolume();
    printCollege();
    return 0;
}
