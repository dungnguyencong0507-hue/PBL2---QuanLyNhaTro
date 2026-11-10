#include <ConsoleUI.h>
using namespace std;

inline string center(const string& text, int width) {
    int len = text.size();
    if (len >= width) return text;
    int pad = (width - len) / 2;
    return string(pad, ' ') + text + string(width - len - pad, ' ');
}

void Box::DrawBorder() const 
{
    Console::SetColor(this->borderColor);
    if (this->borderType){
        Console::Goto(this->x, this->y);
        cout << BORDER_TL2 << Repeat(this->w - 2, BORDER_H2) << BORDER_TR2;
        for (int i = 1; i < this->h-1; i++){
            Console::Goto(this->x, this->y + i);
            cout << BORDER_V2;
            Console::Goto(this->x + this->w - 1, this->y + i);
            cout << BORDER_V2;
        }
        Console::Goto(this->x, this->y + this->h - 1);
        cout << BORDER_BL2 << Repeat(this->w - 2, BORDER_H2) << BORDER_BR2;

        if (!this->title.empty() && this->h > 3) {
            Console::Goto(this->x, this->y + 2);
            cout << BORDER_RT2 << Repeat(this->w - 2, BORDER_H2) << BORDER_LT2;
        }
    }
    else{
        Console::Goto(this->x, this->y);
        cout << BORDER_TL << Repeat(this->w - 2, BORDER_H) << BORDER_TR;
        for (int i = 1; i < this->h-1; i++){
            Console::Goto(this->x, this->y + i);
            cout << BORDER_V;
            Console::Goto(this->x + this->w - 1, this->y + i);
            cout << BORDER_V;
        }
        Console::Goto(this->x, this->y + this->h - 1);
        cout << BORDER_BL << Repeat(this->w - 2, BORDER_H) << BORDER_BR;

        if (!this->title.empty() && this->h > 3) {
            Console::Goto(this->x, this->y + 2);
            cout << BORDER_RT << Repeat(this->w - 2, BORDER_H) << BORDER_LT;
        }
    }
}

void ButtonBox::Draw() 
{
    this->Clear();
    Console::SetCursorVisible(false);
    Console::Goto(this->x, this->y);
    this->DrawBorder();

    Console::Goto(this->x + 1, this->y + 1);
    Console::SetColor(Console::YELLOW);
    cout << center(this->label, this->w - 2);
    Console::SetColor(Console::WHITE);
}

bool ButtonBox::operator()(Console::Key& key) 
{
    this->Use();
    this->DrawBorder();

    while (this->isUsed) {
        char c;
        key = Console::Read(c);
        if (key == Console::ENTER) {
            this->Quit();
            Console::SetColor(Console::WHITE);
            return true;
        }
        else if (key == Console::ESC || key == Console::TAB || key == Console::UP 
            || key == Console::DOWN || key == Console::LEFT || key == Console::RIGHT) {
            this->Quit();
            Console::SetColor(Console::WHITE);
            return false;
        }
    }
    return false;
}

void NavigationBox::DrawBorder() const
{
    Console::Goto(this->x, this->y + 1);
    Console::SetColor(Console::GRAY);
    cout << BORDER_V;

    for (int i = 0; i < int(this->items.size()); i++) {
        if (i == this->selection) {
            Console::SetColor(Console::WHITE);
            Console::Goto(this->GetItemX(i) - 1, this->y + 1);
            cout << BORDER_V;

            Console::Goto(this->GetItemX(i + 1) - 1, this->y + 1);
            cout << BORDER_V;

            Console::Goto(this->GetItemX(i) - 1, this->y);
            cout << BORDER_TL << Repeat(this->itemW, BORDER_H) << BORDER_TR;

            Console::Goto(this->GetItemX(i) - 1, this->y + 2);
            cout << BORDER_BR;

            Console::Goto(this->GetItemX(i + 1) - 1, this->y + 2);
            cout << BORDER_BL;
        }
        else {
            Console::SetColor(Console::GRAY);
            Console::Goto(this->GetItemX(i + 1) - 1, this->y + 1);
            cout << BORDER_V;
        }
    }

    Console::SetColor(Console::WHITE);
    if (this->selection != 0) {
        Console::Goto(this->x, this->y + 2);
        cout << Repeat(this->GetItemX(this->selection) - 1 - this->x, BORDER_H);
    }
    if (this->selection != int(this->items.size() - 1)) {
        Console::Goto(this->GetItemX(this->selection + 1), this->y + 2);
        cout << Repeat(this->x + this->w - this->GetItemX(this->selection + 1), BORDER_H);
    }
}

