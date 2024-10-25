#include <map>
#include "generateUsers.hpp"
using namespace std;


map<string, User> buildMapByUserName(vector<User> users) {
    map<string, User> usersMap;
    
    for (const auto& user : users) {
        usersMap[user.userName] = user;
    }
    
    return usersMap;
}


map<string, User> buildMapByEmail(vector<User> users) {
    map<string, User> usersMap;
    
    for (const auto& user : users) {
        usersMap[user.email] = user;
    }
    
    return usersMap;
}


void printMap(map<string, User> aMap) {
    for (const auto& pair : aMap) {
        cout << "Key: " << pair.first << " -> ";
        printAUser(pair.second);
    }
}

bool isMapSorted(map<string, User> aMap) {
    return true;
}


bool testSearchByKey(map<string, User> aMap, string keyToSearch) {
    auto it = aMap.find(keyToSearch);
    return it != aMap.end();
}


bool testDeleteByKey(map<string, User> aMap, string keyToDelete) {
    size_t numErased = aMap.erase(keyToDelete);
    return numErased > 0;
}


void printActiveUsers(map<string, User> aMap) {
    int activeThreshold = 800;
    for(const auto& pair : aMap){
        if(pair.second.numPosts > activeThreshold)
        {
            printAUser(pair.second);
        }
    }
}


void printMostPopularCategory(map<string, User> aMap) {    
    map<string, int> categoryCount;

    for(const auto&pair : aMap){
    categoryCount[pair.second.mostViewedCategory]++;
    }

    string mostPopular;
    int maxCount = 0;

    for(const auto& category : categoryCount)
    {
        if(category.second > maxCount)
        {
            maxCount = category.second;
            mostPopular = category.first;
        }
    }
}


int main()
{
    int numUsers = 10;
    vector<User> users = generateUsers(numUsers);


    cout << "Build map with username as key" << endl;
    map<string, User> mapByUserName = buildMapByUserName(users);
    if ( mapByUserName.size() == numUsers )
        cout << "  Built successfully." << endl << endl;
    else
        cout << "  Built unsuccessfully." << endl << endl;


    cout << "Print \"mapByUserName\" map:" << endl;
    printMap(mapByUserName);
    cout << endl;


    string keyToSearch = "smith55";
    cout << "Search by key: mapByUserName[\"" << keyToSearch << "\"]" << endl;
    if ( testSearchByKey(mapByUserName, keyToSearch) )
        cout << "  Search successfully." << endl << endl;
    else
        cout << "  Search unsuccessfully." << endl << endl;


    string keyToDelete = "smith55";
    cout << "Delete by key: \"" << keyToDelete << "\"" << endl;
    if ( testDeleteByKey(mapByUserName, keyToDelete) )
        cout << "  Delete successfully." << endl << endl;
    else
        cout << "  Delete unsuccessfully." << endl << endl;


    cout << "Test if map's key is sorted" << endl;
    if ( isMapSorted(mapByUserName) )
        cout << "  Order test passed!" << endl << endl;
    else
        cout << "  Order test failed!" << endl << endl;


    cout << "Print usernames with more than 800 tweets:" << endl;
    printActiveUsers(mapByUserName);
    cout << endl;

    cout << "Print the most popular category" << endl;
    printMostPopularCategory(mapByUserName);
    cout << endl;


    cout << " ============================================================================== " << endl << endl;


    cout << "Build map with username as key" << endl;
    map<string, User> mapByEmail = buildMapByEmail(users);
    if ( mapByEmail.size() == numUsers )
        cout << "  Built successfully." << endl << endl;
    else
        cout << "  Built unsuccessfully." << endl << endl;
    

    keyToSearch = "kat@gmail.com";
    cout << "Search by key: mapByEmail[\"" << keyToSearch << "\"]" << endl;
    if ( testSearchByKey(mapByEmail, keyToSearch) )
        cout << "  Search successfully." << endl << endl;
    else
        cout << "  Search unsuccessfully." << endl << endl;


    keyToDelete = "kat@gmail.com";
    cout << "Delete by key: \"" << keyToDelete << "\"" << endl;
    if ( testDeleteByKey(mapByEmail, keyToDelete) )
        cout << "  Delete successfully." << endl << endl;
    else
        cout << "  Delete unsuccessfully." << endl << endl;


    return 0;
}