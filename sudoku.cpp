#include <iostream>
#include <set>
using namespace std;
int order = 1;
void print_state(int i,int j, set<int> (&state)[9][9])
{
	cout<<"state["<<i+1<<"]["<<j+1<<"] = ";
	set<int>::iterator it = state[i][j].begin();
	while(it != state[i][j].end())
	{
		cout<<*it<<" ";
		it++;
	}
	cout<<endl;
}

void print_state(set<int> &state)
{
	set<int>::iterator it = state.begin();
	while(it != state.end())
	{
		cout<<*it<<" ";
		it++;
	}
	cout<<endl;

}
void print_matrix(int (&matrix)[9][9])
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++) cout<<matrix[i][j]<<" ";
		cout<<endl;
	}
}

set<int> intersection(set<int> & state1, set<int> & state2)
{
	set<int> tmp;
	set<int>::iterator it = state1.begin();
	while(it != state1.end())
	{
		if(state2.find(*it) != state2.end()) tmp.insert(*it);
		it++;
	}
	return tmp;
}
set<int> merge_set(set<int> & state1, set<int> & state2)
{
	set<int> tmp;
	tmp.insert(state1.begin(), state1.end());
	tmp.insert(state2.begin(), state2.end());
	return tmp;
}

set<int> get_exclusive_set(set<int> &state)
{
	set<int> exclusive_set;
	for(int i = 1; i <=9; i++)
	{
		if(state.find(i) == state.end()) exclusive_set.insert(i);
	}
	return exclusive_set;
}

set<int>  get_posible_state(int i , int j, int (&matrix)[9][9] )
{
	set<int> tmp;
	if(matrix[i][j]!= 0) return tmp;
	for(int jj = 0; jj <9; jj++)
	{
		if(matrix[i][jj] != 0) tmp.insert(matrix[i][jj]);
	}
	for(int ii = 0; ii < 9; ii++)
	{
		if(matrix[ii][j] != 0) tmp.insert(matrix[ii][j]);
	}
	int I = i / 3;
	int J = j / 3;
	for(int ii = I * 3; ii < I * 3 + 3; ii++)
	{
		for(int jj = J * 3; jj < J*3+3; jj++)
		{
			if(matrix[ii][jj] != 0) tmp.insert(matrix[ii][jj]);
		}
	}
	return get_exclusive_set(tmp);
}

int check_num(int c,int (&matrix)[9][9])
{
	int mask[9][9]; // 0 for availabe, 1 for filled, 2 for c, 3 for restricted area
	for(int ii = 0; ii < 9; ii++)
	{
		for(int jj = 0; jj < 9; jj++)
		{
			if(matrix[ii][jj] == 0) mask[ii][jj] = 0;
			else if(matrix[ii][jj] != c) mask[ii][jj] = 1;
			else if(matrix[ii][jj] == c) mask[ii][jj] = 2;
		}
	}

	for(int ii = 0; ii < 9; ii++)
	{
		for(int jj = 0; jj < 9; jj++)
		{
			if(mask[ii][jj] == 2)
			{
				for(int i = 0; i < 9; i++)if(mask[i][jj] == 0) mask[i][jj]= 3;
				for(int j = 0; j < 9; j++)if(mask[ii][j] == 0) mask[ii][j]= 3;
				int I = ii / 3, J = jj / 3;
				for(int i = 3*I; i < 3*I + 3; i++)
				{
					for(int j = 3*J; j < 3*J + 3; j++)
					{
						if(mask[i][j] == 0) mask[i][j] = 3;
					}
				}
			}
		}
	}
	int ri = -1;
	int rj = -1;
	int count = 0;
	int numc = 0; // number of char c
	for(int i = 0; i < 9; i++)
	{
		ri = i;
		count = 0;
		numc = 0;
		for(int j = 0; j < 9; j++)
		{
			if(mask[i][j] == 0)
			{
				rj = j;
				count++;
			}
			else if(mask[i][j] == 2) numc++;
		}
		if(count == 1) 
		{
			matrix[ri][rj] = c;
			cout<<order++<<": ("<<ri+1<<","<<rj+1<<") "<<c<<" \tcheck_num horizontal"<<endl;
			return true;
		}
		else if(count == 0 && numc != 1)
		{
			return -1;
		}
	}
	for(int j = 0; j < 9; j++)
	{
		rj = j;
		count = 0;
		numc = 0;
		for(int i = 0; i < 9; i++)
		{
			if(mask[i][j] == 0)
			{
				ri = i;
				count++;
			}
			else if(mask[i][j] ==2) numc++;
		}
		if(count == 1)
		{
			matrix[ri][rj] = c;
			cout<<order++<<": ("<<ri+1<<","<<rj+1<<") "<<c<<" \tcheck_num vertical"<<endl;
			//if(c==2) print_matrix(mask);
			return true;
		}
		else if(count==0 && numc != 1) return -1;
	}

	for(int I = 0; I < 3; I++)
	{
		for(int J = 0; J < 3; J++)
		{
			count = 0;
			numc = 0;
			for(int ii = 3*I; ii < 3*I + 3; ii++)
			{
				for(int jj = 3*J; jj < 3*J + 3; jj++)
				{
					if(mask[ii][jj] == 0)
					{
						ri = ii;
						rj = jj;
						count++;
					}
					else if(mask[ii][jj] == 2) numc++;
				}
			}
			if(count == 1)
			{
				matrix[ri][rj] = c;
				cout<<order++<<": ("<<ri+1<<","<<rj+1<<") "<<c<<" \tcheck_num box"<<endl;
				//if(c==2) print_matrix(mask);
				return true;
			}
			else if(count == 0 && numc != 1) return -1;
		}
	}
	return false;
}
void copy_matrix(int (&from_matrix)[9][9], int (&to_matrix)[9][9])
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			to_matrix[i][j] = from_matrix[i][j];
		}
	}
}
int get_posible_states(set<int> (&posible_state)[9][9], int (&matrix)[9][9])
{
	int i, j;
First:
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			posible_state[i][j].clear();
			if(matrix[i][j]!=0) continue;
			set<int> tmp = get_posible_state(i,j,matrix);
			if(tmp.empty())
			{
				return -1;
			}
			else if(tmp.size() == 1)
			{
				matrix[i][j] = *tmp.begin();
				cout<<order++<<": ("<<i+1<<","<<j+1<<") "<<*tmp.begin()<<endl;
				goto First;
			}
			else posible_state[i][j] = tmp;
		}
	}
	return true;
}

