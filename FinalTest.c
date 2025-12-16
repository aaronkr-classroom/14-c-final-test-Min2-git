#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "students.dat"

// slide 62 형식 유지
typedef struct Student {
    char name[20];
    int kor, eng, math;
    int total;
    float avg;
    int rank;
    struct Student* p_next;
} Student;

// 학생 노드 추가 (slide 62 AddNumber 스타일)
void AddStudent(Student** pp_head, Student** pp_tail, Student data) {
    if (*pp_head != NULL) {
        (*pp_tail)->p_next = (Student*)malloc(sizeof(Student));
        *pp_tail = (*pp_tail)->p_next;
    }
    else {
        *pp_head = (Student*)malloc(sizeof(Student));
        *pp_tail = *pp_head;
    }

    **pp_tail = data;          // 구조체 전체 복사
    (*pp_tail)->p_next = NULL;
}

// 총점, 평균 계산
void CalcScore(Student* p_head) {
    while (p_head) {
        p_head->total = p_head->kor + p_head->eng + p_head->math;
        p_head->avg = p_head->total / 3.0f;
        p_head = p_head->p_next;
    }
}

// 등수 계산
void CalcRank(Student* p_head) {
    Student* p;
    Student* q;

    for (p = p_head; p != NULL; p = p->p_next) {
        p->rank = 1;
        for (q = p_head; q != NULL; q = q->p_next) {
            if (q->total > p->total)
                p->rank++;
        }
    }
}

// 성적 출력
void PrintStudents(Student* p_head) {
    printf("------------------------------------------------\n");
    printf("이름\t국어\t영어\t수학\t총점\t평균\t등수\n");
    printf("------------------------------------------------\n");

    while (p_head) {
        printf("%s\t%d\t%d\t%d\t%d\t%.0f\t%d등\n",
            p_head->name,
            p_head->kor,
            p_head->eng,
            p_head->math,
            p_head->total,
            p_head->avg,
            p_head->rank);

        p_head = p_head->p_next;
    }
}

// .dat 파일 저장
void SaveFile(Student* p_head) {
    FILE* fp = fopen(FILE_NAME, "wb");
    if (!fp) {
        printf("파일 저장 실패\n");
        return;
    }

    while (p_head) {
        fwrite(p_head, sizeof(Student) - sizeof(Student*), 1, fp);
        p_head = p_head->p_next;
    }

    fclose(fp);
    printf("파일 저장 완료\n");
}

// .dat 파일 읽기
void LoadFile(Student** pp_head, Student** pp_tail) {
    FILE* fp = fopen(FILE_NAME, "rb");
    Student temp;

    if (!fp) {
        printf("파일이 존재하지 않습니다.\n");
        return;
    }

    while (fread(&temp, sizeof(Student) - sizeof(Student*), 1, fp)) {
        temp.p_next = NULL;
        AddStudent(pp_head, pp_tail, temp);
    }

    fclose(fp);
    printf("파일 읽기 완료\n");
}

int main(void) {
    Student* p_head = NULL;
    Student* p_tail = NULL;
    Student* p;
    Student temp;
    int menu;
    int count = 1;

    while (1) {
        printf("\n[Menu]\n");
        printf("1. .dat 파일에서 데이터 읽기\n");
        printf("2. 추가 학생 정보 입력\n");
        printf("3. .dat 파일 저장\n");
        printf("4. 성적 확인 (평균 계산 등)\n");
        printf("5. 종료\n");
        printf("-------------------\n");
        printf("선택(1~5): ");
        scanf("%d", &menu);

        if (menu == 1) {
            LoadFile(&p_head, &p_tail);
        }
        else if (menu == 2) {
            printf("%d번째 학생 이름: ", count);
            scanf("%s", temp.name);
            printf("국어 점수: ");
            scanf("%d", &temp.kor);
            printf("영어 점수: ");
            scanf("%d", &temp.eng);
            printf("수학 점수: ");
            scanf("%d", &temp.math);

            temp.total = 0;
            temp.avg = 0.0f;
            temp.rank = 0;
            temp.p_next = NULL;

            AddStudent(&p_head, &p_tail, temp);
            count++;
        }
        else if (menu == 3) {
            SaveFile(p_head);
        }
        else if (menu == 4) {
            if (p_head == NULL) {
                printf("입력된 학생이 없습니다.\n");
            }
            else {
                CalcScore(p_head);
                CalcRank(p_head);
                PrintStudents(p_head);
            }
        }
        else if (menu == 5) {
            printf("프로그램을 종료합니다.\n");
            break;
        }
        else {
            printf("잘못된 선택입니다.\n");
        }
    }

    // 메모리 해제 (slide 62 방식)
    while (p_head != NULL) {
        p = p_head;
        p_head = p_head->p_next;
        free(p);
    }
    p_tail = p_head;

    return 0;
}
