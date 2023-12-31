#!/usr/bin/env python3

import sqlite3
import time
import os

def create_database():
    if os.path.exists("workers.db"):
        os.remove("workers.db")

    print("Removed the previous database")

    connection = sqlite3.connect("workers.db")
    cursor = connection.cursor()

    cursor.execute(""" CREATE TABLE workers_log (
                        log_time text,
                        worker text,
                        terminal_id text
                        )""")
    
    connection.commit()
    connection.close()

    print("Created the new database")

if __name__ == "__main__":
    create_database()
