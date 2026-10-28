#pragma once
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>
#include <cstdlib>
#include <header.h>
using namespace std;

class Console {
private:
    static HANDLE Handle() {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }
public:
    enum Color {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        BROWN = 6,
        WHITE = 7,
        GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        YELLOW = 14,
        BRIGHT_WHITE = 15
    };

    enum Key {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        ENTER,
        ESC,
        BACKSPACE,
        DELETE_KEY,
        TAB,
        OTHER
    };

    static Key Read(char& pressedChar) {
        int ch = _getch();
        pressedChar = '\0';

        if (ch == 0 || ch == 224) {
            int extendedCode = _getch();
            switch (extendedCode) {
                case 72: return UP;
                case 80: return DOWN;
                case 75: return LEFT;
                case 77: return RIGHT;
                case 83: return DELETE_KEY;
                default: return OTHER;
            }
        }

        switch (ch) {
            case 13: return ENTER;
            case 27: return ESC;
            case 8:  return BACKSPACE;
            case 9:  return TAB;
            default:
                if (ch >= 32 && ch <= 126) {
                    pressedChar = static_cast<char>(ch);
                }
                return OTHER;
        }
    }

    static void Goto(int x, int y) {
        COORD coord = { x, y };
        SetConsoleCursorPosition(Console::Handle(), coord);
    }

    static void SetColor(Color textColor, Color backgroundColor) {
        WORD colorAtt = (backgroundColor << 4) | textColor;
        SetConsoleTextAttribute(Console::Handle(), colorAtt);
    }

    static void SetColor(Color textColor) {
        SetConsoleTextAttribute(Console::Handle(), static_cast<WORD>(textColor));
    }

    static void SetCursorVisible(bool visible) {
        HANDLE handle = Console::Handle();
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 20;
        info.bVisible = visible ? TRUE : FALSE;
        SetConsoleCursorInfo(handle, &info);
    }

    static void ClearSegment(int x, int y1, int height, int width = 30) {
        std::string spaces(width, ' ');
        for (int y = y1; y < y1 + height; ++y) {
            Goto(x, y);
            std::cout << spaces;
        }
    }

    static void ClearScreen() {
        system("cls");
    }
};

class Box
{
protected:
    int x, y, w, h;
    string title;
public:
    Box(int x, int y, int w, int h, const string& title = "")
        : x(x), y(y), w(w), h(h), title(title) {}
    virtual ~Box() = default;

    void DrawBorder(bool type) const {
        Console::SetColor(Console::WHITE);
        if (type){
            Console::Goto(this->x, this->y);
            cout << border::TL2 << string(this->w - 2, border::H2) << border::TR2;
            for (int i = 1; i < this->h-1; i++){
                Console::Goto(this->x, this->y + i);
                cout << border::V2;
                Console::Goto(this->x + this->w - 1, this->y + i);
                cout << border::V2;
            }
            Console::Goto(this->x, this->y + this->h - 1);
            cout << border::BL2 << string(this->w - 2, border::H2) << border::BR2;
        }
        else{
            Console::Goto(this->x, this->y);
            cout << border::TL << string(this->w - 2, border::H) << border::TR;
            for (int i = 1; i < this->h-1; i++){
                Console::Goto(this->x, this->y + i);
                cout << border::V;
                Console::Goto(this->x + this->w - 1, this->y + i);
                cout << border::V;
            }
            Console::Goto(this->x, this->y + this->h - 1);
            cout << border::BL << string(this->w - 2, border::H) << border::BR;
        }
    }

    virtual void Draw() const = 0;
};

inline string center(const string& text, int width) {
    int len = text.size();
    if (len >= width) return text;
    int pad = (width - len) / 2;
    return string(pad, ' ') + text;
}

class DataBox : public Box
{
private:
    function<void()> render;
public:
    DataBox(int y, const string& title = "", function<void()> func = nullptr)
        : Box(0, y, 100, 100, title), render(func) {}
    ~DataBox() = default;

    void Draw() const override {
        Console::SetCursorVisible(false);
        Console::Goto(this->x, this->y);
        Console::SetColor(Console::YELLOW);
        if (title != "") cout << center(this->title, this->w);

        Console::Goto(this->x + 2, this->y);
        Console::SetColor(Console::WHITE);
        if (render) render();
    }
};

class MenuBox : public Box
{
private:
    vector<string> options;
    int select;
    bool isUsed = false;
public:
    MenuBox(int x, int y, int w, const string& title = "")
        : Box(x, y, w, 4, title) 
    {
        this->select = 0;
    }
    ~MenuBox() = default;

    int GetSize() const {
        return static_cast<int>(this->options.size());
    }

    void AddOption(const string& op) {
        options.push_back(op);
        this->h++;
    }

    int Exec() {
        if (this->options.empty()) return -1;

        Console::ClearScreen();
        this->isUsed = true;

        while (this->isUsed) {
            this->Draw();

            char c;
            Console::Key k = Console::Read(c);
            if (k == Console::UP){
                if (this->select <= 0) this->select = this->GetSize() - 1;
                else this->select--;
            }
            else if (k == Console::DOWN){
                if (this->select >= this->GetSize() - 1) this->select = 0;
                else this->select++;
            }
            else if (k == Console::ENTER){
                this->Quit();
                Console::SetColor(Console::WHITE);
                Console::ClearScreen();
                return this->select;
            }
            else if (k == Console::ESC){
                this->Quit();
                Console::SetColor(Console::WHITE);
                Console::ClearScreen();
                return -1;
            }
        }

        Console::SetColor(Console::WHITE);
        return -1;
    }
    void Quit() {
        this->isUsed = false;
    }

