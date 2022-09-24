#include<bits/stdc++.h>
using namespace std;

const int BRN = 1, BN = 15, FN = 30;

vector<pair<string, int>>BRANCH[BRN][BN][FN];
int FLOOR[BRN][BN], BUILD[BRN];
map<string, int>mp;
map<int, string>mp2;

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
            vector<int>groupsInBranch;
            // methods
            Branch(const string, const int, const int, const int = 0);
            void getBuilds();
};

Branch::Branch(const string s, const int brn, const int nob, const int v)
{
    name = s;
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


class Group
{
    public: 
            // attributes
            int id, volume, from, to, branchNum;
            string name;
            // methods
            Group(const int, const int, const int, const int, const int, const string);
};

Group::Group(const int d, const int v, const int f, const int t, const int brn, const string s)
{
    id = d;
    volume = v;
    from = f;
    to = t;
    branchNum = brn;
    name = s;
}

vector<Branch>college;
vector<Group>group;

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
        string name, volumeStr, buildStr, floorStr;
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

void getGroups()
{
    int ID = 0;
    ifstream File;
    File.open("studyGroupsData.csv");
    if (File.fail())
    {
        cerr << "Error Opening file" << endl;
        exit(1);
    }
    while (File.good())
    {
        string name, branch, volumeStr, fromStr, toStr;
        int volume, from, to;
        getline(File, name, ',');
        if (name == "end")return;
        getline(File, branch, ',');
        getline(File, volumeStr, ',');
        getline(File, fromStr, ',');
        getline(File, toStr);
        stringstream geek(volumeStr);
        stringstream geek2(fromStr);
        stringstream geek3(toStr);
        geek >> volume;
        geek2 >> from;
        geek3 >> to;
        int brn = mp[branch];
        group.push_back(Group(ID, volume, from, to, brn, name));
        college[brn].groupsInBranch.push_back(ID);
        ID++;
        // cout << name << ' ' << branch << ' ' << volume << ' ' << from << ' ' << to << '\n';
    }
    File.close();
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

void printGroups()
{
    for (Group gg : group)
    {
        cout << college[gg.branchNum].name << ' ' << gg.id << ' ' << gg.name << '\n';
    }
}

void printGroupsInBranches()
{
    for (Branch bb : college)
    {
        for (int id : bb.groupsInBranch)
        {
            cout << group[id].name << ' ';
        }
        cout << '\n';
    }
}

int main()
{  
    int numOfBranches;
    cin >> numOfBranches;
    for (int i = 0; i < numOfBranches; i++)
    {
        string nameOfBranch;
        cin >> nameOfBranch;
        mp[nameOfBranch] = i;
        mp2[i] = nameOfBranch;
    }
    for (int i = 0; i < BRN; i++)
    {
        getInputData(i);
    }
    for (int i = 0; i < BRN; i++)
    {
        college.push_back(Branch(mp2[i], i, BUILD[i] + 1));
    }
    ClacluateVolume();
    getGroups();
    cout << '\n';
    cout << '\n';
    printCollege();
    cout << '\n';
    cout << '\n';
    printGroups();
    cout << '\n';
    cout << '\n';
    printGroupsInBranches();
    return 0;
}