void NavigationBox::Draw()
{
    this->Clear();
    this->DrawBorder();
    for (int i = 0; i < int(this->items.size()); i++) {
        if (i == this->selection) {
            if (this->isUsed) Console::SetColor(Console::BLACK, Console::WHITE);
            else Console::SetColor(Console::WHITE);
        }
        else Console::SetColor(Console::GRAY);

        Console::Goto(this->GetItemX(i), this->y + 1);
        cout << center(this->items[i], this->itemW);
    }
}

int NavigationBox::operator()(Console::Key& key)
{
    if (this->items.empty()) return -1;
    Console::SetCursorVisible(false);

    this->isUsed = true;
    this->Draw();
    while (this->isUsed) {
        char c;
        key = Console::Read(c);
        
        if (key == Console::RIGHT) {
            if (this->selection < int(this->items.size()) - 1) this->selection++;
            else this->selection = 0;
            Console::SetColor(Console::WHITE);
            return this->selection;
        }
        else if (key == Console::LEFT || key == Console::TAB) {
            if (this->selection > 0) this->selection--;
            else this->selection = this->items.size() - 1;
            Console::SetColor(Console::WHITE);
            return this->selection;
        }
        else if (key == Console::DOWN || key == Console::ENTER) {
            this->isUsed = false;
            Console::SetColor(Console::WHITE);
            this->Draw();
            return this->selection;
        }
        else if (key == Console::ESC) {
            this->isUsed = false;
            Console::SetColor(Console::WHITE);
        }
    }
    return -1;
}

int MenuBox::operator()(Console::Key& key) 
{
    if (this->options.empty()) return -1;
    this->isUsed = true;

    int prevSelect;

    while (this->isUsed) {
        this->UpdateStatus(this->selection);

        char c;
        key = Console::Read(c);
        if (key == Console::UP){
            prevSelect = this->selection;
            if (this->selection <= 0) this->selection = this->GetSize() - 1;
            else this->selection--;
            this->UpdateStatus(prevSelect);
        }
        else if (key == Console::DOWN){
            prevSelect = this->selection;
            if (this->selection >= this->GetSize() - 1) this->selection = 0;
            else this->selection++;
            this->UpdateStatus(prevSelect);
        }
        else if (key == Console::ENTER){
            this->isUsed = false;
            Console::SetColor(Console::WHITE);
            return this->selection;
        }
        else if (key == Console::ESC){
            this->isUsed = false;
        }
    }

    Console::SetColor(Console::WHITE);
    return -1;
}

void MenuBox::Draw()  
{
    this->Clear();
    Console::SetCursorVisible(false);
    Box::DrawBorder();
    Console::Goto(this->x + 1, this->y + 1);
    Console::SetColor(Console::YELLOW);
    cout << center(title, this->w - 2);

    int contentWidth = this->w > 3 ? this->w - 3 : 0;
    for (int i = 0; i < (this->GetSize()); i++){
        Console::Goto(this->x + 1, this->y + 3 + i);
        if (i == this->selection) Console::SetColor(Console::BLACK, Console::WHITE);
        else Console::SetColor(Console::WHITE, Console::BLACK);
        cout << string(this->w > 2 ? this->w - 2 : 0, ' ');

        Console::Goto(this->x + 2, this->y + 3 + i);
        cout << this->options[i].substr(0, contentWidth);
    }
    Console::SetColor(Console::WHITE);
}

int NumberMenuBox::operator()(Console::Key& key) 
{
    Console::SetCursorVisible(false);
    if (this->options.empty()) return -1;
    this->Use();

    int prevSelect;

    while (this->isUsed) {
        char c;
        key = Console::Read(c);
        if (key == Console::UP || key == Console::DOWN || key == Console::TAB 
            || key == Console::LEFT || key == Console::RIGHT || key == Console::ESC){
            this->Quit();
        }
        else if (key == Console::OTHER && c != '\0'){
            prevSelect = this->selection;
            int num = c - '0';
            if (num > 0 && num <= this->GetSize()){
                this->selection = num - 1;
                this->UpdateStatus(prevSelect);
                this->UpdateStatus(this->selection);
                return this->selection;
            }
        }
    }

    Console::SetColor(Console::WHITE);
    return -1;
}

