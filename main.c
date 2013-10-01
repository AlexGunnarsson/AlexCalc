#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int MAXlength = 20;
char leftPar = '(';
char rightPar = ')';

struct charNUM
{
    int  singleNum[20];
    int  numLength;
    int  decimals;

};

struct NUMOP
{
    double actualNum;
    char operation;
    char parenthesis;
};

double x_2pow_y (int base, int power)
{
    int i, absolutePower = power;
    double newbase = base;

    if (power == 0)
        return 1;
    else if (power<0)
            absolutePower = -power;

    for (i=0; i<absolutePower-1; i++)
        newbase*=base;

    if (power<0)
        newbase = base / (newbase * base);
    return newbase;
}

double char2num(struct charNUM NUM)
{

    int i,length = NUM.numLength + NUM.decimals;
    double totalnumber=0.0, pow10 = x_2pow_y (10, NUM.numLength-1);


    if (NUM.numLength == 1)
        pow10 = 1;

    for (i=0; i<length; i++)
    {
        totalnumber += NUM.singleNum[i] * pow10;

        pow10 /= 10;
    }
    return totalnumber;
}

double calculatewithPRIO (struct NUMOP EXPRESSION[], int k)
{
    bool loop_virgin = 1;
    int i = 0, j;
    struct NUMOP store[MAXlength];
    double finalResult=0.0;

    for (j=0; j<k; j++)
    {

        while (EXPRESSION[i].operation == '^'){
            if (loop_virgin){
                store[j].actualNum = EXPRESSION[i].actualNum;
                loop_virgin = 0;
            }
            ++i;

            store[j].actualNum = pow((store[j].actualNum), (EXPRESSION[i].actualNum));
            store[j].operation = EXPRESSION[i].operation;

        }
            if (loop_virgin)
                store[j] = EXPRESSION[i];

            ++i;
            loop_virgin = 1;
    }

    i = 0;
    k = j;


    for (j=0; j<k; j++)
    {
        while (store[i].operation == '*'){
            if (loop_virgin){
                store[j].actualNum = store[i].actualNum;
                loop_virgin = 0;
            }

            store[j].actualNum *= store[i + 1].actualNum;
            store[j].operation = store[i + 1].operation;
            ++i;
        }
            if (loop_virgin)
                store[j] = store[i];

            ++i;
            loop_virgin = 1;
    }

    i = 0;
    k = j;

    for (j=0; j<k; j++)
    {
        while (store[i].operation == '/'){
            if (loop_virgin)
                store[j].actualNum = store[i].actualNum; loop_virgin = 0;

            store[j].actualNum /= store[i + 1].actualNum;
            store[j].operation = store[i + 1].operation;
            ++i;
        }
            if (loop_virgin)
                store[j] = store[i];

            ++i;
            loop_virgin = 1;

    }

    k = j;
    finalResult = store[0].actualNum;
    for (i=0; i<k; i++){
        if (store[i].operation == '+')
            finalResult += store[i+1].actualNum;
        else if (store[i].operation == '-')
            finalResult -=store[i+1].actualNum;
    }


    return finalResult;
}

