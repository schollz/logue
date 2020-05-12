
.PHONY: simplepan tremelo pitchshift

all: clean
	mkdir -p build
	DOCKER_BUILDKIT=1 docker build --build-arg BUILDTIME=`date +%s` --build-arg BUILDS="simplepan tremelo pitchshift" -t logue -f Dockerfile --output build .
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

clean:
	rm -rf build
