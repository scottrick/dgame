#ifndef _INPUT_
#define _INPUT_

//define the KeyboardAlpha flags
#define		INPUT_CHAR_A									(1 << 1)
#define		INPUT_CHAR_B									(1 << 2)
#define		INPUT_CHAR_C									(1 << 3)
#define		INPUT_CHAR_D									(1 << 4)
#define		INPUT_CHAR_E									(1 << 5)
#define		INPUT_CHAR_F									(1 << 6)
#define		INPUT_CHAR_G									(1 << 7)
#define		INPUT_CHAR_H									(1 << 8)
#define		INPUT_CHAR_I									(1 << 9)
#define		INPUT_CHAR_J									(1 << 10)
#define		INPUT_CHAR_K									(1 << 11)
#define		INPUT_CHAR_L									(1 << 12)
#define		INPUT_CHAR_M									(1 << 13)
#define		INPUT_CHAR_N									(1 << 14)
#define		INPUT_CHAR_O									(1 << 15)
#define		INPUT_CHAR_P									(1 << 16)
#define		INPUT_CHAR_Q									(1 << 17)
#define		INPUT_CHAR_R									(1 << 18)
#define		INPUT_CHAR_S									(1 << 19)
#define		INPUT_CHAR_T									(1 << 20)
#define		INPUT_CHAR_U									(1 << 21)
#define		INPUT_CHAR_V									(1 << 22)
#define		INPUT_CHAR_W									(1 << 23)
#define		INPUT_CHAR_X									(1 << 24)
#define		INPUT_CHAR_Y									(1 << 25)
#define		INPUT_CHAR_Z									(1 << 26)
#define		INPUT_SPACE										(1 << 27)

class Input
{
public:
	Input();
	~Input();

	int							GetKeyboardAlphaFlags()			{ return m_dwAlphaFlags; }

	void						InputDown(unsigned char key);
	void						InputUp(unsigned char key);

protected:
	void						RaiseAlphaFlags(int dwFlags);
	void						LowerAlphaFlags(int dwFlags);

private:
	void						Init();

	int							m_dwAlphaFlags;

};

extern Input *gInput();

#endif