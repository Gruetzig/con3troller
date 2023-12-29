rm -rf output

cd con3troller && make clean && make && cd ..

mkdir output

cp readme.md output/
cp con3troller/con3troller.3dsx output/
cp con3troller/kconfig.json output/
cp con3troller/server.py output/

cd output
zip -r con3troller.zip .
cd ..