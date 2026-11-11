#pragma once
#include <conio.h>
#include <windows.h>
#include <functional>
#include <List.h>
#include <header.h>
#include <Room.h>
#include <Tenant.h>
#include <initializer_list>
#include <Checker.h>
#include <type_traits>
using std::cout;
using std::endl;
using std::function;
using std::initializer_list;
using std::string;
using std::to_string;

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

    static void Goto(SHORT x, SHORT y) {
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
        Console::SetColor(Console::BLACK, Console::BLACK);
        for (int y = y1; y < y1 + height; ++y) {
            Goto(x, y);
            std::cout << spaces;
        }
    }

    static bool AllowedChar(const unsigned char& c) {
        return c != '*' && c != '#';
    }
};



class Box
{
protected:
    int x, y, w, h;
    string title;
    Console::Color borderColor = Console::WHITE;
    bool borderType = 0;
    bool isUsed = false;
public:
    Box(int x, int y, int w, int h, const string& title = "")
        : x(x), y(y), w(w), h(h), title(title) {}
    virtual ~Box() = default;

    int GetX() const {
        return this->x;
    }

    int GetY() const {
        return this->y;
    }

    int GetH() const {
        return this->h;
    }

    int GetW() const {
        return this->w;
    }

    void Clear() const {
        Console::ClearSegment(this->x, this->y, this->h, this->w);
    }

    void ResetPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void SetBorderColor(Console::Color color) {
        this->borderColor = color;
    }

    void Use() {
        this->SetBorderColor(Console::LIGHT_BLUE);
        this->isUsed = true;
        this->DrawBorder();
    }

    void Quit() {
        this->SetBorderColor(Console::WHITE);
        this->isUsed = false;
        this->DrawBorder();
    }

    void DrawBorder() const;
    virtual void Draw() = 0;
};



class ButtonBox : public Box
{
private:
    string label;
public:
    ButtonBox(int x, int y, int w, const string& lab)
        : Box(x, y, w, 3), label(lab) {
            this->borderType = 0;
        }

    void Draw() override;
    bool operator()(Console::Key& key);
};



class NavigationBox : public Box
{
    int selection;
    int itemW;
    List<string> items;
    
    int GetItemX(int index) const {
        return this->x + 1 + index * (this->itemW + 1);
    }

    void DrawBorder() const;
public:
    NavigationBox(int width, int x = 1, int y = 1)
        : Box(x, y, 1, 3), selection(0), itemW(width) {
    }
    ~NavigationBox() = default;

    void AddItem(const string& item) {
        this->items.push_back(item);
        this->w += this->itemW + 1;
    }

    void Draw() override;
    int operator()(Console::Key&);
};



class MenuBox : public Box
{
protected:
    List<string> options;
    int selection;
    
    void UpdateStatus(int index) const {
        if (index < 0 || index >= int(this->options.size())) return;

        Console::SetCursorVisible(false);
        Console::ClearSegment(this->x + 1, this->y + 3 + index, 1, this->w - 2);
        if (index == this->selection) Console::SetColor(Console::BLACK, Console::WHITE);
        else Console::SetColor(Console::WHITE, Console::BLACK);
        Console::Goto(this->x + 1, this->y + 3 + index);
        cout << string(this->w - 2, ' ');
        Console::Goto(this->x + 2, this->y + 3 + index);
        cout << this->options[index];

        Console::SetColor(Console::WHITE);
    }

public:
    MenuBox(int x, int y, int w, const string& title = "")
        : Box(x, y, w, 4, title) 
    {
        this->selection = 0;
        this->borderType = 1;
    }
    ~MenuBox() = default;

    int GetSize() const {
        return int(this->options.size());
    }

    void AddOption(const string& op) {
        options.push_back(op);
        this->h++;
    }

    int operator()(Console::Key& key);
    void Draw() override;
};

class NumberMenuBox : public MenuBox
{
public:
    NumberMenuBox(int x, int y, int w, const string& title = "")
        : MenuBox(x, y, w, title)
    {
        this->selection = 0;
        this->borderType = 0;
    }
    ~NumberMenuBox() = default;

    int operator()(Console::Key& key);
};



