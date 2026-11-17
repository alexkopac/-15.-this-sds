#include <iostream>
#include <cstring>
#include <windows.h>

using namespace std;

// Допоміжна функція для безпечного динамічного копіювання C-рядків
char* copyString(const char* source) {
    if (source == nullptr) {
        return nullptr;
    }
    char* dest = new char[strlen(source) + 1];
    strcpy(dest, source);
    return dest;
}

// =========================================================
// 1. Клас «Людина»
// =========================================================
class Person {
private:
    char* fullName;
    int age;

public:
    // Конструктор за замовчуванням
    Person() : fullName(nullptr), age(0) {}

    // Параметризований конструктор (демонстрація покажчика this)
    Person(const char* fullName, int age) {
        this->fullName = copyString(fullName);
        this->age = age;
    }

    // Конструктор копіювання (глибоке копіювання динамічного рядка)
    Person(const Person& other) {
        this->fullName = copyString(other.fullName);
        this->age = other.age;
    }

    // Оператор присвоєння копіюванням
    Person& operator=(const Person& other) {
        if (this == &other) { // Захист від самоприсвоєння через this
            return *this;
        }
        delete[] this->fullName;
        this->fullName = copyString(other.fullName);
        this->age = other.age;
        return *this;
    }

    // Деструктор
    ~Person() {
        delete[] this->fullName;
    }

    // Сеттери з поверненням *this для ланцюжкових викликів
    Person& setFullName(const char* fullName) {
        delete[] this->fullName;
        this->fullName = copyString(fullName);
        return *this;
    }

    Person& setAge(int age) {
        this->age = age;
        return *this;
    }

    const char* getFullName() const {
        return this->fullName ? this->fullName : "Не вказано";
    }

    int getAge() const {
        return this->age;
    }

    void print() const {
        cout << "      • " << getFullName() << " (вік: " << this->age << " р.)\n";
    }
};

// =========================================================
// 2. Клас «Квартира»
// =========================================================
class Apartment {
private:
    int number;
    double area;
    Person* residents;   // Динамічний масив об'єктів класу Person
    int residentCount;

public:
    // Конструктор за замовчуванням
    Apartment() : number(0), area(0.0), residents(nullptr), residentCount(0) {}

    // Параметризований конструктор
    Apartment(int number, double area) {
        this->number = number;
        this->area = area;
        this->residents = nullptr;
        this->residentCount = 0;
    }

    // Конструктор копіювання (глибоке копіювання масиву мешканців)
    Apartment(const Apartment& other) {
        this->number = other.number;
        this->area = other.area;
        this->residentCount = other.residentCount;

        if (other.residentCount > 0) {
            this->residents = new Person[other.residentCount];
            for (int i = 0; i < other.residentCount; i++) {
                this->residents[i] = other.residents[i]; // Виклик Person::operator=
            }
        } else {
            this->residents = nullptr;
        }
    }

    // Оператор присвоєння копіюванням
    Apartment& operator=(const Apartment& other) {
        if (this == &other) {
            return *this;
        }
        delete[] this->residents;

        this->number = other.number;
        this->area = other.area;
        this->residentCount = other.residentCount;

        if (other.residentCount > 0) {
            this->residents = new Person[other.residentCount];
            for (int i = 0; i < other.residentCount; i++) {
                this->residents[i] = other.residents[i];
            }
        } else {
            this->residents = nullptr;
        }
        return *this;
    }

    // Деструктор
    ~Apartment() {
        delete[] this->residents;
    }

    // Додавання мешканця з розширенням динамічного масиву
    void addResident(const Person& person) {
        Person* updatedList = new Person[this->residentCount + 1];
        for (int i = 0; i < this->residentCount; i++) {
            updatedList[i] = this->residents[i];
        }
        updatedList[this->residentCount] = person;

        delete[] this->residents;
        this->residents = updatedList;
        this->residentCount++;
    }

    Person* getResident(int index) {
        if (index >= 0 && index < this->residentCount) {
            return &this->residents[index];
        }
        return nullptr;
    }

    int getNumber() const { return this->number; }
    double getArea() const { return this->area; }
    int getResidentCount() const { return this->residentCount; }

    void print() const {
        cout << "   Квартира №" << this->number
             << " | Площа: " << this->area << " м²"
             << " | Мешканців: " << this->residentCount << '\n';

        if (this->residentCount == 0) {
            cout << "      (квартира порожня)\n";
        } else {
            for (int i = 0; i < this->residentCount; i++) {
                this->residents[i].print();
            }
        }
    }
};

