//Kruthiventi Shyama Subrahmanya Nikhil

#define rows 128
#define cols 8

int arr2D[rows][cols];

int main()
{
	for(int iteration = 0;iteration < 100;iteration++)
	{
		for(int row = 0;row < rows;row = row + 64)
		{
			for(int col = 0;col < cols;col++)
			{
				arr2D[row][col] = iteration + row + col;
			}
		}
	}
	return 0;
}
