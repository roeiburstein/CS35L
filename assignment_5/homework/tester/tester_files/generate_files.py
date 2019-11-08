#!/usr/bin/python
import random
import string

def main():
   file1 = open("1000000_words.txt", "w")
   for i in range(1000000):
      file1.write(randomString(4) + " ")
      # file1.write(randomString(4))
   return


def randomString(stringLength):
   """Generate a random string of fixed length """
   letters = string.ascii_letters + string.digits + string.punctuation
   return ''.join(random.choice(letters) for i in range(stringLength))

if __name__ == "__main__":
    main()