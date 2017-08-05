// pgm to check the 2 arrays contain the same element or not
#include<stdio.h>
main()
{
	int limit,idx,jdx,found;
		printf("enter the limit\n");
	scanf("%d",&limit);
	int arr1[limit],arr2[limit];
	printf("enter the first array element\n");
	for (idx = 0;idx < limit ; idx++)
	{
		scanf("%d",&arr1[idx]);
	}
	printf("enter the second array element\n");
	for(idx = 0;idx < limit; idx++)
	{
		scanf("%d",&arr2[idx]);
	}
	for(jdx=0;jdx<limit;jdx++)
	{
		found=0;
	for(idx=0;idx<limit;idx++)
	{
		if(arr1[jdx] == arr2[idx])
		{
		found=1;
		break;
    	}
	}
		if(!found)
			break;
    }
	if(found)
		printf("contain\n");
	else
		printf("not contain\n");
}

