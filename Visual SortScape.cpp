#include<SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
using namespace std;

// Defining the startze of the array and the blocks for visualization
const int arrSize = 50;
const int blockSize = 7;

// Screen dimenstartons
const int screen_breadth = 350;
const int screen_length = 550;

// Arrays to store the initial and working state of the data to be sorted
int arr[arrSize];
int blockBar[arrSize];

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
        cout << "SDL Error: " << SDL_GetError();
        success = false;
    }
    else
    {   // Set texture filtering hint
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout << "Enble the Linear Texture.\n";
        }

        // Creating SDL window
        window = SDL_CreateWindow("Visual Sortscape", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_breadth, screen_length, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "SDL Error: " << SDL_GetError();
            success = false;
        }
        else
        {   // Creating SDL renderer
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                cout << "SDL Error: " << SDL_GetError();
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
// a,b,c variables are indices of elements to highlight during visualization

void visualize(int a = -1, int b = -1, int c = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);   // Set the background color of the renderer
    SDL_RenderClear(renderer);          // Clear the renderer

    int j = 0;

    // Loop through the array and create rectangular blocks for each element
    for (int i = 0; i <= screen_breadth - blockSize; i += blockSize)
    {
        SDL_PumpEvents();

        // SDL rectangle block to represent the current element
        SDL_Rect rect = { i, 0, blockSize, arr[j] };

        // Conditions to determine the colour of the blocks
        // RGBA format

        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 34, 136, 248, 0);     //when the event is completed, the final blocks' colour
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (j == a || j == c)
        {
            SDL_SetRenderDrawColor(renderer, 239, 20, 19, 0);       //the elements bendng compared or swapped
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == b)
        {
            SDL_SetRenderDrawColor(renderer, 142, 196, 209, 0);       //the element bendng searched for the minimum value
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
int quick_partition(int a[], int start, int end)
{
    int min_element = 0;    // number of elements <= pivot

    for (int i = (start + 1); i <= end; i++)
    {
        if (a[i] <= a[start])
        {
            min_element++;
        }
    }

    // index of the partitioning element
    int c = start + min_element;

    //swaping the partitioning element with the first element
    int temporary = a[c];
    a[c] = a[start];
    a[start] = temporary;
    visualize(c, start);   // visualization of the swap

    int i = start, j = end;

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
        {   // Swapping elements on both startdes of the partition
            int temporary_1 = a[j];
            a[j] = a[i];
            a[i] = temporary_1;

            visualize(i, j);    //Visualization of swap
            SDL_Delay(100);     // Delay for visualization

            i++;
            j--;
        }
    }

    return c;   // returning the index of the partition element
}

void Quick_Sort(int a[], int start, int end)
{
    // return when array has one or fewer elements
    if (start >= end)
    {
        return;
    }

    int c = quick_partition(a, start, end);     // Partition array: returning the index of the partitioning element

    // recurstartvely sorting the left and right startde of the partioning element
    Quick_Sort(a, start, c - 1);
    Quick_Sort(a, c + 1, end);

}










// MERGE SORT
void merge(int a[], int start, int end)
{
    int startze_output = (end - start) + 1;    //startze of output array
    int* output = new int[startze_output];     //dynamic array to store the merged output

    // mid-point of the array
    int mid = (start + end) / 2;
    int i = start, j = mid + 1, k = 0;

    // merging the two sorted arrays into the output array
    while (i <= mid && j <= end)
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
    while (j <= end)
    {
        output[k] = a[j];
        visualize(-1, j);
        j++;
        k++;
    }

    // the merged output back to the original array
    int x = 0;
    for (int l = start; l <= end; l++)
    {
        a[l] = output[x];
        visualize(l);
        SDL_Delay(70);
        x++;
    }
    delete[]output;     // deallocating the allocated memory
}

void Merge_Sort(int a[], int start, int end)
{   //array has one or fewer elements
    if (start >= end)
    {
        return;
    }

    // mid-point of the array
    int mid = (start + end) / 2;


    // recurstartvely performing the merge sort on the two halves of the array
    Merge_Sort(a, start, mid);
    Merge_Sort(a, mid + 1, end);

    // merging the two halves
    merge(a, start, end);
}










// BUBBLE SORT
void Bubble_Sort()
{   // Iterates over each element in the array
    for (int i = 0; i < arrSize - 1; i++)
    {   //Compare adjacent elements and swap if necessary
        for (int j = 0; j < arrSize - 1 - i; j++)
        {
            if (arr[j + 1] < arr[j])
            {   //Swapping
                int temporary = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temporary;

                visualize(j + 1, j, arrSize - i);   //visualizing the process
            }
            SDL_Delay(60);
        }
    }
}










// INSERTION SORT
void Insertion_Sort()
{
    // Iterates through the array, starting from SECOND element
    for (int i = 1; i < arrSize; i++)
    {
        // storing the current element which is to be inserted
        int temporary = arr[i];

        // Initialize the index for comparison with the previous elements
        int j = i - 1;


        // Move element greater than temporary to the right until the correct postarttion is found
        while (j >= 0 && arr[j] > temporary)
        {
            arr[j + 1] = arr[j];    //Shift elements to the right
            j--;

            visualize(i, j + 1);    //visualizing the shifting process
            SDL_Delay(60);
        }

        // place temporary in ots correct postarttion in the sorted sequence
        arr[j + 1] = temporary;
    }
}










// SELECTION SORT
void Selection_Sort()
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
        int temporary = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temporary;
    }
}