double deal_with_parentheses (struct NUMOP NumberStruct[], int k)
{
    bool bracketflag = 0;
    int a=0, q, i,j;
    struct NUMOP tempstore[MAXlength], bracket_seq;
    double result=0;



    for (i=0; i<=k; i++){

        if (NumberStruct[i].parenthesis == leftPar){

            NumberStruct[i].parenthesis = '0';
            tempstore[a] = NumberStruct[i];
            a++;
            j=i+1;

            do{
                if (NumberStruct[j].parenthesis == leftPar){
                    a=0;
                    NumberStruct[i].parenthesis = leftPar;
                    break;
                }
                else{
                    tempstore[a] = NumberStruct[j];
                    a++;
                }

            }while (NumberStruct[j++].parenthesis != rightPar);


           if (NumberStruct[j].parenthesis == leftPar)
                continue;

            --j;

            NumberStruct[j].parenthesis = '0';
            tempstore[j].parenthesis = '0';

            if (a-1!=k){
                tempstore[a].operation = tempstore[a-1].operation;
                tempstore[a-1].operation = '0';
                bracket_seq.actualNum = calculatewithPRIO(tempstore, a);
                bracket_seq.operation = tempstore[a].operation;
            }
            else{
                result = (calculatewithPRIO(tempstore, k+1));
                return result;
            }
            for (q=0; q<=((k+1)-(a)); q++){
                if (bracketflag)
                    NumberStruct[q] = NumberStruct[q+(a-1)];

                if (NumberStruct[q].actualNum == tempstore[0].actualNum){ // Remember to change this to more general case
                    NumberStruct [q] = bracket_seq;
                    bracketflag = 1;
                }
            }

            k = (k-a)+2;
            bracketflag = 0;
            i=0;
            a=0;




        }




    }

    result = (calculatewithPRIO(NumberStruct, (k+1)));
    return result;





}

int calculator (void)
{


    typedef int counters;
    double totalCount=0.0;
    char currentChar = 'I', previousChar;
    bool decimalFlag;
    counters j, k=0, z=0;

    char NUMchars[] = {'1','2','3','4','5','6','7','8','9','0'};
    int NUMmatch[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    struct charNUM correspondingNum[MAXlength];
    struct NUMOP EXPRESSION[MAXlength];

    EXPRESSION[0].parenthesis = '0';

    printf("Write expression here: (Type Q to quit)\n");

    for (j=0; j<MAXlength; j++){
        correspondingNum[j].numLength = 0;
        correspondingNum[j].decimals = 0;
    }

    while (currentChar!='\n')
    {
        currentChar = getchar();

        if (currentChar=='Q')
            return 0;
        else if (currentChar == '\n')
            break;

        for (j=0; j<MAXlength; j++){
            if (currentChar == NUMchars[j]){
                correspondingNum[k].singleNum[z] = NUMmatch[j];
                if (!decimalFlag)
                    ++correspondingNum[k].numLength;
                else
                    ++correspondingNum[k].decimals;

                ++z;
                break;

            }
            if (j == MAXlength - 1){
                    if (currentChar == '.'){
                        decimalFlag = 1;
                        break;
                    }
                    else
                        decimalFlag = 0;

                    if (currentChar == leftPar){
                            if (previousChar == leftPar){
                                ++k;
                                EXPRESSION[k].parenthesis = currentChar;
                                EXPRESSION[k-1].operation = '+';
                                EXPRESSION[k-1].actualNum = 0;
                                break;

                            }
                            else
                                EXPRESSION[k].parenthesis = currentChar;

                            previousChar = currentChar;
                            z = 0;
                            break;
                    }


                   if (currentChar == rightPar){
                            if (previousChar == rightPar){
                                ++k;
                                EXPRESSION[k].parenthesis = currentChar;
                                EXPRESSION[k-1].operation = '+';
                                EXPRESSION[k].actualNum = 0;

                                break;

                            }
                            else
                                EXPRESSION[k].parenthesis = currentChar;

                            previousChar = currentChar;
                            z = 0;
                            break;
                    }


                EXPRESSION[k].operation = currentChar;
                ++k;
                EXPRESSION[k].parenthesis = '0';
                z=0;
                previousChar = currentChar;
            }
        }

    }

    for (j=0; j<=k; j++){

        EXPRESSION[j].actualNum = char2num(correspondingNum[j]);
        if (EXPRESSION[j].actualNum == 0.0 && EXPRESSION[j-1].operation == '/'){
            printf("NOOOOOOOOOOOOOO  *UNIVERSE IMPLODES*\n\n");
            return 0;
        }
    }

    totalCount = deal_with_parentheses(EXPRESSION, k);

    printf ("=%.3f\n\n\n", totalCount);

    return 1;
}

int main()
{
    while(calculator());

    return 0;
}
