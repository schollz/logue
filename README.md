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
git clone https://github.com/korginc/logue-sdk

# get CSMSIS libraries
cd logue-sdk && git submodule update --init

# install the logue-cli
cd ../tools/logue-cli
./get_logue_cli_linux.sh
chmod +x logue-cli-*/logue-cli
# put onto path
sudo mv logue-cli-*/logue-cli /usr/local/bin/


# get linux gcc compiler
# the following doesn't work in ubuntu 19+...they dropped 32-bit support
#cd logue-sdk/tools/gcc
#./get_gcc_linux.sh
# instead, do (from head of main repo)
sudo apt install gcc-arm-none-eabi
mkdir -p logue-sdk/tools/gcc/gcc-arm-none-eabi-5_4-2016q3/bin
ln -s /usr/bin/arm-none-eabi-gcc logue-sdk/tools/gcc/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-gcc
ln -s /usr/bin/arm-none-eabi-g++ logue-sdk/tools/gcc/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-g++
```

Now you can build effects using the local environment. Simple `cd` into a directory and run `make`:

```
cd simplepan && make
```

Then to upload into slot `0` do something like 

```bash
INPORT=`logue-cli probe 2>&1 | grep NTS | grep "in " | sed "s/:.*//" | sed "s/in //" | sed 's/^ *//g'`
OUTPORT=`logue-cli probe 2>&1 | grep NTS | grep "out " | sed "s/:.*//" | sed "s/out //" | sed 's/^ *//g'`
logue-cli load -i $INPORT -o $OUTPORT -s 0 -u mod*.ntkdigunit
```

## Effects

### simplepan (modfx)

[simplepan](https://github.com/schollz/logue/tree/master/simplepan) is a simple panning mod effect which allows you to control the depth and rate of a left/right pan.

### pitchshift (delfx)

[pitchshift](https://github.com/schollz/logue/tree/master/pitchshift) is a [fork from @centrevillage](https://github.com/centrevillage/cv_logue), which adds a delay pitch shifting effect. It is licensed under MIT (c) 2019 centrevillage.

### tremelo (modfx)

A nice tremelo from [@ttaschke](https://github.com/ttaschke/tremoxd), licensed under BSD-3 (c) 2020, Tim.

### tldy (delfx)

This is an awesome time delay [from @techno-cat](https://github.com/techno-cat/logue-user-delfx-tdly), licensed under MIT (c) 2019 Tomoaki Itoh.

Note for some reason [Ubuntu can't load this normally](https://github.com/korginc/logue-sdk/issues/37#issuecomment-627982850) to upload you can use

	make -i tldy

which will upload it into slot 1.
