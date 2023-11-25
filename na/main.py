import csv
import os
import random

def generate_pseudo_data(file_number):
    # Example pseudo data
    provinces = ["Sindh", "Punjab", "Balochistan", "KPK"]
    categories = ["Grant", "Merit", "Need-Based"]
    fields = [
        str(file_number).zfill(3),
        random.choice(provinces),
        random.choice(categories),
        "Biology" if random.random() < 0.5 else "Physics",
        random.randint(8000, 15000),
        "https://" + random.choice(["sindh", "punjab", "balochistan", "kpk"]) + ".gov.pk"
    ]
    return fields

def create_csv_file(file_number):
    file_name = f"{str(file_number).zfill(3)}.csv"
    with open(file_name, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(["ID", "Province", "Category", "Field", "Amount", "Website"])
        for _ in range(random.randint(5, 15)):
            csvwriter.writerow(generate_pseudo_data(file_number))

def main():
    # Create 50 CSV files
    for file_number in range(1, 51):
        create_csv_file(file_number)

if __name__ == "__main__":
    main()
