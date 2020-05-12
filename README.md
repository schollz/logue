# logue

Some of these are my creations, and some are compiled from [the Unit index](https://korginc.github.io/logue-sdk/unit-index/) and streamlines for a single workflow.

## Pre-requisites

Clone this repository, and then install the [Korg's logue SDK](https://github.com/korginc/logue-sdk):

```bash
git clone https://github.com/schollz/logue
cd logue 
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


### simplepan

[simplepan](https://github.com/schollz/logue/tree/master/simplepan) is a simple panning mod effect which allows you to control the depth and rate of a left/right pan.

### pitchshift

[pitchshift](https://github.com/schollz/logue/tree/master/pitchshift) is a [fork from @centrevillage](https://github.com/centrevillage/cv_logue), which adds a delay pitch shifting effect.

### tremelo

A nice tremelo from [@ttaschke](github.com/ttaschke/tremoxd), licensed under BSD-3.