// To load contents of "blockBar" array into the "arr" 
void loadingArray()
{
    memcpy(arr, blockBar, sizeof(int) * arrSize);       // startzeof(int) * arrSize = total number of bytes to copy
}


// Generating Array of Random Numbers
void randomArrayGeneration()
{
    for (int i = 0; i < arrSize; i++)
    {
        int random = rand() % 500;  // random numbers till 500
        blockBar[i] = random;
    }

}

void executingProgram()
{   // Initializing SDL
    if (!init())
    {
        cout << "SDL Initialization Failed.\n";
    }
    else
    {
        randomArrayGeneration();
        loadingArray();

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
                        randomArrayGeneration();
                        complete = false;
                        loadingArray();
                        cout << "\nNEW RANDOM LIST GENERATED.\n";
                        break;
                    case(SDLK_1):
                        loadingArray();
                        cout << "\nSELECTION SORT STARTED.\n";
                        complete = false;
                        Selection_Sort();
                        complete = true;
                        cout << "\nSELECTION SORT COMPLETE.\n";
                        break;
                    case(SDLK_2):
                        loadingArray();
                        cout << "\nINSERTION SORT STARTED.\n";
                        complete = false;
                        Insertion_Sort();
                        complete = true;
                        cout << "\nINSERTION SORT COMPLETE.\n";
                        break;
                    case(SDLK_3):
                        loadingArray();
                        cout << "\nBUBBLE SORT STARTED.\n";
                        complete = false;
                        Bubble_Sort();
                        complete = true;
                        cout << "\nBUBBLE SORT COMPLETE.\n";
                        break;
                    case(SDLK_4):
                        loadingArray();
                        cout << "\nMERGE SORT STARTED.\n";
                        complete = false;
                        Merge_Sort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nMERGE SORT COMPLETE.\n";
                        break;
                    case(SDLK_5):
                        loadingArray();
                        cout << "\nQUICK SORT STARTED.\n";
                        complete = false;
                        Quick_Sort(arr, 0, arrSize - 1);
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
bool menuDriven()
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
void introduction()
{
    cout << "============================================\t:: VISUAL SORTSCAPE ::\t============================================\n\n"
        << "A software tool that visually demonstrates and animates the step-by-step operation of various sorting algorithms, providing an intuitive and educational way to understand thendr functionality." << endl;
    cout << "\nBy:\n"
        << "Bhavana  B. S (E22CSEU0730)"
        << "\nAashna Dogra (E22CSEU0732)" << endl;   //Us

    cout << "\nPress ENTER to see list of Controls ->";

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
    introduction();    // Bastartc introduction of our program.

    while (1)
    {
        cout << '\n';
        if (menuDriven())
            executingProgram();
        else
        {
            cout << "\nEXITING PROGRAM.\n";
            break;
        }
    }

    return 0;
}
