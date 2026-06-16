import subprocess
import time
import sys
import os
import re

def runPandoc(filename):
    try:
        ret = subprocess.run(
            ["pandoc", filename, "-o", filename[:-3] + ".html", "-s"],
            capture_output = True,
            text = True,
            check = False
        )

        return 0 if ret.returncode == 0 else 1
    except Exception as e:
        print(f"Error occurred, prob need to install pandoc: {e}", file=sys.stderr)
        return 1

def checkIfMarkdown(filename):
    if filename[-3:] == ".md":
        return 0
    else:
        return 1

def addStyling(filename):
    try:
        # Read file into list
        with open(filename, 'r') as f:
            print("successful first")
            list1 = f.readlines()

        # Copy list1 into list2, but stop at style element
        list2 = []
        for line in list1:
            if not re.search(r'(.+)?<(.+)?style(.+)?>(.+)?', line, re.IGNORECASE):
                list2.append(line)
            else:
                list2.append(line)
                break

        # Copy entire 'markdown.css' file line by line.
        styleFile = os.path.join(os.path.split(__file__)[0], "markdown.css")
        with open(styleFile, 'r') as f:
            markdown = f.readlines()
        list2 += markdown

        # Copy the rest of original file
        pastStyle = False
        for line in list1:
            if pastStyle:
                list2.append(line)
            if re.search(r'<(.+)?/(.+)?style(.+)?>', line, re.IGNORECASE):
                list2.append(line)
                pastStyle = True

        # Write to file
        with open(filename, 'w') as f:
            f.writelines(list2)
    except Exception as e:
        print(f"Error occurred: {e}", file=sys.stderr)
        return 1

    return 0

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: pandoc.py <filename>.md")
        sys.exit(0)
    elif checkIfMarkdown(sys.argv[1]) != 0:
        print("Invalid file extension.")
    else:
        htmlFile = sys.argv[1][:-3] + ".html"
        if runPandoc(sys.argv[1]) != 0:
            print("Error occurred running Pandoc.", file=sys.stderr)
            sys.exit(0)
        
        time.sleep(2)

        if addStyling(htmlFile) != 0:
            print("Error occurred adding custom styling.", file=sys.stderr)
            sys.exit(0)

    print(f"{sys.argv[1]} converted to HTML successfully!")
