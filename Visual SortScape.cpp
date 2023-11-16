#include<SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
using namespace std;

// Defining the size of the array and the blocks for visualization
const int arrSize = 50;
const int rectSize = 7;
int s = arrSize * rectSize; // breadth for the screen in which visualization will be displayed

// Screen dimensions
const int SCREEN_WIDTH = s;
const int SCREEN_HEIGHT = 750;

// Arrays to store the initial and working state of the data to be sorted
int arr[arrSize];
int Barr[arrSize];

// SDL window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// a flag to indicat whether the sorting is complete or not
bool complete = false;

// Function for the initialization of SDL 
bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
        success = false;
    }
    else
    {   // Set texture filtering hint
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout << "Warning: Linear Texture Filtering not enabled.\n";
        }

        // Creating SDL window
        window = SDL_CreateWindow("Visual Sortscape", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
            success = false;
        }
        else
        {   // Creating SDL renderer
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
                success = false;
            }
        }
    }

    return success;
}

// To close SDL window and the renderer
void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}


// Visualization for the Sorting Process
// x,y,z variables are indices of elements to highlight during visualization

void visualize(int x = -1, int y = -1, int z = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);   // Set the background color of the renderer
    SDL_RenderClear(renderer);          // Clear the renderer

    int j = 0;

    // Loop through the array and create rectangular blocks for each element
    for (int i = 0; i <= SCREEN_WIDTH - rectSize; i += rectSize)
    {
        SDL_PumpEvents();

        // SDL rectangle block to represent the current element
        SDL_Rect rect = { i, 0, rectSize, arr[j] };

        // Conditions to determine the colour of the blocks
        // RGBA format

        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 34, 136, 248, 0);     //when the event is completed, the final blocks' colour
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (j == x || j == z)
        {
            SDL_SetRenderDrawColor(renderer, 239, 20, 19, 0);       //the elements being compared or swapped
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == y)
        {
            SDL_SetRenderDrawColor(renderer, 142, 196, 209, 0);       //the element being searched for the minimum value
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {   // Default color of the blocks
            SDL_SetRenderDrawColor(renderer, 34, 181, 109, 0);      //when the blocks are not arranged
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer); // Update the renderer to display the changes
}








// SORTING ALGORTHMS FOR QUICK, MERGE, BUBBLE, INSERTION & SELECTION



//QUICK SORT
int partition_array(int a[], int si, int ei)
{
    int count_small = 0;    // number of elements <= pivot

    for (int i = (si + 1); i <= ei; i++)
    {
        if (a[i] <= a[si])
        {
            count_small++;
        }
    }

    // index of the partitioning element
    int c = si + count_small;

    //swaping the partitioning element with the first element
    int temp = a[c];
    a[c] = a[si];
    a[si] = temp;
    visualize(c, si);   // visualization of the swap

    int i = si, j = ei;

    // Partitioning process
    while (i<c && j>c)
    {
        if (a[i] <= a[c])
        {
            i++;
        }
        else if (a[j] > a[c])
        {
            j--;
        }
        else
        {   // Swapping elements on both sides of the partition
            int temp_1 = a[j];
            a[j] = a[i];
            a[i] = temp_1;

            visualize(i, j);    //Visualization of swap
            SDL_Delay(100);     // Delay for visualization

            i++;
            j--;
        }
    }

    return c;   // returning the index of the partition element
}

void quickSort(int a[], int si, int ei)
{
    // return when array has one or fewer elements
    if (si >= ei)
    {
        return;
    }

    int c = partition_array(a, si, ei);     // Partition array: returning the index of the partitioning element

    // recursively sorting the left and right side of the partioning element
    quickSort(a, si, c - 1);
    quickSort(a, c + 1, ei);

}










// MERGE SORT
void merge2SortedArrays(int a[], int si, int ei)
{
    int size_output = (ei - si) + 1;    //size of output array
    int* output = new int[size_output];     //dynamic array to store the merged output

    // mid-point of the array
    int mid = (si + ei) / 2;
    int i = si, j = mid + 1, k = 0;

    // merging the two sorted arrays into the output array
    while (i <= mid && j <= ei)
    {
        if (a[i] <= a[j])
        {
            output[k] = a[i];   // visualizing the process
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            output[k] = a[j];
            visualize(i, j);    // visualizing the process
            j++;
            k++;
        }

    }

    //the remaining elements from first-half
    while (i <= mid)
    {
        output[k] = a[i];
        visualize(-1, i);
        i++;
        k++;
    }

    //the remaining elements from second-half
    while (j <= ei)
    {
        output[k] = a[j];
        visualize(-1, j);
        j++;
        k++;
    }

    // the merged output back to the original array
    int x = 0;
    for (int l = si; l <= ei; l++)
    {
        a[l] = output[x];
        visualize(l);
        SDL_Delay(70);
        x++;
    }
    delete[]output;     // deallocating the allocated memory
}

void mergeSort(int a[], int si, int ei)
{   //array has one or fewer elements
    if (si >= ei)
    {
        return;
    }

    // mid-point of the array
    int mid = (si + ei) / 2;


    // recursively performing the merge sort on the two halves of the array
    mergeSort(a, si, mid);
    mergeSort(a, mid + 1, ei);

    // merging the two halves
    merge2SortedArrays(a, si, ei);
}










// BUBBLE SORT
void bubbleSort()
{   // Iterates over each element in the array
    for (int i = 0; i < arrSize - 1; i++)
    {   //Compare adjacent elements and swap if necessary
        for (int j = 0; j < arrSize - 1 - i; j++)
        {
            if (arr[j + 1] < arr[j])
            {   //Swapping
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                visualize(j + 1, j, arrSize - i);   //visualizing the process
            }
            SDL_Delay(60);
        }
    }
}










