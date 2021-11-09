#!/bin/sh

doxygen
doxybook2 \
    --input doxygen/xml \
    --output ../../2021_ite2038_2018008340.wiki \
    --config db_project/.doxybook/config.json \
    --summary-input db_project/.doxybook/home.md.tmpl \
    --summary-output ../../2021_ite2038_2018008340.wiki/home.md

cp -r db_project/manual-wiki/* ../../2021_ite2038_2018008340.wiki/

node wiki-gen.js

cd ../../2021_ite2038_2018008340.wiki/

git add .
git commit -m "Wiki update"
git push