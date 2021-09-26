
rm -rf wiki
mkdir wiki

doxygen
doxybook2 \
    --input doxygen/xml \
    --output wiki \
    --config .doxybook/config.json \
    --summary-input home.md.tmpl \
    --summary-output wiki/home.md