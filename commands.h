// VVV PTZ VVV


const String commandNames[] = {"null", "joyUpdate", "writePos", "callPos", "homeNow", "callCoords", "null", "null", "null", "null", "tallyState"};

// Button formatting = none:0, 1:1, 2:2, 3:3, 4:4, 5:5, 6:6, 7:7, 8:8, 9:9, 0:10, *:11, #:12

const byte joyUpdate = 1; // formatting: identifier, joyX, joyY, joyZ, Speed
// ID identifies the call type, in this case a joy update.
// joyX/Y/Z are 0-255, with 127 being centered.
// speed is speed in modes 1-7


const byte writePos = 2; // formatting: identifier, button
const byte callPos = 3; // formatting: identifier, button, speed
// button formatting = none:0, 1:1, 2:2, 3:3, 4:4, 5:5, 6:6, 7:7, 8:8, 9:9, 0:10

const byte homeNow = 4; // formatting: identifier

const byte errorNow = 5; // formatting: identifier, errorcode





//TODO VVV NOT YET IMPLEMENTED 


const byte callCoords = 6; //formatting: identifier, x1, x2, y1, y2. X and Y variables are to large to store in one byte so they are split up in to two, allowing numbers up to 16 bit.


//! VVV Tally VVV

const byte tallyPreview = 10; // formatting: identifier, camera number, camera number, camera number ....

const byte tallyProgram = 11; // formatting: identifier, camera number, camera number, camera number ....

//TODO VVV NOT YET IMPLEMENTED 

