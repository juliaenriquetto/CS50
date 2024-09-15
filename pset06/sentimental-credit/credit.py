# american express - amex - 15 digits, starts with 34 or 37
# mastercard 16 digits, starts with 51, 52, 53, 54 or 55
# visa 13 or 16 digits, starts with 4

'''
checksum - Hans Peter Luhn of IBM algorithm to validate:
1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
2. Add the sum to the sum of the digits that weren’t multiplied by 2.
3. If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
'''


def checksum(number):
    # Convert the credit card number to a list of integers
    digits = [int(digit) for digit in number]

    # Reverse the digits for easier processing
    digits.reverse()

    # Initialize sum for the digits
    total_sum = 0

    # Iterate over the digits
    for i in range(len(digits)):
        if i % 2 == 1:
            # Multiply every second digit by 2
            doubled_digit = digits[i] * 2
            # If the result is greater than 9, subtract 9
            if doubled_digit > 9:
                doubled_digit -= 9
            total_sum += doubled_digit
        else:
            # add digits tht were not doubled
            total_sum += digits[i]

    # if the total sum modulo 10 is 0, the card is valid
    return total_sum % 10 == 0


def card_type(number):
    length = len(number)
    first_digit = number[0]
    first_two_digits = number[:2]

    if length == 15 and (first_two_digits == "34" or first_two_digits == "37"):
        return "AMEX"
    elif length == 16 and first_two_digits in ["51", "52", "53", "54", "55"]:
        return "MASTERCARD"
    elif (length == 13 or length == 16) and first_digit == "4":
        return "VISA"
    else:
        return "INVALID"


def main():
    card = input("Number: ")

    if checksum(card):
        print(card_type(card))
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