// =========================================================
// 3. Клас «Дім»
// =========================================================
class House {
private:
    char* address;           // Динамічний рядок для адреси
    Apartment* apartments;   // Динамічний масив об'єктів класу Apartment
    int apartmentCount;

public:
    // Конструктор за замовчуванням
    House() : address(nullptr), apartments(nullptr), apartmentCount(0) {}

    // Параметризований конструктор
    House(const char* address) {
        this->address = copyString(address);
        this->apartments = nullptr;
        this->apartmentCount = 0;
    }

    // Конструктор копіювання (глибоке копіювання адреси та масиву квартир)
    House(const House& other) {
        this->address = copyString(other.address);
        this->apartmentCount = other.apartmentCount;

        if (other.apartmentCount > 0) {
            this->apartments = new Apartment[other.apartmentCount];
            for (int i = 0; i < other.apartmentCount; i++) {
                this->apartments[i] = other.apartments[i]; // Виклик Apartment::operator=
            }
        } else {
            this->apartments = nullptr;
        }
    }

    // Оператор присвоєння копіюванням
    House& operator=(const House& other) {
        if (this == &other) {
            return *this;
        }
        delete[] this->address;
        delete[] this->apartments;

        this->address = copyString(other.address);
        this->apartmentCount = other.apartmentCount;

        if (other.apartmentCount > 0) {
            this->apartments = new Apartment[other.apartmentCount];
            for (int i = 0; i < other.apartmentCount; i++) {
                this->apartments[i] = other.apartments[i];
            }
        } else {
            this->apartments = nullptr;
        }
        return *this;
    }

    // Деструктор
    ~House() {
        delete[] this->address;
        delete[] this->apartments;
    }

    void setAddress(const char* address) {
        delete[] this->address;
        this->address = copyString(address);
    }

    void addApartment(const Apartment& apt) {
        Apartment* updatedApts = new Apartment[this->apartmentCount + 1];
        for (int i = 0; i < this->apartmentCount; i++) {
            updatedApts[i] = this->apartments[i];
        }
        updatedApts[this->apartmentCount] = apt;

        delete[] this->apartments;
        this->apartments = updatedApts;
        this->apartmentCount++;
    }

    Apartment* getApartment(int index) {
        if (index >= 0 && index < this->apartmentCount) {
            return &this->apartments[index];
        }
        return nullptr;
    }

    void print() const {
        cout << "==================================================\n";
        cout << "Будинок за адресою: " << (this->address ? this->address : "Не вказано") << '\n';
        cout << "Кількість квартир: " << this->apartmentCount << '\n';
        cout << "--------------------------------------------------\n";
        for (int i = 0; i < this->apartmentCount; i++) {
            this->apartments[i].print();
        }
        cout << "==================================================\n\n";
    }
};

// =========================================================
// Тестування роботи класів та конструктора копіювання
// =========================================================
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 1. Створення людей
    Person p1("Коваленко Олександр Іванович", 34);
    Person p2("Коваленко Марія Сергіївна", 31);
    Person p3("Бондаренко Дмитро Олегович", 27);

    // 2. Створення квартир та заселення мешканців
    Apartment apt101(101, 64.5);
    apt101.addResident(p1);
    apt101.addResident(p2);

    Apartment apt102(102, 42.0);
    apt102.addResident(p3);

    // 3. Створення будинку та додавання квартир
    House originalHouse("м. Одеса, вул. Дерибасівська, 15");
    originalHouse.addApartment(apt101);
    originalHouse.addApartment(apt102);

    cout << ">>> ОРИГІНАЛЬНИЙ БУДИНОК:\n";
    originalHouse.print();

    // 4. Перевірка конструктора копіювання (глибоке копіювання)
    House copiedHouse = originalHouse;

    // Змінюємо дані в оригінальному будинку після створення копії
    originalHouse.setAddress("м. Одеса, вул. Дерибасівська, 15 (КОРПУС Б - ЗМІНЕНО)");
    if (Apartment* firstApt = originalHouse.getApartment(0)) {
        if (Person* firstPerson = firstApt->getResident(0)) {
            firstPerson->setFullName("ЗМІНЕНЕ ПІБ МЕШКАНЦЯ").setAge(99);
        }
    }

    cout << ">>> ОРИГІНАЛЬНИЙ БУДИНОК ПІСЛЯ ЗМІН:\n";
    originalHouse.print();

    cout << ">>> СКОПІЙОВАНИЙ БУДИНОК (дані залишилися незмінними завдяки глибокому копіюванню):\n";
    copiedHouse.print();

    return 0;
}