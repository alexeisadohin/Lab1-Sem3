#include <iostream>
#include <string>
#include "ListSequence.h"

void dataType() {
    std::cout<< "Choose data type:\n";
    std::cout<< "1. int\n";
    std::cout<< "2. std::string\n";
    std::cout<< "3. double\n";
    std::cout<< "4. char\n";
    std::cout<< "0. Exit\n";
    std::cout<< "Enter your choice: ";
}

void printMenu() {
    std::cout<< "Choose operation:\n";
    std::cout << "1. Append an item\n";
    std::cout << "2. Prepend an item\n";
    std::cout << "3. Get a subsequence\n";
    std::cout << "4. Insert an item at a specific index\n";
    std::cout << "5. Get the first item\n";
    std::cout << "6. Get the last item\n";
    std::cout << "7. Get an item at a specific index\n";
    std::cout << "8. Get the length of the sequence\n";
    std::cout << "9. Print the sequence\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

template <typename T>
void executeChoice(ListSequence<T>& sequence) {
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1: {
            T item;
            std::cout << "Enter the item to append: ";
            std::cin >> item;
            sequence.Append(item);
            std::cout << "Item appended successfully.\n";
            break;
        }
        case 2: {
            T item;
            std::cout << "Enter the item to prepend: ";
            std::cin >> item;
            sequence.Prepend(item);
            std::cout << "Item prepended successfully.\n";
            break;
        }
        case 3: {
            int startIndex, endIndex;
            std::cout << "Enter the start index: ";
            std::cin >> startIndex;
            std::cout << "Enter the end index: ";
            std::cin >> endIndex;
            try {
                UnqPtr<Sequence<T>> subsequence = sequence.GetSubsequence(startIndex, endIndex);
                std::cout << "Subsequence: ";
                for (int i = 0; i < subsequence->GetLength(); ++i) {
                    std::cout << subsequence->Get(i) << " ";
                }
                std::cout << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 4: {
            int index;
            T item;
            std::cout << "Enter the index: ";
            std::cin >> index;
            std::cout << "Enter the item to insert: ";
            std::cin >> item;
            try {
                sequence.InsertAt(item, index);
                std::cout << "Item inserted successfully.\n";
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 5: {
            try {
                T first = sequence.GetFirst();
                std::cout << "First item: " << first << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 6: {
            try {
                T last = sequence.GetLast();
                std::cout << "Last item: " << last << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 7: {
            int index;
            std::cout << "Enter the index: ";
            std::cin >> index;
            try {
                T value = sequence.Get(index);
                std::cout << "Item at index " << index << ": " << value << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 8:
            std::cout << "Length of the sequence: " << sequence.GetLength() << std::endl;
            break;
        case 9:
            std::cout << "Sequence: ";
            for (int i = 0; i < sequence.GetLength(); ++i) {
                std::cout << sequence.Get(i) << " ";
            }
            std::cout << std::endl;
            break;
        case 0:
            std::cout << "Exiting...\n";
            std::exit(0);
        default:
            std::cout << "Invalid choice. Please try again.\n";
    }
}

template <typename T>
void workWithSequence() {
    ListSequence<T> sequence;
    while (true) {
        printMenu();
        executeChoice(sequence);
    }
}

void menu() {
    std::cout << "Menu:\n";
    int choice;
    do {
        dataType();
        std::cin >> choice;

        switch (choice) {
            case 1:
                workWithSequence<int>();
                break;
            case 2:
                workWithSequence<std::string>();
                break;
            case 3:
                workWithSequence<double>();
                break;
            case 4:
                workWithSequence<char>();
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid input. Please try again." << std::endl;
        }
    } while (choice != 0);
}