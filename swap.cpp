
// #define ARR_SIZE 3
// typedef struct SwitchState_t {
//   int pin;
//   bool pressed;
//   unsigned long press_time;
//   unsigned long press_duration;
// } SwitchState;

// const int sw_pin[ARR_SIZE] = {11,12,13};
// SwitchState sw[ARR_SIZE];
// String antrian;

// void initSwitchState() {
//   for(int i=0;i<ARR_SIZE;i++) {
//     sw[i].pin = sw_pin[i];
//     sw[i].pressed = false;
//     sw[i].press_time = 0;
//     sw[i].press_duration = 0;
//     pinMode(sw[i].pin, INPUT);
//   }
// }

// void checkPinState() {
//   for(int i=0;i<ARR_SIZE;i++) {
//     int val = digitalRead(sw[i].pin);
//     if(val == HIGH && !sw[i].pressed) {
//       sw[i].pressed = true;
//       sw[i].press_time = millis();
//       continue;
//     }
//     if(val == LOW && sw[i].pressed) {
//       sw[i].pressed = false;
//       unsigned long endtime = millis();
//       sw[i].press_duration = endtime - sw[i].press_time;
//       continue;
//     }
//   }
// }

// void swap(SwitchState* xp, SwitchState* yp)
// {
//     SwitchState temp = *xp;
//     *xp = *yp;
//     *yp = temp;
// }

// void bubbleSort(SwitchState arr[], int n)
// {
//     int i, j;
//     bool swapped;
//     for (i = 0; i < n - 1; i++) {
//         swapped = false;
//         for (j = 0; j < n - i - 1; j++) {
//             if (arr[j].press_time > arr[j + 1].press_time) {
//                 swap(&arr[j], &arr[j + 1]);
//                 swapped = true;
//             }
//         }
//         if (swapped == false)
//             break;
//     }
// }
// void setup() {
//   initSwitchState();
//   Serial.begin(9600);
// }

// void loop() {
//   checkPinState();
//   bubbleSort(sw, ARR_SIZE);
//   for(int i=0;i<ARR_SIZE;i++) {
//     if(sw[i].pressed) {
//       Serial.print(sw[i].pin);
//       Serial.print(" ");
//     }
//   }
//   Serial.println();
// }