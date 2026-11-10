#pragma once
#include <ConsoleUI.h>
#include <algorithm>
#include <exception>
#include <functional>
#include <List.h>
#include <Nhatro.h>
#include <Room.h>
#include <Tenant.h>
#include <header.h>
#include <type_traits>
#include <memory>
using std::function;
using std::string;
using std::to_string;
using std::unique_ptr;

template<typename T>
struct SortOption {
    string field;
    function<bool(T*, T*)> compare;
};

template<typename T>
struct FilterOption {
    string field;
    function<bool(T*)> isa;
};

template<typename T>
class Manager
{
private:
    string objectName;
    Nhatro& nhatro;
    const List<unique_ptr<T>>& originData;
    int tableWidth;
    function<void(int, int)> tableInit;
    function<void(int, int)> tableEnd;
    function<void(T* const&)> tableShow;
    function<void(Nhatro&, List<T*>&)> loading;

    function<bool(T*, const std::string&)> searching;
    function<void(Nhatro&)> adding;
    function<void(Nhatro&, T*)> updating;
    function<void(Nhatro&, T*)> removing;
    bool deletable;

    List<T*> filteredData;
    List<T*> sortedData;
    List<FilterOption<T>> filtering;
    List<SortOption<T>> sorting;

    enum Action {
        NONE,
        FILTER,
        SORT,
        ADD,
        REMOVE,
        SEARCH,
        DETAIL
    };
    Action selection = NONE;
    InputHandler<Action> btnLink{Action::NONE, Action::DETAIL};
    
    void ReloadData() {
        this->sortedData.clear();
        for (const auto& item : this->originData)
            this->sortedData.push_back(item.get());
    }

    void GetFullData() {
        this->filteredData = this->sortedData;
    }

    void SearchData(std::string& query);
    void SortData(int);
    void FilterData(int);

public:
    Manager(const string& name, Nhatro& database, const List<unique_ptr<T>>& data, int tableW, 
    function<void(int, int)> tableOpenFunc, function<void(int, int)> tableCloseFunc, function<void(T* const&)> tableShowFunc,
    function<bool(T*, const std::string&)> searchFunc, function<void(Nhatro&)> addFunc, 
    function<void(Nhatro&, T*)> updateFunc, function<void(Nhatro&, T*)> removeFunc = nullptr)
        : objectName(name), nhatro(database), originData(data), tableWidth(tableW), 
        tableInit(tableOpenFunc), tableEnd(tableCloseFunc), tableShow(tableShowFunc), 
        searching(searchFunc), adding(addFunc), updating(updateFunc), removing(removeFunc) 
    {
        this->deletable = (this->removing == nullptr ? false : true);
        this->AddFilter("Tat ca " + this->objectName, nullptr);
        this->AddSortComparison("Ma so " + this->objectName + " tang dan", nullptr);
        this->AddSortComparison("Ma so " + this->objectName + " giam dan", nullptr);
        this->ReloadData();
        this->GetFullData();

        this->btnLink.AddRelation(REMOVE, Console::UP, DETAIL);
        this->btnLink.AddRelation(DETAIL, Console::UP, ADD);
        this->btnLink.AddRelation(ADD, Console::UP, SORT);
        this->btnLink.AddRelation(SORT, Console::UP, FILTER);

        this->btnLink.AddRelation(FILTER, Console::DOWN, SORT);
        this->btnLink.AddRelation(SORT, Console::DOWN, ADD);
        this->btnLink.AddRelation(ADD, Console::DOWN, DETAIL);
        if (this->deletable)
            this->btnLink.AddRelation(DETAIL, Console::DOWN, REMOVE);

        this->btnLink.AddRelation(SEARCH, Console::LEFT, FILTER);

        this->btnLink.AddRelation(FILTER, Console::RIGHT, SEARCH);
        this->btnLink.AddRelation(SORT, Console::RIGHT, SEARCH);
        this->btnLink.AddRelation(ADD, Console::RIGHT, SEARCH);
        this->btnLink.AddRelation(DETAIL, Console::RIGHT, SEARCH);
        if (this->deletable)
            this->btnLink.AddRelation(REMOVE, Console::RIGHT, SEARCH);
    }


    ~Manager() = default;

    void AddFilter(const string& filter, function<bool(T*)> func) {
        this->filtering.push_back(FilterOption<T>{to_string(this->filtering.size() + 1) + ". " + filter, func});
    }

    void AddSortComparison(const string& crit, function<bool(T*, T*)> func) {
        this->sorting.push_back(SortOption<T>{to_string(this->sorting.size() + 1) + ". " + crit, func});
    }

    void Draw();
};


template<typename T>
void Manager<T>::SortData(int choice)
{
    if (choice == 0) return this->ReloadData();

    if (choice == 1) {
        this->sortedData.clear();
        for (int i = int(this->originData.size()); i > 0; --i)
            this->sortedData.push_back(this->originData[i - 1].get());
        return;
    }

    std::stable_sort(this->sortedData.begin(), this->sortedData.end(), 
        [&](T* a, T* b) {
            return this->sorting[choice].compare(a, b);
        });
}

template<typename T>
void Manager<T>::FilterData(int choice)
{
    if (choice == 0) return this->GetFullData();

    this->filteredData.clear();
    for (const auto& item : this->sortedData) {
        if (this->filtering[choice].isa(item)) {
            this->filteredData.push_back(item);
        }
    }
}

