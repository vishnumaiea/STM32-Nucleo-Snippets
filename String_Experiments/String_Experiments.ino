
//========================================================================//
//
//  This program demonstrates how you can use the C++'s inbuilt <string>
//  library to pass strings to functions. You can send strings as objects
//  or as pointers. The <string> library seems compatible with FreeRTOS.
//  I had to do this becasue the Arduino's String class did not work well
//  with FreeRTOS on STM32 Nucleo boards. I was unable to create String
//  objects or pass them to functions.
//
//  Last modified : +05:30 12:23:07 PM, 18-11-2018, Sunday
//
//  Web : https://github.com/vishnumaiea/STM32-Nucleo-Snippets
//
//========================================================================//

#include <string>
#include <STM32FreeRTOS.h> //you need to install this library

using namespace std;

//========================================================================//
//create a class

class stringHandlers {
  public:
    void stringHandler_1(string*);
    void stringHandler_2(string*);
    void stringHandler_2(string, int); //overloaded
};

stringHandlers sh; //create a new object

//========================================================================//

void stringHandlers::stringHandler_1 (string *S) {
  Serial.println("String Handler 1");
  Serial.print("String object received = ");
  Serial.println(S->c_str()); //we have a pointer to an object
  Serial.println();
  stringHandler_2("HELLO WORLD", 13);
}

//------------------------------------------------------------------------//

void stringHandlers::stringHandler_2 (string *S) {
  Serial.println("String Handler 2");
  Serial.print("String object received = ");
  Serial.println(S->c_str()); //we have a pointer to an object
  Serial.println();
}

//------------------------------------------------------------------------//
//An overloaded version of the previuos function but using a copy of the
//object instead of a pointer. Both works with FreeRTOS.

void stringHandlers::stringHandler_2 (string S, int i) {
  Serial.println("String Handler 2 OL");
  Serial.print("String object received = ");
  Serial.println(S.c_str()); //we have a copy of an object
  Serial.print("Integer received = ");
  Serial.println(i);
  Serial.println();
}

//========================================================================//

void stringHandler_3 (string *S) {
  Serial.println("String Handler 3");
  Serial.print("String object received = ");
  Serial.println(S->c_str());
  Serial.println();
}

//========================================================================//

void stringTask (void* pvParameters) {
  while(1) { //infinite loop
    Serial.println("Sending a String Objects");
    Serial.println();
    string firstString = "Hello World 1"; //creates a new object and initialize it
    sh.stringHandler_1 (&firstString); //send it to a class member function as pointer
    Serial.println();
    vTaskDelay(1000); //1 sec delay
    firstString = "Hello World 2"; //change the value
    sh.stringHandler_2 (&firstString); //send it again to a class member function as pointer
    Serial.println();
    vTaskDelay(1000);
    firstString = "Hello World 3"; //change the value
    stringHandler_3 (&firstString); //send it to a non-member function as pointer
    Serial.println();
    vTaskDelay(1000);
  }
}

//========================================================================//

void anotherTask (void* pvParameters) {
  while(1); //infinite loop
}

//========================================================================//

void setup() {
  Serial.begin(115200);
  while (!Serial) {
   ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("String Experiments");
  Serial.println();

  //now create RTOS tasks
  xTaskCreate(
                stringTask,      //Function to implement the task
                "stringTask",    //Name of the task
                10000,           //Stack size in words
                NULL,            //Task input parameter
                0,               //Priority of the task
                NULL);           //Task handle.

  xTaskCreate(
                anotherTask,      //Function to implement the task
                "anotherTask",    //Name of the task
                10000,            //Stack size in words
                NULL,             //Task input parameter
                0,                //Priority of the task
                NULL);            //Task handle

  vTaskStartScheduler(); //to start the scheduler. otherwise FreeRTOS won't work
}

//========================================================================//

void loop() {
  //nothing happens here
}

//========================================================================//
