/////////////////////////////////////////////////////////////////////////////////////
// File       : urca.c
// Language   : C/C++
// Description: Unate Recursive Complement Algorithm
// Author     : Sandesh Prabhakar
// Created    : March 10, 2013
// Modified   : August 19, 2026
// Version    : 1.0
//
// Copyright (c) 2026 Sandesh Prabhakar
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


////////////////////////////////////////////////////////////////////////
// circuit class
////////////////////////////////////////////////////////////////////////

class circuit
{
    // circuit information
    int numvars;                 // total number of variables
    int numcubes;                // total number of cubes
    int **cubelist;    		     // input cube list
    int **complement_cubelist;   // output complement cube list


    // you may wish to add more private variables here

public:
    circuit(char *);   // constructor

    // you may wish to add more function declarations (or public variables) here
	int Complement (int** complement_cubelist, int numvars );	
	int positiveCofactor (int** complement_cubelist, int** P_cubelist, int x, int numvars );
    int negativeCofactor (int** complement_cubelist, int** N_cubelist, int x, int numvars );

};

////////////////////////////////////////////////////////////////////////
// constructor: reads in the *.cubes file and builds basic data structure
// 		for the cube list 
////////////////////////////////////////////////////////////////////////
circuit::circuit(char *cktName)
{
    FILE *inFile;
    FILE *outFile;
    char fName[40];
	char foName[40];
    int i, j, count;
    char c;
    int f1, f2, f3;
    int numdontcares; 
    strcpy(fName, cktName);
    strcat(fName, ".cubes");
    strcpy(foName, cktName);
    strcat(foName, ".out.cubes");

    inFile = fopen(fName, "r");
    if (inFile == NULL)
    {
	fprintf(stderr, "Can't open .cubes file\n");
	exit(-1);
    }

    outFile = fopen(foName, "w");
    if (outFile == NULL)
    {
	fprintf(stderr, "Can't open out.cubes file to write\n");
	exit(-1);
    }


    fscanf(inFile, "%d", &numvars);	 // number of variables
    fscanf(inFile, "%d", &numcubes); // number of cubes

    // allocate space for cubes data structure
	int max_cubes = int (pow (2,10)); 
    cubelist = new int * [max_cubes+1];

	for (i=0; i<=max_cubes; i++)
	{
		cubelist[i] = new int[numvars+1];
		for (j=0; j<=numvars; j++)
		{
		    cubelist[i][j] = 0;
		}
	}

    complement_cubelist = new int * [max_cubes+1];

	for (i=0; i<=max_cubes; i++)
	{
		complement_cubelist[i] = new int[numvars+1];
		for (j=0; j<=numvars; j++)
		{
		    complement_cubelist[i][j] = 0;
		}
	}

	cubelist[0][0] = numcubes;

    // now read in the cubes
    for (i=1; i<=numcubes; i++)
    {
	fscanf(inFile, "%d", &numdontcares);
	cubelist[i][0] = numdontcares;

        for (j=1; j<=numdontcares; j++)
    	{
		fscanf(inFile, "%d", &f1);
			if(f1<0)
				{
				f1=-(f1);
				cubelist[i][f1] = -1;
				}
			else
				{
				cubelist[i][f1] = 1;
				}
		}
	}

	// skip till end of line
	while ((c = getc(inFile)) != '\n' && c != EOF)
	    ;
//    }	// for (i...)
    fclose(inFile);

    printf("Successfully read in cubes:\n");
	printf("\tPrinting input cube list\n");
    printf("\t%d No. of variables.\n", numvars);
    printf("\t%d No. of cubes.\n", cubelist[0][0]);

    for (i=1; i<=numcubes; i++)
    {
		printf("\t%d\t", cubelist[i][0]);
		for (j=1; j<=numvars; j++)
    	{
			if(cubelist[i][j]<0)
				{
				printf("\t-%d\t", j);
				}
			if(cubelist[i][j]>0)
				{
				printf("\t%d\t", j);
				}
		}
    	printf("\n");
	}

	printf("\tCopying the input cube list to the complement cube list\n");

    for (i=0; i<=numcubes; i++)
    {
		for (j=0; j<=numvars; j++)
    	{
			complement_cubelist[i][j]=cubelist[i][j];
		}
	}

	printf("\tPrinting copied complement cube list\n");
    printf("\t%d No. of variables.\n", numvars);
    printf("\t%d No. of cubes.\n", complement_cubelist[0][0]);

    for (i=1; i<=complement_cubelist[0][0]; i++)
    {
		printf("\t%d\t", complement_cubelist[i][0]);
		for (j=1; j<=numvars; j++)
    	{
			if(complement_cubelist[i][j]<0)
				{
				printf("\t-%d\t", j);
				}
			if(complement_cubelist[i][j]>0)
				{
				printf("\t%d\t", j);
				}
		}
    	printf("\n");
	}

Complement (complement_cubelist, numvars);


	printf("\tPrinting Unate Recursive complement cube list\n");
    printf("\t%d No. of variables.\n", numvars);
    printf("\t%d No. of cubes.\n", complement_cubelist[0][0]);

    printf("%d\n", numvars);
    fprintf(outFile, "%d\n", numvars);	 // number of variables

    printf("%d\n", complement_cubelist[0][0]);
    fprintf(outFile, "%d\n", complement_cubelist[0][0]);


    for (i=1; i<=complement_cubelist[0][0]; i++)
    {
		printf("%d", complement_cubelist[i][0]);
		fprintf(outFile, "%d", complement_cubelist[i][0]);

		for (j=1; j<=numvars; j++)
    	{
			if(complement_cubelist[i][j]<0)
				{
				printf(" -%d", j);
				fprintf(outFile, " -%d", j);
				}
			if(complement_cubelist[i][j]>0)
				{
				printf(" %d", j);
				fprintf(outFile, " %d", j);
				}
		}
    	printf("\n");
    	fprintf(outFile, "\n");
	}

    fclose(outFile);

}

