echo Please enter the location of Astro.exe
read astro
mkdir out
cp $astro out/Astro.exe
cp LICENSE.txt out/
cp LICENSE-3RD-PARTY.txt out/
cp -r docs out/docs
cp -r prog out/prog
cp -r assets out/assets