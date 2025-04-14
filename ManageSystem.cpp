#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWARD_LENGTH 20
#define MAX_NAME_LENGTH 20
#define MAX_CLASS_LENGTH 20
#define MAX_SCORES_LENGTH 50
// 细化超过限定范围数目情况 
typedef struct UserNode
{
	char username[MAX_USERNAME_LENGTH];
	char passward[MAX_PASSWARD_LENGTH];
	int role; // 0-学生，1-教师，2-管理员
	struct UserNode *next; 
} UserNode;

typedef struct StudentNode
{
	char name[MAX_NAME_LENGTH];
	char Class[MAX_CLASS_LENGTH];
	int scoreCount;
	double scores[MAX_SCORES_LENGTH];
	struct StudentNode *next;
} StudentNode;

UserNode *userList = NULL;
StudentNode *studentList = NULL;

void loadUsersFromFile()
{
	FILE *file = fopen("users.dat", "rb");
	if (file == NULL)
	{
		return ;
	}
	
	// 常规尾插法，持续更新prev 
	UserNode *prev = NULL;
	while (1)
	{
		UserNode *newNode = (UserNode*)malloc(sizeof(UserNode));
		// 读取完毕 
		if (fread(newNode->username, sizeof(char), MAX_USERNAME_LENGTH, file) != MAX_USERNAME_LENGTH)
		{
			free(newNode);
			break;
		}
		fread(newNode->passward, sizeof(char), MAX_PASSWARD_LENGTH, file);
		fread(&newNode->role, sizeof(int), 1, file);
		newNode->next = NULL;
		
		if (userList == NULL)
		{
			userList = newNode;
		}
		else
		{
			prev->next = newNode;
		}
		prev = newNode;
	}
	fclose(file);
}

void loadStudentsFromFile()
{
	// (如法炮制)
	FILE *file = fopen("students.dat", "rb");
	if (file == NULL)
	{
		return ;
	}
	
	StudentNode *prev = NULL;
	while (1)
	{
		StudentNode *newNode = (StudentNode*)malloc(sizeof(StudentNode));
		if (fread(newNode->name, sizeof(char), MAX_NAME_LENGTH, file) != MAX_NAME_LENGTH)
		{
			free(newNode);
			break;
		}
		fread(newNode->Class, sizeof(char), MAX_CLASS_LENGTH, file);
		fread(newNode->scores, sizeof(double), MAX_SCORES_LENGTH, file);
		fread(&newNode->scoreCount, sizeof(int), 1, file);
		newNode->next = NULL;
		
		if (studentList == NULL)
		{
			studentList = newNode;
		}
		else
		{
			prev->next = newNode;
		}
		prev = newNode;
	}
	fclose(file);
}

void saveUsersToFile()
{
	FILE *file = fopen("users.dat", "wb");
	if (file == NULL)
	{
		printf("打开失败\n");
		return ;
	}
	
	UserNode *curr = userList;
	while (curr != NULL)
	{
		fwrite(curr->username, sizeof(char), MAX_USERNAME_LENGTH, file);
		fwrite(curr->passward, sizeof(char), MAX_PASSWARD_LENGTH, file);
		fwrite(&curr->role, sizeof(int), 1, file);
		curr = curr->next;
	}
	fclose(file);
}

void saveStudentsToFile()
{
	FILE *file = fopen("students.dat", "wb");
	if (file == NULL)
	{
		printf("打开失败\n");
		return ;
	}
	
	StudentNode * curr = studentList;
	while (curr != NULL)
	{
		fwrite(curr->name, sizeof(char), MAX_NAME_LENGTH, file);
        fwrite(curr->Class, sizeof(char), MAX_CLASS_LENGTH, file);
        fwrite(&curr->scoreCount, sizeof(int), 1, file);
        fwrite(curr->scores, sizeof(float), MAX_SCORES_LENGTH, file);
        curr = curr->next;
	}
	fclose(file); 
}

