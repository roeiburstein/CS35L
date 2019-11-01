#!/usr/bin/python
import random, sys, argparse

class shuf:
   def __init__(self, filename: str):
      if filename == None or filename == "-":
         self.lines = sys.stdin.readlines()
         if len(self.lines) == 0:
            sys.exit("File does not exist")
      else:
         f = open(filename, 'r')
         self.lines = f.readlines()
         f.close()

   def chooselines(self, input_range, num_lines, repeat, no_num_input):
      lines_list = []
      file_lines = self.lines

      while True:
         for x in range(num_lines):
            needs_to_repeat = False
            while True:
               if input_range is None:
                  temp_line = random.choice(range(len(file_lines)))
               else:
                  temp_line = random.choice(
                     range(input_range[0] - 1, input_range[1]))
               if repeat:
                  lines_list.append(file_lines[temp_line])
               else:
                  if file_lines[temp_line] not in lines_list:
                     lines_list.append(file_lines[temp_line])
                     needs_to_repeat = False
                  else:
                     needs_to_repeat = True
               if needs_to_repeat is False:
                  sys.stdout.write(file_lines[temp_line])
                  break
         if not no_num_input:
            break


def main():

   parser = argparse.ArgumentParser(description="Parses the different options"
                                                "of shuf")
   parser.add_argument("-i", "--input-range", action="store", dest="range_str",
                       default="0-0", help="Act as if input came from a file "
                                        "containing the range of unsigned "
                                        "decimal integers lo…hi, one per line.")

   parser.add_argument("-n", "--head-count",
                       action="store", dest="numlines", default=0,
                       help="Output at most count lines. By default, all input "
                            "lines are output.")

   parser.add_argument("-r", "--repeat", action="store_true", dest="repeat",
                       help="Repeat output values, that is, select "
                            "with replacement.")

   parser.add_argument("input", nargs="*")

   args = parser.parse_args()

   input_file = args.input[0]
   try:
      f = open(input_file)
      f.close()

   except FileNotFoundError:
      sys.exit("ERROR: File does not exist")

   repeat = bool(args.repeat)
   input_range = get_range(str(args.range_str))
   tot_lines_in_file = file_length(input_file)

   if input_range == [0,0]:
      input_range = [0, tot_lines_in_file]
   if input_range[0] < 0:
      sys.stderr.write("ERROR: invalid lower bound: {0}".format(args.numlines)
                       + "\n")
      input_range[0] = 0

   if input_range[1] > tot_lines_in_file:
      sys.stderr.write("ERROR: invalid upper bound: {0}".format(args.numlines)
                       + "\n")
      input_range[1] = tot_lines_in_file

   if int(args.numlines) != 0:
      try:
         num_lines = int(args.numlines)
      except:
         sys.stderr.write("ERROR: invalid number of lines: {0}".format(
            args.numlines) + "\n")
         num_lines = tot_lines_in_file
      if(not repeat):
         if num_lines < 0:
            sys.stderr.write("ERROR: negative count: {0}".format(num_lines) + "\n")
            num_lines = tot_lines_in_file

         elif num_lines > tot_lines_in_file:
            sys.stderr.write("ERROR: too many lines: {0}".format(num_lines) + "\n")
            num_lines = tot_lines_in_file

      no_numline_input = False

   else:
      num_lines = tot_lines_in_file
      no_numline_input = True

   if not repeat and (1 + input_range[1] - input_range[0]) < num_lines:
      parser.error("range smaller than number of lines. Program will exit")
      sys.exit(1)

   try:
      generator = shuf(input_file)
      generator.chooselines(input_range, num_lines, repeat,
                                    no_numline_input)

   except:
      parser.error("ERROR, generator failed. Program will exit")
      sys.exit(1)

def get_range(range_string: str) -> [int, int]:
   if range_string is "":
      return None
   try:
      return list(map(int, range_string.split("-")))
   except SyntaxError:
      sys.stderr.write("ERROR: invalid format, default range will be used"
                       + "\n")
      return None


def file_length(fname):
   with open(fname) as f:
      for i, l in enumerate(f):
         pass
   return i + 1

if __name__ == "__main__":
   main()
