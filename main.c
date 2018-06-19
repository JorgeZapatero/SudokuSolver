#include <stdio.h>
#include <stdlib.h>

#define RESET "\x1b[0m"
#define RED "\x1B[31m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"

int board[9][9];
int game1[81] = { 3,0,0,5,0,0,6,0,0,
                  0,2,7,8,0,0,0,0,3,
                  8,0,5,6,0,0,9,0,0,
                  0,6,1,7,3,0,0,0,2,
                  2,5,8,0,0,0,1,3,7,
                  7,0,0,0,2,5,8,9,0,
                  0,0,6,0,0,7,2,0,5,
                  5,0,0,0,0,1,7,6,0,
                  0,0,2,0,0,6,0,0,1
                };

int game2[81] = { 1,3,0,0,6,5,2,0,7,
                  0,0,6,0,0,0,0,9,8,
                  9,0,7,0,8,4,0,0,0,
                  0,1,0,2,0,0,0,0,3,
                  6,0,3,0,0,0,7,0,5,
                  7,0,0,0,0,3,0,8,0,
                  0,0,0,4,3,0,8,0,1,
                  8,6,0,0,0,0,9,0,0,
                  3,0,4,8,9,0,0,5,2
                };
int game3[81] = { 2,9,5,7,0,0,8,6,0,
                  0,3,1,8,6,5,0,2,0,
                  8,0,6,0,0,0,0,0,0,
                  0,0,7,0,5,0,0,0,6,
                  0,0,0,3,8,7,0,0,0,
                  5,0,0,0,1,6,7,0,0,
                  0,0,0,5,0,0,1,0,9,
                  0,2,0,6,0,0,3,5,0,
                  0,5,4,0,0,8,6,7,2
                };

int game4[81] = { 0,0,0,2,0,4,8,1,0,
                  0,4,0,0,0,8,2,6,3,
                  3,0,0,1,6,0,0,0,4,
                  1,0,0,0,4,0,5,8,0,
                  6,3,5,8,2,0,0,0,7,
                  2,0,0,5,9,0,1,0,0,
                  9,1,0,7,0,0,0,4,0,
                  0,0,0,6,8,0,7,0,1,
                  8,0,0,4,0,3,0,5,0
                };

void eliminate(int * in, int n){
    if (n<1 || n>9) return;

    int mask = 0b1;
    mask <<= (n-1);
    mask = ~mask;

    *in = (*in) & (mask);
}

// returns 0 if no single solution is found
// returns 1 if a single solution was found
// sets the input integer to the single solution
int single_solution(int * markup_num){
    if(*markup_num < 0b1000000000){
        printf("!! Function 'single_solution' was passed a value that was not a markup value\n (Too small)\n");
        return 0;
    }
    
    int i, num, count, res;
    
    num = *markup_num;
    count = 0;
    res = 0;

    for(i=0; i<9; i++){
        if (num%2 == 1){
            res = i+1;
            count++;
        }
        if (count > 1) return 0;

        num/=2;
    }

    if(count  == 0) {
        printf("!! Function 'single_solution' was passed a value that was not a markup value\n (Empty)\n");
        printf("Value was: %i\n", *markup_num);
        return 0;
    }

    *markup_num = res;
    return 1;
}

int markup_length(int n){
    int i, count;
    count = 0;
    for (i=0; i<9; i++){
        count += n%2;
        n/=2;
    }

    return count;
}

//returns # of entries in the res array
int get_markup_contents(int n, int * res){
    int i, count = 0;
    for(i = 0; i < 8; i++){
        if(n%2 == 1){
            res[count] = i+1;
            count++;
        }
        n/=2;
    }
    return count;
}

void scrub(int * board_val, int num){
    int val = * board_val;

    printf("%i becomes ", val);
    if(val >9 && val != num){
        *board_val = ((num ^ val) & val) + 512;
        single_solution(board_val);
    }
    printf("%i\n", *board_val);
}

void scrub_collumn(int c, int num){
    int i;
    for (i = 0; i<9; i++){
        scrub(&board[i][c], num);
    }
}

void scrub_row(int r, int num){
    int i;
    for (i = 0; i<9; i++){
        scrub(&board[r][i], num);
    }
}

void scrub_square(int r, int c, int n){
    //use % to collapse r and c int proper square index
    //run scrub function on each position in the square (double for loop)
}