    void Draw() const override {
        Console::SetCursorVisible(false);
        Box::DrawBorder(1);
        Console::Goto(this->x + 1, this->y + 1);
        Console::SetColor(Console::YELLOW);
        cout << center(title, this->w - 2);

        Console::Goto(this->x, this->y + 2);
        Console::SetColor(Console::WHITE);
        cout << border::RT2 << string(this->w - 2, border::H2) << border::LT2;

        int contentWidth = this->w > 3 ? this->w - 3 : 0;
        for (int i = 0; i < (this->GetSize()); i++){
            Console::Goto(this->x + 1, this->y + 3 + i);
            if (i == this->select) Console::SetColor(Console::BLACK, Console::WHITE);
            else Console::SetColor(Console::WHITE, Console::BLACK);
            cout << string(this->w > 2 ? this->w - 2 : 0, ' ');

            Console::Goto(this->x + 2, this->y + 3 + i);
            cout << this->options[i].substr(0, contentWidth);
        }
        Console::SetColor(Console::WHITE);
    }
};

struct Field
{
    string label;
    string value;
};

class FormBox : public Box
{
private:
    float valueRatio;
    vector<Field> fields;
    int curr;
    bool isUsed = false;
    bool confirmed = false;

    int ValueStartX() const {
        return this->x + static_cast<int>((this->w - 2) * (1 - this->valueRatio)) + 2;
    }

    int ValueWidth() const {
        int width = this->x + this->w - 1 - this->ValueStartX();
        return width > 0 ? width : 0;
    }
public:
    FormBox(int x, int y, int w, float r, const string& title = "")
        : Box(x, y, w, 5, title), valueRatio(r)
    {
        this->curr = 0;
    }
    FormBox(int x, int y, int w, const string& title = "")
        : FormBox(x, y, w, 0.5f, title) {}
    ~FormBox() = default;

    void AddField(const string& lab, const string& val) {
        this->fields.push_back(Field{lab, val});
        this->h ++;
    }

    int GetSize() const {
        return this->fields.size();
    }

    void Draw() const override {
        Console::SetCursorVisible(false);
        Box::DrawBorder(0);
        Console::Goto(this->x + 1, this->y + 1);
        Console::SetColor(Console::YELLOW);
        cout << center(title, this->w - 2);

        Console::Goto(this->x, this->y + 2);
        Console::SetColor(Console::WHITE);
        cout << border::RT << string(this->w - 2, border::H) << border::LT;

        for (int i = 0; i < this->GetSize(); i++){
            Console::Goto(this->x + 1, this->y + 3 + i);
            Console::SetColor(Console::CYAN);
            cout << this->fields[i].label;

            Console::Goto(this->ValueStartX() - 2, this->y + 3 + i);
            Console::SetColor(Console::CYAN);
            cout << ": " << string(this->ValueWidth(), ' ');
            Console::Goto(this->ValueStartX(), this->y + 3 + i);
            cout << this->fields[i].value.substr(0, this->ValueWidth());
        }
        Console::Goto(this->x + 1, this->y + h - 2);
        if (this->curr == this->GetSize()) Console::SetColor(Console::BLACK, Console::WHITE);
        else Console::SetColor(Console::WHITE, Console::BLACK);
        cout << "[ XAC NHAN LUU THONG TIN ]";
        Console::SetColor(Console::WHITE);
    }

    void Quit() {
        this->isUsed = false;
    }

    const vector<Field>& GetFields() const {
        return this->fields;
    }

    bool Exec() {
        Console::ClearScreen();
        this->isUsed = true;
        this->confirmed = false;
        Console::SetCursorVisible(true);

        while (this->isUsed) {
            this->Draw();
            Console::SetCursorVisible(true);

            if (this->curr < this->GetSize()){
                int cursorX = this->ValueStartX()
                    + min(this->fields[this->curr].value.size(),
                        static_cast<size_t>(this->ValueWidth()));
                if (this->ValueWidth() > 0 && cursorX >= this->x + this->w - 1)
                    cursorX = this->x + this->w - 2;
                Console::Goto(cursorX, this->y + 3 + this->curr);
            }

            char c;
            Console::Key k = Console::Read(c);
            if (k == Console::UP){
                if (this->curr <= 0) this->curr = this->GetSize();
                else this->curr--;
            }
            else if (k == Console::DOWN){
                if (this->curr >= this->GetSize()) this->curr = 0;
                else this->curr++;
            }
            else if (k == Console::ENTER){
                if (this->curr == this->GetSize()){
                    this->confirmed = true;
                    this->Quit();
                }
                else if (this->curr < this->GetSize()) this->curr++;
            }
            else if (k == Console::ESC) this->Quit();
            else if (k == Console::BACKSPACE && this->curr < this->fields.size()){
                if (!this->fields[this->curr].value.empty())
                    this->fields[this->curr].value.pop_back();
            }
            else if (c != '\0' && this->curr < this->fields.size()
                && this->fields[this->curr].value.size()
                    < static_cast<size_t>(this->ValueWidth())){
                this->fields[this->curr].value += c;
            }
        }

        Console::SetCursorVisible(false);
        Console::SetColor(Console::WHITE);
        return this->confirmed;
    }
};