////////////////////////////////////////////////////////////////////////
// other member functions here
////////////////////////////////////////////////////////////////////////
		int circuit::Complement (int** complement_cubelist, int numvars )
                 {
				int i, j, check_empty, temp[numvars+1], temp_id[numvars+1], var[numvars+1][4], uvar[numvars+1][4], bvar[numvars+1][4];
				int temp1, temp2, temp3, i_bvar, i_uvar, index_bvar[numvars+1],index_uvar[numvars+1] ;
				int flag=0;
						for (i=0; i<=numvars; i++)
							{
							temp[i]=0;
							temp_id[i]=0;
							var[i][0]=0;
							var[i][1]=0;
							var[i][2]=0;
							var[i][3]=0;
							uvar[i][0]=0;
							uvar[i][1]=0;
							uvar[i][2]=0;
							uvar[i][3]=0;
							bvar[i][0]=0;
							bvar[i][1]=0;
							bvar[i][2]=0;
							bvar[i][3]=0;
							index_bvar[i]=0;
							index_uvar[i]=0;
							}
					i_bvar=0;
					i_uvar=0;

    int **P_cubelist;    		     // positive cofactor cube list
    int **N_cubelist;   // negative cofactor cube list
    // allocate space for cubes data structure
	int max_cubes = int (pow (2,10)); 
    P_cubelist = new int * [max_cubes+1];
    N_cubelist = new int * [max_cubes+1];

	for (i=0; i<=max_cubes; i++)
	{
		P_cubelist[i] = new int[numvars+1];
		N_cubelist[i] = new int[numvars+1];

		for (j=0; j<=numvars; j++)
		{
		    P_cubelist[i][j] = 0;
		    N_cubelist[i][j] = 0;
		}
	}

	printf("\tCopying the input cube list to the cofactor cube list\n");

    for (i=0; i<=complement_cubelist[0][0]; i++)
    {
		for (j=0; j<=numvars; j++)
    	{
			P_cubelist[i][j]=complement_cubelist[i][j];
			N_cubelist[i][j]=complement_cubelist[i][j];
		}
	}

 	//	if ( F is simple and we can complement it directly )
	//  return( directly computed complement of F )
    int set_flag=0;
				//if number of cubes is 0
					if (complement_cubelist[0][0]==0)
						{
						set_flag=1;
						complement_cubelist[0][0]=1;  //number of cubes
						complement_cubelist[1][0]=0;  //number of non-dont cares
								for (j=1; j<=numvars; j++)
    								{
									complement_cubelist[1][j]=0;
									}
						}


				//if number of cubes is 1
					if ((complement_cubelist[0][0]==1)&&(set_flag==0))
						{
						check_empty=0;
						for (j=1; j<=numvars; j++)
    						{
							if (complement_cubelist[1][j]!=0)
									{
									check_empty++;
									printf(" I am here: check_empty=%d \n",check_empty);
									temp[j]=complement_cubelist[1][j];
									printf(" temp[%d]=%d\n",j,temp[j]);
									temp_id[check_empty]=j;
									printf(" temp_id[%d]=%d\n",check_empty,temp_id[check_empty]);
									}
							}

						if(check_empty==0)
							{
						complement_cubelist[0][0]=0;
							}

						if(check_empty>0)
							{
							flag=1;
							printf(" I am here final: check_empty=%d \n",check_empty);
							complement_cubelist[0][0]=check_empty;
								for (i=1; i<=check_empty; i++)
    								{
									complement_cubelist[i][0]=1;
									for (j=1; j<=numvars; j++)
										{
										complement_cubelist[i][j]=0;
										}
									}

								for (i=1; i<=check_empty; i++)
    								{
									complement_cubelist[i][temp_id[i]]=-(temp[temp_id[i]]);
									}

	printf("Printing intermediate check_empty > 0 complement cube list\n");
    printf("%d No. of variables.\n", numvars);
    printf("%d No. of cubes.\n", complement_cubelist[0][0]);
    printf("%d\n", numvars);
    printf("%d\n", complement_cubelist[0][0]);

    for (i=1; i<=complement_cubelist[0][0]; i++)
    {
		printf("%d", complement_cubelist[i][0]);
		for (j=1; j<=numvars; j++)
    	{
			if(complement_cubelist[i][j]<0)
				{
				printf(" -%d", j);
				}
			if(complement_cubelist[i][j]>0)
				{
				printf(" %d", j);
				}
		}
    	printf("\n");
	}

							}

						}


				//if number of cubes is more than 1
					if ((complement_cubelist[0][0]>1)&&(flag==0))
						{
						int empty_flag=0;
							for (i=1; i<=complement_cubelist[0][0]; i++)
								{
									check_empty=0;
									for (j=1; j<=numvars; j++)
    									{
										if (complement_cubelist[1][j]!=0)
												{
												check_empty++;
												}
										}
									if(check_empty==0)
										{
										complement_cubelist[0][0]=0;
										empty_flag=1;
										}
								}


							if((check_empty>0)&&(empty_flag==0))
								{
									for (i=1; i<=numvars; i++)
    									{
											for (j=1; j<=complement_cubelist[0][0]; j++)
												{
											//	printf("complement_cube[%d][%d]=%d \n",j,i,complement_cubelist[j][i]);
												if(complement_cubelist[j][i]>0)	
													{
													var[i][0]=var[i][0]+1;
											//		printf("var[%d][0]=%d\n",i,var[i][0]);
													}
												if(complement_cubelist[j][i]<0)	
													{
													var[i][1]=var[i][1]+1;
											//		printf("var[%d][1]=%d\n",i,var[i][1]);
													}
												}
											//	printf("var[%d][0]=%d    var[%d][1]=%d\n",i,var[i][0],i,var[i][1]);
											if(var[i][0]>0 && var[i][1]>0) //if binate
												{
													i_bvar++;
											   		index_bvar[i_bvar]=i;	
													bvar[i][0]=var[i][0];
													bvar[i][1]=var[i][1];
													bvar[i][2]=bvar[i][0] + bvar[i][1];

													if(bvar[i][0]<=bvar[i][1])
														{
														bvar[i][3]=bvar[i][1]-bvar[i][0];
														}
													if(bvar[i][0]>bvar[i][1])
														{
														bvar[i][3]=bvar[i][0]-bvar[i][1];
														}
												}			
											else    //if unate
												{
													i_uvar++;
											   		index_uvar[i_uvar]=i;	
													uvar[i][0]=var[i][0];
													uvar[i][1]=var[i][1];
													uvar[i][2]=uvar[i][0] + uvar[i][1];
												}
										}

									if(i_bvar > 0) //if binate
										{
												temp1=bvar[index_bvar[1]][2];
												temp2=bvar[index_bvar[1]][3];
												temp3=index_bvar[1];

										for (i=2; i<=i_bvar; i++)
											{
												if(temp1<bvar[index_bvar[i]][2])
													{
														temp1=bvar[index_bvar[i]][2];
														temp2=bvar[index_bvar[i]][3];
														temp3=index_bvar[i];
													}
												if(temp1==bvar[index_bvar[i]][2])
													{
														if(temp2>bvar[index_bvar[i]][3])
															{
															temp1=bvar[index_bvar[i]][2];
															temp2=bvar[index_bvar[i]][3];
															temp3=index_bvar[i];
															}
													}
											}
										}
									else //if unate
										{
												temp1=uvar[index_uvar[1]][2];
												temp3=index_uvar[1];

										for (i=2; i<=i_uvar; i++)
											{
												if(temp1<uvar[index_uvar[i]][2])
													{
														temp1=uvar[index_uvar[i]][2];
														temp3=index_uvar[i];
													}
											}
										}

					 			// do recursion
								// let x = most binate variable for splitting
							    int x = temp3;	
								printf("most binate variable for splitting is %d \n",x);
								positiveCofactor( complement_cubelist, P_cubelist, x, numvars );
								negativeCofactor( complement_cubelist, N_cubelist, x, numvars );

								printf("\tPrinting positive cofactor cube list\n");

							    for (i=1; i<=P_cubelist[0][0]; i++)
								    {
									printf("\t%d\t", P_cubelist[i][0]);
										for (j=1; j<=numvars; j++)
									    	{
											if(P_cubelist[i][j]<0)
												{
												printf("\t-%d\t", j);
												}
											if(P_cubelist[i][j]>0)
												{
												printf("\t%d\t", j);
												}
											}
							    	printf("\n");
									}

								printf("\tPrinting negative cofactor cube list\n");

							    for (i=1; i<=N_cubelist[0][0]; i++)
								    {
									printf("\t%d\t",N_cubelist[i][0]);
										for (j=1; j<=numvars; j++)
									    	{
											if(N_cubelist[i][j]<0)
												{
												printf("\t-%d\t", j);
												}
											if(N_cubelist[i][j]>0)
												{
												printf("\t%d\t", j);
												}
											}
							    	printf("\n");
									}



								// cubeList P = Complement( positiveCofactor( F, x ) )
						    	Complement (P_cubelist, numvars );
								// cubeList N = Complement( negativeCofactor( F, x ) )
							    Complement (N_cubelist, numvars);

	printf("Printing intermediate complemented P cube list\n");
    printf("%d No. of variables.\n", numvars);
    printf("%d No. of cubes.\n", P_cubelist[0][0]);
    printf("%d\n", numvars);
    printf("%d\n", P_cubelist[0][0]);

    for (i=1; i<=P_cubelist[0][0]; i++)
    {
		printf("%d", P_cubelist[i][0]);
		for (j=1; j<=numvars; j++)
    	{
			if(P_cubelist[i][j]<0)
				{
				printf(" -%d", j);
				}
			if(P_cubelist[i][j]>0)
				{
				printf(" %d", j);
				}
		}
    	printf("\n");
	}

	printf("Printing intermediate complemented N cube list\n");
    printf("%d No. of variables.\n", numvars);
    printf("%d No. of cubes.\n", N_cubelist[0][0]);
    printf("%d\n", numvars);
    printf("%d\n", N_cubelist[0][0]);

    for (i=1; i<=N_cubelist[0][0]; i++)
    {
		printf("%d", N_cubelist[i][0]);
		for (j=1; j<=numvars; j++)
    	{
			if(N_cubelist[i][j]<0)
				{
				printf(" -%d", j);
				}
			if(N_cubelist[i][j]>0)
				{
				printf(" %d", j);
				}
		}
    	printf("\n");
	}


								// P = AND( x , P )
							    for (i=1; i<=P_cubelist[0][0]; i++)	
									{
									P_cubelist[i][x]=1;
									P_cubelist[i][0]++;
									}
								// N = AND( x', N)
								for (i=1; i<=N_cubelist[0][0]; i++)	
									{
									N_cubelist[i][x]=-1;
									N_cubelist[i][0]++;
									}
								// return( OR( P, N ) )
								int i_comp=0;
							    for (i=1; i<=P_cubelist[0][0]; i++)	
									{
										i_comp++;
									for (j=0; j<=numvars; j++)
										{	
										complement_cubelist[i][j]=P_cubelist[i][j];
										}
									}

							    for (i=1; i<=N_cubelist[0][0]; i++)	
									{
										i_comp++;
									for (j=0; j<=numvars; j++)
										{	
										complement_cubelist[i_comp][j]=N_cubelist[i][j];
										}
									}

								complement_cubelist[0][0]=i_comp;

								}
						}

	printf("Printing intermediate complement cube list\n");
    printf("%d No. of variables.\n", numvars);
    printf("%d No. of cubes.\n", complement_cubelist[0][0]);
    printf("%d\n", numvars);
    printf("%d\n", complement_cubelist[0][0]);

    for (i=1; i<=complement_cubelist[0][0]; i++)
    {
		printf("%d", complement_cubelist[i][0]);
		for (j=1; j<=numvars; j++)
    	{
			if(complement_cubelist[i][j]<0)
				{
				printf(" -%d", j);
				}
			if(complement_cubelist[i][j]>0)
				{
				printf(" %d", j);
				}
		}
    	printf("\n");
	}


			    } //function ends

