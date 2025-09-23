#include <stdio.h>

//write a program to check if a number is divisible by 2
/*int main(){
    int x;
    printf("Enter a number:");
    scanf("%d",&x);
    printf("%d",x%2==0);
    return 0; 
}
    
//Write a program to check if a number is odd or even
int main(){
    //even->1
    //odd->0
    int x;
    printf("Enter a number:");
    scanf("%d",&x);
    printf("%d", x%2==0);
    return 0;

}
    */

//Write a program to print the average of 3 numbers
/*
int main(){
    int first_number;
    int second_number;
    int third_number;
    printf("Enter first number:");
    scanf("%d",&first_number);
    printf("Enter second number:");
    scanf("%d",&second_number);
    printf("Enter third number");
    scanf("%d",&third_number);
    printf("The average of three numbers is:%d",(first_number+second_number+third_number)/3);
}
*/
//Write a program to check if given character is digit or not
 /*   
int main(){
    char ch;
    printf("Enter a character:");
    scanf("%c",&ch);
    if (ch>='0' && ch<='9')
    {printf("The character '%c' is a Digit",ch);

        
    }
    else{printf("The charater'%c' is not a Digit",ch);}
    

}
*/
/*
int main()
{
    printf("GPA");
    printf("\n 10");
    printf("  ______________________________________________");
    printf("\n    |");
    printf("\n    |");
    printf("\n    |.");
    printf("\n G 9| .");
    printf("\n P  |   .");
    printf("\n A  |    .                          .");
    printf("\n    |      . . .. .               .");
    printf("\n   8|                  .      .");
    printf("\n    |                     .");
    printf("\n    |");
    printf("\n    |");
    printf("\n   7|______________________________________________");
    printf("\n     1      2      3      4      5      6 \n");
    printf("                 S E M E S T E R");


    



}

/*
#include<stdio.h>

int main() {
    // Declare a character array to store the student's name
    char studentName[30];

    // Total number of classes for each subject (assuming same for both)
    int totalClasses;

    // Attendance for Subject 1 from Monday to Friday
    int s1_mon, s1_tue, s1_wed, s1_thu, s1_fri;

    // Attendance for Subject 2 from Monday to Friday
    int s2_mon, s2_tue, s2_wed, s2_thu, s2_fri;

    // Prompt user to enter student name
    printf("Enter student name: ");
    scanf("%s", studentName);

    // Ask for total number of classes held per subject
    printf("Enter total classes per subject: ");
    scanf("%d", &totalClasses);

    // --- Subject 1 Attendance Inputs ---
    printf("Enter attendance for Subject 1:\n");

    printf("Monday: ");
    scanf("%d", &s1_mon);

    printf("Tuesday: ");
    scanf("%d", &s1_tue);

    printf("Wednesday: ");
    scanf("%d", &s1_wed);

    printf("Thursday: ");
    scanf("%d", &s1_thu);

    printf("Friday: ");
    scanf("%d", &s1_fri);

    // --- Subject 2 Attendance Inputs ---
    printf("Enter attendance for Subject 2:\n");

    printf("Monday: ");
    scanf("%d", &s2_mon);

    printf("Tuesday: ");
    scanf("%d", &s2_tue);

    printf("Wednesday: ");
    scanf("%d", &s2_wed);

    printf("Thursday: ");
    scanf("%d", &s2_thu);

    printf("Friday: ");
    scanf("%d", &s2_fri);

    // Calculate total attendance per subject
    int total_s1 = s1_mon + s1_tue + s1_wed + s1_thu + s1_fri;
    int total_s2 = s2_mon + s2_tue + s2_wed + s2_thu + s2_fri;

    // Calculate attendance percentage
    float percent_s1 = ((float)total_s1 / totalClasses) * 100;
    float percent_s2 = ((float)total_s2 / totalClasses) * 100;

    // Calculate day-wise total attendance
    int monday_total = s1_mon + s2_mon;
    int tuesday_total = s1_tue + s2_tue;
    int wednesday_total = s1_wed + s2_wed;
    int thursday_total = s1_thu + s2_thu;
    int friday_total = s1_fri + s2_fri;

    // --- Output Section ---
    printf("\n--- Attendance Report for %s ---\n", studentName);
    printf("Subject 1 Attendance: %d / %d (%.2f%%)\n", total_s1, totalClasses, percent_s1);
    printf("Subject 2 Attendance: %d / %d (%.2f%%)\n", total_s2, totalClasses, percent_s2);

    printf("\nDay-wise Total Attendance:\n");
    printf("Monday: %d\n", monday_total);
    printf("Tuesday: %d\n", tuesday_total);
    printf("Wednesday: %d\n", wednesday_total);
    printf("Thursday: %d\n", thursday_total);
    printf("Friday: %d\n", friday_total);

    return 0;
}
*/
/*
//h.w question 2:01:03
int main(){
    //q uewrite a program to print the average of 3 numbers
    //taking input and defining variables
    float num1,num2,num3,average;
    printf("Enter the first number: ");
    scanf("%f",&num1);
    printf("Enter the second number:");
    scanf("%f",&num2);
    printf("Enter the third number:");
    scanf("%f",&num3);
    //calculations
    average = (num1+num2+num3)/3;
    //output
    printf("The average of the three number is: %.1f \n",average);

    


    return 0;
}
    */
   /*
//qu2 write a program to check if a given character is digit or not
#include <ctype.h>
int main(){
    char ch;

    //taking input
    printf("Enter a character: ");
    scanf("%c",&ch);
    //check if character is using digit or not
    if (isdigit(ch))
    {printf("'%c'is a digit.\n",ch);
        
    }
    else{
        printf("'%c'is not a Digit.\n",ch);
    }
    

    return 0;

}
*/
/*
//qu3 write a program to print the smallest number
int main(){
    int num1,num2,num3 ,smallest;
    //taking in
    printf("Enter the first number:");
    scanf("%d",&num1);
    printf("Enter the second number:");
    scanf("%d",&num2);
    printf("Enter the third number:");
    scanf("%d",&num3);
    //Assume the first number is smallest
    smallest=num1;
    // compare with second number
    if (num2<smallest)
    {
        smallest=num2;
    }
    if (num3<smallest)
    {
        num3=smallest;
    }
    //output result
    printf("The smallest number is :%d\n",smallest);

    return 0;
    
}
    */
   
