#!/usr/bin/env python3
# pylint: disable=no-member
import time

def main():
    print("Witaj na Raspberry Pi!")
    time.sleep(1)
    print("Przygotowuję się do wywołania śmiesznego skryptu...")
    time.sleep(1)
    print("wysadzamy Olka za..")

    for i in reversed(range(3)):
        time.sleep(2)
        print(f"{i+1}...")

    time.sleep(3)
    print('💥💥👨‍✈️💥💥')

if __name__ == "__main__":
    main()