//Kruthiventi Shyama Subrahmanya Nikhil

#define rows 3000
#define cols 500

int arr2D[rows][cols];

int main()
{
	for(int row = 0;row < rows;row++)
	{
		for(int col = 0;col < cols;col++)
		{
			arr2D[row][col] = row + col;
		}
	}
	return 0;
}