template<typename T>
void Manager<T>::SearchData(std::string& query) {
    this->filteredData.clear();
    int idIndex = -1;
    if constexpr (std::is_same_v<T, Room>) {
        idIndex = this->nhatro.FindRoom(query);
    } else if constexpr (std::is_same_v<T, Tenant>) {
        idIndex = this->nhatro.FindTenant(query);
    }

    if (idIndex >= 0 && idIndex < this->originData.size()
        && this->originData[idIndex]
        && this->originData[idIndex]->GetId() == query) {
        this->filteredData.push_back(this->originData[idIndex].get());
        return;
    }

    for (int i = 0; i < int(this->sortedData.size()); i++) {
        if (this->searching(this->sortedData[i], query)) {
            this->filteredData.push_back(this->sortedData[i]);
        }
    }
}

template<typename T>
void Manager<T>::Draw() {
    static int toolBarW = 32;
    static int searchBoxW = 50;
    static int padding = 2;    
    
    NumberMenuBox filterBox(padding, padding, toolBarW, "LOC THONG TIN " + this->objectName);
    for (auto at : this->filtering) filterBox.AddOption(at.field);

    NumberMenuBox sortBox(padding, filterBox.GetY() + filterBox.GetH(), toolBarW, "SAP XEP " + this->objectName);
    for (auto crit : this->sorting) sortBox.AddOption(crit.field);

    ButtonBox addBtn(padding, sortBox.GetY() + sortBox.GetH(), toolBarW, "THEM " + this->objectName);

    ButtonBox detailBtn(padding, addBtn.GetY() + addBtn.GetH(), toolBarW, "XEM VA CAP NHAT " + this->objectName);

    ButtonBox deleteBtn(padding, detailBtn.GetY() + detailBtn.GetH(), toolBarW, "XOA " + this->objectName);

    string searchValue = "";
    SingleLineBox searchBox(padding*2 + toolBarW, padding, searchBoxW, "TIM KIEM " + this->objectName, searchValue);
    TableBox<T*> dataBox(padding*2 + toolBarW, padding*2 + searchBox.GetH(), this->tableWidth, this->filteredData, this->tableInit, this->tableEnd, this->tableShow);

    this->selection = FILTER;
    bool redraw = true;
    int filterChoice = 0, sortChoice = 0;
    
    while (true) {
    try {
        if (redraw) {
            system("cls");
            Console::Goto(36, 0);
            Console::SetColor(Console::YELLOW);
            cout << "QUAN LY " << this->objectName;

            filterBox.Draw();
            sortBox.Draw();
            addBtn.Draw();
            if (this->deletable) deleteBtn.Draw();
            searchBox.Draw();
            detailBtn.Draw();
            dataBox.Draw();
            redraw = false;
        }

        Console::Key k;
        if (this->selection == FILTER) {
            int choice = filterBox(k);
            if (choice != -1) {
                this->FilterData(choice);
                dataBox.Draw();
                filterChoice = choice;
            } 
        }
        if (this->selection == SORT) {
            int choice = sortBox(k);
            if (choice != -1) {
                this->SortData(choice);
                this->FilterData(filterChoice);
                dataBox.Draw();
                sortChoice = choice;
            } 
        }
        if (this->selection == ADD) {
            bool result = addBtn(k);
            if (result) {
                this->adding(this->nhatro);
                this->ReloadData();
                this->SortData(sortChoice);
                this->FilterData(filterChoice);
                redraw = true;
            }
        } 
        if (this->selection == REMOVE && this->deletable) {
            bool result = deleteBtn(k);
            if (result) {
                int index = dataBox(k);
                if (index != -1) {
                    this->removing(this->nhatro, this->filteredData[index]);
                    this->ReloadData();
                    this->SortData(sortChoice);
                    this->FilterData(filterChoice);
                    redraw = true;
                }
                k = Console::OTHER;
            }
        } 
        if (this->selection == SEARCH) {
            searchBox(k);
            if (k == Console::ENTER) {
                this->SearchData(searchValue);
                dataBox.Draw();
            }
        } 
        if (this->selection == DETAIL) {
            bool result = detailBtn(k);
            if (result) {
                int index = dataBox(k);
                if (index != -1) {
                    this->updating(this->nhatro, this->filteredData[index]);
                    this->SortData(sortChoice);
                    this->FilterData(filterChoice);
                    redraw = true;
                }
                k = Console::OTHER;
            }
        }

        if (k == Console::ESC) break;
        this->selection = this->btnLink.GetDest(this->selection, k);
    }
    catch (const std::exception& e) {
        Console::Goto(padding, padding*6 + toolBarW + filterBox.GetH() + sortBox.GetH() 
                        + addBtn.GetH() + detailBtn.GetH() + deleteBtn.GetH());
        Console::SetColor(Console::LIGHT_RED);
        cout << e.what() << " ";
        system("pause");
        redraw = true;
    }
    catch (const string& e) {
        Console::Goto(padding, padding*6 + toolBarW + filterBox.GetH() + sortBox.GetH() 
                        + addBtn.GetH() + detailBtn.GetH() + deleteBtn.GetH());
        Console::SetColor(Console::LIGHT_RED);
        cout << e << " ";
        system("pause");
        redraw = true;
    }
    }
}
