Name: Gali Polyak
Student Number: 1229946
State of implementation: Complete

A short summary of what you have learned about the relative amounts of time required to perform these
tasks:
First I learned that storing the FASTA records in the array takes very little time, meaning it is efficient.
This is because Dynamic arrays offer a O(1) time complexity for random access to elements making it very efficient when 
you need to append a record. The linked list with a tail pointer is also very efficient for appending records. 
This is because we can directly add records to the end of the list using the tail pointer. Appending to this data 
structure also has a time complexity of O(1). Last the linked list with only the head pointer is the least efficient 
and takes the most time. This is because each append operation requires traversing the entire list meaning the time 
complexity of this operation is O(n). This results in a time-consuming operation, especially when the list is as 
large as hundreds of thousands of records. Overall, for appending elements, I learned that it is more efficient to 
use a data structure with a tail pointer or a dynamic array.