class FormBox : public Box
{
protected:
    struct Label {
        string field;
        string prefix;
        string postfix;
    };

    float valueRatio;
    List<Label> labels;
    List<string> values;
    int curr;
    bool confirmed = false;
    bool choiceBlinkOn = true;

    int ColonX() const {
        return this->x + int((this->w - 2) * (1 - this->valueRatio));
    }

    int ValueStartX(int idx) const {
        return this->ColonX() + 2 + int(this->labels[idx].prefix.size());
    }

    int ValueWidth(int idx) const {
        int width = this->x + this->w - 2 - this->ValueStartX(idx) - int(this->labels[idx].postfix.size());
        return width > 0 ? width : 0;
    }
    
    void UpdateStatus(int index) const;

public:
    FormBox(int x, int y, int w, float r, const string& title = "")
        : Box(x, y, w, 5, title), valueRatio(r)
    {
        this->curr = 0;
        this->borderType = 0;
    }
    ~FormBox() = default;

    void AddField(const string& lab, const string& val = "", const string& prefix = "", const string& postfix = "") {
        this->labels.push_back({lab, prefix, postfix});
        this->values.push_back(val);
        this->h ++;
    }

    string GetValue(int index) const {
        if (index < 0 || index >= int(this->values.size())) throw runtime_error("Form: Khong co gia tri");
        return this->values[index];
    }

    void AddConstField(const string& lab, const string& constValue = "") {
        this->labels.push_back({lab, constValue, ""});
        this->values.push_back("*");
        this->h ++;
    }

    void AddChosenField(const string& lab, initializer_list<string> choice, int sel = 0) {
        string val = "#" + to_string(choice.size()) + to_string(sel);
        for (const string& s : choice) {
            val += "|" + s;
        }

        this->labels.push_back({lab, "", ""});
        this->values.push_back(val);
        this->h ++;
    }
    
    int GetChosenValue(int index) const {
        if (index < 0 || index >= int(this->values.size())) return -1;
        if (this->values[index].empty() || this->values[index][0] != '#') return -1;
        return this->values[index][2] - '0';
    }

    int GetSize() const {
        return this->labels.size();
    }

    void Draw() override;
    bool operator()(Console::Key& key);
};



class SingleLineBox : public Box
{
private:
    string label;
    string& value;

    int ValueWidth() const {
        int width = this->w - int(this->label.size()) - 5;
        return width > 0 ? width : 0;
    }

public:
    SingleLineBox(int x, int y, int w, const string& lab, string& val)
        : Box(x, y, w, 3), label(lab), value(val) {
            this->borderType = 0;
        }
    ~SingleLineBox() = default;

    void Draw() override;
    void operator()(Console::Key& key);
};


class WarningBox : public Box
{
    string labelY, labelN;
    List<string> line;
    bool choice = 1;

    int WrapedText(int start, const string& question) {
        if (start >= int(question.size())) return 0;
        int width = max(1, this->w - 2);
        int remaining = int(question.size()) - start;
        if (remaining <= width) return remaining;

        int breakAt = int(question.rfind(' ', start + width - 1));
        if (breakAt >= start) return breakAt - start;
        return width;
    }
public:
    WarningBox(int x, int y, int w, string title, string warning, string confirm = "YES", string reject = "NO")
        : Box(x, y, w, 5, title), labelY(confirm), labelN(reject) {
            this->borderType = 1;
            int curr = 0;
            while (curr < int(warning.size())) {
                while (curr < int(warning.size()) && warning[curr] == ' ') curr++;
                if (curr >= int(warning.size())) break;
                int len = this->WrapedText(curr, warning);
                this->line.push_back(warning.substr(curr, len));
                curr += len;
                this->h ++;
            }
        }
    ~WarningBox() = default;

    void Draw() override;
    bool operator()();
};


template<typename T>
class TableBox : public Box
{
private:
    const List<T>& data;
    function<void(int, int)> tableInit;
    function<void(int, int)> tableEnd;
    function<void(const T&)> tableShow;
    int selection = -1;
    
