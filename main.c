//abdallah kokash---1220116----sec3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define heapCapacity 50
//definitions
typedef struct node *list;
typedef struct node *node;
typedef struct minHeap *heap;
typedef struct nodeCourse *coursesList;
typedef struct nodeCourse *courseNode;
///////////////////////////////////////////////////
//structure
struct node {
    char building[50];
    int cost;
    struct node *nextDest;
    struct node *nextBuilding;
};

struct nodeCourse {
    char course[50];
    struct nodeCourse *nextCourse;
    struct nodeCourse *nextPrerequisites;
    int deleted;
    int indegree;
};

struct minHeap {
    struct heapNode *heap;
    int size;
    int capacity;
};

struct heapNode {
    int cost;
    node vertix;
    char pathVia[50];
};

///////////////////////////////////////////////////////
//global variables
list adjacenctList = NULL;
heap minHeap = NULL;
coursesList adjacentlistCourses = NULL;
struct heapNode result[heapCapacity];
char sort[50][50];
int sortSize = 0;
int resultSize;
int flag = 0;
int flag1 = 0;
int flag3 = 0;
////////////////////////////////////////////////////
//functions
node search(list l, char building[50]);

int duplicate(node loc, char distination[50]);

list makeEmpty(list l);

void insert(list l, char building[50], char destination[50], int cost);

void load();

heap makeHeap();

void enqueue(char src[50]);

int searchHeap(char key[50]);

void swap(int a, int b);

void readjuctHeap(int loc);

void dequeue();

void shortestPath(char src[50], char dest[50]);

void shortestRoute(char src[50], char dest[50]);

coursesList makeCousreList(coursesList l);

int duplicateCourse(courseNode loc, char prerequisites[50]);

courseNode searchCourse(coursesList l, char course[50]);

void insertCours(coursesList l, char course[50], char prerequisites[50]);

void calculateInDegree();

void topologicalSort();

//////////////////////////////////////////////////////

