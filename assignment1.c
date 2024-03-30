#include <stdio.h>
#include <stdlib.h>
#include <float.h>
//Group members:
//2088249, Haichuan Yin
//2052313, Zhongxing Sun
//2054765, Tianyang Lyu

// read file，get the x dimension, y dimension, running time, number of particles
// I assume the format of the input file is correct, so I did not judge the wrong format in my program
int **read_input(const char *filename, int *x_dim, int *y_dim, int *time, int *particle_count)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening input file.\n");
        return NULL;
    }

    fscanf(file, "%d", x_dim);
    fscanf(file, "%d", y_dim);
    fscanf(file, "%d", time);

    // Assuming maximum 100 particles in our case
    int **particles = (int **)malloc(sizeof(int *) * 100);
    for (int i = 0; i < 100; i++)
    {
        // Each particle has 4 properties: x, y, vx, vy
        particles[i] = (int *)malloc(sizeof(int) * 4);
    }
    *particle_count = 0;

    while (fscanf(file, "%d,%d,%d,%d", &particles[*particle_count][0], &particles[*particle_count][1], &particles[*particle_count][2], &particles[*particle_count][3]) == 4)
    {
        (*particle_count)++;
    }

    fclose(file);
    return particles;
}

//print the 2D number array
void printArrayNum(int rows, int cols, int arr[rows][cols])
{
    for (int i = cols - 1; i >= 0; i--)
    {
        for (int j = 0; j < rows; j++)
        {
            printf("%d ", arr[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

// write the error.txt when input conditions are worng
void write_error(const char *filename, int **particles)
{
    FILE *inputFile = fopen(filename, "r");
    FILE *outputFile = fopen("error.txt", "w");

    if (inputFile == NULL || outputFile == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL)
    {
        fputs(buffer, outputFile);
    }
    fputs("\n", outputFile);
    fputs("E\n", outputFile);
    if (particles)
    {
        for (int i = 0; i < 100; i++)
        {
            free(particles[i]);
        }
        free(particles);
    }
    fclose(inputFile);
    fclose(outputFile);
}

// put the 2D int array in to the 2D char array, and write the output.txt file
void write_output(int rows, int cols, int arr[rows][cols], const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error opening output file.\n");
        return;
    }
    // create a 2D char array, which has 2 more rows and cols than 2D int array
    char **charArr = (char **)malloc((rows + 2) * sizeof(char *));
    for (int i = 0; i < rows + 2; i++)
    {
        charArr[i] = (char *)malloc((cols + 2) * sizeof(char));
    }
    // fill the 2D char array
    for (int i = 0; i < rows + 2; i++)
    {
        for (int j = 0; j < cols + 2; j++)
        {
            if (i == 0 || i == rows + 1 || j == 0 || j == cols + 1)
            {
                // set the boundary
                charArr[i][j] = '*';
            }
            else if (arr[i - 1][j - 1] == 1)
            {
                // turing 1 from arr to in charArr '+'
                charArr[i][j] = '+';
            }
            else
            {
                // turing 0 from arr to in charArr ' '
                charArr[i][j] = ' ';
            }
        }
    }
    // write the value of charArr into file
    for (int i = cols + 1; i >= 0; i--)
    {
        for (int j = 0; j < rows + 2; j++)
        {
            //printf("%c", charArr[j][i]);
            fputc(charArr[j][i], file);
        }
        //printf("\n");
        fputc('\n', file);
    }
    //printf("\n");
    fclose(file);
    // free the memory of charArr
    for (int i = 0; i < rows + 2; i++)
    {
        free(charArr[i]);
    }
    free(charArr);
}

//particles' move in one second
void move(int x_dim, int y_dim, int field[x_dim][y_dim], int particle[4])
{
    // set the timeRemain as 1 second, only consider 1 second movement in this function
    double timeRemain = 1;
    int maxX = x_dim - 1;
    int maxY = y_dim - 1;
    // the x and y location, using double because need to consider the speed reversed after hit the boundary
    double x = particle[0];
    double y = particle[1];
    // the velocity of particle
    int vx = particle[2];
    int vy = particle[3];
    // let the fild[x][y] -1 before move, cannot set it to zero
    field[(int)x][(int)y]--;
    while (timeRemain > 0)
    {
        // calculate the time from x,y to boundary
        double timeToXBoundary;
        if (vx > 0)
        {
            timeToXBoundary = ((maxX)-x) / vx;
        }
        else if (vx < 0)
        {
            timeToXBoundary = -x / vx;
        }
        else
        {
            timeToXBoundary = DBL_MAX;
        }
        double timeToYBoundary;
        if (vy > 0)
        {
            timeToYBoundary = ((maxY)-y) / vy;
        }
        else if (vy < 0)
        {
            timeToYBoundary = -y / vy;
        }
        else
        {
            timeToYBoundary = DBL_MAX;
        }
        // select the minimum time to reach the boundary
        double timeToBoundary = timeToXBoundary < timeToYBoundary ? timeToXBoundary : timeToYBoundary;
        // if timeToBoundary bigger than timeRemain, which means this the last running in the loop
        // so let  timeToBoundary = timeRemain
        if (timeToBoundary > timeRemain)
        {
            timeToBoundary = timeRemain;
        }
        // update the x, y location
        x += vx * timeToBoundary;
        y += vy * timeToBoundary;
        // check the velocity reversed x-direction or y-direction
        if (timeToBoundary == timeToXBoundary)
        {
            vx = -vx;
        }
        if (timeToBoundary == timeToYBoundary)
        {
            vy = -vy;
        }
        // update the timeRemain
        timeRemain -= timeToBoundary;
    }
    // let the fild[x][y] + 1 after move, cannot set it to 1
    field[(int)x][(int)y]++;
    // update the values of particle
    particle[0] = (int)x;
    particle[1] = (int)y;
    particle[2] = vx;
    particle[3] = vy;
}

//check collide of the particles at the stoping status
void checkCollide(int x, int y, int particle_count, int **particles)
{
    for (int i = 0; i < particle_count; i++)
    {
        if (particles[i][0] == x && particles[i][1] == y)
        {
            particles[i][0] = -1;
            particles[i][1] = -1;
            particles[i][2] = 0;
            particles[i][3] = 0;
        }
    }
}

//The function to run the whole process of the particles movement
void process(int x_dim, int y_dim, int time, int particle_count, int **particles, const char *filename)
{
    // check if the x_dim and y_dim not correct when intialized
    if (x_dim <= 0 || y_dim <= 0)
    {
        printf("Field size is smaller than 1*1 \n");
        write_error("input.txt", particles);
        return;
    }
    for (int i = 0; i < particle_count; i++)
    {
        // check whether any particle outside the boundary when intialized
        if (particles[i][0] < 0 || particles[i][0] > x_dim - 1 || particles[i][1] < 0 || particles[i][1] > y_dim - 1)
        {
            printf("Particle is out of the boundary \n");
            write_error("input.txt", particles);
            return;
        }
        // when velocity has angle，check if the angle is 45 degree direction
        if (particles[i][2] != 0 && particles[i][3] != 0)
        {
            if (abs(particles[i][2]) != abs(particles[i][3]))
            {
                printf("Velocity is non-45 degree direction \n");
                write_error("input.txt", particles);
                return;
            }
        }
    }
    // initialize the 2D int array
    int field[x_dim][y_dim];
    for (int i = 0; i < x_dim; i++)
    {
        for (int j = 0; j < y_dim; j++)
        {
            field[i][j] = 0;
        }
    }
    for (int i = 0; i < particle_count; i++)
    {
        field[particles[i][0]][particles[i][1]]++;
    }
    for (int i = 0; i < x_dim; i++)
    {
        for (int j = 0; j < y_dim; j++)
        {
            if (field[i][j] > 1)
            {
                //check collide before the movement
                checkCollide(i, j, particle_count, particles);
                field[i][j] = 0;
            }
        }
    }
    printArrayNum(x_dim, y_dim, field);
    printf("-----------------------------------\n");
    int second = 1;
    while (second <= time)
    {
        for (int i = 0; i < particle_count; i++)
        {
            // if the x of particle is -1, means particle is deleted
            if (particles[i][0] == -1)
            {
                continue;
            }
            //particles' move in one second
            move(x_dim, y_dim, field, particles[i]);
        }
        //check collide after each second movement, if field[i][j]>1 mean collide happened
        for (int i = 0; i < x_dim; i++)
        {
            for (int j = 0; j < y_dim; j++)
            {
                if (field[i][j] > 1)
                {
                    checkCollide(i, j, particle_count, particles);
                    field[i][j] = 0;
                }
            }
        }
        second++;
    }
    printArrayNum(x_dim, y_dim, field);
    write_output(x_dim, y_dim, field, filename);
}

int main()
{
    int x_dim, y_dim, time, particle_count;
    int **particles = read_input("input.txt", &x_dim, &y_dim, &time, &particle_count);
    if (particles)
    {
        //The function to run the whole process of the particles movement
        process(x_dim, y_dim, time, particle_count, particles, "output.txt");
        for (int i = 0; i < 100; i++)
        {
            free(particles[i]);
        }
        free(particles);
    }
    return 0;
}