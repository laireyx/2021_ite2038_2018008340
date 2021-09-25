
rm -rf ../wiki
mkdir ../wiki

doxygen
doxybook2 --input doxygen/xml --output ../wiki