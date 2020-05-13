# logue

Some of these are my creations, and some are compiled from [the Unit index](https://korginc.github.io/logue-sdk/unit-index/) and streamlines for a single workflow.

## Pre-requisites

You'll need the Korg logue-sdk environment to start using these effects. The easiest way to do this is using Docker, but you can also do it using a local dev environment.

### Docker dev environment

You can build everything with Docker-initiated development environment. Make sure you have Docker 19.03+ and run

```
make
```

It will build all units into the `build/` folder. Re-run `make` to rebuild (Docker caches dev environment so its not slow). You can then upload them manually using the `logue-cli` (which is also built and stashed in the `build/` dir).

If you want to just build all and upload into specified slots you can do:

```
make -i upload
```

If you want to clear everything and upload all:

```
make -i reset
```

### Local dev environment 

Clone this repository, and then install the [Korg's logue SDK](https://github.com/korginc/logue-sdk):

```bash
git clone https://github.com/schollz/logue
cd logue 

# get SDK
git clone --depth 1 https://github.com/korginc/logue-sdk

# get CSMSIS libraries
cd logue-sdk && git submodule update --depth 1 --init

# install the logue-cli
cd ../tools/logue-cli
./get_logue_cli_linux.sh
chmod +x logue-cli-*/logue-cli
sudo mv logue-cli-*/logue-cli /usr/local/bin/

# get linux gcc compiler
cd logue-sdk/tools/gcc
./get_gcc_linux.sh
```

## Effects

To build the effects, do the following (with `simplepan` as an example):

```
cd simplepan && make
```

Then to upload into slot `5` do something like 

```
logue-cli load -i 1 -o 1 -s 5 -u mod*.ntkdigunit
```


### simplepan (modfx)

[simplepan](https://github.com/schollz/logue/tree/master/simplepan) is a simple panning mod effect which allows you to control the depth and rate of a left/right pan.

### pitchshift (delfx)

[pitchshift](https://github.com/schollz/logue/tree/master/pitchshift) is a [fork from @centrevillage](https://github.com/centrevillage/cv_logue), which adds a delay pitch shifting effect.

### tremelo (modfx)

A nice tremelo from [@ttaschke](github.com/ttaschke/tremoxd), licensed under BSD-3.

### tldy (delfx)

This is an awesome time delay [from @techno-cat](https://github.com/techno-cat/logue-user-delfx-tdly), licensed under MIT. Note for some reason [Ubuntu can't load this normally](https://github.com/korginc/logue-sdk/issues/37#issuecomment-627982850) to upload you can use

	make -i tldy

which will upload it into slot 1.