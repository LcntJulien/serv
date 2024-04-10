#!/usr/bin/env python3

import cgi
import cgitb

# Enable detailed error messages to be displayed in the browser
cgitb.enable()

# Print HTTP header
print("Content-Type: text/plain\n")

# Parse request parameters
form = cgi.FieldStorage()

# Print request parameters
print("Request Parameters:")
for key in form.keys():
    print(f"{key}: {form[key].value}")