int sudoku(int (& matrix)[9][9])
{
	set<int> posible_state[9][9];
Second:
	if(get_posible_states(posible_state, matrix) == -1) return -1;
	for(int c = 1; c <= 9; c++)
	{
		int rt = check_num(c,matrix);
		if(rt) goto Second;
		else if(rt == -1) return -1;
	}

	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
		{
			//if(matrix[i][j]==0)print_state(i,j, posible_state);
			if(posible_state[i][j].size() == 2)
			{
				int bak_matrix[9][9];
				int bak_order = order;
				copy_matrix(matrix, bak_matrix);
				set<int>::iterator it = posible_state[i][j].begin();
				cout<<" =========== asume "<<order++<<": ("<<i+1<<","<<j+1<<") "<<*it<<" ================="<<endl;
				matrix[i][j] = *it;
				if(sudoku(matrix) == -1)
				{
					cout<<"========== asume error ============="<<endl;
					copy_matrix(bak_matrix, matrix);
					order = bak_order;
					it++;
					cout<<order++<<": ("<<i+1<<","<<j+1<<") "<<*it<<endl;
					matrix[i][j] = *it;
					return sudoku(matrix);
				}
				else return true;
			}
		}

	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(posible_state[i][j].size() == 3)
			{
				int bak_matrix[9][9];
				int bak_order = order;
				copy_matrix(matrix, bak_matrix);
				set<int>::iterator it = posible_state[i][j].begin();
				cout<<" =========== asume "<<order++<<": ("<<i+1<<","<<j+1<<") "<<*it<<" ================="<<endl;
				matrix[i][j] = *it;
				if(sudoku(matrix) == -1)
				{
					cout<<"========== asume error ============="<<endl;
					copy_matrix(bak_matrix, matrix);
					order = bak_order;
					it++;
					cout<<" =========== asume "<<order++<<": ("<<i+1<<","<<j+1<<") "<<*it<<endl;
					matrix[i][j] = *it;
					if(sudoku(matrix) == -1)
					{
					cout<<"========== asume error ============="<<endl;
					copy_matrix(bak_matrix, matrix);
					order = bak_order;
					it++;
					cout<<" =========== asume "<<order++<<": ("<<i+1<<","<<j+1<<") "<<*it<<endl;
					matrix[i][j] = *it;
					return sudoku(matrix);
					}
				}
				else return true;
			}		
		}
	}

	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(matrix[i][j] == 0 ) return false;
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	int matrix[9][9];
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)cin >> matrix[i][j];
	print_matrix(matrix);
	if(sudoku(matrix)==true) 
	{
		cout<<"================== Successful! ======================"<<endl;
		print_matrix(matrix);
	}
	return 0;
}