void registerUser()
{
	UserNode *newUser = (UserNode*)malloc(sizeof(UserNode));
	
	printf("输入用户名:");
	scanf("%s", newUser->username);
	
	// 遍历链表，确保账号首次注册 
	UserNode *curr = userList;
	while (curr != NULL)
	{
		if (strcmp(curr->username, newUser->username) == 0)
		{
			printf("用户名已注册\n");
			free(newUser);
			return ;
		}
		curr = curr->next;
	}
	printf("输入密码:");
	scanf("%s", newUser->passward);
	
	// 身份验证细化，防止恶意注册 
	int role;
	do
	{
		printf("输入身份:(0-学生，1-教师，2-管理员)");
		scanf("%d", &role);
		if (role < 0 || role > 2)
		{
			printf("无效输入\n");
		}
		// 循环保证数字输入正确 
	} while (role < 0 || role > 2);
	newUser->role = role;
	newUser->next = userList; // 接入链表 
	userList = newUser;
	
	saveUsersToFile(); // 保存 
	printf("操作成功\n");
}

int loginUser()
{
	char username[MAX_USERNAME_LENGTH];
	char passward[MAX_PASSWARD_LENGTH];
	printf("输入账号:");
	scanf("%s", username);
	printf("输入密码:");
	scanf("%s", passward);
	
	UserNode *curr = userList;
	while (curr != NULL)
	{
		if (strcmp(curr->username, username) == 0 &&
			strcmp(curr->passward, passward) == 0)
			{
				printf("登陆成功\n");
				return curr->role;
			}
	}
	printf("账号密码有误\n");
	return -1;
}

void studentQueryScores()
{
	char className[MAX_CLASS_LENGTH];
	printf("输入班级:");
	scanf("%s", className);
	
	bool check = false;
	StudentNode *curr = studentList;
	while (curr != NULL)
	{
		if (strcmp(curr->Class, className) == 0)
		{
			check = true;
			printf("姓名: %s\n", curr->name);
			printf("成绩: \n");
			for (int i = 0; i < curr->scoreCount; i ++)
			{
				printf("第 %d 个科目成绩为 %.2f\n", i + 1, curr->scores[i]);
			}
			printf("\n"); 
		}
		curr = curr->next;
	}
	if (check == false)
	{
		printf("查无此班\n");
		system("pause"); 
	}
}

void changePassward()
{
	char username[MAX_USERNAME_LENGTH];
	char oldPassward[MAX_PASSWARD_LENGTH];
	char newPassward[MAX_PASSWARD_LENGTH];
	printf("输入用户名:");
	scanf("%s", username);
	printf("输入旧密码:");
	scanf("%s", oldPassward);
	
	UserNode *curr = userList;
	while (curr != NULL)
	{
		if (strcmp(curr->username, username) == 0 &&
			strcmp(curr->passward, oldPassward) == 0)
			{
				printf("请输入新密码:");
				scanf("%s", newPassward);
				strcpy(curr->passward, newPassward);
				printf("修改成功\n");
				return ;
			}
		curr = curr->next;
	}
	printf("账号密码有误\n");
}

