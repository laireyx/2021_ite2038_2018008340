
rm -rf ../wiki
mkdir ../wiki

doxygen
doxybook2 \
    --input doxygen/xml \
    --output ../wiki \
    --config .doxybook/config.json

cd ..
git add wiki/
git commit -m "Wiki update"
git subtree push --prefix wiki origin-wiki master