int find_preemptive_set(){
    //select an anchor index pair
    //  +search in collumn for a preemptive set
    //     + if a set is found
    //         * does the set exist in the same box?
    //             * scrub the box
    //     + scrub the collumn
    //  + search in a row
    //      + if a set is found
    //          * does the set exist in the same box?
    //              * scrub the box
    //      + scrub the row


    int i,j, m,n;
    int len, num, count;
    int markup_contents[8];
    for(i=0; i<9; i++){ //for each position
        for(j=0; j<9; j++){

            
            num = board[i][j];
            if (num>0 && num<10) continue; //skip solutions

            len = markup_length(num);
            if (len > 8) continue; // too many solutions
            if (len < 2) { //shouldn't be just on solution
                printf("!! markup len = %i, markup = %i, too small !!\n",len,num);
                continue;
            }

            //SEARCH IN COLLUMN
            count = 0;
            for (m=0; m<9; m++){
                if (board[m][j] == num){
                    count++;
                    printf("count++\n");
                }
            }
            printf("count: %i, len: %i\n", count, len);
            if (count == len){
                printf("preemptive set found for %i, len: %i\n", num, len);
                scrub_collumn(j, num);
                return 1;
            }


            //SEARCH IN ROW
            count = 0;
            for (m=0; m<9; m++){
                if (board[i][m] == num){
                    count++;
                    printf("count++\n");
                }
            }
            printf("count: %i, len: %i\n", count, len);
            if (count == len){
                printf("preemptive set found for %i, len: %i\n", num, len);
                scrub_row(i, num);
                return 1;
            }
        }
    }
    return 0;
}



void print_markup_value(int n){
    int i;
    for(i=0; i<9; i++){
        if(n%2 == 1) printf(" %i", i+1);
        n/=2;
    }
}

//returns the number of "apparent" solutions found
int markup(){
    int i,j, m,n;
    int x,y;
    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
            if (board[i][j] > 0 && board[i][j]< 10) continue;
            
            board[i][j] = 0b1111111111;
            for(m = 0; m < 9; m++){
                eliminate( &board[i][j], board[m][j] );
                eliminate( &board[i][j], board[i][m] );
            }

            x = i/3;
            y = j/3;

            for(m = x*3; m<x*3+3; m++){
                for(n = y*3; n<y*3+3; n++){
                    eliminate( &board[i][j], board[m][n]);
                }
            }

            if (single_solution( &board[i][j]) == 1){
                
                
                //!!UNCOMENT for RECURSIVE MARKUP()
                //return markup() +1;
            }
        }
    }
    return 0;
}

void print_board(){
    int i,j,c;
    char disp;

    printf("\n    1 2 3 4 5 6 7 8 9\n");
    printf(  "    –––––––––––––––––\n");
    
    for(i=0; i<9; i++){
        printf("%c | ", (i + 1) + 48);
        
        c = 1;
        if(i > 2 && i < 6) c *= -1;

        for(j=0; j<9; j++){

            if (j==3 || j==6) c = c * (-1);
            
            disp = '-';

            if (board[i][j] > 0 && board[i][j] < 10) disp = board[i][j] + 48;

            if (c>0) printf(RED "%c " RESET, disp);
            else     printf(BLU "%c " RESET, disp); 
        }
        printf("\n");
    }

    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
            if(board[i][j]<10) continue;

            printf("[%i,%i]:",i+1,j+1);
            print_markup_value(board[i][j]);
            printf("\n");

            //printf("[%i,%i]:%i\n", i+1,j+1, board[i][j]);
        }
    }
}

int main(){



    int i, j, k;
    
    i = -1;
    j =  0;
    for(k=0; k<81; k++){
        
        j = k % 9;
        if (j==0) i++;
        board[i][j] = game3[k];
    }


    print_board();
    int found = markup();
    printf("\nmarkup found %i apparent solutions\n", found);
    print_board();

    found = find_preemptive_set();
    if(found == 1) printf("preemptive set succesful\n");
    else printf("preemptive set unsuccesful\n");





    
//    int found = 1;
//    while(found > 0){
//        found = markup();
//        printf("markup found apparent %i solutions\n", found);
//    }

// TEST eliminate
//    int testn = 0b1111;
//    printf("in: %i", testn);
//    eliminate( &testn, 3);
//    printf(", res: %i\n", testn);
// END TEST eliminate    


//TEST single_solution
//    int testn = 0b1000000000;
//    printf("in: %i", testn);
//    int success = single_solution( &testn);
//    printf(", res: %i, %i\n", success, testn);
// END TEST single_solution    

//TEST markup_length
//    int testn = 0b1010011000;
//    printf("in: %i", testn);
//    int res = markup_length(testn);
//    printf(", res: %i\n", res);
// END TEST markup_length    

//TEST get_markup_contents
//    int res[8];
//    int testn = 0b1010011000;
//    int len = get_markup_contents(testn, res);
//    printf("in: %i\n", testn);
//    printf("length: %i\n", len);
//    printf("{ ");
//    int a;
//    for(a=0; a<len; a++){
//        printf("%i ", res[a]);
//    }
//    printf("}\n");
// END TEST markup_length    


    printf("fin.\n");	

//    printf("%i\n", (0b1101110000 ^ 0b1001010000) + 512);
}


