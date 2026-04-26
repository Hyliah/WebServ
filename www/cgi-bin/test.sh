#!/bin/bash

echo "Status: 200 OK"
echo "Content-Type: text/html"
echo ""

echo "<html><body>"
echo "<h1>BASH CGI WORKS </h1>"
echo "<p>Method: $REQUEST_METHOD</p>"
echo "<p>Query: $QUERY_STRING</p>"

echo "<p>Body:</p>"
echo "<pre>"
cat   # lit stdin (IMPORTANT pour POST)
echo "</pre>"

echo "</body></html>"