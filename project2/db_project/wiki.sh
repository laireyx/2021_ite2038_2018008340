
pushd .
cd ..
cd ..
git subtre pull origin-wiki --prefix wiki
popd

doxygen
doxybook2 \
    --input doxygen/xml \
    --output ../../wiki \
    --config .doxybook/config.json \
    --summary-input .doxybook/home.md.tmpl \
    --summary-output ../../wiki/home.md

node wiki-gen.js

cd ../..

git add wiki
git commit -m "Wiki update"
git subtree push --prefix wiki origin-wiki master