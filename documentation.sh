doxygen Doxyfile
moxygen ./docs/xml/ --output api.md
rm ./docs/xml/ -R
mv api.md ../../doc/source/includes/
cd ../../doc/
bundle exec middleman build --clean
cd build
cp -R * ../../lib/pHProbe/docs/
cd ../../lib/pHProbe/
