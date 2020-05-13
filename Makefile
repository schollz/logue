
.PHONY: simplepan tremelo pitchshift clipper

all: clean
	mkdir -p build
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="simplepan tremelo pitchshift clipper hipass" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli

simplepan:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="simplepan" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	./build/logue-cli load -i 1 -o 1 -s 0 -u build/modfx_simplepan.ntkdigunit

tremelo:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="tremelo" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	./build/logue-cli load -i 1 -o 1 -s 1 -u build/tremelo.ntkdigunit

pitchshift:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="pitchshift" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	./build/logue-cli load -i 1 -o 1 -s 0 -u build/pitchshift.mnlgxdunit

hipass:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="hipass" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	./build/logue-cli load -i 1 -o 1 -s 0 -u build/hipass.ntkdigunit

clipper:
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="clipper" -t logue -f Dockerfile --output build .
	chmod +x build/logue-cli
	./build/logue-cli load -i 1 -o 1 -s 1 -u build/clipper.ntkdigunit

upload: simplepan tremelo pitchshift hipass

clean:
	rm -rf build

reset: all
	./build/logue-cli clear -i 1 -o 1 -a -m revfx
	./build/logue-cli clear -i 1 -o 1 -a -m modfx
	./build/logue-cli clear -i 1 -o 1 -a -m delfx
	./build/logue-cli load -i 1 -o 1 -u build/hipass.ntkdigunit
	./build/logue-cli load -i 1 -o 1 -u build/pitchshift.mnlgxdunit
	./build/logue-cli load -i 1 -o 1 -u build/tremelo.ntkdigunit
	./build/logue-cli load -i 1 -o 1 -u build/modfx_simplepan.ntkdigunit
	./build/logue-cli load -i 1 -o 1 -u build/clipper.ntkdigunit
	./build/logue-cli probe -i 1 -o 1 -s 6 -m modfx
	./build/logue-cli probe -i 1 -o 1 -s 6 -m delfx
	./build/logue-cli probe -i 1 -o 1 -s 6 -m revfx