void FormBox::UpdateStatus(int index) const
{
    if (index < 0 || index > this->GetSize()) return;

    if (index == this->GetSize()) {
        Console::Goto(this->x + 1, this->y + h - 2);
        if (this->curr == this->GetSize() && this->isUsed) Console::SetColor(Console::BLACK, Console::WHITE);
        else Console::SetColor(Console::WHITE, Console::BLACK);
        cout << "[ XAC NHAN LUU THONG TIN ]";
        return;
    }

    Console::SetCursorVisible(false);
    if (this->values[index].empty() || (this->values[index][0] != '#' && this->values[index][0] != '*')) 
        Console::ClearSegment(this->x + 1, this->y + 3 + index, 1, this->w - 2);

    Console::Goto(this->x + 1, this->y + 3 + index);
    if (this->values[index] != "*" && index == this->curr && this->isUsed) Console::SetColor(Console::CYAN);
    else Console::SetColor(Console::WHITE);
    cout << this->labels[index].field;
    Console::Goto(this->ColonX(), this->y + 3 + index);
    cout << ": ";
    Console::SetColor(Console::GRAY);
    cout << this->labels[index].prefix;
    if (!this->labels[index].postfix.empty()) {
        Console::Goto(this->x + this->w - 1 - this->labels[index].postfix.size(), this->y + 3 + index);
        cout << this->labels[index].postfix;
    }
    if (this->values[index] == "*") return;

    Console::Goto(this->ValueStartX(index), this->y + 3 + index);
    if (!this->values[index].empty() && this->values[index][0] == '#') {
        istringstream iss(this->values[index]);
        string button;
        const int selectedChoice = this->values[index][2] - '0';
        int count = this->values[index][1] - '0';
        getline(iss, button, '|');

        for (int choice = 0; choice < count; ++choice) {
            getline(iss, button, '|');
            
            if (choice == selectedChoice && (index != this->curr || this->choiceBlinkOn || !this->isUsed)) {
                Console::SetColor(Console::BLACK, Console::WHITE);
                cout << "[" << button << "]";
            }
            else {
                Console::SetColor(Console::YELLOW, Console::BLACK);
                cout << "[" << button << "]";
            }
            
            Console::SetColor(Console::YELLOW, Console::BLACK);
            cout << " ";
        }
    }
    else {
        Console::SetColor(Console::YELLOW);
        const int valueWidth = this->ValueWidth(index);
        const string& value = this->values[index];
        if (int(value.size()) <= valueWidth) cout << value;
        else if (valueWidth > 0) cout << value.substr(value.size() - valueWidth, valueWidth);
    }
}

void FormBox::Draw() 
{
    Console::SetCursorVisible(false);
    this->Clear();

    Box::DrawBorder();
    Console::Goto(this->x + 1, this->y + 1);
    Console::SetColor(Console::YELLOW);
    cout << center(title, this->w - 2);

    for (int i = 0; i <= this->GetSize(); i++){
        this->UpdateStatus(i);
    }
}

bool FormBox::operator()(Console::Key& key) 
{
    this->isUsed = true;
    this->confirmed = false;
    this->curr = 0;
    while (this->curr < this->GetSize() && this->values[this->curr] == "*") this->curr++;
    int preCurr;
    Console::SetCursorVisible(true);

    while (this->isUsed) {
        this->UpdateStatus(this->curr);

        bool choiceFieldFocused = this->curr < this->GetSize()
            && !this->values[this->curr].empty()
            && this->values[this->curr][0] == '#';
        static constexpr DWORD blinkInterval = 500;
        DWORD lastBlink = GetTickCount();
        this->choiceBlinkOn = true;

        if (choiceFieldFocused) {
            while (!_kbhit()) {
                DWORD now = GetTickCount();
                if (now - lastBlink >= blinkInterval) {
                    this->choiceBlinkOn = !this->choiceBlinkOn;
                    lastBlink = now;
                    this->UpdateStatus(this->curr);
                }
                Sleep(20);
            }
        }

        if (this->curr < this->GetSize() && this->values[this->curr] != "*" && 
        (this->values[this->curr].empty() || this->values[this->curr][0] != '#')){
            Console::SetCursorVisible(true);
            int cursorX = this->ValueStartX(this->curr) + this->values[this->curr].size();
            if (int(this->values[this->curr].size()) > this->ValueWidth(this->curr)) 
                cursorX = this->ValueStartX(this->curr) + this->ValueWidth(this->curr);
            Console::Goto(cursorX, this->y + 3 + this->curr);
        }
        else Console::SetCursorVisible(false);

        char c;
        key = Console::Read(c);
        
        if (key == Console::UP){
            preCurr = this->curr;
            do {
                this->curr--;
                if (this->curr < 0) this->curr = this->GetSize(); 
            } while (this->curr < this->GetSize() && this->values[this->curr] == "*");
            this->UpdateStatus(preCurr);
        }
        else if (key == Console::DOWN || (key == Console::ENTER && this->curr < this->GetSize())){
            preCurr = this->curr;
            do {
                this->curr++;
                if (this->curr > this->GetSize()) this->curr = 0;
            } while (this->curr < this->GetSize() && this->values[this->curr] == "*");
            this->UpdateStatus(preCurr);
        }
        else if (key == Console::ENTER && this->curr == this->GetSize()){
            this->confirmed = true;
            this->isUsed = false;
        }
        else if (key == Console::ESC) this->isUsed = false;
        else if (key == Console::BACKSPACE
            && this->curr < int(this->GetSize()) && !this->values[this->curr].empty()) {
            this->values[this->curr].pop_back();
        }
        else if (key == Console::LEFT && this->curr < this->GetSize()
            && !this->values[this->curr].empty() && this->values[this->curr][0] == '#') {
            int choice = this->values[this->curr][2] - '0';
            int count = this->values[this->curr][1] - '0';
            if (choice > 0) 
                this->values[this->curr][2] = static_cast<char>(choice - 1 + '0');
            else this->values[this->curr][2] = static_cast<char>(count - 1 + '0');
        }
        else if (key == Console::RIGHT && this->curr < this->GetSize() 
            && !this->values[this->curr].empty() && this->values[this->curr][0] == '#') {
            istringstream iss(this->values[this->curr]);
            int choice = this->values[this->curr][2] - '0';
            int count = this->values[this->curr][1] - '0';
            if (choice < count - 1) 
                this->values[this->curr][2] = static_cast<char>(choice + 1 + '0');
            else this->values[this->curr][2] = '0';
        }
        else if (this->curr < this->GetSize() && c != '\0' 
            && (this->values[this->curr].empty() || this->values[this->curr][0] != '#') 
            && (Console::AllowedChar(c))) {
            this->values[this->curr] += c;
        }

    }

    this->UpdateStatus(this->curr);
    Console::SetCursorVisible(false);
    Console::SetColor(Console::WHITE);
    return this->confirmed;
}

