from cs50 import get_int

# almost like a do-while
# prompt and validate user input
while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break

# draw pyramid
    # iterates from 1 to height
for i in range(1, height + 1):
    print(" " * (height - i) + "#" * i + " " * 2 + "#" * i)
    # " " * (height - i) creates the left-aligned spaces
    # "#" * i creates the left pyramid
    # " " * 2 adds a gap of two spaces between the pyramids
    # "#" * i creates the right pyramid