int main(void) {
    int op;
    adjacenctList = makeEmpty(NULL);
    minHeap = makeHeap();
    adjacentlistCourses = makeCousreList(NULL);
    do {
        printf("\nmenu\n");
        printf("1.Load the two files.\n");
        printf("2.Calculate the shortest distance between two buildings.\n");
        printf("3.Print the shortest route between two buildings and the total distance.\n");
        printf("4.Sort the courses using the topological sort.\n");
        printf("5.Print the sorted courses.\n");
        printf("6.Exit the application.\n");
        printf("select an operation: ");
        //here we check if the input entered by the user is not a character
        while (scanf("%d", &op) != 1) {
            while (getchar() != '\n') {
            }
            printf("invalid input\n");
        }
        switch (op) {
            case 1:
                load();
                break;
            case 2:
                printf("\n");
                if (flag == 1) {
                    printf("enter the sourse building\n");
                    char src[50];
                    scanf("%s", src);
                    printf("enter the distination building\n");
                    char dest[50];
                    scanf("%s", dest);
                    shortestPath(src, dest);
                } else
                    printf("no input has been inserted\n");

                break;
            case 3:
                printf("\n");
                if (flag == 1) {
                    printf("enter the sourse building\n");
                    char src[50];
                    scanf("%s", src);
                    printf("enter the distination building\n");
                    char dest[50];
                    scanf("%s", dest);
                    shortestRoute(src, dest);
                } else
                    printf("no input has been inserted\n");
                break;
            case 4:
                topologicalSort();
                break;
            case 5:
                printf("\n");
                if (flag3 == 1) {
                    for (int i = 0; i < sortSize; i++) {
                        printf("%s", sort[i]);
                        if (i != sortSize - 1)
                            printf("->");
                    }
                } else
                    printf("courses are still not sorted\n");
                break;
            case 6:
                printf("exiting system.....\n");
                break;
            default:
                printf("not a correct operation!!...please enter a valid operation\n");
                break;
        }
    } while (op != 6);
}
//load function that will load the two files
void load() {
    printf("\n");
    FILE *input_file = fopen("input_buildings.txt", "r");
    if (input_file == NULL) {
        printf("error opening file\n");
    } else {
        char building[50];
        char destination[50];
        int cost;
        char line[100];
        while (fgets(line, sizeof(line), input_file) != NULL) {
            char *ptr = strtok(line, "#");
            strcpy(building, ptr);
            ptr = strtok(NULL, "#");
            strcpy(destination, ptr);
            ptr = strtok(NULL, "#");
            cost = atoi(ptr);
            insert(adjacenctList, building, destination, cost);
        }
        printf("load buildings was successfull\n");
        fclose(input_file);
        flag = 1;
    }
    input_file = fopen("input_courses.txt", "r");
    if (input_file == NULL) {
        printf("error opening file\n");
    } else {
        char line[100];
        char course[50];
        char prerequisites[50];
        while (fgets(line, sizeof(line), input_file) != NULL) {
            int len = strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
            char *ptr = strtok(line, "#");
            strcpy(course, ptr);
            while ((ptr = strtok(NULL, "#")) != NULL) {
                strcpy(prerequisites, ptr);
                insertCours(adjacentlistCourses, course, prerequisites);
            }
        }
        printf("load courses was successfull\n");
        fclose(input_file);
        flag = 1;
    }
}
//function that will calculate all the indeggres for the nodes
void calculateInDegree() {
    courseNode course = adjacentlistCourses->nextCourse;
    while (course != NULL) {
        course->deleted = 0;
        course->indegree = 0;
        course = course->nextCourse;
    }
    course = adjacentlistCourses->nextCourse;
    while (course != NULL) {
        courseNode preq = course->nextPrerequisites;
        while (preq != NULL) {
            courseNode ch = searchCourse(adjacentlistCourses, preq->course);
            ch->indegree++;
            preq = preq->nextPrerequisites;
        }
        course = course->nextCourse;
    }
}
//function that will sort the courses
void topologicalSort() {
    printf("\n");
    if (flag3 == 1)
        printf("courses already sorted\n");
    if (flag == 1 && flag3 == 0) {
        calculateInDegree();
        int totalCourses = 0;
        courseNode temp = adjacentlistCourses->nextCourse;
        while (temp != NULL) {
            totalCourses++;
            temp = temp->nextCourse;
        }
        courseNode *queue = malloc(sizeof(struct nodeCourse) * totalCourses);
        if (queue == NULL) {
            printf("memory is full");
            exit(1);
        }
        int front = 0;
        int rear = 0;
        courseNode course = adjacentlistCourses->nextCourse;
        while (course != NULL) {
            if (course->indegree == 0 && course->deleted == 0) {
                queue[rear++] = course;
            }
            course = course->nextCourse;
        }
        while (front < rear) {
            courseNode p = queue[front++];
            strcpy(sort[sortSize], p->course);
            sortSize++;
            totalCourses--;
            p->deleted = 1;
            courseNode preq = p->nextPrerequisites;
            while (preq != NULL) {
                courseNode ch = searchCourse(adjacentlistCourses, preq->course);
                if (ch->deleted == 0 && --ch->indegree == 0)
                    queue[rear++] = ch;
                preq = preq->nextPrerequisites;
            }
        }
        if(totalCourses!=0) {
            printf("error in the courses file!!!\n");
            return;
        }
        printf("courses sorted successfull\n");
        free(queue);
        flag3 = 1;
    }
    if (flag == 0)
        printf("courses are not loaded\n");
}
//function that will insert the courses in the list
void insertCours(coursesList l, char course[50], char prerequisites[50]) {
    courseNode loc = searchCourse(l, prerequisites);
    if (loc == NULL) {
        courseNode temp = (courseNode) malloc(sizeof(struct nodeCourse));
        if (temp == NULL) {
            printf("out of memory");
            exit(1);
        }
        strcpy(temp->course, prerequisites);
        temp->nextPrerequisites = NULL;
        temp->nextCourse = l->nextCourse;
        l->nextCourse = temp;
        loc = temp;
    }
    courseNode loc1 = searchCourse(l, course);
    if (loc1 == NULL) {
        courseNode temp = (courseNode) malloc(sizeof(struct nodeCourse));
        if (temp == NULL) {
            printf("out of memory");
            exit(1);
        }
        strcpy(temp->course, course);
        temp->nextPrerequisites = NULL;
        temp->nextCourse = l->nextCourse;
        l->nextCourse = temp;
    }
    if (duplicateCourse(loc, course) != 1) {
        courseNode temp = (courseNode) malloc(sizeof(struct nodeCourse));
        if (temp == NULL) {
            printf("out of memory");
            exit(1);
        }
        strcpy(temp->course, course);
        temp->nextPrerequisites = loc->nextPrerequisites;
        loc->nextPrerequisites = temp;
    }
}
//function that will make the course list
coursesList makeCousreList(coursesList l) {
    l = (coursesList) malloc(sizeof(struct nodeCourse));
    if (l == NULL) {
        printf("memory is full");
        exit(1);
    }
    l->nextCourse = NULL;
    l->nextPrerequisites = NULL;
    return l;
}
//function to find the shotest route
void shortestRoute(char src[50], char dest[50]) {
    shortestPath(src, dest);
    if (flag1 != -1) {
        char path[heapCapacity][50];
        int pathSize = 0;
        char current[50];
        strcpy(current, dest);
        while (strcasecmp(current, src) != 0) {
            strcpy(path[pathSize++], current);
            for (int i = 0; i < resultSize; i++) {
                if (strcasecmp(result[i].vertix->building, current) == 0) {
                    strcpy(current, result[i].pathVia);
                    break;
                }
            }
        }
        strcpy(path[pathSize++], src);

        for (int k = pathSize - 1; k >= 0; k--) {
            printf("%s", path[k]);
            if (k != 0)
                printf("->");
        }
        printf("\n");
    }
}
//function to calculate the shortest path
void shortestPath(char src[50], char dest[50]) {
    resultSize = 0;
    if (search(adjacenctList, src) == NULL || search(adjacenctList, dest) == NULL) {
        printf("source or destination not found\n");
        flag1 = -1;
        return;
    }
    enqueue(src);
    while (minHeap->size != 0) {
        node p = minHeap->heap[1].vertix;
        while (p->nextDest != NULL) {
            p = p->nextDest;
            int loc = searchHeap(p->building);
            if ((minHeap->heap[1].cost + p->cost) < minHeap->heap[loc].cost) {
                minHeap->heap[loc].cost = minHeap->heap[1].cost + p->cost;
                strcpy(minHeap->heap[loc].pathVia, minHeap->heap[1].vertix->building);
                readjuctHeap(loc);
            }
        }
        dequeue();
    }
    for (int i = 0; i < resultSize; i++) {
        if (strcasecmp(dest, result[i].vertix->building) == 0) {
            if (result[i].cost <0|| result[i].cost >10000000) {
                printf("building cant be reached\n");
                flag1 = -1;
            } else {
                printf("Shortest distance: %d\n", result[i].cost);
                flag1 = i;
            }
        }
    }
}
//function to deqeue the root node from the heap and rearrange the array
void dequeue() {
    result[resultSize].cost = minHeap->heap[1].cost;
    result[resultSize].vertix = minHeap->heap[1].vertix;
    strcpy(result[resultSize].pathVia, minHeap->heap[1].pathVia);
    resultSize++;
    swap(1, minHeap->size);
    minHeap->size--;
    int loc = 1;
    while (1) {
        int left = 2 * loc;
        int right = (2 * loc) + 1;
        int smallest = loc;
        if (left <= minHeap->size && minHeap->heap[left].cost < minHeap->heap[smallest].cost)
            smallest = left;
        if (right <= minHeap->size && minHeap->heap[right].cost < minHeap->heap[smallest].cost)
            smallest = right;
        if (smallest != loc) {
            swap(loc, smallest);
            loc = smallest;
        } else
            break;;
    }
}
//function to hepify the array each time a node is updates
void readjuctHeap(int loc) {
    int parent = loc / 2;

    if (parent != 0 && minHeap->heap[parent].cost > minHeap->heap[loc].cost) {
        swap(loc, parent);
        readjuctHeap(parent);
    }
}
//function to swap two nodes in the heap array
void swap(int a, int b) {
    int temp = minHeap->heap[a].cost;
    minHeap->heap[a].cost = minHeap->heap[b].cost;
    minHeap->heap[b].cost = temp;
    char temp1[50];
    strcpy(temp1, minHeap->heap[a].pathVia);
    strcpy(minHeap->heap[a].pathVia, minHeap->heap[b].pathVia);
    strcpy(minHeap->heap[b].pathVia, temp1);
    node temp2 = minHeap->heap[a].vertix;
    minHeap->heap[a].vertix = minHeap->heap[b].vertix;
    minHeap->heap[b].vertix = temp2;
}
//function to search in the result array
int searchHeap(char key[50]) {
    for (int i = 1; i <= minHeap->size; i++) {
        if (strcasecmp(minHeap->heap[i].vertix->building, key) == 0)
            return i;
    }
    return -1;
}
//function to insert all the nodes in the array
void enqueue(char src[50]) {
    int i = ++minHeap->size;
    minHeap->heap[i].cost = 0;
    minHeap->heap[i].vertix = search(adjacenctList, src);
    list p = adjacenctList->nextBuilding;
    while (p != NULL) {
        if (strcasecmp(p->building, src) != 0) {
            i = ++minHeap->size;
            minHeap->heap[i].cost = INT_MAX;
            minHeap->heap[i].vertix = p;
        }
        p = p->nextBuilding;
    }
}

