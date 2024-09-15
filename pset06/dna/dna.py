import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py /databases/[x.csv] /sequences/[n.txt]")
        sys.exit()

    # Read database file into a variable
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        database = list(reader)  # Store the contents of the CSV in a list of dictionaries

    # Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        sequence = file.read().strip()

    # Extract the STRs from the first row of the database
    strs = list(database[0].keys())[1:]  # Exclude the first column (name)

    # Find longest match of each STR in DNA sequence
    str_counts = {}
    for str_ in strs:
        str_counts[str_] = longest_match(sequence, str_)

    # Check database for matching profiles
    for person in database:
        match = True
        for str_ in strs:
            if int(person[str_]) != str_counts[str_]:
                match = False
                break
        if match:
            print(person["name"])
            return

    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


main()
