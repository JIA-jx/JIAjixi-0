#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWARD_LENGTH 20
#define MAX_NAME_LENGTH 20
#define MAX_CLASS_LENGTH 20
#define MAX_SCORES_LENGTH 50
// ϸ�������޶���Χ��Ŀ��� 
typedef struct UserNode
{
	char username[MAX_USERNAME_LENGTH];
	char passward[MAX_PASSWARD_LENGTH];
	int role; // 0-ѧ����1-��ʦ��2-����Ա
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
	
	// ����β�巨����������prev 
	UserNode *prev = NULL;
	while (1)
	{
		UserNode *newNode = (UserNode*)malloc(sizeof(UserNode));
		// ��ȡ��� 
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
	// (�編����)
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
		printf("��ʧ��\n");
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
		printf("��ʧ��\n");
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
	
	printf("�����û���:");
	scanf("%s", newUser->username);
	
	// ��������ȷ���˺��״�ע�� 
	UserNode *curr = userList;
	while (curr != NULL)
	{
		if (strcmp(curr->username, newUser->username) == 0)
		{
			printf("�û�����ע��\n");
			free(newUser);
			return ;
		}
		curr = curr->next;
	}
	printf("��������:");
	scanf("%s", newUser->passward);
	
	// �����֤ϸ������ֹ����ע�� 
	int role;
	do
	{
		printf("�������:(0-ѧ����1-��ʦ��2-����Ա)");
		scanf("%d", &role);
		if (role < 0 || role > 2)
		{
			printf("��Ч����\n");
		}
		// ѭ����֤����������ȷ 
	} while (role < 0 || role > 2);
	newUser->role = role;
	newUser->next = userList; // �������� 
	userList = newUser;
	
	saveUsersToFile(); // ���� 
	printf("�����ɹ�\n");
}

int loginUser()
{
	char username[MAX_USERNAME_LENGTH];
	char passward[MAX_PASSWARD_LENGTH];
	printf("�����˺�:");
	scanf("%s", username);
	printf("��������:");
	scanf("%s", passward);
	
	UserNode *curr = userList;
	while (curr != NULL)
	{
		if (strcmp(curr->username, username) == 0 &&
			strcmp(curr->passward, passward) == 0)
			{
				printf("��½�ɹ�\n");
				return curr->role;
			}
	}
	printf("�˺���������\n");
	return -1;
}

void studentQueryScores()
{
	char className[MAX_CLASS_LENGTH];
	printf("����༶:");
	scanf("%s", className);
	
	bool check = false;
	StudentNode *curr = studentList;
	while (curr != NULL)
	{
		if (strcmp(curr->Class, className) == 0)
		{
			check = true;
			printf("����: %s\n", curr->name);
			printf("�ɼ�: \n");
			for (int i = 0; i < curr->scoreCount; i ++)
			{
				printf("�� %d ����Ŀ�ɼ�Ϊ %.2f\n", i + 1, curr->scores[i]);
			}
			printf("\n"); 
		}
		curr = curr->next;
	}
	if (check == false)
	{
		printf("���޴˰�\n");
		system("pause"); 
	}
}

void changePassward()
{
	char username[MAX_USERNAME_LENGTH];
	char oldPassward[MAX_PASSWARD_LENGTH];
	char newPassward[MAX_PASSWARD_LENGTH];
	printf("�����û���:");
	scanf("%s", username);
	printf("���������:");
	scanf("%s", oldPassward);
	
	UserNode *curr = userList;
	while (curr != NULL)
	{
		if (strcmp(curr->username, username) == 0 &&
			strcmp(curr->passward, oldPassward) == 0)
			{
				printf("������������:");
				scanf("%s", newPassward);
				strcpy(curr->passward, newPassward);
				printf("�޸ĳɹ�\n");
				return ;
			}
		curr = curr->next;
	}
	printf("�˺���������\n");
}

void teacherManageStudents()
{
	int choice;
	do
	{
		system("pause");
		system("cls");
		printf("1. ���ѧ����Ϣ\n");
        printf("2. ɾ��ѧ����Ϣ\n");
        printf("3. �޸�ѧ����Ϣ\n");
        printf("4. ��ѯѧ����Ϣ\n");
        printf("5. ��ѯ���ڳɼ�\n");
        printf("6. ����\n");
        printf("��ѡ�����: ");
        scanf("%d", &choice);
        
        switch(choice)
        {
        	case 1:
        	{
        		StudentNode *newStudent = (StudentNode*)malloc(sizeof(StudentNode));
				if (newStudent == NULL)
				{
					printf("�ڴ����ʧ��\n");
					break;
				}
				
				printf("����ѧ������: ");
                scanf("%s", newStudent->name);
                printf("����ѧ���༶: ");
                scanf("%s", newStudent->Class);
                printf("����ɼ�����: ");
                scanf("%d", &newStudent->scoreCount);
                for (int i = 0; i < newStudent->scoreCount; i++)
				{
                    printf("����� %d �ųɼ�: ", i + 1);
                    scanf("%lf", &newStudent->scores[i]);
                }
                newStudent->next = studentList;
                studentList = newStudent;
                saveStudentsToFile();
                printf("��ӳɹ�\n");
        		break;
			}
			case 2:
			{
				char name[MAX_NAME_LENGTH];
                printf("����Ҫɾ����ѧ������: ");
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
                        printf("ɾ���ɹ�\n");
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
                printf("����Ҫ�޸ĵ�ѧ������: ");
                scanf("%s", name);
                
                StudentNode *curr = studentList;
                while (curr != NULL)
				{
                    if (strcmp(curr->name, name) == 0)
					{
                        printf("�����µ�ѧ������: ");
                        scanf("%s", curr->name);
                        printf("�����µ�ѧ���༶: ");
                        scanf("%s", curr->Class);
                        printf("�����µĳɼ�����: ");
                        scanf("%d", &curr->scoreCount);
                        for (int i = 0; i < curr->scoreCount; i++)
						{
                            printf("����� %d �ųɼ�: ", i + 1);
                            scanf("%lf", &curr->scores[i]);
                        }
                        saveStudentsToFile();
                        printf("�޸ĳɹ�\n");
                        break;
                    }
                    curr = curr->next;
                }
                break;
            }
            case 4:
            {
            	char name[MAX_NAME_LENGTH];
                printf("����Ҫ��ѯ��ѧ������: ");
                scanf("%s", name);
                
                StudentNode *curr = studentList;
                while (curr != NULL)
				{
                    if (strcmp(curr->name, name) == 0)
					{
                        printf("����: %s\n", curr->name);
                        printf("�༶: %s\n", curr->Class);
                        printf("�ɼ�: ");
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
                printf("����༶����: ");
                scanf("%s", className);
                
                StudentNode *curr = studentList;
                while (curr != NULL)
				{
                    if (strcmp(curr->Class, className) == 0)
					{
                        printf("����: %s\n", curr->name);
                        printf("�ɼ�: ");
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
				printf("����\n");
				system("pause");
				break;
			default:
				printf("��Ч����\n");
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
        printf("1. ���ѧ���˺�\n");
        printf("2. ɾ��ѧ���˺�\n");
        printf("3. �޸�ѧ���˺�����\n");
        printf("4. ��ѯѧ���˺���Ϣ\n");
        printf("5. ��ɾ�Ĳ�ѧ���ɼ�\n");
        printf("6. ����ѧ������\n");
        printf("7. ����\n");
        printf("��ѡ�����: ");
        scanf("%d", &choice);
        
		switch (choice)
        {
        	case 1: // ��ʼ�˺ų�ʼ�� 
        	{
        		UserNode *newUser = (UserNode*)malloc(sizeof(UserNode));
                if (newUser == NULL)
				{
                    printf("�ڴ����ʧ��");
                    break;
                }
                
                strcpy(newUser->username, "student");
                int count = 0;
				
                UserNode *curr = userList;
                // ��ȡ��ʼ�˺Ÿ��� 
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
                printf("��ʼ�˺���ӳɹ�\n�û���: %s����ʼ����: 123456\n", newUser->username);
                break;
			}
			case 2:
			{
				char username[MAX_USERNAME_LENGTH];
                printf("����Ҫɾ����ѧ����: ");
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
                        printf("ɾ���ɹ�\n");
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
                printf("����Ҫ�޸ĵ�ѧ����: ");
                scanf("%s", username);
                
                UserNode *curr = userList;
                while (curr != NULL)
				{
                    if (strcmp(curr->username, username) == 0 && curr->role == 0)
					{
                        printf("����������: ");
                        scanf("%s", newPassward);
                        strcpy(curr->passward, newPassward);
                        saveUsersToFile();
                        printf("�޸ĳɹ�\n");
                        break;
                    }
                    curr = curr->next;
                }
                break;
			}
			case 4:
			{
				char username[MAX_USERNAME_LENGTH];
                printf("����Ҫ��ѯ��ѧ����: ");
                scanf("%s", username);
                
                UserNode *curr = userList;
                while (curr != NULL)
				{
                    if (strcmp(curr->username, username) == 0 && curr->role == 0)
					{
                        printf("�û���: %s������: %s\n", curr->username, curr->passward);
                        break;
                    }
                    curr = curr->next;
                }
                break;
			}
			case 5:
			{
				teacherManageStudents(); // �ͽ�ʦ��ͬ����
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
                printf("�ѵ���\n");
                break;
			}
			case 7:
				printf("����\n");
				system("pause");
				break;
			default:
				printf("��Ч����\n");
		}
	} while (choice != 7);
}

void mainMenu()
{
	// δ�˳���ʼ��ѭ�� 
	int choice;
	do
	{
		system("pause");
		system("cls");
		printf("1. ע��\n");
		printf("2. ��¼\n");
		printf("3. �޸�����\n");
		printf("4. �˳�\n");
		printf("��ѡ��: ");
		scanf("%d", &choice);
		// ����ѡ����벻ͬ���� 
		switch (choice)
		{
			case 1:
				registerUser();
				break;
			case 2:
			{
				// ��ͬ����ֵ������ԵĽ��� 
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
				printf("�˳�ϵͳ\n");
				system("pause");
				break;
			default:
				printf("��Ч���룬������\n");
		}
	} while (choice != 4);
}

// ����ɾ��������� 
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
	// ���ȼ���������Ϣ�����ں���ʹ��
	// ʵ��Ϊ��ȡ�ļ��������� 
	loadUsersFromFile();
	loadStudentsFromFile();
	
	// ��ҳ�� 
	mainMenu();
	// �ͷ��ڴ� 
	freeList();
	
	return 0;
}
