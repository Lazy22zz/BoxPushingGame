#include<iostream>
using namespace std;

const char gMapData[] = "\
########\n\
# __ p #\n\
# o    #\n\
#  o   #\n\
#      #\n\
########";
const int gMapWidth = 8;
const int gMapHeight = 6;

enum object {
	Space,
	Wall,
	Block,
	Block_On_Goal,
	Person,
	Person_On_Goal,
	Destination,

	Unknown
};

void initalize(object* map_instance, int w, int h, const char* gMapData);
void draw(const object* map_instance, int w, int h);
void update(object* map_instance, int w, int h, const char input);
bool checkgameclear(const object* map_instance, int w, int h );

int main()
{
	object* map_instance = new object[gMapHeight * gMapWidth];

	initalize(map_instance, gMapWidth, gMapHeight, gMapData);

	// main function cycle
	while (true)
	{
		draw(map_instance, gMapWidth, gMapHeight);

		if (checkgameclear(map_instance, gMapWidth, gMapHeight))
		{
			break;
		}

		cout << "a:left, s:down, d:right, w:up. Command?" << endl;

		char input;
		cin >> input;

		update(map_instance, gMapWidth, gMapHeight, input);
	}

	cout << "Congratulaion, You win !" << endl;

	delete[] map_instance;
	map_instance = 0;

	return 0;
}

void initalize(object* map_instance, int w, int h, const char* gMapData)
{
	const char* data = gMapData;
	int x = 0;
	int y = 0;

	while (*data != '\0')
	{
		object o;
		switch (*data)
		{
		case '#': o = Wall; break;
		case ' ': o = Space; break;
		case 'o': o = Block; break;
		case 'O': o = Block_On_Goal; break;
		case 'P': o = Person_On_Goal; break;
		case 'p': o = Person; break;
		case '_': o = Destination; break;
		case'\n': 
			x = 0;
			++y;
			o = Unknown;
			break;
		default: o = Unknown; break;
		}
		++data;

		if (o != Unknown) {
			map_instance[y * w + x] = o;
			++x;
		}
	}
}

void draw(const object* map_instance, int w, int h)
{
	/*
	Space,
	Wall,
	Block,
	Block_On_Goal,
	Person,
	Person_On_Goal,
	Destination,
	*/
	const char instance[] = { ' ', '#', 'o', 'O', 'p', 'P', '_' };
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			object o = map_instance[y * w + x];
			cout << instance[o];
		}
		cout << endl;
	}
}

void update(object* map_instance, int w, int h, const char input)
{
	int dx = 0;
	int dy = 0;
	switch (input)
	{
	case 'a': dx = -1; break;
	case 'd': dx = 1; break;
	case 'w': dy = -1; break;
	case 's': dy = 1; break;
	}

	int i = -1;
	for (i = 0; i < w * h; ++i)
	{
		if (map_instance[i] == Person|| map_instance[i] == Person_On_Goal)
		{
			break;
		}
	}

	int x = i % w;
	int y = i / w;

	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}

	int player_position = y * w + x;
	int target_position = ty * w + tx;
	if (map_instance[target_position] == Space || map_instance[target_position] == Destination)
	{
		map_instance[target_position] = (map_instance[target_position] == Destination) ? Person_On_Goal : Person;
		map_instance[player_position] = (map_instance[player_position] == Person_On_Goal) ? Destination : Space;
	}
	else if (map_instance[target_position] == Block || map_instance[target_position] == Block_On_Goal)
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}
		int target_position2 = (ty + dy) * w + (tx + dx);
		if (map_instance[target_position2] == Space || map_instance[target_position2] == Destination)
		{
			map_instance[target_position2] = (map_instance[target_position2] == Destination) ?
				Block_On_Goal : Block;
			map_instance[target_position] = (map_instance[target_position] == Block_On_Goal) ?
				Person_On_Goal : Person;
			map_instance[player_position] = (map_instance[player_position] == Person_On_Goal) ?
				Destination : Space;
		}

	}
}

bool checkgameclear(const object* map_instance, int w, int h)
{
	for (int i = 0; i < w * h; ++i)
	{
		if (map_instance[i] == Block)
		{
			return false;
		}
	}
	return true;
}
