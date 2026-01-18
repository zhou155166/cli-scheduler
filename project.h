#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
//header file for sleep();
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <string>
//header file for hash
#include <openssl/sha.h>
using namespace std;
enum Priority {HIGH,MEDIUM,LOW};
enum Category {STUDY,ENTERTAINMENT,LIFE};
struct Task{
        string taskname;
	string startTime;
	Priority priority = MEDIUM;
	Category category = LIFE;
	string remindTime;
	int id;
};
class TaskManager{
private:
        static vector<Task>tasks;
	static int nextID;
	static string filename;
public:
	static string username;
	static string passwordHash; 
	static Priority stringToPriority(const string &priorityStr);
	static Category stringToCategory(const string &categoryStr);
	static string priorityToString(Priority pri);
	static string categoryToString(Category cat);
	static void hashFunction(const string &password,string &hash);
        static void addTask(const string &taskname,const string &startTime,const Priority &priority,const Category &category,const string &remindTime);
        void saveTasks();
        static void loadTasks();
        static void deleteTask(int id);
        static void showTasks();
        time_t getTimeFromStr(const string &timeStr);
        static void reminderTasks();
};
