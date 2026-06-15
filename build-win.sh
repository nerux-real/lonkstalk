[ -f build-win/prod/scores.db ] && cp build-win/prod/scores.db gamestuff/scores.db
[ -f build-win/prod/settings.ini ] && cp build-win/prod/settings.ini gamestuff/settings.ini
rm -rf build-win
mkdir build-win
cp -r gamestuff/beatmaps build-win/beatmaps
cp -r gamestuff/skins build-win/skins
[ -f gamestuff/scores.db ] && cp gamestuff/scores.db build-win/scores.db
[ -f gamestuff/settings.ini ] && cp gamestuff/settings.ini build-win/settings.ini
cd build-win
cmake .. -G "MinGW Makefiles"
mingw32-make
mkdir prod
cp Lonkstalk.exe prod/VLBeats.exe
cp -r gamestuff/skins prod/skins
cp -r gamestuff/beatmaps prod/beatmaps
ldd Lonkstalk.exe | grep '/mingw64/bin/' | awk '{print $3}' | while read -r dll; do
    cp -u "$dll" prod/
done
cp LKPacker.exe prod/LKPacker.exe
ldd LKPacker.exe | grep '/mingw64/bin/' | awk '{print $3}' | while read -r dll; do
    cp -u "$dll" prod/
done
[ -f gamestuff/scores.db ] && cp gamestuff/scores.db build-win/prod/scores.db
[ -f gamestuff/settings.ini ] && cp gamestuff/settings.ini build-win/prod/settings.ini