//qu9 write a program to check if a student passed or failed marks>30 is pass and marks<=30 is fail
/*
int main(){
    
    //making variables
    float marks;
    //input from user
    printf("Enter your marks:");
    scanf("%f",&marks);
    if (marks>30 && marks<=100)
    {
        printf("Pass,Congratulations!!");
        
    }
    else if (marks<=30)
    {
        printf("Fail");
    }
    else{
        printf("Wrong marks entered");
    }
    
    return 0;
}
    
   int main(){
    int marks=40;
    marks <=30 ? printf("Fail \n"):printf("Pass \n");
return 0;
   }
*/
/*que10 write a program to give grades to a student
marks<30 is c
30<=marks<70 is B
70<=marks<90 is A
90<= marks<=100 is A+
*/
/*
int main(){
    int marks;
    printf("Enter marks (0-100):");
    scanf("%d",&marks);

    if (marks<30)
        {
            printf("C\n");
    }
    else if (marks>=30 &&marks<70)
    {
      printf("B\n")  ;
    }
    else if (marks>=70 && marks<90)
    {
        printf("A\n");
    }
    else{
        printf("A+\n");
    }
    
    
    return 0;

}
    */
//aue 12 write a program to find if a character entered by user is upper case or not
/*
int main(){
    char ch;
    printf("enter character : ");
    scanf("%c",&ch);

    if (ch >= 'A' && ch<= 'Z'){
        printf("upper case\n");
    }
    else if (ch >='a' && ch<='z'){
        printf("lower case\n");

    }
    else{
        printf("not english letter \n");
    }
    return 0;
}
    */
   /*
   
   EMI=P*r*(1+r)^n
        (1+r)^n-1 
        
        EMI
        r->R/100
        */
       /*
int main(){
    for(int i=10;i>=0;i=--i){
        printf("%d \n",i);
        
    }
    for (char ch='a';ch<='z';++ch){
        printf("%c\n ",ch);
    }
    for(float i=1.0;i<=5.0;i++){
        printf("%.2f \n",i);
        
    }
    
    return 0;
}
    */
   /*
int main(){
    int i=1;
    while(i<=5){
        printf("Hello world \n");
        i++;

    }
    return 0;
}
    */
