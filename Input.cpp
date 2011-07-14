#include "Input.h"

Input *gInput()
{
	static Input theInput;
	return &theInput;
}

Input::Input()
{
	Init();
}

Input::~Input()
{

}

void Input::Init()
{
	m_dwAlphaFlags		= 0;
}

void Input::InputDown(unsigned char key)
{
	switch (key)
	{
	case 'a':
	case 'A':
		RaiseAlphaFlags(INPUT_CHAR_A);
		break;
	case 'b':
	case 'B':
		RaiseAlphaFlags(INPUT_CHAR_B);
		break;
	case 'c':
	case 'C':
		RaiseAlphaFlags(INPUT_CHAR_C);
		break;
	case 'd':
	case 'D':
		RaiseAlphaFlags(INPUT_CHAR_D);
		break;
	case 'e':
	case 'E':
		RaiseAlphaFlags(INPUT_CHAR_E);
		break;
	case 'f':
	case 'F':
		RaiseAlphaFlags(INPUT_CHAR_F);
		break;
	case 'g':
	case 'G':
		RaiseAlphaFlags(INPUT_CHAR_G);
		break;
	case 'h':
	case 'H':
		RaiseAlphaFlags(INPUT_CHAR_H);
		break;
	case 'i':
	case 'I':
		RaiseAlphaFlags(INPUT_CHAR_I);
		break;
	case 'j':
	case 'J':
		RaiseAlphaFlags(INPUT_CHAR_J);
		break;
	case 'k':
	case 'K':
		RaiseAlphaFlags(INPUT_CHAR_K);
		break;
	case 'l':
	case 'L':
		RaiseAlphaFlags(INPUT_CHAR_L);
		break;
	case 'm':
	case 'M':
		RaiseAlphaFlags(INPUT_CHAR_M);
		break;
	case 'n':
	case 'N':
		RaiseAlphaFlags(INPUT_CHAR_N);
		break;
	case 'o':
	case 'O':
		RaiseAlphaFlags(INPUT_CHAR_O);
		break;
	case 'p':
	case 'P':
		RaiseAlphaFlags(INPUT_CHAR_P);
		break;
	case 'q':
	case 'Q':
		RaiseAlphaFlags(INPUT_CHAR_Q);
		break;
	case 'r':
	case 'R':
		RaiseAlphaFlags(INPUT_CHAR_R);
		break;
	case 'S':
	case 's':
		RaiseAlphaFlags(INPUT_CHAR_S);
		break;
	case 't':
	case 'T':
		RaiseAlphaFlags(INPUT_CHAR_T);
		break;
	case 'u':
	case 'U':
		RaiseAlphaFlags(INPUT_CHAR_U);
		break;
	case 'v':
	case 'V':
		RaiseAlphaFlags(INPUT_CHAR_V);
		break;
	case 'w':
	case 'W':
		RaiseAlphaFlags(INPUT_CHAR_W);
		break;
	case 'x':
	case 'X':
		RaiseAlphaFlags(INPUT_CHAR_X);
		break;
	case 'y':
	case 'Y':
		RaiseAlphaFlags(INPUT_CHAR_Y);
		break;
	case 'z':
	case 'Z':
		RaiseAlphaFlags(INPUT_CHAR_Z);
		break;
	case ' ':
		RaiseAlphaFlags(INPUT_SPACE);
		break;

	default:
		break;
	}
}

void Input::InputUp(unsigned char key)
{
	switch (key)
	{
	case 'a':
	case 'A':
		LowerAlphaFlags(INPUT_CHAR_A);
		break;
	case 'b':
	case 'B':
		LowerAlphaFlags(INPUT_CHAR_B);
		break;
	case 'c':
	case 'C':
		LowerAlphaFlags(INPUT_CHAR_C);
		break;
	case 'd':
	case 'D':
		LowerAlphaFlags(INPUT_CHAR_D);
		break;
	case 'e':
	case 'E':
		LowerAlphaFlags(INPUT_CHAR_E);
		break;
	case 'f':
	case 'F':
		LowerAlphaFlags(INPUT_CHAR_F);
		break;
	case 'g':
	case 'G':
		LowerAlphaFlags(INPUT_CHAR_G);
		break;
	case 'h':
	case 'H':
		LowerAlphaFlags(INPUT_CHAR_H);
		break;
	case 'i':
	case 'I':
		LowerAlphaFlags(INPUT_CHAR_I);
		break;
	case 'j':
	case 'J':
		LowerAlphaFlags(INPUT_CHAR_J);
		break;
	case 'k':
	case 'K':
		LowerAlphaFlags(INPUT_CHAR_K);
		break;
	case 'l':
	case 'L':
		LowerAlphaFlags(INPUT_CHAR_L);
		break;
	case 'm':
	case 'M':
		LowerAlphaFlags(INPUT_CHAR_M);
		break;
	case 'n':
	case 'N':
		LowerAlphaFlags(INPUT_CHAR_N);
		break;
	case 'o':
	case 'O':
		LowerAlphaFlags(INPUT_CHAR_O);
		break;
	case 'p':
	case 'P':
		LowerAlphaFlags(INPUT_CHAR_P);
		break;
	case 'q':
	case 'Q':
		LowerAlphaFlags(INPUT_CHAR_Q);
		break;
	case 'r':
	case 'R':
		LowerAlphaFlags(INPUT_CHAR_R);
		break;
	case 'S':
	case 's':
		LowerAlphaFlags(INPUT_CHAR_S);
		break;
	case 't':
	case 'T':
		LowerAlphaFlags(INPUT_CHAR_T);
		break;
	case 'u':
	case 'U':
		LowerAlphaFlags(INPUT_CHAR_U);
		break;
	case 'v':
	case 'V':
		LowerAlphaFlags(INPUT_CHAR_V);
		break;
	case 'w':
	case 'W':
		LowerAlphaFlags(INPUT_CHAR_W);
		break;
	case 'x':
	case 'X':
		LowerAlphaFlags(INPUT_CHAR_X);
		break;
	case 'y':
	case 'Y':
		LowerAlphaFlags(INPUT_CHAR_Y);
		break;
	case 'z':
	case 'Z':
		LowerAlphaFlags(INPUT_CHAR_Z);
		break;
	case ' ':
		LowerAlphaFlags(INPUT_SPACE);
		break;

	default:
		break;
	}	
}

void Input::LowerAlphaFlags(int dwFlags)
{
	dwFlags				= ~dwFlags;
	m_dwAlphaFlags		&= dwFlags;
}

void Input::RaiseAlphaFlags(int dwFlags)
{
	m_dwAlphaFlags		|= dwFlags;
}