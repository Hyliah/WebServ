#!/usr/bin/env python3

import os

query = os.environ.get("QUERY_STRING", "")
filename = query.replace("?file=", "")
path = os.path.join("./www/upload/", filename)

print("Content-Type: text/html")
print("")
print("Content-Type: text/html")
if os.path.exists(path):
    os.remove(path)
    print(f"<h1>File {filename} deleted by CGI</h1>")
else:
    print("<h1>File not found</h1>")