/*
    int n;
    printf("Enter number: ");
    scanf("%d",&n);
    */
    /*
    int i=0;
    while(i<=n){
        printf("%d\n",i);
        i++;
    }
        */
       /*
    for (int i=0;i<=n;i++){
        printf("%d\n",i);
    }
        */
       /*
    int i =5;
    do
    {
      printf("%d\n",i);
      i--;
    } while (i>=1);
    */
   /*
   int n;
   printf("Enter number: ") ;
   scanf("%d",&n);
   int sum=0;
   for(int i=1,j=n;i<=n && j>=1;i++,j--){
    sum=sum+i;
    printf("%d\n",j);
   }
   printf("sum is %d",sum);
  */
 //print the table of a number input by the user
 /*
 int n;
 printf("Enter number: ");
 scanf("%d",&n);
 for(int i=1;i<=10;i++){
    printf("%d\n",n*i);
 }
*/
/*
for(int i=1;i<=5;i++){
    if(i==3){
        break;
    }
    printf("%d\n",i);
}
printf("End");

    
*/
//q17 keep taking numbers as input from user until user enters an odd number
/*
int n;
do
{
   printf("Enter number: ");
   scanf("%d",&n);
   printf("%d\n",n);

   if(n%2!=0){break;}
} while (1);
printf("Thankyou");

*/
//keep taking numbers as input from user until user enters a number which is multiple of 7
/*
int n;
do
{
    printf("Enter number: ");
    scanf("%d",&n);
    printf("%d\n",n);
    if (n%7==0)
    {
       //multiple of 7
       break;
    }
    
} while (1);
printf("thankyou");
*/
//q19 print all numbers from 1 to 10 except for 6
/*
for(int i=1;i<=10;i++){
    if(i==6){//skip
        continue;}
        printf("%d\n",i);
}
        */
    //To check if a user given number is positive or not
/*
    float num;
    printf("Enter Number: ");
    scanf("%f",&num);
    if (num==0)
    {
        printf("The given num is 0");
    }
    else if (num<=0)
    {
        printf("The number given is negative");
    }
    
    else{
        printf("The number given is positive");
    }
*/
//declaring /prototype
/*
void printhello();
  printhello();//functioncall

    
return 0;

}
//function definition
void printhello(){
    printf("Hello");
}
    */
   /*

int main(){
    

// Declaring variables
float num1;  
float num2;
int choice;
float r;
// Taking input from user
printf("Welcome to Calculator, Enter your choice:\n ");
printf("1.ADD\n");
printf("2.SUBTRACT\n");
printf("3.MUTIPLY\n");
printf("4.DIVIDE\n");
printf("Enter First Number:\n");
scanf("%f",&num1);
printf("Enter Second Number:\n");
scanf("%f",&num2);
printf("Enter Your Choice(1-4):\n");
scanf("%d",&choice);

switch (choice)
{
case 1:
    printf("The sum is: %.2f",num1+num2);
    
    break;
case 2:
    printf("The Subtraction is: %.2f",num1-num2);
    
    break;
case 3:
    printf("The Multiplication is: %.2f",num1*num2);
    
    break;
case 4:
    
    if (num2==0)
    {
        printf("Invalid input,Divisor cannot be 0");
    }
    else
    {
        printf("The Divison is: %.2f",num1/num2);
    }
    
    
    break;

default:
    printf("Number should be in the range of 1-4");
    break;
}

    
}
*/
/*
int main(){
int Ans1;
int Ans2;
int i=0;

printf("            | Quizz|\n");
printf(" Round1\n");
printf("Q1What is your name?\n");
printf("1.Yash\n");
printf("2.Aniket\n");
printf("3.Shantanu\n");
printf("4.Atharva\n");
scanf("%d",&Ans1);
switch (Ans1)
{
case 1:
    printf("Right Answer",i++);
    break;
case 2:
    printf("Wrong Answer");
    break;
case 3:
    printf("Wrong Answer");
    break;
case 4:
    printf(" Wrong Answer");
    break;

default:
      printf("Wrong input");
    break;
}
printf("Q1What is your name?\n");
printf("1.Yash\n");
printf("2.Aniket\n");
printf("3.Shantanu\n");
printf("4.Atharva\n");
scanf("%d",&Ans2);
switch (Ans2)
{
    case 1:
    printf("Right Answer",i++);
    break;
case 2:
    printf("Wrong Answer");
    break;
case 3:
    printf("Wrong Answer");
    break;
case 4:
    printf(" Wrong Answer");
    break;

default:
      printf("Wrong input");
    break;
}
printf("Score is :%d ",i);


}
*/
/*
((a%b)==0)?printf("Even"):printf("ODD"); Ternary operator
int main(){
    int a;
    printf("Enter 1");
    scanf("%d",&a);
    ((a==1)?printf("Yes"):printf("No"));
}
    */









