#include<bits/stdc++.h>
using namespace std;

map<string, int>mp;

vector<Group>group;

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

int ID = 0;

void getGroups()
{
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
        group.push_back(Group(ID, volume, from, to, mp[branch], name));
        ID++;
        // cout << name << ' ' << branch << ' ' << volume << ' ' << from << ' ' << to << '\n';
    }
    File.close();
}

int main()
{
    getGroups();
}