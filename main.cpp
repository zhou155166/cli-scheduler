#include "project.h"
//define lock variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//determine if the project stopped
bool stopped = false;
void *inputoutputThread(void *arg){
	TaskManager* taskManager = static_cast<TaskManager*>(arg);
	cout << "I/O begin" << endl;
	string password;
        cout << "Please send your name and password for save" << endl;
        cin >> TaskManager::username >> password;
        TaskManager::hashFunction(password,TaskManager::passwordHash);
        cout << "Having save your name and password: " << endl;
	cout << "your name: "<< TaskManager::username << " your passwordHash: " << TaskManager::passwordHash << endl;
	while(!stopped){
	                cout << "Please send your name,password and your order,order as below: " << endl;
	                cout << "1.addtask <taskname> startTime(year,month,day,hour,minute,second) priority(HIGH/MEDIUM/LOW) category(STUDY,ENTERTAINMENT,LIFE) remindTime(year,month,day,hour,minute,second)" << endl;
	                cout << "2.loadtasks" << endl;
	                cout << "3.deltask <id>" << endl;
	                cout << "4.showtasks" << endl;
	                cout << "5.quit" << endl;
			string name, key, keyHash;
                        cin >> name >> key;
                        TaskManager::hashFunction(key,keyHash);
                        while(name != TaskManager::username ||keyHash != TaskManager::passwordHash){
                                cout << "You don't send the right name and password, please input again" << endl;
				cin >> name >> key;
                                TaskManager::hashFunction(key,keyHash);
                                if (name == TaskManager::username && keyHash == TaskManager::passwordHash){
                                        break;
				}else{
                                        continue;
                                }
			}
                        cout <<"Begin to function your order" << endl;
                        string command;
                        cin >> command;
		        Task task;
		        if(command == "quit"){
		               cout << "Quit successfully" << endl;
			       stopped = true;
		        }else if(command == "addtask"){
		                string priorityStr,categoryStr;
			        cin >> task.taskname >> task.startTime >> priorityStr >> categoryStr >> task.remindTime;
			        task.priority = TaskManager::stringToPriority(priorityStr);
			        task.category = TaskManager::stringToCategory(categoryStr);
			        pthread_mutex_lock(&mutex);
			        TaskManager::addTask(task.taskname,task.startTime,task.priority,task.category,task.remindTime);
			        pthread_mutex_unlock(&mutex);
		        }else if(command == "loadtasks"){
		                TaskManager::loadTasks();
		        }else if(command == "deltask"){
		                int ID;
		                cin >> ID;
		                pthread_mutex_lock(&mutex);
		                TaskManager::deleteTask(ID);
		                pthread_mutex_unlock(&mutex);
		        }else if(command == "showtasks"){
		                pthread_mutex_lock(&mutex);
		                TaskManager::showTasks();
		                pthread_mutex_unlock(&mutex);
		        }else{
		                cout << "Invalid input:please send again" << endl; 
		        }
        }
	return NULL;
}
void *reminderThread(void *arg){
	TaskManager* taskManager = static_cast<TaskManager*>(arg);
	cout << "reminder begin" << endl;
        while(!stopped){
                pthread_mutex_lock(&mutex);
                TaskManager::reminderTasks();
                pthread_mutex_unlock(&mutex);
                sleep(5);
        }
	return NULL;
}
string TaskManager::username;
string TaskManager::passwordHash;
int main(int argc,char *argv[]){
        if(argc != 2){
                cout << "Usage: " << argv[0] << " run" << endl;
                return 1;
        }
        string input = argv[1];
        if(argc == 2 && input == "run"){
		TaskManager taskManager;
                pthread_t inputoutputThreadId,reminderThreadId;
                pthread_create(&inputoutputThreadId,NULL,inputoutputThread,&taskManager);
		pthread_create(&reminderThreadId,NULL,reminderThread,&taskManager);
                pthread_join(inputoutputThreadId,NULL);
                pthread_join(reminderThreadId,NULL);
        }else{
                cout << "Invalid input: please send " << argv[0] << " run" << endl;
        }
        return 0;
}