void teacherManageStudents()
{
	int choice;
	do
	{
		system("pause");
		system("cls");
		printf("1. 添加学生信息\n");
        printf("2. 删除学生信息\n");
        printf("3. 修改学生信息\n");
        printf("4. 查询学生信息\n");
        printf("5. 查询班内成绩\n");
        printf("6. 返回\n");
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch(choice)
        {
        	case 1:
        	{
        		StudentNode *newStudent = (StudentNode*)malloc(sizeof(StudentNode));
				if (newStudent == NULL)
				{
					printf("内存分配失败\n");
					break;
				}
				
				printf("输入学生姓名: ");
                scanf("%s", newStudent->name);
                printf("输入学生班级: ");
                scanf("%s", newStudent->Class);
                printf("输入成绩数量: ");
                scanf("%d", &newStudent->scoreCount);
                for (int i = 0; i < newStudent->scoreCount; i++)
				{
                    printf("输入第 %d 门成绩: ", i + 1);
                    scanf("%lf", &newStudent->scores[i]);
                }
                newStudent->next = studentList;
                studentList = newStudent;
                saveStudentsToFile();
                printf("添加成功\n");
        		break;
			}
			case 2:
			{
				char name[MAX_NAME_LENGTH];
                printf("输入要删除的学生姓名: ");
                scanf("%s", name);
                
                StudentNode *curr = studentList;
                StudentNode *prev = NULL;
                while (curr != NULL)
                {
                	if (strcmp(curr->name, name) == 0)
					{
                        if (prev == NULL)
						{
                            studentList = curr->next;
                        }
						else
						{
                            prev->next = curr->next;
                        }
                        free(curr);
                        saveStudentsToFile();
                        printf("删除成功\n");
                        break;
                    }
                    prev = curr;
                    curr = curr->next;
				}
				break;
			}
			case 3:
			{
				char name[MAX_NAME_LENGTH];
                printf("输入要修改的学生姓名: ");
                scanf("%s", name);
                
                StudentNode *curr = studentList;
                while (curr != NULL)
				{
                    if (strcmp(curr->name, name) == 0)
					{
                        printf("输入新的学生姓名: ");
                        scanf("%s", curr->name);
                        printf("输入新的学生班级: ");
                        scanf("%s", curr->Class);
                        printf("输入新的成绩数量: ");
                        scanf("%d", &curr->scoreCount);
                        for (int i = 0; i < curr->scoreCount; i++)
						{
                            printf("输入第 %d 门成绩: ", i + 1);
                            scanf("%lf", &curr->scores[i]);
                        }
                        saveStudentsToFile();
                        printf("修改成功\n");
                        break;
                    }
                    curr = curr->next;
                }
                break;
            }
            case 4:
            {
            	char name[MAX_NAME_LENGTH];
                printf("输入要查询的学生姓名: ");
                scanf("%s", name);
                
                StudentNode *curr = studentList;
                while (curr != NULL)
				{
                    if (strcmp(curr->name, name) == 0)
					{
                        printf("姓名: %s\n", curr->name);
                        printf("班级: %s\n", curr->Class);
                        printf("成绩: ");
                        for (int i = 0; i < curr->scoreCount; i++)
						{
                            printf("%.2f ", curr->scores[i]);
                        }
                        printf("\n");
                        break;
                    }
                    curr = curr->next;
                }
                break;
			}
			case 5:
			{
				char className[MAX_CLASS_LENGTH];
                printf("输入班级名称: ");
                scanf("%s", className);
                
                StudentNode *curr = studentList;
                while (curr != NULL)
				{
                    if (strcmp(curr->Class, className) == 0)
					{
                        printf("姓名: %s\n", curr->name);
                        printf("成绩: ");
                        for (int i = 0; i < curr->scoreCount; i++)
						{
                            printf("%.2f ", curr->scores[i]);
                        }
                        printf("\n");
                    }
                    curr = curr->next;
                }
                break;
			}
			case 6:
				printf("返回\n");
				system("pause");
				break;
			default:
				printf("无效输入\n");
		}
	} while (choice != 6);
}

