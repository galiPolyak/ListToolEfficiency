#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "LLvNode.h"

#include "fasta.h"

void freeFASTARecord(LLvNode *node, void *arg);

int
processFasta(char *filename, double *timeTaken)
{
  FILE *fp;
  int lineNumber = 0, recordNumber = 0, status;
  int eofSeen = 0;
  clock_t startTime, endTime;
  

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failure opening %s : %s\n",
        filename, strerror(errno));
    return -1;
  }

  /** record the time now, before we do the work */
  startTime = clock();

  //Add
	struct LLvNode *head = NULL;
	struct LLvNode *tail = NULL;

  do {
    /** print a '.' every 10,000 records so
    * we know something is happening */
    if ((recordNumber % 10000) == 0) {
      printf(".");
      fflush(stdout);
    }

    //Added
    //printf("Initializes Fasta\n");
    FASTArecord *newRecord = (FASTArecord *)malloc(sizeof(FASTArecord));
    fastaInitializeRecord(newRecord);

    //Added
    status = fastaReadRecord(fp, newRecord);

    //printf("Reads Fasta Record\n");
    if (status == 0) {
      eofSeen = 1;
      free(newRecord);

    } 
    else if (status > 0) 
    {
      lineNumber += status;
      recordNumber++;
      //fastaPrintRecord(stdout, &fRecord);

      //Added
      LLvNode* newNode;
      newNode = llNewNode("", newRecord);

      if (head == NULL) 
      {
          head = newNode;
          tail = newNode;
      } 
      else 
      {
          tail->next = newNode;
          tail = newNode;
      }


    } else {
      fprintf(stderr, "status = %d\n", status);
      fprintf(stderr, "Error: failure at line %d of '%s'\n",
          lineNumber, filename);
      return -1;
    }

  } while ( ! eofSeen);
  printf(" %d FASTA records\n", recordNumber);

  /** record the time now, when the work is done,
   *  and calculate the difference*/
  endTime = clock();

  (*timeTaken) = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;


  fclose(fp);

  /*Added- print nodes
  struct LLvNode *current = head;  // Start from the head of the list
  int count = 0;

  while (current != NULL && count < 3) {
      FASTArecord *fRec;
      fRec = (FASTArecord *)current->value;
      printf("FASTA Record:\n");
      printf("ID   (%ld)\n", fRec->id);
      printf("DESC [%s]\n", fRec->description);
      printf("SEQ  [%s]\n", fRec->sequence);
      current = current->next;
      count++;
  }*/

  llFree(head, freeFASTARecord, NULL);

  return recordNumber;
}


int
processFastaRepeatedly(
    char *filename,
    long repeatsRequested
  )
{

  double timeThisIterationInSeconds;
  double totalTimeInSeconds = 0;
  int minutesPortion;
  int status;
  long i;

  for (i = 0; i < repeatsRequested; i++) {
    status = processFasta(filename, &timeThisIterationInSeconds);
    if (status < 0) return -1;
    totalTimeInSeconds += timeThisIterationInSeconds;
  }

  printf("%.3lf seconds taken for processing total\n", totalTimeInSeconds);

  totalTimeInSeconds /= (double) repeatsRequested;

  minutesPortion = (int) (totalTimeInSeconds / 60);
  totalTimeInSeconds = totalTimeInSeconds - (60 * minutesPortion);
  printf("On average: %d minutes, %.3lf second per run\n",
            minutesPortion, totalTimeInSeconds);

  return status;
}

void usage(char *progname)
{
  fprintf(stderr, "%s [<OPTIONS>] <file> [ <file> ...]\n", progname);
  fprintf(stderr, "\n");
  fprintf(stderr, "Prints timing of loading and storing FASTA records.\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options: \n");
  fprintf(stderr, "-R <REPEATS> : Number of times to repeat load.\n");
  fprintf(stderr, "             : Time reported will be average time.\n");
  fprintf(stderr, "\n");
}

/**
 * Program mainline
 */
int
main(int argc, char **argv)
{
  int i, recordsProcessed = 0;
  long repeatsRequested = 1;

  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'R') {
        if (i >= argc) {
          fprintf(stderr,
              "Error: need argument for repeats requested\n");
          return 1;
        }
        if (sscanf(argv[++i], "%ld", &repeatsRequested) != 1) {
          fprintf(stderr,
              "Error: cannot parse repeats requested from '%s'\n",
              argv[i]);
          return 1;
        }
      } else {
        fprintf(stderr,
            "Error: unknown option '%s'\n", argv[i]);
        usage(argv[0]);
      }
    } else {
      recordsProcessed = processFastaRepeatedly(argv[i],
          repeatsRequested);
      if (recordsProcessed < 0) {
        fprintf(stderr, "Error: Processing '%s' failed -- exitting\n",
            argv[i]);
        return 1;
      }
      printf("%d records processed from '%s'\n",
          recordsProcessed, argv[i]);
    }
  }

  if ( recordsProcessed == 0 ) {
    fprintf(stderr,
        "No data processed -- provide the name of"
        " a file on the command line\n");
    usage(argv[0]);
    return 1;
  }

  return 0;
}

void freeFASTARecord(LLvNode *node, void *arg) 
{
    if (node != NULL) 
    {
        FASTArecord *fRec = (FASTArecord *)node->value;
        if (fRec->description != NULL) 
        {
          free(fRec->description);
        }
        if (fRec->sequence != NULL) {
          free(fRec->sequence);
        }

        free(fRec);
    }
    
}