    void UpdateStatus(int index) const {
        if (index < 0 || index >= int(this->data.size())) return;
        Console::ClearSegment(this->x, this->y + 3 + index, 1, this->w);
        Console::Goto(this->x, this->y + 3 + index);
        if (index == this->selection && this->isUsed) Console::SetColor(Console::BLACK, Console::WHITE);
        else Console::SetColor(Console::WHITE, Console::BLACK);
        
        this->tableShow(data[index]);

        Console::SetColor(Console::WHITE);
    }

public:
    TableBox(int x, int y, int w, const List<T>& d, function<void(int, int)> init, 
    function<void(int, int)> close, function<void(const T&)> show)
        : Box(x, y, w, 4), data(d), tableInit(init), tableEnd(close), tableShow(show) {}
    ~TableBox() = default;

    void Draw() override;
    int operator()(Console::Key& key);
};

template<typename T>
inline void TableBox<T>::Draw()
{
    int newH = int(this->data.size()) + 4;
    Console::ClearSegment(this->x, this->y, this->h, this->w);
    this->h = newH;
    Console::SetColor(Console::WHITE);
    this->tableInit(this->x, this->y);
    for (int i = 0; i < int(this->data.size()); i++) {
        Console::Goto(this->x, this->y + 3 + i);
        
       this->tableShow(data[i]);
        
    }
    this->tableEnd(this->x, this->y + 3 + int(this->data.size()));
}

template<typename T>
int TableBox<T>::operator()(Console::Key& key)
{
    if (this->data.empty()) {
        Console::SetColor(Console::WHITE);
        return -1;
    }

    this->isUsed = true;
    Console::SetCursorVisible(false);
    this->selection = 0;
    this->UpdateStatus(this->selection);
    while (this->isUsed) {
        char c;
        key = Console::Read(c);

        if (key == Console::UP) {
            int previousSelection = this->selection;
            if (this->selection <= 0) this->selection = int(this->data.size()) - 1;
            else this->selection--;
            this->UpdateStatus(previousSelection);
            this->UpdateStatus(this->selection);
        }
        else if (key == Console::DOWN) {
            int previousSelection = this->selection;
            if (this->selection >= int(this->data.size()) - 1) this->selection = 0;
            else this->selection++;
            this->UpdateStatus(previousSelection);
            this->UpdateStatus(this->selection);
        }
        else if (key == Console::ENTER) {
            this->isUsed = false;
            Console::SetColor(Console::WHITE);
            this->UpdateStatus(this->selection);
            return this->selection;
        }
        else if (key == Console::ESC || key == Console::TAB) {
            this->isUsed = false;
        }
    }

    this->UpdateStatus(this->selection);
    Console::SetColor(Console::WHITE);
    return -1;
}

template<typename T>
class InputHandler
{
    T remaining;
    List<List<T>> relations;
    List<List<bool>> hasRelation;

    static int GetCase(const Console::Key& key) {
        switch (key) {
            case Console::ENTER: return 0;
            case Console::TAB:   return 1;
            case Console::UP:    return 2;
            case Console::DOWN:  return 3;    
            case Console::LEFT:  return 4;
            case Console::RIGHT: return 5;
            default:             return -1;
        }
    }

public:
    InputHandler(T remaining, T maxval) 
        : remaining(remaining),
          relations(int(maxval) + 1, List<T>(6, remaining)),
          hasRelation(int(maxval) + 1, List<bool>(6, false))
    {
        if (int(maxval) < 0) throw invalid_argument("Gia tri enum toi da khong hop le!");
    }

    void AddRelation(const T& before, const Console::Key& key, const T& after) {
        int action = this->GetCase(key);
        if (action == -1) throw invalid_argument("Phim ko hop le, khong the tao ket noi!");
        int start = int(before);
        int end = int(after);
        if (start < 0 || start >= int(relations.size())
            || end < 0 || end >= int(relations.size()))
            throw invalid_argument("Gia tri ket noi phai nam trong khoang (0, enum::max)!");

        relations[start][action] = after;
        hasRelation[start][action] = true;
    }

    T GetDest(const T& before, const Console::Key& key) const {
        int action = this->GetCase(key);
        int start = int(before);
        if (start < 0 || start >= int(relations.size())) return this->remaining;
        if (action == -1 || !hasRelation[start][action]) return before;
        return relations[start][action];
    }
};