void SingleLineBox::Draw() 
{
    Console::SetCursorVisible(false);
    this->Clear();
    this->DrawBorder();

    Console::Goto(this->x + 1, this->y + 1);
    Console::SetColor(Console::YELLOW);
    cout << this->label << ": ";
    if (int(this->value.size()) <= this->ValueWidth()) cout << this->value;
    else cout << this->value.substr(int(this->value.size()) - this->ValueWidth(), this->ValueWidth());
    Console::SetColor(Console::WHITE);
}

void SingleLineBox::operator()(Console::Key& key) 
{
    this->Use();

    while (this->isUsed) {
        this->Draw();
        Console::SetCursorVisible(true);

        if (int(this->value.size()) <= this->ValueWidth()) 
            Console::Goto(this->x + 3 + this->label.size() + this->value.size(), this->y + 1);
        else Console::Goto(this->x + this->w - 2, this->y + 1);

        char c;
        key = Console::Read(c);
        if (key == Console::ENTER) this->isUsed = false;
        if (key == Console::ESC || key == Console::TAB || key == Console::UP 
            || key == Console::DOWN || key == Console::LEFT || key == Console::RIGHT){
            this->Quit();
        }
        else if (key == Console::BACKSPACE){
            if (!this->value.empty())
                this->value.pop_back();
        }
        else if (c != '\0') this->value += c;
    }
}

void WarningBox::Draw()
{
    Console::SetCursorVisible(false);
    this->Clear();
    this->DrawBorder();

    Console::SetColor(Console::WHITE);
    for (int i = 0; i < this->line.size(); i++) {
        Console::Goto(this->x + 1, this->y + 3 + i);
        cout << center(this->line[i], this->w - 2);
    }

    int wid = (this->w - 2) / 2;
    Console::Goto(this->x + 1, this->y + this->h - 2);
    if (this->choice == 1) Console::SetColor(Console::BLACK, Console::WHITE);
    else Console::SetColor(Console::WHITE);
    cout << center(this->labelY, wid);
    
    if (this->choice == 0) Console::SetColor(Console::BLACK, Console::WHITE);
    else Console::SetColor(Console::WHITE);
    cout << center(this->labelN, this->w - 2 - wid);
}

bool WarningBox::operator()()
{
    this->Draw();
    this->isUsed = true;
    this->choice = 1;

    while (this->isUsed) {
        char c;
        Console::Key key = Console::Read(c);

        if (key == Console::ENTER) {
            this->isUsed = false;
            Console::SetColor(Console::WHITE);
            return this->choice;
        }
        else if (key == Console::LEFT && this->choice == 0) this->choice = 1;
        else if (key == Console::RIGHT && this->choice == 1) this->choice = 0;

        this->Draw();
    }
    Console::SetColor(Console::WHITE);
    return false;
}