//
		int circuit::positiveCofactor (int** complement_cubelist, int** P_cubelist, int x, int numvars )
                 {

						int i, j, p_index;
						p_index=0;
						for (i=1; i<=complement_cubelist[0][0]; i++)
							{
							if (complement_cubelist[i][x]==0) 
								{
								p_index++;
								for (j=1; j<=numvars; j++)
    								{
									P_cubelist[p_index][j]=complement_cubelist[i][j];
									}
									P_cubelist[p_index][0]=complement_cubelist[i][0];
								}

							if (complement_cubelist[i][x]>0)
								{
								p_index++;
								for (j=1; j<=numvars; j++)
    								{
									P_cubelist[p_index][j]=complement_cubelist[i][j];
									}
									P_cubelist[p_index][x]=0;
									P_cubelist[p_index][0]=complement_cubelist[i][0]-1;
								}

							if (complement_cubelist[i][x]<0)
								{
								}

							}

						P_cubelist[0][0]=p_index;

			    } //function ends

		int circuit::negativeCofactor(int** complement_cubelist, int** N_cubelist, int x, int numvars )
                {

						int i, j, n_index;
						n_index=0;
						for (i=1; i<=complement_cubelist[0][0]; i++)
							{
							if (complement_cubelist[i][x]==0) 
								{
								n_index++;
								for (j=1; j<=numvars; j++)
    								{
									N_cubelist[n_index][j]=complement_cubelist[i][j];
									}
									N_cubelist[n_index][0]=complement_cubelist[i][0];
								}

							if (complement_cubelist[i][x]<0)
								{
								n_index++;
								for (j=1; j<=numvars; j++)
    								{
									N_cubelist[n_index][j]=complement_cubelist[i][j];
									}
									N_cubelist[n_index][x]=0;
									N_cubelist[n_index][0]=complement_cubelist[i][0]-1;
								}

							if (complement_cubelist[i][x]>0)
								{
								}

							}

						N_cubelist[0][0]=n_index;
			    } //function ends
//
////////////////////////////////////////////////////////////////////////
// main starts here
////////////////////////////////////////////////////////////////////////
main(int argc, char *argv[])
{
    char cktName[20];
    circuit *ckt;
    int vecWidth, poNum;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <ckt>\n", argv[0]);
        exit(-1);
    }

    strcpy(cktName, argv[1]);

    // read in the cubes and build data structure
    ckt = new circuit(cktName);

    // NOW ADD YOUR FUNCTION HERE
}