// INSERTION SORT
void insertionSort()
{
    // Iterates through the array, starting from SECOND element
    for (int i = 1; i < arrSize; i++)
    {
        // storing the current element which is to be inserted
        int temp = arr[i];

        // Initialize the index for comparison with the previous elements
        int j = i - 1;


        // Move element greater than temp to the right until the correct position is found
        while (j >= 0 && arr[j] > temp)
        {
            arr[j + 1] = arr[j];    //Shift elements to the right
            j--;

            visualize(i, j + 1);    //visualizing the shifting process
            SDL_Delay(60);
        }

        // place temp in ots correct position in the sorted sequence
        arr[j + 1] = temp;
    }
}










// SELECTION SORT
void selectionSort()
{
    int minIndex;

    // Iterates throughout the array
    for (int i = 0; i < arrSize - 1; i++)
    {
        minIndex = i;       // Assuming current element to be the smallest

        // Loop to search the minimum element
        for (int j = i + 1; j < arrSize; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;   // Updating, when found
                visualize(i, minIndex);     // Visualzing the process
            }
            SDL_Delay(50);
        }

        // Swapping of element with the first one
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}











// To load contents of "Barr" array into the "arr" 
void loadArr()
{
    memcpy(arr, Barr, sizeof(int) * arrSize);       // sizeof(int) * arrSize = total number of bytes to copy
}


// Generating Array of Random Numbers
void randomizeAndSaveArray()
{
    for (int i = 0; i < arrSize; i++)
    {
        int random = rand() % 500;  // random numbers till 500
        Barr[i] = random;
    }

}

void execute()
{   // Initializing SDL
    if (!init())
    {
        cout << "SDL Initialization Failed.\n";
    }
    else
    {
        randomizeAndSaveArray();
        loadArr();

        // SDL Event Handler
        SDL_Event e;
        bool quit = false;

        // Main SDL events
        while (!quit)
        {
            // If User closes the window, quit the program
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    complete = false;
                }
                // Handle key presses
                else if (e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                    case(SDLK_q):
                        quit = true;
                        complete = false;
                        cout << "\nEXITING VISUAL SORTSCAPE.\n";
                        break;
                    case(SDLK_0):
                        randomizeAndSaveArray();
                        complete = false;
                        loadArr();
                        cout << "\nNEW RANDOM LIST GENERATED.\n";
                        break;
                    case(SDLK_1):
                        loadArr();
                        cout << "\nSELECTION SORT STARTED.\n";
                        complete = false;
                        selectionSort();
                        complete = true;
                        cout << "\nSELECTION SORT COMPLETE.\n";
                        break;
                    case(SDLK_2):
                        loadArr();
                        cout << "\nINSERTION SORT STARTED.\n";
                        complete = false;
                        insertionSort();
                        complete = true;
                        cout << "\nINSERTION SORT COMPLETE.\n";
                        break;
                    case(SDLK_3):
                        loadArr();
                        cout << "\nBUBBLE SORT STARTED.\n";
                        complete = false;
                        bubbleSort();
                        complete = true;
                        cout << "\nBUBBLE SORT COMPLETE.\n";
                        break;
                    case(SDLK_4):
                        loadArr();
                        cout << "\nMERGE SORT STARTED.\n";
                        complete = false;
                        mergeSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nMERGE SORT COMPLETE.\n";
                        break;
                    case(SDLK_5):
                        loadArr();
                        cout << "\nQUICK SORT STARTED.\n";
                        complete = false;
                        quickSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nQUICK SORT COMPLETE.\n";
                        break;
                    default:
                        cout << "Invalid Choice. Choose from Menu." << endl;
                    }
                }
            }
            visualize();
        }
        close();
    }
}








// A Menu Driven Program of our Visual Sortscape 
bool controls()
{
    cout << "Note: Please give new command once the current command is executed, in order to avoid any latency .\n\n"
        << "Menu Driven:-\n"
        << "    0. Generate a different randomized list.\n"
        << "    1. Selection Sort Visualization.\n"
        << "    2. Insertion Sort Visualization.\n"
        << "    3. Bubble Sort Visualization.\n"
        << "    4. Merge Sort Visualization.\n"
        << "    5. Quick Sort Visualization.\n\n"
        << "    Enter 'q' for exiting the program.\n\n"
        << "PRESS ENTER TO START VISUAL SORTSCAPE...\n\n"
        << "\t\tOr\n\n" << "type - 1 and press ENTER to quit the program.";

    string s;
    getline(cin, s);
    if (s == "-1")
    {
        return false;
    }
    return true;
}


// Introduction
void intro()
{
    cout << "============================================\t:: VISUAL SORTSCAPE ::\t============================================\n\n"
        << "A software tool that visually demonstrates and animates the step-by-step operation of various sorting algorithms, providing an intuitive and educational way to understand their functionality." << endl;
    cout << "\nBy:\n"
        << "Bhavana  B. S (E22CSEU0730)"
        << "\nAashna Dogra (E22CSEU0732)" << endl;   //Us

    cout << "\nPress ENTER to show controls ->";

    string s;
    getline(cin, s);
    if (s == "\n")
    {
        return;     //returning to the main program
    }
}


// MAIN PROGRAM
int main(int argc, char* args[])
{
    intro();    // Basic introduction of our program.

    while (1)
    {
        cout << '\n';
        if (controls())
            execute();
        else
        {
            cout << "\nEXITING PROGRAM.\n";
            break;
        }
    }

    return 0;
}