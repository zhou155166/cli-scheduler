#include "project.h"
string TaskManager::filename = "tasks.txt";
vector<Task>TaskManager::tasks;
int TaskManager::nextID =1;
//function of save Hash
void TaskManager::hashFunction(const string &password,string &hash){
	unsigned char temp[SHA_DIGEST_LENGTH];
	SHA1((const unsigned char*)password.c_str(),password.size(),temp);
	char buf[SHA_DIGEST_LENGTH*2+1];
	buf[SHA_DIGEST_LENGTH*2] = '\0';
	for(int i = 0; i < SHA_DIGEST_LENGTH; i++){
		sprintf(&buf[i*2],"%02x",(unsigned int)temp[i]);
	}
	hash = buf;
}
string TaskManager::priorityToString(Priority pri){
        switch(pri){
                case Priority::HIGH:return "HIGH";
                case Priority::MEDIUM:return "MEDIUM";
                case Priority::LOW:return "LOW";
                default:return "UNKOWN";
        }
}
string TaskManager::categoryToString(Category cat){
        switch(cat){
                case Category::STUDY:return "STUDY";
                case Category::ENTERTAINMENT:return "ENTERTAINMENT";
                case Category::LIFE:return "LIFE";
                default:return "UNKOWN";
        }
}
Priority TaskManager::stringToPriority(const string &priorityStr){
        if(priorityStr == "HIGH"){
                        return HIGH;        
        }else if(priorityStr == "MEDIUM"){
                        return MEDIUM;
        }else if(priorityStr == "LOW"){
                        return LOW;
        }
}
Category TaskManager::stringToCategory(const string &categoryStr){
        if(categoryStr == "STUDY"){
                        return STUDY;        
        }else if(categoryStr == "ENTERTAINMENT"){
                        return ENTERTAINMENT;
        }else if(categoryStr == "LIFE"){
                        return LIFE;
        }
}
void TaskManager::saveTasks(){        
        ofstream file(filename);
        if(file.is_open()){
               for(const Task &task:tasks){
                       file << task.id << " " << task.taskname << " " << task.startTime << " " << task.priority << " " << task.category << " " << task.remindTime << endl;
               }
               file.close();
               cout << "Having save to file" << endl;
        }else{
               cout << "Unable to open file for save" << endl;
        }
}
void TaskManager::addTask(const string &taskname,const string &startTime,const Priority &priority,const Category &category,const string &remindTime){
        Task newTask;
        newTask.id = nextID++;
        newTask.taskname = taskname;
        newTask.startTime = startTime;
        newTask.priority = priority;
        newTask.category = category;
        newTask.remindTime = remindTime;
        for(const Task &task:tasks){
                if(task.taskname == newTask.taskname || task.startTime == newTask.startTime){
                        cout << "Error:the task already exists" << endl; 
                        return;
                }
        }
        tasks.push_back(newTask);
	cout << "Having add:" << endl;
	string priorityfromInt = priorityToString(priority);
	string categoryfromInt = categoryToString(category);
	cout << "id: " << newTask.id << " name: " << newTask.taskname << " startTime: " << newTask.startTime << " priority: " << priorityfromInt << " category: " << categoryfromInt << " remindTime: " << newTask.remindTime << endl;
	TaskManager manager;
	manager.saveTasks();
}
void TaskManager::loadTasks(){ 
        ifstream file(filename);
        if(file.is_open()){
               tasks.clear();
               string line;
               
               while(getline(file,line)){
                      istringstream iss(line);
                      Task taskload;
                      int priorityInt,categoryInt;
                      iss >> taskload.id >> taskload.taskname >> taskload.startTime >> priorityInt >> categoryInt >> taskload.remindTime;
                      taskload.priority = static_cast<Priority>(priorityInt);
                      taskload.category = static_cast<Category>(categoryInt);
                      string priorityfromInt = priorityToString(taskload.priority);
	              string categoryfromInt = categoryToString(taskload.category);
                      cout << "Loading: " << taskload.id << " " << taskload.taskname << " " << taskload.startTime << " " << priorityfromInt << " " << categoryfromInt << " " << taskload.remindTime << endl;
                      tasks.push_back(taskload);
               }
               file.close();
               cout << "Having successfully loading from file" << endl;
        }else{
               cout << "Unable to open file for load" << endl;
        }
}
void TaskManager::deleteTask(int id){
        auto it = tasks.begin();
        while(it != tasks.end()){
                    if(it->id == id){
                                   cout << "Deleting task id: " << it->id << " taskname: " << it->taskname << endl;
                                   it = tasks.erase(it);
                                   TaskManager Manager;
	                           Manager.saveTasks();
                                   return;
                    }else{
                                   ++it;
                    }
        }
        cout << "Unable to find task with id:" << id << endl;
}
void TaskManager::showTasks(){
        sort(tasks.begin(),tasks.end(),[](const Task &a,const Task &b){return a.startTime < b.startTime;});
        cout << "Showing by the turn of starttime: " << endl;
        for(const Task &task:tasks){
                    cout << "id: " << task.id << " taskname: " << task.taskname << " starttime: " << task.startTime << " priority: " << priorityToString(task.priority) << " category: " << categoryToString(task.category) << " remindtime: " << task.remindTime << endl;
        }
}
time_t TaskManager::getTimeFromStr(const string &timeStr){
        struct tm t;
        strptime(timeStr.c_str(),"%Y%m%d%H%M%S",&t);
        return mktime(&t);
}
void TaskManager::reminderTasks(){
        //get the current time
        time_t currentTime = time(NULL);
        struct tm *now = localtime(&currentTime);
        char current[15];
        strftime(current,15,"%Y%m%d%H%M%S",now);
        for(const Task &task:tasks){
                   //check if it is soon before remindTime
                   TaskManager Manager;
                   time_t remindTime = Manager.getTimeFromStr(task.remindTime);
                   time_t BeforeRemindTimeLower = remindTime - 6;
                   time_t BeforeRemindTimeHigher = remindTime;
                   if(currentTime >= BeforeRemindTimeLower && currentTime <= BeforeRemindTimeHigher){
                           cout << "Reminder:task " << task.taskname << " will be done later!" << endl;
                   }
        }
}
