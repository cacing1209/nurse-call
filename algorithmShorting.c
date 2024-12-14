// #include <stdio.h>
// //tinggal build aja boy
// const char menuMSG[] = "pilihlan algoritma di bawah ini: \n "
//                        "1.BUBBLE SHORT \n "
//                        "2.";

// // void shorting(float *data, int size)
// void shortingDown(int *data, int size)
// {
//     printf("\n is buble \n");
//     for (int x = 0; x < size - 1; x++)
//     {
//         printf("\n%d X: ", x);
//         for (int y = 0; y < size - 1 - x; y++)
//         {
//             if (data[y] > data[y + 1])
//             {
//                 printf("%d Y: ", y);
//                 // float current = data[y];
//                 int current = data[y];
//                 data[y] = data[y + 1];
//                 data[y + 1] = current;
//             }
//         }
//     }
// }

// // void shortingUp(float *data, int size)
// void shortingUp(int *data, int size)
// {
//     for (int x = 0; x < size - 1; x++)
//     {
//         for (int y = 0; y < size - x - 1; y++)
//         {
//             if (data[y] < data[y + 1])
//             {
//                 // float current = data[y];
//                 int current = data[y];
//                 data[y] = data[y + 1];
//                 data[y + 1] = current;
//             }
//         }
//     }
// }

// // void showAlgorithm(float *data, int Size)
// void showAlgorithm(int *data, int Size)
// {
//     printf("\nsebelum di urutkan: ");
//     for (int i = 0; i < Size; i++)
//     {
//         // printf("%0.3f ", aray1[i]);
//         printf("%d ", data[i]);
//     }
//     printf("\n");
//     shortingDown(data, Size);
//     printf("\n sesudah di urutkan kecil-besar: ");
//     for (int i = 0; i < Size; i++)
//     {
//         // printf("%0.3f ", aray1[i]);
//         printf("%d ", data[i]);
//     }
//     printf("\n");
//     shortingUp(data, Size);
//     printf("\n sesudah di urutkan besar-kecil: ");
//     for (int i = 0; i < Size; i++)
//     {
//         // printf("%0.3f ", aray1[i]);
//         printf("%d ", data[i]);
//     }
//     printf("\n");
// }

// #define Sizearay 5   // size
// int aray1[Sizearay]; // definisi aray dulu nilainya di function saja
// // float aray1[Sizearay] = {67.000, 12.020, 323.453, 3.223, 1e9};

// void buble_short()
// {
//     int aray1[Sizearay] = {223, 4445, 2, 4323, 45}; // contoh value aray
//     printf(" #define Sizearay 5 //ukuran aray \n");
//     printf(" int aray1[Sizearay] = {223, 4445, 2, 4323, 45}; // contoh value aray \n");
//     printf(" Mengurutkan angka dari yang TERKECIL hingga TERBESAR di dalam aray \n");
//     showAlgorithm(aray1, Sizearay);
// }

// void setup()
// {
//     int input = 0;
//     for (const char *i = menuMSG; *i != '\0'; i++)
//     {
//         printf("%c", *i);
//     }
//     scanf("%d", &input);
//     printf("\n ");
//     switch (input)
//     {
//     case 1:
//         buble_short();
//         break;
//     case 2:
//         break;
//     default:
//         printf("Kesalahan input \n");
//         break;
//     }
// }
// int main()
// {
//     setup();
//     return 0;
// }