void adminManage()
{
	int choice;
	do
	{
		system("pause");
		system("cls");
        printf("1. 添加学生账号\n");
        printf("2. 删除学生账号\n");
        printf("3. 修改学生账号密码\n");
        printf("4. 查询学生账号信息\n");
        printf("5. 增删改查学生成绩\n");
        printf("6. 导出学生数据\n");
        printf("7. 返回\n");
        printf("请选择操作: ");
        scanf("%d", &choice);
        
		switch (choice)
        {
        	case 1: // 初始账号初始化 
        	{
        		UserNode *newUser = (UserNode*)malloc(sizeof(UserNode));
                if (newUser == NULL)
				{
                    printf("内存分配失败");
                    break;
                }
                
                strcpy(newUser->username, "student");
                int count = 0;
				
                UserNode *curr = userList;
                // 获取初始账号个数 
                while (curr != NULL)
				{
                    if (strstr(curr->username, "student") != NULL)
					{
                        count ++;
                    }
                    curr = curr->next;
                }
                sprintf(newUser->username + 7, "%d", count + 1);
                strcpy(newUser->passward, "123456");
                newUser->role = 0;
				
                newUser->next = userList;
                userList = newUser;
                saveUsersToFile();
                printf("初始账号添加成功\n用户名: %s，初始密码: 123456\n", newUser->username);
                break;
			}
			case 2:
			{
				char username[MAX_USERNAME_LENGTH];
                printf("输入要删除的学生名: ");
                scanf("%s", username);
                
                UserNode *curr = userList;
                UserNode *prev = NULL;
                while (curr != NULL)
				{
                    if (strcmp(curr->username, username) == 0 && curr->role == 0)
					{
                        if (prev == NULL)
						{
                            userList = curr->next;
                        }
						else
						{
                            prev->next = curr->next;
                        }
                        free(curr);
                        
                        saveUsersToFile();
                        printf("删除成功\n");
                        break;
                    }
                    prev = curr;
                    curr = curr->next;
                }
                break;
			}
			case 3:
			{
				char username[MAX_USERNAME_LENGTH];
                char newPassward[MAX_PASSWARD_LENGTH];
                printf("输入要修改的学生名: ");
                scanf("%s", username);
                
                UserNode *curr = userList;
                while (curr != NULL)
				{
                    if (strcmp(curr->username, username) == 0 && curr->role == 0)
					{
                        printf("输入新密码: ");
                        scanf("%s", newPassward);
                        strcpy(curr->passward, newPassward);
                        saveUsersToFile();
                        printf("修改成功\n");
                        break;
                    }
                    curr = curr->next;
                }
                break;
			}
			case 4:
			{
				char username[MAX_USERNAME_LENGTH];
                printf("输入要查询的学生名: ");
                scanf("%s", username);
                
                UserNode *curr = userList;
                while (curr != NULL)
				{
                    if (strcmp(curr->username, username) == 0 && curr->role == 0)
					{
                        printf("用户名: %s，密码: %s\n", curr->username, curr->passward);
                        break;
                    }
                    curr = curr->next;
                }
                break;
			}
			case 5:
			{
				teacherManageStudents(); // 和教师相同功能
				break; 
			}
			case 6:
			{
				FILE *eFile = fopen("stuList.dat", "wb");
                if (eFile == NULL)
				{
                    break;
                }
                
                StudentNode *curr = studentList;
                while (curr != NULL)
				{
                    fwrite(curr->name, sizeof(char), MAX_NAME_LENGTH, eFile);
                    fwrite(curr->Class, sizeof(char), MAX_CLASS_LENGTH, eFile);
                    fwrite(&curr->scoreCount, sizeof(int), 1, eFile);
                    fwrite(curr->scores, sizeof(float), MAX_SCORES_LENGTH, eFile);
                    curr = curr->next;
                }
                fclose(eFile);
                printf("已导出\n");
                break;
			}
			case 7:
				printf("返回\n");
				system("pause");
				break;
			default:
				printf("无效输入\n");
		}
	} while (choice != 7);
}

void mainMenu()
{
	// 未退出则始终循环 
	int choice;
	do
	{
		system("pause");
		system("cls");
		printf("1. 注册\n");
		printf("2. 登录\n");
		printf("3. 修改密码\n");
		printf("4. 退出\n");
		printf("请选择: ");
		scanf("%d", &choice);
		// 根据选择进入不同函数 
		switch (choice)
		{
			case 1:
				registerUser();
				break;
			case 2:
			{
				// 不同返回值进入各自的界面 
				int role = loginUser();
				if (role != -1)
				{
					switch (role)
					{
						case 0:
							studentQueryScores();
							break;
						case 1:
							teacherManageStudents();
							break;
						case 2:
							adminManage();
							break;
					}
				}
				break;
			}
			case 3:
				changePassward();
				break;
			case 4:
				system("cls");
				printf("退出系统\n");
				system("pause");
				break;
			default:
				printf("无效输入，请重试\n");
		}
	} while (choice != 4);
}

// 常规删除链表操作 
void freeList()
{
	UserNode *curr1 = userList;
	while (curr1 != NULL)
	{
		UserNode *tmp1 = curr1;
		curr1 = curr1->next;
		free(tmp1);
	}
	
	StudentNode *curr2 = studentList;
	while (curr2 != NULL)
	{
		StudentNode *tmp2 = curr2;
		curr2 = curr2->next;
		free(tmp2);
	}
}

int main()
{
	// 事先加载所有信息，便于后续使用
	// 实际为读取文件存入链表 
	loadUsersFromFile();
	loadStudentsFromFile();
	
	// 主页面 
	mainMenu();
	// 释放内存 
	freeList();
	
	return 0;
}
