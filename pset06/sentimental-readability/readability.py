from cs50 import get_string

text = get_string("Text: ")
letter = 0
word = 1
sentence = 0

for i in range(len(text)):
    if text[i].isalpha():
        letter += 1
    elif text[i].isspace():
        word += 1
    elif (text[i] == '.' or text[i] == '!' or text[i] == '?'):
        sentence += 1

L = (letter / word) * 100
S = (sentence / word) * 100

index = round(0.0588 * L - 0.296 * S - 15.8)  # Coleman Liau Index

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
