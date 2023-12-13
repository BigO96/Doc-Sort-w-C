
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_SIZE 100

typedef struct
{
    int front, back, size;
    char **array;
} Queue;

char *NewString(char *s)
{
    int len = strlen(s);
    char *rv = malloc(len + 1);
    strcpy(rv, s);
    rv[len - 1] = '\0'; /* take out newline */
    return rv;
}

void InitializeQueue(Queue *q)
{
    q->array = malloc(sizeof(char *) * QUEUE_SIZE);
    q->front = 0;
    q->back = 0;
    q->size = 0;

    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        q->array[i] = NULL;
    }
}

void Enqueue(Queue *q, char *str)
{
    if (q->size == QUEUE_SIZE)
        printf("ERROR!\n");
    q->back = (q->back) % QUEUE_SIZE;
    q->array[q->back] = str;
    q->size++;
    q->back++;
}

char *Dequeue(Queue *q)
{
    if (q->size == 0)
        return NULL;

    char *item = q->array[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->size--;
    // q->back--;
    return item;
}
char *Front(Queue *q)
{
    if (q->size == 0)
        return NULL;

    return q->array[q->front];
}

void PrettyPrintQueue(Queue *q, char *type)
{
    if (Front(q) == NULL)
    {
        printf("No unmatched entries for %s\n", type);
    }
    else
    {
        char *s;
        printf("Unmatched %s:\n", type);
        while ((s = Dequeue(q)))
        {
            printf("%s\n", s);
        }
    }
}

void sortq(Queue *q, Queue *female_donors, Queue *female_recipients, Queue *male_donors, Queue *male_recipients, Queue *hospitals)
{
    char *a;

    char *Deq1;
    char *Deq2;
    char *Deq3;

    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        // Check if q is NULL, If NULL, Func to print end of list

        if (Front(female_donors) != NULL && Front(female_recipients) != NULL && Front(hospitals) != NULL)
        {
            printf("MATCH: %s donates to %s at hospital %s\n", Dequeue(female_donors), Dequeue(female_recipients), Dequeue(hospitals));
        }
        if (Front(male_donors) != NULL && Front(male_recipients) != NULL && Front(hospitals) != NULL)
        {
            printf("MATCH: %s donates to %s at hospital %s\n", Dequeue(male_donors), Dequeue(male_recipients), Dequeue(hospitals));
        }

        a = Dequeue(q);
        if (a != NULL)
        {
            if (a[0] == 'H')
            {
                Enqueue(hospitals, a + 2);
            }

            if (a[0] == 'R')
            {
                if (a[2] == 'F')
                {
                    Enqueue(female_recipients, a + 4);
                }
                else
                {
                    Enqueue(male_recipients, a + 4);
                }
            }
            if (a[0] == 'D')
            {
                if (a[2] == 'F')
                {
                    Enqueue(female_donors, a + 4);
                }
                else
                {
                    Enqueue(male_donors, a + 4);
                }
            }
        }
    }
    PrettyPrintQueue(female_donors, "female donors");
    PrettyPrintQueue(female_recipients, "female recipients");
    PrettyPrintQueue(male_donors, "male donors");
    PrettyPrintQueue(male_recipients, "male recipients");
    PrettyPrintQueue(hospitals, "hospitals");
}

int main(int argc, char *argv[])
{
    Queue q;
    InitializeQueue(&q);

    FILE *file = fopen(argv[1], "r");
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file))
    {
        char *line = NewString(buffer);
        Enqueue(&q, line);
    }

    Queue female_donors;
    Queue female_recipients;
    Queue male_donors;
    Queue male_recipients;
    Queue hospitals;
    InitializeQueue(&female_donors);
    InitializeQueue(&female_recipients);
    InitializeQueue(&male_donors);
    InitializeQueue(&male_recipients);
    InitializeQueue(&hospitals);

    sortq(&q, &female_donors, &female_recipients, &male_donors, &male_recipients, &hospitals);

    fclose(file);
}