//function to make the heap
heap makeHeap() {
    heap minHeap = (heap) malloc(sizeof(struct minHeap));
    if (minHeap == NULL) {
        printf("memory is");
        exit(1);
    }
    minHeap->size = 0;
    minHeap->capacity = heapCapacity;
    minHeap->heap = (struct heapNode *) malloc(sizeof(struct heapNode) * heapCapacity);
    return minHeap;
}
//function to make the building list
list makeEmpty(list l) {
    l = (list) malloc(sizeof(struct node));
    if (l == NULL) {
        printf("memory is full");
        exit(1);
    }
    l->nextDest = NULL;
    l->nextBuilding = NULL;
    return l;
}
//function that will insert in the building list
void insert(list l, char building[50], char destination[50], int cost) {
    node loc = search(l, building);
    if (loc == NULL) {
        node temp = (node) malloc(sizeof(struct node));
        if (temp == NULL) {
            printf("out of memory");
            exit(1);
        }
        strcpy(temp->building, building);
        temp->cost = 0;
        temp->nextDest = NULL;
        temp->nextBuilding = l->nextBuilding;
        l->nextBuilding = temp;
        loc = temp;
    }
    node loc1 = search(l, destination);
    if (loc1 == NULL) {
        node temp = (node) malloc(sizeof(struct node));
        if (temp == NULL) {
            printf("out of memory");
            exit(1);
        }
        strcpy(temp->building, destination);
        temp->cost = 0;
        temp->nextDest = NULL;
        temp->nextBuilding = l->nextBuilding;
        l->nextBuilding = temp;
    }
    if (duplicate(loc, destination) != 1) {
        node temp = (node) malloc(sizeof(struct node));
        if (temp == NULL) {
            printf("out of memory");
            exit(1);
        }
        node p = loc;
        strcpy(temp->building, destination);
        temp->cost = cost;
        temp->nextDest = p->nextDest;
        p->nextDest = temp;
    }
}
//search functions that will search in the course list and the building list(4)
node search(list l, char building[50]) {
    node p = l->nextBuilding;
    if (p == NULL)
        return NULL;
    while (strcasecmp(p->building, building) != 0) {
        p = p->nextBuilding;
        if (p == NULL)
            return NULL;
    }
    return p;
}

courseNode searchCourse(coursesList l, char course[50]) {
    courseNode p = l->nextCourse;
    if (p == NULL)
        return NULL;
    while (strcasecmp(p->course, course) != 0) {
        p = p->nextCourse;
        if (p == NULL)
            return NULL;
    }
    return p;
}

int duplicate(node loc, char destination[50]) {
    node p = loc->nextDest;
    if (p == NULL)
        return 0;
    while (strcasecmp(p->building, destination) != 0) {
        p = p->nextDest;
        if (p == NULL)
            return 0;
    }
    return 1;
}

int duplicateCourse(courseNode loc, char prerequisites[50]) {
    coursesList p = loc->nextPrerequisites;
    if (p == NULL)
        return 0;
    while (strcasecmp(p->course, prerequisites) != 0) {
        p = p->nextPrerequisites;
        if (p == NULL)
            return 0;
    }
    return 1;
}
